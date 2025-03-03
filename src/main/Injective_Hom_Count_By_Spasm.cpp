//
// Created by Christoph Greger on 02.03.25.
//

#include "../../include/Injective_Hom_Count_By_Spasm.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct spasm_graph {
    Graph graph;
    std::string canonicalStr;
    int count;
    long long factor;
};

vector<spasm_graph> convertFileToSpasmGraph(const std::string &spasm_file) {
    vector<spasm_graph> spasmGraphs;
    ifstream inFile(spasm_file);
    if (!inFile.is_open()) {
        cerr << "Error opening file: " << spasm_file << endl;
        return spasmGraphs;
    }

    string line;
    // First line: total number of graph entries.
    if (!getline(inFile, line)) {
        cerr << "File is empty" << endl;
        return spasmGraphs;
    }

    istringstream countStream(line);
    int numGraphs = 0;
    countStream >> numGraphs;

    for (int i = 0; i < numGraphs; i++) {
        // Skip empty lines.
        while(getline(inFile, line) && line.empty()) { }
        if (line.empty()) break;

        // Parse the header: number of vertices and edges.
        istringstream headerStream(line);
        int numVertices = 0, numEdges = 0;
        headerStream >> numVertices >> numEdges;

        // Create a new Graph; colored set to false.
        Graph spGraph(false);
        for (int v = 0; v < numVertices; v++) {
            spGraph.addNode(Node());
        }

        // Read exactly numEdges lines for edges.
        for (int e = 0; e < numEdges; e++) {
            if (!getline(inFile, line)) break;
            istringstream edgeStream(line);
            int u = 0, v = 0;
            char arrowChar;  // to consume '-' and '>'
            edgeStream >> u >> arrowChar >> arrowChar >> v;
            spGraph.addEdge(u, v);
        }

        // Skip possible empty lines before the canonical string.
        while(getline(inFile, line) && line.empty()) { }
        if (line.empty()) break;
        string canonicalStr = line;

        // Read count line.
        if (!getline(inFile, line)) break;
        istringstream countLineStream(line);
        int graphCount = 0;
        countLineStream >> graphCount;

        // Read factor line.
        if (!getline(inFile, line)) break;
        istringstream factorStream(line);
        long long factor = 0;
        factorStream >> factor;

        // Create spasm_graph and add it to the vector.
        spasm_graph sg { spGraph, canonicalStr, graphCount, factor };
        spasmGraphs.push_back(sg);
    }

    inFile.close();
    return spasmGraphs;
}

long long injective_count_by_spasm(const std::string& spasm_file, Graph &target) {

    const vector<spasm_graph> spasmGraphs = convertFileToSpasmGraph(spasm_file);

    long long totalResult = 0;
    // Loop over the expected graphs.
    for (const auto & i : spasmGraphs) {

        // Compute the number of injective homomorphisms from spasmGraph to target
        Graph spasmGraph = i.graph;
        long long Count = spasmGraph.calculateNumberofHomomorphismsTo(target);
        totalResult += Count * i.factor;
    }
    return totalResult;
}

void convert_spasm_to_smaller(const std::string& spasm_file, const std::string& output_file) {
    const vector<spasm_graph> spasmGraphs = convertFileToSpasmGraph(spasm_file);

    std::unordered_map<std::string, Graph> canonicalMap; //Maps canonical Strings to a Graph

    //Get map of all connected graphs in the spasm file
    for (const auto &spasmGraph : spasmGraphs) {
        Graph graph = spasmGraph.graph;
        for (const auto &comp : graph.connectedComponents()) {
            canonicalMap[comp.canonicalString()] = comp;
        }
    }

    //Now we write the following in the output file:
    //The number of distinct connected components. and all distinct connected components of all the graphs in the spasm file
    //For each graph in the (unconnected) spasm we write a list of its connected components. But there will be no duplicates written,
    //but if a component appears twice we multiple its factor by 2 etc.

    std::ofstream outFile(output_file);
    if (!outFile) {
        std::cerr << "Error opening file: " << output_file << std::endl;
        return;
    }

    outFile << spasmGraphs.size() << "\n"; //First Line: Number of maybe unconnected graphs in the spasm file
    outFile << canonicalMap.size() << "\n"; //Second Line: Number of distinct connected components

    //Write all distinct connected components
    for (const auto &[fst, snd] : canonicalMap) {
        outFile << snd.numVertices << " " << snd.edges.size() << "\n"; //First Line: Number of vertices and edges
        for (const auto &[u, v] : snd.edges) {
            outFile << u << " -> " << v << "\n"; //Then all edges
        }
        outFile << fst << "\n"; //Then the canonical string
    }

    //write the unconnected graph with its connected components
    for (const auto &spasmGraph : spasmGraphs) {
        Graph graph = spasmGraph.graph;
        std::unordered_map<std::string, int> componentMap; //Maps canonical strings to the number of times they appeared
        for (const auto &comp : graph.connectedComponents()) {
            std::string canon = comp.canonicalString();
            if (!componentMap.contains(canon)) {
                componentMap[canon] = 1;
            } else {
                componentMap[canon]++;
            }
        }


        outFile << componentMap.size() << "\n"; //First Line: Number of distinct connected components
        for (const auto &[fst, snd] : componentMap) {
            outFile << fst << "\n"; //Second Line: The canonical string
            outFile << snd << "\n"; //Third Line: The number of times it appeared
        }
        outFile << spasmGraph.factor << "\n"; //Last Line: The (maybe new) factor

    }

    cout << canonicalMap.size() << " distinct connected components written to " << output_file << endl;

    outFile.close();

}

