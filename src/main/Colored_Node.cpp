//
// Created by Christoph Greger on 25.12.24.
//

#include "Colored_Node.h"


bool Colored_Node::equals(Node node) {
    return true;
}

bool Colored_Node::equals(Colored_Node node) const{
    return color == node.color;
}
