#include "Paraview.h"
#include <fstream>
#include <iostream>
#include <math.h>

int main() {
  int x_size = 300, y_size = 30;
  std::vector<std::pair<int, int>> input_data;
  for (size_t i = 0; i <= x_size; ++i) {
    for (size_t j = 0; j <= y_size; ++j) {
      input_data.emplace_back(i, j);
    }
  }
  Grid Pois(input_data);
  Pois.boundaries();
  for (int j = 0; j < Pois.grid[0].size(); ++j) {
    Pois.grid[0][j].open_bound = true;
    Pois.grid[0][j].bound = false;
  }

  for (int j = 0; j < Pois.grid[0].size(); ++j) {
    Pois.grid[Pois.grid.size()-1][j].bound = false;
    Pois.grid[Pois.grid.size()-1][j].open_bound = true;
  }

  Pois.weight_calculate();
  for (int j = 0; j <= y_size; ++j) {
    for (int i = 0; i <= x_size; ++i) {
      Pois.grid[i][j].rho = 1.;
      Pois.grid[i][j].T = 1.;
      Pois.grid[i][j].v = Vector2D<double>{0, 0};
      Pois.grid[i][j].eq();
    }
    Pois.grid[Pois.grid.size()-1][j].rho = 1;
    Pois.grid[Pois.grid.size()-1][j].eq();
  }
  for (int i = 0; i <= x_size; ++i) {
    for (int j = 0; j <= y_size; ++j) {
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
        Pois.grid[i][j].f_temp[k] = 0;
      }
    }
  }
  for (size_t t = 0; t < iterations; ++t) {
    for (int i = 0; i <= x_size; ++i) {
      for (int j = 0; j <= y_size; ++j) {
        Pois.transfer(i, j);
      }
    }
    Pois.open_boundaries();
    for (int i = 0; i <= x_size; ++i) {
      for (int j = 0; j <= y_size; ++j) {
        if (Pois.grid[i][j].interior) {
          Pois.grid[i][j].macro();
          Pois.grid[i][j].eq();
          Pois.grid[i][j].k_rel = Pois.grid[i][j].k_rel_calculate(y_size);
          Pois.grid[i][j].tau = Pois.grid[i][j].tau_calculate(y_size);
          Pois.grid[i][j].col();
        }
        if (Pois.grid[i][j].bound || Pois.grid[i][j].open_bound) {
          Pois.grid[i][j].col_for_bound();
        }
        Pois.grid[i][j].zeroing_temp();
      }
    }
    if (t % 100 == 0) {
      ExportToVtk(Pois.grid, x_size + 1, y_size + 1, t);
    }
    if (t % 1000 == 0) {
      double mass_flow = 0;
      for (int j = 1; j < y_size; ++j) {
      mass_flow += Pois.grid[50][j].rho * Pois.grid[50][j].v.x;
      }
      std::cout << mass_flow << std::endl;
    }
  }

  std::ofstream out;
  out.open("results.dat");
  if (out.is_open()) {
    for (int i = 0; i <= x_size; ++i) {
      for (int j = 0; j <= y_size; ++j) {
        out << i << " " << j << " " << Pois.grid[i][j].f << " "
            << Pois.grid[i][j].rho << " " << Pois.grid[i][j].T << " "
            << Pois.grid[i][j].v.x << " " << Pois.grid[i][j].v.y << std::endl;
      }
    }
  }
  out.close();
  std::ofstream out1;
  out1.open("results1.dat");
  if (out1.is_open()) {
    for (int i = 0; i <= x_size; ++i) {
      for (int j = 0; j <= y_size; ++j) {
        if (Pois.grid[i][j].open_bound) {
          out1 << i << " " << j << " " << Pois.grid[i][j].w_for_bound_point[0]
               << " " << Pois.grid[i][j].w_for_bound_point[1] << " "
               << Pois.grid[i][j].w_for_bound_point[2] << " "
               << Pois.grid[i][j].w_for_bound_point[3] << " "
               << Pois.grid[i][j].w_for_bound_point[4] << " "
               << Pois.grid[i][j].w_for_bound_point[5] << " "
               << Pois.grid[i][j].w_for_bound_point[6] << " "
               << Pois.grid[i][j].w_for_bound_point[7] << " "
               << Pois.grid[i][j].w_for_bound_point[8];
          out1 << std::endl;
        }
      }
    }
  }
  out1.close();

  return 0;
}
