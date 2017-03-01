/**
 * pins.h
 */

#ifndef PINS_H
#define PINS_H

#include "boards.h"

// Preset optional pins
#define X_MS1_PIN -1
#define X_MS2_PIN -1
#define Y_MS1_PIN -1
#define Y_MS2_PIN -1
#define Z_MS1_PIN -1
#define Z_MS2_PIN -1
#define E0_MS1_PIN -1
#define E0_MS2_PIN -1
#define E1_MS1_PIN -1
#define E1_MS2_PIN -1

#define DIGIPOTSS_PIN -1

#define X_MIN_PIN -1
#define X_MAX_PIN -1
#define Y_MIN_PIN -1
#define Y_MAX_PIN -1
#define Z_MIN_PIN -1
#define Z_MAX_PIN -1

#define HEATER_3_PIN -1
#define TEMP_3_PIN -1
#define HEATER_BED_PIN -1
#define TEMP_BED_PIN -1


#define PS_ON_PIN -1

#if MB(GEN7_CUSTOM)
  #include "pins/pins_GEN7_CUSTOM.h"
#elif MB(GEN7_12)
  #include "pins/pins_GEN7_12.h"
#elif MB(GEN7_13)
  #include "pins/pins_GEN7_13.h"
#elif MB(GEN7_14)
  #include "pins/pins_GEN7_14.h"
#elif MB(CHEAPTRONIC)
  #include "pins/pins_CHEAPTRONIC.h"
#elif MB(SETHI)
  #include "pins/pins_SETHI.h"
#elif MB(RAMPS_OLD)
  #include "pins/pins_RAMPS_OLD.h"
#elif MB(RAMPS_13_EFB) || MB(RAMPS_13_EEB) || MB(RAMPS_13_EFF) || MB(RAMPS_13_EEF)
  #include "pins/pins_RAMPS_13.h"
#elif MB(DUEMILANOVE_328P)
  #include "pins/pins_DUEMILANOVE_328P.h"
#elif MB(GEN6)
  #include "pins/pins_GEN6.h"
#elif MB(GEN6_DELUXE)
  #include "pins/pins_GEN6_DELUXE.h"
#elif MB(SANGUINOLOLU_11)
  #include "pins/pins_SANGUINOLOLU_11.h"
#elif MB(SANGUINOLOLU_12)
  #include "pins/pins_SANGUINOLOLU_12.h"
#elif MB(MELZI)
  #include "pins/pins_MELZI.h"
#elif MB(STB_11)
  #include "pins/pins_STB_11.h"
#elif MB(AZTEEG_X1)
  #include "pins/pins_AZTEEG_X1.h"
#elif MB(MELZI_1284)
  #include "pins/pins_MELZI_1284.h"
#elif MB(AZTEEG_X3)
  #include "pins/pins_AZTEEG_X3.h"
#elif MB(AZTEEG_X3_PRO)
  #include "pins/pins_AZTEEG_X3_PRO.h"
#elif MB(ULTIMAKER)
  #include "pins/pins_ULTIMAKER.h"
#elif MB(ULTIMAKER_OLD)
  #include "pins/pins_ULTIMAKER_OLD.h"
#elif MB(ULTIMAIN_2)
  #include "pins/pins_ULTIMAIN_2.h"
#elif MB(3DRAG)
  #include "pins/pins_3DRAG.h"
#elif MB(K8200)
  #include "pins/pins_K8200.h"
#elif MB(TEENSYLU)
  #include "pins/pins_TEENSYLU.h"
#elif MB(RUMBA)
  #include "pins/pins_RUMBA.h"
#elif MB(PRINTRBOARD)
  #include "pins/pins_PRINTRBOARD.h"
#elif MB(BRAINWAVE)
  #include "pins/pins_BRAINWAVE.h"
#elif MB(SAV_MKI)
  #include "pins/pins_SAV_MKI.h"
