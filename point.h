//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H

#include "constants.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

class Point {
public:
  std::array<double, Q> f, f_temp, f_eq;
  double tau, T, rho, P;
  bool exist = false;
  bool bound = false;
  Vector<double> v;
  void eq();
  void col();
  void macro(); // TODO:check
  explicit Point(double = 0., double = 0., Vector<double> = 0., double = 0.);
};
class Grid {
  Grid(std::vector<std::pair<int, int>>);
  void boundaries();
  void transfer(int, int);
  void eval();
  std::array<double, 5> macro_at(size_t, size_t); // TODO: result output
private:
  std::vector<std::vector<Point>> grid;
};

void Point::col() {
  for (size_t k = 0; k < Q; ++k) {
    f[k] = f_temp[k] - 1. / tau * (f_temp[k] - f_eq[k]);
  }
}
void Point::eq() {
  double c = T;
  for (size_t k = 0; k < Q; ++k) {
    double sc = e[k] * v;
    f_eq[k] = rho * w[k] *
              (1. + 3 * sc / c + 4.5 * sc * sc / c / c - v * v * 1.5 / c / c);
  }
}
void Point::macro() {
  rho = 0;
  for (size_t k = 0; k < Q; ++k) {
    rho += f_temp[k];
  }
  T = 0;
  for (size_t k = 0; k < Q; ++k) {
    T += (e[k] - v) * (e[k] - v) * f_temp[k] * 3 / 2;
  }
  v.x = 0;
  v.y = 0;
  for (size_t k = 0; k < Q; ++k) {
    v += f_temp[k] / rho * e[k];
  }
}

Point::Point(double rho, double T, Vector<double> v, double P)
    : tau(0.), T(T), rho(rho), v(v), P(P) {
  eq();
  for (int k = 0; k < Q; ++k) {
    f[k] = f_eq[k];
    f_temp[k] = f_eq[k];
  }
}

Grid::Grid(std::vector<std::pair<int, int>> indata) {
  int xmax, ymax, xmin, ymin;
  auto result = std::minmax_element(
      indata.begin(), indata.end(),
      [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first < b.first;
      });
  xmin = indata[result.first - indata.begin()].first;
  xmax = indata[result.second - indata.begin()].first;
  result = std::minmax_element(
      indata.begin(), indata.end(),
      [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second < b.second;
      });
  ymin = indata[result.first - indata.begin()].second;
  ymax = indata[result.second - indata.begin()].second;
  int flag = 0;
  for (int i = 0; i < xmax - xmin; ++i) {
    for (int j = 0; j < ymax - ymin; ++j) {
      for (int k = 0; k < indata.size(); ++k) {
        if (i == indata[k].first && j == indata[k].second) {
          grid[i][j].exist = 1;
          flag = 1;
          break;
        }
      }
      if (flag == 0) {
        grid[i][j].exist = 0;
      } else
        flag = 0;
    }
  }
}

void Grid::transfer(int x, int y) {
  for (size_t k = 0; k < Q; ++k) {
    if (grid[x][y].bound) {
      if (grid[x + e[k].x][y].bound && !grid[x][y + e[k].y].bound) {
        e[k].y = -e[k].y;
      }
      if (!grid[x + e[k].x][y].bound && grid[x][y + e[k].y].bound) {
        e[k].x = -e[k].x;
      }
    }
    if (grid[x][y].exist) {
      grid[x + e[k].x][y + e[k].y].f_temp[k] = grid[x][y].f[k];
    }
  }
}
/***
 * this func will find points on the bound
 */
void Grid::boundaries() {
  int count = 0;
  for (size_t i = 0; i < grid.size(); ++i) {
    for (size_t j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j].exist) {
        if (i == 0 || j == 0 || i == grid.size() || j == grid[i].size()) {
          grid[i][j].bound = 1; // If point is on the boundary of the grid, then
                                // it is the boundary
        } else {
          for (int a = -1; a <= 1; ++a) {
            for (int b = -1; b <= 1; ++b) {
              count += grid[i + a][j + b].exist;
              /*counter in all directions. if at least in 1 direction
              point does not belong to the grid, then the starting point i j
              at the boundary*/
            }
          }
        }
        grid[i][j].bound = count < 9;
      } else
        grid[i][j].bound = 0;
    }
  }
}

#endif // LBM_CPP_POINT_H
