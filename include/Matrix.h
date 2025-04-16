#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>

namespace mp = boost::multiprecision;

// Definiere den Rationaltyp auf Basis von cpp_int
typedef boost::rational<mp::cpp_int> Rational;

class Matrix {
public:
    // Standardkonstruktoren:
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(const std::vector<std::vector<Rational>> &data);

    // Getter für die Dimensionen
    size_t numRows() const;
    size_t numCols() const;

    // Zugriff auf Zeile i (Lesen/Schreiben)
    const std::vector<Rational>& operator[](size_t index) const;
    std::vector<Rational>& operator[](size_t index);

    // Matrixoperationen
    Matrix multiply(const Matrix &other) const;
    bool isIdentity() const;

    // Gegeben eine untere Dreiecksmatrix: Berechne deren Inverses effizient.
    // Vorausgesetzt, die Matrix ist quadratisch, untere Dreiecksmatrix und alle Diagonalelemente ungleich 0.
    Matrix invertLowerTriangular() const;

    // Erzeuge eine zufällige untere Dreiecksmatrix der Größe n x n,
    // wobei die Einträge (auf der Diagonale und darunter) natürliche Zahlen (im Bereich [min, max]) sind.
    // Alle Elemente oberhalb der Diagonale werden auf 0 gesetzt.
    static Matrix generateRandomLowerTriangular(size_t n, mp::cpp_int min = 1, mp::cpp_int max = 10);

private:
    std::vector<std::vector<Rational>> data;
};

#endif // MATRIX_H