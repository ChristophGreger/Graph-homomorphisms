//
// Created by Christoph Greger on 01.03.25.
//


#include "CalcHoms.h"
#include "gtest/gtest.h"
#include "Graph.h"
#include "RandomGraphGenerator.h"
#include "Spasm.h"


/**
TEST(Spasm, generate_spasm_files) {
    for (int k = 1; k <= 7; ++k) {
        std::string filename = "k_" + std::to_string(k) + ".txt";
        Spasm::create_and_store_Spasm_k_Matching(filename, k);
        cout << "Finished k = " << k << endl;
    }
}

TEST(Spasm, generate_File_8) {
    Spasm::create_and_store_Spasm_k_Matching("k_8.txt", 8);
}

TEST(Spasm, generate_File_6) {
    Spasm::create_and_store_Spasm_k_Matching("k_3_neu.txt", 3);
}
*/



TEST(Spasm, generateSquare) {
    GraphTemplate squaret = GraphTemplate(false);
    for (int i = 0; i < 4; i++) {
        squaret.addNode(Node());
    }
    squaret.addEdge(0, 1);
    squaret.addEdge(1, 2);
    squaret.addEdge(2, 3);
    squaret.addEdge(3, 0);
    Graph square(squaret);
    Spasm::create_and_store_Spasm("square.txt", square);
    RandomGraphGenerator randomS = RandomGraphGenerator(30, 100, false);
    Graph G = randomS.generateRandomConnectedGraph();
    ASSERT_EQ(CalcHoms::calcNumInjHoms(square, G), CalcHoms::calcNumInjHoms("square.txt", G));
}

TEST(Spasm, kmatching) {
    Spasm::create_and_store_Spasm_k_Matching("k_3_test.txt", 3);
    GraphTemplate k_3 = GraphTemplate(false);
    for (int i = 0; i < 6; i++) {
        k_3.addNode(Node());
    }
    k_3.addEdge(0, 1);
    k_3.addEdge(2, 3);
    k_3.addEdge(4, 5);

    Graph k_3g = Graph(k_3);
    RandomGraphGenerator randomS = RandomGraphGenerator(30, 100, false);
    Graph G = randomS.generateRandomConnectedGraph();
    ASSERT_EQ(CalcHoms::calcNumInjHoms(k_3g, G), CalcHoms::calcNumInjHoms("k_3_test.txt", G));
}