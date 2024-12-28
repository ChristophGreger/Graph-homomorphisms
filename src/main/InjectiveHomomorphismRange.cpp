//
// Created by Christoph Greger on 27.12.24.
//

#include "InjectiveHomomorphismRange.h"
#include <algorithm>

//This whole thing is ai generated and really slow

// ================== InjectiveHomomorphismIterator ==================

InjectiveHomomorphismIterator::InjectiveHomomorphismIterator(int n, int m, long long index)
        : n_(n), m_(m), index_(index), currentMapping_(n)
{
    // Initialize currentMapping_ with the first n elements of the codomain
    for (int i = 0; i < n_; ++i) {
        currentMapping_[i] = i;
    }
    // Decode the index immediately
    decode();
}

InjectiveHomomorphismIterator::value_type InjectiveHomomorphismIterator::operator*() const {
    return currentMapping_;
}

InjectiveHomomorphismIterator& InjectiveHomomorphismIterator::operator++() {
    ++index_;
    decode();
    return *this;
}

bool InjectiveHomomorphismIterator::operator==(const InjectiveHomomorphismIterator& other) const {
    return index_ == other.index_;
}

bool InjectiveHomomorphismIterator::operator!=(const InjectiveHomomorphismIterator& other) const {
    return !(*this == other);
}

void InjectiveHomomorphismIterator::decode() {
    // If index_ is out of range (e.g., < 0 or >= P(m, n)),
    // this iterator is effectively an "end" iterator.
    if (index_ < 0) {
        return;
    }

    // Generate the permutation corresponding to index_
    std::vector<int> temp(m_);
    for (int i = 0; i < m_; ++i) {
        temp[i] = i;
    }

    long long tmp = index_;
    for (int i = 0; i < n_; ++i) {
        int j = tmp % (m_ - i);
        std::swap(temp[i], temp[i + j]);
        tmp /= (m_ - i);
    }

    currentMapping_.assign(temp.begin(), temp.begin() + n_);
}

// ================== InjectiveHomomorphismRange ==================

InjectiveHomomorphismRange::InjectiveHomomorphismRange(int n, int m)
        : n_(n), m_(m), total_(1)
{
    // total_ = P(m, n) = m! / (m - n)!
    for (int i = 0; i < n_; ++i) {
        total_ *= (m_ - i);
    }
}

InjectiveHomomorphismIterator InjectiveHomomorphismRange::begin() const {
    // The first mapping corresponds to index = 0
    return InjectiveHomomorphismIterator(n_, m_, 0LL);
}

InjectiveHomomorphismIterator InjectiveHomomorphismRange::end() const {
    // The end iterator corresponds to index = P(m, n), i.e., out of the valid range
    return InjectiveHomomorphismIterator(n_, m_, total_);
}