spasm_smaller_graph getFromFile_spasm_smaller(std::string &filename) {
    spasm_smaller_graph result;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return result;
    }
    std::string line;

    // Read total number of unconnected spasm graphs
    int totalGraphs = 0;
    if (!std::getline(inFile, line)) {
        std::cerr << "File is empty or format error." << std::endl;
        return result;
    }
    std::istringstream iss(line);
    iss >> totalGraphs;

    // Read number of distinct connected components
    int totalComponents = 0;
    if (!std::getline(inFile, line)) {
        std::cerr << "Missing distinct components count." << std::endl;
        return result;
    }
    std::istringstream iss2(line);
    iss2 >> totalComponents;

    // Map canonical string to spasm_smaller_component
    std::unordered_map<std::string, spasm_smaller_component> compMap;

    // Read distinct connected components
    for (int i = 0; i < totalComponents; i++) {
        // Read header: number of vertices and edges
        if (!std::getline(inFile, line)) break;
        std::istringstream headerStream(line);
        int numVertices = 0, numEdges = 0;
        headerStream >> numVertices >> numEdges;

        // Create the graph component
        Graph compGraph(false);
        for (int v = 0; v < numVertices; v++) {
            compGraph.addNode(Node());
        }
        for (int e = 0; e < numEdges; e++) {
            if (!std::getline(inFile, line)) break;
            std::istringstream edgeStream(line);
            int u = 0, v = 0;
            char ch1, ch2;
            edgeStream >> u >> ch1 >> ch2 >> v;
            compGraph.addEdge(u, v);
        }
        // Read canonical string
        std::string canon;
        if (!std::getline(inFile, canon)) break;

        spasm_smaller_component comp;
        comp.graph = compGraph;
        comp.canonicalStr = canon;
        compMap[canon] = comp;
        result.components.push_back(comp);
    }

    // Read full graphs (unconnected spasm graphs)
    for (int i = 0; i < totalGraphs; i++) {
        if (!std::getline(inFile, line)) break;
        int compCount = 0;
        std::istringstream compCountStream(line);
        compCountStream >> compCount;
        std::vector<pair<spasm_smaller_component, int>> fullComponents;
        for (int j = 0; j < compCount; j++) {
            std::string canon;
            if (!std::getline(inFile, canon)) break;
            int count;
            std::string countString;
            if (!std::getline(inFile, countString)) break;
            count = std::stoi(countString);
            if (compMap.contains(canon))
                fullComponents.emplace_back(compMap[canon], count);
        }
        if (!std::getline(inFile, line)) break;
        long long factor = 0;
        std::istringstream factorStream(line);
        factorStream >> factor;

        spasm_smaller_full_graphs fullGraph;
        fullGraph.components = fullComponents;
        fullGraph.factor = factor;
        result.fullGraphs.push_back(fullGraph);
    }

    inFile.close();
    return result;
}

long long injective_count_by_spasm_smaller(const std::string &spasm_smaller_file, Graph &target) {

    std::string spasm_file = spasm_smaller_file;
    auto smallspasm = getFromFile_spasm_smaller(spasm_file);

    long long total = 0;

    std::unordered_map<std::string, long long> componentMap;

    for (const auto &comp : smallspasm.components) {
        std::string canon = comp.canonicalStr;
        Graph graph = comp.graph;
        componentMap[canon] = graph.calculateNumberofHomomorphismsTo(target);
    }

    for (const auto &fullGraph : smallspasm.fullGraphs) {
        long long factor = fullGraph.factor;
        for (const auto &comp : fullGraph.components) {
            std::string canon = comp.first.canonicalStr;
            int exponent = comp.second;
            factor *= powlong(componentMap[canon], exponent);
        }
        total += factor;
    }

    return total;
}


