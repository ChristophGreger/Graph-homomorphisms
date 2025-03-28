//
// Created by Christoph Greger on 25.12.24.
//

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <vector>

#include "CalcHoms.h"
#include "CFIGraph.h"
#include "Graph.h"
#include "GraphTemplate.h"  // New header for GraphTemplate
#include "Node.h"
#include "RandomGraphGenerator.h"

using std::cout;
using std::endl;

TEST(GraphTest, PrintGraph) {
    GraphTemplate t(false);
    Node node1, node2, node3;
    t.addNode(node1);
    t.addNode(node2);
    t.addNode(node3);
    t.addEdge(0, 1);
    t.addEdge(1, 2);

    // Convert the fully built template into a Graph.
    Graph graph(t);

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
    GraphTemplate t(false);
    Node node1, node2, node3;
    t.addNode(node1);
    t.addNode(node2);
    t.addNode(node3);

    // After constructing the template, convert it.
    Graph graph(t);

    EXPECT_EQ(graph.nodes.size(), 3);
    EXPECT_EQ(graph.numVertices, 3);
}

TEST(GraphTest, CalculateAdjMatrix) {
    GraphTemplate t(false);
    Node node1, node2, node3;
    t.addNode(node1);
    t.addNode(node2);
    t.addNode(node3);
    t.addEdge(0, 1);
    t.addEdge(1, 2);
    Graph graph(t);

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
    // First Graph G:
    GraphTemplate tG(false);
    Node node1, node2, node3;
    tG.addNode(node1);
    tG.addNode(node2);
    tG.addNode(node3);
    tG.addEdge(0, 1);
    tG.addEdge(1, 2);
    Graph G(tG);

    // First Graph H:
    GraphTemplate tH(false);
    Node node4, node5;
    tH.addNode(node4);
    tH.addNode(node5);
    tH.addEdge(0, 1);
    Graph H(tH);

    EXPECT_EQ(CalcHoms::calcNumHoms(G,H), 2);

    // To “modify” H, we need to build a new template.
    GraphTemplate tH2(false);
    tH2.addNode(node4);
    tH2.addNode(node5);
    tH2.addNode(node3);  // Additional node added.
    tH2.addEdge(0, 1);
    tH2.addEdge(1, 2);
    Graph H2(tH2);

    EXPECT_EQ(CalcHoms::calcNumHoms(G,H2), 6);
}


