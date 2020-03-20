//
// Created by mikhail on 8/12/18.
//
#ifndef LBM_CPP_POINT_H
#define LBM_CPP_POINT_H

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "constants.h"


class Point{
public:
    std::array<double, Q> f, f_temp, f_eq;
    double tau, T, rho, P;

    bool exist;

    Vector<double> v;
    void eq();
    void col(); // TODO: implement
    void macro(); // TODO:check
    Point();
    Point(std::array<double, 4>);// TODO: initials

};
class Grid{
    Grid(std::vector<std::pair<int, int>>);
    std::vector<std::vector<Point>> points;
    bool is_possible(int, int,int);  // TODO: is needed?
    void mirroring(int,int,int);  // TODO: boundaries
    void boundaries(Point **desc_grid);
    void transfer(int, int); // TODO: standalone function
    void at(int, int); // TODO: is this needed?
    void eval();
public:
    std::array<double,5> macro_at(size_t, size_t); // TODO: result output
    Point **desc_grid;
    bool **bound_grid;
};

void Point::col() {
    for (size_t k = 0; k < Q; ++k)
    {
        f[k] = f_temp[k] - 1./tau * (f_temp[k] -  f_eq[k]);
    }
}
void Point::eq() {
    double c = T;
    for (size_t k = 0; k < Q; ++k){
        double sc = e[k] * v;
        f_eq[k] = rho * w[k] * (1. + 3 * sc/c + 4.5 * sc * sc / c / c -  v * v * 1.5/ c / c);
    }

}
void Point::macro() {
    rho = 0;
    for (size_t k=0; k<Q; ++k)
    {
        rho+=f_temp[k];
    }
    T = 0;
    for (size_t k = 0; k < Q; ++k){
        T += (e[k] - v)*(e[k] - v) * f_temp[k] * 3/2;
    }
    v.x = 0;
    v.y = 0;
    for (size_t k = 0; k < Q; ++k){
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
Point::Point(std::array<double, 4> init){
    tau = 0.;
    T = init[3];
    rho = init[0];
    v.x = init[1];
    v.y = init[2];
    eq();
    for (int k = 0; k < Q; ++k){
        f[k] = f_eq[k];
        f_temp[k] = f_eq[k];
    }
}

Grid::Grid(std::vector<std::pair<int, int>> indata){
    int xmax = 0, ymax = 0, xmin = 0, ymin = 0;
    xmax = indata[0].first;
    xmin = indata[0].first;
    ymax = indata[0].second;
    ymin = indata[0].second;
    for(int i = 0; i < indata.size(); ++i) {
        xmax = std::max_element(xmax, indata[i].first);
        ymax = std::max_element(ymax, indata[i].second);
        xmin = std::min_element(xmin, indata[i].first);
        ymin = std::min_element(ymin, indata[i].second);
    }
    desc_grid = new Point*[xmax-xmin];
    for (int i = 0; i < xmax-xmin; i++)
        desc_grid[i] = new Point[ymax-ymin];
    int flag = 0;
    for(int i = 0; i < xmax - xmin; ++i){
        for(int j = 0; j < ymax - ymin; ++j){
            for(int k = 0; k < indata.size(); ++k){
                if(i==indata[k].first &&
                   j==indata[k].second)
                {
                    desc_grid[i][j].exist = 0;
                    flag = 1;
                    break;
                }
            }
            if (flag==0){
                desc_grid[i][j].exist = 0;
            } else flag = 0;
        }
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
void Grid::boundaries(Point **grid){
    bound_grid = new bool*[sizeof(grid)];
    for (unsigned int i = 0; i < sizeof(grid); i++)
        bound_grid[i] = new bool[sizeof(*grid)];
    for(int i = 0; i < sizeof(grid); ++i){
        for(int j = 0; j < sizeof(*grid); ++j){
            if(grid[i][j].exist){
                if (i==0 || j==0 || i==sizeof(grid) || j==sizeof(*grid)){
                    bound_grid[i][j] = 1;// Если точка на границе сетки, то она -- граница
                }
                else {
                    int count = 0;
                    for (int a = -1; a < 1; ++a) {
                        for (int b = -1; b < 1; ++b) {
                            count += grid[i + a][j + b].exist;
                            /*счётчик во всех направлениях. если хотя бы по 1 направлению
                            точка не принадлежит сетке, то исходная точка i j на границе*/
                        }
                    }
                }
                bound_grid[i][j] = count < 9;
            } else bound_grid[i][j] = false;
        }
    }
}



#endif //LBM_CPP_POINT_H

