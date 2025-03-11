//
// Created by falco on 02.03.25.
//

#ifndef CALCHOMS_H
#define CALCHOMS_H
#include "Graph.h"
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::int256_t;


class CalcHoms {
public:
    //returns the exponent of the number of homomorphisms from H to S with the mapping as the homomorphism (with base 2)
    static int calcNumHomsCFI(const Graph& H, const Graph& S, const int* mapping, const bool inverted = false, const pair<int,int> &edge = {0,0});
    static int calcNumHomsCFI2(const Graph& H, const Graph& S, const int* mapping, const bool inverted = false, const pair<int,int> &edge = {0,0});

    //returns the number of homs from H to CFI Graph of S, (by trying every possible mapping) (works only for uncolored)
    static int256_t calcNumHomsCFI_uncolored(const Graph& H, const Graph& S, const bool inverted = false);

    static long long calcNumHoms(const Graph& H, const Graph& G);
    static long long calcNumInjHoms(const Graph &H, const Graph &G);
    static int256_t calcNumInjHoms(const std::string &spasm_file_name, const Graph &G, bool CFI_OF_G = false, bool CFI_inverted = false);
    static int256_t calcNumAutomorphisms(const Graph &G);
};

#endif //CALCHOMS_H