TEST(GraphTest, ColoredGraphHomomorphisms1) {
    GraphTemplate tG(true);
    GraphTemplate tH(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    tG.addNode(cnode1);
    tG.addNode(cnode2);
    tG.addNode(cnode3);
    tG.addEdge(0, 1);
    tG.addEdge(1, 2);
    Graph G(tG);

    Node cnode4 = Node(4);
    Node cnode5 = Node(5);
    Node cnode6 = Node(6);
    tH.addNode(cnode4);
    tH.addNode(cnode5);
    tH.addNode(cnode6);
    tH.addEdge(0, 1);
    tH.addEdge(1, 2);
    Graph H(tH);

    EXPECT_EQ(CalcHoms::calcNumHoms(G,H), 0);

    // To “modify” H (set color of node at index 2), build a new template.
    GraphTemplate tH2(true);
    tH2.addNode(cnode4);
    tH2.addNode(cnode5);
    // Modify cnode6 before adding:
    cnode6.color = 2;
    tH2.addNode(cnode6);
    tH2.addEdge(0, 1);
    tH2.addEdge(1, 2);
    Graph H2(tH2);

    EXPECT_EQ(CalcHoms::calcNumHoms(G,H2), 0);
}


TEST(GraphTest, SurjectiveHomomorphisms) {
    GraphTemplate t(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    t.addNode(cnode1);
    t.addNode(cnode2);
    t.addNode(cnode3);
    t.addEdge(0, 1);
    t.addEdge(1, 2);
    Graph G(t);

    EXPECT_EQ(CalcHoms::calcNumHoms(G,G), 1);
}


TEST(GraphTest, ColoredHomomorphisms2) {
    GraphTemplate tG(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    tG.addNode(cnode1);
    tG.addNode(cnode2);
    tG.addNode(cnode3);
    tG.addEdge(0, 1);
    tG.addEdge(1, 2);
    tG.addEdge(0, 2);
    Graph G(tG);

    GraphTemplate tH(true);
    Node cnode4 = Node(1);
    Node cnode5 = Node(2);
    Node cnode6 = Node(3);
    Node cnode7 = Node(1);
    Node cnode8 = Node(4);
    tH.addNode(cnode4);
    tH.addNode(cnode5);
    tH.addNode(cnode6);
    tH.addNode(cnode7);
    tH.addNode(cnode8);
    tH.addEdge(0, 1);
    tH.addEdge(1, 2);
    tH.addEdge(0, 2);
    tH.addEdge(1, 3);
    tH.addEdge(2, 3);
    tH.addEdge(3, 4);
    tH.addEdge(2, 4);
    Graph H(tH);

    EXPECT_EQ(CalcHoms::calcNumHoms(G,H), 2);
}


TEST(GraphTest, UncoloredtoColoredHomomorphisms) {
    // Build a colored graph G.
    GraphTemplate tG(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    tG.addNode(cnode1);
    tG.addNode(cnode2);
    tG.addNode(cnode3);
    tG.addEdge(0, 1);
    tG.addEdge(1, 2);
    tG.addEdge(0, 2);
    Graph G(tG);

    // Build an uncolored graph H.
    GraphTemplate tH(false);
    Node nnode1 = Node();
    Node nnode2 = Node();
    Node nnode3 = Node();
    Node nnode4 = Node();
    Node nnode5 = Node();
    tH.addNode(nnode1);
    tH.addNode(nnode2);
    tH.addNode(nnode3);
    tH.addNode(nnode4);
    tH.addNode(nnode5);
    tH.addEdge(0, 1);
    tH.addEdge(1, 2);
    tH.addEdge(0, 2);
    tH.addEdge(1, 3);
    tH.addEdge(2, 3);
    tH.addEdge(3, 4);
    tH.addEdge(2, 4);
    Graph H(tH);

    H.printGraph();
    G.printGraph();

    cout << "hi" << endl;
    cout << CalcHoms::calcNumHoms(G,H) << endl;

    EXPECT_EQ(CalcHoms::calcNumHoms(G,H), 18);
}


/*
TEST(GraphTest, calculateNumberofInjectiveHomomorphismsTo) {
    GraphTemplate t(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    Node cnode4 = Node(4);
    t.addNode(cnode1);
    t.addNode(cnode2);
    t.addNode(cnode3);
    t.addNode(cnode4);
    t.addEdge(0, 1);
    t.addEdge(1, 2);
    t.addEdge(2, 3);
    t.addEdge(3, 0);
    Graph G(t);

    ASSERT_EQ(G.calculateNumberofInjectiveHomomorphismsTo(G), 1);

    // If "colored" is a property that can be changed, assume it is mutable
    // on the Graph object. Otherwise, rebuild a new template.
    G.colored = false;

    ASSERT_EQ(G.calculateNumberofInjectiveHomomorphismsTo(G), 8);
}
*/


TEST(GraphTest, calculateNodeIndex) {
    GraphTemplate t(false);
    Node n1 = Node(1);
    Node n2 = Node(2);
    Node n3 = Node(3);
    Node n4 = Node(4);
    t.addNode(n1);
    t.addNode(n2);
    t.addNode(n3);
    t.addNode(n4);
    t.addEdge(0, 1);
    t.addEdge(1, 2);
    t.addEdge(2, 3);
    t.addEdge(3, 0);
    Graph G(t);

    int* nodeIndex = G.nodeIndex;

    cout << "Edges: \n";
    for (int i = 0; i < G.edges.size(); i++) {
        cout << G.edgeArray[i].first << " -> " << G.edgeArray[i].second << "\n";
    }

    cout << "NodeIndex: ";
    for (int i = 0; i < 4; i++) {
        cout << nodeIndex[i] << " ";
    }

    int shouldbe[] = {0, 1, 2, 4};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(nodeIndex[i], shouldbe[i]);
    }
}


TEST(GraphTest, calculateNodeIndexAutomatic) {
    RandomGraphGenerator generator = RandomGraphGenerator(100, 300, false);
    Graph G = generator.generateRandomConnectedGraph();
    int* nodeIndex = G.nodeIndex;
    ASSERT_EQ(nodeIndex[0], 0);
    for (int i = 1; i < G.numVertices; i++) {
        for (int j = nodeIndex[i-1]; j < nodeIndex[i]; j++) {
            ASSERT_EQ(G.edgeArray[j].second, i);
        }
    }
    ASSERT_EQ(nodeIndex[G.numVertices - 1], G.edges.size());
}


TEST(GraphTest, coloredHoms) {
    for (int times = 0; times < 1000; times++) {
        cout << times << endl;
        GraphTemplate t(true);
        for (int i = 0; i < 4; i++) {
            Node node = Node(i);
            t.addNode(node);
        }
        t.addEdge(0, 1);
        t.addEdge(0, 3);
        t.addEdge(1, 2);
        Graph pattern(t);

        CFIGraph CFI = CFIGraph(pattern);
        Graph input = CFI.toGraph();

        ASSERT_EQ(CalcHoms::calcNumHoms(pattern,input), 1);
    }
}


TEST(GraphTest, coloredHoms2) {
    for (int times = 0; times < 1000; times++) {
        GraphTemplate t1(true);
        for (int i = 0; i < 4; i++) {
            Node node = Node(i);
            t1.addNode(node);
        }
        t1.addEdge(0, 3);
        t1.addEdge(1, 3);
        t1.addEdge(2, 3);
        Graph pattern(t1);

        GraphTemplate t2(true);
        for (int i = 0; i < 4; i++) {
            int color = 0;
            switch (i) {
                case 0: color = 0; break;
                case 1: color = 1; break;
                case 2: color = 2; break;
                default: color = 3; break;
            }
            Node node = Node(color);
            t2.addNode(node);
        }
        t2.addEdge(0, 3);
        t2.addEdge(0, 6);
        t2.addEdge(1, 3);
        t2.addEdge(1, 4);
        t2.addEdge(2, 3);
        t2.addEdge(2, 5);
        Graph input(t2);

        ASSERT_EQ(CalcHoms::calcNumHoms(pattern,input), 1);
    }
}


TEST(GRAPH_Test, Neighbors) {
    GraphTemplate t(true);
    for (int i = 0; i < 4; i++) {
        t.addNode(Node(i));
    }
    t.addEdge(0, 1);
    t.addEdge(0, 2);
    t.addEdge(1, 2);
    t.addEdge(1, 3);
    t.addEdge(2, 3);
    Graph S(t);

    auto neighbors = S.neighbours;

    for (int i = 0; i < 4; i++) {
        cout << "Node: " << i << endl;
        for (int neighbor : neighbors[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}


TEST(GRAPH_Test, Neighbors2) {
    GraphTemplate t(true);
    for (int i = 0; i < 4; i++) {
        t.addNode(Node(i));
    }
    t.addNode(Node(0));
    t.addEdge(0, 1);
    t.addEdge(0, 2);
    t.addEdge(1, 2);
    t.addEdge(1, 3);
    t.addEdge(2, 3);
    t.addEdge(4, 1);
    Graph H(t);

    auto degree = H.degree; // Expected: 2 4 3 2 1
    auto expected = std::vector<int>{2, 4, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        cout << expected[i] << " " << degree[i] << endl;
        ASSERT_EQ(degree[i], expected[i]);
    }
}


TEST(GRAPH_Test, Degree) {
    GraphTemplate t(true);
    for (int i = 0; i < 4; i++) {
        t.addNode(Node(i));
    }
    t.addEdge(0, 1);
    t.addEdge(0, 2);
    t.addEdge(1, 2);
    t.addEdge(1, 3);
    t.addEdge(2, 3);
    Graph S(t);

    auto degree = S.degree;
    for (int i = 0; i < 4; i++) {
        cout << "Node: " << i << " Degree: " << degree[i] << endl;
    }
}


TEST(GRAPH_Test, ShrinkGraph) {
    // Build first graph S via its template.
    GraphTemplate tS(true);
    for (int i = 0; i < 4; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    tS.addEdge(2, 3);
    Graph S(tS);

    // Build second graph H via its template.
    GraphTemplate tH(true);
    for (int i = 0; i < 4; i++) {
        tH.addNode(Node(i));
    }
    tH.addNode(Node(0));
    tH.addEdge(0, 1);
    tH.addEdge(0, 2);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    tH.addEdge(2, 3);
    tH.addEdge(4, 1);
    Graph H(tH);

    cout << "pre result " << endl;
    auto shrinked = H.shrinkGraph(S);
    cout << "result: " << shrinked.first << endl;
    ASSERT_EQ(shrinked.first, true);
    shrinked.second.printGraph(true);
    cout << "homs: " << CalcHoms::calcNumHoms(shrinked.second,S) << endl;
    ASSERT_EQ(CalcHoms::calcNumHoms(shrinked.second,S), 1);
    S.printGraph(true);
}


TEST(Graph_Explanation, test) {
    RandomGraphGenerator generator = RandomGraphGenerator(10, 20, false);
    Graph G = generator.generateRandomConnectedGraph();

    int* nodeIndex = G.nodeIndex;
    for (int i = 0; i < G.edges.size(); i++) {
        cout << G.edgeArray[i].first << " -> " << G.edgeArray[i].second << endl;
    }

    cout << "NodeIndex: ";
    for (int i = 0; i < G.numVertices; i++) {
        cout << nodeIndex[i] << " ";
    }
    cout << endl;
}


TEST(GRAPH_Shrink, shrink2) {
    GraphTemplate tS(true);
    for (int i = 0; i < 5; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(0, 3);
    tS.addEdge(0, 4);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    Graph S(tS);

    GraphTemplate tH(true);
    tH.addNode(Node(2));
    tH.addNode(Node(0));
    tH.addNode(Node(2));
    tH.addNode(Node(4));
    tH.addEdge(1, 0);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    Graph H(tH);

    auto shrunken = H.shrinkGraph(S);
    ASSERT_EQ(shrunken.first, true);
    cout << "Shrunken Graph: " << endl;
    shrunken.second.printGraph(true);
    cout << "Shrunken with: " << endl;
    S.printGraph(true);
}


TEST(GraphConnectedComponents, SingleComponent) {
    GraphTemplate t(false);
    for (int i = 0; i < 4; i++) {
        t.addNode(Node());
    }
    t.addEdge(0, 1);
    t.addEdge(1, 2);
    t.addEdge(2, 3);
    Graph graph(t);

    auto components = graph.connectedComponents();
    cout << "Components: " << components.size() << endl;
    // Expect exactly one connected component.
    ASSERT_EQ(components.size(), 1);
    // The component should have the same number of nodes.
    cout << "components[0].numVertices: " << components[0].numVertices << endl;
    ASSERT_EQ(components[0].numVertices, 4);
    cout << "wath" << endl;
}


TEST(GraphConnectedComponents, TwoComponents) {
    GraphTemplate t(false);
    for (int i = 0; i < 6; i++) {
        t.addNode(Node());
    }
    // Component 1
    t.addEdge(0, 1);
    t.addEdge(1, 2);
    // Component 2
    t.addEdge(3, 4);
    t.addEdge(4, 5);
    Graph graph(t);

    auto components = graph.connectedComponents();
    ASSERT_EQ(components.size(), 2);

    int comp1Size = 0, comp2Size = 0;
    for (const auto &comp : components) {
        if (comp.numVertices == 3) {
            if (comp1Size == 0)
                comp1Size = comp.numVertices;
            else
                comp2Size = comp.numVertices;
        }
    }
    ASSERT_EQ(comp1Size, 3);
    ASSERT_EQ(comp2Size, 3);
}


TEST(GraphConnectedComponents, IsolatedVertices) {
    GraphTemplate t(false);
    for (int i = 0; i < 5; i++) {
        t.addNode(Node());
    }
    Graph graph(t);

    auto components = graph.connectedComponents();
    // Each vertex is a component.
    ASSERT_EQ(components.size(), 5);
    for (const auto &comp : components) {
        ASSERT_EQ(comp.numVertices, 1);
    }
}

TEST(GraphTest, TreeWidth) {
    for (int i = 1; i < 10; i++) {
        GraphTemplate t(false);
        for (int j = 0; j < i; j++) {
            t.addNode(Node(j));
        }
        for (int u = 0; u < i; u++) {
            for (int v = u + 1; v < i; v++) {
                t.addEdge(u, v);
            }
        }
        Graph G(t);
        int treeWidth = G.getTreeWidth();
        cout << "Treewidth of the " << i << "-clique: " << treeWidth << endl;
        ASSERT_EQ(treeWidth, i - 1);
    }
}


// Helper to create a triangle graph (cycle of 3 vertices)
Graph createTriangleGraph() {
    GraphTemplate gt(false);
    // Add three nodes.
    for (int i = 0; i < 3; ++i) {
        gt.addNode(Node());
    }
    // Add edges in one order.
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    gt.addEdge(0, 2);
    return Graph(gt);
}

// Helper to create a triangle graph with edge order reversed
Graph createTriangleGraphDifferentOrder() {
    GraphTemplate gt(false);
    for (int i = 0; i < 3; ++i) {
        gt.addNode(Node());
    }
    // Add the same edges in different order.
    gt.addEdge(1, 2);
    gt.addEdge(0, 2);
    gt.addEdge(0, 1);
    return Graph(gt);
}

// Helper to create a non-isomorphic graph: a simple path with 3 vertices.
Graph createPathGraph() {
    GraphTemplate gt(false);
    for (int i = 0; i < 3; ++i) {
        gt.addNode(Node());
    }
    // Create a path: 0-1, 1-2.
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    return Graph(gt);
}

TEST(CanonicalStringUncoloredTest, IsomorphicGraphsHaveSameCanonicalString) {
    Graph triangle1 = createTriangleGraph();
    Graph triangle2 = createTriangleGraphDifferentOrder();

    std::string canon1 = triangle1.canonicalString_uncolored();
    std::string canon2 = triangle2.canonicalString_uncolored();

    // The canonical strings of isomorphic graphs must be equal.
    cout << "Canonical strings: " << canon1 << " " << canon2 << endl;
    EXPECT_EQ(canon1, canon2);
}

TEST(CanonicalStringUncoloredTest, NonIsomorphicGraphsHaveDifferentCanonicalString) {
    Graph triangle = createTriangleGraph();
    Graph path = createPathGraph();

    std::string canonTriangle = triangle.canonicalString_uncolored();
    std::string canonPath = path.canonicalString_uncolored();

    // Canonical strings of non-isomorphic graphs should differ.
    cout << "Canonical strings: " << canonTriangle << " " << canonPath << endl;
    EXPECT_NE(canonTriangle, canonPath);
}


// Helper to create a colored triangle graph, all nodes having the same color.
Graph createColoredTriangleGraph() {
    GraphTemplate gt(true);
    // Create three nodes with color 1.
    for (int i = 0; i < 3; ++i) {
        gt.addNode(Node(i));
    }
    // Add edges in one order.
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    gt.addEdge(0, 2);
    return Graph(gt);
}

// Helper to create a colored triangle graph with different edge order.
Graph createColoredTriangleGraphDifferentOrder() {
    GraphTemplate gt(true);
    for (int i = 0; i < 3; ++i) {
        gt.addNode(Node(i));
    }
    // Add the same edges but in a different order.
    gt.addEdge(1, 2);
    gt.addEdge(0, 2);
    gt.addEdge(0, 1);
    return Graph(gt);
}

// Helper to create a non-isomorphic colored graph.
// A colored path graph of 3 nodes, all nodes same color.
Graph createColoredPathGraph() {
    GraphTemplate gt(true);
    for (int i = 0; i < 3; ++i) {
        gt.addNode(Node(i));
    }
    // Create a path: 0-1, 1-2.
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    return Graph(gt);
}

TEST(CanonicalStringColoredTest, IsomorphicColoredGraphsHaveSameCanonicalString) {
    Graph triangle1 = createColoredTriangleGraph();
    Graph triangle2 = createColoredTriangleGraphDifferentOrder();

    std::string canon1 = triangle1.canonicalString_colored();
    std::string canon2 = triangle2.canonicalString_colored();

    // Isomorphic colored graphs must yield the same canonical string.
    cout << "Canonical strings: " << canon1 << " " << canon2 << endl;
    EXPECT_EQ(canon1, canon2);
}

TEST(CanonicalStringColoredTest, NonIsomorphicColoredGraphsHaveDifferentCanonicalString) {
    Graph triangle = createColoredTriangleGraph();
    Graph path = createColoredPathGraph();

    std::string canonTriangle = triangle.canonicalString_colored();
    std::string canonPath = path.canonicalString_colored();

    // Non-isomorphic graphs should yield different canonical strings.
    cout << "Canonical strings: " << canonTriangle << " " << canonPath << endl;
    EXPECT_NE(canonTriangle, canonPath);
}


// Helper to create a graph with a single colored node.
Graph createSingleColoredNodeGraph() {
    GraphTemplate gt(true);
    gt.addNode(Node(5));
    return Graph(gt);
}

// Helper to create an isomorphic colored graph with an extra isolated node.
Graph createColoredGraphWithIsolatedNode() {
    GraphTemplate gt(true);
    // Create 4 nodes with colors 1, 2, 3, 4.
    gt.addNode(Node(1));
    gt.addNode(Node(2));
    gt.addNode(Node(3));
    gt.addNode(Node(4));
    // Connect nodes 0-1 and 1-2
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    // Node 3 remains isolated.
    return Graph(gt);
}

Graph createColoredGraphWithDifferentEdgeOrder() {
    GraphTemplate gt(true);
    // Same colored nodes as in createColoredGraphWithIsolatedNode.
    gt.addNode(Node(1));
    gt.addNode(Node(2));
    gt.addNode(Node(3));
    gt.addNode(Node(4));
    // Adding the same edges but in reversed order.
    gt.addEdge(1, 2);
    gt.addEdge(0, 1);
    return Graph(gt);
}

// Helper to create a non-isomorphic graph that forces a different permutation, by different colors
Graph createDistinctColoredGraph() {
    GraphTemplate gt(true);
    // Create 3 nodes with distinct colors.
    gt.addNode(Node(10));
    gt.addNode(Node(20));
    gt.addNode(Node(30));
    // Connect in a triangle.
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    gt.addEdge(0, 2);
    return Graph(gt);
}

TEST(CanonicalStringColoredExtraTest, SingleNodeGraph) {
    Graph singleNode = createSingleColoredNodeGraph();
    // The canonical string should contain the node's color.
    std::string canon = singleNode.canonicalString_colored();
    EXPECT_NE(canon.find("5"), std::string::npos);
}

TEST(CanonicalStringColoredExtraTest, IsomorphicGraphsWithIsolatedNode) {
    Graph graph1 = createColoredGraphWithIsolatedNode();
    Graph graph2 = createColoredGraphWithDifferentEdgeOrder();
    // Even though the edge order is different, the graphs are isomorphic.
    std::string canon1 = graph1.canonicalString_colored();
    std::string canon2 = graph2.canonicalString_colored();
    EXPECT_EQ(canon1, canon2);
}

TEST(CanonicalStringColoredExtraTest, DistinctColoredGraphsDiffer) {
    Graph graph1 = createDistinctColoredGraph();
    // Create a second graph with the same structure but alter one node's color.
    GraphTemplate gt(true);
    gt.addNode(Node(10));
    gt.addNode(Node(20));
    // Change the third node's color
    gt.addNode(Node(99));
    gt.addEdge(0, 1);
    gt.addEdge(1, 2);
    gt.addEdge(0, 2);
    Graph graph2(gt);

    std::string canon1 = graph1.canonicalString_colored();
    std::string canon2 = graph2.canonicalString_colored();

    EXPECT_NE(canon1, canon2);
}

TEST(CanonicalStringColoredExtraTest, ExceptionForUncoloredGraph) {
    // Create a colored graph template but later treat it as uncolored.
    GraphTemplate gt(false);
    gt.addNode(Node());
    gt.addNode(Node());
    gt.addEdge(0, 1);
    Graph uncolored(gt);

    // Calling canonicalString_colored on an uncolored graph should throw.
    EXPECT_THROW({
        uncolored.canonicalString_colored();
    }, std::runtime_error);
}


//DEBUGGING
TEST(CanonicalStringColoredExtraTest, Debugging) {
    GraphTemplate Ht = GraphTemplate(true);
    Ht.addNode(Node(0));
    Ht.addNode(Node(0));
    Ht.addNode(Node(1));
    Ht.addNode(Node(0));

    Ht.addEdge(0, 1);
    Ht.addEdge(1, 3);
    Ht.addEdge(2, 3);

    Graph Hg = Graph(Ht);

    auto Hgcanon = Hg.canonicalString_colored();

    cout << Hgcanon << endl;

    GraphTemplate Gt = GraphTemplate(true);

    Gt.addNode(Node(0));
    Gt.addNode(Node(0));
    Gt.addNode(Node(1));
    Gt.addNode(Node(0));

    Gt.addEdge(0, 1);
    Gt.addEdge(1, 2);
    Gt.addEdge(2, 3);

    Graph Gg = Graph(Gt);

    auto Gtcanon = Gg.canonicalString_colored();

    cout << Gtcanon  << endl;

    ASSERT_NE(Hgcanon, Gtcanon);
}


