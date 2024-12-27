//
// Created by Christoph Greger on 26.12.24.
//

#include <gtest/gtest.h>
#include "CFIGraph.h"
#include "RandomGraphGenerator.h"
#include "utilities.h"

TEST(CFIGraphTest, Constructor) {
    Graph G = Graph(true);
    Node node0 = Node(0);
    Node node1 = Node(1);
    Node node2 = Node(2);
    Node node3 = Node(3);
    G.addNode(node0);
    G.addNode(node1);
    G.addNode(node2);
    G.addNode(node3);
    G.addEdge(0, 1);
    G.addEdge(0, 3);
    G.addEdge(1, 3);
    G.addEdge(3, 2);
    G.addEdge(2, 1);

    CFIGraph CFI = CFIGraph(G);

    EXPECT_EQ(CFI.numofVertices, 12);
    EXPECT_EQ(CFI.numofEdges, 24);
}

//Takes 8 (!!!???) seconds to run, but seems to work
TEST(CFIGraphTest, ByHomomorphisms) {
    int vertices = 5;
    int edges = 9;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), intPow(2, (edges-vertices+1)));
}

TEST(CFIGraphTest, ByHomomorphismsInverted) {
    int vertices = 5;
    int edges = 9;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G, true);
    Graph H = CFI.toGraph();
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H),0);
}


