//
// Created by Christoph Greger on 25.12.24.
//

#include <gtest/gtest.h>
#include <sstream>

#include "CFIGraph.h"
#include "Graph.h"
#include "Node.h"
#include "RandomGraphGenerator.h"

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

    cout << "Next test" << endl;

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

TEST(GraphTest, calculateNumberofInjectiveHomomorphismsTo) {
    Graph G(true);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    Node cnode4 = Node(4);
    G.addNode(cnode1);
    G.addNode(cnode2);
    G.addNode(cnode3);
    G.addNode(cnode4);
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 3);
    G.addEdge(3, 0);

    ASSERT_EQ(G.calculateNumberofInjectiveHomomorphismsTo(G), 1);

    G.colored = false;

    ASSERT_EQ(G.calculateNumberofInjectiveHomomorphismsTo(G), 8);

}

TEST(GraphTest, calculateNodeIndex) {
    Graph G(false);
    Node cnode1 = Node(1);
    Node cnode2 = Node(2);
    Node cnode3 = Node(3);
    Node cnode4 = Node(4);
    G.addNode(cnode1);
    G.addNode(cnode2);
    G.addNode(cnode3);
    G.addNode(cnode4);
    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 3);
    G.addEdge(3, 0);

    int* nodeIndex = G.calculateNodeIndex();


    //print the edges
    cout << "Edges: \n";
    for (int i = 0; i < G.edges.size(); i++) {
        cout << G.edgeArray[i].first << " -> " << G.edgeArray[i].second << "\n";
    }

    cout << "NodeIndex: ";
    for (int i = 0; i < 4; i++) {
        cout << nodeIndex[i] << " "; //Shold be: 0 1 2 4
    }


    int shouldbe[] = {0, 1, 2, 4};
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(nodeIndex[i], shouldbe[i]);
    }
}

TEST(GraphTest, calculateNodeIndexAutomatic) {
    RandomGraphGenerator generator = RandomGraphGenerator(100, 300, false);
    Graph G = generator.generateRandomConnectedGraph();
    int* nodeIndex = G.calculateNodeIndex();
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
        Graph pattern = Graph(true);
        for (int i = 0; i < 4; i++) {
            Node node = Node(i);
            pattern.addNode(node);
        }
        pattern.addEdge(0, 1);
        pattern.addEdge(0, 3);
        pattern.addEdge(1, 2);

        CFIGraph CFI = CFIGraph(pattern);
        Graph input = CFI.toGraph();

        ASSERT_EQ(pattern.calculateNumberofHomomorphismsTo(input), 1);
    }
}


TEST(GraphTest, coloredHoms2) {
    for (int times = 0; times < 1000; times++) {
        Graph pattern = Graph(true);
        for (int i = 0; i < 4; i++) {
            Node node = Node(i);
            pattern.addNode(node);
        }
        pattern.addEdge(0, 3);
        pattern.addEdge(1, 3);
        pattern.addEdge(2, 3);

        Graph input = Graph(true);
        for (int i = 0; i < 4; i++) {
            int color = 0;
            switch (i) {
                case 0:
                    color = 0;
                    break;
                case 1:
                    color = 1;
                    break;
                case 2:
                    color = 2;
                    break;
                default:
                    color = 3;
                    break;
            }
            Node node = Node(color);
            input.addNode(node);
        }
        input.addEdge(0, 3);
        input.addEdge(0, 6);
        input.addEdge(1, 3);
        input.addEdge(1, 4);
        input.addEdge(2, 3);
        input.addEdge(2, 5);

        ASSERT_EQ(pattern.calculateNumberofHomomorphismsTo(input), 1);
    }
}

