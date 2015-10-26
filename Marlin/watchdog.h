#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "Marlin.h"
#include <avr/wdt.h>

// Initialize watchdog with a 4 second interrupt time
void watchdog_init();

// Reset watchdog. MUST be called at least every 4 seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset() { wdt_reset(); }

#endif
