//
// Created by Christoph Greger on 19.01.25.
//

#include "NextInjection.h"
#include <stdexcept>  // for std::invalid_argument

NextInjection::NextInjection(int n, int m)
    : n(n), m(m), arr(n)
{
    // Basic validation
    if (n < 0 || m < 0) {
        throw std::invalid_argument("n and m must be non-negative");
    }

    // 1) Initialize arr with the first (smallest) injection: [0, 1, 2, ..., n-1]
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }

    // 2) Everything else in [n..m-1] is initially free
    for (int val = n; val < m; val++) {
        freeVals.insert(val);
    }
}

bool NextInjection::next() {
    // If there's no valid injection (e.g. n == 0 or n > m), we cannot advance.
    if (n == 0 || m < n) {
        return false;
    }

    // Attempt from the rightmost index to the leftmost
    for (int i = n - 1; i >= 0; --i) {
        int oldVal = arr[i];
        // Make that old value free again, because we might increment this slot
        freeVals.insert(oldVal);

        // Find the next bigger free value than oldVal
        auto it = freeVals.upper_bound(oldVal);
        if (it == freeVals.end()) {
            // No larger free value => we "roll over": keep oldVal in freeVals
            // and move left to position i-1.
        } else {
            // We can increment arr[i] to this bigger free value
            int newVal = *it;
            arr[i] = newVal;
            freeVals.erase(it); // it's now used

            // Reset the suffix arr[i+1..n-1] with the smallest free values
            for (int pos = i + 1; pos < n; pos++) {
                // take the smallest available from freeVals
                auto fit = freeVals.begin();
                arr[pos] = *fit;
                freeVals.erase(fit);
            }

            // We have successfully generated the next injection
            return true;
        }
    }

    // If we reach here, it means arr[0] couldn't be incremented => last injection
    return false;
}

const std::vector<int>& NextInjection::current() const {
    return arr;
}