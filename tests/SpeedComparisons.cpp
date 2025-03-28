//
// Created by Christoph Greger on 28.12.24.
//

#include <gtest/gtest.h>

#include "CalcHoms.h"
#include "RandomGraphGenerator.h"
#include "Graph.h"
#include "CFIGraph.h"

//Takes 1 sec 296ms at the moment
TEST(SpeedTest, Nodes256) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(256, 6343, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    GraphTemplate squaret = GraphTemplate();
    for (int i = 0; i < 4; i++) {
        squaret.addNode(Node());
    }
    squaret.addEdge(0, 1);
    squaret.addEdge(1, 2);
    squaret.addEdge(2, 3);
    squaret.addEdge(3, 0);

    Graph square = Graph(squaret);

    cout << "Graphs generated" << endl;

    unsigned long time = clock();

    cout << CalcHoms::calcNumHoms(square, G) << endl;

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

    for (int i = 0; i < 2; i++) {
        RandomGraphGenerator gen2 = RandomGraphGenerator(9, 8, false);
        Graph square = gen2.generateRandomConnectedGraph();


        cout << "Graphs generated" << endl;

        unsigned long time = clock();

        cout << CalcHoms::calcNumHoms(square, G) << endl;

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
    for (int i = 0; i < 23; i++) {
        RandomGraphGenerator gen2 = RandomGraphGenerator(8, 7, false);
        Graph square = gen2.generateRandomConnectedGraph();

        cout << "Graphs generated" << endl;

        unsigned long time = clock();

        long long numhoms = CalcHoms::calcNumHoms(square, G);

        totalnumberofhoms += numhoms;

        cout << numhoms << endl;

        cout << "Time in ms: " << (clock() - time)/1000 << endl;
    }

    cout << "Total number of homomorphisms: " << totalnumberofhoms << endl;
}

//Takes 1.7 Seconds, and produces about 2464 vertices and 700160 edges. So quite fast
TEST(SpeedTest, CFIGraphGeneration) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(20, 60, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    Graph H = CFI.toGraph();
    cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
}


TEST(SpeedTest, HomomorphismIteratorHomomorphismcountcomparison) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(70, 200, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    GraphTemplate squaret = GraphTemplate();
    for (int i = 0; i < 4; i++) {
        squaret.addNode(Node());
    }
    squaret.addEdge(0, 1);
    squaret.addEdge(1, 2);
    squaret.addEdge(2, 3);
    squaret.addEdge(3, 0);

    Graph square = Graph(squaret);

    cout << "Graphs generated" << endl;

    cout << CalcHoms::calcNumHoms(square, G) << endl;
}


//Takes 368 ms at the moment after generating the graph
TEST(SpeedTest, clique3nequals1000) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(1000, 99900, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    GraphTemplate clique3t = GraphTemplate();
    for (int i = 0; i < 3; i++) {
        clique3t.addNode(Node());
    }
    clique3t.addEdge(0, 1);
    clique3t.addEdge(1, 2);
    clique3t.addEdge(2, 0);

    Graph clique3 = Graph(clique3t);

    cout << "Graphs generated" << endl;

    auto time = clock();

    cout << CalcHoms::calcNumHoms(clique3, G) << endl;

    cout << "Time in ms: " << (clock() - time)/1000 << endl;
}





