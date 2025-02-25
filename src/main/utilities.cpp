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
    // Iteriere über alle Spalten
    for (int col = 0; col < cols && rank < rows; col++) {
        // Finde in der aktuellen Spalte einen Pivot (Zeile mit 1)
        int pivotRow = -1;
        for (int r = rank; r < rows; r++) {
            if (matrix[r * cols + col] == 1) {
                pivotRow = r;
                break;
            }
        }
        // Falls kein Pivot gefunden wurde, fahre mit der nächsten Spalte fort.
        if (pivotRow == -1) {
            continue;
        }
        // Falls der Pivot nicht in der aktuellen "Pivotzeile" steht, tausche die Zeilen.
        if (pivotRow != rank) {
            for (int j = 0; j < cols; j++) {
                unsigned char temp = matrix[rank * cols + j];
                matrix[rank * cols + j] = matrix[pivotRow * cols + j];
                matrix[pivotRow * cols + j] = temp;
            }
        }
        // Nutze den Pivot, um alle anderen Zeilen in der Spalte zu eliminieren.
        for (int r = 0; r < rows; r++) {
            // Überspringe den Pivot selbst.
            if (r == rank)
                continue;
            if (matrix[r * cols + col] == 1) {
                // Eliminiere den Eintrag in Spalte 'col' durch XOR (Addition in GF(2))
                for (int j = col; j < cols; j++) {
                    matrix[r * cols + j] ^= matrix[rank * cols + j];
                }
            }
        }
        // Erhöhe den Rang (eine weitere unabhängige Zeile gefunden)
        rank++;
    }
    // Dimension des Lösungsraums: Anzahl Unbekannte - Rang
    return cols - rank;
}

void printMatrix(int rows, int columns, unsigned char *matrix) {
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << (int)matrix[i * columns + j] << " ";
        }
        cout << endl;
    }
}

