//
// Created by Christoph Greger on 01.03.25.
//


#include "gtest/gtest.h"
#include "Graph.h"

// Test, ob wiederholte Aufrufe der kanonischen Form konsistent sind.
TEST(CanonicalStringTest, Consistency) {
    // Erzeuge einen Graphen (z. B. ein Dreieck)
    Graph g(false);
    // Wir gehen davon aus, dass Node einen Default-Konstruktor besitzt.
    g.addNode(Node());
    g.addNode(Node());
    g.addNode(Node());
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    std::string canon1 = g.canonicalString();
    std::string canon2 = g.canonicalString();

    EXPECT_EQ(canon1, canon2);
}

// Test, ob zwei isomorphe Graphen dieselbe kanonische Form liefern.
TEST(CanonicalStringTest, IsomorphicGraphs) {
    // Erzeuge den ersten Graphen
    Graph g1(false);
    g1.addNode(Node());
    g1.addNode(Node());
    g1.addNode(Node());
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(0, 2);

    // Erzeuge einen zweiten Graphen, der isomorph zu g1 ist,
    // aber die Knoten und Kanten in anderer Reihenfolge hinzugefügt wurden.
    Graph g2(false);
    g2.addNode(Node());
    g2.addNode(Node());
    g2.addNode(Node());
    // Die Reihenfolge der Kanten ist anders, trotzdem die gleiche Struktur:
    g2.addEdge(1, 2);
    g2.addEdge(0, 2);
    g2.addEdge(0, 1);

    std::string canon1 = g1.canonicalString();
    std::string canon2 = g2.canonicalString();

    EXPECT_EQ(canon1, canon2);
}


// test für die spasm generierung
TEST(SpasmGeneration, test_k_equals_7) {

    Graph k_5(false);
    for (int i = 0; i < 14; i++) {
        k_5.addNode(Node());
    }

    k_5.addEdge(0, 1);
    k_5.addEdge(2, 3);
    k_5.addEdge(4, 5);
    k_5.addEdge(6, 7);
    k_5.addEdge(8, 9);
    k_5.addEdge(10, 11);
    k_5.addEdge(12, 13);

    k_5.enumerateQuotientGraphs_K_Matching("EineTestDatei_k_5.txt");

    cout << "Done!" << endl;
}


// test für die spasm generierung
TEST(SpasmGeneration, test_k_equals_2) {

    Graph k_5(false);
    for (int i = 0; i < 4; i++) {
        k_5.addNode(Node());
    }

    k_5.addEdge(0, 1);
    k_5.addEdge(2, 3);

    k_5.enumerateQuotientGraphs_K_Matching("EineTestDatei_k_2.txt");

    cout << "Done!" << endl;
}

// test für die spasm generierung
TEST(SpasmGeneration, test_k_equals_5) {

    Graph k_5(false);
    for (int i = 0; i < 14; i++) {
        k_5.addNode(Node());
    }

    k_5.addEdge(0, 1);
    k_5.addEdge(2, 3);
    k_5.addEdge(4, 5);
    k_5.addEdge(6, 7);
    k_5.addEdge(8, 9);
    k_5.addEdge(10, 11);
    k_5.addEdge(12, 13);

    k_5.enumerateQuotientGraphs_K_Matching("EineTestDatei_k_7.txt");

    cout << "Done!" << endl;
}


// test für die spasm generierung
TEST(SpasmGeneration, test_k_equals_8) {

    Graph k_5(false);
    for (int i = 0; i < 16; i++) {
        k_5.addNode(Node());
    }

    k_5.addEdge(0, 1);
    k_5.addEdge(2, 3);
    k_5.addEdge(4, 5);
    k_5.addEdge(6, 7);
    k_5.addEdge(8, 9);
    k_5.addEdge(10, 11);
    k_5.addEdge(12, 13);
    k_5.addEdge(14, 15);

    k_5.enumerateQuotientGraphs_K_Matching("EineTestDatei_k_8.txt");

    cout << "Done!" << endl;
}