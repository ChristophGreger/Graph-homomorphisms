//
// Created by Christoph Greger on 16.04.25.
//


#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include "Matrix.h"

using namespace std;

TEST(Matrix_inv_test, n_20) {
    Matrix M = Matrix::generateRandomLowerTriangular(20, 1, 1);
    cout << "Matrix generated" << endl;
    auto time = clock();
    Matrix inv = M.invertLowerTriangular();
    cout << "Time in ms: " << (clock() - time)/1000 << endl;
    ASSERT_EQ(M.multiply(inv).isIdentity(), true);
}