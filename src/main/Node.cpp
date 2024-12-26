//
// Created by Christoph Greger on 26.12.24.
//

#include "Node.h"

Node::Node(bool colored, int color) : colored(colored), color(color) {}

bool Node::equals(Node &node) const {
    if (colored && node.colored) {
        return color == node.color;
    }
    return true;
}