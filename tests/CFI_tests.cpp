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


TEST(CFIGraphTest, ByHomomorphisms) {
    for (int times = 0; times < 10; times++) {
        for (int vertices = 2; vertices < 9; vertices++) {
            for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
                for (int i = 0; i < 3; i++) {
                    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
                    CFIGraph CFI = CFIGraph(G);
                    Graph H = CFI.toGraph();
                    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
                    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
                    long long homs = G.calculateNumberofHomomorphismsTo(H);
                    long long expected = intPow(2, (edges - vertices + 1));
                    if (homs != expected) {
                        cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
                        G.printGraph(true);
                        cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
                        H.printGraph(true);
                    }
                    ASSERT_EQ(homs, expected);
                    cout << "Calculated successfully" << endl;
                }
            }
        }
    }
}

TEST(CFIGraphTest, ByHomomorphisms2) {
    int vertices = 10;
    int edges = 30;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
    ASSERT_EQ(G.calculateNumberofHomomorphismsTo(H), intPow(2, (edges - vertices + 1)));
    cout << "Calculated successfully" << endl;

}

TEST(CFIGraphTest, ByHomomorphismsInverted) {
    for (int vertices = 2; vertices < 6; vertices++) {
        for (int edges = vertices - 1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
            for (int i = 0; i < 3; i++) {
                RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                Graph G = randomGraphGenerator.generateRandomConnectedGraph();
                CFIGraph CFI = CFIGraph(G, true);
                Graph H = CFI.toGraph();
                ASSERT_EQ(G.calculateNumberofHomomorphismsTo(H), 0);
            }
        }
    }
}

TEST(CFIGraphTest, ByHomomorphismsInverted2) {
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

    CFIGraph CFI = CFIGraph(G, true);

    EXPECT_EQ(CFI.numofVertices, 12);
    EXPECT_EQ(CFI.numofEdges, 24);
}


TEST(CFIGraphTest, ByHomomorphisms3) {
    int vertices = 20;
    int edges = 40;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
    unsigned long time = clock();
    ASSERT_EQ(G.calculateNumberofHomomorphismsTo(H), intPow(2, (edges - vertices + 1)));
    cout << intPow(2, (edges - vertices + 1)) << endl;
    cout << "Time in ms: " << (clock() - time)/1000 << endl;
    cout << "Calculated successfully" << endl;

}


