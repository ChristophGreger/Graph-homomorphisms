//
// Created by Christoph Greger on 27.12.24.
//

#include "utilities.h"

#include <random>

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

