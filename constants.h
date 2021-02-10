//
// Created by mikhail on 8/13/18.
//
#ifndef LBM_CPP_CONSTANTS_H
#define LBM_CPP_CONSTANTS_H

#include <array>
#include <iostream>
#include <vector>

template <typename T> struct Vector {
  T x;
  T y;
  Vector(T x = 0, T y = 0) : x(x), y(y) {}
  T operator*(const Vector<T> &rhs) const {

    return this->x * rhs.x + this->y * rhs.y;
  }
  Vector operator+=(const Vector<T> &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }
  friend Vector operator+(Vector<T> lhs, const Vector<T> &rhs) {
    lhs += rhs;
    return lhs;
  }
  Vector operator-=(const Vector<T> &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
  }
  friend Vector operator-(Vector<T> lhs, const Vector<T> &rhs) {
    lhs -= rhs;
    return lhs;
  }
  friend Vector operator*(T scalar, const Vector<T> &rhs) {
    return Vector(scalar * rhs.x, scalar * rhs.y);
  }
};

constexpr std::size_t Q = 9;
double Pr = 2. / 3;
double Kn = 0.08;
size_t iterations = 1;
std::array<Vector<double>, Q> e = {{{0, 0},
                                    {1, 0},
                                    {0, 1},
                                    {-1, 0},
                                    {0, -1},
                                    {1, 1},
                                    {-1, 1},
                                    {-1, -1},
                                    {1, -1}}};
std::array<double, Q> w = {4. / 9,  1. / 9,  1. / 9,  1. / 9, 1. / 9,
                           1. / 36, 1. / 36, 1. / 36, 1. / 36};
size_t x_size, y_size;
constexpr std::size_t alpha = 0;
constexpr std::size_t balance = 1;
#endif // LBM_CPP_CONSTANTS_H
