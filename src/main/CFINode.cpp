//
// Created by Christoph Greger on 26.12.24.
//

#include <CFINode.h>
#include <utility>

CFINode::CFINode(int number, std::unique_ptr<BitArray> edgeSubset) : number(number), edgeSubset(std::move(edgeSubset)) {}
CFINode::CFINode(): number(0), edgeSubset(nullptr) {}