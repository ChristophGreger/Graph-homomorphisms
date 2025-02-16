//
// Created by Christoph Greger on 26.12.24.
//

#include <gtest/gtest.h>
#include "CFIGraph.h"
#include "CFINode.h"
#include "RandomGraphGenerator.h"
#include "utilities.h"
#include "BruteForceAlgo.h"

TEST(CFIGraphTest, ByHomomorphisms3) {
    int vertices = 20;
    int edges = 40;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    unsigned long time = clock();
    long long numHoms = calcHoms(CFI, G);
    ASSERT_EQ(numHoms, intPow(2, (edges - vertices + 1)));
    cout << intPow(2, (edges - vertices + 1)) << endl;
    cout << "Time in ms: " << (clock() - time)/1000 << endl;
    cout << "Calculated successfully" << endl;
}

TEST(CFIGraphTest, ByHomomorphisms) {
    for (int times = 0; times < 10; times++) {
        for (int vertices = 2; vertices < 9; vertices++) {
            for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
                for (int i = 0; i < 3; i++) {
                    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
                    CFIGraph CFI = CFIGraph(G);
                    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
                    long long homs = calcHoms(CFI, G);
                    long long expected = intPow(2, (edges - vertices + 1));
                    if (homs != expected) {
                        G.printGraph(true);
                    }
                    ASSERT_EQ(homs, expected);
                    cout << "Calculated successfully" << endl;
                }
            }
        }
    }
}

TEST(CFIGraphTest, ByHomomorphisms2) {
    int vertices = 10;
    int edges = 30;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    ASSERT_EQ(calcHoms(CFI, G), intPow(2, (edges - vertices + 1)));
    cout << "Calculated successfully" << endl;
}



