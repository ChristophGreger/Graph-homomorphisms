#include "BitArray.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>

BitArray::BitArray(uint8_t* bits, int size) {
    this->bits = bits;
    this->size = size;
}

BitArray::BitArray(int size) {
    int sizeInBytes = ((int) size / 8) + 1;
    bits = (uint8_t*) calloc(sizeInBytes, sizeof(uint8_t));
    this->size = size;
}

void BitArray::set(int index) {
    bits[(index) / 8] |= (1U << ((index) % 8));
}

void BitArray::clear(int index) {
    bits[(index) / 8] &= ~(1U << ((index) % 8));
}

void BitArray::toggle(int index) {
    bits[(index) / 8] ^= (1U << ((index) % 8));
}

int BitArray::get(int index) {
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