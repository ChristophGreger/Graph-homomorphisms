#ifndef GRAPH_HOMOMORPHISMS_BITARRAY_H
#define GRAPH_HOMOMORPHISMS_BITARRAY_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "utilities.h"
#include "Graph.h"
#include "CFINode.h"

using namespace std;

class BitArray {
public:

    uint8_t *bits;
    int size;

    BitArray(const BitArray& old);
    BitArray(uint8_t* bits, int size);
    BitArray(int size);

    void set(int index);
    void toggle(int index);
    void clear(int index);
    int get(int index);
    char* toString();
    void free();
    bool hasEvenParity();
};


#endif //GRAPH_HOMOMORPHISMS_BITARRAY_H