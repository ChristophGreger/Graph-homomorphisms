//
// Created by falco on 04.03.25.
//

#ifndef GRAPHTEMPLATE_H
#define GRAPHTEMPLATE_H

#include <vector>
#include <unordered_set>

#include "Node.h"
#include "utilities.h"

using namespace std;

class GraphTemplate {

public:

    bool colored;
    unordered_set<pair<int, int>, PairHash> edges;
    vector<Node> nodes;

    explicit GraphTemplate(bool colored = false);
    ~GraphTemplate();

    void addNode(const Node& node);
    void addEdge(int node1, int node2);

};


#endif //GRAPHTEMPLATE_H