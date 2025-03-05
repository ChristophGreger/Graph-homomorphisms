//
// Created by falco on 04.03.25.
//

#include "../../include/GraphTemplate.h"

GraphTemplate::GraphTemplate(const bool colored) : colored(colored) {
    nodes = vector<Node>();
    edges = unordered_set<pair<int, int>, PairHash>();
}

void GraphTemplate::addNode(const Node& node) {
    nodes.push_back(node);
}

void GraphTemplate::addEdge(int node1, int node2) {
    if (node1 < nodes.size() && node2 < nodes.size()) {
        if (node2 < node1) {
            swap(node1, node2);
        }
        edges.insert(make_pair(node1, node2));
    }
}

bool GraphTemplate::isEdge(int node1, int node2) const {
    if (node2 < node1) {
        swap(node1, node2);
    }
    return edges.find(make_pair(node1, node2)) != edges.end();
}