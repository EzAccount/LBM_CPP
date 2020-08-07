//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H
#include "constants.h"
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
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
public:
  Grid(std::vector<std::pair<int, int>>);
  void boundaries();
  void eval();
  void transfer(int, int);
  std::array<double, 5> macro_at(size_t, size_t); // TODO: result output
private:
  std::vector<std::vector<Point>> grid;
  FRIEND_TEST(GridConstructor__Test, MAX_MIN_Test);
  FRIEND_TEST(GridBoundaries__Test, Boundaries_Test);
  FRIEND_TEST(GridTransfer__Test, Transfer_Test);
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
    double q = sc / c;
    f_eq[k] = rho * w[k] *
              (1. + 3 * q + 4.5 * q * q - v * v * 1.5 / c / c);
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
  for (auto k : indata) {
    k.first -= xmin;
  }
  xmax -= xmin;
  result = std::minmax_element(
      indata.begin(), indata.end(),
      [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second < b.second;
      });
  ymin = indata[result.first - indata.begin()].second;
  ymax = indata[result.second - indata.begin()].second;
  for (auto k : indata) {
    k.second -= ymin;
  }
  ymax -= ymin;
  int flag = 0;
  grid.resize(xmax + 1);
  for (int i = 0; i < grid.size(); ++i)
    grid[i].resize(ymax + 1);
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
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
    int k_temp = 0;
    int x_cord = e[k].x, y_cord = e[k].y;
    int xOffset = x + e[k].x, yOffset = y + e[k].y;
    bool flag =
        false; // this flag is responsible for changing the direction of move
    if (grid[x][y].exist) {
      if (grid[xOffset][yOffset].bound) {
        if (grid[xOffset][y].bound) {
          x_cord = -e[k].x;
          flag = true;
        }
        if (grid[x][yOffset].bound) {
          y_cord = -e[k].y;
          flag = true;
        }
        if (!flag) {
          x_cord = -e[k].x;
          y_cord = -e[k].y;
          flag = true;
        }
      }
      for (size_t j = 0; j < Q; ++j) {
        if (e[j].x == x_cord && e[j].y == y_cord) {
          k_temp = j;
          break;
        }
      }
      if (flag) {
        grid[xOffset + e[k_temp].x][yOffset + e[k_temp].y].f_temp[k_temp] =
            alpha * grid[x][y].f[k] +
            (1. - alpha) *
                (grid[xOffset + e[k_temp].x][yOffset + e[k_temp].y]
                     .f_eq[k_temp] -
                 grid[x][y].f_eq[k_temp]) /
                2;
      } else {
        grid[xOffset][yOffset].f_temp[k] =
            alpha * grid[x][y].f[k] +
            (1. - alpha) *
                (grid[xOffset][yOffset].f_eq[k] - grid[x][y].f_eq[k]) / 2;
      }
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
        for (int a = -1; a <= 1; ++a) {
          for (int b = -1; b <= 1; ++b) {
            if (i + a < grid.size() && j + b < grid[i].size() && i + a >= 0 &&
                i + b >= 0) {
              count += grid[i + a][j + b].exist;
              /*counter in all directions. if at least in 1 direction
              point does not belong to the grid, then the starting point i j
              at the boundary*/
            }
          }
        }
        grid[i][j].bound = count < 9;
        count = 0;
      } else
        grid[i][j].bound = 0;
    }
  }
}

#endif // LBM_CPP_POINT_H
