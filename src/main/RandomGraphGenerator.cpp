//
// Created by Christoph Greger on 27.12.24.
//

#include "RandomGraphGenerator.h"

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

    Graph G = Graph(colored);

    //Adding the nodes
    if (colored) {
        if (surjectivecoloring) { //colors is ignored in this case
            for (int i = 0; i < vertices; i++) {
                G.addNode(Node(i));
            }
        } else {
            for (int i = 0; i < vertices; i++) {
                if (i < colors && allcolorsneeded) {
                    G.addNode(Node(i));
                } else {
                    G.addNode(Node(getRandomNumberBetween(0, colors-1)));
                }
            }
        }
    } else {
        for (int i = 0; i < vertices; i++) {
            G.addNode(Node());
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
        G.addEdge(added[randomnode], notyetadded[randomnotyetadded]);
        added.push_back(notyetadded[randomnotyetadded]);
        notyetadded.erase(notyetadded.begin() + randomnotyetadded);
    }

    //Add the remaining edges
    for (int i = 0; i < edges - vertices + 1; i++) {
        int node1 = getRandomNumberBetween(0, vertices-1);
        int node2 = getRandomNumberBetween(0, vertices-1);
        while (node1 == node2 || G.isEdgebySet(node1, node2)) {
            node1 = getRandomNumberBetween(0, vertices-1);
            node2 = getRandomNumberBetween(0, vertices-1);
        }
        G.addEdge(node1, node2);
    }

    return G;
}
