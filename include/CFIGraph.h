//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_CFIGRAPH_H
#define GRAPH_HOMOMORPHISMS_CFIGRAPH_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "utilities.h"
#include "Graph.h"
#include "CFINode.h"

using namespace std;

//CFI Graphs are always colored!
class CFIGraph {
public:

    Graph& G;
    /// Creates the CFIGraph of a given graph G.
    /// @param G The Graph from which to create the CFIGraph; needs to be surjectively colored; needs to have at least 2 vertices if inverted is true;
    explicit CFIGraph(Graph &G); //Inverted invertes the edges between two groups of nodes after the construction. No need for specifying the two groups, because of the isomorphisms between them
};


#endif //GRAPH_HOMOMORPHISMS_CFIGRAPH_H
