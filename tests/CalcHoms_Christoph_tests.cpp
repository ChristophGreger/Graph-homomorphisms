//
// Created by Christoph Greger on 04.03.25.
//


#include "gtest/gtest.h"
#include "Graph.h"
#include "CalcHoms.h"
#include "CFIGraph.h"
#include "RandomGraphGenerator.h"


TEST(CalcHoms_Christoph, calcNumHomsCFI_uncolored) {
    RandomGraphGenerator randomH = RandomGraphGenerator(5, 8);
    RandomGraphGenerator randomS = RandomGraphGenerator(5, 8, true);

    Graph H = randomH.generateRandomConnectedGraph();
    Graph S = randomS.generateRandomConnectedGraph();

    CFIGraph cfiS = CFIGraph(S);
    Graph cfiGraph = cfiS.toGraph();

    long long numHomsBruteForce = H.calculateNumberofHomomorphismsTo(cfiGraph);

    long long numHoms = CalcHoms::calcNumHomsCFI_uncolored(H, S);

    ASSERT_EQ(numHoms, numHomsBruteForce);
}


TEST(CalcHoms_Christoph, calcNumInjectiveHomsCFI) {

    Graph K_2_mal_3 = Graph(false);

    for (int i = 0; i < 6; i++) {
        K_2_mal_3.addNode(Node());
    }

    K_2_mal_3.addEdge(0, 1);
    K_2_mal_3.addEdge(2, 3);
    K_2_mal_3.addEdge(4, 5);

    RandomGraphGenerator randomS = RandomGraphGenerator(5, 8, true);

    Graph S = randomS.generateRandomConnectedGraph();

    CFIGraph cfiS = CFIGraph(S);
    Graph cfiGraph = cfiS.toGraph();

    long long numHomsBruteForce = K_2_mal_3.calculateNumberofInjectiveHomomorphismsTo(cfiGraph);

    long long numHoms = CalcHoms::calcNumInjectiveHomsCFI("EineTestDatei_k_3_smaller.txt", S);

    ASSERT_EQ(numHoms, numHomsBruteForce);
}