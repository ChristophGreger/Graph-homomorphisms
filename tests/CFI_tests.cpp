//
// Created by Christoph Greger on 26.12.24.
//

#include <gtest/gtest.h>

#include "CalcHoms.h"
#include "CFIGraph.h"
#include "RandomGraphGenerator.h"
#include "utilities.h"

TEST(CFIGraphTest, Constructor) {
    GraphTemplate Gt = GraphTemplate(true);
    Node node0 = Node(0);
    Node node1 = Node(1);
    Node node2 = Node(2);
    Node node3 = Node(3);
    Gt.addNode(node0);
    Gt.addNode(node1);
    Gt.addNode(node2);
    Gt.addNode(node3);
    Gt.addEdge(0, 1);
    Gt.addEdge(0, 3);
    Gt.addEdge(1, 3);
    Gt.addEdge(3, 2);
    Gt.addEdge(2, 1);

    Graph G = Graph(Gt);

    CFIGraph CFI = CFIGraph(G);

    EXPECT_EQ(CFI.numofVertices, 12);
    EXPECT_EQ(CFI.numofEdges, 24);
}


TEST(CFIGraphTest, ByHomomorphisms) {
    for (int times = 0; times < 1; times++) {
        for (int vertices = 2; vertices < 9; vertices++) {
            for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
                for (int i = 0; i < 2; i++) {
                    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
                    CFIGraph CFI = CFIGraph(G);
                    Graph H = CFI.toGraph();
                    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
                    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
                    long long homs = CalcHoms::calcNumHoms(G,H);
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
    ASSERT_EQ(CalcHoms::calcNumHoms(G,H), intPow(2, (edges - vertices + 1)));
    cout << "Calculated successfully" << endl;
}

TEST(CFIGraphTest, ByHomomorphismsInverted) {
    for (int vertices = 2; vertices < 6; vertices++) {
        for (int edges = vertices - 1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
            for (int i = 0; i < 1; i++) {
                RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                Graph G = randomGraphGenerator.generateRandomConnectedGraph();
                CFIGraph CFI = CFIGraph(G, true);
                Graph H = CFI.toGraph();
                ASSERT_EQ(CalcHoms::calcNumHoms(G,H), 0);
            }
        }
    }
}

TEST(CFIGraphTest, ByHomomorphismsInverted2) {
    GraphTemplate Gt = GraphTemplate(true);
    Node node0 = Node(0);
    Node node1 = Node(1);
    Node node2 = Node(2);
    Node node3 = Node(3);
    Gt.addNode(node0);
    Gt.addNode(node1);
    Gt.addNode(node2);
    Gt.addNode(node3);
    Gt.addEdge(0, 1);
    Gt.addEdge(0, 3);
    Gt.addEdge(1, 3);
    Gt.addEdge(3, 2);
    Gt.addEdge(2, 1);

    Graph G = Graph(Gt);

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
    ASSERT_EQ(CalcHoms::calcNumHoms(G,H), intPow(2, (edges - vertices + 1)));
    cout << intPow(2, (edges - vertices + 1)) << endl;
    cout << "Time in ms: " << (clock() - time)/1000 << endl;
    cout << "Calculated successfully" << endl;
}



