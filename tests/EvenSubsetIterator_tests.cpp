//
// Created by Christoph Greger on 26.12.24.
//


#include <gtest/gtest.h>
#include "EvenSubsetIterator.h"


//I'm not sure if this is a good test case or what it should output
TEST(EvenSubsetIteratorTest, EmptyVector) {
    std::vector<int> vec;
    EvenSubsetIterator<int> it(vec);

    EXPECT_TRUE(it.hasNext());
    auto result = it.next();
    EXPECT_EQ(result.first.size(), 0); // Empty subset
}

TEST(EvenSubsetIteratorTest, SingleElementVector) {
    std::vector<int> vec = {1};
    EvenSubsetIterator<int> it(vec);

    EXPECT_TRUE(it.hasNext());
    auto result = it.next();
    EXPECT_EQ(result.first.size(), 0); // Empty subset
}

TEST(EvenSubsetIteratorTest, TwoElementVector) {
    std::vector<int> vec = {1, 2};
    EvenSubsetIterator<int> it(vec);

    ASSERT_TRUE(it.hasNext());
    auto result = it.next();
    EXPECT_EQ(result.first.size(), 0); // Empty subset
    EXPECT_EQ(result.second.size(), 2);
    EXPECT_EQ(result.second[0], 1);
    EXPECT_EQ(result.second[1], 2);

    ASSERT_TRUE(it.hasNext());
    result = it.next();
    EXPECT_EQ(result.first.size(), 2);
    EXPECT_EQ(result.first[0], 1);
    EXPECT_EQ(result.first[1], 2);
    EXPECT_EQ(result.second.size(), 0);

    EXPECT_FALSE(it.hasNext());
}

TEST(EvenSubsetIteratorTest, ThreeElementVector) {
    std::vector<int> vec = {1, 2, 3};
    EvenSubsetIterator<int> it(vec);

    ASSERT_TRUE(it.hasNext());
    auto result = it.next();
    EXPECT_EQ(result.first.size(), 0); // Empty subset
    EXPECT_EQ(result.second.size(), 3);
    EXPECT_EQ(result.second[0], 1);
    EXPECT_EQ(result.second[1], 2);
    EXPECT_EQ(result.second[2], 3);

    ASSERT_TRUE(it.hasNext());
    result = it.next();
    EXPECT_EQ(result.first.size(), 2);
    EXPECT_EQ(result.first[0], 1);
    EXPECT_EQ(result.first[1], 2);
    EXPECT_EQ(result.second.size(), 1);
    EXPECT_EQ(result.second[0], 3);

    ASSERT_TRUE(it.hasNext());
    result = it.next();
    EXPECT_EQ(result.first.size(), 2);
    EXPECT_EQ(result.first[0], 1);
    EXPECT_EQ(result.first[1], 3);
    EXPECT_EQ(result.second.size(), 1);
    EXPECT_EQ(result.second[0], 2);

    ASSERT_TRUE(it.hasNext());
    result = it.next();
    EXPECT_EQ(result.first.size(), 2);
    EXPECT_EQ(result.first[0], 2);
    EXPECT_EQ(result.first[1], 3);
    EXPECT_EQ(result.second.size(), 1);
    EXPECT_EQ(result.second[0], 1);

    EXPECT_FALSE(it.hasNext());
}


TEST(EvenSubsetIteratorTest, FourElementVector) {
    std::vector<int> vec = {1, 2, 3, 4};
    EvenSubsetIterator<int> it(vec);

    std::vector<std::pair<std::vector<int>, std::vector<int>>> expectedResults = {
            {{}, {1, 2, 3, 4}}, {{1, 2}, {3, 4}}, {{1, 3}, {2, 4}}, {{2, 3}, {1, 4}},
            {{1, 4}, {2, 3}}, {{2, 4}, {1, 3}}, {{3, 4}, {1, 2}},
            {{1, 2, 3, 4}, {}}
    };

    for (const auto& expectedResult : expectedResults) {
        ASSERT_TRUE(it.hasNext());
        auto result = it.next();
        EXPECT_EQ(result.first, expectedResult.first);
        EXPECT_EQ(result.second, expectedResult.second);
    }

    EXPECT_FALSE(it.hasNext());
}