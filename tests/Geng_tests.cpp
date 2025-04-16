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
