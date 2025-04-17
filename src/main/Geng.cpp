//
// Created by Christoph Greger on 16.04.25.
//

#include "../../include/Geng.h"

#include <boost/process.hpp>
#include <string>
#include "GraphTemplate.h"
#include <fstream>
#include "CalcHoms.h"

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

    // Sortieren nach der Summe von Knoten- und Kantenanzahl
    std::sort(result.begin(), result.end(), [](const Graph& a, const Graph& b) {
        size_t sumA = a.numVertices + a.edges.size();
        size_t sumB = b.numVertices + b.edges.size();
        return sumA < sumB;
    });

    return result;
}

void Geng::write_Components_Combinations_to_File(const std::string &filename, int n, int minEdges, int maxEdges) {
    std::unordered_map<std::string, Graph> componentMap;
    auto graphs = Geng::generateGraphs_without_isolated(n, minEdges, maxEdges);
    for (const auto& graph : graphs) {
        for (const auto& component : graph.connectedComponents()) {
            componentMap.insert({component.canonicalString_uncolored(), component});
        }
    }
    std::vector<pair<Graph, std::string>> componentVector;
    componentVector.reserve(componentMap.size());
    for (const auto& [key, value] : componentMap) {
        componentVector.emplace_back(value, key);
    }

    std::ofstream file(filename);
    file << n << " " << minEdges << ":" << maxEdges << "\n";

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    for (int i = 0; i < componentVector.size(); i++) {
        for (int j = 0; j < componentVector.size(); j++) {
            file << componentVector[i].second << " " << componentVector[j].second << " " << CalcHoms::calcNumHoms(componentVector[i].first, componentVector[j].first) << "\n";
            cout << i << " " << j << "\n";
        }
    }

    file.close();
}

struct Graph_Components {
    Graph graph;
    std::string canon_string;
    vector<std::string> components_canonicals;
};

void Geng::storeMatrix(const std::string &filename, const std::string &combination_filename, int n, int minEdges, int maxEdges) {
    vector<Graph_Components> Graphs;
    auto graphs = Geng::generateGraphs_without_isolated(n, minEdges, maxEdges);
    for (const auto& graph : graphs) {
        Graph_Components gc;
        gc.graph = graph;
        gc.canon_string = graph.canonicalString_uncolored();
        for (const auto& component : graph.connectedComponents()) {
            gc.components_canonicals.push_back(component.canonicalString_uncolored());
        }
        Graphs.push_back(gc);
    }

    std::ifstream combinations(combination_filename);

    if (!combinations.is_open()) {
        throw std::runtime_error("Cannot open file: " + combination_filename);
    }

    std::string first = "", second = "";
    int256_t count = 0;

    // Erste Zeile überspringen
    std::string skipLine;
    std::getline(combinations, skipLine);


    //Fill map with component combinations and their hom counts
    std::unordered_map<std::string, int256_t> combination_hom_count_map;

    cout << "Starting filling the combination_hom_count_map" << endl;

    while (combinations >> first >> second >> count) {
        combination_hom_count_map.insert({first + " " + second, count});
    }

    cout << "Filled combination_hom_count_map" << endl;

    std::ofstream file(filename);

    file << n << " " << minEdges << ":" << maxEdges << "\n";

    file << Graphs.size() << "\n";

    cout << "Writing " << Graphs.size() << " graphs to file" << endl;
    for (auto const &graph : Graphs) {
        file << graph.graph.string_uncolored_one_line() << "\n";
    }

    cout << "Writing the matrix!" << endl;
    int counter = 1;
    for (auto const &graph_from : Graphs) {
        cout << "Writing line " << counter << " of " << Graphs.size() << endl;
        counter++;
        for (auto const &graph_to : Graphs) {
            boost::multiprecision::cpp_int number = 0;
            for (auto const &component : graph_to.components_canonicals) {
                if (number == 0) {
                    number = combination_hom_count_map[graph_from.canon_string + " " + component];
                } else {
                    number *= combination_hom_count_map[graph_from.canon_string + " " + component];
                }
            }
            file << number << " ";
        }
        file << "\n";
    }
    file.close();
    combinations.close();
}




