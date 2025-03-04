//
// Created by Christoph Greger on 27.12.24.
//

#include <gtest/gtest.h>
#include "RandomGraphGenerator.h"

TEST(RandomGraphGeneratorTest, Constructor) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(10, 20, true, true, 3);
    EXPECT_EQ(randomGraphGenerator.vertices, 10);
    EXPECT_EQ(randomGraphGenerator.edges, 20);
    EXPECT_EQ(randomGraphGenerator.colored, true);
    EXPECT_EQ(randomGraphGenerator.surjectivecoloring, true);
    EXPECT_EQ(randomGraphGenerator.colors, 3);
}

//This Test takes a long time to run, start only if you have time
TEST(RandomGraphGeneratorTest, GenerateRandomConnectedGraph_colored_surjective) {
    for (int vertices = 2; vertices < 10; vertices++) {
        for (int edges = vertices - 1; edges < vertices * (vertices - 1) / 2; edges++) {
            for (int i = 0; i < 3; i++) {
                RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                Graph G = randomGraphGenerator.generateRandomConnectedGraph();
                EXPECT_EQ(G.numVertices, vertices);
                EXPECT_EQ(G.edges.size(), edges);
                EXPECT_TRUE(G.isConnected());
            }
        }

    }
}