#include "Matrix.h"

#include <iostream>

// Standardkonstruktor: Leere Matrix
Matrix::Matrix() {
    // Leere Daten
}

// Konstruktor mit Zeilen- und Spaltenangabe
Matrix::Matrix(size_t rows, size_t cols) {
    data.resize(rows, std::vector<Rational>(cols, 0));
}

// Konstruktor aus einem Vektor von Vektoren
Matrix::Matrix(const std::vector<std::vector<Rational>> &data) : data(data) {}

// Gibt die Anzahl der Zeilen zurück
size_t Matrix::numRows() const {
    return data.size();
}

// Gibt die Anzahl der Spalten zurück (bei leerer Matrix 0)
size_t Matrix::numCols() const {
    return data.empty() ? 0 : data[0].size();
}

// Zugriff auf Zeile i (const Version)
const std::vector<Rational>& Matrix::operator[](size_t index) const {
    return data[index];
}

// Zugriff auf Zeile i (nicht-konstante Version)
std::vector<Rational>& Matrix::operator[](size_t index) {
    return data[index];
}

// Matrixmultiplikation
Matrix Matrix::multiply(const Matrix &other) const {
    if (numCols() != other.numRows()) {
        throw std::invalid_argument("Dimensionen passen nicht zur Multiplikation.");
    }
    size_t m = numRows();
    size_t n = numCols();
    size_t p = other.numCols();

    Matrix result(m, p);
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < p; ++j) {
            Rational sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += data[i][k] * other[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

// Überprüft, ob die Matrix eine Einheitsmatrix ist
bool Matrix::isIdentity() const {
    if (numRows() != numCols()) {
        return false;
    }
    size_t n = numRows();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            // Diagonalelemente müssen 1 sein, alle anderen 0
            if (i == j) {
                if (data[i][j] != 1) return false;
            } else {
                if (data[i][j] != 0) return false;
            }
        }
    }
    return true;
}

Matrix Matrix::invertLowerTriangular() const {
    size_t n = numRows();
    if (n != numCols()) {
        throw std::invalid_argument("Matrix muss quadratisch sein.");
    }

    Matrix inv(n, n);

    long long counter = 0;

    for (size_t i = 0; i < n; ++i) {
        // Überprüfen, ob das Diagonalelement ungleich 0 ist
        if (data[i][i] == 0) {
            throw std::invalid_argument("Matrix ist singulär (Diagonalelement ist 0).");
        }

        std::cout << "Zeile : " << i << std::endl;

        inv[i][i] = 1 / data[i][i];

        for (size_t j = 0; j < i; ++j) {
            Rational sum = 0;
            // Direkt nur über bekannte Werte der inversen Matrix iterieren.
            for (size_t k = j; k < i; ++k) {
                sum += data[i][k] * inv[k][j];
            }
            inv[i][j] = -sum / data[i][i];
            std::cout << ++counter << std::endl;
        }
    }

    return inv;
}

// Erzeugt eine zufällige untere Dreiecksmatrix mit natürlichen Zahlen
Matrix Matrix::generateRandomLowerTriangular(size_t n, mp::cpp_int min, mp::cpp_int max) {
    Matrix result(n, n);
    // Initialisiere den Zufallsgenerator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dist((min - 1).convert_to<long long>(), max.convert_to<long long>());

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (j > i) {
                // Alle Einträge oberhalb der Diagonalen = 0
                result[i][j] = 0;
            } else {
                // Erzeuge einen zufälligen natürlichen Zahleneintrag
                // Für die Diagonalelemente sollte man sicherstellen, dass sie ungleich 0 sind
                long long randVal = dist(gen);
                if (i == j && randVal == 0) {
                    randVal = 1; // Sicherstellen, dass das Diagonalelement nicht 0 ist
                }
                result[i][j] = Rational(randVal);
            }
        }
    }
    return result;
}