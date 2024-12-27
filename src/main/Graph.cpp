//
// Created by Christoph Greger on 25.12.24.
//

#include "Graph.h"
#include "HomomorphismRange.h"
#include "InjectiveHomomorphismRange.h"

#include <iostream>
#include <stack>



Graph::Graph(bool colored) : colored(colored) {
    numVertices = 0;
    adjMatrix = nullptr;
    nodes = vector<Node>();
    edges = unordered_set<pair<int, int>, PairHash>();
    edgeArray = nullptr;
}

void Graph::addNode(const Node& node) {
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

void Graph::calculateEdgeArray() {
    edgeArray = new pair<int, int>[edges.size()];
    int i = 0;
    for (auto edge : edges) {
        edgeArray[i] = edge;
        i++;
    }
}

Graph::~Graph() {
    delete[] adjMatrix;
    delete[] edgeArray;
}

bool Graph::isEdge(int node1, int node2) const {
    return adjMatrix[node1 * numVertices + node2] == 1;
}

bool Graph::isEdgebySet(int node1, int node2) const {
    if (node2 < node1) {
        swap(node1, node2);
    }
    return edges.find(make_pair(node1, node2)) != edges.end();
}

int Graph::calculateNumberofHomomorphismsTo(Graph &H) {
    H.calculateAdjMatrix();
    calculateEdgeArray();

    int numHomomorphisms = 0;
    for (const vector<int>& hom : HomomorphismRange(numVertices, H.numVertices)) {
        bool isHomomorphism = true;

        if (colored && H.colored) {
            for (int i = 0; i < numVertices; i++) {
                if (!H.nodes[hom[i]].equals(nodes[i])) {
                    isHomomorphism = false;
                    break;
                }
            }
        }

        if (!isHomomorphism) {
            continue;
        }

        for (int i = 0; i < edges.size(); i++) {
            if (!H.isEdge(hom[edgeArray[i].first], hom[edgeArray[i].second])) {
                isHomomorphism = false;
                break;
            }
        }

        if (isHomomorphism) {
            numHomomorphisms++;
        }
    }
    return numHomomorphisms;
}

bool Graph::isConnected() const {
    if (numVertices == 0) return true;

    std::unordered_set<int> visited;
    std::stack<int> stack;
    stack.push(0); // Start DFS from the first node

    while (!stack.empty()) {
        int node = stack.top();
        stack.pop();

        if (visited.find(node) == visited.end()) {
            visited.insert(node);

            // Add all adjacent nodes to the stack
            for (int i = 0; i < numVertices; ++i) {
                if (isEdgebySet(node, i) && visited.find(i) == visited.end()) {
                    stack.push(i);
                }
            }
        }
    }

    return visited.size() == numVertices;
}

//Just a copy of the calculateNumberofHomomorphismsTo function, but with the InjectiveHomomorphismRange
int Graph::calculateNumberofInjectiveHomomorphismsTo(Graph &H) {
    H.calculateAdjMatrix();
    calculateEdgeArray();

    int numHomomorphisms = 0;
    for (const vector<int>& hom : InjectiveHomomorphismRange(numVertices, H.numVertices)) {
        bool isHomomorphism = true;

        if (colored && H.colored) {
            for (int i = 0; i < numVertices; i++) {
                if (!H.nodes[hom[i]].equals(nodes[i])) {
                    isHomomorphism = false;
                    break;
                }
            }
        }

        if (!isHomomorphism) {
            continue;
        }

        for (int i = 0; i < edges.size(); i++) {
            if (!H.isEdge(hom[edgeArray[i].first], hom[edgeArray[i].second])) {
                isHomomorphism = false;
                break;
            }
        }

        if (isHomomorphism) {
            numHomomorphisms++;
        }
    }
    return numHomomorphisms;
}

int Graph::calculateNumberofAutomorphismsWithoutColoring() {
    bool oldcolored = colored;
    colored = false;
    int numAutomorphisms = calculateNumberofHomomorphismsTo(*this);
    colored = oldcolored;
    return numAutomorphisms;
}

int Graph::calculateNumberofAutomorphismsWithColoring() {
    bool oldcolored = colored;
    colored = true;
    int numAutomorphisms = calculateNumberofHomomorphismsTo(*this);
    colored = oldcolored;
    return numAutomorphisms;
}

int Graph::calculateNumberofSubGraphsTo(Graph &H) {
    if (colored && H.colored) {
        return calculateNumberofInjectiveHomomorphismsTo(H) / calculateNumberofAutomorphismsWithColoring();
    }
    return calculateNumberofInjectiveHomomorphismsTo(H) / calculateNumberofAutomorphismsWithoutColoring();
}









