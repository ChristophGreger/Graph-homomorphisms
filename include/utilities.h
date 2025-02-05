//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_UTILITIES_H
#define GRAPH_HOMOMORPHISMS_UTILITIES_H

#include <utility>    // For std::pair
#include <functional> // For std::hash

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

#include <vector>

template <typename T>
bool contains(const std::vector<T>& vec, const T& value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

///RandomNumberGenerator
/// @param first Inclusive
/// @param second Inclusive
/// @return A random number between first and second
int getRandomNumberBetween(int first, int second); //Inclusive both values


int intPow(int base, int exponent);

void printVector(const std::vector<int>& vec);

template<typename T1, typename T2>
///Sorts array by the second element of each pair
///@param arr The array to be sorted by the second element of each pair
///@param size The size of the array
void sortBySecond(std::pair<T1, T2>* arr, std::size_t size);

long long powBase2(int exponent);


/// Using Gauss Elimination to solve a system of linear equations
/// @param rows Number of rows in the matrix (number of equations)
/// @param cols Number of cols in the matrix (number of variables)
/// @param matrix The matrix to run Gauss on. Only 0 and 1 are allowed
/// @return The dimension of the solution space
int getSolutionDimension(int rows, int cols, char *matrix);


#endif //GRAPH_HOMOMORPHISMS_UTILITIES_H
