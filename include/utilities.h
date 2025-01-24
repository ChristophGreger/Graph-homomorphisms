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

int getRandomNumberBetween(int first, int second); //Inclusive both values


int intPow(int base, int exponent);

void printVector(const std::vector<int>& vec);

// Function declaration for sorting a vector of pairs by the second element
template<typename T1, typename T2>
void sortBySecond(std::pair<T1, T2>* arr, std::size_t size);


#endif //GRAPH_HOMOMORPHISMS_UTILITIES_H
