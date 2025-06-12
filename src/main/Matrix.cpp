#include "Matrix.h"
#include <stdexcept>
#include <iostream>
#include <thread>
#include <fstream>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <atomic>

Matrix::Matrix(std::size_t dim) : n_(dim) {
    if (n_ > 0) data_ = new Rational[n_ * n_]();
}

Matrix::Matrix(const long long* values, std::size_t dim) : n_(dim) {
    if (!values) throw std::invalid_argument("values pointer is null");
    if (n_ == 0) throw std::invalid_argument("dimension zero");
    data_ = new Rational[n_ * n_];
    for (std::size_t i = 0; i < n_ * n_; ++i)
        data_[i] = Rational(BigInt(values[i]));
}

Matrix::Matrix(const Matrix& other) : n_(other.n_) {
    if (n_ > 0) {
        data_ = new Rational[n_ * n_];
        std::copy(other.data_, other.data_ + n_ * n_, data_);
    }
}

Matrix::Matrix(Matrix&& other) noexcept : n_(other.n_), data_(other.data_) {
    other.n_ = 0;
    other.data_ = nullptr;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        Matrix tmp(other);
        std::swap(n_, tmp.n_);
        std::swap(data_, tmp.data_);
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        n_ = other.n_;
        data_ = other.data_;
        other.n_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}

Matrix::~Matrix() {
    delete[] data_;
}

Matrix::Rational& Matrix::operator()(std::size_t row, std::size_t col) {
    if (row >= n_ || col >= n_) throw std::out_of_range("Matrix index out of range");
    return data_[index(row, col)];
}

const Matrix::Rational& Matrix::operator()(std::size_t row, std::size_t col) const {
    if (row >= n_ || col >= n_) throw std::out_of_range("Matrix index out of range");
    return data_[index(row, col)];
}


Matrix Matrix::invertLowerTriangular() const {
    if (n_ == 0) return Matrix(0);

    Matrix inv(n_);
    const unsigned int numThreads = 10;

    boost::asio::thread_pool pool(numThreads);

    std::atomic<int> done{0};

    //For each column
    for (std::size_t column = 0; column < n_; ++column) {
        const Rational& diag = (*this)(column, column);
        if (diag == 0) throw std::runtime_error("Matrix is singular (zero on diagonal)");

        //Add the task to the thread pool
        boost::asio::post(pool, [this, &inv, column, &diag, &done]() {
            // Diagonalelement im Thread setzen
            inv(column, column) = Rational(1) / diag;

            for (std::size_t row = column + 1; row < n_; ++row) {
                Rational sum = 0;
                for (std::size_t k = column + 1; k < row; ++k) {
                    sum += (*this)(row, k) * inv(k, column);
                }
                inv(row, column) = -sum / (*this)(row, row);
            }
            // Update the progress
            ++done;
            std::cout << "Finished column: " << column << " (" << done.load() << "/" << n_ << ")" << std::endl;

        });
    }

    //Wait for all tasks to finish
    pool.join();

    return inv;
}


void Matrix::writeToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Datei konnte nicht geöffnet werden.");
    }

    // Schreibe die Dimension
    file << n_ << "\n";

    // Schreibe die Matrixelemente
    for (std::size_t i = 0; i < n_; ++i) {
        for (std::size_t j = 0; j < n_; ++j) {
            file << (*this)(i, j).numerator() << "/" << (*this)(i, j).denominator() << " ";
        }
        file << "\n";
    }

    file.close();
}

Matrix Matrix::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Datei konnte nicht geöffnet werden.");
    }

    // Lese die Dimension
    std::size_t dim;
    file >> dim;

    Matrix matrix(dim);

    // Lese die Matrixelemente
    for (std::size_t i = 0; i < dim; ++i) {
        for (std::size_t j = 0; j < dim; ++j) {
            std::string fraction;
            file >> fraction;

            // Bruch in Zähler und Nenner aufteilen
            auto pos = fraction.find('/');
            if (pos == std::string::npos) {
                throw std::runtime_error("Ungültiges Format in der Datei.");
            }

            BigInt numerator(fraction.substr(0, pos));
            BigInt denominator(fraction.substr(pos + 1));

            matrix(i, j) = Rational(numerator, denominator);
        }
    }

    file.close();
    return matrix;
}
