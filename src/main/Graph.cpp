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

    delete [] adjMatrix;

    adjMatrix = new char[numVertices * numVertices];
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
    delete [] edgeArray;
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


//stores at the index i the index to which exklusively all the edges in the sorted edgearray do not contain a node bigger than i
int * Graph::calculateNodeIndex() {
    int *nodeIndex = new int[numVertices];
    sortEdges();
    int currnode = 0;
    int currnumber = 0;
    for (int edgeindex = 0; edgeindex < edges.size(); edgeindex++) {
        if (edgeArray[edgeindex].second == currnode) {
            ++currnumber;
        } else {
            int second = edgeArray[edgeindex].second;
            for (int i = currnode; i < second; i++) {
                nodeIndex[i] = currnumber;
            }
            currnode = second;
            ++currnumber;
        }
    }
    nodeIndex[currnode] = currnumber;
    return nodeIndex;
}

long long Graph::calculateNumberofHomomorphismsTo(Graph &H) {
    H.calculateAdjMatrix();

    int *nodeIndex = calculateNodeIndex();


    bool coloredhoms = colored && H.colored;


    long long numHomomorphisms = 0;

    //Initialize the homomorphism array with only 0s
    int* hom = new int[numVertices];
    for (int i = 0; i < numVertices; i++) {
        hom[i] = 0;
    }


    int currtochange = 1;
    bool increment = false; //If the current node has to be incremented, only false at the first iteration; increment = false means going right in the hom array

    if (!coloredhoms) {
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
                    ++currtochange;
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
                            break;
                        }
                    }
                }
                if (!foundani || currtochange == numVertices - 1) {
                    increment = true;
                    --currtochange;
                    continue;
                }
                ++currtochange;
                continue;
            }

        }
        delete[] hom;
        delete[] nodeIndex;
        return numHomomorphisms;
    } // ---------- COLORED BRANCH (MODIFIED) ---------- AI GENERATED (some parts)
    else {
        // Precompute valid mappings based on color
        // possibleMappings[v].first is an array of valid node-IDs in H
        // possibleMappings[v].second is how many valid node-IDs in that array

        pair<int*, int> * possibleMappings = new pair<int*, int>[numVertices];
        for (int v = 0; v < numVertices; v++) {
            vector<int> valid;
            for (int w = 0; w < H.numVertices; w++) {
                // If color matches, node w in H is a valid image of v
                if (H.nodes[w].equals(nodes[v])) {
                    valid.push_back(w);
                }
            }
            // Store them in the pair
            int *arr = new int[valid.size()];
            for (int i = 0; i < (int)valid.size(); i++) {
                arr[i] = valid[i];
            }
            possibleMappings[v] = make_pair(arr, (int)valid.size());
        }

        // candidateIndex[v] is the index in the array possibleMappings[v].first
        int* candidateIndex = new int[numVertices];

        // Initialize
        for (int v = 0; v < numVertices; v++) {
            hom[v] = -1;            // no candidate chosen
            candidateIndex[v] = 0;  // start with 0
        }

        currtochange = 0;

        // Start the backtracking:
        while (true) {

            if (currtochange == -1) {
                break; // done
            }

            if (increment) {
                // We have to move to the next candidate for hom[currtochange]
                bool foundNext = false;

                // Iterate over valid color-candidates
                auto &pm = possibleMappings[currtochange];
                for (int idx = candidateIndex[currtochange] + 1; idx < pm.second; idx++) {
                    int candidate = pm.first[idx];
                    // Check adjacency constraints
                    bool works = true;

                    if (currtochange == 0) {
                        hom[currtochange] = candidate;
                        candidateIndex[currtochange] = idx;
                        foundNext = true;
                        increment = false;
                        ++currtochange;
                        break;
                    }

                    for (int edgeindex = nodeIndex[currtochange - 1];
                         edgeindex < nodeIndex[currtochange];
                         edgeindex++)
                    {
                        if (!H.isEdge(hom[edgeArray[edgeindex].first], candidate)) {
                            works = false;
                            break;
                        }
                    }
                    if (works) {
                        hom[currtochange] = candidate;
                        candidateIndex[currtochange] = idx;
                        foundNext = true;
                        increment = false;
                        ++currtochange;
                        break;
                    }
                }

                if (!foundNext) {
                    // Need to backtrack further
                    increment = true;
                    --currtochange;
                }
            }
            else {
                // "Go right" fill from scratch
                bool foundFirst = false;
                auto &pm = possibleMappings[currtochange];
                for (int idx = 0; idx < pm.second; idx++) {
                    int candidate = pm.first[idx];
                    bool works = true;
                    // We only need to check edges whose 'second' is currtochange
                    if (currtochange != 0) {
                        for (int edgeindex = nodeIndex[currtochange - 1];
                             edgeindex < nodeIndex[currtochange];
                             edgeindex++)
                        {
                            if (!H.isEdge(hom[edgeArray[edgeindex].first], candidate)) {
                                works = false;
                                break;
                            }
                        }
                    }
                    if (works) {
                        hom[currtochange] = candidate;
                        candidateIndex[currtochange] = idx;
                        if (currtochange == numVertices - 1) {
                            // We assigned the last vertex => valid homomorphism found
                            ++numHomomorphisms;
                            continue;
                        } else {
                            foundFirst = true;
                            break;
                        }
                    }
                }
                if (!foundFirst || currtochange == numVertices - 1) {
                    increment = true;
                    --currtochange;
                } else {
                    ++currtochange;
                }
            }
        } // end while

        // Clean up
        if (possibleMappings) {
            for (int v = 0; v < numVertices; v++) {
                delete[] possibleMappings[v].first;
            }
            delete[] possibleMappings;
        }
        delete[] hom;
        delete[] nodeIndex;
        delete[] candidateIndex;
        return numHomomorphisms;
    }

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
long long Graph::calculateNumberofInjectiveHomomorphismsTo(Graph &H) {
    H.calculateAdjMatrix();
    calculateEdgeArray();

    if (H.numVertices < numVertices) {
        return 0;
    }

    long long numHomomorphisms = 0;
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

long long Graph::calculateNumberofAutomorphismsWithoutColoring() {
    bool oldcolored = colored;
    colored = false;
    long long numAutomorphisms = calculateNumberofHomomorphismsTo(*this);
    colored = oldcolored;
    return numAutomorphisms;
}

long long Graph::calculateNumberofAutomorphismsWithColoring() {
    bool oldcolored = colored;
    colored = true;
    long long numAutomorphisms = calculateNumberofHomomorphismsTo(*this);
    colored = oldcolored;
    return numAutomorphisms;
}

long long Graph::calculateNumberofSubGraphsTo(Graph &H) {
    if (colored && H.colored) {
        return calculateNumberofInjectiveHomomorphismsTo(H) / calculateNumberofAutomorphismsWithColoring();
    }
    return calculateNumberofInjectiveHomomorphismsTo(H) / calculateNumberofAutomorphismsWithoutColoring();
}


vector<vector<int>> Graph::neighbors() {
    vector<vector<int>> neighbors;
    for (int i = 0; i < numVertices; i++) {
        vector<int> n;
        for (int j = 0; j < numVertices; j++) {
            if (isEdgebySet(i, j)) {
                n.push_back(j);
            }
        }
        neighbors.push_back(n);
    }
    return neighbors;
}

vector<int> Graph::degree() {
    vector<int> deg;
    for (int i = 0; i < numVertices; i++) {
        int d = 0;
        for (int j = 0; j < numVertices; j++) {
            if (isEdgebySet(i, j)) {
                d++;
            }
        }
        deg.push_back(d);
    }
    return deg;
}

pair<bool, Graph> Graph::shrinkGraph(Graph &S) {

    int todelete = -1;
    int tomatch = -1;

    auto degS = S.degree();
    auto deg = degree();
    auto neighborsthis = neighbors();

    for (int i = 0; i < numVertices; i++) {

        if (deg[i] >= degS[nodes[i].color] - 1) {

            for (int neighbor1 : neighborsthis[i]) {
                for (int neighbor2 : neighborsthis[neighbor1]) {

                    //Check for same color
                    if (nodes[i].color != nodes[neighbor2].color) {
                        continue;
                    }

                    if (neighbor2 == i) {
                        continue;
                    }

                    //Now check if i and neighbor2 have enough neighbors with different color in common
                    vector<int> commonneighbors;
                    unordered_set<int> commoncolorsneighbors = unordered_set<int>();
                    for (int neighbor : neighborsthis[i]) {
                        if (neighborsthis[neighbor2].end() != find(neighborsthis[neighbor2].begin(), neighborsthis[neighbor2].end(), neighbor)) {
                            commonneighbors.push_back(neighbor);
                            commoncolorsneighbors.insert(nodes[neighbor].color);
                        }
                    }

                    if (commoncolorsneighbors.size() >= degS[nodes[i].color] - 1) {
                        todelete = i;
                        tomatch = neighbor2;
                        break;
                    }
                }
                if (todelete != -1) {
                    break;
                }
            }
            if (todelete != -1) {
                break;
            }
        }
    }

    if (todelete == -1) {
        return make_pair(false, *this);
    }

    Graph newGraph = Graph(true);

    if (todelete < tomatch) {
        swap(todelete, tomatch);
    }

    for (int i = 0; i < numVertices; i++) {
        if (i == todelete) {
            continue;
        }
        newGraph.addNode(nodes[i]);
    }


    cout << "Deleting " << todelete << " and matching it with " << tomatch << endl;
    for (auto edge : edges) {
        auto first = edge.first;
        auto second = edge.second;
        if (first == todelete) {
            first = tomatch;
        } else if (second == todelete) {
            second = tomatch;
        }
        if (first > todelete) {
            first--;
        }
        if (second > todelete) {
            second--;
        }
        newGraph.addEdge(first, second);
    }

    return make_pair(true, newGraph);
}


long long Graph::calculateNumberofhomomorphismsTo_CFI_from(Graph &S) {
    //Make sure both are colored
    if (!colored || !S.colored) {
        throw invalid_argument("Both graphs have to be colored for this function.");
    }

    //Make sure there is no color in this that is not in S
    //The colors in S are from 0 to S.numVertices - 1
    for (int i = 0; i < numVertices; i++) {
        if (nodes[i].color >= S.numVertices) {
            return 0;
        }
    }

    S.calculateAdjMatrix();

    //Make sure there is no edge between colors in this where there is no edge between this to colors in S
    for (auto edge : edges) {
        auto edgecolor1 = nodes[edge.first].color;
        auto edgecolor2 = nodes[edge.second].color;
        if (!S.isEdge(edgecolor1, edgecolor2)) {
            cout << "No edge between " << edgecolor1 << " and " << edgecolor2 << endl;
            return 0;
        }
    }


    //TODO: Implement with Gauss


    int exponent = 0;
    return powBase2(exponent);
}

















