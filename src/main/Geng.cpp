//
// Created by Christoph Greger on 16.04.25.
//

#include "../../include/Geng.h"

#include <boost/process.hpp>
#include <string>
#include "GraphTemplate.h"

/// Ruft ./geng mit n Knoten und minEdges:maxEdges auf,
/// parst jede graph6‑Zeile und gibt alle Graphen zurück.
std::vector<Graph> Geng::generateGraphs(int n, int minEdges, int maxEdges) {
    namespace bp = boost::process;
    // Stream für die Ausgabe von geng
    bp::ipstream pipe_out;

    // Argument für die Kantenzahl
    std::string range = std::to_string(minEdges) + ":" + std::to_string(maxEdges);

    // Prozess starten
    bp::child gen("../external/nauty/geng",
                  std::to_string(n),
                  range,
                  bp::std_out > pipe_out);

    std::vector<Graph> result;
    std::string line;
    // Zeilenweise einlesen
    while (std::getline(pipe_out, line)) {
        // Header-/Footer‑Zeilen überspringen
        if (line.empty() || line[0] == '>' || line[0] == 'Z')
            continue;

        // Erzeuge Template für unbunten Graphen
        GraphTemplate gt(false);

        // Anzahl Knoten aus dem ersten Zeichen (char = 63 + n)
        int numNodes = static_cast<int>(line[0]) - 63;
        // Knoten hinzufügen (Node hat einen Default-Konstruktor)
        for (int i = 0; i < numNodes; ++i) {
            gt.addNode(Node());
        }

        // Gesamtzahl der zu lesenden Bits
        int totalBits = numNodes * (numNodes - 1) / 2;
        int bitIndex = 0;

        // Durchlaufe die obere Dreiecksmatrix (i>j)
        for (int i = 1; i < numNodes && bitIndex < totalBits; ++i) {
            for (int j = 0; j < i && bitIndex < totalBits; ++j) {
                // Welches Zeichen und welches Bit in diesem Zeichen?
                int charPos = 1 + bitIndex / 6;
                int bitPos  = 5 - (bitIndex % 6);

                if (charPos < static_cast<int>(line.size())) {
                    int sixBits = static_cast<int>(line[charPos]) - 63;
                    bool isEdge = ((sixBits >> bitPos) & 0x1) != 0;
                    if (isEdge) {
                        gt.addEdge(j, i);
                    }
                }
                ++bitIndex;
            }
        }

        // Aus Template einen Graph bauen und speichern
        result.emplace_back(Graph(gt));
    }

    // Auf Prozessende warten
    gen.wait();
    return result;
}

std::vector<Graph> Geng::generateGraphs_without_isolated(int n, int minEdges, int maxEdges) {
    std::vector<Graph> graphs = Geng::generateGraphs(n, minEdges, maxEdges);
    std::vector<Graph> result;
    for (auto g : graphs) {
        result.push_back(g.withOutIsolatedNodes());
    }
    return result;
}

