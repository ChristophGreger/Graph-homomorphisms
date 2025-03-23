//
// Created by Christoph Greger on 03.03.25.
//

#ifndef LINEAR_EQUATIONS_F2_SMALL_H
#define LINEAR_EQUATIONS_F2_SMALL_H

#include <bitset>
#include <vector>

#define MAX_LSOE_BITSET 256

struct LinearSystemOfEquations {
    std::vector<std::bitset<MAX_LSOE_BITSET>> matrix;
    int columns;
};

// Calculates the dimension of the solution space of a homogeneous linear system of equations in GF(2)
// mat: Vector of bitset representations of the equations (each row is an equation)
// num_vars: Number of variables (maximum 128)
// Return value: Dimension of the solution space = num_vars - rank of the matrix
// The unused bits (because num_vars < 128) are not considered.
// The used variables must be the first num_vars bits.
inline int solution_space_dimension_f2_small_homogen(LinearSystemOfEquations lsoe) {
    auto& [mat, columns] = lsoe;
    int m = static_cast<int>(mat.size());
    int rank = 0;
    // iterates over all columns (variables) and performs elimination
    for (int col = 0; col < columns && rank < m; ++col) {
        // Search for a pivot in the current column
        int pivot = rank;
        while (pivot < m && !mat[pivot].test(col))
            ++pivot;
        if (pivot == m)
            continue; // no pivot found in this row
        // switch the pivot row with the current row
        std::swap(mat[rank], mat[pivot]);
        // Elimination: Remove the bit in column col from all following rows
        for (int i = rank + 1; i < m; ++i) {
            if (mat[i].test(col))
                mat[i] ^= mat[rank]; // XOR is addition in GF(2)
        }
        ++rank;
    }
    // The dimension of the solution space is the number of variables minus the rank of the matrix
    return columns - rank;
}


// Calculates the dimension of the solution space of an inhomogeneous linear GF(2) system,
// represented by a vector of std::bitset<128>.
// Assumption: Bits 0 to num_vars-1 contain the coefficients of the variables,
// Bit num_vars contains the right-hand side value (b).
// num_vars should therefore be at most 127.
// Return value: Dimension of the solution space (if consistent) or -1 if the system is inconsistent.
inline int solution_space_dimension_f2_small_inhomogen(LinearSystemOfEquations lsoe) {
    auto& [mat,columns] = lsoe;
    const int m = static_cast<int>(mat.size());
    int rank = 0;

    for (int col = 0; col < columns && rank < m; ++col) {
        int pivot = rank;
        while (pivot < m && !mat[pivot].test(col))
            ++pivot;
        if (pivot == m)
            continue;

        std::swap(mat[rank], mat[pivot]);

        for (int i = 0; i < m; ++i) {
            if (i != rank && mat[i].test(col))
                mat[i] ^= mat[rank];
        }
        ++rank;
    }

    std::bitset<MAX_LSOE_BITSET> var_mask;
    for (int i = 0; i < columns; ++i)
        var_mask.set(i);

    for (int i = rank; i < m; ++i) {
        if ((mat[i] & var_mask).none() && mat[i].test(columns))
            return -1; // Inconsistent
    }

    return columns - rank;
}

#endif //LINEAR_EQUATIONS_F2_SMALL_H
