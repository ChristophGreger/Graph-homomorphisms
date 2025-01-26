//
// Created by Christoph Greger on 26.12.24.
//

#include "CFIGraph.h"
#include "EvenSubsetIterator.h"
#include "utilities.h"
#include <iostream>

using namespace std;

CFIGraph::CFIGraph(Graph &G, bool invert): G(G) {

    int counter = 0;
    //G.edges is sorted by the smaller edge
    for (const pair<int, int>& edge : G.edges) {
        int id = edge.first * G.numVertices + edge.second;
        //std::cout << edge.first << ":" << edge.second << " id:" << id << " counter:" << counter << std::endl;
        edgeMap.insert({id,counter});
        counter++;
    }

    G.calculateAdjMatrix();

}

Graph CFIGraph::toGraph() {
    //not required anymore
}

vector<int> CFIGraph::calcEdgeRepresentation(unordered_set<pair<int, int>, PairHash> edges) {

    std::vector<int> a(G.edges.size(), 0);

    for(pair<int,int> edge : edges) {
        a.at(getEdgeId(edge)) = 1;
    }

    return a;

}

bool CFIGraph::isEdge(CFINode node1, CFINode node2) {

    if(! G.isEdge(node1.number, node2.number)) {
        return false;
    }

    int id = getEdgeId({node1.number,node2.number});

    std::cout << "id:" << id << std::endl;

    return node1.edgeSubset[id] == node2.edgeSubset[id];//same opinion on edge between
    
}

//private
int CFIGraph::getEdgeId(pair<int,int> edge) {
    int number1 = edge.first;
    int number2 = edge.second;

    if(number2 < number1) {
        int tmp = number1;
        number1 = number2;
        number2 = tmp;
    }

    int id = number1 * G.numVertices + number2;

    return edgeMap.at(id);
}