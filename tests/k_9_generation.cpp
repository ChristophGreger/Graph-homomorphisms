//
// Created by Christoph Greger on 07.03.25.
//


#include <gtest/gtest.h>
#include "Spasm.h"


TEST(K_GENERATION, K_9) {
    Spasm::create_and_store_Spasm_k_Matching("k_9.txt", 9);
}