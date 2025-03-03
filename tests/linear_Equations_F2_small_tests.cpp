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


// GTest Speedtest: Erzeugt 1 Million zufälliger Systeme und misst die benötigte Zeit.
TEST(HomogeneousGF2SpeedTest, MillionRandomSystems_SolvingOnly) {
    const int numSystems = 1000000;
    const int num_vars = 128;  // Anzahl der Variablen (und damit auch Bitset-Größe)
    const int m = 20;         // Anzahl der Gleichungen pro System

    std::mt19937 rng(12345);  // Fester Seed für Reproduzierbarkeit
    std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());

    // dummySum verhindert, dass der Compiler den Funktionsaufruf komplett optimiert.
    int dummySum = 0;
    long long totalSolveTimeMicro = 0;

    for (int sys = 0; sys < numSystems; ++sys) {
        std::vector<std::bitset<128>> mat;
        mat.reserve(m);
        // Erzeuge das zufällige System (diese Zeit wird nicht gemessen)
        for (int i = 0; i < m; ++i) {
            unsigned long long part1 = dist(rng);
            unsigned long long part2 = dist(rng);
            std::bitset<128> bs;
            for (int bit = 0; bit < 64; bit++) {
                if (part1 & (1ULL << bit))
                    bs.set(bit);
            }
            for (int bit = 64; bit < 128; bit++) {
                if (part2 & (1ULL << (bit - 64)))
                    bs.set(bit);
            }
            mat.push_back(bs);
        }
        // Messe nur die Zeit, die für das Lösen benötigt wird
        auto startSolve = std::chrono::high_resolution_clock::now();
        dummySum += solution_space_dimension_f2_small_homogen(mat, num_vars);
        auto endSolve = std::chrono::high_resolution_clock::now();
        totalSolveTimeMicro += std::chrono::duration_cast<std::chrono::microseconds>(endSolve - startSolve).count();
    }
    std::cout << "Solving time for " << numSystems << " systems: "
              << totalSolveTimeMicro << " microseconds. Dummy sum: " << dummySum << "\n";
    SUCCEED();
}

// GTest Speedtest: Erzeugt 1 Million zufälliger inhomogener Systeme (20 Gleichungen pro System)
// und misst ausschließlich die Zeit, die zum Lösen benötigt wird.
TEST(InhomogeneousGF2SpeedTest, MillionRandomSystems_SolvingOnly) {
    const int numSystems = 1000000;
    const int num_vars = 127;  // 127 Variablen, Bit 127 (Index 127) ist der rechte Seitenwert.
    const int m = 20;          // 20 Gleichungen pro System

    std::mt19937 rng(12345);  // Fester Seed für Reproduzierbarkeit
    std::uniform_int_distribution<unsigned long long> dist(0, std::numeric_limits<unsigned long long>::max());

    int dummySum = 0;
    long long totalSolveTimeMicro = 0;

    for (int sys = 0; sys < numSystems; ++sys) {
        std::vector<std::bitset<128>> mat;
        mat.reserve(m);
        // Erzeuge das zufällige System (diese Zeit wird nicht gemessen)
        for (int i = 0; i < m; ++i) {
            std::bitset<128> bs;
            unsigned long long part1 = dist(rng);
            unsigned long long part2 = dist(rng);
            for (int bit = 0; bit < 64; ++bit) {
                if (part1 & (1ULL << bit))
                    bs.set(bit);
            }
            for (int bit = 64; bit < 128; ++bit) {
                if (part2 & (1ULL << (bit - 64)))
                    bs.set(bit);
            }
            mat.push_back(bs);
        }
        // Messe ausschließlich die Zeit für das Lösen des Systems
        auto startSolve = std::chrono::high_resolution_clock::now();
        dummySum += solution_space_dimension_f2_small_inhom(mat, num_vars);
        auto endSolve = std::chrono::high_resolution_clock::now();
        totalSolveTimeMicro += std::chrono::duration_cast<std::chrono::microseconds>(endSolve - startSolve).count();
    }
    std::cout << "Solving time for " << numSystems
              << " inhomogeneous systems: " << totalSolveTimeMicro
              << " microseconds. Dummy sum: " << dummySum << "\n";
    SUCCEED();
}




// Testfall 1: Leere Matrix => keine Gleichungen, daher ist der Lösungsraum ℤ₂^(num_vars)
TEST(SolutionSpaceDimensionTest, EmptyMatrix) {
    std::vector<std::bitset<128>> mat;
    const int num_vars = 5;
    // Erwartete Dimension: 5
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(mat, num_vars), 5);
}

// Testfall 2: Eine Gleichung, die x0 = 0 erzwingt
TEST(SolutionSpaceDimensionTest, SingleEquation) {
    std::vector<std::bitset<128>> mat;
    std::bitset<128> eq;
    eq.reset();
    eq.set(0, true); // entspricht x0 = 0
    mat.push_back(eq);
    const int num_vars = 5;
    // Rang = 1, also Dimension = 5 - 1 = 4
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(mat, num_vars), 4);
}

// Testfall 3: Zwei linear unabhängige Gleichungen (z.B. x0+x1=0 und x1+x2=0)
TEST(SolutionSpaceDimensionTest, TwoIndependentEquations) {
    std::vector<std::bitset<128>> mat;
    std::bitset<128> eq1, eq2;
    eq1.reset();
    eq2.reset();
    eq1.set(0, true); eq1.set(1, true); // x0 + x1 = 0
    eq2.set(1, true); eq2.set(2, true); // x1 + x2 = 0
    mat.push_back(eq1);
    mat.push_back(eq2);
    const int num_vars = 5;
    // Rang = 2, also Dimension = 5 - 2 = 3
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(mat, num_vars), 3);
}

// Testfall 4: Zwei identische Gleichungen (linear abhängig)
TEST(SolutionSpaceDimensionTest, DuplicateEquations) {
    std::vector<std::bitset<128>> mat;
    std::bitset<128> eq;
    eq.reset();
    eq.set(0, true); eq.set(1, true); // x0 + x1 = 0
    mat.push_back(eq);
    mat.push_back(eq); // identische Zeile
    const int num_vars = 5;
    // Rang = 1, also Dimension = 5 - 1 = 4
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(mat, num_vars), 4);
}

// Testfall 5: Vollständiges System in 3 Variablen (x0=0, x1=0, x2=0)
TEST(SolutionSpaceDimensionTest, FullSystem) {
    std::vector<std::bitset<128>> mat;
    const int num_vars = 3;
    for (int i = 0; i < num_vars; ++i) {
        std::bitset<128> eq;
        eq.reset();
        eq.set(i, true);
        mat.push_back(eq);
    }
    // Rang = 3, also Dimension = 3 - 3 = 0
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(mat, num_vars), 0);
}

// Testfall 6: Eine Nullzeile (alle Bits 0) bei 4 Variablen
TEST(SolutionSpaceDimensionTest, ZeroRow) {
    std::vector<std::bitset<128>> mat;
    std::bitset<128> eq;
    eq.reset(); // Nullzeile
    mat.push_back(eq);
    const int num_vars = 4;
    // Nullzeile trägt nicht zum Rang bei, Dimension = 4
    EXPECT_EQ(solution_space_dimension_f2_small_homogen(mat, num_vars), 4);
}
