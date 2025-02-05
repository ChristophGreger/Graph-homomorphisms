//
// Created by Christoph Greger on 26.12.24.
//

#ifndef GRAPH_HOMOMORPHISMS_EVENSUBSETITERATOR_H
#define GRAPH_HOMOMORPHISMS_EVENSUBSETITERATOR_H

//This whole thing is ai generated


#include <vector>

template <typename T>
class EvenSubsetIterator {
public:
    /// Iterator to iterate over all subsets of even size of a given vector
    /// @param vec The vector to create the even subsets from
    explicit EvenSubsetIterator(const std::vector<T>& vec);
    bool hasNext() const;
    std::pair<std::vector<T>, std::vector<T>> next();

    // Iterator functions
    EvenSubsetIterator<T>& begin();
    EvenSubsetIterator<T>& end();
    bool operator!=(const EvenSubsetIterator<T>& other) const;
    std::pair<std::vector<T>, std::vector<T>> operator*() const;
    EvenSubsetIterator<T>& operator++();

private:
    const std::vector<T>& vec;
    int subsetMask;
    int maxMask;

    void advanceToNextEvenSubset();
};

#endif //GRAPH_HOMOMORPHISMS_EVENSUBSETITERATOR_H
