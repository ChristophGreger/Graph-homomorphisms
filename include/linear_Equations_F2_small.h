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

// Berechnet die Dimension des Lösungsraumes eines homogenen linearen Gleichungssystems in GF(2)
// mat: Vektor von bitset-Repräsentationen der Gleichungen (jede Zeile ist eine Gleichung)
// num_vars: Anzahl der Variablen (maximal 128)
// Rückgabewert: Dimension des Lösungsraumes = num_vars - Rang der Matrix
//Die nicht genutzen bit (weil num_vars < 128) werden nicht berücksichtigt.
//Die benutzen variablen müssen die ersten num_vars bit sein.
inline int solution_space_dimension_f2_small_homogen(LinearSystemOfEquations lsoe) {
    auto& [mat, columns] = lsoe;
    int m = static_cast<int>(mat.size());
    int rank = 0;
    // Iteriere über alle Spalten (Variablen) und führe Eliminierung durch
    for (int col = 0; col < columns && rank < m; ++col) {
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
    return columns - rank;
}


// Berechnet die Dimension des Lösungsraumes eines inhomogenen linearen GF(2)-Systems,
// dargestellt durch einen Vektor von std::bitset<128>.
// Annahme: Bits 0 bis num_vars-1 enthalten die Koeffizienten der Variablen,
// Bit num_vars enthält den rechten Seitenwert (b).
// num_vars sollte daher maximal 127 betragen.
// Rückgabewert: Dimension des Lösungsraumes (bei Konsistenz) oder -1, falls das System inkonsistent ist.
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
            return -1; // Inkonsistent
    }

    return columns - rank;
}

#endif //LINEAR_EQUATIONS_F2_SMALL_H
