//
// Created by Christoph Greger on 25.12.24.
//

#include "Graph.h"

#include <iostream>



Graph::Graph() {
    numVertices = 0;
    adjMatrix = nullptr;
    nodes = vector<Node>();
    edges = unordered_set<pair<int, int>, PairHash>();
}

void Graph::addNode(Node node) {
    nodes.push_back(node);
    numVertices++;
}

void Graph::addEdge(int node1, int node2) {
    if (node1 < numVertices && node2 < numVertices) {
        if (node2 < node1) {
            swap(node1, node2);
        }
        edges.insert(make_pair(node1, node2));
    }
}

void Graph::printGraph() {
    cout << "Graph with " << numVertices << " vertices and " << edges.size() << " edges" << endl;

    // Copy edges to a vector
    vector<pair<int, int>> sortedEdges(edges.begin(), edges.end());

    // Sort the edges
    sort(sortedEdges.begin(), sortedEdges.end());

    // Print the sorted edges
    for (const auto& edge : sortedEdges) {
        cout << edge.first << " -> " << edge.second << endl;
    }
}

void Graph::calculateAdjMatrix() {
    adjMatrix = new int[numVertices * numVertices];
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            adjMatrix[i * numVertices + j] = 0;
        }
    }
    for (auto edge : edges) {
        adjMatrix[edge.first * numVertices + edge.second] = 1;
        adjMatrix[edge.second * numVertices + edge.first] = 1;
    }
}





