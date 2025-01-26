//
// Created by Christoph Greger on 25.01.25.
//
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <Graph.h>
#include <CFIGraph.h>
#include <utilities.h>

TEST(Closed_Formula_Trying, SurjectivelyColored) {
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

    H.addEdge(0 , 1);
    H.addEdge(0 , 2);
    H.addEdge(1 , 2);
    //H.addEdge(1 , 3);
    H.addEdge(2 , 3);

    ASSERT_EQ(S.calculateNumberofHomomorphismsTo(input), H.calculateNumberofHomomorphismsTo(input)/2);

}


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

TEST(Closed_Formula_Trying, NonSurjectivelyColoredSimple2) {
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
    H.addEdge(1, 3);

    int expected = intPow(2, 11-8);
    ASSERT_EQ(H.calculateNumberofHomomorphismsTo(input), expected/2);
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
