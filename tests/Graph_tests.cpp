//
// Created by Christoph Greger on 25.12.24.
//

#include <gtest/gtest.h>
#include <sstream>
#include "Graph.h"
#include "Node.h"

TEST(GraphTest, PrintGraph) {
    Graph graph;
    Node node1, node2, node3;

    graph.addNode(node1);
    graph.addNode(node2);
    graph.addNode(node3);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    std::ostringstream output;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

    graph.printGraph();

    std::cout.rdbuf(oldCoutBuffer);

    std::string expectedOutput = "Graph with 3 vertices and 2 edges\nEdges:\n0 -> 1\n1 -> 2\n";
    EXPECT_EQ(output.str(), expectedOutput);
}


TEST(NodeTest, Equals) {
    Node node1, node2, node3;
    EXPECT_TRUE(node1.equals(node2));
    EXPECT_TRUE(node1.equals(node3));
}

TEST(ColoredNodeTest, Equals) {
    Node cnode1 = Node(1);
    Node cnode2 = Node(1);
    Node cnode3 = Node(2);
    EXPECT_TRUE(cnode1.equals(cnode2));
    EXPECT_FALSE(cnode1.equals(cnode3));
}

TEST(NodesMixed, Equals) {
    Node cnode1 = Node(1);
    Node node = Node(1);
    EXPECT_TRUE(cnode1.equals(node));
    EXPECT_TRUE(node.equals(cnode1));
}

TEST(GraphTest, AddNode) {
    Graph graph;
    Node node1, node2, node3;
    graph.addNode(node1);
    graph.addNode(node2);
    graph.addNode(node3);

    EXPECT_EQ(graph.nodes.size(), 3);
    EXPECT_EQ(graph.numVertices, 3);
}

TEST(GraphTest, CalculateAdjMatrix) {
    Graph graph;
    Node node1, node2, node3;

    graph.addNode(node1);
    graph.addNode(node2);
    graph.addNode(node3);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);

    graph.calculateAdjMatrix();

    EXPECT_EQ(graph.isEdge(0, 1), true);
    EXPECT_EQ(graph.isEdge(1, 0), true);
    EXPECT_EQ(graph.isEdge(1, 2), true);
    EXPECT_EQ(graph.isEdge(2, 1), true);
    EXPECT_EQ(graph.isEdge(0, 2), false);
    EXPECT_EQ(graph.isEdge(2, 0), false);
    EXPECT_EQ(graph.isEdge(0, 0), false);
    EXPECT_EQ(graph.isEdge(1, 1), false);
    EXPECT_EQ(graph.isEdge(2, 2), false);
}



TEST(GraphTest, UncoloredGraphHomomorphisms) {
    Graph G;
    Node node1, node2, node3;
    G.addNode(node1);
    G.addNode(node2);
    G.addNode(node3);
    G.addEdge(0, 1);
    G.addEdge(1, 2);

    Graph H;
    Node node4, node5;
    H.addNode(node4);
    H.addNode(node5);
    H.addEdge(0, 1);

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), 2);

    H.addNode(node3);
    H.addEdge(1, 2);

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), 6);
}

TEST(GraphTest, ColoredGraphHomomorphisms1) {
    Graph G(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    G.addNode(cnode1);
    G.addNode(cnode2);
    G.addNode(cnode3);
    G.addEdge(0, 1);
    G.addEdge(1, 2);

    Graph H(true);
    Node cnode4 = Node(4);
    Node cnode5 = Node(5);
    Node cnode6 = Node(6);
    H.addNode(cnode4);
    H.addNode(cnode5);
    H.addNode(cnode6);
    H.addEdge(0, 1);
    H.addEdge(1, 2);

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), 0);

    cnode6.color = 2;
    H.nodes[2] = cnode6;

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), 0);
}

TEST(GraphTest, SurjectiveHomomorphisms) {
    Graph G(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    G.addNode(cnode1);
    G.addNode(cnode2);
    G.addNode(cnode3);
    G.addEdge(0, 1);
    G.addEdge(1, 2);

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(G), 1);
}

TEST(GraphTest, ColoredHomomorphisms2) {
    Graph G(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    G.addNode(cnode1);
    G.addNode(cnode2);
    G.addNode(cnode3);
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(0, 2);

    Graph H(true);
    Node cnode4 = Node(1);
    Node cnode5 = Node(2);
    Node cnode6 = Node(3);
    Node cnode7 = Node(1);
    Node cnode8 = Node(4);
    H.addNode(cnode4);
    H.addNode(cnode5);
    H.addNode(cnode6);
    H.addNode(cnode7);
    H.addNode(cnode8);
    H.addEdge(0, 1);
    H.addEdge(1, 2);
    H.addEdge(0, 2);
    H.addEdge(1, 3);
    H.addEdge(2, 3);
    H.addEdge(3, 4);
    H.addEdge(2, 4);

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), 2);
}

TEST(GraphTest, UncoloredtoColoredHomomorphisms) {
    Graph G(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    G.addNode(cnode1);
    G.addNode(cnode2);
    G.addNode(cnode3);
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(0, 2);

    Graph H = Graph();
    Node cnode4 = Node();
    Node cnode5 = Node();
    Node cnode6 = Node();
    Node cnode7 = Node();
    Node cnode8 = Node();
    H.addNode(cnode4);
    H.addNode(cnode5);
    H.addNode(cnode6);
    H.addNode(cnode7);
    H.addNode(cnode8);
    H.addEdge(0, 1);
    H.addEdge(1, 2);
    H.addEdge(0, 2);
    H.addEdge(1, 3);
    H.addEdge(2, 3);
    H.addEdge(3, 4);
    H.addEdge(2, 4);

    EXPECT_EQ(G.calculateNumberofHomomorphismsTo(H), 18);
}
