//
// Created by Christoph Greger on 26.12.24.
//

#include "CFIGraph.h"
#include "EvenSubsetIterator.h"
#include "utilities.h"


//The Graph has to be surjectively colored
CFIGraph::CFIGraph(Graph &G) : numofVertices(0), numofEdges(0) {

    //Store the nodes of G with its edges
    unordered_map<int, vector<int>> nodeEdges;
    for (int i = 0; i < G.numVertices; i++) {
        vector<int> edgesofx;
        nodeEdges.insert({i, edgesofx});
    }
    for (const pair<int, int>& edge : G.edges) {
        nodeEdges[edge.first].push_back(edge.second);
        nodeEdges[edge.second].push_back(edge.first);
    }

    //Create the CFINodes
    for (int i = 0; i < G.numVertices; i++) {
        for (auto x : EvenSubsetIterator<int>(nodeEdges[i])) {
            CFINode node = CFINode(G.nodes[i].color, i, x.first, x.second);
            nodes.push_back(node);
            numofVertices++;
        }
    }

    //Create the edges
    for (int i = 0; i < nodes.size(); i++) {
        int currnumber = nodes[i].number;
        for(int j = i; j < nodes.size(); j++) {
            if (contains(nodes[i].ownedEdges, nodes[j].number) && contains(nodes[j].ownedEdges, currnumber)
                || contains(nodes[i].notownedEdges, nodes[j].number) && contains(nodes[j].notownedEdges, currnumber)) {
                edges.insert({i, j});
            }
        }
    }
    numofEdges = static_cast<int>(edges.size());
    numofVertices = static_cast<int>(nodes.size());

}

Graph CFIGraph::toGraph() {
    Graph G = Graph(true);
    for (int i = 0; i < numofVertices; i++) {
        G.addNode(Node(true, nodes[i].color));
    }
    for (const pair<int, int>& edge : edges) {
        G.addEdge(edge.first, edge.second);
    }
    return G;
}

