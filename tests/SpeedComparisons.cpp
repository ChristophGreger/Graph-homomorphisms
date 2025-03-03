//
// Created by Christoph Greger on 28.12.24.
//

#include <gtest/gtest.h>
#include "RandomGraphGenerator.h"
#include "Graph.h"
#include "CFIGraph.h"

//Takes 1 sec 296ms at the moment
TEST(SpeedTest, Nodes256) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(256, 6343, false);
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

    unsigned long time = clock();

    cout << square.calculateNumberofHomomorphismsTo(G) << endl;

    cout << "Time in ms: " << (clock() - time)/1000 << endl;
}


TEST(SpeedTest, Nodes256_2) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(30, 40, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();

    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    while (H.numVertices > 120) {
        G = randomGraphGenerator.generateRandomConnectedGraph();
        CFI = CFIGraph(G);
        H = CFI.toGraph();
    }

    for (int i = 0; i < 45; i++) {
        RandomGraphGenerator gen2 = RandomGraphGenerator(9, 8, false);
        Graph square = gen2.generateRandomConnectedGraph();


        cout << "Graphs generated" << endl;

        unsigned long time = clock();

        cout << square.calculateNumberofHomomorphismsTo(G) << endl;

        cout << "Time in ms: " << (clock() - time)/1000 << endl;
    }
}

TEST(SpeedTest, JustTrying) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(30, 40, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();

    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    while (H.numVertices > 120) {
        G = randomGraphGenerator.generateRandomConnectedGraph();
        CFI = CFIGraph(G);
        H = CFI.toGraph();
    }

    long long totalnumberofhoms = 0;
    for (int i = 0; i < 45; i++) {
        RandomGraphGenerator gen2 = RandomGraphGenerator(9, 8, false);
        Graph square = gen2.generateRandomConnectedGraph();


        cout << "Graphs generated" << endl;

        unsigned long time = clock();

        long long numhoms = square.calculateNumberofHomomorphismsTo(G);

        totalnumberofhoms += numhoms;

        cout << numhoms << endl;

        cout << "Time in ms: " << (clock() - time)/1000 << endl;
    }

    cout << "Total number of homomorphisms: " << totalnumberofhoms << endl;
}

TEST(SpeedTest, SortingEdges1024) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(1024, 104755, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    G.sortEdges();
}

//Takes 1.7 Seconds, and produces about 2464 vertices and 700160 edges. So quite fast
TEST(SpeedTest, CFIGraphGeneration) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(20, 60, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
}


//takes about 37 ms
TEST(SpeedTest, CounterTimeTest) {
    int count = 0;
    int max = 70 * 69 * 68 * 67;
    for (int i = 0; i < max; i++) {
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

//At the moment 32 seconds --> Generation of possible Homomorphisms is really slow, checking if valid is really fast.
TEST(SpeedTest, HomomorphismIteratorHomomorphismcountcomparison) {
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

    cout << square.calculateNumberofHomomorphismsTo(G) << endl;
}


//Takes 368 ms at the moment after generating the graph
//Thats a lot faster than the master work implementation ??????????????????????
//And than the glasgow subgraph solver etc ?????????????????
TEST(SpeedTest, clique3nequals1000) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(1000, 99900, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    Graph clique3 = Graph();
    for (int i = 0; i < 3; i++) {
        clique3.addNode(Node());
    }
    clique3.addEdge(0, 1);
    clique3.addEdge(1, 2);
    clique3.addEdge(2, 0);

    cout << "Graphs generated" << endl;

    auto time = clock();

    cout << clique3.calculateNumberofHomomorphismsTo(G) << endl;

    cout << "Time in ms: " << (clock() - time)/1000 << endl;
}





