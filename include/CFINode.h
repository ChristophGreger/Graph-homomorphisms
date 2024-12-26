//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GITIGNORE_CFINODE_H
#define GITIGNORE_CFINODE_H

#include <vector>


using namespace std;


class CFINode {
public:
    CFINode(int color, int number, vector<int> ownedEdges, vector<int> notownedEdges);
    int color;
    int number;
    vector<int> ownedEdges; //there are the owned and not-owned Edges in an CFI Graph
    vector<int> notownedEdges;
};


#endif //GITIGNORE_CFINODE_H
