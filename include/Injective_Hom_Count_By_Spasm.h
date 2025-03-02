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

#endif //INJECTIVE_HOM_COUNT_BY_SPASM_H
