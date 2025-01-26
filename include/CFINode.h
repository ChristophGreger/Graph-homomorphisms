//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_CFINODE_H
#define GRAPH_HOMOMORPHISMS_CFINODE_H

#include <vector>


using namespace std;


class CFINode {
public:
    CFINode(int number, vector<int> edgeSubset);
    int number;
    vector<int> edgeSubset;
};


#endif //GRAPH_HOMOMORPHISMS_CFINODE_H
