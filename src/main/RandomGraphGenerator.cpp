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

Graph RandomGraphGenerator::generateRandomConnectedGraphNoDoubleColorNeighbors() const {
    // Diese Methode setzt voraus, dass der Graph farbig ist.
    if (!colored) {
        throw std::invalid_argument("Diese Methode erfordert einen farbigen Graphen.");
    }

    // Prüfe die Eingabeparameter
    if (edges < vertices - 1) {
        throw std::invalid_argument("Die Anzahl der Kanten muss mindestens Anzahl der Knoten - 1 sein.");
    }
    if (edges > vertices * (vertices - 1) / 2) {
        throw std::invalid_argument("Die Anzahl der Kanten darf maximal (n choose 2) betragen.");
    }
    if (vertices < 2) {
        throw std::invalid_argument("Die Anzahl der Knoten muss mindestens 2 betragen.");
    }

    // Erzeuge den Graphen und füge die Knoten hinzu
    Graph G(colored);
    if (surjectivecoloring) { // Hier wird 'colors' ignoriert
        for (int i = 0; i < vertices; i++) {
            G.addNode(Node(i));
        }
    } else {
        for (int i = 0; i < vertices; i++) {
            if (i < colors && allcolorsneeded) {
                G.addNode(Node(i));
            } else {
                G.addNode(Node(getRandomNumberBetween(0, colors - 1)));
            }
        }
    }


    // Lambda-Funktion zur Prüfung, ob das Hinzufügen der Kante (u,v) einen Knoten dazu bringt,
    // zwei Nachbarn gleicher Farbe zu haben.
    // Wir greifen hier direkt auf das nodes-Array zu (z. B. G.nodes[v].getColor()).
    auto checkEdgeValidity = [&G, this](int u, int v) -> bool {
        int colorV = G.nodes[v].color;
        // Für u: Iteriere über alle Knoten und prüfe, ob es eine Kante (u,j) gibt,
        // bei der der Knoten j die gleiche Farbe wie v hat.
        for (int j = 0; j < vertices; j++) {
            if (G.isEdgebySet(u, j)) {
                if (G.nodes[j].color == colorV) {
                    return false;
                }
            }
        }
        int colorU = G.nodes[u].color;
        // Für v: Iteriere über alle Knoten und prüfe, ob es eine Kante (v,j) gibt,
        // bei der der Knoten j die gleiche Farbe wie u hat.
        for (int j = 0; j < vertices; j++) {
            if (G.isEdgebySet(v, j)) {
                if (G.nodes[j].color == colorU) {
                    return false;
                }
            }
        }
        return true;
    };

    // --- Erzeuge einen zufälligen Spannbaum unter Einhaltung der Farbbeschränkung ---
    vector<int> notyetadded;
    notyetadded.reserve(vertices);
    for (int i = 0; i < vertices; i++) {
        notyetadded.push_back(i);
    }

    vector<int> added;
    added.reserve(vertices);

    // Wähle einen zufälligen Startknoten:
    int startIndex = getRandomNumberBetween(0, vertices - 1);
    added.push_back(notyetadded[startIndex]);
    notyetadded.erase(notyetadded.begin() + startIndex);

    // Solange noch nicht alle Knoten im Baum sind:
    while (!notyetadded.empty()) {
        vector<std::pair<int, int>> candidateEdges;
        // Suche alle gültigen Kanten zwischen einem bereits hinzugefügten Knoten und einem noch nicht
        // hinzugefügten Knoten, die die Farbbeschränkung erfüllen.
        for (int u : added) {
            for (int v : notyetadded) {
                if (checkEdgeValidity(u, v)) {
                    candidateEdges.push_back({u, v});
                }
            }
        }
        if (candidateEdges.empty()) {
            throw std::runtime_error("Kein gültiges Kantenpaar gefunden, um den Spannbaum ohne Farbkonflikt zu erweitern.");
        }
        // Wähle zufällig eines der möglichen Kantenpaare:
        int randIndex = getRandomNumberBetween(0, candidateEdges.size() - 1);
        int u = candidateEdges[randIndex].first;
        int v = candidateEdges[randIndex].second;
        G.addEdge(u, v);
        added.push_back(v);
        auto it = std::find(notyetadded.begin(), notyetadded.end(), v);
        if (it != notyetadded.end()) {
            notyetadded.erase(it);
        }
    }

    // --- Füge die restlichen Kanten hinzu ---
    int remainingEdges = edges - (vertices - 1);
    for (int i = 0; i < remainingEdges; i++) {
        bool edgeAdded = false;
        const int maxAttempts = 1000;  // Vermeide Endlosschleifen
        for (int attempt = 0; attempt < maxAttempts; attempt++) {
            int u = getRandomNumberBetween(0, vertices - 1);
            int v = getRandomNumberBetween(0, vertices - 1);
            if (u == v) continue;                // Selbstschleifen nicht zulassen
            if (G.isEdgebySet(u, v)) continue;     // Vermeide doppelte Kanten
            if (!checkEdgeValidity(u, v)) continue; // Achte auf die Farbbeschränkung
            G.addEdge(u, v);
            edgeAdded = true;
            break;
        }
        if (!edgeAdded) {
            throw std::runtime_error("Zusätzliche Kante konnte nicht ohne Verletzung der Farbbeschränkung hinzugefügt werden.");
        }
    }

    return G;
}
