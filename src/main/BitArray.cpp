#include "BitArray.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>

BitArray::BitArray(const BitArray& old) {
    size = old.size;
    sizeInBytes = old.sizeInBytes;
    bits = new uint8_t[sizeInBytes];
    std::copy(old.bits, old.bits + sizeInBytes, bits);
}

BitArray::BitArray(int size) {
    sizeInBytes = (size + 7) / 8;
    bits = new uint8_t[sizeInBytes]();
    this->size = size;
}

void BitArray::set(int index) {
    if(index >= size) {
        perror("index out of bounds");
        exit(EXIT_FAILURE);
        return;
    }
    bits[(index) / 8] |= (1U << ((index) % 8));
}

void BitArray::clear(int index) {
    if(index >= size) {
        perror("index out of bounds");
        exit(EXIT_FAILURE);
        return;
    }
    bits[(index) / 8] &= ~(1U << ((index) % 8));
}

void BitArray::toggle(int index) {
    if(index >= size) {
        perror("index out of bounds");
        exit(EXIT_FAILURE);
        return;
    }
    bits[(index) / 8] ^= (1U << ((index) % 8));
}

int BitArray::get(int index) {
    if(index >= size) {
        perror("index out of bounds");
        exit(EXIT_FAILURE);
        return 0;
    }
    return (bits[(index) / 8] >> ((index) % 8)) & 1;
}

char* BitArray::toString() {

    size_t strSize = size + (int)(size / 8) + 1; //bits + spaces + null terminator
    char *str = (char*) malloc(strSize);

    size_t index = 0;
    for (size_t i = 0; i < size; i++) {
        str[index] = get(i) ? '1' : '0';
        index++;
        if ((i + 1) % 8 == 0) {
            str[index] = ' '; //add space after every byte
            index++;
        }
    }
    str[index] = '\0';

    return str;
}

bool BitArray::hasEvenParity() {
    int count = 0;
    for (int i = 0; i < sizeInBytes; i++) {
        count ^= __builtin_popcount(bits[i]);  // XOR popcount of each byte
    }
    return (count % 2) == 0;  // True if even
}


BitArray::~BitArray() {
    delete[] bits;
    bits = nullptr;
}

void BitArray::reset() {
    int sizeInBytes = (size + 7) / 8;
    memset(bits, 0, sizeInBytes);  // Set all bits to 0 efficiently
}