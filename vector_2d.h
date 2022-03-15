//
// Created by Evgenii Zheltonozhskii on 13/02/2021.
//

#ifndef LBM_CPP_VECTOR_2D_H
#define LBM_CPP_VECTOR_2D_H

template<typename T>
struct Vector2D {
    T x;
    T y;

    explicit Vector2D(T x_ = 0, T y_ = 0)
    : x(x_)
    , y(y_) {}

    T operator*(const Vector2D<T> &rhs) const {

        return this->x * rhs.x + this->y * rhs.y;
    }

    Vector2D operator+=(const Vector2D<T> &rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    friend Vector2D operator+(Vector2D<T> lhs, const Vector2D<T> &rhs) {
        lhs += rhs;
        return lhs;
    }

    Vector2D operator-=(const Vector2D<T> &rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    friend Vector2D operator-(Vector2D<T> lhs, const Vector2D<T> &rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Vector2D operator*(T scalar, const Vector2D<T> &rhs) {
        return Vector2D(scalar * rhs.x, scalar * rhs.y);
    }

    friend bool operator==(const Vector2D<T> &lhs, const Vector2D<T> &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

#endif // LBM_CPP_VECTOR_2D_H
