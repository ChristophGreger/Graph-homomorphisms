//
// Created by Christoph Greger on 27.12.24.
//

#include "RandomGraphGenerator.h"
#include <iostream>
#include "utilities.h"

RandomGraphGenerator::RandomGraphGenerator(int vertices, int edges, bool colored, bool surjectivecoloring, int colors, bool allcolorsneeded) : allcolorsneeded(allcolorsneeded), vertices(vertices), edges(edges), colored(colored), surjectivecoloring(surjectivecoloring), colors(colors) {}

Graph RandomGraphGenerator::generateRandomConnectedGraph() const {

    if (edges < vertices - 1) {
        throw std::invalid_argument("The number of edges has to be at least the number of vertices - 1");
    }

    if (edges > vertices * (vertices - 1) / 2) {
        throw std::invalid_argument("The number of edges has to be at most the number of vertices choose 2");
    }

    if (vertices < 2) {
        throw std::invalid_argument("The number of vertices has to be at least 2");
    }

    GraphTemplate t = GraphTemplate(colored);

    //Adding the nodes
    if (colored) {
        if (surjectivecoloring) { //colors is ignored in this case
            for (int i = 0; i < vertices; i++) {
                t.addNode(Node(i));
            }
        } else {
            for (int i = 0; i < vertices; i++) {
                if (i < colors && allcolorsneeded) {
                    t.addNode(Node(i));
                } else {
                    t.addNode(Node(getRandomNumberBetween(0, colors-1)));
                }
            }
        }
    } else {
        for (int i = 0; i < vertices; i++) {
            t.addNode(Node());
        }
    }

    //For the algorithm have a look here: https://doi.org/10.1145/237814.237880 and here: https://stackoverflow.com/questions/2041517/random-simple-connected-graph-generation-with-given-sparseness

    //Create a random spanning tree
    vector<int> notyetadded;
    notyetadded.reserve(vertices);
    for (int i = 0; i < vertices; i++) {
        notyetadded.push_back(i);
    }

    vector<int> added;
    added.reserve(vertices);

    int startnode = getRandomNumberBetween(0, vertices-1);
    added.push_back(startnode);
    notyetadded.erase(notyetadded.begin() + startnode);

    while (!notyetadded.empty()) {
        int randomnode = getRandomNumberBetween(0, added.size()-1);
        int randomnotyetadded = getRandomNumberBetween(0, notyetadded.size()-1);
        t.addEdge(added[randomnode], notyetadded[randomnotyetadded]);
        added.push_back(notyetadded[randomnotyetadded]);
        notyetadded.erase(notyetadded.begin() + randomnotyetadded);
    }

    //Add the remaining edges
    for (int i = 0; i < edges - vertices + 1; i++) {
        int node1 = getRandomNumberBetween(0, vertices-1);
        int node2 = getRandomNumberBetween(0, vertices-1);
        while (node1 == node2 || t.isEdge(node1, node2)) {
            node1 = getRandomNumberBetween(0, vertices-1);
            node2 = getRandomNumberBetween(0, vertices-1);
        }
        t.addEdge(node1, node2);
    }

    return Graph(t);
}

Graph RandomGraphGenerator::generateRandomConnectedGraphNoDoubleColorNeighbors() const {

    // this method assumes that the graph is colored
    if (!colored) {
        throw std::invalid_argument("This method needs a colored graph.");
    }

    // check the input parameters
    if (edges < vertices - 1) {
        throw std::invalid_argument("The number of edges has to be at least the number of vertices - 1");
    }
    if (edges > vertices * (vertices - 1) / 2) {
        throw std::invalid_argument("The number of edges can be at most (n choose 2)");
    }
    if (vertices < 2) {
        throw std::invalid_argument("The number of nodes must be at least 2.");
    }

    // create the graph and add the nodes
    GraphTemplate t(colored);
    if (surjectivecoloring) { // here colors is ignored
        for (int i = 0; i < vertices; i++) {
            t.addNode(Node(i));
        }
    } else {
        for (int i = 0; i < vertices; i++) {
            if (i < colors && allcolorsneeded) {
                t.addNode(Node(i));
            } else {
                t.addNode(Node(getRandomNumberBetween(0, colors - 1)));
            }
        }
    }

    // Lambda function to check if adding the edge (u,v) would cause a node to have two neighbors of the same color.
    // We directly access the nodes array here (e.g. t.nodes[v].getColor()).
    auto checkEdgeValidity = [&t, this](int u, int v) -> bool {
        int colorV = t.nodes[v].color;
        // For u: Iterate over all nodes and check if there is an edge (u,j) where node j has the same color as v.
        for (int j = 0; j < vertices; j++) {
            if (t.isEdge(u, j)) {
                if (t.nodes[j].color == colorV) {
                    return false;
                }
            }
        }
        int colorU = t.nodes[u].color;
        // For v: Iterate over all nodes and check if there is an edge (v,j) where node j has the same color as u.
        for (int j = 0; j < vertices; j++) {
            if (t.isEdge(v, j)) {
                if (t.nodes[j].color == colorU) {
                    return false;
                }
            }
        }
        return true;
    };

    // --- Create a random spanning tree while respecting the color constraint ---
    vector<int> notyetadded;
    notyetadded.reserve(vertices);
    for (int i = 0; i < vertices; i++) {
        notyetadded.push_back(i);
    }

    vector<int> added;
    added.reserve(vertices);

    // Chose a random starting node:
    int startIndex = getRandomNumberBetween(0, vertices - 1);
    added.push_back(notyetadded[startIndex]);
    notyetadded.erase(notyetadded.begin() + startIndex);

    // While not all edges are in the tree:
    while (!notyetadded.empty()) {
        vector<std::pair<int, int>> candidateEdges;
        // Search all valid edges between an already added node and a not yet added node that satisfy the color constraint.
        for (int u : added) {
            for (int v : notyetadded) {
                if (checkEdgeValidity(u, v)) {
                    candidateEdges.push_back({u, v});
                }
            }
        }
        if (candidateEdges.empty()) {
            throw std::runtime_error("No valid edge found to extend the spanning tree without color conflict.");
        }
        // Chose a random edge from the candidate edges:
        int randIndex = getRandomNumberBetween(0, candidateEdges.size() - 1);
        int u = candidateEdges[randIndex].first;
        int v = candidateEdges[randIndex].second;
        t.addEdge(u, v);
        added.push_back(v);
        auto it = std::find(notyetadded.begin(), notyetadded.end(), v);
        if (it != notyetadded.end()) {
            notyetadded.erase(it);
        }
    }

    // --- Add the remaining edges ---
    int remainingEdges = edges - (vertices - 1);
    for (int i = 0; i < remainingEdges; i++) {
        bool edgeAdded = false;
        const int maxAttempts = 1000;  // Prevent non termination
        for (int attempt = 0; attempt < maxAttempts; attempt++) {
            int u = getRandomNumberBetween(0, vertices - 1);
            int v = getRandomNumberBetween(0, vertices - 1);
            if (u == v) continue;                // prevent self loops
            if (t.isEdge(u, v)) continue;     // prevent double edges
            if (!checkEdgeValidity(u, v)) continue; // make sure the color constraint is not violated
            t.addEdge(u, v);
            edgeAdded = true;
            break;
        }
        if (!edgeAdded) {
            throw std::runtime_error("Could not add the remaining edges without violating the color constraint.");
        }
    }

    return Graph(t);
}
