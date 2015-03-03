/**
 * Melzi with ATmega1284 (MaKr3d version) pin assignments
 */

#define MELZI

#undef MOTHERBOARD
#define MOTHERBOARD BOARD_SANGUINOLOLU_11
#define SANGUINOLOLU_V_1_2

#if defined(__AVR_ATmega1284P__)
  #define LARGE_FLASH true
#endif

#include "pins_SANGUINOLOLU_11.h"
