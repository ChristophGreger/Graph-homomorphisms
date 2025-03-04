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

    //GANZ WICHTIG!!!!!!
    S.calculateAdjMatrix();

    long long numHomsBruteForce = H.calculateNumberofHomomorphismsTo(cfiGraph);

    cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

    long long numHoms = CalcHoms::calcNumHomsCFI_uncolored(H, S);

    cout << "numHoms: " << numHoms << endl;

    ASSERT_EQ(numHoms, numHomsBruteForce);
}


TEST(CalcHoms_Christoph, calcNumInjectiveHomsCFI) {

    Graph K_2_mal_6 = Graph(false);

    for (int i = 0; i < 12; i++) {
        K_2_mal_6.addNode(Node());
    }

    K_2_mal_6.addEdge(0, 1);
    K_2_mal_6.addEdge(2, 3);
    K_2_mal_6.addEdge(4, 5);
    K_2_mal_6.addEdge(6, 7);
    K_2_mal_6.addEdge(8, 9);
    K_2_mal_6.addEdge(10, 11);


    RandomGraphGenerator randomS = RandomGraphGenerator(30, 40, true);

    Graph S = randomS.generateRandomConnectedGraph();

    CFIGraph cfiS = CFIGraph(S);
    Graph cfiGraph = cfiS.toGraph();

    //long long numHomsBruteForce = K_2_mal_6.calculateNumberofInjectiveHomomorphismsTo(cfiGraph);

    //long long numHomsBruteForce = 0;

    //cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

    long long numHoms = CalcHoms::calcNumInjectiveHomsCFI("EineTestDatei_k_6_smaller.txt", S);

    cout << "numHoms: " << numHoms << endl;

    //ASSERT_EQ(numHoms, numHomsBruteForce);
}