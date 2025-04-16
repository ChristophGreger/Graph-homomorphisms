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
