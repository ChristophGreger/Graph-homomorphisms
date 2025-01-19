//
// Created by Christoph Greger on 19.01.25.
//

#include <gtest/gtest.h>
#include "NextInjection.h"
#include <vector>

// TEST 1: Basic enumeration with n=2, m=4
// We expect the following sequence of injections in lex order:
//  0 1
//  0 2
//  0 3
//  1 0
//  1 2
//  1 3
//  2 0
//  2 1
//  2 3
//  3 0
//  3 1
//  3 2
TEST(NextInjectionTest, Enumerate2Of4) {
    int n = 2;
    int m = 4;
    NextInjection ni(n, m);

    // The expected sequence in a 2D list
    std::vector<std::vector<int>> expected = {
        {0, 1},
        {0, 2},
        {0, 3},
        {1, 0},
        {1, 2},
        {1, 3},
        {2, 0},
        {2, 1},
        {2, 3},
        {3, 0},
        {3, 1},
        {3, 2}
    };

    size_t idx = 0;

    // First injection should match expected[0]
    {
        auto curr = ni.current();
        EXPECT_EQ(curr, expected[idx]) << "Initial injection mismatch at index=0";
    }

    // Now call next() repeatedly and compare
    while (ni.next()) {
        idx++;
        ASSERT_LT(idx, expected.size())
            << "We got more injections than expected (index ran out of range).";

        auto curr = ni.current();
        EXPECT_EQ(curr, expected[idx])
            << "Mismatch at index=" << idx << " (in the enumerated sequence).";
    }

    // At this point, next() returned false => we must have enumerated them all
    // Check that we reached the last one exactly
    EXPECT_EQ(idx, expected.size() - 1)
        << "We didn't get as many injections as expected.";
}

// TEST 2: n = 0
// There's exactly one injection: the "empty" mapping
TEST(NextInjectionTest, ZeroCase) {
    NextInjection ni(0, 10);
    // With n=0, we effectively have an empty injection.
    // 'next()' should immediately return false after the initial.
    {
        const auto& curr = ni.current();
        EXPECT_TRUE(curr.empty())
            << "For n=0, the current injection should be empty.";
    }
    bool hasNext = ni.next();
    EXPECT_FALSE(hasNext) << "For n=0, there should be no 'next' injection.";
}

// TEST 3: n > m => no valid injection
// next() should return false from the start
TEST(NextInjectionTest, InvalidCase) {
    NextInjection ni(5, 3); // n=5, m=3 => can't have an injective mapping
    // The first 'next()' should fail immediately
    bool hasNext = ni.next();
    EXPECT_FALSE(hasNext)
        << "No valid injections when n > m; next() should be false immediately.";

    // The "current" array is sized n=5, but it doesn't represent a valid injection.
    // Implementation-defined, but typically it's [0,1,2,3,4].
    // This test just ensures that next() won't iterate.
}

// TEST 4: Single-step checks with n=3, m=5
// We'll step through the first few injections manually.
TEST(NextInjectionTest, SmallCheck3of5) {
    NextInjection ni(3, 5);
    // initial => arr = {0,1,2}
    EXPECT_EQ(ni.current(), (std::vector<int>{0,1,2}));

    // next => {0,1,3}
    bool hasNext = ni.next();
    EXPECT_TRUE(hasNext);
    EXPECT_EQ(ni.current(), (std::vector<int>{0,1,3}));

    // next => {0,1,4}
    hasNext = ni.next();
    EXPECT_TRUE(hasNext);
    EXPECT_EQ(ni.current(), (std::vector<int>{0,1,4}));

    // next => {0,2,1}
    hasNext = ni.next();
    EXPECT_TRUE(hasNext);
    EXPECT_EQ(ni.current(), (std::vector<int>{0,2,1}));

    // We won't enumerate all permutations here, just verifying correct stepping.
}

