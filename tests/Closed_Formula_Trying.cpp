//
// Created by Christoph Greger on 25.01.25.
//
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <Graph.h>
#include <CFIGraph.h>
#include <utilities.h>
#include <RandomGraphGenerator.h>

TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple) {
    //See the CFI graph from the paper

    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    Graph H = Graph(true);

    for (int i = 0; i < 3; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(2));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(0, 3);

    int expected = intPow(2, 11-8);
    ASSERT_EQ(H.calculateNumberofHomomorphismsTo(input), expected);
}


TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple3) {
    //See the CFI graph from the paper

    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    Graph H = Graph(true);

    for (int i = 0; i < 4; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(2));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(1 , 3);
    H.addEdge(2 , 3);

    H.addEdge(4 , 1);
    H.addEdge(4 , 3);

    ASSERT_EQ(S.calculateNumberofHomomorphismsTo(input), H.calculateNumberofHomomorphismsTo(input) * intPow(2, 3-3));
}



TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple4) {
    //See the CFI graph from the paper

    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    Graph H = Graph(true);

    for (int i = 0; i < 4; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(0));
    H.addNode(Node(1));
    H.addNode(Node(2));
    H.addNode(Node(3));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(1 , 3);
    H.addEdge(2 , 3);

    H.addEdge(1 , 4);
    H.addEdge(4 , 5);
    H.addEdge(4 , 6);
    H.addEdge(5 , 6);
    H.addEdge(5 , 7);
    H.addEdge(6 , 7);


    ASSERT_EQ(H.calculateNumberofHomomorphismsTo(input), S.calculateNumberofHomomorphismsTo(input) * intPow(2, 10-2-2-2-2-1));
}

TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple5) {
    //See the CFI graph from the paper

    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    Graph H = Graph(true);

    for (int i = 0; i < 4; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(0));
    H.addNode(Node(1));
    H.addNode(Node(2));
    H.addNode(Node(3));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(1 , 3);
    H.addEdge(2 , 3);

    H.addEdge(1 , 4);
    H.addEdge(4 , 5);
    H.addEdge(4 , 6);
    H.addEdge(5 , 6);
    H.addEdge(5 , 7);
    //H.addEdge(6 , 7); THE SAME WITH ONE EDGE LESS !!!!!!!!


    ASSERT_EQ(H.calculateNumberofHomomorphismsTo(input), S.calculateNumberofHomomorphismsTo(input) * intPow(2, 10-2-2-2-2-1)); //SAME RESULT !!!!!
}


TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple6) {
    //See the CFI graph from the paper

    Graph S = Graph(true);

    for (int i = 0; i < 4; i++) {
        S.addNode(Node(i));
    }

    S.addEdge(0 , 1);
    S.addEdge(0 , 2);
    S.addEdge(1 , 2);
    S.addEdge(1 , 3);
    S.addEdge(2 , 3);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    Graph H = Graph(true);

    for (int i = 0; i < 4; i++) {
        H.addNode(Node(i));
    }

    H.addNode(Node(3));

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    H.addEdge(1 , 3);
    //H.addEdge(2 , 3);
    H.addEdge(2 , 4);


    ASSERT_EQ(H.calculateNumberofHomomorphismsTo(input), intPow(2, 2));
}

TEST(Closed_Formula_Trying, TryingOut_Automated_CLOSEDFORMULA) {

    for (int snodes = 3; snodes < 12; ++snodes) {
        for (int sedges = snodes - 1; sedges <= snodes * (snodes - 1) / 2; ++sedges) {
            for (int times = 0; times < 100; ++times) {
                RandomGraphGenerator sgen = RandomGraphGenerator(snodes, sedges, true, true);
                Graph S = sgen.generateRandomConnectedGraph();
                CFIGraph X_of_S = CFIGraph(S);
                Graph input = X_of_S.toGraph();

                int hnodes = getRandomNumberBetween(2, 6);
                int hedges = getRandomNumberBetween(hnodes - 1, hnodes * (hnodes - 1) / 2);
                RandomGraphGenerator hgen = RandomGraphGenerator(hnodes, hedges, true, false, snodes, false);
                Graph H = hgen.generateRandomConnectedGraph();
                long long number = H.calculateNumberofhomomorphismsTo_CFI_from(S);
                cout << "Trying for Nodes:" << snodes << ", Edges: " << sedges << " of S and Nodes: " << hnodes << ", Edges: " << hedges << " of H"<< endl;
                cout << number << endl;
                long long countedhoms = H.calculateNumberofHomomorphismsTo(input);
                if (number != countedhoms) {
                    cout << "S: " <<endl;
                    S.printGraph(true);
                    cout << "H: " <<endl;
                    H.printGraph(true);
                }
                ASSERT_EQ(H.calculateNumberofHomomorphismsTo(input), number);
            }
        }
    }
}
