/**
 * RIGIDBOARD Arduino Mega with RAMPS v1.3 pin assignments
 */

#include "pins_RAMPS_13.h"

#if ENABLED(Z_MIN_PROBE_ENDSTOP)
  #define Z_MIN_PROBE_PIN  19
#endif

#undef HEATER_0_PIN
#undef HEATER_1_PIN
#define HEATER_0_PIN        9    // EXTRUDER 1
#define HEATER_1_PIN       12    // EXTRUDER 2 (FAN On Sprinter)

#undef TEMP_0_PIN
#undef TEMP_1_PIN
#define TEMP_0_PIN         14   // ANALOG NUMBERING
#define TEMP_1_PIN         13   // ANALOG NUMBERING

// LCD Panel options for the RigidBoard

#if ENABLED(RIGIDBOT_PANEL)

  #undef BEEPER_PIN
  #define BEEPER_PIN -1

  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN 22

  // Extra button definitions, substitute for EN1 / EN2
  #define BTN_UP  37 // BTN_EN1
  #define BTN_DWN 35 // BTN_EN2
  #define BTN_LFT 33
  #define BTN_RT  32

  // Marlin can respond to UP/DOWN by default
  // #undef  BTN_EN1
  // #undef  BTN_EN2
  //#define BTN_EN1 -1
  //#define BTN_EN2 -1

  #undef  BTN_ENC
  #define BTN_ENC 31

  #undef  SD_DETECT_PIN
  #define SD_DETECT_PIN 22

#elif ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

  #undef BEEPER_PIN
  #define BEEPER_PIN -1

  #undef  SD_DETECT_PIN
  #define SD_DETECT_PIN 22

  #undef  KILL_PIN
  #define KILL_PIN 32

#endif

// SPI for Max6675 Thermocouple
#undef MAX6675_SS
#if ENABLED(SDSUPPORT)
  #define MAX6675_SS       49 // Don't use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#else
  #define MAX6675_SS       53 // Don't use pin 53 if there is even the remote possibility of using Display/SD card
#endif

// RigidBot swaps E0 / E1 plugs vs RAMPS 1.3

#undef E0_STEP_PIN
#undef E0_DIR_PIN
#undef E0_ENABLE_PIN
#define E0_STEP_PIN        36
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      30

#undef E1_STEP_PIN
#undef E1_DIR_PIN
#undef E1_ENABLE_PIN
#define E1_STEP_PIN        26
#define E1_DIR_PIN         28
#define E1_ENABLE_PIN      24

#undef  FAN_PIN
#define FAN_PIN             8 // Same as RAMPS_13_EEF

#undef  PS_ON_PIN
#define PS_ON_PIN          -1

#undef  HEATER_BED_PIN
#define HEATER_BED_PIN     10

#undef  TEMP_BED_PIN
#define TEMP_BED_PIN       15   // ANALOG NUMBERING

#define STEPPER_RESET_PIN  41   // Stepper drivers have a reset on RigidBot
