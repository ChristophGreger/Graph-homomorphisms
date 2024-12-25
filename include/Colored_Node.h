//
// Created by Christoph Greger on 25.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_COLORED_NODE_H
#define GRAPH_HOMOMORPHISMS_COLORED_NODE_H


#include "Node.h"

class Colored_Node : public Node {
public:
    int color;
    static bool equals(Node node);
    bool equals(Colored_Node node) const;
};


#endif //GRAPH_HOMOMORPHISMS_COLORED_NODE_H