//WORKS!
TEST(GRAPH_Test, Neighbors) {
    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    auto neighbors = S.neighbors();

    for (int i = 0; i < 4; i++) {
        cout << "Node: " << i << endl;
        for (int neighbor : neighbors[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

TEST(GRAPH_Test, Neighbors2) {
    Graph H = Graph(true);
    for (int i = 0; i < 4; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(0));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(1 , 3);
    H.addEdge(2 , 3);

    H.addEdge(4,1);

    auto degree = H.degree(); //2 4 3 2 1
    auto expected = vector<int>{2, 4, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        cout << expected[i] << " " << degree[i] << endl;
        ASSERT_EQ(degree[i] == expected[i], true);
    }
}


//WORKS
TEST(GRAPH_Test, Degree) {
    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    auto degree = S.degree();

    for (int i = 0; i < 4; i++) {
        cout << "Node: " << i << " Degree: " << degree[i] << endl;
    }
}

TEST(GRAPH_Test, ShrinkGraph) {
    //See the CFI graph from the paper

    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    Graph H = Graph(true);
    for (int i = 0; i < 4; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(0));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(1 , 3);
    H.addEdge(2 , 3);

    H.addEdge(4 , 1);

    auto shrinked = H.shrinkGraph(S);
    ASSERT_EQ(shrinked.first, true);
    shrinked.second.printGraph(true);
    ASSERT_EQ(shrinked.second.calculateNumberofHomomorphismsTo(S), 1);
    S.printGraph(true);
}


TEST(Graph_Explanation, test) {
    RandomGraphGenerator generator = RandomGraphGenerator(10, 20, false);
    Graph G = generator.generateRandomConnectedGraph();

    int * nodeIndex = G.calculateNodeIndex();
    for (int i = 0; i < G.edges.size(); i++) {
        cout << G.edgeArray[i].first << " -> " << G.edgeArray[i].second << endl;
    }



    cout << "NodeIndex: ";
    for (int i = 0; i < G.numVertices; i++) {
        cout << nodeIndex[i] << " ";
    }
    cout << endl;
}


//WORKS!
TEST(GRAPH_Shrink, shrink2) {
    Graph S = Graph(true);
    for (int i = 0; i < 5; i++) {
        S.addNode(Node(i));
    }
    S.addEdge(0,1);
    S.addEdge(0,2);
    S.addEdge(0,3);
    S.addEdge(0,4);
    S.addEdge(1,2);
    S.addEdge(1,3);

    Graph H = Graph(true);
    H.addNode(Node(2));
    H.addNode(Node(0));
    H.addNode(Node(2));
    H.addNode(Node(4));

    H.addEdge(1,0);
    H.addEdge(1,2);
    H.addEdge(1,3);


    auto shrunken = H.shrinkGraph(S);
    ASSERT_EQ(shrunken.first, true);
    cout << "Shrunken Graph: " << endl;
    shrunken.second.printGraph(true);
    cout << "Shrunken with: " << endl;
    S.printGraph(true);
}

TEST(GraphConnectedComponents, SingleComponent) {
    // Create a connected graph with 4 nodes and edges connecting all nodes.
    Graph graph(false);
    for (int i = 0; i < 4; i++) {
        graph.addNode(Node());
    }
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);

    auto components = graph.connectedComponents();
    // Expect exactly one connected component.
    ASSERT_EQ(components.size(), 1);
    // The component should have the same number of nodes.
    ASSERT_EQ(components[0].numVertices, 4);
}

TEST(GraphConnectedComponents, TwoComponents) {
    // Create a graph with 6 nodes and two separate components:
    // Component 1: Nodes 0,1,2. Component 2: Nodes 3,4,5.
    Graph graph(false);
    for (int i = 0; i < 6; i++) {
        graph.addNode(Node());
    }
    // Component 1
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    // Component 2
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);

    auto components = graph.connectedComponents();
    ASSERT_EQ(components.size(), 2);

    // Check sizes of both components.
    // As ordering is not guaranteed, count by numVertices.
    int comp1Size = 0, comp2Size = 0;
    for (const auto &comp : components) {
        if (comp.numVertices == 3) {
            if (comp1Size == 0) comp1Size = comp.numVertices;
            else comp2Size = comp.numVertices;
        }
    }
    ASSERT_EQ(comp1Size, 3);
    ASSERT_EQ(comp2Size, 3);
}

TEST(GraphConnectedComponents, IsolatedVertices) {
    // Create a graph with 5 nodes and no edges.
    Graph graph(false);
    for (int i = 0; i < 5; i++) {
        graph.addNode(Node());
    }

    auto components = graph.connectedComponents();
    // Each vertex is a component.
    ASSERT_EQ(components.size(), 5);
    for (const auto &comp : components) {
        ASSERT_EQ(comp.numVertices, 1);
    }
}




