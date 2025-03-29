//
// Created by Christoph Greger on 06.03.25.
//

#ifndef SPASM_H
#define SPASM_H

#include "Graph.h"
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::uint256_t;
using boost::multiprecision::int256_t;


namespace Spasm {

    struct Spasm_Component {
        Graph Graph;
        std::string canonicalString;
    };

    struct Spasm_Graph {
        std::vector<std::pair<std::string, int>> Components; //Component Graphs with its count (potentiate afterwards)
        int256_t Factor{};
    };

    struct Spasm {
        Graph underlying_Graph;
        uint256_t numAutomorphisms;
        std::vector<Spasm_Graph> Graphs;
        std::vector<Spasm_Component> Components;
    };

    Spasm getFromFile(const std::string &spasm_file);

    void writeToFile(const std::string &output_file, const Spasm &spasm);

    void create_and_store_Spasm_k_Matching(const std::string &filename, int k);

    Spasm create_Spasm(const Graph &G, const int k = -1, const uint256_t &numAutomorphisms = 1);

    Spasm create_Spasm_withAut(const Graph &G);

    void create_and_store_Spasm(const std::string &filename, const Graph &G, const int k = -1, const uint256_t &numAutomorphisms = 1);

}



#endif //SPASM_H
