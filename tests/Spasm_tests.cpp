//
// Created by Christoph Greger on 01.03.25.
//


#include "CalcHoms.h"
#include "gtest/gtest.h"
#include "Graph.h"
#include "Spasm_k_Matching.h"
#include "Injective_Hom_Count_By_Spasm.h"
#include "RandomGraphGenerator.h"
#include "Spasm.h"


TEST(SPASM_CLASS, test5) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    GraphTemplate match3t = GraphTemplate(false);

    for (int i = 0; i < 6; i++) {
        match3t.addNode(Node());
    }

    match3t.addEdge(0 , 1);
    match3t.addEdge(2 , 3);
    match3t.addEdge(4 , 5);
    Graph match3(match3t);

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm("EineTestDatei_k_3.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << CalcHoms::calcNumInjHoms(match3,G) << std::endl;
    ASSERT_EQ(CalcHoms::calcNumInjHoms(match3,G), injective_count_by_spasm("EineTestDatei_k_3.txt", G));
}


TEST(Spasm_make_smaller, test_total) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    GraphTemplate match3t = GraphTemplate(false);

    for (int i = 0; i < 6; i++) {
        match3t.addNode(Node());
    }

    match3t.addEdge(0 , 1);
    match3t.addEdge(2 , 3);
    match3t.addEdge(4 , 5);
    Graph match3(match3t);

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_3_smaller.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << CalcHoms::calcNumInjHoms(match3,G) << std::endl;
    ASSERT_EQ(CalcHoms::calcNumInjHoms(match3,G), injective_count_by_spasm_smaller("EineTestDatei_k_3_smaller.txt", G));

}

TEST(Spasm_make_smaller, test_total_k_2) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    GraphTemplate match3t = GraphTemplate(false);

    for (int i = 0; i < 4; i++) {
        match3t.addNode(Node());
    }

    match3t.addEdge(0 , 1);
    match3t.addEdge(2 , 3);
    Graph match3(match3t);

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_2_smaller.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << CalcHoms::calcNumInjHoms(match3,G) << std::endl;
    ASSERT_EQ(CalcHoms::calcNumInjHoms(match3,G), injective_count_by_spasm_smaller("EineTestDatei_k_2_smaller.txt", G));

}


TEST(Spasm_make_smaller, test_total_k_4) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    GraphTemplate match3t = GraphTemplate(false);

    for (int i = 0; i < 8; i++) {
        match3t.addNode(Node());
    }

    match3t.addEdge(0 , 1);
    match3t.addEdge(2 , 3);
    match3t.addEdge(4 , 5);
    match3t.addEdge(6 , 7);
    Graph match3(match3t);

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_4_smaller.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << CalcHoms::calcNumInjHoms(match3,G) << std::endl;
    ASSERT_EQ(CalcHoms::calcNumInjHoms(match3,G), injective_count_by_spasm_smaller("EineTestDatei_k_4_smaller.txt", G));

}

TEST(Spasm_make_smalle, test_non_small_k_2) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    GraphTemplate match3t = GraphTemplate(false);

    for (int i = 0; i < 4; i++) {
        match3t.addNode(Node());
    }

    match3t.addEdge(0 , 1);
    match3t.addEdge(2 , 3);
    Graph match3(match3t);

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm("EineTestDatei_k_2.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << CalcHoms::calcNumInjHoms(match3,G) << std::endl;
    ASSERT_EQ(CalcHoms::calcNumInjHoms(match3,G), injective_count_by_spasm("EineTestDatei_k_2.txt", G));

}


TEST(Spasm_make_smaller, test_total_k_7_speedtest) {
    RandomGraphGenerator generator = RandomGraphGenerator(30, 70);
    Graph G = generator.generateRandomConnectedGraph();

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_8_smaller.txt", G) << std::endl;
}


TEST(Spasm, generate_spasm_files) {
    for (int k = 1; k <= 7; ++k) {
        std::string filename = "k_" + std::to_string(k) + ".txt";
        Spasm::create_and_store_Spasm_k_Matching(filename, k);
        cout << "Finished k = " << k << endl;
    }
}

