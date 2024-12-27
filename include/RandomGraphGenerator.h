//
// Created by Christoph Greger on 27.12.24.
//

#ifndef GITIGNORE_RANDOMGRAPHGENERATOR_H
#define GITIGNORE_RANDOMGRAPHGENERATOR_H


#include "Graph.h"

class RandomGraphGenerator {
public:
    int vertices;
    int edges;
    bool colored;
    bool surjectivecoloring;
    int colors;
    //nodes: number of vertices
    //edges: number of edges
    //colored: if the graph should be colored
    //surjectivecoloring: if the coloring should be surjective (not two nodes with same color)
    //colors: number of colors (from 0 to colors exclusive)
    RandomGraphGenerator(int vertices, int edges, bool colored = false, bool surjectivecoloring = true, int colors = 0);
    Graph generateRandomConnectedGraph() const;

};


#endif //GITIGNORE_RANDOMGRAPHGENERATOR_H
