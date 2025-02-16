#include "BruteForceAlgo.h"
#include "BitArray.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

using namespace std;

/*
 * Recursive call
 * map node with number index in Graph H to a corresponding CFINode
 * @param index
 * @param hom is the current mapping
 * @param d is contains the CFI Graph, the mapped Graph H and some pre generated vectors
 */
long long setNextNode(int index, CFINode* hom, const StaticData& d) 
{
    long long numHoms = 0;

    const Graph& G = d.CFI.G;
    const Graph& H = d.H;

    //all nodes are mapped
    if(index >= H.numVertices) {
        return 1;
    }

    const Node& node = H.nodes[index];

    //nodes in the Graph G with the correct color
    const vector<int>& targetNodes = d.colorBucketG[node.color];

    hom[index].edgeSubset->reset();

    for(int targetNode : targetNodes) {

        hom[index].number = targetNode;

        const vector<pair<int,int>>& forwardEdges = d.forwardEdgesH[index];
        const vector<pair<int,int>>& backwardEdges = d.backwardEdgesH[index];

        for(const pair<int,int>& backwardEdge : backwardEdges) {
            int otherNode = backwardEdge.first;
            int edgeIndex = backwardEdge.second;
            int opinion = hom[otherNode].edgeSubset->get(edgeIndex);
            if(opinion) {
                hom[index].edgeSubset->set(edgeIndex);
            }
        }
        
        int n = forwardEdges.size();
        int numSubsets = 1 << n;  // 2^n possible subsets

        for (int mask = 0; mask < numSubsets; mask++) {
            for (int i = 0; i < n; i++) {
                int edgeIndex = forwardEdges[i].second;
                if (mask & (1 << i)) {  // Check if i-th element is in the subset
                    hom[index].edgeSubset->set(edgeIndex);
                }else {
                    hom[index].edgeSubset->clear(edgeIndex);
                }
            }

            if(hom[index].edgeSubset->hasEvenParity()) {
                int value = setNextNode(index+1, hom, d);
                numHoms += value;
            }
        }

    }

    return numHoms;
}

/*
 * Search the Graph G for nodes with the same color and combine them in a vector<int>
 */
vector<vector<int>> createColorBuckets(Graph* G) {

    int highestColor = 0;

    for(Node& n : G->nodes) {
        if(n.color > highestColor) {
            highestColor = n.color;
        }
    }

    vector<vector<int>> list(highestColor+1);

    for(int i = 0; i < highestColor+1; i++) {
        for(int index = 0; index < G->numVertices; index++) {
            Node& n = G->nodes[index];
            if(n.color == i) {
                list[i].push_back(index);
            }
        }
    }

    return list;
}

//debug
void print(const vector<vector<pair<int,int>>>& list) {
    for (size_t index = 0; index < list.size(); index++) {
        cout << "index " << index << ": ";
        for (pair<int,int> nodeIndex : list[index]) {
            cout << nodeIndex.first << "," << nodeIndex.second << " ";
        }
        cout << endl;
    }
}

/*
 * Process to create a list of forward and backward edges. 
 * The data in the lists are in the form {nodeIndex, edgeIndex}.
 * These dataset are than added to lists representing all the forward(or backward) edges of one node
 * So in the end we can in O(1) access all the forward(and backward) edges of a node.
 * In the return value the first list are the backward edges.
 */
pair<vector<vector<pair<int,int>>>,vector<vector<pair<int,int>>>> createForwardBackwardEdges(Graph* G) {

    vector<vector<pair<int,int>>> backwardEdges(G->numVertices);
    vector<vector<pair<int,int>>> forwardEdges(G->numVertices);

    int index = 0;
    for(int edgeIndex = 0; edgeIndex < G->numEdges; edgeIndex++) {
        pair<int,int> edge = G->edgeArray[edgeIndex];
        int i = edge.first;
        int j = edge.second;
        if(i > j) {
            swap(i,j);
        }

        forwardEdges[i].push_back({j,edgeIndex});
        backwardEdges[j].push_back({i,edgeIndex});
    }

    return {backwardEdges, forwardEdges};
}

/*
 * Calc homomorphisms from H to the CFI Graph.
 * First some preprocessing is done (creating the color buckets of CFI.G or the creation of the forward/backward edges of H).
 * Then the inital call to setNextNode(0, hom, d) is executed.
 */
long long calcHoms(CFIGraph& CFI, Graph& H)
{
    H.calculateEdgeArray();

    int numEdgesG = CFI.G.numEdges;
    int numVerticesH = H.numVertices;

    std::cout << "calcHoms(" << "CFI->G{numEdges:" << CFI.G.numEdges << ", numVertices: " << CFI.G.numVertices << "} H{numEdges:" << H.numEdges << ", numVertices: " << H.numVertices << "})" << std::endl;

    //store the current mapping
    CFINode* hom = new CFINode[numVerticesH];

    for(int i = 0; i < numVerticesH; i++) {
        hom[i].edgeSubset = std::make_unique<BitArray>(numEdgesG);
    }

    vector<vector<int>> colorBuckets = createColorBuckets(&CFI.G);
    pair<vector<vector<pair<int,int>>>,vector<vector<pair<int,int>>>> sortedEdgesH = createForwardBackwardEdges(&H);
    vector<vector<pair<int,int>>> backwardEdgesH = sortedEdgesH.first;
    vector<vector<pair<int,int>>> forwardEdgesH = sortedEdgesH.second;

    const StaticData d = {CFI, H, colorBuckets, forwardEdgesH, backwardEdgesH};

    long long numHoms = setNextNode(0, hom, d);

    delete[] hom;

    return numHoms;

}