//
// Created by Christoph Greger on 01.03.25.
//


#include "gtest/gtest.h"
#include "Graph.h"
#include "Spasm_k_Matching.h"
#include "Injective_Hom_Count_By_Spasm.h"
#include "RandomGraphGenerator.h"

/**
TEST(SPASM_CLASS, test) {
    createMultiSpasm_k_Matching(7, "EineTestDatei_k_7.txt");
}

TEST(SPASM_CLASS, test2) {
    createMultiSpasm_k_Matching(8, "EineTestDatei_k_8.txt");
}
*/

TEST(SPASM_CLASS, test3) {
    createMultiSpasm_k_Matching(5, "EineTestDatei_k_5.txt");
}

TEST(SPASM_CLASS, test4) {
    createMultiSpasm_k_Matching(3, "EineTestDatei_k_3.txt");
}


TEST(SPASM_CLASS, test5) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    Graph match3 = Graph(false);

    for (int i = 0; i < 6; i++) {
        match3.addNode(Node());
    }

    match3.addEdge(0 , 1);
    match3.addEdge(2 , 3);
    match3.addEdge(4 , 5);
    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm("EineTestDatei_k_3.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << match3.calculateNumberofInjectiveHomomorphismsTo(G) << std::endl;
    ASSERT_EQ(match3.calculateNumberofInjectiveHomomorphismsTo(G), injective_count_by_spasm("EineTestDatei_k_3.txt", G));
}