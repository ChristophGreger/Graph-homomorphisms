#include "BruteForceAlgo.h"
#include "BitArray.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>

#include <iostream>

using namespace std;

struct CFINode2 {
    int number;
    BitArray* edgeSubset;
};

//set hom[index]
long long callNext(CFIGraph* CFI, Graph* H, int index, CFINode2* hom) 
{

    long long numHoms = 0;

    //all nodes are mapped
    if(index >= H->numVertices) {
        return 1;
    }

    Graph& G = CFI->G;

    Node& node = H->nodes[index];

    //nodes in the Graph G with the correct color should be optimized (pre sort the nodes based on the color and than access in O(1))
    vector<int> targetNodes = vector<int>();
    for(int index = 0; index < G.numVertices; index++) {
        Node& n = G.nodes[index];
        if(n.color == node.color) {
            targetNodes.push_back(index);
        }
    }

    for(int targetNode : targetNodes) {
        //the mapping Node
        //std::cout << "[" << index << "] " << "map " << index << " with color " << node.color << " --> " << targetNode << " with color " << G.nodes[index].color << std::endl;

        //BitArray edgeSubset = BitArray(G.numEdges);
        //CFINode2 cfiNode = {targetNode, &edgeSubset};
        hom[index].number = targetNode;
        hom[index].edgeSubset = new BitArray(G.numEdges);

        vector<int> edges = vector<int>();

        for(int edgeIndex = 0; edgeIndex < H->numEdges; edgeIndex++) {
            pair<int,int> edge = H->edgeArray[edgeIndex];
            if(edge.first == index || edge.second == index) {
                int otherNode;
                if(edge.first == index) {
                    otherNode = edge.second;
                }else if(edge.second == index) {
                    otherNode = edge.first;
                }

                if(otherNode > index) {
                    edges.push_back(edgeIndex);
                }else {
                    //there is a opinion on the edge!
                    int opinion = hom[otherNode].edgeSubset->get(edgeIndex);
                    if(opinion) {
                        hom[index].edgeSubset->set(edgeIndex);
                    }
                }
            }
        }

        //std::cout << "[" << index << "] " << "edgeSubset: " << edgeSubset.toString() << std::endl;

        //std::cout << "[" << index << "] " << "edges: ";
        for(int n : edges) {
            pair<int,int> edge = H->edgeArray[n];
            //std::cout << n << "(" << edge.first << "," << edge.second << ") ";
        }
        //std::cout << std::endl;
        
        int n = edges.size();
        int numSubsets = 1 << n;  // 2^n possible subsets

        for (int mask = 0; mask < numSubsets; mask++) {
            //std::cout << "{ ";
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {  // Check if i-th element is in the subset
                    //std::cout << edges[i] << " ";
                    hom[index].edgeSubset->set(edges[i]);
                }else {
                    hom[index].edgeSubset->clear(edges[i]);
                }
            }
            //std::cout << "} ";
            //std::cout << edgeSubset.toString();

            if(hom[index].edgeSubset->hasEvenParity()) {
                //std::cout << " even parity! ";
                int value = callNext(CFI, H, index+1, hom);
                numHoms += value;
                //std::cout << "[" << index << "] " << "returned with value: " << value << " numHoms: " << numHoms << std::endl;
            }

            //std::cout << std::endl;
        }

    }

    return numHoms;
}

long long calculateNumberofHomomorphisms(CFIGraph* CFI, Graph* H)
{
    H->calculateEdgeArray();

    long long numHoms = 0;

    int numEdges = CFI->G.numEdges;
    int numVertices = CFI->G.numVertices;

    std::cout << "numEdges: " << numEdges << std::endl;

    CFINode2* hom = new CFINode2[numVertices];

    numHoms = callNext(CFI, H, 0, hom);

    delete[] hom;

    std::cout << "end" << std::endl;

    return numHoms;

}