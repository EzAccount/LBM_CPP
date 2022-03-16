//
// Created by mikhail on 8/13/18.
//
#ifndef LBM_CPP_CONSTANTS_H
#define LBM_CPP_CONSTANTS_H

#include <array>
#include <iostream>
#include <vector>
#include <cmath>
#include "vector_2d.h"

/**<
 * Q - number of grid directions.
 * e - vector of all directions.
 * w - array containing the Maxwell distribution coefficients for each
 * direction.
 * balance - balancing factor for boundary conditions.
 */
constexpr std::size_t Q = 17;
std::array<Vector2D<double>, 9> e_9 = {
        Vector2D<double>(0, 0),
        Vector2D<double>(1, 0),
        Vector2D<double>(0, 1),
        Vector2D<double>(-1, 0),
        Vector2D<double>(0, -1),
        Vector2D<double>(1, 1),
        Vector2D<double>(-1, 1),
        Vector2D<double>(-1, -1),
        Vector2D<double>(1, -1)
};

std::array<double, 9> w_9 = {4. / 9, 1. / 9, 1. / 9, 1. / 9, 1. / 9,
                             1. / 36, 1. / 36, 1. / 36, 1. / 36};

double r = sqrt((125 + 5 * sqrt(193)) / 72);

std::array<Vector2D<double>, 17> e_17 = {
        Vector2D<double>(0, 0), Vector2D<double>(1, 0),
        Vector2D<double>(0, 1), Vector2D<double>(-1, 0),
        Vector2D<double>(0, -1), Vector2D<double>(1, 1),
        Vector2D<double>(-1, 1), Vector2D<double>(-1, -1),
        Vector2D<double>(1, -1), Vector2D<double>(2, 2),
        Vector2D<double>(-2, 2), Vector2D<double>(-2, -2),
        Vector2D<double>(2, -2), Vector2D<double>(3, 0),
        Vector2D<double>(0, 3), Vector2D<double>(-3, 0),
        Vector2D<double>(0, -3)
};

std::array<double, 17> w_17 = {
        (575 + 193 * sqrt(193)) / 8100., (3355 - 91 * sqrt(193)) / 18000., (3355 - 91 * sqrt(193)) / 18000.,
        (3355 - 91 * sqrt(193)) / 18000., (3355 - 91 * sqrt(193)) / 18000., (655 + 17 * sqrt(193)) / 27000.,
        (655 + 17 * sqrt(193)) / 27000., (655 + 17 * sqrt(193)) / 27000., (655 + 17 * sqrt(193)) / 27000.,
        (685 - 49 * sqrt(193)) / 54000., (685 - 49 * sqrt(193)) / 54000., (685 - 49 * sqrt(193)) / 54000.,
        (685 - 49 * sqrt(193)) / 54000., (1445 - 101 * sqrt(193)) / 162000., (1445 - 101 * sqrt(193)) / 162000.,
        (1445 - 101 * sqrt(193)) / 162000., (1445 - 101 * sqrt(193)) / 162000.
};

const auto &e() {
    if constexpr (Q == 9) {
        return e_9;
    } else if constexpr (Q == 17) {
        return e_17;
    }
}

const auto &w() {
    if constexpr (Q == 9) {
        return w_9;
    } else if constexpr (Q == 17) {
        return w_17;
    }
}

constexpr std::size_t balance = 1;

double Kn = 0.25;
size_t iterations = 20000;
constexpr std::size_t alpha = 0;
#endif // LBM_CPP_CONSTANTS_H