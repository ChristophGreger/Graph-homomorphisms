//
// Created by Christoph Greger on 25.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_NODE_H
#define GRAPH_HOMOMORPHISMS_NODE_H


class Node {
public:
    bool equals(Node& node) const;
    int color;
    explicit Node(int color = 0);
};


#endif //GRAPH_HOMOMORPHISMS_NODE_H
