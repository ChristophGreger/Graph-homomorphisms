//
// Created by Christoph Greger on 03.03.25.
//

#ifndef LINEAR_EQUATIONS_F2_SMALL_H
#define LINEAR_EQUATIONS_F2_SMALL_H

#include <bitset>
#include <vector>

struct LinearSystemOfEquations {
    vector<bitset<128>> matrix;
    int columns;
    bitset<128> skipColumn = bitset<128>();
    int numVars = 0;
};

// Berechnet die Dimension des Lösungsraumes eines homogenen linearen Gleichungssystems in GF(2)
// mat: Vektor von bitset-Repräsentationen der Gleichungen (jede Zeile ist eine Gleichung)
// num_vars: Anzahl der Variablen (maximal 128)
// Rückgabewert: Dimension des Lösungsraumes = num_vars - Rang der Matrix
//Die nicht genutzen bit (weil num_vars < 128) werden nicht berücksichtigt.
//Die benutzen variablen müssen die ersten num_vars bit sein.
inline int solution_space_dimension_f2_small_homogen(LinearSystemOfEquations lsoe) {
    auto& [mat, columns, skipColumn, numVars] = lsoe;
    int m = static_cast<int>(mat.size());
    int rank = 0;
    // Iteriere über alle Spalten (Variablen) und führe Eliminierung durch
    for (int col = 0; col < columns && rank < m; ++col) {
        if (skipColumn[col]) {
            continue;
        }
        // Suche die Zeile mit einem Pivot in der aktuellen Spalte
        int pivot = rank;
        while (pivot < m && !mat[pivot].test(col))
            ++pivot;
        if (pivot == m)
            continue; // Kein Pivot in dieser Spalte gefunden
        // Tausche die gefundene Pivotzeile in die aktuelle Zeile
        std::swap(mat[rank], mat[pivot]);
        // Eliminierung: Entferne in allen folgenden Zeilen das Bit in Spalte col
        for (int i = rank + 1; i < m; ++i) {
            if (mat[i].test(col))
                mat[i] ^= mat[rank]; // XOR entspricht der Addition in GF(2)
        }
        ++rank;
    }
    // Die Dimension des Lösungsraumes ist (Anzahl der Variablen - Rang)
    return numVars - rank;
}


// Berechnet die Dimension des Lösungsraumes eines inhomogenen linearen GF(2)-Systems,
// dargestellt durch einen Vektor von std::bitset<128>.
// Annahme: Bits 0 bis num_vars-1 enthalten die Koeffizienten der Variablen,
// Bit num_vars enthält den rechten Seitenwert (b).
// num_vars sollte daher maximal 127 betragen.
// Rückgabewert: Dimension des Lösungsraumes (bei Konsistenz) oder -1, falls das System inkonsistent ist.
inline int solution_space_dimension_f2_small_inhomogen(LinearSystemOfEquations lsoe) {
    auto& [mat,columns,skipColumn,numVars] = lsoe;
    const int m = static_cast<int>(mat.size());
    int rank = 0;

    for (int col = 0; col < columns && rank < m; ++col) {
        if (skipColumn[col]) {continue;}
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

    std::bitset<128> var_mask;
    for (int i = 0; i < columns; ++i)
        var_mask.set(i);

    for (int i = rank; i < m; ++i) {
        if ((mat[i] & var_mask).none() && mat[i].test(columns))
            return -1; // Inkonsistent
    }

    return numVars - rank;
}

#endif //LINEAR_EQUATIONS_F2_SMALL_H
