//
// Created by Christoph Greger on 02.03.25.
//

#ifndef INJECTIVE_HOM_COUNT_BY_SPASM_H
#define INJECTIVE_HOM_COUNT_BY_SPASM_H

#include <string>
#include "Graph.h"

long long injective_count_by_spasm(const std::string& spasm_file, Graph &target);

void convert_spasm_to_smaller(const std::string& spasm_file, const std::string& output_file);

long long injective_count_by_spasm_smaller(const std::string &spasm_smaller_file, Graph &target);


struct spasm_smaller_component {
    Graph graph;
    std::string canonicalStr;
};

struct spasm_smaller_full_graphs {
    std::vector<pair<spasm_smaller_component, int>> components; //first in the pair is the component, second is the number of times it appeared (potentiate afterwards)
    long long factor{};
};

struct spasm_smaller_graph {
    std::vector<spasm_smaller_component> components;
    std::vector<spasm_smaller_full_graphs> fullGraphs;
};


spasm_smaller_graph getFromFile_spasm_smaller(const std::string &spasm_smaller_file);


#endif //INJECTIVE_HOM_COUNT_BY_SPASM_H
