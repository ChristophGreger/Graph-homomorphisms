//
// Created by falco on 04.03.25.
//

#include <gtest/gtest.h>
#include "utilities.h"

#include "Graph.h"
#include "CalcHoms.h"
#include "RandomGraphGenerator.h"
#include "CFIGraph.h"
#include "GraphTemplate.h"

//Check one specific graph
//S is a triangle with two equal color (0,1)
//H has two nodes with color (0,1) and one edge
//we map the nodes of H to different nodes in S
//the result should be the same in both tests
TEST(CalcHomsTest, calcNumHomsCFI1) {
    //S is a triangle
    GraphTemplate STemplate = GraphTemplate();
    STemplate.addNode(Node(0));//node has id: 0
    STemplate.addNode(Node(1));
    STemplate.addNode(Node(1));
    STemplate.addEdge(0,1);//edge between node with id 0 and 1
    STemplate.addEdge(0,2);
    STemplate.addEdge(1,2);

    Graph S(STemplate);

    GraphTemplate HTemplate = GraphTemplate();
    HTemplate.addNode(Node(0));
    HTemplate.addNode(Node(1));
    HTemplate.addEdge(0,1);

    Graph H(HTemplate);

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

//Check Graphs up to 10 vertices
//S is mapped into the CFI of S
//This testcase does not cover duplicate colors in S
TEST(CalcHomsTest, calcNumHomsCFI2) {
    for (int vertices = 2; vertices < 10; vertices++) {
        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();
            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }
            int dim = CalcHoms::calcNumHomsCFI(S,S,mapping);
            int expected = (edges - vertices + 1);
            ASSERT_EQ(dim, expected);
        }
    }
}

//Check Graphs up to 10 vertices
//A Graph with 4 nodes and 4 edges is mapped into the CFI of a random Graph S
//This testcase does not cover duplicate colors in S
TEST(CalcHomsTest, calcNumHomsInvCFI) {
    int count = 0;
    for (int vertices = 2; vertices < 10; vertices++) {
        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {

            cout << "check Graph combination:" << count << endl;
            count++;

            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();

            RandomGraphGenerator randomGraphGenerator2 = RandomGraphGenerator(4, 4, true, true);
            Graph H = randomGraphGenerator2.generateRandomConnectedGraph();

            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }

            int dim = -1;
            try {
                dim = CalcHoms::calcNumHomsCFI(H,S,mapping, true, S.edgeArray[0]);
            } catch (const std::exception& e) {
                std::cerr << "Caught exception: " << e.what() << std::endl;
            }

            if (dim != -1) {
                cout << "dim:" << dim << endl;
                cout << "S:" << endl;
                S.printGraph();
                cout << "H:" << endl;
                H.printGraph();
            }
            long long result = 0;
            if (dim != -1) {
                result = intPow(2,dim);
            }

            CFIGraph CFI = CFIGraph(S, true);
            Graph G = CFI.toGraph();
            long long expected = CalcHoms::calcNumHoms(H,G);

            ASSERT_EQ(result, expected);

        }
    }
}


TEST(CalcHomsTest, calcNumHomsInvCFI2) {
    RandomGraphGenerator Sgen = RandomGraphGenerator(10, 15, true, true);
    for (int i = 0; i < 100; i++) {
        auto S = Sgen.generateRandomConnectedGraph();
        int* mapping = new int[S.numVertices];
        for (int j = 0; j < S.numVertices; j++) {
            mapping[j] = j;
        }
        ASSERT_EQ(-1, CalcHoms::calcNumHomsCFI(S, S, mapping, true, S.edgeArray[0]));
        delete [] mapping;
    }
}