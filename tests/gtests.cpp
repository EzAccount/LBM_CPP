//
// Created by thear on 03.04.2020.
//

#include "../point.h"
#include <iostream>
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(GridConstructor__Test, MAX_MIN_Test) {
  std::vector<std::pair<int, int>> indata = {{2, 4}, {0, 5}, {1, 5}, {1, 3},
                                             {0, 0}, {2, 3}, {5, 4}, {5, 5},
                                             {7, 5}, {7, 0}};
  Grid A(indata);
  EXPECT_EQ(A.grid.size(), 8);
  EXPECT_EQ(A.grid[0].size(), 6);
}
TEST(GridBoundaries__Test, Boundaries_Test) {
  std::vector<std::pair<int, int>> indata;
  for (size_t j = 0; j < 6; ++j) {
    indata.emplace_back(0, j);
    indata.emplace_back(1, j);
    indata.emplace_back(2, j);
    indata.emplace_back(5, j);
    indata.emplace_back(6, j);
    indata.emplace_back(7, j);
  }
  for (size_t j = 0; j < 5; ++j) {

    indata.emplace_back(3, j);
    indata.emplace_back(4, j);
  }
  Grid A(indata);
  A.boundaries();
  EXPECT_EQ(A.grid[2][5].bound, 0);
  EXPECT_EQ(A.grid[4][4].bound, 1);
  EXPECT_EQ(A.grid[0][5].bound, 1);
  EXPECT_EQ(A.grid[6][0].bound, 1);
  EXPECT_EQ(A.grid[1][3].bound, 0);
}

TEST(GridTransfer__Test, Transfer_Test) {
  e[0] = {0., 0.};
  e[1] = {1., 0.};
  e[2] = {0., 1.};
  e[3] = {-1., 0.};
  e[4] = {0., -1.};
  e[5] = {1., 1.};
  e[6] = {-1., 1.};
  e[7] = {-1., -1.};
  e[8] = {1., -1.};
  // i did initialization here because main functions are conflicting TODO: fix this later
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
  A.grid[1][1].f_eq[1] = 1.;
  A.grid[1][1].f[3] = 0;
  A.grid[6][1].f[5] = 0;
  A.grid[6][4].f[7] = 0;
  A.grid[6][3].f_eq[6] = 10.;
  A.grid[6][1].f_eq[6] = 4.;
  A.grid[6][4].f_eq[7] = 2.;
  A.grid[5][3].f_eq[7] = 12.;
  A.transfer(1, 1);
  A.transfer(6, 1);
  A.transfer(6, 4);
  EXPECT_EQ(A.grid[1][1].f_temp[1], 0); // the left wall
  EXPECT_EQ(A.grid[6][3].f_temp[6], 3); // the right wall
  EXPECT_EQ(A.grid[5][3].f_temp[7], 5); // simple move
}