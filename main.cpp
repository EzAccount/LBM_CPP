#include "point.h"
#include <iostream>
#include <fstream>


int main() {
  std::vector<std::pair<int, int>> input_data;
  for (size_t i = 0; i <= 100; ++i) {
    for (size_t j = 0; j <= 10; ++j) {
      input_data.emplace_back(i, j);
    }
  }
  Grid Pois(input_data);
  Pois.boundaries();
  for (int j = 0; j <= 10; ++j) {
    for (int i = 0; i <= 100; ++i) {
      Pois.grid[i][j].rho = 1.;
      Pois.grid[i][j].T = 1.;
      Pois.grid[i][j].v = {0, 0};
      Pois.grid[i][j].eq();
    }
    Pois.grid[100][j].rho = 1.2;
    Pois.grid[100][j].eq();
  }
  for (int i = 0; i <= 100; ++i) {
    for (int j = 0; j <= 10; ++j) {
      for (size_t k = 0; k < Q; ++k) {
        Pois.grid[i][j].f[k] = Pois.grid[i][j].f_eq[k];
        Pois.grid[i][j].f_temp[k] = Pois.grid[i][j].f_eq[k];
      }
    }
  }
  for (size_t t = 0; t < iterations; ++t) {
    for (int i = 0; i <= 100; ++i) {
      for (int j = 0; j <= 10; ++j) {
        Pois.transfer(i, j);
        Pois.grid[i][j].col();
        Pois.grid[i][j].macro();
        Pois.grid[i][j].eq();
      }
    }
  }
  std::ofstream out;
  out.open("results.dat");
  if (out.is_open()) {
    for (int i = 0; i <= 100; ++i) {
      for (int j = 0; j <= 10; ++j) {
        out << i << " " << j << " " << Pois.grid[i][j].rho << std::endl;
      }
    }
  }
    /*
    for (int k = 0; k <= 9; ++k) {
      out1 << 49 << " " << 5 << " " << Pois.grid[49][5].f[k] << std::endl;
    }*/
  out.close();
  return 0;
}
