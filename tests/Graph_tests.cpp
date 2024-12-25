//
// Created by Christoph Greger on 25.12.24.
//

#include <gtest/gtest.h>
#include <sstream>
#include "Graph.h"

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

    std::string expectedOutput = "Graph with 3 vertices and 2 edges\n0 -> 1\n1 -> 2\n";
    EXPECT_EQ(output.str(), expectedOutput);
}
