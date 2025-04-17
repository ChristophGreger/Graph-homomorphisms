//
// Created by Christoph Greger on 16.04.25.
//

#include <gtest/gtest.h>
#include "Geng.h"

TEST(Geng, n20_1_10) {
    // Teste die Generierung von Graphen mit 20 Knoten und 1 bis 10 Kanten
    std::vector<Graph> graphs = Geng::generateGraphs(20, 1, 10);
    EXPECT_EQ(graphs.size(), 6876);
}

TEST(Geng, n20_1_10_2) {
    // Teste die Generierung von Graphen mit 20 Knoten und 1 bis 10 Kanten
    std::vector<Graph> graphs = Geng::generateGraphs(20, 1, 10);
    std::vector<Graph> graph2 = std::vector<Graph>();
    cout << "Graph size: " << graphs.size() << endl;
    int i = 0;
    for (auto g : graphs) {
        graph2.push_back(g.withOutIsolatedNodes());
        cout << i++ << endl;
    }
}

TEST(Geng, n20_1_10_3) {
    // Teste die Generierung von Graphen mit 20 Knoten und 1 bis 10 Kanten
    std::vector<Graph> graphs = Geng::generateGraphs_without_isolated(20, 1, 10);
}

TEST(Geng, n22_1_11_3) {
    // Teste die Generierung von Graphen mit 20 Knoten und 1 bis 10 Kanten
    std::vector<Graph> graphs = Geng::generateGraphs_without_isolated(22, 1, 11);
}

TEST(Geng, n24_1_12_3) {
    // Teste die Generierung von Graphen mit 20 Knoten und 1 bis 10 Kanten
    std::vector<Graph> graphs = Geng::generateGraphs_without_isolated(24, 1, 12);
}

TEST(Geng, combinationhomcounts_18_1_9) {
    Geng::write_Components_Combinations_to_File("Combination_hom_counts_18_1_9.txt", 18, 1, 9);
}

TEST(Geng, combinationhomcounts_20_1_10) {
    Geng::write_Components_Combinations_to_File("Combination_hom_counts_20_1_10.txt", 20, 1, 10);
}

TEST(Geng, storeMatrix) {
    Geng::storeMatrix("Matrix_20_1_10.txt", "Combination_hom_counts_20_1_10.txt", 20, 1, 10);
}


