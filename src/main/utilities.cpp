//
// Created by Christoph Greger on 27.12.24.
//

#include "utilities.h"

#include <random>
#include <iostream>
#include <algorithm>

using namespace std;

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

int getSolutionDimension(int rows, int cols, unsigned char *matrix) {
    int rank = 0;
    // iterate over all columns
    for (int col = 0; col < cols && rank < rows; col++) {
        // find a pivot in the current column
        int pivotRow = -1;
        for (int r = rank; r < rows; r++) {
            if (matrix[r * cols + col] == 1) {
                pivotRow = r;
                break;
            }
        }
        // if no pivot was found, continue with the next column
        if (pivotRow == -1) {
            continue;
        }
        // if the pivot is not in the current "pivot row", swap the rows
        if (pivotRow != rank) {
            for (int j = 0; j < cols; j++) {
                unsigned char temp = matrix[rank * cols + j];
                matrix[rank * cols + j] = matrix[pivotRow * cols + j];
                matrix[pivotRow * cols + j] = temp;
            }
        }
        // eliminate all other rows in the column using the pivot
        for (int r = 0; r < rows; r++) {
            // skip the pivot itself
            if (r == rank)
                continue;
            if (matrix[r * cols + col] == 1) {
                // Eliminate the entry in column 'col' by XOR (addition in GF(2))
                for (int j = col; j < cols; j++) {
                    matrix[r * cols + j] ^= matrix[rank * cols + j];
                }
            }
        }
        // increment the rank (another independent row found)
        rank++;
    }
    // dimension of the solution space: number of unknowns - rank
    return cols - rank;
}

void printMatrix(const int rows, const int columns, const unsigned char *matrix) {
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << (int)matrix[i * columns + j] << " ";
        }
        cout << endl;
    }
}

long long powlong(long long base, int exponent) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

// fast exponentiation for int256_t (Exponentiation by Squaring)
int256_t int256_pow(int256_t base, int exponent) {
    if (exponent < 0) {
        throw std::runtime_error("Negative exponent not supported for int256_t");
    }
    int256_t result = 1;
    if (base == 0) {
        return 0;
    }
    while (exponent > 0) {
        if (exponent & 1) {  // if the exponent is odd
            if (result > std::numeric_limits<int256_t>::max() / base) {
                throw std::overflow_error("Overflow detected during multiplication");
            }
            result *= base;
        }
        if (base > std::numeric_limits<int256_t>::max() / base) {
            throw std::overflow_error("Overflow detected during squaring");
        }
        base *= base;  // quadrate the base
        exponent >>= 1; // divide the exponent by 2
    }
    return result;
}
