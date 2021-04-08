//
// Created by mikhail on 8/13/18.
//
#ifndef LBM_CPP_CONSTANTS_H
#define LBM_CPP_CONSTANTS_H

#include <array>
#include <iostream>
#include <vector>

#include "vector_2d.h"

/**<
 * Q - number of grid directions.
 * e - vector of all directions.
 * w - array containing the Maxwell distribution coefficients for each
 * direction.
 * balance - balancing factor for boundary conditions.
 */
constexpr std::size_t Q = 9;
std::array<Vector2D<double>, Q> e = {{Vector2D<double>(0, 0),
                                    Vector2D<double>(1, 0),
                                    Vector2D<double>(0, 1),
                                    Vector2D<double>(-1, 0),
                                    Vector2D<double>(0, -1),
                                    Vector2D<double>(1, 1),
                                    Vector2D<double>(-1, 1),
                                    Vector2D<double>(-1, -1),
                                    Vector2D<double>(1, -1)}};

std::array<double, Q> w = {4. / 9,  1. / 9,  1. / 9,  1. / 9, 1. / 9,
                           1. / 36, 1. / 36, 1. / 36, 1. / 36};
constexpr std::size_t balance = 1;

double Pr = 2. / 3;
double Kn = 0.08;
size_t iterations = 2;
constexpr std::size_t alpha = 0;
#endif // LBM_CPP_CONSTANTS_H