//
// Created by Christoph Greger on 27.12.24.
//


#include <gtest/gtest.h>
#include "InjectiveHomomorphismRange.h"

using namespace std;

TEST(InjectiveHomomorphismRangeTest, EmptyRange) {
    InjectiveHomomorphismRange range(0, 0);
    int count = 0;
    for (auto x: range) {
        count += 1;
    }
    EXPECT_EQ(count, 1);
}

TEST(InjectiveHomomorphismRangeTest, SingleElementRange) {
    InjectiveHomomorphismRange range(1, 1);
    auto it = range.begin();
    auto end = range.end();
    ASSERT_NE(it, end);
    EXPECT_EQ(*it, std::vector<int>({0}));
    ++it;
    EXPECT_EQ(it, end);
}

TEST(InjectiveHomomorphismRangeTest, MultipleElementsRange) {
    InjectiveHomomorphismRange range(2, 3);
    vector<vector<int>> expected = {
            {0, 1},
            {0, 2},
            {1, 0},
            {1, 2},
            {2, 0},
            {2, 1}
    };

    vector<vector<int>> actual;
    for (auto x: range) {
        actual.push_back(x);
    }
    for (auto x: expected) {
        EXPECT_TRUE(find(actual.begin(), actual.end(), x) != actual.end());
    }
    for (auto x: actual) {
        EXPECT_TRUE(find(expected.begin(), expected.end(), x) != expected.end());
    }
}

TEST(InjectiveHomomorphismRangeTest, LargerRange) {
    InjectiveHomomorphismRange range(3, 4);
    vector<vector<int>> expected = {
            {0, 1, 2},
            {0, 1, 3},
            {0, 2, 1},
            {0, 2, 3},
            {0, 3, 1},
            {0, 3, 2},
            {1, 0, 2},
            {1, 0, 3},
            {1, 2, 0},
            {1, 2, 3},
            {1, 3, 0},
            {1, 3, 2},
            {2, 0, 1},
            {2, 0, 3},
            {2, 1, 0},
            {2, 1, 3},
            {2, 3, 0},
            {2, 3, 1},
            {3, 0, 1},
            {3, 0, 2},
            {3, 1, 0},
            {3, 1, 2},
            {3, 2, 0},
            {3, 2, 1}
    };

    vector<vector<int>> actual;
    for (auto x: range) {
        actual.push_back(x);
    }
    for (auto x: expected) {
        EXPECT_TRUE(find(actual.begin(), actual.end(), x) != actual.end());
    }
    for (auto x: actual) {
        EXPECT_TRUE(find(expected.begin(), expected.end(), x) != expected.end());
    }
}

TEST(InjectiveHomomorphismRangeTest, NumberofMappings) {
    InjectiveHomomorphismRange range(3, 5);
    int count = 0;
    for (auto x: range) {
        count += 1;
    }
    EXPECT_EQ(count, 60); // 5P3 = 5! / (5-3)! = 60
}