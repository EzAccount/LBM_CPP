#ifndef PARAVIEW_H_INCLUDED
#define PARAVIEW_H_INCLUDED
#define SCALE 100
#include "point.h"
#include "constants.h"
#include "utils.h"

int ExportToVtk(std::vector<std::vector<Point>> field, int x, int y, int iter) {
  char fname[30];
  sprintf(fname, "Result_%d.vtk", iter);
  FILE *vtkfile = fopen(fname, "w");
  int i, j;

  // header
  fprintf(vtkfile, "# vtk DataFile Version 2.0\n");
  fprintf(vtkfile, "Latice-Boltzman Results\n");
  fprintf(vtkfile, "ASCII\n");

  // grid
  fprintf(vtkfile, "DATASET STRUCTURED_POINTS\n");
  fprintf(vtkfile, "DIMENSIONS %d %d %d\n", x, y,
          1);                         /// dimensions (3 axis, z=1)
  fprintf(vtkfile, "ORIGIN 0 0 0\n"); /// initial poin of coordinates
  fprintf(vtkfile, "SPACING %lf %lf %lf\n", 1.0, 1.0, 1.0); /// grid step

  // P
  fprintf(vtkfile, "POINT_DATA %d\n", x * y); /// size of data array

  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS Density(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].rho * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // V(X)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS VELOCITY_X(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].v.x * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // V(Y)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS VELOCITY_Y(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].v.y * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // T
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS TEMP(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].T * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(0)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_0(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[0] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (0)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_0(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[0] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(1)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_1(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[1] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (1)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_1(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[1] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(2)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_2(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[2] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (2)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_2(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[2] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(3)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_3(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[3] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (3)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_3(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[3] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(4)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_4(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[4] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (4)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_4(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[4] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(5)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_5(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[5] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (5)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_5(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[5] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(6)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_6(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[6] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (6)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_6(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[6] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(7)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_7(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[7] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (7)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_7(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[7] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F(8)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS FUNC_8(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f[8] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }

  // F_EQ (8)
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS EQ_FUNC_8(x%dSCALE) float\n", SCALE);
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i][j].f_eq[8] * SCALE);
    }

    fprintf(vtkfile, "\n");
  }
  /*
  // LAMBDA
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS LAMBDA(x1SCALE) float\n");
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i + j * x].lambda);
    }

    fprintf(vtkfile, "\n");
  }
  // TYPE
  fprintf(vtkfile, "\n\n");
  fprintf(vtkfile, "SCALARS TYPE(x1SCALE) float\n");
  fprintf(vtkfile, "LOOKUP_TABLE default\n");

  for (j = 0; j < y; j++) {
    for (i = 0; i < x; i++) {
      fprintf(vtkfile, "%lf ", field[i + j * x].type * 1.0);
    }

    fprintf(vtkfile, "\n");
  }*/

  fclose(vtkfile);

  return 0;
}

#endif // PARAVIEW_H_INCLUDED
