#ifndef MATRIX_H
#define MATRIX_H

#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <cstddef>
#include <algorithm>

/**
 * @brief Square matrix of boost::rational<cpp_int> values stored in a flat C array (row‑major).
 */
class Matrix {
public:
    using BigInt   = boost::multiprecision::cpp_int;
    using Rational = boost::rational<BigInt>;

    /**
     * @brief Construct an n×n zero matrix.
     */
    explicit Matrix(std::size_t dim = 0);

    /**
     * @brief Construct from a flat C array of long long values (row‑major).
     * @param values Pointer to the first element of a dim×dim long long array.
     * @param dim    Number of rows/columns (matrix is square).
     */
    Matrix(const long long* values, std::size_t dim);

    // Rule‑of‑five
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    ~Matrix();

    std::size_t size() const { return n_; }

    // Element access
    Rational&       operator()(std::size_t row, std::size_t col);
    const Rational& operator()(std::size_t row, std::size_t col) const;

    /**
     * @brief Invert a lower‑triangular matrix.
     * @throws std::runtime_error if the matrix is singular.
     */
    Matrix invertLowerTriangular() const;

    /**
     * @brief Schreibe die Matrix in eine Datei.
     * @param filename Der Name der Datei.
     */
    void writeToFile(const std::string& filename) const;

    /**
     * @brief Lese eine Matrix aus einer Datei.
     * @param filename Der Name der Datei.
     * @return Die gelesene Matrix.
     */
    static Matrix readFromFile(const std::string& filename);

private:
    std::size_t index(std::size_t row, std::size_t col) const { return row * n_ + col; }

    std::size_t n_    = 0;       ///< Dimension n
    Rational*   data_ = nullptr; ///< Flat C array (size n×n)
};

#endif // MATRIX_H