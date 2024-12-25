//
// Created by Christoph Greger on 24.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_GRAPH_H
#define GRAPH_HOMOMORPHISMS_GRAPH_H

class Graph {
public:
    virtual int getNumberOfVertices() = 0; //The vertices have to be numbered from 1 to n
    virtual bool areEquivalent(int vertex1, int vertex2) = 0; //Returns true if the vertices are equivalent, for example have the same color
    virtual bool areAdjacent(int vertex1, int vertex2) = 0; //Returns true if the vertices are adjacent
};

#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
