//
// Created by falco on 04.03.25.
//

#include <gtest/gtest.h>
#include "utilities.h"

#include "Graph.h"
#include "CalcHoms.h"

//S is a triangle with two equal color (0,1)
//H has two nodes with color (0,1) and one edge
//we map the nodes of H to different nodes in S
//the result should be the same in both tests
TEST(CalcHomsTest, calcNumHomsCFI1) {
    //S is a triangle
    Graph S = Graph();
    S.addNode(Node(0));//node has id: 0
    S.addNode(Node(1));
    S.addNode(Node(1));
    S.addEdge(0,1);//edge between node with id 0 and 1
    S.addEdge(0,2);
    S.addEdge(1,2);

    Graph H = Graph();
    H.addNode(Node(0));
    H.addNode(Node(1));
    H.addEdge(0,1);

    int* mapping = new int[2];
    mapping[0] = 0;
    mapping[1] = 1;

    const int dim = CalcHoms::calcNumHomsCFI(H,S,mapping);

    EXPECT_EQ(dim, 1);

    int* mapping2 = new int[2];
    mapping2[0] = 0;
    mapping2[1] = 2;

    const int dim2 = CalcHoms::calcNumHomsCFI(H,S,mapping2);

    EXPECT_EQ(dim2, 1);
}