#ifndef GRAPH_HOMOMORPHISMS_BRUTEFORCEALGO_H
#define GRAPH_HOMOMORPHISMS_BRUTEFORCEALGO_H

#include "CFIGraph.h"
#include "Graph.h"
#include "BitArray.h"

using namespace std;

long long calculateNumberofHomomorphisms(CFIGraph* G, Graph* H);

long long callNext(CFIGraph* CFI, Graph* H, int index, CFINode* hom);

#endif