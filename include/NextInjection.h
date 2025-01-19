//
// Created by Christoph Greger on 19.01.25.
//

#ifndef NEXTINJECTION_H
#define NEXTINJECTION_H

#pragma once

#include <vector>
#include <set>

/**
 * Class to iterate over all injective mappings (0..n-1) -> (0..m-1)
 * in strictly lexicographical order.
 */
class NextInjection {
public:
    /**
     * Constructor.
     * Creates the "smallest" injection if n <= m:
     *   arr = [0, 1, 2, ..., n-1]
     * and puts the remaining values [n..m-1] into freeVals.
     *
     * If n > m, then no valid injection is possible;
     * the 'next()' function will return false immediately.
     */
    NextInjection(int n, int m);

    /**
     * Moves the current injection to the next one in lexicographic order.
     * @return true if a next injection was found, false if we were at the last injection.
     */
    bool next();

    /**
     * @return const reference to the current injection (size n).
     */
    const std::vector<int>& current() const;

private:
    int n;                 // number of elements to map (domain size)
    int m;                 // range size [0..m-1]
    std::vector<int> arr;  // current injective mapping, arr[i] is the image of i
    std::set<int> freeVals;// set of unused values in [0..m-1]
};

#endif //NEXTINJECTION_H
