//
// Created by Christoph Greger on 25.12.24.
//

#include "Graph.h"
#include <iostream>
#include <stack>
#include "utilities.h"
#include "NextInjection.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <algorithm>

#include "Nauty_wrapper.h"
#include <iomanip>
#include <cmath>

Graph::Graph(const GraphTemplate& t) {
    colored = t.colored;
    edges = t.edges;
    nodes = t.nodes;
    numVertices = nodes.size();

    calculateAdjMatrix();
    calculateEdgeArray();

    calcNodeIndex();
    calcNeighbours();
    calcDegree();
    sortEdges();
}

//preprocessing and private
void Graph::calculateAdjMatrix() {

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
    edgeArray = new pair<int, int>[edges.size()];
    int i = 0;
    for (auto edge : edges) {
        edgeArray[i] = edge;
        i++;
    }
}

//stores at the index i the index to which exklusively all the edges in the sorted edgearray do not contain a node bigger than i
void Graph::calcNodeIndex() {
    nodeIndex = new int[numVertices];
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
}

void Graph::calcNeighbours() {
    for (int i = 0; i < numVertices; i++) {
        vector<int> n;
        for (int j = 0; j < numVertices; j++) {
            if (isEdge(i, j)) {
                n.push_back(j);
            }
        }
        neighbours.push_back(n);
    }
}

void Graph::calcDegree() {
    for (int i = 0; i < numVertices; i++) {
        int d = 0;
        for (int j = 0; j < numVertices; j++) {
            if (isEdge(i, j)) {
                d++;
            }
        }
        degree.push_back(d);
    }
}

void Graph::sortEdges() {
    sortBySecond(edgeArray, edges.size());
}



Graph::~Graph() {
    delete[] adjMatrix;
    delete[] edgeArray;
    delete[] nodeIndex;
}

void Graph::printGraph(bool printcolors) const{
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

bool Graph::isEdge(int node1, int node2) const {
    int index = node1*numVertices+node2;
    if (index >= numVertices*numVertices) {
        throw std::runtime_error("isEdge is out of bounds");
    }
    return adjMatrix[index] == 1;
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
                if (isEdge(node, i) && visited.find(i) == visited.end()) {
                    stack.push(i);
                }
            }
        }
    }

    return visited.size() == numVertices;
}



pair<bool, Graph> Graph::shrinkGraph(Graph &S) const{

    int todelete = -1;
    int tomatch = -1;

    auto degS = S.degree;
    auto deg = degree;
    auto neighborsthis = neighbours;

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

    GraphTemplate t = GraphTemplate(true);

    if (todelete < tomatch) {
        swap(todelete, tomatch);
    }

    for (int i = 0; i < numVertices; i++) {
        if (i == todelete) {
            continue;
        }
        t.addNode(nodes[i]);
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
        t.addEdge(first, second);
    }

    Graph newGraph = Graph(t);

    return make_pair(true, newGraph);
}

// Methode, die den Graphen als String darstellt
std::string Graph::toString() const {
    std::stringstream ss;
    ss << "Graph with " << numVertices << " vertices and " << edges.size() << " edges\n";
    if (colored) {
        ss << "Colors:\n";
        for (int i = 0; i < numVertices; i++) {
            ss << i << " -> " << nodes[i].color << "\n";
        }
        ss << "End colors\n";
    }
    ss << "Edges:\n";
    std::vector<std::pair<int, int>> sortedEdges(edges.begin(), edges.end());
    std::sort(sortedEdges.begin(), sortedEdges.end());
    for (const auto &edge : sortedEdges) {
        ss << edge.first << " -> " << edge.second << "\n";
    }
    return ss.str();
}

// Beispielimplementierung von Graph::canonicalString() mit nauty
// Nur für Graphen mit weniger als 20 Knoten geeignet !!!!
std::string Graph::canonicalString() const {

#define set nauty_set //Wirklich kein Plan warum es das hier braucht

    int maxn = 20;
    int maxm = 1;
    graph g[maxm*maxn];
    int lab[maxn], ptn[maxn], orbits[maxn];
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;

    int n,m,v;

    options.getcanon = TRUE;

    n = numVertices;

    m = 1;

    nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);

    EMPTYGRAPH(g,m,n);
    for (auto &e : edges) {
        ADDONEEDGE(g, e.first, e.second, m);
    }

    graph canon[maxm*maxn];
    EMPTYGRAPH(canon, m, n);

    densenauty(g, lab, ptn, orbits, &options, &stats, m, n, canon);

    std::ostringstream oss;
    // Optional: stelle sicher, dass der Stream im hexadezimalen Format arbeitet.
    oss << std::hex;
    for (int i = 0; i < maxm*numVertices; i++) {
        // setfill und setw sorgen für führende Nullen, sodass jedes setword als 16-stellige hexadezimale Zahl ausgegeben wird.
        oss << std::setfill('0') << std::setw(16) << canon[i];
    }
    return oss.str();
}

vector<Graph> Graph::connectedComponents() const{
    vector<Graph> components;
    vector visited(numVertices, false);
    const vector<vector<int>> allNeighbors = neighbours;

    for (int i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            vector<int> comp;
            stack<int> s;
            s.push(i);
            visited[i] = true;

            // DFS to collect vertices in the component
            while (!s.empty()) {
                int v = s.top();
                s.pop();
                comp.push_back(v);
                for (int nb : allNeighbors[v]) {
                    if (!visited[nb]) {
                        visited[nb] = true;
                        s.push(nb);
                    }
                }
            }

            // Create new component Graph and map old indices to new ones.
            GraphTemplate t = GraphTemplate(colored);

            unordered_map<int, int> mapping;
            for (size_t j = 0; j < comp.size(); j++) {
                mapping[comp[j]] = j;
                t.addNode(nodes[comp[j]]);
            }
            // Add only edges internal to the component.
            for (auto [fst, snd] : edges) {
                int u = fst, v = snd;
                if (mapping.contains(u) && mapping.contains(v)) {
                    t.addEdge(mapping[u], mapping[v]);
                }
            }
            components.emplace_back(Graph(t));
        }
    }
    return components;
}
