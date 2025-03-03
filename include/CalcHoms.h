//
// Created by falco on 02.03.25.
//

#ifndef CALCHOMS_H
#define CALCHOMS_H
#include "Graph.h"


class CalcHoms {
    static long long calcNumHoms(Graph& H, Graph& G);
    static long long calcNumHomsCFI(const Graph& H, const Graph& S, const int* mapping);
};


#endif //CALCHOMS_H
