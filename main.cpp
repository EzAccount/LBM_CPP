#include "point.h"
#include <iostream>
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
}*/ //Do i need this func?

int main(int argc, char **argv) {
    if (argc > 2){
        sscanf(argv[1], "%zu", &iterations);
        sscanf(argv[2], "%lf", &Kn);
    }
    //e = {{0.,0.},{1.,0.},{0.,1.},{-1.,0.},{0.,-1.},{1.,1.},{-1.,1.},{-1.,-1.},{1.,-1.}};
    e[0].x = 0.;     e[0].y = 0.;
    e[1].x = 1.;     e[1].y = 0.;
    e[2].x = 0.;     e[2].y = 1.;
    e[3].x = -1.;    e[3].y = 0.;
    e[4].x = 0.;     e[4].y = -1.;
    e[5].x = 1.;     e[5].y = 1.;
    e[6].x = -1.;    e[6].y = 1.;
    e[7].x = -1.;    e[7].y = -1.;

    e[8].x = 1.;     e[8].y = -1.;
    std::vector<double> p = {1., 0.1, -0.1, 1.};
    p.macro();
    std::cout<<p.tau << " " << p.rho << " " << p.T <<" " << p.v.x << " " << p.v.y << " ";
    std::cout << std::endl;
}