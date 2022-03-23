#include "Paraview.h"
#include <fstream>
#include <iostream>
#include <cmath>

size_t x_size = 300, y_size = 30;

struct physical_values {
    physical_values() = default;

    physical_values(double mass_, double energy_)
    : mass(mass_)
    , energy(energy_)
    {}

    double mass, energy;
};

physical_values find_physical_values(const Grid &g) {
    double mass = 0.;
    for (size_t i = 0; i < g.grid.size(); ++i) {
        for (size_t j = 0; j < g.grid[i].size(); ++j) {
            mass += g.grid[i][j].rho;
        }
    }

    double energy = 0.;
    for (const std::vector<Point> &row : g.grid) {
        for (const Point &point : row) {
            energy += point.rho * (point.v.x + point.v.y) * (point.v.x + point.v.y) / 2;
        }
    }

    return physical_values(mass, energy);

}

void check_conservation_laws(const Grid &g, physical_values &pv, bool print = true) {
    physical_values pv_ = find_physical_values(g);
    if (print) {
        const auto print_difference = [](const double &val_, const double &val) {
            (val_ > val) ? std::cout << "increasing from " << val << " to " << val_ :
            (val_ == val ? std::cout << "stable" : std::cout << "decreasing from " << val << " to " << val_);
        };
        std::cout << "mass is ";
        print_difference(pv_.mass, pv.mass);
        std::cout << ", ";
        std::cout << "energy is ";
        print_difference(pv_.energy, pv.energy);
        std::cout << '\n';
    }
    pv = pv_;
}

void print_mass_flow(const Grid &g) {
    double mass_flow = 0;
    for (size_t j = 1; j < y_size; ++j) {
        mass_flow += g.grid[x_size / 2][j].rho * g.grid[x_size / 2.][j].v.x;
    }
    std::cout << "Q: " << mass_flow << std::endl;
}

void initialize_pois_flow_open_boundaries(Grid &Pois) {
    for (size_t i = 0; i < max_component_of_grid_vectors(); ++i) {
        for (size_t j = 0; j < Pois.grid[i].size(); ++j) {
            Pois.grid[i][j].open_bound = true;
            // Pois.grid[i][j].interior = false;
            Pois.grid[i][j].bound = false;
        }
    }

    for (size_t i = 0; i < max_component_of_grid_vectors(); ++i) {
        for (size_t j = 0; j < Pois.grid[i].size(); ++j) {
            Pois.grid[Pois.grid.size() - i - 1][j].open_bound = true;
            // Pois.grid[Pois.grid.size() - i - 1][j].interior = false;
            Pois.grid[Pois.grid.size() - i - 1][j].bound = false;
        }
    }

    for (size_t i = 1; i < Pois.grid.size() - 1; ++i) {
        for (size_t j = 0; j < max_component_of_grid_vectors(); ++j) {
            Pois.grid[i][j].bound = true;
            Pois.grid[i][Pois.grid[i].size() - j - 1].bound = true;
            Pois.grid[i][j].interior = false;
            Pois.grid[i][Pois.grid[i].size() - j - 1].interior = false;
        }
    }
}

void initialize_pois_flow_macro(Grid &Pois) {
    for (size_t j = 0; j <= y_size; ++j) {
        for (size_t i = 0; i <= x_size; ++i) {
            Pois.grid[i][j].rho = 1.;
            Pois.grid[i][j].T = 1.;
            Pois.grid[i][j].v = Vector2D<double>{0, 0};
            Pois.grid[i][j].eq();
        }
        Pois.grid[Pois.grid.size() - 1][j].rho = 1.; // TODO: should i use it?
        Pois.grid[Pois.grid.size() - 1][j].eq();
    }
    for (size_t i = 0; i <= x_size; ++i) {
        for (size_t j = 0; j <= y_size; ++j) {
            if (Pois.grid[i][j].interior) {
                Pois.grid[i][j].f = Pois.grid[i][j].f_eq;
            } else {
                for (size_t k = 0; k < Q; k++) {
                    if (Pois.grid[i][j].w_for_bound_point[k] > 0) {
                        Pois.grid[i][j].f[k] = Pois.grid[i][j].f_eq[k];
                    } else {
                        Pois.grid[i][j].f[k] = 0.;
                    }
                }
            }
            for (size_t k = 0; k < Q; k++) {
                Pois.grid[i][j].f_temp[k] = 0.;
            }
        }
    }
}

void start_transfer_collision_loop(Grid &g) {
    physical_values pv;
    for (size_t t = 0; t < iterations; ++t) {
        if (t % 100 == 0) {
            ExportToVtk(g.grid, x_size + 1, y_size + 1, t);
        }
        g.transfer();
        g.maintain_open_boundaries();
        for (size_t i = 0; i <= x_size; ++i) {
            for (size_t j = 0; j <= y_size; ++j) {
                if (g.grid[i][j].interior) {
                    g.grid[i][j].macro();
                    g.grid[i][j].eq();
                    g.grid[i][j].k_rel = g.grid[i][j].k_rel_calculate(y_size);
                    g.grid[i][j].tau = g.grid[i][j].tau_calculate(y_size);
                    g.grid[i][j].col();
                }
                if (g.grid[i][j].bound || g.grid[i][j].open_bound) {
                    g.grid[i][j].col_for_bound();
                }
                g.grid[i][j].zeroing_temp();
            }
        }
        if (t % 1000 == 0) {
            check_conservation_laws(g, pv, bool(t));
            print_mass_flow(g);
        }
    }
}

void collect_data_from_grid(Grid &g) {
    std::ofstream out;
    out.open("results.dat");
    if (out.is_open()) {
        for (size_t i = 0; i <= x_size; ++i) {
            for (size_t j = 0; j <= y_size; ++j) {
                out << i << " " << j << " " << g.grid[i][j].f << " "
                    << g.grid[i][j].rho << " " << g.grid[i][j].T << " "
                    << g.grid[i][j].v.x << " " << g.grid[i][j].v.y << std::endl;
            }
        }
    }
    out.close();
    std::ofstream out1;
    out1.open("results1.dat");
    if (out1.is_open()) {
        for (size_t i = 0; i <= x_size; ++i) {
            for (size_t j = 0; j <= y_size; ++j) {
                if (g.grid[i][j].open_bound) {
                    out1 << i << " " << j << " " << g.grid[i][j].w_for_bound_point << std::endl;
                }
            }
        }
    }
    out1.close();
}

int main() {
    std::vector<std::pair<int, int>> input_data;
    for (size_t i = 0; i <= x_size; ++i) {
        for (size_t j = 0; j <= y_size; ++j) {
            input_data.emplace_back(i, j);
        }
    }

    Grid Pois(input_data);
    Pois.find_boundaries();
    initialize_pois_flow_open_boundaries(Pois);
    Pois.weight_calculate();
    initialize_pois_flow_macro(Pois);

    start_transfer_collision_loop(Pois);
    collect_data_from_grid(Pois);

    return 0;
}
