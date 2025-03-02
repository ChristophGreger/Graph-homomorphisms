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
            //cout << "No edge between " << edgecolor1 << " and " << edgecolor2 << endl;
            return 0;
        }
    }

    auto neighborsS = S.neighbors();
    auto degS = S.degree();

    //We want to create an possible mapping from (vertice, neighbor) pairs to an index in the matrix (column)
    //this vector mapps every node in this to <beginning, end>, which is the range of the neighbors in S
    //and so the range of the columns in the matrix which this node in affiliated with
    //(from inclusive, to exclusive)
    vector<pair<int, int>> indexMapping = vector<pair<int, int>>();
    indexMapping.reserve(numVertices);

    int columns = 0;
    for (int i = 0; i < numVertices; ++i) {
        indexMapping.emplace_back(columns, columns + degS[nodes[i].color]);
        columns += degS[nodes[i].color];
    }

    //Now columns is the number of columns in the matrix

    // Now calculate Number of rows, the matrix will have
    int rows = numVertices + edges.size();

    //Now we create the matrix
    auto * matrix = new unsigned char[rows * columns];

    //Initialize the matrix with 0s
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i * columns + j] = 0;
        }
    }

    //Fill with the even subset guarantee
    for (int i = 0; i < numVertices; i++) {
        for(int j = indexMapping[i].first; j < indexMapping[i].second; j++) {
            matrix[i * columns + j] = 1;
        }
    }

    int currentrow = numVertices;

    //Fill for the edges
    for (auto edge : edges) {
        int first = edge.first;
        int second = edge.second;
        int firstcolor = nodes[first].color;
        int secondcolor = nodes[second].color;
        //We have to find the index that the two nodes are affiliated with
        int firstindex = -1;
        int secondindex = -1;

        //Find firstindex
        for (int i = 0; i < degS[firstcolor]; ++i) {
            if (neighborsS[firstcolor][i] == secondcolor) {
                firstindex = i;
                break;
            }
        }
        //Find secondindex
        for (int i = 0; i < degS[secondcolor]; ++i) {
            if (neighborsS[secondcolor][i] == firstcolor) {
                secondindex = i;
                break;
            }
        }

        matrix[currentrow * columns + indexMapping[first].first + firstindex] = 1;
        matrix[currentrow * columns + indexMapping[second].first + secondindex] = 1;
        ++currentrow;
    }


    //printMatrix(rows, columns, matrix);

    //Now we can calculate the dimension of the solution space
    int exponent = getSolutionDimension(rows, columns, matrix);

    //printMatrix(rows, columns, matrix);

    delete [] matrix;
    return powBase2(exponent);
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
    for (int i = 0; i < m*n; i++) {
        // setfill und setw sorgen für führende Nullen, sodass jedes setword als 16-stellige hexadezimale Zahl ausgegeben wird.
        oss << std::setfill('0') << std::setw(16) << canon[i];
    }
    return oss.str();
}


// Diese Methode erzeugt alle Quotientgraphen des aktuellen Graphen (ohne Self-Loops) mittels Backtracking.
// Für jeden Quotientgraph wird die kanonische Form (als String) bestimmt und zusammen mit einer
// String-Repräsentation und der Häufigkeit in die angegebene Datei geschrieben.
// VORSICHT!!!!!!! IST SO OPTIMIERT DASS ES NUR FÜR K MATCHINGS FUNKTIONIERT!!!!
void Graph::enumerateQuotientGraphs_K_Matching(const std::string &filename) {

    long long total = 0;
    // Map: Schlüssel ist die kanonische Form (als String),
    // Wert ist ein Paar aus der String-Repräsentation des Quotientgraphen und der Zählung.
    std::unordered_map<std::string, std::pair<std::string, int>> canonicalMap;

    calculateEdgeArray();

    int numEdges = edges.size();

    // partition[i] gibt an, zu welchem Block der Knoten i gehört.
    std::vector<int> partition(numVertices, -1);
    partition[0] = 0;  // Der erste Knoten wird fest Block 0 zugeordnet

    // Rekursive Backtracking-Funktion:
    std::function<void(int, int)> backtrack = [&](int vertex, int currentMax) {
        if (vertex == numVertices) {
            // Erzeuge den Quotientgraphen anhand der aktuellen Partition.
            Graph quotient(false);
            for (int i = 0; i <= currentMax; i++) {
                quotient.addNode(Node());
            }
            // Für jede Kante im Originalgraphen, die zwei verschiedene Blöcke verbindet,
            // wird im Quotientgraphen eine entsprechende Kante eingefügt.
            for (int i = 0; i < numEdges; i++) {
                auto edge = edgeArray[i];
                int u = edge.first;
                int v = edge.second;
                int bu = partition[u];
                int bv = partition[v];
                quotient.addEdge(bu, bv);
            }
            // Berechne die kanonische Form des Quotientgraphen.

            std::string canon = quotient.canonicalString();

            // Hole die String-Repräsentation des Quotientgraphen.
            std::string quotientStr = quotient.toString();
            if (canonicalMap.find(canon) == canonicalMap.end()) {
                canonicalMap[canon] = std::make_pair(quotientStr, 1);
            } else {
                canonicalMap[canon].second++;
            }
            ++total;


            //DEBUGGING
            if (total % 1000000 == 0) {
                cout << "Total: " << total << endl;
            }
            return;
        }
        // Versuche, den aktuellen Knoten in jeden existierenden Block einzufügen.
        for (int block = 0; block <= currentMax; block++) {
            bool valid = true;
            // Überprüfe, ob das Hinzufügen in Block 'block' zu einer Self-Loop (Kante innerhalb des Blocks) führen würde.
            /**for (int v = 0; v < vertex; v++) {
                if (partition[v] == block) {
                    if (isEdgebySet(v, vertex)) {
                        valid = false;
                        break;
                    }
                }
            }
            **/
            //Hier jetzt auf K-Matching optimiert:
            if (vertex % 2 == 1) {
                if (partition[vertex - 1] == block) {
                    valid = false;
                }

            } else {
                if (partition[vertex + 1] == block) {
                    valid = false;
                }

            }

            //Ende K Matching Optimierung

            if (!valid) continue;
            partition[vertex] = block;
            backtrack(vertex + 1, currentMax);
        }
        // Eröffne einen neuen Block für den aktuellen Knoten.
        partition[vertex] = currentMax + 1;
        backtrack(vertex + 1, currentMax + 1);
    };

    backtrack(1, 0);

    // Schreibe die Ergebnisse in die Datei.
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << std::endl;
        return;
    }
    auto totaldifferent = canonicalMap.size();
    for (const auto &entry : canonicalMap) {
        outFile << "Quotientgraph:\n" << entry.second.first << "\n";
        outFile << "Kanonische Form:\n" << entry.first << "\n";
        outFile << "Anzahl: " << entry.second.second << "\n\n";
    }
    cout << "Total: " << total << endl;
    outFile.close();
    cout << "Total different: " << totaldifferent << endl;
}















