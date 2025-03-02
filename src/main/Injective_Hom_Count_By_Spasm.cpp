//
// Created by Christoph Greger on 02.03.25.
//

#include "../../include/Injective_Hom_Count_By_Spasm.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

long long injective_count_by_spasm(const std::string& spasm_file, Graph &target) {

    std::ifstream inFile(spasm_file);
    if (!inFile.is_open()) {
        std::cerr << "Error opening file: " << spasm_file << std::endl;
        return 0;
    }

    std::string line;
    // First line: total number of graph entries
    if (!std::getline(inFile, line)) {
        std::cerr << "File is empty" << std::endl;
        return 0;
    }

    std::istringstream countStream(line);
    int numGraphs = 0;
    countStream >> numGraphs;
    long long totalResult = 0;

    // Loop over the expected graphs.
    for (int i = 0; i < numGraphs; i++) {
        // Skip empty lines
        while(std::getline(inFile, line) && line.empty()) { }
        if (line.empty()) break;

        // Parse the graph header with number of vertices and edges.
        std::istringstream header(line);
        int numVertices = 0, numEdges = 0;
        header >> numVertices >> numEdges;

        // Create a new Graph; note: setting colored = false.
        Graph spasmGraph(false);
        // Add numVertices nodes (dummy nodes for graph construction).
        for (int v = 0; v < numVertices; v++) {
            spasmGraph.addNode(Node());
        }

        // Read exactly numEdges lines for edges.
        for (int e = 0; e < numEdges; e++) {
            if (!std::getline(inFile, line)) break;
            // Expect line format: "u -> v"
            std::istringstream edgeStream(line);
            int u, v;
            char arrow1, arrow2;
            edgeStream >> u >> arrow1 >> arrow2 >> v;
            spasmGraph.addEdge(u, v);
        }

        // Skip possible empty line
        while(std::getline(inFile, line) && line.empty()) { }
        if (line.empty()) break;
        // This line is the canonical string, currently unused.
        std::string canonicalStr = line;

        // Read count line
        if (!std::getline(inFile, line)) break;
        std::istringstream countLineStream(line);
        int graphCount = 0;
        countLineStream >> graphCount;

        // Read factor line
        if (!std::getline(inFile, line)) break;
        std::istringstream factorLineStream(line);
        long long factor = 0;
        factorLineStream >> factor;

        // Compute the number of injective homomorphisms from spasmGraph to target
        long long Count = spasmGraph.calculateNumberofHomomorphismsTo(target);
        totalResult += Count * factor;
    }

    inFile.close();
    return totalResult;
}
