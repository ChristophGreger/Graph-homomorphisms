//
// Created by Christoph Greger on 06.03.25.
//

#include "Spasm.h"
#include <fstream>
#include <sstream>
#include "utilities.h"
#include "Nauty_wrapper.h"

#define set nauty_set

int maxn = 31;
int maxm = 1;

inline std::string getCanonicalString(graph *g, const int numVertices) {

    int lab[maxn], ptn[maxn], orbits[maxn]; //maxn
    static DEFAULTOPTIONS_GRAPH(options);
    options.getcanon = TRUE;
    statsblk stats;

    graph canon[maxn*maxm]; //maxn*maxm

    EMPTYGRAPH(canon, maxm, numVertices);

    densenauty(g, lab, ptn, orbits, &options, &stats, maxm, numVertices, canon);

    std::ostringstream oss;
    // Optional: stelle sicher, dass der Stream im hexadezimalen Format arbeitet.
    oss << std::hex;
    for (int i = 0; i < maxm*numVertices; i++) {
        // setfill und setw sorgen für führende Nullen, sodass jedes setword als 16-stellige hexadezimale Zahl ausgegeben wird.
        oss << std::setfill('0') << std::setw(16) << canon[i];
    }
    return oss.str();
}

void Spasm::create_and_store_Spasm_k_Matching(const std::string &filename, int k) {

    //Construct underlying Graph
    GraphTemplate underlying;
    for (int i = 0; i < 2 * k; ++i) underlying.addNode(Node());
    for (int i = 0; i < k; ++i) underlying.addEdge(2 * i, 2 * i + 1);
    Graph underlying_Graph = Graph(underlying);

    long long total = 0;

    //string: canonical Form
    //pair: first (string) is a string representation of the graph
    //second is the factor
    std::unordered_map<std::string, std::pair<std::string, long long>> canonicalMap;

    const int numEdges = k;
    const int numVertices = 2 * k;

    const uint256_t numAutomorphisms = (1 << k) * factorial(k);

    // partition[i] gibt an, zu welchem Block der Knoten i gehört.
    std::vector<int> partition(numVertices, -1);
    partition[0] = 0;  // Der erste Knoten wird fest Block 0 zugeordnet
    std::vector<int> numperpartition(numVertices, 0);


    // Rekursive Backtracking-Funktion:
    //&g muss gross genug sein!
    //edgearray sollte numEdges platz haben!
    std::function<void(int, int, graph*, std::pair<int, int> *)> backtrack = [&](const int vertex, const int currentMax, graph *g, std::pair<int, int> *edgeArray) {
        if (vertex == numVertices) {
            // Erzeuge den Quotientgraphen anhand der aktuellen Partition.

            int numEdgeshere = 0;

            const int numVerticeshere = currentMax + 1;

            EMPTYGRAPH(g, maxm, numVerticeshere);

            // Für jede Kante im Originalgraphen, die zwei verschiedene Blöcke verbindet,
            // wird im Quotientgraphen eine entsprechende Kante eingefügt.
            for (int i = 0; i < numVertices; i += 2) {
                const int bu = partition[i];
                const int bv = partition[i+1];

                if (ISELEMENT(&g[bu], bv)) {
                    continue;
                }
                ADDONEEDGE(g, bu, bv, maxm);
                edgeArray[numEdgeshere] = std::make_pair(bu, bv);
                ++numEdgeshere;
            }
            // Berechne die kanonische Form des Quotientgraphen.

            const std::string canon = getCanonicalString(g, currentMax+1);

            //Computing the factor (or at least the Partition element product part of it) of the graph
            long long factor = 1;

            for (int i = 0; i < numVertices; i++) {
                numperpartition[i] = 0;
            }

            for (int i = 0; i < numVertices; i++) {
                ++numperpartition[partition[i]];
            }

            for (int i = 0; i < numVerticeshere; i++) {
                factor *= factorial(numperpartition[i]-1);
            }
            //End of factor computation (first part)


            if (!canonicalMap.contains(canon)) {
                std::stringstream ss;
                ss << numVerticeshere << " " << numEdgeshere << "\n";
                for (int i = 0; i < numEdgeshere; i++) {
                    ss << edgeArray[i].first << " -> " << edgeArray[i].second << "\n";
                }
                canonicalMap[canon] = make_pair(ss.str(), factor);
            } else {
                canonicalMap[canon].second += factor;
            }
            ++total;


            //DEBUGGING
            if (total % 1000000 == 0) {
                std::cout << "Total: " << total << std::endl;
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
            backtrack(vertex + 1, currentMax, g, edgeArray);
        }
        // Eröffne einen neuen Block für den aktuellen Knoten.
        partition[vertex] = currentMax + 1;
        backtrack(vertex + 1, currentMax + 1, g, edgeArray);
    };

    auto *g = new graph[maxn * maxm];
    auto *edgearray = new std::pair<int, int>[numEdges];
    backtrack(1, 0, g, edgearray);

    delete[] g;
    delete[] edgearray;

    //Now the canonicalMap is filled with all the graphs

    //Convert the canonicalMap to a vector of (canonical string, Graph, factor) tuples
    vector<std::tuple<std::string, Graph, long long>> big_graphs;
    for (const auto &[canon, pair] : canonicalMap) {
        std::istringstream ss(pair.first);
        int numVertices_, numEdges_;
        ss >> numVertices_ >> numEdges_;

        GraphTemplate gt;
        for (int i = 0; i < numVertices_; ++i) gt.addNode(Node());

        for (int i = 0; i < numEdges_; ++i) {
            int u, v;
            char arrow1, arrow2;
            ss >> u >> arrow1 >> arrow2 >> v;
            gt.addEdge(u, v);
        }

        Graph graph = Graph(gt);

        //We have to make sure that the factor has the right sign (positive or negative)
        const int signexponent = numVertices - numVertices_;
        long long factor = pair.second;
        if (signexponent % 2 == 1) {
            factor = -factor;
        }

        big_graphs.emplace_back(canon, graph, factor);
    }

    Spasm spasm;
    spasm.underlying_Graph = underlying_Graph;
    spasm.numAutomorphisms = numAutomorphisms;

    std::unordered_map<std::string, Graph> canonical_SmallMap; //Maps canonical Strings of the Components to a graph

    //filling the canonical_SmallMap and the Spasm_Graph vector in the spams
    for (const auto &[canon, graph, factor] : big_graphs) {
        std::unordered_map<std::string, int> componentMap; //Maps canonical Strings of the component to the count of that component
        for (const auto &comp : graph.connectedComponents()) {
            std::string canonical_of_component = comp.canonicalString();
            canonical_SmallMap.insert({canonical_of_component ,comp});
            if (!componentMap.contains(canonical_of_component)) {
                componentMap.insert({canonical_of_component, 1});
            } else {
                componentMap[canonical_of_component]++;
            }
        }
        Spasm_Graph this_big_graph;
        std::vector<std::pair<std::string, int>> components;
        for (const auto &[canon, count] : componentMap) {
            components.emplace_back(canon, count);
        }
        this_big_graph.Components = components;
        this_big_graph.Factor = factor;
        spasm.Graphs.push_back(this_big_graph);
    }

    for (const auto &[canon, graph] : canonical_SmallMap) {
        Spasm_Component comp;
        comp.Graph = graph;
        comp.canonicalString = canon;
        spasm.Components.push_back(comp);
    }

    writeToFile(filename, spasm);
}

void Spasm::writeToFile(const std::string &output_file, const Spasm &spasm) {
    std::ofstream file(output_file);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + output_file);
    }

    file << spasm.underlying_Graph.numVertices << " " << spasm.underlying_Graph.edges.size() << "\n";
    for (const auto &edge : spasm.underlying_Graph.edges) {
        file << edge.first << " " << edge.second << "\n";
    }

    file << spasm.numAutomorphisms << "\n";

    file << spasm.Graphs.size() << "\n";
    for (const auto &sg : spasm.Graphs) {
        file << sg.Factor << " " << sg.Components.size() << "\n";
        for (const auto &compPair : sg.Components) {
            file << compPair.first << "\n" << compPair.second << "\n";
        }
    }

    file << spasm.Components.size() << "\n";
    for (const auto &comp : spasm.Components) {
        file << comp.canonicalString << "\n";
        file << comp.Graph.numVertices << " " << comp.Graph.edges.size() << "\n";
        for (const auto &edge : comp.Graph.edges) {
            file << edge.first << " " << edge.second << "\n";
        }
    }

    file.close();
}

