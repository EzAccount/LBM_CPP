//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H

#include <iostream>
#include <vector>
#include <array>
#include "constants.h"


class Point{
public:
    std::array<double, Q> f_temp, f_eq; // TODO:vectors?
    std::vector<double> f(Q);
    double tau, T, rho, P;
    Vector<double> v;
    void eq(); // TODO: check
    void col(); // TODO: implement
    void macro(); // TODO:check
    Point();
    Point(std::array<double,5>);// TODO: initials

};
class Grid{
    std::vector<std::vector<Point>> points;
    bool is_possible(int, int,int);  // TODO: is needed?
    void mirroring(int,int,int);  // TODO: boundaries
    void transfer(int, int); // TODO: standalone function
    void at(int, int); // TODO: is this needed?
    void eval();
public:
    std::array<double,5> macro_at(size_t, size_t); // TODO: result output
};

void Point::col() {
    for (size_t k=0; k<Q; ++k)
    {
        f[k] = f_temp[k] - 1./tau * (f_temp[k] -  f_eq[k]);
    }
}
void Point::eq() {
    double c = T;
    for (size_t k=0; k<Q; ++k){
        double sc = e[k] * v;
        f_eq[k] = rho * w[k] * (1. + 3 * sc/c + 4.5 * sc * sc / c / c -  v * v * 1.5/ c);
    }

}
void Point::macro() {
    rho = 0;
    for (size_t k=0; k<Q; ++k)
    {
        rho+=f_temp[k];
    }
    T = 0;
    for (size_t k=0; k<Q; ++k){
        T += (e[k] - v)*(e[k] - v) * f_temp[k] * 3/2;
    }
    v.x = 0;
    v.y = 0;
    for (size_t k=0; k<Q; ++k){
        v += f_temp[k] / rho * e[k];
    }
}
Point::Point() {
    tau = 0.;
    T = 0.;
    rho = 0.;
    v.x = 0.;
    v.y = 0.;
    eq();
    for (int k=0; k<Q; ++k){
        f[k] = f_eq[k];
        f_temp[k] = f_eq[k];
    }
}
Point::Point(std::vector<double> init){
    tau = 0.;
    T = init[3];
    rho = init[0];
    v.x = init[1];
    v.y = init[2];
    eq();
    for (int k=0; k<Q; ++k){
        f[k] = f_eq[k];
        f_temp[k] = f_eq[k];
    }
}

bool Grid::is_possible(int x, int y, int k) {
    // add check for k corresponding to speed/2 [speed/3, etc]
    if ((x + e[k].x < x_size) && (x + e[k].x >= 0) && (x + e[k].x < y_size) && (x + e[k].x >= 0)){
        return (points[x + e[k].x][y + e[k].y].rho != -1) ;
    }
    return false;
}
void Grid::transfer(int x, int y) {
    for (size_t k=0; k<Q; ++k){
        if (is_possible(x,y,k)) points[x+e[k].x][y+e[k].y].f_temp[k] = points[x][y].f[k];
    }
}





#endif //LBM_CPP_POINT_H

