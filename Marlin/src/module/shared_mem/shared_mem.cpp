#include "shared_mem.h"

volatile uint8_t __attribute__ ((aligned (4))) shared_mem[SHARED_MEM_SIZE];

