//
// Created by Christoph Greger on 26.12.24.
//

#include "CFIGraph.h"
#include "EvenSubsetIterator.h"
#include "utilities.h"
#include <iostream>

using namespace std;

CFIGraph::CFIGraph(Graph &G): G(G) {
    G.calculateAdjMatrix();
    G.calculateEdgeArray();
}