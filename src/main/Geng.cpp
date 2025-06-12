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

void Geng::storeMatrix_Hom(const std::string &filename, const std::string &combination_filename, int n, int minEdges, int maxEdges) {
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
            boost::multiprecision::cpp_int number = 1;
            for (auto const &component_from : graph_from.components_canonicals) {
                boost::multiprecision::cpp_int temp = 0;
                for (auto const &component_to : graph_to.components_canonicals) {
                    temp += combination_hom_count_map.at(component_from + " " + component_to);
                }
                number *= temp;
            }
            file << number << " ";
        }
        file << "\n";
    }
    file.close();
    combinations.close();
}

void Geng::storeMatrix(const std::string &filename, const Graph_Matrix &hom_matrix) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file << hom_matrix.numVertices_max << " " << hom_matrix.minEdges << ":" << hom_matrix.maxEdges << "\n";
    file << hom_matrix.numGraphs << "\n";
    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        file << hom_matrix.graphs[i].string_uncolored_one_line() << "\n";
    }
    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        for (int j = 0; j < hom_matrix.numGraphs; j++) {
            file << hom_matrix.matrix[i * hom_matrix.numGraphs + j] << " ";
        }
        file << "\n";
    }
    file.close();
}


Geng::Graph_Matrix Geng::readMatrix(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    auto hom_matrix = Graph_Matrix();

    std::string firstline;
    std::getline(file, firstline);
    std::istringstream iss(firstline);
    iss >> hom_matrix.numVertices_max;
    iss >> hom_matrix.minEdges;
    char c;
    iss >> c;
    iss >> hom_matrix.maxEdges;

    file >> hom_matrix.numGraphs;

    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        GraphTemplate gt(false);
        int numVertices, numEdges;
        file >> numVertices;
        file >> numEdges;
        for (int j = 0; j < numVertices; j++) {
            gt.addNode(Node());
        }
        for (int j = 0; j < numEdges; j++) {
            int from, to;
            file >> from;
            file >> to;
            gt.addEdge(from, to);
        }
        hom_matrix.graphs.emplace_back(Graph(gt));
    }

    hom_matrix.matrix = new long long[hom_matrix.numGraphs * hom_matrix.numGraphs];
    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        for (int j = 0; j < hom_matrix.numGraphs; j++) {
            file >> hom_matrix.matrix[i * hom_matrix.numGraphs + j];
        }
    }
    file.close();
    return hom_matrix;
}



void Geng::storeMatrix_Surj(const std::string &filename, const std::string &hom_matrix_file) {
    auto hom_matrix = readMatrix(hom_matrix_file);

    //First: construct canonical string --> index mapping
    std::unordered_map<std::string, int> canonical_index_map;
    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        canonical_index_map.insert({hom_matrix.graphs[i].canonicalString_uncolored(), i});
    }

    //For each graph construct every subgraph (by deleting edges, no isolated vertices)
    //Make list for each graph that contains the indices of the graph that are subgraphs, and how often they are subgraphs

    std::vector<std::vector<std::pair<int, int>>> surj_sub_index_times; //at position i is a vector of pairs of index and numbers. if i is the index of a graph than it contains the index and number of all the subgraphs of the graphs that has to be subtracted from its number of homomorphisms in order to get the number of surj ones.
    surj_sub_index_times.reserve(hom_matrix.numGraphs);
    //For this: Create all subgraphs of the graph by edge masks
    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        //cout << "Creating subgraphs for graph " << i << endl;
        auto graph = hom_matrix.graphs[i];
        int numEdges = graph.edges.size();
        std::unordered_map<int, int> subgraph_map; //Map from index of subgraph to number of it apearing
        int max = (1 << numEdges) - 1;
        for (int j = 1; j < max; j++) {
            int index = canonical_index_map[graph.getSubgraph(j).canonicalString_uncolored()];
            if (!subgraph_map.contains(index)) {
                subgraph_map.insert({index, 1});
            } else {
                subgraph_map[index]++;
            }
        }
        std::vector<std::pair<int, int>> subgraph_vector;
        for (auto const &pair : subgraph_map) {
            subgraph_vector.push_back(pair);
        }
        surj_sub_index_times.push_back(subgraph_vector);
    }

    int matrixsize = hom_matrix.numGraphs * hom_matrix.numGraphs;
    long long * surjmatrix = new long long[matrixsize];
    for (int i = 0; i < matrixsize; i++) {
        surjmatrix[i] = 0;
    }

    //Now calculate the surjmatrix with the help of everything we just created
    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        cout << "Calculating surjmatrix for graph " << i << endl;
        for (int j = 0; j <= i; j++) {
            surjmatrix[i*hom_matrix.numGraphs + j] = hom_matrix.matrix[i * hom_matrix.numGraphs + j];
            for (const auto &[index, times] : surj_sub_index_times[j]) {
                surjmatrix[i*hom_matrix.numGraphs + j] -= times * surjmatrix[i * hom_matrix.numGraphs + index];
            }
        }
    }

    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        for (int j = 0; j < 5; j++) {
            cout << surjmatrix[i * hom_matrix.numGraphs + j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        if (surjmatrix[i*hom_matrix.numGraphs + i] == 0) {
            cout << "FEHLER!!!!!!" << i << endl;
        }
    }

    for (int i = 0; i < hom_matrix.numGraphs; i++) {
        for (int j = 0; j < hom_matrix.numGraphs; j++) {
            if (surjmatrix[i*hom_matrix.numGraphs + j] < 0) {
                cout << "FEHLER!!!!!!" << i << " " << j << endl;
            }
        }
    }

    Graph_Matrix surj_matrix = Graph_Matrix();
    surj_matrix.numGraphs = hom_matrix.numGraphs;
    surj_matrix.numVertices_max = hom_matrix.numVertices_max;
    surj_matrix.minEdges = hom_matrix.minEdges;
    surj_matrix.maxEdges = hom_matrix.maxEdges;
    surj_matrix.graphs = hom_matrix.graphs;
    surj_matrix.matrix = surjmatrix;

    storeMatrix(filename, surj_matrix);
    delete[] hom_matrix.matrix;
}





