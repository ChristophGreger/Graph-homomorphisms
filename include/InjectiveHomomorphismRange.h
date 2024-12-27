//
// Created by Christoph Greger on 27.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_INJECTIVEHOMOMORPHISMRANGE_H
#define GRAPH_HOMOMORPHISMS_INJECTIVEHOMOMORPHISMRANGE_H

#include <vector>
#include <cstdint>
#include <iterator>

/**
 * @brief A forward iterator that traverses all injective mappings {0..n-1} -> {0..m-1}.
 *
 * Each mapping is represented as a std::vector<int>.
 */
class InjectiveHomomorphismIterator {
public:
    // Type definitions according to the C++ Iterator concept
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::vector<int>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

    /**
     * @brief Constructor
     * @param n Number of elements in the domain (0..n-1)
     * @param m Number of elements in the codomain (0..m-1)
     * @param index Current counter in [0 .. P(m, n)]
     */
    InjectiveHomomorphismIterator(int n, int m, long long index);

    /**
     * @brief Dereference operator
     * @return Returns the current std::vector<int> representing the mapping
     */
    value_type operator*() const;

    /**
     * @brief Prefix increment: moves to the next mapping
     */
    InjectiveHomomorphismIterator& operator++();

    /**
     * @brief Equality operator
     */
    bool operator==(const InjectiveHomomorphismIterator& other) const;

    /**
     * @brief Inequality operator
     */
    bool operator!=(const InjectiveHomomorphismIterator& other) const;

private:
    /**
     * @brief Decodes index_ into a permutation and stores it in currentMapping_.
     */
    void decode();

    int n_;
    int m_;
    long long index_;
    std::vector<int> currentMapping_;
};

/**
 * @brief A "container" class that provides a [begin(), end()] range
 *        of all injective mappings {0..n-1} -> {0..m-1}.
 */
class InjectiveHomomorphismRange {
public:
    /**
     * @brief Constructor
     * @param n Number of elements in the domain (0..n-1)
     * @param m Number of elements in the codomain (0..m-1)
     */
    InjectiveHomomorphismRange(int n, int m);

    /**
     * @brief Returns the beginning of the iterator range
     */
    InjectiveHomomorphismIterator begin() const;

    /**
     * @brief Returns the end of the iterator range (one past the last element)
     */
    InjectiveHomomorphismIterator end() const;

private:
    int n_;
    int m_;
    long long total_;  ///< total_ = P(m, n) = m! / (m - n)!
};

#endif //GRAPH_HOMOMORPHISMS_INJECTIVEHOMOMORPHISMRANGE_H
