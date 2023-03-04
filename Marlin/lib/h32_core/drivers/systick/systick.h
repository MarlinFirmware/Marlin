#pragma once
#include <stdint.h>

#define TICKS_PER_SECOND 1000ul

void systick_init();
uint32_t millis();
uint32_t micros();
