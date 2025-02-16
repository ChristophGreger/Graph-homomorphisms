#ifndef GRAPH_HOMOMORPHISMS_BRUTEFORCEALGO_H
#define GRAPH_HOMOMORPHISMS_BRUTEFORCEALGO_H

#include "CFIGraph.h"
#include "Graph.h"
#include "BitArray.h"

using namespace std;

struct StaticData {
    const CFIGraph& CFI;
    const Graph& H;
    const vector<vector<int>>& colorBucketG;
    const vector<vector<pair<int,int>>>& forwardEdgesH;
    const vector<vector<pair<int,int>>>& backwardEdgesH;
};

long long calcHoms(CFIGraph& G, Graph& H);

long long callNext(int index, CFINode* hom, const StaticData& d);

#endif