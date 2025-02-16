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
long long callNext(int index, CFINode2* hom, const StaticData& d) 
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
                int value = callNext(index+1, hom, d);
                numHoms += value;
            }
        }

    }

    return numHoms;
}

vector<vector<int>> sortyByColor(Graph* G) {

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

void print(const vector<vector<pair<int,int>>>& list) {
    for (size_t index = 0; index < list.size(); index++) {
        cout << "index " << index << ": ";
        for (pair<int,int> nodeIndex : list[index]) {
            cout << nodeIndex.first << "," << nodeIndex.second << " ";
        }
        cout << endl;
    }
}

pair<vector<vector<pair<int,int>>>,vector<vector<pair<int,int>>>> sortEdgesTopo(Graph* G) {

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

long long calcHoms(CFIGraph& CFI, Graph& H)
{
    H.calculateEdgeArray();

    long long numHoms = 0;

    int numEdgesG = CFI.G.numEdges;
    int numVerticesH = H.numVertices;

    std::cout << "calcHoms(" << "CFI->G{numEdges:" << CFI.G.numEdges << ", numVertices: " << CFI.G.numVertices << "} H{numEdges:" << H.numEdges << ", numVertices: " << H.numVertices << "})" << std::endl;

    CFINode2* hom = new CFINode2[numVerticesH];

    for(int i = 0; i < numVerticesH; i++) {
        hom[i].edgeSubset = new BitArray(numEdgesG);
    }

    vector<vector<int>> colorBuckets = sortyByColor(&CFI.G);
    pair<vector<vector<pair<int,int>>>,vector<vector<pair<int,int>>>> sortedEdgesH = sortEdgesTopo(&H);
    vector<vector<pair<int,int>>> backwardEdgesH = sortedEdgesH.first;
    vector<vector<pair<int,int>>> forwardEdgesH = sortedEdgesH.second;

    const StaticData d = {CFI, H, colorBuckets, forwardEdgesH, backwardEdgesH};

    numHoms = callNext(0, hom, d);

    for (int i = 0; i < numVerticesH; i++) {
        delete hom[i].edgeSubset;
    }
    delete[] hom;

    return numHoms;

}