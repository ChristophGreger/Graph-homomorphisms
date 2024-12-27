//
// Created by Christoph Greger on 26.12.24.
//

#include "Node.h"

Node::Node(int color) : color(color) {}

bool Node::equals(Node &node) const {
    return color == node.color;
}