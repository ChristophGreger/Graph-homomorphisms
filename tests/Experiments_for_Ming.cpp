//
// Created by Christoph Greger on 12.03.25.
//


#include <gtest/gtest.h>

#include "CalcHoms.h"
#include "CFIGraph.h"
#include "RandomGraphGenerator.h"
#include "utilities.h"


TEST(Experiments, M_3) {
    GraphTemplate k_1_3_t = GraphTemplate(false);
    for (int i = 0; i < 4; i++) {
        k_1_3_t.addNode(Node());
    }

    k_1_3_t.addEdge(0, 1);
    k_1_3_t.addEdge(0, 2);
    k_1_3_t.addEdge(0, 3);
    Graph k_1_3 = Graph(k_1_3_t);

    cout << "Normal CFI : " << CalcHoms::calcNumInjHoms("k_3.txt", k_1_3, true, false) << endl;
    cout << "Inverted CFI : " << CalcHoms::calcNumInjHoms("k_3.txt", k_1_3, true, true) << endl;
}

TEST(Experiments, Perfect) {
    for (int i = 0; i < 1; i++) {
        RandomGraphGenerator cfigen = RandomGraphGenerator(6, 10, true, true);
        Graph cfi = cfigen.generateRandomConnectedGraph();
        CFIGraph cfiGraph = CFIGraph(cfi);
        Graph cfiGraphGraph = cfiGraph.toGraph();
        /*if (cfiGraphGraph.numVertices != 18) {
            continue;
        }*/
        auto x = CalcHoms::calcNumInjHoms("k_9.txt", cfi, true, false);
        auto y = CalcHoms::calcNumInjHoms("k_9.txt", cfi, true, true);
        if (x == y) {
            //cout << "GLEICH!!!!!!! Die Nächsten!" << endl;
        }
        if (x < y) {
            cout << "Hier ist normal kleiner als inverted !!!!!" << endl;
            //cfi.printGraph();
        }
        if (x != y) {
            cout << "Normal CFI : " << x  << " Inverted : " << y<< endl;
        }

    }
}
