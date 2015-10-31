/**
 * pins.h
 */

#ifndef PINS_H
#define PINS_H

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
#define HEATER_3_PIN -1
#define TEMP_3_PIN -1

#if MB(GEN7_CUSTOM)
  #include "pins_GEN7_CUSTOM.h"
#elif MB(GEN7_12)
  #include "pins_GEN7_12.h"
#elif MB(GEN7_13)
  #include "pins_GEN7_13.h"
#elif MB(GEN7_14)
  #include "pins_GEN7_14.h"
#elif MB(CHEAPTRONIC)
  #include "pins_CHEAPTRONIC.h"
#elif MB(SETHI)
  #include "pins_SETHI.h"
#elif MB(RAMPS_OLD)
  #include "pins_RAMPS_OLD.h"
#elif MB(RAMPS_13_EFB)
  #include "pins_RAMPS_13_EFB.h"
#elif MB(RAMPS_13_EEB) || MB(RAMPS_13_EFF) || MB(RAMPS_13_EEF) || MB(RAMPS_13_SF)
  #include "pins_RAMPS_13.h"
#elif MB(RAMPS_14_EFB)
  #define IS_RAMPS_14
  #include "pins_RAMPS_13_EFB.h"
#elif MB(RAMPS_14_EEB) || MB(RAMPS_14_EFF) || MB(RAMPS_14_EEF) || MB(RAMPS_14_SF)
  #define IS_RAMPS_14
  #include "pins_RAMPS_13.h"
#elif MB(GEN6)
  #include "pins_GEN6.h"
#elif MB(GEN6_DELUXE)
  #include "pins_GEN6_DELUXE.h"
#elif MB(SANGUINOLOLU_11)
  #include "pins_SANGUINOLOLU_11.h"
#elif MB(SANGUINOLOLU_12)
  #include "pins_SANGUINOLOLU_12.h"
#elif MB(MELZI)
  #include "pins_MELZI.h"
#elif MB(STB_11)
  #include "pins_STB_11.h"
#elif MB(AZTEEG_X1)
  #include "pins_AZTEEG_X1.h"
#elif MB(MELZI_MAKR3D)
  #include "pins_MELZI_MAKR3D.h"
#elif MB(AZTEEG_X3)
  #include "pins_AZTEEG_X3.h"
#elif MB(AZTEEG_X3_PRO)
  #include "pins_AZTEEG_X3_PRO.h"
#elif MB(ULTIMAKER)
  #include "pins_ULTIMAKER.h"
#elif MB(ULTIMAKER_OLD)
  #include "pins_ULTIMAKER_OLD.h"
#elif MB(ULTIMAIN_2)
  #include "pins_ULTIMAIN_2.h"
#elif MB(3DRAG)
  #include "pins_3DRAG.h"
#elif MB(K8200)
  #include "pins_K8200.h"
#elif MB(TEENSYLU)
  #include "pins_TEENSYLU.h"
#elif MB(RUMBA)
  #include "pins_RUMBA.h"
#elif MB(PRINTRBOARD)
  #include "pins_PRINTRBOARD.h"
#elif MB(BRAINWAVE)
  #include "pins_BRAINWAVE.h"
#elif MB(BRAINWAVE_PRO)
  #include "pins_BRAINWAVE_PRO.h"
#elif MB(SAV_MKI)
  #include "pins_SAV_MKI.h"
#elif MB(TEENSY2)
  #include "pins_TEENSY2.h"
#elif MB(GEN3_PLUS)
  #include "pins_GEN3_PLUS.h"
#elif MB(GEN3_MONOLITHIC)
  #include "pins_GEN3_MONOLITHIC.h"
#elif MB(MEGATRONICS)
  #include "pins_MEGATRONICS.h"
#elif MB(MINITRONICS)
  #include "pins_MINITRONICS.h"
#elif MB(MEGATRONICS_2)
  #include "pins_MEGATRONICS_2.h"
#elif MB(MEGATRONICS_3)
  #include "pins_MEGATRONICS_3.h"
#elif MB(OMCA_A)
  #include "pins_OMCA_A.h"
#elif MB(OMCA)
  #include "pins_OMCA.h"
#elif MB(RAMBO)
  #include "pins_RAMBO.h"
#elif MB(MINIRAMBO)
  #include "pins_MINIRAMBO.h"
#elif MB(ELEFU_3)
  #include "pins_ELEFU_3.h"
#elif MB(5DPRINT)
  #include "pins_5DPRINT.h"
#elif MB(LEAPFROG)
  #include "pins_LEAPFROG.h"
