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
    Graph();
    ~Graph();
    void printGraph(bool printcolors = false) const;

    bool isEdge(int node1, int node2) const;

    bool isConnected() const;

    int getTreeWidth() const;

    pair<bool, Graph> shrinkGraph(Graph &S) const;

    std::string toString() const;

    std::string canonicalString() const;

    vector<Graph> connectedComponents() const;

    //copy assignment operator (copy-and-swap idiom)
    Graph& operator=(Graph other) { // pass by value: copy is made using deep copy constructor
        swap(*this, other);
        return *this;
    }

    friend void swap(Graph& first, Graph& second) noexcept {
        using std::swap;
        swap(first.colored, second.colored);
        swap(first.numVertices, second.numVertices);
        swap(first.nodes, second.nodes);
        swap(first.edges, second.edges);
        swap(first.adjMatrix, second.adjMatrix);
        swap(first.edgeArray, second.edgeArray);
        swap(first.nodeIndex, second.nodeIndex);
        swap(first.neighbours, second.neighbours);
        swap(first.degree, second.degree);
    }

    //deep copy constructor
    Graph(const Graph& other)
        : colored(other.colored),
          numVertices(other.numVertices),
          nodes(other.nodes),
          edges(other.edges),
          neighbours(other.neighbours),
          degree(other.degree)
    {
        //allocate and copy the adjacency matrix.
        adjMatrix = new char[numVertices * numVertices];
        std::copy(other.adjMatrix, other.adjMatrix + numVertices * numVertices, adjMatrix);

        //allocate and copy the edge array.
        int edgeCount = static_cast<int>(other.edges.size());
        edgeArray = new std::pair<int,int>[edgeCount];
        for (int i = 0; i < edgeCount; i++) {
            edgeArray[i] = other.edgeArray[i];
        }

        //allocate and copy the nodeIndex array.
        nodeIndex = new int[numVertices];
        std::copy(other.nodeIndex, other.nodeIndex + numVertices, nodeIndex);
    }
};


#endif //GRAPH_HOMOMORPHISMS_GRAPH_H
