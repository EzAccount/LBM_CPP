//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H
#include "constants.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>
#include <numeric>
#include <vector>

/***
 * The class remembers the macro parameters of each point taken from the input
 * data. Methods contribute to the recalculation of these parameters and the
 * distribution.
 */
class Point {
public:
  std::array<double, Q> f, f_temp, f_eq;
  double k_rel, T, rho, P, tau;
  /**<
   * f - distribution function.
   * f_temp - distribution function after one step.
   * f_eq - equilibrium distribution function.
   * T - temperature at point.
   * rho - density at point.
   * P - pressure at point.
   * tau - relaxation time.
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
  void boundaries();
  void open_boundaries();
  void transfer(int, int);
  void weight_calculate();
  std::array<double, 5> macro_at(size_t, size_t); // TODO: result output
  std::vector<std::vector<Point>> grid;
};

double Point::tau_calculate(int size) {
  double k = sqrt(3.141592653 / 6) * rho / Kn / size;
  double tau = 1 / k + 0.5;
  return tau;
}

double Point::k_rel_calculate(int size) {
  double k = sqrt(3.141592653 / 6) * rho / Kn / size;
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
  double c = T;
  for (size_t k = 0; k < Q; ++k) {
    double sc = e[k] * v;
    double q = sc / c / c;
    f_eq[k] = rho * w[k] * (1. + 3 * q + 4.5 * q * q - v * v * 1.5 / c / c);
  }
}

/***
 * Open bound for Poiseuille flow
 */
void Grid::open_boundaries() {
  for (int j = 0; j < grid[0].size(); ++j) {
    grid[0][j].rho = 1.1;
    grid[0][j].T = 1.;
    grid[0][j].v = Vector2D<double>{0, 0};
    grid[0][j].eq();

    if (grid[0][j].interior) {
      grid[0][j].f_temp = grid[0][j].f_eq;
    } else {
      for (size_t k = 0; k < Q; k++) {
        if (grid[0][j].w_for_bound_point[k] > 0) {
          grid[0][j].f_temp[k] = grid[0][j].f_eq[k];
        } else {
          grid[0][j].f_temp[k] = 0.;
        }
      }
    }
  }
  for (int j = 0; j < grid[grid.size() - 1].size(); ++j) {
    grid[grid.size() - 1][j].rho = 1.;
    grid[grid.size() - 1][j].T = 1.;
    grid[grid.size() - 1][j].v = Vector2D<double>{0, 0};
    grid[grid.size() - 1][j].eq();

    if (grid[grid.size() - 1][j].interior) {
      grid[grid.size() - 1][j].f_temp = grid[grid.size() - 1][j].f_eq;
    } else {
      for (size_t k = 0; k < Q; k++) {
        if (grid[grid.size() - 1][j].w_for_bound_point[k] > 0) {
          grid[grid.size() - 1][j].f_temp[k] = grid[grid.size() - 1][j].f_eq[k];
        } else {
          grid[grid.size() - 1][j].f_temp[k] = 0.;
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
      v += f_temp[k] / rho * e[k];
    }
    T = 0;
    for (size_t k = 0; k < Q; ++k) {
      T += (e[k] - v) * (e[k] - v) * f_temp[k] / rho;
    }
  }
  T *= 1.5;
}

/***
 * Assignment of input macro parameters, recalculation of f and f_temp via
 * distribution.
 * @param rho1 - Input rho
 * @param T1 - Input T
 * @param v1 - Input v
 * @param P1 - Input P
 */
Point::Point(double rho1, double T1, Vector2D<double> v1,
             double P1) { // TODO: initialization list
  rho = rho1;
  T = T1;
  v = v1;
  P = P1;
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
  int x_max, y_max, x_min, y_min; // TODO: should those be size_t?
  auto result = std::minmax_element(
      input_data.begin(), input_data.end(),
      [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first < b.first;
      });
  x_min = input_data[result.first - input_data.begin()].first;
  x_max = input_data[result.second - input_data.begin()].first;
  for (auto k : input_data) {
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
  for (auto k : input_data) {
    k.second -= y_min;
  }
  y_max -= y_min;
  int flag = 0;
  grid.resize(x_max + 1);
  for (auto &grid_row : grid)
    grid_row.resize(y_max + 1);
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      for (auto &input_cell : input_data) {
        if (i == input_cell.first && j == input_cell.second) {
          grid[i][j].interior = true;
          flag = 1;
          break;
        }
      }
      if (flag == 0) {
        grid[i][j].interior = false;
      } else
        flag = 0;
    }
  }
}

/***
 * Moving the points along the nine directions.
 * If point is aimed at a boundary point on some direction, then put
 * distribution function in f_temp of boundary point for all
 * directions, multiplied by its weight. If the weight in any direction is zero,
 * then f_temp is also.
 * @param x - first coordinate
 * @param y - second coordinate
 */
void Grid::transfer(int x, int y) {
  for (int k = 0; k < Q; ++k) {
    if (grid[x][y].f[k] > 0) {
      int x_offset = x + e[k].x, y_offset = y + e[k].y;
      bool offset_in_bounds = x_offset >= 0 && y_offset >= 0 &&
                              x_offset < grid.size() &&
                              y_offset < grid[x_offset].size();
      bool bound_point = offset_in_bounds &&
                         grid[x_offset][y_offset].w_for_bound_point.size() > 0;
      if (bound_point) {
        if (grid[x_offset][y_offset].bound) {
          for (int direction = 0; direction < Q; direction++) {
            double weight =
                grid[x_offset][y_offset].w_for_bound_point[direction];
            if (weight != 0) {
              grid[x_offset][y_offset].f_temp[direction] +=
                  grid[x][y].f[k] * weight;
            }
          }
        } else { // simple move
          grid[x_offset][y_offset].f_temp[k] += grid[x][y].f[k];
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
void Grid::boundaries() {
  int count = 0;
  for (size_t i = 0; i < grid.size(); ++i) {
    for (size_t j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j].interior) {
        for (int a = -1; a <= 1; ++a) {
          for (int b = -1; b <= 1; ++b) {
            if (i + a < grid.size() && j + b < grid[i].size()) {
              count += grid[i + a][j + b].interior;
            }
          }
        }
        grid[i][j].bound = count < 9;
        count = 0;
      } else
        grid[i][j].bound = false;
    }
  }
  for (auto &row : grid) {
    for (auto &item : row) {
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
        grid[i][j].w_for_bound_point = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int a = -1; a <= 1; ++a) {
          for (int b = -1; b <= 1; ++b) {
            bool offset = i + a >= 0 && j + b >= 0 && i + a < grid.size() &&
                          j + b < grid[i + a].size();
            if (offset) {
              size_t direction_to_change = 0;
              for (size_t q = 0; q < Q; ++q) {
                if (e[q].x == a && e[q].y == b) {
                  direction_to_change = q;
                  break;
                }
              }
              grid[i][j].w_for_bound_point[direction_to_change] =
                  w[direction_to_change];
            }
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
std::array<double, 5> Grid::macro_at(size_t, size_t) {
  return std::array<double, 5>();
}

#endif // LBM_CPP_POINT_H
