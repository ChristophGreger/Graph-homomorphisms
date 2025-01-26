//
// Created by Christoph Greger on 26.12.24.
//

#include "CFINode.h"
#include <utility>

CFINode::CFINode(int number, vector<int> edgeSubset) : number(number), edgeSubset(std::move(edgeSubset)) {}