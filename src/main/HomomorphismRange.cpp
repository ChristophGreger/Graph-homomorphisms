//
// Created by Christoph Greger on 26.12.24.
//

#include "HomomorphismRange.h"

//This whole thing is ai generated, and needs a lot of improvement, as its really slow

// ================== HomomorphismIterator ==================

HomomorphismIterator::HomomorphismIterator(int n, int m, long long index)
        : n_(n), m_(m), index_(index), currentMapping_(n)
{
    // Decode the index immediately
    decode();
}

HomomorphismIterator::value_type HomomorphismIterator::operator*() const {
    return currentMapping_;
}

HomomorphismIterator& HomomorphismIterator::operator++() {
    ++index_;
    decode();
    return *this;
}

bool HomomorphismIterator::operator==(const HomomorphismIterator& other) const {
    return index_ == other.index_;
}

bool HomomorphismIterator::operator!=(const HomomorphismIterator& other) const {
    return !(*this == other);
}

void HomomorphismIterator::decode() {
    // If index_ is out of range (e.g., < 0 or >= m^n),
    // this iterator is effectively an "end" iterator.
    if (index_ < 0) {
        return;
    }

    long long tmp = index_;
    for (int i = 0; i < n_; ++i) {
        // Each position i in the vector gets (tmp % m_)
        currentMapping_[i] = static_cast<int>(tmp % m_);
        tmp /= m_;
    }
}

// ================== HomomorphismRange ==================

HomomorphismRange::HomomorphismRange(int n, int m)
        : n_(n), m_(m), total_(1)
{
    // total_ = m^n
    // Watch out for overflow if n and m are large.
    for (int i = 0; i < n_; ++i) {
        total_ *= m_;
    }
}

HomomorphismIterator HomomorphismRange::begin() const {
    // The first mapping corresponds to index = 0
    return HomomorphismIterator(n_, m_, 0LL);
}

HomomorphismIterator HomomorphismRange::end() const {
    // The end iterator corresponds to index = m^n, i.e., out of the valid range
    return HomomorphismIterator(n_, m_, total_);
}