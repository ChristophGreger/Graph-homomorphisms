//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_HOMOMORPHISMRANGE_H
#define GRAPH_HOMOMORPHISMS_HOMOMORPHISMRANGE_H

#include <vector>
#include <cstdint>
#include <iterator>

//This whole thing is ai generated

/**
 * @brief A forward iterator that traverses all mappings {0..n-1} -> {0..m-1}.
 *
 * Each mapping is represented as a std::vector<int>.
 */
class HomomorphismIterator {
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
     * @param index Current counter in [0 .. m^n]
     */
    HomomorphismIterator(int n, int m, long long index);

    /**
     * @brief Dereference operator
     * @return Returns the current std::vector<int> representing the mapping
     */
    value_type operator*() const;

    /**
     * @brief Prefix increment: moves to the next mapping
     */
    HomomorphismIterator& operator++();

    /**
     * @brief Equality operator
     */
    bool operator==(const HomomorphismIterator& other) const;

    /**
     * @brief Inequality operator
     */
    bool operator!=(const HomomorphismIterator& other) const;

private:
    /**
     * @brief Decodes index_ into an m-ary representation and stores it in currentMapping_.
     *
     * The resulting values will be in the range [0..m-1].
     */
    void decode();

    int n_;
    int m_;
    long long index_;
    std::vector<int> currentMapping_;
};

/**
 * @brief A "container" class that provides a [begin(), end()] range
 *        of all mappings {0..n-1} -> {0..m-1}.
 */
class HomomorphismRange {
public:
    /**
     * @brief Constructor
     * @param n Number of elements in the domain (0..n-1)
     * @param m Number of elements in the codomain (0..m-1)
     */
    HomomorphismRange(int n, int m);

    /**
     * @brief Returns the beginning of the iterator range
     */
    HomomorphismIterator begin() const;

    /**
     * @brief Returns the end of the iterator range (one past the last element)
     */
    HomomorphismIterator end() const;

private:
    int n_;
    int m_;
    long long total_;  ///< total_ = m^n
};


#endif //GRAPH_HOMOMORPHISMS_HOMOMORPHISMRANGE_H
