//
// Created by Christoph Greger on 25.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_GRAPH_H
#define GRAPH_HOMOMORPHISMS_GRAPH_H

#include <vector>
#include "Node.h"
#include <unordered_set>

#include "GraphTemplate.h"
#include "utilities.h"

using namespace std;

class Graph {

    // Calculate the adjacency matrix of the graph, has to be called before using the adjacency matrix
    void calculateAdjMatrix();

    // Calculate the edge array of the graph, has to be called before using the edge array
    void calculateEdgeArray();

    void sortEdges();

    void calcNodeIndex();

    void calcNeighbours() const;

    void calcDegree() const;

public:

    const bool colored;
    const unordered_set<pair<int, int>, PairHash> edges;
    const vector<Node> nodes;

    //calculated in the constructor
    const pair<int, int> * edgeArray;
    const char * adjMatrix;
    const int numVertices;

    const int* nodeIndex;
    const vector<vector<int>> neighbours;
    const vector<int> degrees;

    explicit Graph(const GraphTemplate& t);
    ~Graph();
    void printGraph(bool printcolors = false);

    bool isEdge(int node1, int node2) const; // adjacency matrix has to be calculated before using this function

    bool isEdgebySet(int node1, int node2) const; // adjacency matrix has NOT to be calculated before using this function

    long long calculateNumberofHomomorphismsTo(Graph &H);

    long long calculateNumberofInjectiveHomomorphismsTo(Graph &H);

    long long calculateNumberofAutomorphismsWithoutColoring();

    long long calculateNumberofAutomorphismsWithColoring();

    long long calculateNumberofSubGraphsTo(Graph &H);

    bool isConnected() const;

    pair<bool, Graph> shrinkGraph(Graph &S);

    long long calculateNumberofhomomorphismsTo_CFI_from(Graph &S);

    std::string toString() const;

    std::string canonicalString() const;

    vector<Graph> connectedComponents();
};


#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
