#ifndef NAUTY_WRAPPER_H

#define NAUTY_WRAPPER_H

// Vor dem Einbinden umbenennen, damit "set" in nauty.h nicht kollidiert
#define set nauty_set

extern "C" {
#include "nauty.h"
}

// Danach die Umdefinition wieder aufheben
#undef set

#endif // NAUTY_WRAPPER_H