//
// Created by Christoph Greger on 25.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_NODE_H
#define GRAPH_HOMOMORPHISMS_NODE_H


class Node {
public:
    /// Equals on Nodes
    /// @param node The node to be compared with
    /// @return True if they share the same color, false otherwise
    bool equals(Node& node) const;
    int color;
    explicit Node(int color = 0);
};


#endif //GRAPH_HOMOMORPHISMS_NODE_H
