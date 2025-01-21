//
// Created by Christoph Greger on 25.12.24.
//

#include "Graph.h"
#include <iostream>
#include <stack>
#include "utilities.h"
#include "NextInjection.h"


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

void Graph::printGraph(bool printcolors) {
    cout << "Graph with " << numVertices << " vertices and " << edges.size() << " edges" << endl;

    if (printcolors) {
        cout << "Colors:" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << i << " -> " << nodes[i].color << endl;
        }
        cout << "End colors" << endl;
    }

    cout << "Edges:" << endl;

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


//TODO: Do not check Homomorphisms that can't be valid, For this sort the edges and check them before "advancing" to the next homomorphism
//TODO: Improve the performance for colored by only checking the homomorphisms that are valid for the right colors.
//At the moment only for uncolored
int Graph::calculateNumberofHomomorphismsTo(Graph &H) {
    H.calculateAdjMatrix();
    calculateEdgeArray();
    sortEdges();

    //Now we calculate an array that does the following: at index i it stores the index until which (exklusively) the nodes in the edge are less than i + 1
    //Probably this should be taken out to be a function later
    //Graph has to be connected for this to work !!!!!!!!! else: just crap coming out of this
    int *nodeIndex = new int[numVertices];

    int currnode = 0;
    int currnumber = 0;
    for (int edgeindex = 0; edgeindex < edges.size(); edgeindex++) {
        if (edgeArray[edgeindex].second == currnode) {
            ++currnumber;
        } else {
            nodeIndex[currnode] = currnumber;
            ++currnode;
        }
    }
    //end of calculation of nodeIndex


    bool coloredhoms = colored && H.colored;

    int numHomomorphisms = 0;
    int* hom = new int[numVertices];
    for (int i = 0; i < numVertices; i++) {
        hom[i] = 0;
    }


    int currtochange = 1;
    bool increment = false; //If the current node has to be incremented, only false at the first iteration; increment = false means going right in the hom array


    while (true) {

        if (currtochange == -1) {
            break; //All homomorphisms have been checked
        }

        if (increment) {
            if (hom[currtochange] == H.numVertices - 1) {
                increment = true;
                --currtochange;
                continue;
            }
            if (currtochange == 0) {
                ++hom[currtochange];
                increment = false;
                continue;
            }
            bool foundani = false;
            for (int i = hom[currtochange] + 1; i < H.numVertices; i++) {
                bool iworks = true;
                for (int edgeindex = nodeIndex[currtochange-1]; edgeindex < nodeIndex[currtochange]; edgeindex++) {
                    if (!H.isEdge(hom[edgeArray[edgeindex].first], i)) {
                        iworks = false;
                        break;
                    }
                }
                if (iworks) {
                    foundani = true;
                    hom[currtochange] = i;
                    increment = false;
                    ++currtochange;
                    break;
                }
            }
            if (!foundani) {
                increment = true;
                --currtochange;
            } else {
                continue;
            }
        }


        //If going right in the hom array
        if (!increment) {
            bool foundani = false;
            for (int i = 0; i < H.numVertices; i++) {
                bool iworks = true;
                for (int edgeindex = nodeIndex[currtochange-1]; edgeindex < nodeIndex[currtochange]; edgeindex++) {
                    if (!H.isEdge(hom[edgeArray[edgeindex].first], i)) {
                        iworks = false;
                        break;
                    }
                }
                if (iworks) {
                    foundani = true;
                    hom[currtochange] = i;
                    if (currtochange == numVertices - 1) {
                        ++numHomomorphisms;
                    } else {
                        ++currtochange;
                        break;
                    }
                }
            }
            if (!foundani) {
                increment = true;
                --currtochange;
            }
            if (currtochange == numVertices - 1) {
                increment = true;
                --currtochange;
            }
            continue;
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

void Graph::sortEdges() {
    calculateEdgeArray();
    sortBySecond(edgeArray, edges.size());
}


//TODO: Write tests for the following 4 functions. They are not tested yet.
int Graph::calculateNumberofInjectiveHomomorphismsTo(Graph &H) {
    H.calculateAdjMatrix();
    calculateEdgeArray();

    if (H.numVertices < numVertices) {
        return 0;
    }

    int numHomomorphisms = 0;
    auto nextInjection = NextInjection(numVertices, H.numVertices);
    const vector<int>& hom = nextInjection.current();



    bool alreadynexthom = true;
    bool isnohom = false;

    while (true) {

        isnohom = false;

        if (!alreadynexthom) {
            if (!nextInjection.next()) {
                break;
            }
        }

        alreadynexthom = false;

        if (colored && H.colored) {
            for (int i = 0; i < numVertices; i++) {
                if (!H.nodes[hom[i]].equals(nodes[i])) {
                    isnohom = true;
                    break;
                }
            }
            if (isnohom) {
                continue;
            }
        }

        for (int i = 0; i < edges.size(); i++) {
            if (!H.isEdge(hom[edgeArray[i].first], hom[edgeArray[i].second])) {
                isnohom = true;
                break;
            }
        }

        if (isnohom) {
            continue;
        }

        numHomomorphisms++;
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









