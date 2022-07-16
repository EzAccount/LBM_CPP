//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H

#include "constants.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>

size_t max_component_of_grid_vectors() {
    const auto it = std::max_element(e().begin(), e().end(), [](const auto &l, const auto &r){
        return l.x < r.x;
    });
    return e()[it - e().begin()].x;
}

/***
 * The class remembers the macro parameters of each point taken from the input
 * data. Methods contribute to the recalculation of these parameters and the
 * distribution.
 */
class Point {
public:
    std::array<double, Q> f, f_temp, f_eq;
    double k_rel, rho, T, P, tau;
    /***
     * f - distribution function.
     * f_temp - distribution function after one step.
     * f_eq - equilibrium distribution function.
     * T - temperature at point.
     * rho - density at point.
     * P - pressure at point.
     * tau - relaxation time.
     * interior points - points inside our vessel except boundary points
     */
    bool interior = false;
    bool bound = false;
    bool open_bound = false;
    Vector2D<double> v;

    void eq();

    void col();

    void macro();

    void zeroing_temp();

    void col_for_bound();

    double k_rel_calculate(int);

    double tau_calculate(int size);

    std::vector<double> w_for_bound_point;

    explicit Point(double = 0., double = 0.,
                   Vector2D<double> = Vector2D<double>(0., 0.), double = 0.);
};

/***
 * The main role of this class is to create a grid with right angles
 * in each node of which there is an input data point.
 * Subsequently, move the points and record the change of macro parameters.
 */
class Grid {
public:
    explicit Grid(std::vector<std::pair<int, int>>);

    void find_boundaries();

    void maintain_open_boundaries();

    void transfer();

    void weight_calculate();

    bool on_grid(int i, int j) const;

    std::vector<std::vector<Point>> grid;
};

double Point::tau_calculate(int size) {
    double k = sqrt(M_PI / 6) * rho / Kn / size;
    return 1 / k + 0.5;
}

double Point::k_rel_calculate(int size) {
    double k = sqrt(M_PI / 6) * rho / Kn / size;
    if (k > 1) {
        k = 1;
    }
    return k;
}

/***
 * Recalculation of f after collision
 */
void Point::col() {
    for (size_t k = 0; k < Q; ++k) {
        f[k] = f_temp[k] - k_rel * (f_temp[k] - f_eq[k]);
    }
}

/***
 * Recalculation of f after collision for bound points
 */
void Point::col_for_bound() {
    for (size_t k = 0; k < Q; ++k) {
        f[k] = f_temp[k];
    }
}

/***
 * Zeroing f_temp after transfer
 */
void Point::zeroing_temp() {
    for (size_t k = 0; k < Q; ++k) {
        f_temp[k] = 0;
    }
}

/***
 * Calculation of distribution in all directions.
 */
void Point::eq() {
    double c = sqrt(T);
    for (size_t k = 0; k < Q; ++k) {
        double sc = e()[k] * v;
        double q = sc / c / c;
        f_eq[k] = rho * w()[k] * (1. + 3 * q + 4.5 * q * q - v * v * 1.5 / c / c + (T - 1) * (r() * r() * e()[k] * e()[k] - D) / 2);
    }
}

/***
 * Open bound for Poiseuille flow
 */
