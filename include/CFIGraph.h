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
    std::unordered_map<int,int> edgeMap;
    /// Creates the CFIGraph of a given graph G.
    /// @param G The Graph from which to create the CFIGraph; needs to be surjectively colored; needs to have at least 2 vertices if inverted is true;
    /// @param inverted
    explicit CFIGraph(Graph &G, bool invert=false); //Inverted invertes the edges between two groups of nodes after the construction. No need for specifying the two groups, because of the isomorphisms between them

    /// Convertion of the CFIGraph to a Graph.
    /// @return The Graph from the CFIGraph.
    Graph toGraph();

    bool isEdge(CFINode node1, CFINode node2);
    vector<int> calcEdgeRepresentation(unordered_set<pair<int, int>, PairHash> edges);

private:

    int getEdgeId(pair<int,int> edge);
};


#endif //GRAPH_HOMOMORPHISMS_CFIGRAPH_H
