/**
 * FELIXprinters v2.0/3.0 (RAMPS v1.3) pin assignments
 */

#define IS_RAMPS_EFB

#include "pins_RAMPS_13.h"

#undef HEATER_1_PIN
#define HEATER_1_PIN        7 // EXTRUDER 2

#define DISABLE_MAX_ENDSTOPS

#undef SDPOWER
#define SDPOWER             1

#define PS_ON_PIN          12

#if defined(ULTRA_LCD) && defined(NEWPANEL)

  #define BLEN_C 2
  #define BLEN_B 1
  #define BLEN_A 0
  #define SDCARDDETECT 6

#endif // NEWPANEL && ULTRA_LCD
