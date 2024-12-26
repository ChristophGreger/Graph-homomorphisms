//
// Created by Christoph Greger on 26.12.24.
//

#include "EvenSubsetIterator.h"
#include <cmath>

template <typename T>
EvenSubsetIterator<T>::EvenSubsetIterator(const std::vector<T>& vec) : vec(vec), subsetMask(0) {
    maxMask = (1 << vec.size());
    // No need to call advanceToNextEvenSubset() here, as we start with the empty subset
}

template <typename T>
bool EvenSubsetIterator<T>::hasNext() const {
    return subsetMask < maxMask;
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>> EvenSubsetIterator<T>::next() {
    std::vector<T> subset;
    std::vector<T> remaining;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (subsetMask & (1 << i)) {
            subset.push_back(vec[i]);
        } else {
            remaining.push_back(vec[i]);
        }
    }
    advanceToNextEvenSubset();
    return {subset, remaining};
}

template <typename T>
void EvenSubsetIterator<T>::advanceToNextEvenSubset() {
    do {
        ++subsetMask;
    } while (subsetMask < maxMask && __builtin_popcount(subsetMask) % 2 != 0);
}

// Iterator functions
template <typename T>
EvenSubsetIterator<T>& EvenSubsetIterator<T>::begin() {
    subsetMask = 0;
    return *this;
}

template <typename T>
EvenSubsetIterator<T>& EvenSubsetIterator<T>::end() {
    subsetMask = maxMask;
    return *this;
}

template <typename T>
bool EvenSubsetIterator<T>::operator!=(const EvenSubsetIterator<T>& other) const {
    return subsetMask != other.subsetMask;
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>> EvenSubsetIterator<T>::operator*() const {
    std::vector<T> subset;
    std::vector<T> remaining;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (subsetMask & (1 << i)) {
            subset.push_back(vec[i]);
        } else {
            remaining.push_back(vec[i]);
        }
    }
    return {subset, remaining};
}

template <typename T>
EvenSubsetIterator<T>& EvenSubsetIterator<T>::operator++() {
    advanceToNextEvenSubset();
    return *this;
}

// Explicit template instantiation
template class EvenSubsetIterator<int>;