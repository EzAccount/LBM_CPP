//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H
#include "constants.h"
#include <algorithm>
#include <array>
#include <iostream>
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
  double tau, T, rho, P;
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
  Vector2D<double> v;
  void eq();
  void col();
  void macro();
  explicit Point(double = 0., double = 0., Vector2D<double> = Vector2D<double>(0.,0.), double = 0.);
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
  void transfer(int, int);
  std::array<double, 5> macro_at(size_t, size_t); // TODO: result output
  std::vector<std::vector<Point>> grid;
};

/***
 * Recalculation of f after collision
 */
void Point::col() {
  for (size_t k = 0; k < Q; ++k) {
    f[k] = f_temp[k] - 1. / tau * (f_temp[k] - f_eq[k]) ;
  }
}

/***
 * Calculation of distribution in all directions.
 */
void Point::eq() {
  double c = T;
  for (size_t k = 0; k < Q; ++k) {
    double sc = e[k] * v;
    double q = sc / c;
    f_eq[k] = rho * w[k] * (1. + 3 * q + 4.5 * q * q - v * v * 1.5 / c / c);
  }
}

/***
 * Recalculation of macro parameters (rho, T, v) after move.
 */
void Point::macro() {
  rho = 0;
  rho = std::accumulate(f_temp.begin(), f_temp.end(), 0.);

  T = 0;
  for (size_t k = 0; k < Q; ++k) {
    T += (e[k] - v) * (e[k] - v) * f_temp[k];
  }
  T *= 0.5;
  v.x = 0;
  v.y = 0;
  for (size_t k = 0; k < Q; ++k) {
    v += f_temp[k] / rho * e[k];
  }
}

/***
 * Assignment of input macro parameters, recalculation of f and f_temp via distribution.
 * @param rho1 - Input rho
 * @param T1 - Input T
 * @param v1 - Input v
 * @param P1 - Input P
 */
Point::Point(double rho1, double T1, Vector2D<double> v1, double P1) { // TODO: initialization list
  rho = rho1;
  T = T1;
  v = v1;
  P = P1;
  tau = 1;
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
  int xmax, ymax, xmin, ymin; //TODO: should those be size_t?
  auto result = std::minmax_element(
      input_data.begin(), input_data.end(),
      [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.first < b.first;
      });
  xmin = input_data[result.first - input_data.begin()].first;
  xmax = input_data[result.second - input_data.begin()].first;
  for (auto k : input_data) {
    k.first -= xmin;
  }
  xmax -= xmin;
  result = std::minmax_element(
      input_data.begin(), input_data.end(),
      [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second < b.second;
      });
  ymin = input_data[result.first - input_data.begin()].second;
  ymax = input_data[result.second - input_data.begin()].second;
  for (auto k : input_data) {
    k.second -= ymin;
  }
  ymax -= ymin;
  int flag = 0;
  grid.resize(xmax + 1);
  for (auto &grid_row: grid)
    grid_row.resize(ymax + 1);
  for (size_t i = 0; i < grid.size(); ++i) {
    for (size_t j = 0; j < grid[i].size(); ++j) {
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
 * If by one component of the direction taken at a given step the point moves to the boundary,
 * and by the other remains inside, then changing only one coordinate. Otherwise both.
 * Depending on this there are four outcomes. Move back, reflection along each axis and simple move.
 * @param x - first coordinate
 * @param y - second coordinate
 */
void Grid::transfer(int x, int y) {
  for (size_t k = 0; k < Q; ++k) {
    int k_temp = 0;
    //TODO why those are int but e is double?
    int x_cord = e[k].x, y_cord = e[k].y;
    int xOffset = x + e[k].x, yOffset = y + e[k].y;
    bool change_coordinate = false, move_belongs_to_boundary = false;
    if (grid[x][y].interior) {
      if (grid[x + e[k].x][y + e[k].y].bound) {
        move_belongs_to_boundary = true;
        if (grid[x + e[k].x][y].bound && grid[x][y + e[k].y].interior) {
          x_cord = -e[k].x;
          xOffset = x;
          change_coordinate = true;
        }
        if (grid[x][y + e[k].y].bound && grid[x + e[k].x][y].interior) {
          y_cord = -e[k].y;
          yOffset = y;
          change_coordinate = true;
        }
        if (!change_coordinate) {
          x_cord = -e[k].x;
          y_cord = -e[k].y;
          xOffset = x;
          yOffset = y;
        }
      }
      for (size_t j = 0; j < Q; ++j) {
        if (e[j].x == x_cord && e[j].y == y_cord) {
          k_temp = j;
          break;
        }
      }
      if (move_belongs_to_boundary) {                                              // push-off move
        if (e[k_temp].x == -e[k].x && e[k_temp].y == -e[k].y) { // going back
          grid[xOffset][yOffset].f_temp[k_temp] = (grid[x][y].f[k] + balance *
                  (grid[x + e[k].x][y + e[k].y].f_eq[k_temp])) / 2;
        } else {                        // displacement
          if (e[k].x == -e[k_temp].x) { // displacement by y
            grid[xOffset][yOffset].f_temp[k_temp] =
                grid[x][y].f[k] + balance *
                    (grid[x + e[k].x][y + e[k].y].f_eq[k_temp] +
                     grid[x + e[k].x][y].f_eq[k_temp]) / 2;
          } else { // displacement by x
            grid[xOffset][yOffset].f_temp[k_temp] =
                grid[x][y].f[k] + balance *
                    (grid[x + e[k].x][y + e[k].y].f_eq[k_temp] +
                     grid[x][y + e[k].y].f_eq[k_temp]) / 2;
          }
        }
      } else { // simple move
        grid[xOffset][yOffset].f_temp[k] = grid[x][y].f[k];

    }
  }
  }
}

/***
 * Determines if the point belongs to grid, by checking the existence of all neighbors.
 * If the point is a boundary, then set it to be non-interior and boundary
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

std::array<double, 5> Grid::macro_at(size_t, size_t) {
  return std::array<double, 5>();
}

#endif // LBM_CPP_POINT_H
