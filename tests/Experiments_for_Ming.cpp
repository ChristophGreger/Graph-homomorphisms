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
