//
// Created by Christoph Greger on 26.12.24.
//

#include <gtest/gtest.h>
#include "HomomorphismRange.h"

using namespace std;

TEST(HomomorphismRangeTest, EmptyRange) {
    HomomorphismRange range(0, 0);
    int count = 0;
    for (auto x : range) {
        count += 1;
    }
    EXPECT_EQ(count, 1);
}

TEST(HomomorphismRangeTest, SingleElementRange) {
    HomomorphismRange range(1, 1);
    auto it = range.begin();
    auto end = range.end();
    ASSERT_NE(it, end);
    EXPECT_EQ(*it, std::vector<int>({0}));
    ++it;
    EXPECT_EQ(it, end);
}

TEST(HomomorphismRangeTest, MultipleElementsRange) {
    HomomorphismRange range(2, 2);
    vector<vector<int>> expected = {
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };

    vector<vector<int>> actual;
    for (auto x : range) {
        actual.push_back(x);
    }
    for (auto x : expected) {
        EXPECT_TRUE(find(actual.begin(), actual.end(), x) != actual.end());
    }
    for (auto x : actual) {
        EXPECT_TRUE(find(expected.begin(), expected.end(), x) != expected.end());
    }
}

TEST(HomomorphismRangeTest, LargerRange) {
    HomomorphismRange range(3, 2);
    std::vector<std::vector<int>> expected = {
            {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
            {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
    };
    vector<vector<int>> actual;
    for (auto x : range) {
        actual.push_back(x);
    }
    for (auto x : expected) {
        EXPECT_TRUE(find(actual.begin(), actual.end(), x) != actual.end());
    }
    for (auto x : actual) {
        EXPECT_TRUE(find(expected.begin(), expected.end(), x) != expected.end());
    }
}

TEST(HomomorphismRangeTest, NumberofMappings) {
    HomomorphismRange range(10, 2);
    int count = 0;
    for (auto x : range) {
        count += 1;
    }
    EXPECT_EQ(count, 1024);
}