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
    std::array<double, Q> f, f_temp, f_eq;
    double tau, T, rho;
    Vector<double> v;
    void eq();
    void col();
    void macro();
    Point();
    Point(std::vector<double>);

};
class Grid{
    Grid(std::vector<std::pair<int, int>>);
    std::vector<std::vector<Point>> points;
    bool is_possible(int, int, int);
    void mirroring(int, int, int);
    void transfer(int, int);
    void at(int, int); // Do i need this one?
    void eval();
public:
    int **desc_grid;
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

Grid::Grid(std::vector<std::pair<int, int>> indata){
    int xmax = -100000, ymax = -100000, xmin = 100000, ymin = 100000;
    for(int i = 0; i < indata.size(); ++i){
        if (indata[i].first > xmax){
            xmax = indata[i].first;
        }
        if (indata[i].second > ymax){
            ymax = indata[i].second;
        }
        if (indata[i].first < xmin){
            xmin = indata[i].first;
        }
        if (indata[i].second < ymin){
            ymin = indata[i].second;
        }
    }
    desc_grid = new int*[xmax-xmin];
    for (int i = 0; i < xmax-xmin; i++)
        desc_grid[i] = new int[ymax-ymin];
    int flag = 0;
    for(int i = 0; i < xmax - xmin; ++i){
        for(int j = 0; j < ymax - ymin; ++j){
            for(int k = 0; k < indata.size(); ++k){
                if(i==indata[k].first &&
                   j==indata[k].second)
                {
                    desc_grid[i][j] = 0;
                    flag = 1;
                    break;
                }
            }
            if (flag==0){
                desc_grid[i][j] = -1;
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





#endif //LBM_CPP_POINT_H

