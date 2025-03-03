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
    char * adjMatrix;
    unordered_set<pair<int, int>, PairHash> edges;
    pair<int, int> * edgeArray;

    bool colored;
    vector<Node> nodes;
    explicit Graph(bool colored = false);
    ~Graph();
    void addNode(const Node& node);
    void addEdge(int node1, int node2);
    void printGraph(bool printcolors = false);

    // Calculate the adjacency matrix of the graph, has to be called before using the adjacency matrix
    void calculateAdjMatrix();

    // Calculate the edge array of the graph, has to be called before using the edge array
    void calculateEdgeArray();

    bool isEdge(int node1, int node2) const; // adjacency matrix has to be calculated before using this function

    bool isEdgebySet(int node1, int node2) const; // adjacency matrix has NOT to be calculated before using this function

    long long calculateNumberofHomomorphismsTo(Graph &H);

    long long calculateNumberofInjectiveHomomorphismsTo(Graph &H);

    long long calculateNumberofAutomorphismsWithoutColoring();

    long long calculateNumberofAutomorphismsWithColoring();

    long long calculateNumberofSubGraphsTo(Graph &H);

    int numVertices;

    bool isConnected() const;

    void sortEdges();

    int * calculateNodeIndex();

    vector<vector<int>> neighbors() const;

    vector<int> degree() const;

    pair<bool, Graph> shrinkGraph(Graph &S);

    long long calculateNumberofhomomorphismsTo_CFI_from(Graph &S);

    std::string toString() const;

    std::string canonicalString() const;

    void enumerateQuotientGraphs_K_Matching(const std::string &filename);
};


#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
