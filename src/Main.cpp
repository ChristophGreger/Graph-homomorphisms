//
// Created by Christoph Greger on 24.12.24.
//

#include <fstream>
#include <iostream>
#include <string>

#include "Graph.h"

struct Flags {
    bool cfi = false;
    bool time = false;
};

Flags processFlags(int argc, char* argv[], int start) {
    bool cfi = false;
    bool time = false;

    // Process optional flags -cfi, -time and -sf <spasm_file>
    for (int i = start; i < argc; i++) {
        std::string option = argv[i];
        if (option == "--cfi") {
            cfi = true;
        } else if (option == "--time") {
            time = true;
        } else {
            std::cerr << "Unknown flag: " << option << std::endl;
            exit(1);
        }
    }
    const Flags flags = {cfi,time};
    return flags;
}

/*
 * format:
 *
 * graph adj-list <n> <m>
 * <u_1> <v_1>
 * ...
 * <u_m> <v_m>
 */
Graph convertToGraph(const std::string& file) {

    std::ifstream inputFile(file);
    if (!inputFile) {
        std::cerr << "Error opening file: " << file << std::endl;
        exit(1);
    }

    std::string graphKey;
    std::string graphType;
    int numNodes, numEdges;

    // First line: Graph type, number of nodes, and number of edges
    inputFile >> graphKey >> graphType >> numNodes >> numEdges;

    // Initialize the graph with the number of nodes
    GraphTemplate tempGraph = GraphTemplate();

    for (int i = 0; i < numNodes; i++) {
        tempGraph.addNode(Node(i));
    }

    // Read edges and add to the graph
    int u, v;
    for (int i = 0; i < numEdges; ++i) {
        inputFile >> u >> v;
        tempGraph.addEdge(u, v);
    }

    inputFile.close();
    return Graph(tempGraph);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: ./main (hom | emb | mat)" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "hom") {
        if (argc < 4) {
            std::cerr << "Usage: ./main hom <H> <G> [--cfi] [--time]";
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);

        std::string HFile = argv[2];
        std::string GFile = argv[3];

        Graph H = convertToGraph(HFile);
        Graph G = convertToGraph(GFile);

        // Debugging output (or actual functionality based on flags)
        std::cout << "Command: " << command << std::endl;
        std::cout << "H: " << HFile << std::endl;
        std::cout << "G: " << GFile << std::endl;
        std::cout << "Option -cfi: " << (flags.cfi ? "enabled" : "disabled") << std::endl;
        std::cout << "Option -time: " << (flags.time ? "enabled" : "disabled") << std::endl;
    }
    if (command == "emb") {
        if (argc < 4) {
            std::cerr << "Usage: ./main emb <H> <G> [--spasmFile <spasmFile>] [--cfi] [--time]";
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);
    }
    if (command == "mat") {
        if (argc < 4) {
            std::cerr << "Usage: ./main mat <k> <G> [--cfi] [--time]";
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);
    }

    return 0;
}
