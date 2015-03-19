/**
 * FELIXprinters v2.0/3.0 (RAMPS v1.3) pin assignments
 */

#include "pins_RAMPS_13.h"

#define X_MAX_PIN          -1
#define Y_MAX_PIN          -1
#define Z_MAX_PIN          -1

#define Y2_STEP_PIN        -1
#define Y2_DIR_PIN         -1
#define Y2_ENABLE_PIN      -1
#define Z2_STEP_PIN        -1
#define Z2_DIR_PIN         -1
#define Z2_ENABLE_PIN      -1

#define E1_STEP_PIN        36 // FELIX E1 uses RAMPS Y2/Z2 pins
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define SDPOWER             1

#define FAN_PIN             9 // (Sprinter config)
#define PS_ON_PIN          12

#define HEATER_1_PIN        7 // EXTRUDER 2

#if defined(ULTRA_LCD) && defined(NEWPANEL)

  #define BLEN_C 2
  #define BLEN_B 1
  #define BLEN_A 0
  #define SDCARDDETECT 6

#endif // NEWPANEL && ULTRA_LCD
