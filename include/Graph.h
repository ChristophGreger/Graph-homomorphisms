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
public:
    int * adjMatrix;
    unordered_set<pair<int, int>, PairHash> edges;
    pair<int, int> * edgeArray;

    bool colored;
    vector<Node> nodes;
    explicit Graph(bool colored = false);
    ~Graph();
    void addNode(const Node& node);
    void addEdge(int node1, int node2);
    void printGraph();

    // Calculate the adjacency matrix of the graph, has to be called before using the adjacency matrix
    void calculateAdjMatrix();

    // Calculate the edge array of the graph, has to be called before using the edge array
    void calculateEdgeArray();

    bool isEdge(int node1, int node2) const; // adjacency matrix has to be calculated before using this function

    bool isEdgebySet(int node1, int node2) const; // adjacency matrix has NOT to be calculated before using this function

    int calculateNumberofHomomorphismsTo(Graph &H);

    //TODO: Refactoring von Node. Node sollte immer colored sein, aber der Graph entscheidet über das equals. Das ist besser
    int calculateNumberofInjectiveHomomorphismsTo(Graph &H);

    int calculateNumberofAutomorphismsWithoutColoring();

    int calculateNumberofAutomorphismsWithColoring();

    int calculateNumberofSubGraphsTo(Graph &H);

    int numVertices;

    bool isConnected() const;





};


#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
