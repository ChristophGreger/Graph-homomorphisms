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

    for (int i = 0; i < 100; i++) {
        RandomGraphGenerator gen2 = RandomGraphGenerator(10, 9, false);
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
    for (int i = 0; i < 23; i++) {
        RandomGraphGenerator gen2 = RandomGraphGenerator(8, 7, false);
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


//1 min last time executed with edge density = 0.2. With density 0.1 it takes 6 seconds
TEST(SpeedTest, Nodes1024) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(1024, 104755, false);
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

//Takes 1 min 13 secs to run and produces 6948 vertices and 4377696 edges
TEST(SpeedTest, CFIGraphGeneration2) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(30, 100, true, true);
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


//Takes 731ms, thats much faster than the master work implementation and glasgow subgraph solver
TEST(SpeedTest, clique4nequals150) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(150, 6705, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    Graph clique4 = Graph();
    for (int i = 0; i < 4; i++) {
        clique4.addNode(Node());
    }
    clique4.addEdge(0, 1);
    clique4.addEdge(1, 2);
    clique4.addEdge(2, 0);
    clique4.addEdge(3, 0);
    clique4.addEdge(3, 1);
    clique4.addEdge(3, 2);


    cout << "Graphs generated" << endl;

    auto time = clock();

    cout << clique4.calculateNumberofHomomorphismsTo(G) << endl;

    cout << "Time in ms: " << (clock() - time)/1000 << endl;
}

//1.2 seconds, also faster than the master work implementation and glasgow subgraph solver
TEST(SpeedTest, clique5nequals60) {
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(60, 1416, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    Graph clique5 = Graph();
    for (int i = 0; i < 5; i++) {
        clique5.addNode(Node());
    }
    clique5.addEdge(0, 1);
    clique5.addEdge(1, 2);
    clique5.addEdge(2, 0);
    clique5.addEdge(3, 0);
    clique5.addEdge(3, 1);
    clique5.addEdge(3, 2);
    clique5.addEdge(4, 0);
    clique5.addEdge(4, 1);
    clique5.addEdge(4, 2);
    clique5.addEdge(4, 3);

    cout << "Graphs generated" << endl;

    auto time = clock();

    cout << clique5.calculateNumberofHomomorphismsTo(G) << endl;

    cout << "Time in ms: " << (clock() - time)/1000 << endl;
}


TEST(GraphHomTest, HomTest) {
    Graph k_7(false);
    for (int i = 0; i < 8; i++) {
        k_7.addNode(Node());
    }

    k_7.addEdge(0, 1);
    k_7.addEdge(1, 2);
    k_7.addEdge(2, 3);
    k_7.addEdge(3, 4);
    k_7.addEdge(4, 5);
    k_7.addEdge(5, 6);
    k_7.addEdge(6, 7);


    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(45, 100, false);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    cout << "Graphs generated" << endl;
    cout << "Number of homomorphisms: " << k_7.calculateNumberofHomomorphismsTo(G) << endl;
}


