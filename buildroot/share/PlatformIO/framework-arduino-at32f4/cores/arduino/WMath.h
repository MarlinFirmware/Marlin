#ifndef __WMATH_H
#define __WMATH_H

extern "C" {
#include "stdlib.h"
}

void randomSeed(unsigned long seed);
long random(long howbig);
long random(long howsmall, long howbig);

#endif
