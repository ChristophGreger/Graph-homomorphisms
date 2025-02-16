#ifndef GRAPH_HOMOMORPHISMS_BITARRAY_H
#define GRAPH_HOMOMORPHISMS_BITARRAY_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "utilities.h"
#include "Graph.h"

using namespace std;

class BitArray {
public:

    uint8_t *bits;
    int size;
    int sizeInBytes;

    BitArray(const BitArray& old);
    BitArray(int size);
    ~BitArray();

    void set(int index);
    void toggle(int index);
    void clear(int index);
    int get(int index);
    char* toString();
    void free();
    bool hasEvenParity();
    void reset();
};


#endif //GRAPH_HOMOMORPHISMS_BITARRAY_H