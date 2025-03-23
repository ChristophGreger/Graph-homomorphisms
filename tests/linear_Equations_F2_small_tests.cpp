//
// Created by Christoph Greger on 03.03.25.
//

#include <gtest/gtest.h>
#include <bitset>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <limits>
#include "linear_Equations_F2_small.h"


// GTest Speedtest: creates 1 million random systems and measures the time needed.
TEST(HomogeneousGF2SpeedTest, MillionRandomSystems_SolvingOnly) {
    const int numSystems = 1000000;
    const int num_vars = MAX_LSOE_BITSET;  // Number of variables (and thus also bitset size)
    const int m = 20;         // Number of equations per system

    std::mt19937 rng(12345);  // Fixed seed for reproducibility
    std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());

    //dummySum prevents the compiler from optimizing the function call away.
    int dummySum = 0;
    long long totalSolveTimeMicro = 0;

    for (int sys = 0; sys < numSystems; ++sys) {
        std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
        mat.reserve(m);
        // Create the random system (this time is not measured)
        for (int i = 0; i < m; ++i) {

            std::bitset<MAX_LSOE_BITSET> bs;
            for (int j = 0; j < num_vars / 64; j++) {
                unsigned long long part = dist(rng);
                for (int bit = 0; bit < 64; bit++) {
                    if (part & (1ULL << bit))
                        bs.set(j * 64 + bit);
                }
            }
            mat.push_back(bs);
        }
        //Measure only the time needed for solving
        auto startSolve = std::chrono::high_resolution_clock::now();
        dummySum += solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars));
        auto endSolve = std::chrono::high_resolution_clock::now();
        totalSolveTimeMicro += std::chrono::duration_cast<std::chrono::microseconds>(endSolve - startSolve).count();
    }
    std::cout << "Solving time for " << numSystems << " systems: "
              << totalSolveTimeMicro << " microseconds. Dummy sum: " << dummySum << "\n";
    SUCCEED();
}

// GTest Speedtest: creates 1 million random systems and measures the time needed.
TEST(InhomogeneousGF2SpeedTest, MillionRandomSystems_SolvingOnly) {
    const int numSystems = 1000000;
    const int num_vars = MAX_LSOE_BITSET - 1;  // 127 variables, bit 127 (index 127) is the right-hand side value.
    const int m = 20;          // 20 equations per system

    std::mt19937 rng(12345);  // fixed seed for reproducibility
    std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());

    int dummySum = 0;
    long long totalSolveTimeMicro = 0;

    for (int sys = 0; sys < numSystems; ++sys) {
        std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
        mat.reserve(m);
        // create the random system (this time is not measured)
        for (int i = 0; i < m; ++i) {

            std::bitset<MAX_LSOE_BITSET> bs;
            for (int j = 0; j < num_vars / 64; j++) {
                unsigned long long part = dist(rng);
                for (int bit = 0; bit < 64; bit++) {
                    if (part & (1ULL << bit))
                        bs.set(j * 64 + bit);
                }
            }

            mat.push_back(bs);
        }
        // Measure only the time needed for solving
        auto startSolve = std::chrono::high_resolution_clock::now();
        dummySum += solution_space_dimension_f2_small_inhomogen(LinearSystemOfEquations(mat, num_vars));
        auto endSolve = std::chrono::high_resolution_clock::now();
        totalSolveTimeMicro += std::chrono::duration_cast<std::chrono::microseconds>(endSolve - startSolve).count();
    }
    std::cout << "Solving time for " << numSystems
              << " inhomogeneous systems: " << totalSolveTimeMicro
              << " microseconds. Dummy sum: " << dummySum << "\n";
    SUCCEED();
}




// Testcase 1: empty matrix => no equations, so the solution space is ℤ₂^(num_vars)
TEST(SolutionSpaceDimensionTest, EmptyMatrix) {
    std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
    const int num_vars = 5;
    // epected dimension: 5
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars)), 5);
}

// Testcase 2: A single equation that forces x0 = 0
TEST(SolutionSpaceDimensionTest, SingleEquation) {
    std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
    std::bitset<MAX_LSOE_BITSET> eq;
    eq.reset();
    eq.set(0, true); // => x0 = 0
    mat.push_back(eq);
    const int num_vars = 5;
    // rank = 1, so dimension = 5 - 1 = 4
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars)), 4);
}

// Testcase 3: Two linearly independent equations (e.g. x0+x1=0 and x1+x2=0)
TEST(SolutionSpaceDimensionTest, TwoIndependentEquations) {
    std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
    std::bitset<MAX_LSOE_BITSET> eq1, eq2;
    eq1.reset();
    eq2.reset();
    eq1.set(0, true); eq1.set(1, true); // x0 + x1 = 0
    eq2.set(1, true); eq2.set(2, true); // x1 + x2 = 0
    mat.push_back(eq1);
    mat.push_back(eq2);
    const int num_vars = 5;
    // rank = 2, so dimension = 5 - 2 = 3
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars)), 3);
}

// Testcase 4: Two identical equations (linearly dependent)
TEST(SolutionSpaceDimensionTest, DuplicateEquations) {
    std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
    std::bitset<MAX_LSOE_BITSET> eq;
    eq.reset();
    eq.set(0, true); eq.set(1, true); // x0 + x1 = 0
    mat.push_back(eq);
    mat.push_back(eq); // identical row
    const int num_vars = 5;
    // rank = 1, so dimension = 5 - 1 = 4
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars)), 4);
}

// Testcase 5: Full system in 3 variables (x0=0, x1=0, x2=0)
TEST(SolutionSpaceDimensionTest, FullSystem) {
    std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
    const int num_vars = 3;
    for (int i = 0; i < num_vars; ++i) {
        std::bitset<MAX_LSOE_BITSET> eq;
        eq.reset();
        eq.set(i, true);
        mat.push_back(eq);
    }
    //rank = 3, so dimension = 3 - 3 = 0
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars)), 0);
}

// Testcase 6: A single equation that forces x0 = 0 in a system with 4 variables
TEST(SolutionSpaceDimensionTest, ZeroRow) {
    std::vector<std::bitset<MAX_LSOE_BITSET>> mat;
    std::bitset<MAX_LSOE_BITSET> eq;
    eq.reset(); // null row
    mat.push_back(eq);
    const int num_vars = 4;
    // Nullrow does not contribute to the rank, so dimension = 4
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(LinearSystemOfEquations(mat, num_vars)), 4);
}
