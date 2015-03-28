/**
 * AZTEEG_X3_PRO (Arduino Mega) pin assignments
 */

#include "pins_RAMPS_13.h"

#undef FAN_PIN
#define FAN_PIN             9 // (Sprinter config)

#define BEEPER             33

#define E2_STEP_PIN        23
#define E2_DIR_PIN         25
#define E2_ENABLE_PIN      40

#define E3_STEP_PIN        27
#define E3_DIR_PIN         29
#define E3_ENABLE_PIN      41

#define E4_STEP_PIN        43
#define E4_DIR_PIN         37
#define E4_ENABLE_PIN      42

#undef HEATER_1_PIN
#undef HEATER_2_PIN
#undef HEATER_3_PIN
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       16
#define HEATER_3_PIN       17
#define HEATER_4_PIN        4
#define HEATER_5_PIN        5
#define HEATER_6_PIN        6
#define HEATER_7_PIN       11

#undef TEMP_2_PIN
#undef TEMP_3_PIN
#define TEMP_2_PIN         12   // ANALOG NUMBERING
#define TEMP_3_PIN         11   // ANALOG NUMBERING
#define TEMP_4_PIN         10   // ANALOG NUMBERING
#define TC1                 4   // ANALOG NUMBERING Thermo couple on Azteeg X3Pro
#define TC2                 5   // ANALOG NUMBERING Thermo couple on Azteeg X3Pro
