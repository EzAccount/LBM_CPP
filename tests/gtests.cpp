//
// Created by thear on 03.04.2020.
//

#include "../point.h"
#include <gtest/gtest.h>
#include <iostream>


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

std::vector<std::pair<int, int>> init(int x_size, int y_size) {
    std::vector<std::pair<int, int>> input_data;
    for (size_t i = 0; i <= x_size; ++i) {
        for (size_t j = 0; j <= y_size; ++j) {
            input_data.emplace_back(i, j);
        }
    }
    return input_data;
}

Grid pois_flow(std::vector<std::pair<int, int>> init) {
    Grid pois(init);
    pois.boundaries();
    return pois;
}

TEST(GridBoundaries__Test, Boundaries_Test) {
    std::vector<std::pair<int, int>> init;
    for (size_t i = 0; i <= 2; ++i) {
        for (size_t j = 0; j <= 10; ++j) {
            init.emplace_back(i, j);
        }
    }

    for (size_t j = 0; j <= 5; ++j) {
        init.emplace_back(3, j);
    }


    for (size_t j = 0; j <= 7; ++j) {
        init.emplace_back(4, j);
    }


    for (size_t i = 5; i <= 8; ++i) {
        for (size_t j = 0; j <= 9; ++j) {
            init.emplace_back(i, j);
        }
    }

    Grid vessel(init);
    vessel.boundaries();
    for (int j = 0; j <= 10; ++j) {
        EXPECT_EQ(vessel.grid[0][j].bound, 1);
    }
    for (int j = 1; j < 10; ++j) {
        EXPECT_EQ(vessel.grid[1][j].interior, 1);
    }
    for (int i = 0; i <= 8; ++i) {
        EXPECT_EQ(vessel.grid[i][0].bound, 1);
    }
    for (int j = 0; j <= 9; ++j) {
        EXPECT_EQ(vessel.grid[8][j].bound, 1);
    }

    EXPECT_EQ(vessel.grid[1][10].bound, 1);
    EXPECT_EQ(vessel.grid[2][10].bound, 1);
    EXPECT_EQ(vessel.grid[2][5].bound, 1);
    EXPECT_EQ(vessel.grid[3][5].bound, 1);
    EXPECT_EQ(vessel.grid[4][5].bound, 1);
    for (int j = 5; j <= 7; ++j) {
        EXPECT_EQ(vessel.grid[4][j].bound, 1);
    }
    EXPECT_EQ(vessel.grid[5][7].bound, 1);
    EXPECT_EQ(vessel.grid[5][8].bound, 1);
    EXPECT_EQ(vessel.grid[5][9].bound, 1);
    EXPECT_EQ(vessel.grid[6][9].bound, 1);
    EXPECT_EQ(vessel.grid[7][9].bound, 1);
    EXPECT_EQ(vessel.grid[4][8].bound || vessel.grid[4][8].interior, 0);
    EXPECT_EQ(vessel.grid[6][10].bound || vessel.grid[6][10].interior, 0);
    EXPECT_EQ(vessel.grid[2][3].interior, 1);
    EXPECT_EQ(vessel.grid[5][1].interior, 1);
    EXPECT_EQ(vessel.grid[4][0].interior, 0);
    EXPECT_EQ(vessel.grid[7][6].interior, 1);
}

TEST(GridWeightCalculate__Test, WeightCalculate_Test) {
    Grid vessel(init(100, 10));
    vessel.boundaries();
    vessel.grid[50][5].bound = true;
    vessel.weight_calculate();
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[0], 4. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[1], 1. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[2], 1. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[5], 1. / 36 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[3], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[4], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[6], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[7], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].w_for_bound_point[8], 0);

    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[0], 4. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[3], 1. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[4], 1. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[7], 1. / 36 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[1], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[2], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[5], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[6], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[100][10].w_for_bound_point[8], 0);

    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[0], 4. / 9);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[1], 1. / 9);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[2], 1. / 9);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[3], 1. / 9);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[4], 1. / 9);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[5], 1. / 36);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[6], 1. / 36);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[7], 1. / 36);
    EXPECT_DOUBLE_EQ(vessel.grid[50][5].w_for_bound_point[8], 1. / 36);

    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[0], 4. / 9 * 36 / 30);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[1], 1. / 9 * 36 / 30);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[3], 1. / 9 * 36 / 30);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[7], 1. / 36 * 36 / 30);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[8], 1. / 36 * 36 / 30);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[2], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[5], 0);
    EXPECT_DOUBLE_EQ(vessel.grid[67][10].w_for_bound_point[6], 0);
}

TEST(GridTransfer__Test, Transfer_Test) {
    Grid vessel(init(100, 10));
    vessel.boundaries();
    vessel.weight_calculate();
    vessel.grid[1][1].f = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    vessel.grid[0][0].f_temp = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vessel.grid[2][1].f_temp = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vessel.grid[1][2].f_temp = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vessel.transfer(1, 1);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].f_temp[0], 4. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].f_temp[1], 1. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].f_temp[2], 1. / 9 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[0][0].f_temp[5], 1. / 36 * 36. / 25);
    EXPECT_DOUBLE_EQ(vessel.grid[1][1].f[1], 1);
    EXPECT_DOUBLE_EQ(vessel.grid[2][1].f_temp[1], 1);
    EXPECT_DOUBLE_EQ(vessel.grid[1][2].f_temp[2], 1);

}