void Grid::maintain_open_boundaries() {
    for (size_t i = 0; i < max_component_of_grid_vectors(); ++i) {
        for (size_t j = 0; j < grid[0].size(); ++j) {
            grid[i][j].rho = 1.1;
            grid[i][j].T = 1.;
            grid[i][j].v = Vector2D<double>{0, 0};
            grid[i][j].eq();

            if (grid[i][j].interior) {
                grid[i][j].f_temp = grid[i][j].f_eq;
            } else {
                for (size_t k = 0; k < Q; k++) {
                    if (grid[i][j].w_for_bound_point[k] > 0) {
                        grid[i][j].f_temp[k] = grid[i][j].f_eq[k];
                    } else {
                        grid[i][j].f_temp[k] = 0.;
                    }
                }
            }
        }

        for (size_t j = 0; j < grid[grid.size() - i - 1].size(); ++j) {
            grid[grid.size() - i - 1][j].rho = 1.;
            grid[grid.size() - i - 1][j].T = 1.;
            grid[grid.size() - i - 1][j].v = Vector2D<double>{0, 0};
            grid[grid.size() - i - 1][j].eq();

            if (grid[grid.size() - i - 1][j].interior) {
                grid[grid.size() - i - 1][j].f_temp = grid[grid.size() - i - 1][j].f_eq;
            } else {
                for (size_t k = 0; k < Q; k++) {
                    if (grid[grid.size() - i - 1][j].w_for_bound_point[k] > 0) {
                        grid[grid.size() - i - 1][j].f_temp[k] = grid[grid.size() - i - 1][j].f_eq[k];
                    } else {
                        grid[grid.size() - i - 1][j].f_temp[k] = 0.;
                    }
                }
            }
        }
    }
}

/***
 * Recalculation of macro parameters (rho, T, v) after move.
 */
void Point::macro() {
    rho = std::accumulate(f_temp.begin(), f_temp.end(), 0.);
    v.x = 0;
    v.y = 0;
    if (rho != 0) {
        for (size_t k = 0; k < Q; ++k) {
            v += f_temp[k] / rho * e()[k];
        }
        T = 0;
        for (size_t k = 0; k < Q; ++k) {
            T += (e()[k] - v) * (e()[k] - v) * f_temp[k] / rho;
        }
    }
    T *= r() * r() / D;
}

/***
 * Assignment of input macro parameters, recalculation of f and f_temp via
 * distribution.
 * @param rho1 - Input rho
 * @param T1 - Input T
 * @param v1 - Input v
 * @param P1 - Input P
 */
Point::Point(double rho_, double T_, Vector2D<double> v_, double P_)
        : rho(rho_), T(T_), P(P_), v(v_) {
    k_rel = 1;
    eq();
    for (size_t k = 0; k < Q; ++k) {
        f[k] = f_eq[k];
        f_temp[k] = f_eq[k];
    }
}

/***
 * Inscribe the grid into a minimal rectangle.
 * Mark points of the rectangle inside and outside the grid.
 * @param input_data
 */
Grid::Grid(std::vector<std::pair<int, int>> input_data) {
    int x_max, y_max, x_min, y_min;
    auto result = std::minmax_element(
            input_data.begin(), input_data.end(),
            [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
                return a.first < b.first;
            });
    x_min = input_data[result.first - input_data.begin()].first;
    x_max = input_data[result.second - input_data.begin()].first;
    for (auto k: input_data) {
        k.first -= x_min;
    }
    x_max -= x_min;
    result = std::minmax_element(
            input_data.begin(), input_data.end(),
            [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
                return a.second < b.second;
            });
    y_min = input_data[result.first - input_data.begin()].second;
    y_max = input_data[result.second - input_data.begin()].second;
    for (auto k: input_data) {
        k.second -= y_min;
    }
    y_max -= y_min;
    grid.resize(x_max + 1);
    for (auto &grid_row: grid)
        grid_row.resize(y_max + 1);
    bool was_interior = false;
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            for (const auto &input_cell: input_data) {
                if (i == input_cell.first && j == input_cell.second) {
                    grid[i][j].interior = true;
                    was_interior = true;
                    break;
                }
            }
            if (!was_interior) {
                grid[i][j].interior = false;
            } else
                was_interior = false;
        }
    }
}

/***
 * Decides whether the point [i, j] lies on the grid
 * @param i - first coordinate
 * @param j - second coordinate
 * @return true, if point lies on the grid, false otherwise
 */
bool Grid::on_grid(int i, int j) const {
    return !grid.empty() && i >= 0 && j >= 0 && i < grid.size() && j < grid[0].size();
}

