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

Grid poseuille_flow_initialization(std::vector<std::pair<int, int>> input_data) {
  Grid poseuille(input_data);
  poseuille.boundaries();
  for (int j = 0; j < poseuille.grid[0].size(); ++j) {
    for (int i = 0; i < poseuille.grid.size(); ++i) {
      poseuille.grid[i][j].rho = 1.;
      poseuille.grid[i][j].T = 1.;
      poseuille.grid[i][j].v = {0, 0};
      poseuille.grid[i][j].eq();
    }
    poseuille.grid[poseuille.grid.size()-1][j].rho = 1.2;
    poseuille.grid[poseuille.grid.size()-1][j].eq();
  }
  for (int i = 0; i < poseuille.grid.size(); ++i) {
    for (int j = 0; j < poseuille.grid[0].size(); ++j) {
      for (int k = 0; k < Q; ++k) {
        poseuille.grid[i][j].f[k] = poseuille.grid[i][j].f_eq[k];
        poseuille.grid[i][j].f_temp[k] = poseuille.grid[i][j].f_eq[k];
      }
    }
  }
  return poseuille;
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

TEST(GridEquilibrium_Test, MacroEqPoseuille_Test) {
  auto input_data = initialization();
  auto Pose = poseuille_flow_initialization(input_data);
  for (size_t i = 0; i < Pose.grid.size() - 1; ++i) {
    for (size_t j = 0; j < Pose.grid[0].size(); ++j) {
      EXPECT_EQ(Pose.grid[i][j].f_eq[0], 4./9);
    }
  }
  for (size_t j = 0; j < Pose.grid[0].size(); ++j) {
    EXPECT_EQ(Pose.grid[Pose.grid.size() - 1][j].f_eq[0], 1.2 * 4./9);
  }
}

TEST(GridBoundaries__Test, Boundaries_Test) {
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
  EXPECT_EQ(A.grid[7][5].interior, 0);
  EXPECT_EQ(A.grid[4][5].bound, 0);
  EXPECT_EQ(A.grid[0][5].bound, 1);
  EXPECT_EQ(A.grid[6][0].bound, 1);
  EXPECT_EQ(A.grid[1][3].bound, 0);
}

TEST(GridCollision__Test, MacroCol_Test) {
  auto input_data = initialization();

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
