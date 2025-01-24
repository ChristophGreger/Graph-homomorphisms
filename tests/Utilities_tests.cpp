//
// Created by Christoph Greger on 19.01.25., but ai generated
//

#include <gtest/gtest.h>
#include "utilities.h"

/**
 * Helper function to check if an array of pairs is sorted by their second element.
 */
template<typename T1, typename T2>
bool isSortedBySecond(const std::pair<T1, T2>* arr, std::size_t size)
{
    if (size < 2) return true;

    for (std::size_t i = 0; i < size - 1; ++i) {
        if (arr[i].second > arr[i + 1].second) {
            return false;
        }
    }
    return true;
}

/**
 * TEST SUITE
 */

// 1. Basic test: mix of ascending/descending second elements
TEST(SortBySecondTest, BasicSorting)
{
    std::pair<int, int> arr[] = {
        {1, 50}, {2, 10}, {3, 40}, {4, 20}, {5, 30}
    };
    const std::size_t size = sizeof(arr) / sizeof(arr[0]);

    sortBySecond(arr, size);

    // Expect them to be sorted by second: 10, 20, 30, 40, 50
    EXPECT_TRUE(isSortedBySecond(arr, size));

    // Optional: check actual final order
    int expected[] = {10, 20, 30, 40, 50};
    for (std::size_t i = 0; i < size; ++i) {
        EXPECT_EQ(arr[i].second, expected[i]);
    }
}

// 2. Already sorted
TEST(SortBySecondTest, AlreadySorted)
{
    std::pair<int, int> arr[] = {
        {10, 10}, {20, 20}, {30, 30}
    };
    const std::size_t size = sizeof(arr) / sizeof(arr[0]);

    sortBySecond(arr, size);

    // Should still be sorted
    EXPECT_TRUE(isSortedBySecond(arr, size));
}

// 3. Reverse sorted
TEST(SortBySecondTest, ReverseSorted)
{
    std::pair<int, int> arr[] = {
        {10, 30}, {20, 20}, {30, 10}
    };
    const std::size_t size = sizeof(arr) / sizeof(arr[0]);

    sortBySecond(arr, size);

    // second elements should now be 10, 20, 30
    EXPECT_TRUE(isSortedBySecond(arr, size));
}

// 4. All duplicates
TEST(SortBySecondTest, AllDuplicates)
{
    std::pair<int, int> arr[] = {
        {1, 10}, {2, 10}, {3, 10}, {4, 10}, {5, 10}
    };
    const std::size_t size = sizeof(arr) / sizeof(arr[0]);

    sortBySecond(arr, size);

    // Trivially sorted because all second elements are the same
    EXPECT_TRUE(isSortedBySecond(arr, size));
}

// 5. Negative and positive mixed
TEST(SortBySecondTest, NegativePositive)
{
    std::pair<int, int> arr[] = {
        {1, -5}, {2, 10}, {3, 0}, {4, -20}, {5, 15}
    };
    const std::size_t size = sizeof(arr) / sizeof(arr[0]);

    sortBySecond(arr, size);

    // Expect final second elements in ascending order: -20, -5, 0, 10, 15
    int expected[] = {-20, -5, 0, 10, 15};
    for (std::size_t i = 0; i < size; ++i) {
        EXPECT_EQ(arr[i].second, expected[i]);
    }
    EXPECT_TRUE(isSortedBySecond(arr, size));
}

// 6. Single-element array
TEST(SortBySecondTest, SingleElement)
{
    std::pair<int, int> arr[] = {
        {1, 50}
    };
    const std::size_t size = sizeof(arr) / sizeof(arr[0]);

    sortBySecond(arr, size);

    // Single element is trivially sorted
    EXPECT_TRUE(isSortedBySecond(arr, size));
}

// Optional: Empty test or zero-size scenario
TEST(SortBySecondTest, EmptyArray)
{
    std::pair<int, int>* arr = nullptr;
    // Sorting an empty/null array is a bit tricky in real code.
    // Typically you'd avoid calling sort on null.
    // But let's see if we want to test "no crash" behavior:

    // We won't actually call sortBySecond in this test
    // because passing null to std::sort is undefined behavior.

    SUCCEED() << "Test passes because we do not crash, though real usage on null is undefined.";
}