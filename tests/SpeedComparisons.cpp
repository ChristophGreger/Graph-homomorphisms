//
// Created by Christoph Greger on 28.12.24.
//

#include <gtest/gtest.h>
#include "RandomGraphGenerator.h"
#include "InjectiveHomomorphismRange.h"
#include "Graph.h"
#include "CFIGraph.h"


//Edited for faster stuff
TEST(SpeedTest, Nodes256) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(40, 100, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    Graph square = Graph();
    for (int i = 0; i < 4; i++) {
        square.addNode(Node());
    }
    square.addEdge(0, 1);
    square.addEdge(1, 2);
    square.addEdge(2, 3);
    square.addEdge(3, 0);

    cout << "Graphs generated" << endl;

    cout << square.calculateNumberofInjectiveHomomorphismsTo(G) << endl;
}

//Takes 1.7 Seconds, and produces about 2464 vertices and 700160 edges. So quite fast
TEST(SpeedTest, CFIGraphGeneration) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(20, 60, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
}

//Takes 1 min 13 secs to run and produces 6948 vertices and 4377696 edges
TEST(SpeedTest, CFIGraphGeneration2) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(30, 100, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
}


//At the moment: 31 secs
TEST(SpeedTest, InjectiveHomomorphismIterator) {
    int count = 0;
    for (auto x : InjectiveHomomorphismRange(4, 70)) {
        ++count;
        if (count % 1000000 == 0) {
            cout << count << endl;
        }
    }
    cout << count << endl;
}

//At the moment 32 seconds --> Generation of possible Homomorphisms is really slow, checking if valid is really fast.
TEST(SpeedTest, InjectiveHomomorphismIteratorHomomorphismcountcomparison) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(70, 200, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    Graph square = Graph();
    for (int i = 0; i < 4; i++) {
        square.addNode(Node());
    }
    square.addEdge(0, 1);
    square.addEdge(1, 2);
    square.addEdge(2, 3);
    square.addEdge(3, 0);

    cout << "Graphs generated" << endl;

    cout << square.calculateNumberofInjectiveHomomorphismsTo(G) << endl;
}
