//
// Created by Christoph Greger on 25.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_GRAPH_H
#define GRAPH_HOMOMORPHISMS_GRAPH_H

#include <vector>
#include "Node.h"
#include <unordered_set>
#include "utilities.h"

using namespace std;

class Graph {
private:
    int * adjMatrix;
    int numVertices;
    unordered_set<pair<int, int>, PairHash> edges;
public:
    vector<Node> nodes;
    Graph();
    void addNode(Node node);
    void addEdge(int node1, int node2);
    void printGraph();
    void calculateAdjMatrix();
};


#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
