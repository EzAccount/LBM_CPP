#include "point.h"

int main(int argc, char **argv) {
    if (argc > 2){
        sscanf(argv[1], "%zu", &iterations);
        sscanf(argv[2], "%lf", &Kn);
    }
    e[0].x = 0.;     e[0].y = 0.;
    e[1].x = 1.;     e[1].y = 0.;
    e[2].x = 0.;     e[2].y = 1.;
    e[3].x = -1.;    e[3].y = 0.;
    e[4].x = 0.;     e[4].y = -1.;
    e[5].x = 1.;     e[5].y = 1.;
    e[6].x = -1.;    e[6].y = 1.;
    e[7].x = -1.;    e[7].y = -1.;
    e[8].x = 1.;     e[8].y = -1.;
    std::array<double,5> r = {1.,1., 0.1, -0.1, 1.};
    Point p(r);
    p.macro();
    std::cout<<p.tau << " " << p.rho << " " << p.T <<" " << p.v.x << " " << p.v.y << " ";
    std::cout << std::endl;
}