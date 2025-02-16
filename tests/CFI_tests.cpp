//
// Created by Christoph Greger on 26.12.24.
//

#include <gtest/gtest.h>
#include "CFIGraph.h"
#include "CFINode.h"
#include "RandomGraphGenerator.h"
#include "utilities.h"
#include "BruteForceAlgo.h"


TEST(CFIGraphTest, edgeRepresentation) {
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

    vector<int> r1 = CFI.calcEdgeRepresentation({{0,1},{1,3}});
    std::vector<int> e1 = {0, 0, 0, 1, 1};
    ASSERT_EQ(r1, e1);

    vector<int> r2 = CFI.calcEdgeRepresentation({{0,1},{1,3},{1,2},{2,3},{0,3}});
    std::vector<int> e2 = {1, 1, 1, 1, 1};
    ASSERT_EQ(r2, e2);

    vector<int> r3 = CFI.calcEdgeRepresentation({});
    std::vector<int> e3 = {0, 0, 0, 0, 0};
    ASSERT_EQ(r3, e3);

    //create CFINodes

    vector<int> r4 = CFI.calcEdgeRepresentation({{1,2},{1,0}});
    std::vector<int> e4 = {1, 0, 0, 0, 1};
    ASSERT_EQ(r4, e4);

    CFINode c1 = CFINode(1, r4);

    vector<int> r5 = CFI.calcEdgeRepresentation({{1,3},{3,2}});
    std::vector<int> e5 = {0, 1, 0, 1, 0};
    ASSERT_EQ(r5, e5);

    CFINode c2 = CFINode(3, r5);

    ASSERT_EQ(CFI.isEdge(c1,c2), false);

    vector<int> r6 = CFI.calcEdgeRepresentation({{0,3},{3,2}});
    std::vector<int> e6 = {0, 1, 1, 0, 0};
    ASSERT_EQ(r6, e6);

    CFINode c3 = CFINode(3, r6);

    ASSERT_EQ(CFI.isEdge(c1,c3), true);
    
}

TEST(CFIGraphTest, ByHomomorphisms3) {
    int vertices = 20;
    int edges = 40;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    unsigned long time = clock();
    long long numHoms = calculateNumberofHomomorphisms(&CFI, &G);
    ASSERT_EQ(numHoms, intPow(2, (edges - vertices + 1)));
    cout << intPow(2, (edges - vertices + 1)) << endl;
    cout << "Time in ms: " << (clock() - time)/1000 << endl;
    cout << "Calculated successfully" << endl;
}

/*
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
*/



