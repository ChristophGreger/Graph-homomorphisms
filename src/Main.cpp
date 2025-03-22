//
// Created by Christoph Greger on 24.12.24.
//

#include <fstream>
#include <iostream>
#include <string>

#include "CalcHoms.h"
#include "Graph.h"
#include <cstdio>
#include <random>
#include <boost/dll/runtime_symbol_info.hpp>

#include "Spasm.h"

namespace fs = std::filesystem;

struct Flags {
    bool cfi = false;
    bool debug = false;
    std::string spasmFile;
};

Flags processFlags(int argc, char* argv[], int start) {
    bool cfi = false;
    bool debug = false;
    std::string spasmFile;

    // Process optional flags -cfi, -debug and -sf <spasm_file>
    for (int i = start; i < argc; i++) {
        std::string option = argv[i];
        if (option == "--cfi") {
            cfi = true;
        } else if (option == "--debug") {
            debug = true;
        } else if (option == "--spasmFile") {
            if (i+1 < argc) {
                spasmFile = argv[++i];
            }else {
                std::cerr << "Invalid value for option --spasmFile" << std::endl;
            }
        } else {
            std::cerr << "Unknown flag: " << option << std::endl;
            exit(1);
        }
    }
    const Flags flags = {cfi,debug,spasmFile};
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
            std::cerr << "Usage: ./main hom <H> <G> [--cfi] [--debug]" << std::endl;
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);

        std::string HFile = argv[2];
        std::string GFile = argv[3];

        Graph H = convertToGraph(HFile);
        Graph G = convertToGraph(GFile);

        if (flags.debug) {
            std::cout << "Command: " << command << std::endl;
            std::cout << "H: " << HFile << std::endl;
            std::cout << "G: " << GFile << std::endl;
            std::cout << "Option --cfi: " << (flags.cfi ? "enabled" : "disabled") << std::endl;
            std::cout << "Option --debug: enabled" << std::endl;
        }

        auto start1 = std::chrono::high_resolution_clock::now();

        int256_t result;
        if (flags.cfi) {
            result = CalcHoms::calcNumHomsCFI_uncolored(H,G,false);
        }else {
            result = CalcHoms::calcNumHoms(H,G);
        }

        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);

        if (flags.debug) {
            std::cout << "Time: " << duration1.count() << "ms" << std::endl;
        }

        cout << result << endl;
    }

    if (command == "emb") {

        if (argc < 4) {
            std::cerr << "Usage: ./main emb <H> <G> [--spasmFile <spasmFile>] [--cfi] [--debug]" << endl;
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);

        std::string HFile = argv[2];
        std::string GFile = argv[3];

        Graph H = convertToGraph(HFile);
        Graph G = convertToGraph(GFile);

        if (flags.debug) {
            std::cout << "Command: " << command << std::endl;
            std::cout << "H: " << HFile << std::endl;
            std::cout << "G: " << GFile << std::endl;
            std::cout << "Option --cfi: " << (flags.cfi ? "enabled" : "disabled") << std::endl;
            std::cout << "Option --debug: " << "enabled" << std::endl;
        }

        auto start1 = std::chrono::high_resolution_clock::now();

        std::string spasmFile = flags.spasmFile;
        if (flags.spasmFile.empty()) {

            std::random_device rd;  // Initialize a random device for seeding
            std::mt19937 gen(rd());  // Mersenne Twister pseudo-random generator seeded with rd()

            // Define a uniform distribution between 0 and 100
            std::uniform_int_distribution<> dist(100000000, 999999999);

            // Generate a random number
            int random_number = dist(gen);

            spasmFile = std::filesystem::temp_directory_path() / ("temp_spasm_" + std::to_string(random_number) + ".tmp");
            Spasm::create_and_store_Spasm(spasmFile,H);
        }

        if (flags.debug) {
            std::cout << "Option --spasmFile: " << spasmFile << std::endl;
        }

        int256_t result = CalcHoms::calcNumInjHoms(spasmFile,G,flags.cfi, false);

        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);

        if (flags.debug) {
            std::cout << "Time: " << duration1.count() << "ms" << std::endl;
        }

        cout << result << endl;
    }

    if (command == "mat") {
        if (argc < 4) {
            std::cerr << "Usage: ./main mat <k> <G> [--cfi] [--debug]" << std::endl;
            return 1;
        }
        int k = 0;
        try {
            k = stoi(argv[2]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input: k is not a valid number" << std::endl;
            exit(1);
        }

        if (k < 1 || k > 9) {
            std::cerr << "Invalid value for k only 1-9 is available: " << k << std::endl;
        }

        std::string GFile = argv[3];
        Graph G = convertToGraph(GFile);

        const Flags flags = processFlags(argc, argv, 4);

        if (flags.debug) {
            std::cout << "Command: " << command << std::endl;
            std::cout << "k: " << k << std::endl;
            std::cout << "G: " << GFile << std::endl;
            std::cout << "Option --cfi: " << (flags.cfi ? "enabled" : "disabled") << std::endl;
            std::cout << "Option --debug: " << "enabled" << std::endl;
        }

        auto start1 = std::chrono::high_resolution_clock::now();

        // Get the executable path from argv[0]
        fs::path exePath(argv[0]);

        // If exePath is relative, convert it to an absolute path
        if (exePath.is_relative()) {
            exePath = fs::absolute(exePath);
        }

        // Get the directory of the executable by removing the file name
        fs::path exeDir = exePath.parent_path();
        fs::path assetPath = exeDir / "assets" / ("k_" + std::to_string(k) + ".txt");

        int256_t result = CalcHoms::calcNumInjHoms(assetPath.string(),G,flags.cfi, false);

        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);

        if (flags.debug) {
            std::cout << "Time:" << duration1.count() << "ms" << std::endl;
        }

        cout << result << endl;
    }

    return 0;
}
