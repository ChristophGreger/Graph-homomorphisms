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

    void calculateAdjMatrix();

    void calculateEdgeArray();

    void sortEdges();

    void calcNodeIndex();

    void calcNeighbours();

    void calcDegree();

public:

    //given as parameter from the GraphTemplate
    bool colored;
    unordered_set<pair<int, int>, PairHash> edges;
    vector<Node> nodes;

    //calculated in the constructor
    pair<int, int> * edgeArray{};
    char * adjMatrix{};
    int numVertices;

    int* nodeIndex{};
    vector<vector<int>> neighbours;
    vector<int> degree;

    explicit Graph(const GraphTemplate& t);
    Graph(const Graph& g);
    ~Graph();
    void printGraph(bool printcolors = false) const;

    bool isEdge(int node1, int node2) const;

    bool isConnected() const;

    pair<bool, Graph> shrinkGraph(Graph &S) const;

    std::string toString() const;

    std::string canonicalString() const;

    vector<Graph> connectedComponents() const;
};


#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
