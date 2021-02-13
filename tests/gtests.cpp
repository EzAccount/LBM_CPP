//
// Created by thear on 03.04.2020.
//

#include "../point.h"
#include <iostream>
#include <gtest/gtest.h>

std::vector<std::pair<int, int>> initialization() {
  std::vector<std::pair<int, int>> input_data;
  for (size_t i = 0; i <= 100; ++i) {
    for (size_t j = 0; j <= 10; ++j) {
      input_data.emplace_back(i, j);
    }
  }
  return input_data;
}

Grid poiseuille_flow_initialization(std::vector<std::pair<int, int>> input_data) {
  Grid poiseuille(input_data);
  poiseuille.boundaries();
  for (int j = 0; j < poiseuille.grid[0].size(); ++j) {
    for (int i = 0; i < poiseuille.grid.size(); ++i) {
      poiseuille.grid[i][j].rho = 1.;
      poiseuille.grid[i][j].T = 1.;
      poiseuille.grid[i][j].v = Vector2D<double>(0.,0.);
      poiseuille.grid[i][j].eq();
    }
    poiseuille.grid[poiseuille.grid.size()-1][j].rho = 1.2;
    poiseuille.grid[poiseuille.grid.size()-1][j].eq();
  }
  for (int i = 0; i < poiseuille.grid.size(); ++i) {
    for (int j = 0; j < poiseuille.grid[0].size(); ++j) {
      for (int k = 0; k < Q; ++k) {
        poiseuille.grid[i][j].f[k] = poiseuille.grid[i][j].f_eq[k];
        poiseuille.grid[i][j].f_temp[k] = poiseuille.grid[i][j].f_eq[k];
      }
    }
  }
  return poiseuille;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(GridConstructor__Test, MAX_MIN_Test) {
  auto input_data = initialization();
  Grid A(input_data);
  EXPECT_EQ(A.grid.size(), 101);
  EXPECT_EQ(A.grid[0].size(), 11);
}

TEST(GridEquilibrium_Test, MacroEqPoiseuille_Test) {
  auto input_data = initialization();
  auto Pois = poiseuille_flow_initialization(input_data);
  for (size_t i = 0; i < Pois.grid.size() - 1; ++i) {
    for (size_t j = 0; j < Pois.grid[0].size(); ++j) {
      EXPECT_EQ(Pois.grid[i][j].f_eq[0], 4./9);
    }
  }
  for (size_t j = 0; j < Pois.grid[0].size(); ++j) {
    EXPECT_EQ(Pois.grid[Pois.grid.size() - 1][j].f_eq[0], 1.2 * 4./9);
  }
}

TEST(GridBoundaries__Test, Boundaries_Test) {
  std::vector<std::pair<int, int>> input_data;
  for (size_t j = 0; j < 6; ++j) {
    input_data.emplace_back(0, j);
    input_data.emplace_back(1, j);
    input_data.emplace_back(5, j);
    input_data.emplace_back(6, j);
    input_data.emplace_back(7, j);
  }
  for (size_t j = 0; j < 5; ++j) {
    input_data.emplace_back(2, j);
    input_data.emplace_back(3, j);
    input_data.emplace_back(4, j);
  }
  Grid A(input_data);
  A.boundaries();
  EXPECT_EQ(A.grid[7][5].interior, 0);
  EXPECT_EQ(A.grid[4][5].bound, 0);
  EXPECT_EQ(A.grid[0][5].bound, 1);
  EXPECT_EQ(A.grid[6][0].bound, 1);
  EXPECT_EQ(A.grid[1][3].bound, 0);
}

TEST(GridCollision__Test, MacroCol_Test) {
  auto input_data = initialization();
  auto Pois = poiseuille_flow_initialization(input_data);
  for (int i = 0; i <= 100; ++i) {
    for (int j = 0; j <= 10; ++j) {
      Pois.transfer(i, j);
    }
  }
  for (size_t k = 0; k < Q; ++k) {
    EXPECT_EQ(Pois.grid[1][2].f[k], Pois.grid[1][2].f_temp[k]);
  }
}

TEST(GridTransfer__Test, Transfer_Test) {
  auto input_data = initialization();
  auto Pois = poiseuille_flow_initialization(input_data);
  for (int i = 0; i <= 100; ++i) {
    for (int j = 0; j <= 10; ++j) {
      Pois.transfer(i, j);
    }
  }
  EXPECT_EQ(Pois.grid[1][2].f_temp[0], 4./9);
  EXPECT_EQ(Pois.grid[1][2].f_temp[1], 1./9);
  EXPECT_EQ(Pois.grid[1][2].f_temp[2], 1./9);
  EXPECT_EQ(Pois.grid[1][2].f_temp[3], 1./9);
  EXPECT_EQ(Pois.grid[1][2].f_temp[4], 1./9);
  EXPECT_EQ(Pois.grid[1][2].f_temp[5], 1./36);
  EXPECT_EQ(Pois.grid[1][2].f_temp[6], 1./36);
  EXPECT_EQ(Pois.grid[1][2].f_temp[7], 1./36);
  EXPECT_EQ(Pois.grid[1][2].f_temp[8], 1./36);
}

TEST(GridMacro__Test, Macro_Test) {
  auto input_data = initialization();
  auto Pois = poiseuille_flow_initialization(input_data);
  for (int i = 0; i <= 100; ++i) {
    for (int j = 0; j <= 10; ++j) {
      Pois.transfer(i, j);
      Pois.grid[i][j].col();
      Pois.grid[i][j].macro();
      Pois.grid[i][j].eq();
    }
  }
  EXPECT_DOUBLE_EQ(Pois.grid[1][2].rho, 1.);
}

/*TEST(GridTransfer__Test, Transfer_Test) {
  e[0] = {0., 0.};
  e[1] = {1., 0.};
  e[2] = {0., 1.};
  e[3] = {-1., 0.};
  e[4] = {0., -1.};
  e[5] = {1., 1.};
  e[6] = {-1., 1.};
  e[7] = {-1., -1.};
  e[8] = {1., -1.};
  // i did initialization here because main functions are conflicting TODO: fix
  // this later
  std::vector<std::pair<int, int>> indata;
  for (size_t j = 0; j < 6; ++j) {
    indata.emplace_back(0, j);
    indata.emplace_back(1, j);
    indata.emplace_back(5, j);
    indata.emplace_back(6, j);
    indata.emplace_back(7, j);
  }
  for (size_t j = 0; j < 5; ++j) {
    indata.emplace_back(2, j);
    indata.emplace_back(3, j);
    indata.emplace_back(4, j);
  }
  Grid A(indata);
  A.boundaries();
  A.grid[2][2].f[1] = 1.;
  A.transfer(2, 2);
  EXPECT_EQ(A.grid[3][2].f_temp[1], 1); // simple move
  A.grid[6][3].f[5] = 0;
  A.grid[7][4].f_eq[7] = 2.;
  A.transfer(6, 3);
  EXPECT_EQ(A.grid[6][3].f_temp[7], 2); // double bound
  A.grid[7][4].f_eq[6] = 6;
  A.grid[7][2].f_eq[6] = 0;
  A.grid[6][2].f[5] = 0;
  A.transfer(6, 2);
  EXPECT_EQ(A.grid[6][4].f_temp[6], 3); // simple bound left wall
  A.grid[6][4].f[6] = 0;
  A.grid[5][5].f_eq[8] = 4;
  A.transfer(6, 4);
  EXPECT_EQ(A.grid[6][4].f_temp[8], 4); // concave corner
  A.grid[6][3].f[6] = 0;
  A.grid[5][4].f_eq[8] = 5;
  A.transfer(6, 3);
  EXPECT_EQ(A.grid[6][3].f_temp[8], 5); // convex corner}
  */