#elif MB(TEENSY2)
  #include "pins/pins_TEENSY2.h"
#elif MB(GEN3_PLUS)
  #include "pins/pins_GEN3_PLUS.h"
#elif MB(GEN3_MONOLITHIC)
  #include "pins/pins_GEN3_MONOLITHIC.h"
#elif MB(MEGATRONICS)
  #include "pins/pins_MEGATRONICS.h"
#elif MB(MEGATRONICS_1)
  #include "pins/pins_MEGATRONICS_1.h"
#elif MB(MEGATRONICS_2)
  #include "pins/pins_MEGATRONICS_2.h"
#elif MB(MEGATRONICS_3)
  #include "pins/pins_MEGATRONICS_3.h"
#elif MB(OMCA_A)
  #include "pins/pins_OMCA_A.h"
#elif MB(OMCA)
  #include "pins/pins_OMCA.h"
#elif MB(RAMBO)
  #include "pins/pins_RAMBO.h"
#elif MB(ELEFU_3)
  #include "pins/pins_ELEFU_3.h"
#elif MB(5DPRINT)
  #include "pins/pins_5DPRINT.h"
#elif MB(LEAPFROG)
  #include "pins/pins_LEAPFROG.h"
#elif MB(BQ_CNC)
  #include "pins/pins_BQ_CNC.h"
#elif MB(BQ_ZUM_MEGA_3D) || MB(BQ_HPH_ZUM_3D)
  #include "pins/pins_BQ_ZUM_MEGA_3D.h"
#elif MB(HEPHESTOS)
  #include "pins/pins_HEPHESTOS.h"
#elif MB(BAM_DICE)
  #include "pins/pins_RAMPS_13.h"
#elif MB(BAM_DICE_DUE)
  #include "pins/pins_BAM_DICE_DUE.h"
#elif MB(99)
  #include "pins/pins_99.h"
#else
  #error Unknown MOTHERBOARD value set in Configuration.h
#endif

// List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN, analogInputToDigitalPin(TEMP_0_PIN),
#define _E1_PINS
#define _E2_PINS
#define _E3_PINS

#if EXTRUDERS > 1
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN, analogInputToDigitalPin(TEMP_1_PIN),
  #if EXTRUDERS > 2
    #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN, analogInputToDigitalPin(TEMP_2_PIN),
    #if EXTRUDERS > 3
      #define _E3_PINS E3_STEP_PIN, E3_DIR_PIN, E3_ENABLE_PIN, HEATER_3_PIN, analogInputToDigitalPin(TEMP_3_PIN),
    #endif
  #endif
#endif

#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN X_STOP_PIN
    #define X_MAX_PIN -1
  #else
    #define X_MIN_PIN -1
    #define X_MAX_PIN X_STOP_PIN
  #endif
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN Y_STOP_PIN
    #define Y_MAX_PIN -1
  #else
    #define Y_MIN_PIN -1
    #define Y_MAX_PIN Y_STOP_PIN
  #endif
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN Z_STOP_PIN
    #define Z_MAX_PIN -1
  #else
    #define Z_MIN_PIN -1
    #define Z_MAX_PIN Z_STOP_PIN
  #endif
#endif

#ifdef DISABLE_MAX_ENDSTOPS
  #define X_MAX_PIN          -1
  #define Y_MAX_PIN          -1
  #define Z_MAX_PIN          -1
#endif

#ifdef DISABLE_MIN_ENDSTOPS
  #define X_MIN_PIN          -1
  #define Y_MIN_PIN          -1
  #define Z_MIN_PIN          -1
#endif

#define SENSITIVE_PINS { 0, 1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, PS_ON_PIN, \
                        HEATER_BED_PIN, FAN_PIN, \
                        _E0_PINS _E1_PINS _E2_PINS _E3_PINS \
                        analogInputToDigitalPin(TEMP_BED_PIN) \
                       }

#endif //__PINS_H
