#include "point.h"
#include <iostream>
#include <fstream>

// typedef Point point;

/*double scalar(double *a, double *b) // dot product in R^2
{
    return a[0]*b[0]+a[1]*b[1];
}

double sq_module(double *a) // dot square
{
    return a[0]*a[0]+a[1]*a[1];
}

void vec_sum(double *a, double *b) // sum in R^2
{
    a[0] += b[0];
    a[1] += b[1];
}

double*  vec_mul(double *a, double b) // vector * scalar
{
    a[0] *= b;
    a[1] *= b;
    return a;
}*/

int main() { // int argc, char **argv
  /*if (argc > 2) {
    sscanf(argv[1], "%zu", &iterations);
    sscanf(argv[2], "%lf", &Kn);
  }*/
  // e =
  // {{0.,0.},{1.,0.},{0.,1.},{-1.,0.},{0.,-1.},{1.,1.},{-1.,1.},{-1.,-1.},{1.,-1.}};
  e[0].x = 0.;
  e[0].y = 0.;
  e[1].x = 1.;
  e[1].y = 0.;
  e[2].x = 0.;
  e[2].y = 1.;
  e[3].x = -1.;
  e[3].y = 0.;
  e[4].x = 0.;
  e[4].y = -1.;
  e[5].x = 1.;
  e[5].y = 1.;
  e[6].x = -1.;
  e[6].y = 1.;
  e[7].x = -1.;
  e[7].y = -1.;
  e[8].x = 1.;
  e[8].y = -1.;
  std::vector<std::pair<int, int>> indata;
  for (size_t i = 0; i <= 100; ++i) {
    for (size_t j = 0; j <= 10; ++j) {
      indata.emplace_back(i, j);
    }
  }
  Grid Puas(indata);
  Puas.boundaries();
  for (int j = 0; j <= 10; ++j) {
    for (int i = 0; i <= 100; ++i) {
      Puas.grid[i][j].rho = 1.;
      Puas.grid[i][j].T = 1.;
      Puas.grid[i][j].v = {0, 0};
      Puas.grid[i][j].eq();
    }
    Puas.grid[100][j].rho = 1.2;
    Puas.grid[100][j].eq();
  }
  for (int i = 0; i <= 100; ++i) {
    for (int j = 0; j <= 10; ++j) {
      for (int k = 0; k < Q; ++k) {
        Puas.grid[i][j].f[k] = Puas.grid[i][j].f_eq[k];
        Puas.grid[i][j].f_temp[k] = Puas.grid[i][j].f_eq[k];
      }
    }
  }
  for (int t = 0; t < iterations; ++t) {
    for (int i = 0; i <= 100; ++i) {
      for (int j = 0; j <= 10; ++j) {
        //if (Puas.grid[i][j].f_temp[5] < 0) {std::cout<< i << ' ' << j << ' ' << Puas.grid[i][j].f_temp[1] << std::endl;}
        Puas.transfer(i, j);
        Puas.grid[i][j].col();
        Puas.grid[i][j].macro();
        Puas.grid[i][j].eq();
      }
    }
  }

  std::ofstream out;
  out.open("results.dat");
  if (out.is_open()) {
    for (int i = 0; i <= 100; ++i) {
      for (int j = 0; j <= 10; ++j) {
        out << i << " " << j << " " << Puas.grid[i][j].rho << std::endl;
      }
    }
    /*
    for (int k = 0; k <= 9; ++k) {
      out1 << 49 << " " << 5 << " " << Puas.grid[49][5].f[k] << std::endl;
    }*/
  }
  return 0;
}
