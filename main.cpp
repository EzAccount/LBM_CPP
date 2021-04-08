#include "point.h"
#include "utils.h"
#include <fstream>
#include <iostream>

int main() {
  int x_size = 100, y_size = 10;
  std::vector<std::pair<int, int>> input_data;
  for (size_t i = 0; i <= x_size; ++i) {
    for (size_t j = 0; j <= y_size; ++j) {
      input_data.emplace_back(i, j);
    }
  }
  Grid Pois(input_data);
  Pois.boundaries();
  for (int j = 0; j <= y_size; ++j) {
    for (int i = 0; i <= x_size; ++i) {
      Pois.grid[i][j].rho = 1.;
      Pois.grid[i][j].T = 1.;
      Pois.grid[i][j].v = Vector2D<double>{0, 0};
      Pois.grid[i][j].eq();
    }
    Pois.grid[100][j].rho = 1;
    Pois.grid[100][j].eq();
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
    Pois.open_bound();
    for (int i = 0; i <= x_size; ++i) {
      for (int j = 0; j <= y_size; ++j) {
        if (Pois.grid[i][j].interior) {
          Pois.grid[i][j].macro();
          Pois.grid[i][j].eq();
          Pois.grid[i][j].col();
        }
        if (Pois.grid[i][j].bound) {
          Pois.grid[i][j].col_for_bound();
        }
        Pois.grid[i][j].zeroing_temp();
      }
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
        if (Pois.grid[i][j].bound) {
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