Spasm::Spasm Spasm::getFromFile(const std::string &spasm_file) {
    std::ifstream file(spasm_file);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + spasm_file);
    }

    Spasm spasm;

    int vertices, edges;
    file >> vertices >> edges;

    GraphTemplate gt;
    for (int i = 0; i < vertices; ++i) gt.addNode(Node());

    for (int i = 0; i < edges; ++i) {
        int u, v;
        file >> u >> v;
        gt.addEdge(u, v);
    }
    spasm.underlying_Graph = Graph(gt);

    file >> spasm.numAutomorphisms;

    int numGraphs;
    file >> numGraphs;
    for (int i = 0; i < numGraphs; ++i) {
        Spasm_Graph sg;
        int compCount;
        file >> sg.Factor >> compCount;

        for (int j = 0; j < compCount; ++j) {
            std::string canonical;
            int count;
            file >> canonical >> count;
            sg.Components.emplace_back(canonical, count);
        }
        spasm.Graphs.push_back(sg);
    }

    int numComponents;
    file >> numComponents;
    for (int i = 0; i < numComponents; ++i) {
        Spasm_Component comp;

        file >> comp.canonicalString; // Position geändert

        int compVertices, compEdges;
        file >> compVertices >> compEdges;

        GraphTemplate compGt;
        for (int j = 0; j < compVertices; ++j) compGt.addNode(Node());

        for (int j = 0; j < compEdges; ++j) {
            int u, v;
            file >> u >> v;
            compGt.addEdge(u, v);
        }

        comp.Graph = Graph(compGt);

        spasm.Components.push_back(comp);
    }

    file.close();

    return spasm;
}



