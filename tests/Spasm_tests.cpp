//
// Created by Christoph Greger on 01.03.25.
//


#include "gtest/gtest.h"
#include "Graph.h"
#include "Spasm_k_Matching.h"
#include "Injective_Hom_Count_By_Spasm.h"
#include "RandomGraphGenerator.h"

/**
TEST(SPASM_CLASS, test_generation_of_files) {
    for (int k = 1; k <= 7; k++) {
        std::string filename = "EineTestDatei_k_" + std::to_string(k) + ".txt";
        std::string smallerFilename = "EineTestDatei_k_" + std::to_string(k) + "_smaller.txt";
        createMultiSpasm_k_Matching(k, filename);
        convert_spasm_to_smaller(filename, smallerFilename);
    }
}

TEST(SPASM_CLASS, test_generation_of_files_for_k_8) {
    createMultiSpasm_k_Matching(8, "EineTestDatei_k_8.txt");
    convert_spasm_to_smaller("EineTestDatei_k_8.txt", "EineTestDatei_k_8_smaller.txt");
}
*/


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


TEST(Spasm_make_smaller, test_total) {
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
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_3_smaller.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << match3.calculateNumberofInjectiveHomomorphismsTo(G) << std::endl;
    ASSERT_EQ(match3.calculateNumberofInjectiveHomomorphismsTo(G), injective_count_by_spasm_smaller("EineTestDatei_k_3_smaller.txt", G));

}

TEST(Spasm_make_smaller, test_total_k_2) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    Graph match3 = Graph(false);

    for (int i = 0; i < 4; i++) {
        match3.addNode(Node());
    }

    match3.addEdge(0 , 1);
    match3.addEdge(2 , 3);
    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_2_smaller.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << match3.calculateNumberofInjectiveHomomorphismsTo(G) << std::endl;
    ASSERT_EQ(match3.calculateNumberofInjectiveHomomorphismsTo(G), injective_count_by_spasm_smaller("EineTestDatei_k_2_smaller.txt", G));

}


TEST(Spasm_make_smaller, test_total_k_4) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    Graph match3 = Graph(false);

    for (int i = 0; i < 8; i++) {
        match3.addNode(Node());
    }

    match3.addEdge(0 , 1);
    match3.addEdge(2 , 3);
    match3.addEdge(4 , 5);
    match3.addEdge(6 , 7);
    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_4_smaller.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << match3.calculateNumberofInjectiveHomomorphismsTo(G) << std::endl;
    ASSERT_EQ(match3.calculateNumberofInjectiveHomomorphismsTo(G), injective_count_by_spasm_smaller("EineTestDatei_k_4_smaller.txt", G));

}

TEST(Spasm_make_smalle, test_non_small_k_2) {
    RandomGraphGenerator generator = RandomGraphGenerator(14, 20);
    Graph G = generator.generateRandomConnectedGraph();

    Graph match3 = Graph(false);

    for (int i = 0; i < 4; i++) {
        match3.addNode(Node());
    }

    match3.addEdge(0 , 1);
    match3.addEdge(2 , 3);
    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm("EineTestDatei_k_2.txt", G) << std::endl;
    std::cout << "Injective Homs by brute force: " << match3.calculateNumberofInjectiveHomomorphismsTo(G) << std::endl;
    ASSERT_EQ(match3.calculateNumberofInjectiveHomomorphismsTo(G), injective_count_by_spasm("EineTestDatei_k_2.txt", G));

}


TEST(Spasm_make_smaller, test_total_k_7_speedtest) {
    RandomGraphGenerator generator = RandomGraphGenerator(30, 70);
    Graph G = generator.generateRandomConnectedGraph();

    //Wir wollen jetzt die injectiven Homs von
    std::cout << "Injective Homs: " << injective_count_by_spasm_smaller("EineTestDatei_k_8_smaller.txt", G) << std::endl;
}