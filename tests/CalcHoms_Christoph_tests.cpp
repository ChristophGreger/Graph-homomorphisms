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

    long long numHomsBruteForce = CalcHoms::calcNumHoms(H,cfiGraph);

    cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

    int256_t numHoms = CalcHoms::calcNumHomsCFI_uncolored(H, S);

    cout << "numHoms: " << numHoms << endl;

    ASSERT_EQ(numHoms, numHomsBruteForce);
}


TEST(CalcHoms_Christoph, calcNumInjHomsCFI) {

    GraphTemplate K2_mal_3T = GraphTemplate(false);

    for (int i = 0; i < 6; i++) {
        K2_mal_3T.addNode(Node());
    }

    K2_mal_3T.addEdge(0, 1);
    K2_mal_3T.addEdge(2, 3);
    K2_mal_3T.addEdge(4, 5);

    Graph K2_mal_3(K2_mal_3T);

    RandomGraphGenerator randomS = RandomGraphGenerator(5, 6, true);

    for (int i = 0; i < 20; i++) {
        Graph S = randomS.generateRandomConnectedGraph();

        CFIGraph cfiS = CFIGraph(S);
        Graph cfiGraph = cfiS.toGraph();

        long long numHomsBruteForce = CalcHoms::calcNumInjHoms(K2_mal_3, cfiGraph);

        cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

        int256_t numHoms = CalcHoms::calcNumInjHoms("k_3.txt", S, true);

        cout << "numHoms: " << numHoms << endl;

        ASSERT_EQ(numHoms, numHomsBruteForce);
    }
}

TEST(CalcHoms_Christoph, calcNumInjHomsCFI_inverted) {
    GraphTemplate K2_mal_3T = GraphTemplate(false);

    for (int i = 0; i < 6; i++) {
        K2_mal_3T.addNode(Node());
    }

    K2_mal_3T.addEdge(0, 1);
    K2_mal_3T.addEdge(2, 3);
    K2_mal_3T.addEdge(4, 5);

    Graph K2_mal_3(K2_mal_3T);


    RandomGraphGenerator randomS = RandomGraphGenerator(6, 7, true);

    for (int i = 0; i < 3; i++) {
        Graph S = randomS.generateRandomConnectedGraph();

        auto cfiS = CFIGraph(S, true);
        Graph cfiGraph = cfiS.toGraph();


        long long numHomsBruteForce = CalcHoms::calcNumInjHoms(K2_mal_3, cfiGraph);

        cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

        int256_t numHoms = CalcHoms::calcNumInjHoms("k_3.txt", S, true, true);

        cout << "numHoms: " << numHoms << endl;

        ASSERT_EQ(numHoms, numHomsBruteForce);

    }
}

TEST(CalcHoms_Christoph, calcNumInjHomsCFI_speed) {


    RandomGraphGenerator randomS = RandomGraphGenerator(30, 40, false);

    for (int i = 0; i < 1; i++) {
        Graph S = randomS.generateRandomConnectedGraph();

        int256_t numHoms = CalcHoms::calcNumInjHoms("k_8.txt", S, true, true);

        cout << "numHoms: " << numHoms << endl;

        int256_t numHoms_not_inverted = CalcHoms::calcNumInjHoms("k_8.txt", S, true, false);

        ASSERT_EQ(numHoms_not_inverted, numHoms);

    }
}

TEST(CalcHoms_Christoph, Clique_testing3) {
    int n = 6;
    RandomGraphGenerator randomS = RandomGraphGenerator(n, n+2);
    auto S = randomS.generateRandomConnectedGraph();
    std::string filename = "k_" + std::to_string(n) + ".txt";
    auto inverted = CalcHoms::calcNumInjHoms(filename, S, true, true);
    auto normal = CalcHoms::calcNumInjHoms(filename, S, true, false);
    cout << "k = " << n <<" Inverted: " << inverted << " Normal: " << normal << endl;
    ASSERT_EQ(inverted, normal);
}

TEST(CalcHoms_Christoph, Thesis) {
    RandomGraphGenerator clique4gen = RandomGraphGenerator(4, 6);
    Graph clique4 = clique4gen.generateRandomConnectedGraph();
    auto normal = CalcHoms::calcNumInjHoms("k_6.txt", clique4, true, false);
    auto inverted = CalcHoms::calcNumInjHoms("k_6.txt", clique4, true, true);
    cout << "Normal: " << normal << " Inverted: " << inverted << endl;
}