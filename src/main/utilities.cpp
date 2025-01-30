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

long long powBase2(int exponent) {
    return 1LL << exponent;
}

void printVector(const std::vector<int>& vec) {
    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

template<typename T1, typename T2>
void sortBySecond(std::pair<T1, T2>* arr, std::size_t size)
{
    // Sort the array in place by the second element
    std::sort(arr, arr + size, [](const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
        return a.second < b.second;
    });
}

template void sortBySecond<int, int>(std::pair<int, int>* arr, std::size_t size);
template void sortBySecond<double, double>(std::pair<double, double>* arr, std::size_t size);
// Add more instantiations as needed