#ifndef NAUTY_WRAPPER_H

#define NAUTY_WRAPPER_H

// Rename "set" to avoid conflicts with nauty.h
#define set nauty_set

extern "C" {
#include "nauty.h"
}

// Undefine the "set" macro to avoid conflicts
#undef set

#endif // NAUTY_WRAPPER_H