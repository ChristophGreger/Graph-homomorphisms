//
// Created by Christoph Greger on 26.12.24.
//

#include "CFIGraph.h"
#include "EvenSubsetIterator.h"
#include "utilities.h"

using namespace std;

CFIGraph::CFIGraph(Graph &G, bool inverted) : numofVertices(0), numofEdges(0), inverted(inverted) {

    if (inverted && G.numVertices < 2) {
        throw invalid_argument("Graph has to have at least 2 vertices to be inverted");
    }

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

    int forinverting1 = 0;
    int forinverting2 = nodeEdges[0][0];

    //Create the edges
    for (int i = 0; i < nodes.size(); i++) {
        int currnumber = nodes[i].number;
        for(int j = i; j < nodes.size(); j++) {

            if (contains(nodes[i].ownedEdges, nodes[j].number) && contains(nodes[j].ownedEdges, currnumber)
                || contains(nodes[i].notownedEdges, nodes[j].number) && contains(nodes[j].notownedEdges, currnumber)) {

                //Check for the inverted case (we switch the edges between the 0 and 1 nodes)
                if (inverted && nodes[i].number == forinverting1 && nodes[j].number == forinverting2) {
                    continue;
                }

                //Normal case
                edges.insert({i, j});
            } else {

                //Check for the inverted case (we switch the edges between the 0 and 1 nodes)
                if (inverted && nodes[i].number == forinverting1 && nodes[j].number == forinverting2) {
                    edges.insert({i, j});
                }
            }
        }
    }
    numofEdges = static_cast<int>(edges.size());
    numofVertices = static_cast<int>(nodes.size());

}


Graph CFIGraph::toGraph() {
    Graph G = Graph(true);
    for (int i = 0; i < numofVertices; i++) {
        G.addNode(Node(nodes[i].color));
    }
    for (const pair<int, int>& edge : edges) {
        G.addEdge(edge.first, edge.second);
    }
    return G;
}

