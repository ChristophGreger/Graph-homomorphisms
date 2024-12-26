//
// Created by Christoph Greger on 26.12.24.
//

#include "CFINode.h"

#include <utility>


CFINode::CFINode(int color, int number, vector<int> ownedEdges, vector<int> notownedEdges) : number(number), color(color), ownedEdges(std::move(ownedEdges)), notownedEdges(std::move(notownedEdges)) {}




