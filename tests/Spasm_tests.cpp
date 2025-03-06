//
// Created by Christoph Greger on 01.03.25.
//


#include "CalcHoms.h"
#include "gtest/gtest.h"
#include "Graph.h"
#include "Spasm.h"

TEST(Spasm, generate_spasm_files) {
    for (int k = 1; k <= 7; ++k) {
        std::string filename = "k_" + std::to_string(k) + ".txt";
        Spasm::create_and_store_Spasm_k_Matching(filename, k);
        cout << "Finished k = " << k << endl;
    }
}

