//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GITIGNORE_CFIGRAPH_H
#define GITIGNORE_CFIGRAPH_H

#include <vector>
#include <unordered_set>
#include "utilities.h"
#include "Graph.h"
#include "CFINode.h"

using namespace std;

//CFI Graphs are always colored!
class CFIGraph {
public:
    unordered_set<pair<int, int>, PairHash> edges;
    explicit CFIGraph(Graph &G);
    vector<CFINode> nodes;
    int numofVertices;
    int numofEdges;
    Graph toGraph();
};


#endif //GITIGNORE_CFIGRAPH_H