#elif MB(BAM_DICE)
  #include "pins_RAMPS_13.h"
#elif MB(BAM_DICE_DUE)
  #include "pins_BAM_DICE_DUE.h"
#elif MB(FELIX2)
  #include "pins_FELIX2.h"
#elif MB(MKS_BASE)
  #include "pins_MKS_BASE.h"
#elif MB(RIGIDBOARD)
  #include "pins_RIGIDBOARD.h"
#elif MB(MEGACONTROLLER)
  #include "pins_MEGACONTROLLER.h"
#elif MB(99)
  #include "pins_99.h"
#else
  #error Unknown MOTHERBOARD value set in Configuration.h
#endif

// List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
#define _E0_PINS E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, HEATER_0_PIN, analogInputToDigitalPin(TEMP_0_PIN),
#define _E1_PINS
#define _E2_PINS
#define _E3_PINS

#if EXTRUDERS > 1
  #undef _E1_PINS
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN, HEATER_1_PIN, analogInputToDigitalPin(TEMP_1_PIN),
  #if EXTRUDERS > 2
    #undef _E2_PINS
    #define _E2_PINS E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN, HEATER_2_PIN, analogInputToDigitalPin(TEMP_2_PIN),
    #if EXTRUDERS > 3
      #undef _E3_PINS
      #define _E3_PINS E3_STEP_PIN, E3_DIR_PIN, E3_ENABLE_PIN, HEATER_3_PIN, analogInputToDigitalPin(TEMP_3_PIN),
    #endif
  #endif
#elif ENABLED(Y_DUAL_STEPPER_DRIVERS) || ENABLED(Z_DUAL_STEPPER_DRIVERS)
  #undef _E1_PINS
  #define _E1_PINS E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN,
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

#if ENABLED(DISABLE_MAX_ENDSTOPS)
  #undef X_MAX_PIN
  #undef Y_MAX_PIN
  #undef Z_MAX_PIN
  #define X_MAX_PIN          -1
  #define Y_MAX_PIN          -1
  #define Z_MAX_PIN          -1
#endif

#if ENABLED(DISABLE_MIN_ENDSTOPS)
  #undef X_MIN_PIN
  #undef Y_MIN_PIN
  #undef Z_MIN_PIN
  #define X_MIN_PIN          -1
  #define Y_MIN_PIN          -1
  #define Z_MIN_PIN          -1
#endif

#if ENABLED(DISABLE_Z_MIN_PROBE_ENDSTOP) || DISABLED(Z_MIN_PROBE_ENDSTOP) // Allow code to compile regardless of Z_MIN_PROBE_ENDSTOP setting.
  #undef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN    -1
#endif

#if ENABLED(DISABLE_XMAX_ENDSTOP)
  #undef X_MAX_PIN
  #define X_MAX_PIN          -1
#endif

#if ENABLED(DISABLE_XMIN_ENDSTOP)
  #undef X_MIN_PIN
  #define X_MIN_PIN          -1
#endif

#if ENABLED(DISABLE_YMAX_ENDSTOP)
  #define Y_MAX_PIN          -1
#endif

#if ENABLED(DISABLE_YMIN_ENDSTOP)
  #undef Y_MIN_PIN
  #define Y_MIN_PIN          -1
#endif

#if ENABLED(DISABLE_ZMAX_ENDSTOP)
  #undef Z_MAX_PIN
  #define Z_MAX_PIN          -1
#endif

#if ENABLED(DISABLE_ZMIN_ENDSTOP)
  #undef Z_MIN_PIN
  #define Z_MIN_PIN          -1
#endif

#ifndef Y2_STEP_PIN
  #define Y2_STEP_PIN      E1_STEP_PIN
  #define Y2_DIR_PIN       E1_DIR_PIN
  #define Y2_ENABLE_PIN    E1_ENABLE_PIN
#endif

#ifndef Z2_STEP_PIN
  #define Z2_STEP_PIN      E1_STEP_PIN
  #define Z2_DIR_PIN       E1_DIR_PIN
  #define Z2_ENABLE_PIN    E1_ENABLE_PIN
#endif

#define SENSITIVE_PINS { 0, 1, \
    X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, \
    Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, \
    Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, Z_MIN_PROBE_PIN, \
    PS_ON_PIN, HEATER_BED_PIN, FAN_PIN, \
    _E0_PINS _E1_PINS _E2_PINS _E3_PINS \
    analogInputToDigitalPin(TEMP_BED_PIN) \
  }

#define HAS_DIGIPOTSS (DIGIPOTSS_PIN >= 0)

#endif //__PINS_H

