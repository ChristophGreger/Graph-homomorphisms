//
// Created by Christoph Greger on 25.01.25.
//
#include <gtest/gtest.h>
#include <iostream>
#include <Graph.h>
#include <CFIGraph.h>
#include <utilities.h>
#include <RandomGraphGenerator.h>
#include <exception>

#include "CalcHoms.h"

using std::cout;
using std::endl;

TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple) {
    // See the CFI graph from the paper

    GraphTemplate tS(true);
    for (int i = 0; i < 4; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    tS.addEdge(2, 3);
    Graph S(tS);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    GraphTemplate tH(true);
    for (int i = 0; i < 3; i++) {
        tH.addNode(Node(i));
    }
    tH.addNode(Node(2));
    tH.addEdge(0, 1);
    tH.addEdge(0, 2);
    tH.addEdge(1, 2);
    tH.addEdge(0, 3);
    Graph H(tH);

    int expected = intPow(2, 11 - 8);
    ASSERT_EQ(CalcHoms::calcNumHoms(H,input), expected);
}


TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple3) {
    // See the CFI graph from the paper

    GraphTemplate tS(true);
    for (int i = 0; i < 4; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    tS.addEdge(2, 3);
    Graph S(tS);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    GraphTemplate tH(true);
    for (int i = 0; i < 4; i++) {
        tH.addNode(Node(i));
    }
    tH.addNode(Node(2));
    tH.addEdge(0, 1);
    tH.addEdge(0, 2);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    tH.addEdge(2, 3);
    tH.addEdge(4, 1);
    tH.addEdge(4, 3);
    Graph H(tH);

    ASSERT_EQ(CalcHoms::calcNumHoms(S,input),
              CalcHoms::calcNumHoms(H,input) * intPow(2, 3 - 3));
}


TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple4) {
    // See the CFI graph from the paper

    GraphTemplate tS(true);
    for (int i = 0; i < 4; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    tS.addEdge(2, 3);
    Graph S(tS);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    GraphTemplate tH(true);
    for (int i = 0; i < 4; i++) {
        tH.addNode(Node(i));
    }
    // Adding extra nodes
    tH.addNode(Node(0));
    tH.addNode(Node(1));
    tH.addNode(Node(2));
    tH.addNode(Node(3));

    tH.addEdge(0, 1);
    tH.addEdge(0, 2);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    tH.addEdge(2, 3);

    tH.addEdge(1, 4);
    tH.addEdge(4, 5);
    tH.addEdge(4, 6);
    tH.addEdge(5, 6);
    tH.addEdge(5, 7);
    tH.addEdge(6, 7);
    Graph H(tH);

    ASSERT_EQ(CalcHoms::calcNumHoms(H,input),
              CalcHoms::calcNumHoms(S,input) * intPow(2, 10 - 2 - 2 - 2 - 2 - 1));
}


TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple5) {
    // See the CFI graph from the paper

    GraphTemplate tS(true);
    for (int i = 0; i < 4; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    tS.addEdge(2, 3);
    Graph S(tS);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    GraphTemplate tH(true);
    for (int i = 0; i < 4; i++) {
        tH.addNode(Node(i));
    }
    tH.addNode(Node(0));
    tH.addNode(Node(1));
    tH.addNode(Node(2));
    tH.addNode(Node(3));

    tH.addEdge(0, 1);
    tH.addEdge(0, 2);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    tH.addEdge(2, 3);

    tH.addEdge(1, 4);
    tH.addEdge(4, 5);
    tH.addEdge(4, 6);
    tH.addEdge(5, 6);
    tH.addEdge(5, 7);
    // Note: The next edge is commented out (i.e. one edge less).
    // tH.addEdge(6, 7);
    Graph H(tH);

    ASSERT_EQ(CalcHoms::calcNumHoms(H,input),
              CalcHoms::calcNumHoms(S,input) * intPow(2, 10 - 2 - 2 - 2 - 2 - 1)); // SAME RESULT!!!!
}


TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple6) {
    // See the CFI graph from the paper

    GraphTemplate tS(true);
    for (int i = 0; i < 4; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    tS.addEdge(2, 3);
    Graph S(tS);

    CFIGraph X_of_S = CFIGraph(S);
    Graph input = X_of_S.toGraph();

    GraphTemplate tH(true);
    for (int i = 0; i < 4; i++) {
        tH.addNode(Node(i));
    }
    tH.addNode(Node(3));
    tH.addEdge(0, 1);
    tH.addEdge(0, 2);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    // tH.addEdge(2, 3); // omitted
    tH.addEdge(2, 4);
    Graph H(tH);

    ASSERT_EQ(CalcHoms::calcNumHoms(H,input), intPow(2, 2));
}


TEST(CLOSED, Example) {
    GraphTemplate tS(true);
    for (int i = 0; i < 5; i++) {
        tS.addNode(Node(i));
    }
    tS.addEdge(0, 1);
    tS.addEdge(0, 2);
    tS.addEdge(0, 3);
    tS.addEdge(0, 4);
    tS.addEdge(1, 2);
    tS.addEdge(1, 3);
    Graph S(tS);

    GraphTemplate tH(true);
    tH.addNode(Node(2));
    tH.addNode(Node(0));
    tH.addNode(Node(2));
    tH.addNode(Node(4));
    tH.addEdge(1, 0);
    tH.addEdge(1, 2);
    tH.addEdge(1, 3);
    Graph H(tH);

    int* mapping = new int[4];
    mapping[0] = 2;
    mapping[1] = 0;
    mapping[2] = 2;
    mapping[3] = 4;

    ASSERT_EQ(intPow(2,CalcHoms::calcNumHomsCFI(H,S,mapping)), 4);

    delete[] mapping;
}


TEST(Closed_Formula_Trying, TryingOut_Automated_CLOSEDFORMULA2) {

    for (int snodes = 3; snodes < 9; ++snodes) {
        for (int sedges = snodes - 1; sedges <= snodes * (snodes - 1) / 2; ++sedges) {
            for (int times = 0; times < 10; ++times) {
                RandomGraphGenerator sgen = RandomGraphGenerator(snodes, sedges, true, true);
                Graph S = sgen.generateRandomConnectedGraph();
                CFIGraph X_of_S = CFIGraph(S);
                Graph input = X_of_S.toGraph();

                int hnodes = getRandomNumberBetween(2, 6);
                int hedges = getRandomNumberBetween(hnodes - 1, hnodes * (hnodes - 1) / 2);
                RandomGraphGenerator hgen = RandomGraphGenerator(hnodes, hedges, true, false, snodes, false);
                Graph H;
                try {
                    H = hgen.generateRandomConnectedGraph();
                } catch (std::exception &e) {
                    continue;
                }
                int* mapping = new int[hnodes];
                for (int i = 0; i < hnodes; ++i) {
                    mapping[i] = H.nodes[i].color;
                }

                int expo = CalcHoms::calcNumHomsCFI(H,S,mapping);

                long long number = 0;
                if (expo != -1) {
                    number = intPow(2,expo);
                }

                cout << "Starting BruteForce" << endl;
                long long countedhoms = CalcHoms::calcNumHoms(H,input);
                cout << "Ending BruteForce" << endl;

                if (number != countedhoms) {
                    cout << "S: " << endl;
                    S.printGraph(true);
                    cout << "H: " << endl;
                    H.printGraph(true);
                }
                cout << "Number: " << number << " Counted: " << countedhoms << endl;
                ASSERT_EQ(countedhoms, number);

                delete[] mapping;
            }
        }
    }
}