/***
 * Moving the points along the Q directions.
 * If point is aimed at a boundary point on some direction, then put
 * distribution function in f_temp of boundary point for all
 * directions, multiplied by its weight. If the weight in any direction is zero,
 * then f_temp is also.
 * @param x - first coordinate
 * @param y - second coordinate
 */
void Grid::transfer() {
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid[x].size(); ++y) {
            for (size_t k = 0; k < Q; ++k) {
                if (grid[x][y].f[k] > 0) {
                    /*int distance = std::max(std::abs(e()[k].x), std::abs(e()[k].y));
                    int x_offset = x, y_offset = y;
                    for (int i = 1; i <= distance; ++i) {
                         x_offset = x + e()[k].x * i / distance;
                         y_offset = y + e()[k].y * i / distance;
                         // std::cout << x_offset << " " << y_offset << std::endl;
                         if (on_grid(x_offset, y_offset) && grid[x_offset][y_offset].bound) break;
                    }*/
                    int x_offset = x + e()[k].x, y_offset = y + e()[k].y;
                    if (on_grid(x_offset, y_offset)) {
                        if (grid[x_offset][y_offset].bound) {
                            for (size_t direction = 0; direction < Q; direction++) {
                                double weight =
                                        grid[x_offset][y_offset].w_for_bound_point[direction];
                                grid[x_offset][y_offset].f_temp[direction] +=
                                        grid[x][y].f[k] * weight;
                            }
                        } else { // simple move
                            grid[x_offset][y_offset].f_temp[k] += grid[x][y].f[k];
                        }
                    }
                }
            }
        }
    }
}

/***
 * Determines if the point belongs to grid, by checking the existence of all
 * neighbors. If the point is a boundary, then set it to be non-interior and
 * boundary. The next step is to find out which directions are aimed at the gas
 * for boundary points. For these directions we recalculate the weights of the
 * distribution function.
 */
void Grid::find_boundaries() {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j].interior) {
                const size_t count =
                        std::count_if(e().begin(), e().begin() + 9, [&](const auto &direction) {
                            int direction_x = direction.x, direction_y = direction.y, i_ = i, j_ = j;
                            return on_grid(i_ + direction_x, j_ + direction_y)
                                   && grid[i_ + direction_x][j_ + direction_y].interior;
                        });
                grid[i][j].bound = count < 9;
            } else grid[i][j].bound = false;
        }
    }
    for (auto &row: grid) {
        for (auto &item: row) {
            if (item.bound) {
                item.interior = false;
            }
        }
    }
}

/***
 * Weight calculation for each solid boundary point.
 */
void Grid::weight_calculate() {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j].bound || grid[i][j].open_bound) {
                grid[i][j].w_for_bound_point = std::vector<double>(Q, 0.);
                for (const auto &direction: e()) {
                    int direction_x = direction.x, direction_y = direction.y, i_ = i, j_ = j;
                    const auto direction_to_change =
                            std::find_if(e().begin(), e().end(), [&](const auto &direction_) {
                                return on_grid(i_ + direction_x, j_ + direction_y) && direction_ == direction;
                            });
                    if (direction_to_change != e().end()) {
                        size_t direction_to_change_idx = direction_to_change - e().begin();
                        grid[i][j].w_for_bound_point[direction_to_change_idx] =
                                w()[direction_to_change_idx];
                    }
                }
                if (grid[i][j].bound) {
                    double normalization_factor =
                            std::accumulate(grid[i][j].w_for_bound_point.begin(),
                                            grid[i][j].w_for_bound_point.end(), 0.);
                    std::transform(grid[i][j].w_for_bound_point.begin(),
                                   grid[i][j].w_for_bound_point.end(),
                                   grid[i][j].w_for_bound_point.begin(),
                                   [normalization_factor](double &direction) {
                                       return direction / normalization_factor;
                                   });
                } else {
                    grid[i][j].w_for_bound_point[0] = 0;
                }
            }
        }
    }
}

#endif // LBM_CPP_POINT_H
