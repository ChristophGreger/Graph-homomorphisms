//
// Created by Christoph Greger on 27.12.24.
//

#include "utilities.h"

#include <random>
#include <iostream>
#include <algorithm>

int getRandomNumberBetween(int first, int second) { //Inclusive both values
    // Create a random number generator
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister RNG

    // Define the distribution for the range [1, n]
    std::uniform_int_distribution<> distrib(first, second);

    // Generate and return the random number
    return distrib(gen);
}


int intPow(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

void printVector(const std::vector<int>& vec) {
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

// Function definition for sorting a vector of pairs by the second element
template<typename T1, typename T2>
std::vector<std::pair<T1, T2>> sortBySecond(const std::vector<std::pair<T1, T2>>& vec) {
    // Create a copy of the input vector
    std::vector<std::pair<T1, T2>> sortedVec = vec;

    // Sort the copied vector by the second element
    std::sort(sortedVec.begin(), sortedVec.end(), [](const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
        return a.second < b.second;
    });

    return sortedVec; // Return the sorted vector
}

// Explicit template instantiation (needed for non-inline templates)
template std::vector<std::pair<int, int>> sortBySecond(const std::vector<std::pair<int, int>>& vec);
template std::vector<std::pair<double, double>> sortBySecond(const std::vector<std::pair<double, double>>& vec);
// Add more instantiations as needed