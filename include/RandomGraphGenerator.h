//
// Created by Christoph Greger on 27.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_RANDOMGRAPHGENERATOR_H
#define GRAPH_HOMOMORPHISMS_RANDOMGRAPHGENERATOR_H


#include "Graph.h"

class RandomGraphGenerator {
public:
    int vertices;
    int edges;
    bool colored;
    bool surjectivecoloring;
    int colors;
    bool allcolorsneeded;

    ///Creates Instance to be able to generate Graphs with this afterwards
    /// @param vertices Number of vertices that the graph should have
    /// @param edges Number of edges that the graph should have
    /// @param colored True if the Graph should be colored, else False
    /// @param surjectivecoloring True if the Graph shouldn't have to vertices with the same color
    /// @param colors Nummber of colors (colors will range from 0 to colors-1)
    /// @param allcolorsneeded True if all colors should be used, else False
    RandomGraphGenerator(int vertices, int edges, bool colored = false, bool surjectivecoloring = true, int colors = 0, bool allcolorsneeded = true);

    /// RandomConnectedGraph
    /// @return A Graph that is connected and has the properties that were set in the constructor
    Graph generateRandomConnectedGraph() const;

    /// NoDoubleColorNeighbours
    /// @return A random connected graph in which no vertice has two neighbours with the same color and has the properties that were set in the constructor
    Graph generateRandomConnectedGraphNoDoubleColorNeighbors() const;

};


#endif //GRAPH_HOMOMORPHISMS_RANDOMGRAPHGENERATOR_H
