//
// Created by falco on 02.03.25.
//

#ifndef CALCHOMS_H
#define CALCHOMS_H
#include "Graph.h"


class CalcHoms {
public:
    //Returns the exponent of the number of homomorphisms from H to S with the mapping as the homomorphism (with base 2)
    static int calcNumHomsCFI(const Graph& H, const Graph& S, const int* mapping);

    //returns the number of homs from H to CFI Graph of S mod3, (by trying every possible mapping) (works only for uncolored)
    static long long calcNumHomsCFI_uncolored(Graph& H, const Graph& S);

    //Returns the number of homomorphisms from H to the CFI graph of S, but mod3
    static long long calcNumInjectiveHomsCFI(const std::string &small_spasm_file_name, Graph& S);

    static long long calcNumHoms(const Graph& H, const Graph& G);
    static long long calcNumInjHoms(const Graph &H, const Graph &G);
};


#endif //CALCHOMS_H
