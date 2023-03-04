#pragma once
#include <stdint.h>

#define AVAILABLE_IRQn_COUNT 10
#define FIRST_IRQn 10
#define LAST_IRQn (FIRST_IRQn + AVAILABLE_IRQn_COUNT)

#define CHANGE 2
#define FALLING 3
#define RISING 4

typedef void (*isr_handler_t)(void);

//
// Public API
//
static inline void interrupts()
{
    asm volatile("cpsie i");
}

static inline void noInterrupts()
{
    asm volatile("cpsid i");
}

/**
 * attach a external interrupt to the pin. max. 10 are available
 */
void attachInterrupt(uint32_t pin, isr_handler_t callback, uint32_t mode);
void detachInterrupt(uint32_t pin);

bool checkAndClearExtIRQFlag(uint32_t pin);
void interrupts_init();
