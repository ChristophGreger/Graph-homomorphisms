//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_CFINODE_H
#define GRAPH_HOMOMORPHISMS_CFINODE_H

#include <vector>
#include <memory> 

#include "BitArray.h"

using namespace std;

class CFINode {
public:
    CFINode();
    CFINode(int number, std::unique_ptr<BitArray> edgeSubset);
    int number;
    std::unique_ptr<BitArray> edgeSubset;
};


#endif //GRAPH_HOMOMORPHISMS_CFINODE_H
