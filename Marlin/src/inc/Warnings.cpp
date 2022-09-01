/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Warnings.cpp
 * Test configuration values and give warnings at compile-time.
 */

#include "MarlinConfig.h"

//
// Warnings! Located here so they will appear just once in the build output.
//

#if ENABLED(MARLIN_DEV_MODE)
  #warning "WARNING! Disable MARLIN_DEV_MODE for the final build!"
#endif

#if ENABLED(LA_DEBUG)
  #warning "WARNING! Disable LA_DEBUG for the final build!"
#endif

#if NUM_AXES_WARNING
  #warning "Note: NUM_AXES is now based on the *_DRIVER_TYPE settings so you can remove NUM_AXES from Configuration.h."
#endif

// Safety Features
#if DISABLED(USE_WATCHDOG)
  #warning "Safety Alert! Enable USE_WATCHDOG for the final build!"
#endif
#if HAS_HOTEND && DISABLED(THERMAL_PROTECTION_HOTENDS)
  #warning "Safety Alert! Enable THERMAL_PROTECTION_HOTENDS for the final build!"
#endif
#if HAS_HEATED_BED && DISABLED(THERMAL_PROTECTION_BED)
  #warning "Safety Alert! Enable THERMAL_PROTECTION_BED for the final build!"
#endif
#if HAS_HEATED_CHAMBER && DISABLED(THERMAL_PROTECTION_CHAMBER)
  #warning "Safety Alert! Enable THERMAL_PROTECTION_CHAMBER for the final build!"
#endif
#if HAS_COOLER && DISABLED(THERMAL_PROTECTION_COOLER)
  #warning "Safety Alert! Enable THERMAL_PROTECTION_COOLER for the final build!"
#endif
#if ANY_THERMISTOR_IS(998) || ANY_THERMISTOR_IS(999)
  #warning "Warning! Don't use dummy thermistors (998/999) for final build!"
#endif

#if NONE(HAS_RESUME_CONTINUE, HOST_PROMPT_SUPPORT)
  #warning "Your Configuration provides no method to acquire user feedback!"
#endif

#if MB(DUE3DOM_MINI) && PIN_EXISTS(TEMP_2) && !TEMP_SENSOR_BOARD
  #warning "Onboard temperature sensor for BOARD_DUE3DOM_MINI has moved from TEMP_SENSOR_2 (TEMP_2_PIN) to TEMP_SENSOR_BOARD (TEMP_BOARD_PIN)."
#elif MB(BTT_SKR_E3_TURBO) && PIN_EXISTS(TEMP_2) && !TEMP_SENSOR_BOARD
  #warning "Onboard temperature sensor for BOARD_BTT_SKR_E3_TURBO has moved from TEMP_SENSOR_2 (TEMP_2_PIN) to TEMP_SENSOR_BOARD (TEMP_BOARD_PIN)."
#endif

#ifndef NO_AUTO_ASSIGN_WARNING

  #if AUTO_ASSIGNED_LCD_SERIAL
    #warning "Note: Auto-assigned LCD_SERIAL_PORT. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif

  #if AUTO_ASSIGNED_X2_STEPPER
    #warning "Note: Auto-assigned X2 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_X2_MS1
    #warning "Note: Auto-assigned X2_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_X2_MS2
    #warning "Note: Auto-assigned X2_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_X2_MS3
    #warning "Note: Auto-assigned X2_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_X2_CS
    #warning "Note: Auto-assigned X2_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_X2_DIAG
    #if X2_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned X2_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned X2_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned X2_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif X2_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned X2_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_Y2_STEPPER
    #warning "Note: Auto-assigned Y2 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Y2_MS1
    #warning "Note: Auto-assigned Y2_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Y2_MS2
    #warning "Note: Auto-assigned Y2_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Y2_MS3
    #warning "Note: Auto-assigned Y2_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Y2_CS
    #warning "Note: Auto-assigned Y2_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Y2_DIAG
    #if Y2_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Y2_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned Y2_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_Z2_STEPPER
    #warning "Note: Auto-assigned Z2 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z2_MS1
    #warning "Note: Auto-assigned Z2_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z2_MS2
    #warning "Note: Auto-assigned Z2_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z2_MS3
    #warning "Note: Auto-assigned Z2_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z2_CS
    #warning "Note: Auto-assigned Z2_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z2_DIAG
    #if Z2_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z2_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned Z2_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_Z3_STEPPER
    #warning "Note: Auto-assigned Z3 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z3_CS
    #warning "Note: Auto-assigned Z3_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z3_MS1
    #warning "Note: Auto-assigned Z3_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z3_MS2
    #warning "Note: Auto-assigned Z3_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z3_MS3
    #warning "Note: Auto-assigned Z3_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z3_DIAG
    #if Z3_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z3_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned Z3_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_Z4_STEPPER
    #warning "Note: Auto-assigned Z4 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z4_CS
    #warning "Note: Auto-assigned Z4_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z4_MS1
    #warning "Note: Auto-assigned Z4_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z4_MS2
    #warning "Note: Auto-assigned Z4_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z4_MS3
    #warning "Note: Auto-assigned Z4_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_Z4_DIAG
    #if Z4_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif Z4_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned Z4_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_I_STEPPER
    #warning "Note: Auto-assigned I STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_I_CS
    #warning "Note: Auto-assigned I_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_I_MS1
    #warning "Note: Auto-assigned I_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_I_MS2
    #warning "Note: Auto-assigned I_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_I_MS3
    #warning "Note: Auto-assigned I_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_I_DIAG
    #if I_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned I_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned I_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned I_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned I_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned I_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned I_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned I_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned I_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned I_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif I_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned I_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_J_STEPPER
    #warning "Note: Auto-assigned J STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_J_CS
    #warning "Note: Auto-assigned J_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_J_MS1
    #warning "Note: Auto-assigned J_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_J_MS2
    #warning "Note: Auto-assigned J_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_J_MS3
    #warning "Note: Auto-assigned J_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_J_DIAG
    #if J_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned J_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned J_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned J_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned J_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned J_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned J_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned J_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned J_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned J_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif J_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned J_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_K_STEPPER
    #warning "Note: Auto-assigned K STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_K_CS
    #warning "Note: Auto-assigned K_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_K_MS1
    #warning "Note: Auto-assigned K_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_K_MS2
    #warning "Note: Auto-assigned K_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_K_MS3
    #warning "Note: Auto-assigned K_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_K_DIAG
    #if K_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned K_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned K_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _XSTOP_
      #warning "Note: Auto-assigned K_DIAG_PIN to X_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned K_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned K_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _YSTOP_
      #warning "Note: Auto-assigned K_DIAG_PIN to Y_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned K_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned K_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _ZSTOP_
      #warning "Note: Auto-assigned K_DIAG_PIN to Z_STOP_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned K_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif

  #if AUTO_ASSIGNED_U_STEPPER
    #warning "Note: Auto-assigned U STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_U_CS
    #warning "Note: Auto-assigned U_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_U_MS1
    #warning "Note: Auto-assigned U_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_U_MS2
    #warning "Note: Auto-assigned U_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_U_MS3
    #warning "Note: Auto-assigned U_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_U_DIAG
    #if U_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned U_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned U_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif K_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned U_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned U_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned U_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned U_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif U_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned U_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_V_STEPPER
    #warning "Note: Auto-assigned V STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_V_CS
    #warning "Note: Auto-assigned V_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_V_MS1
    #warning "Note: Auto-assigned V_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_V_MS2
    #warning "Note: Auto-assigned V_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_V_MS3
    #warning "Note: Auto-assigned V_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_V_DIAG
    #if V_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned V_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned V_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned V_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned V_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned V_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned V_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif V_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned V_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif
  #if AUTO_ASSIGNED_W_STEPPER
    #warning "Note: Auto-assigned W STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_W_CS
    #warning "Note: Auto-assigned W_CS_PIN to an unused En_CS_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_W_MS1
    #warning "Note: Auto-assigned W_MS1_PIN to an unused En_MS1_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_W_MS2
    #warning "Note: Auto-assigned W_MS2_PIN to an unused En_MS2_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_W_MS3
    #warning "Note: Auto-assigned W_MS3_PIN to an unused En_MS3_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif
  #if AUTO_ASSIGNED_W_DIAG
    #if W_USE_ENDSTOP == _XMIN_
      #warning "Note: Auto-assigned W_DIAG_PIN to X_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _XMAX_
      #warning "Note: Auto-assigned W_DIAG_PIN to X_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _YMIN_
      #warning "Note: Auto-assigned W_DIAG_PIN to Y_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _YMAX_
      #warning "Note: Auto-assigned W_DIAG_PIN to Y_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _ZMIN_
      #warning "Note: Auto-assigned W_DIAG_PIN to Z_MIN_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _ZMAX_
      #warning "Note: Auto-assigned W_DIAG_PIN to Z_MAX_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _XDIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to X_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _YDIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to Y_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _ZDIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to Z_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E0DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E0_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E1DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E1_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E2DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E2_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E3DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E3_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E4DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E4_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E5DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E5_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E6DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E6_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #elif W_USE_ENDSTOP == _E7DIAG_
      #warning "Note: Auto-assigned W_DIAG_PIN to E7_DIAG_PIN. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
    #endif
  #endif

  #if ENABLED(CHAMBER_FAN) && !defined(CHAMBER_FAN_INDEX)
    #warning "Note: Auto-assigned CHAMBER_FAN_INDEX to the first free FAN pin. (Define NO_AUTO_ASSIGN_WARNING to suppress this warning.)"
  #endif

#endif // !NO_AUTO_ASSIGN_WARNING

#if IS_LEGACY_TFT
  #warning "Don't forget to update your TFT settings in Configuration.h."
#endif

#if ENABLED(EMIT_CREALITY_422_WARNING)
  #warning "Creality 4.2.2 boards come with a variety of stepper drivers. Check the board label and set the correct *_DRIVER_TYPE! (C=HR4988, E=A4988, A=TMC2208, B=TMC2209, H=TMC2225). (Define EMIT_CREALITY_422_WARNING false to suppress this warning.)"
#endif

#if PRINTCOUNTER_SYNC
  #warning "To prevent step loss, motion will pause for PRINTCOUNTER auto-save."
#endif

#if HOMING_Z_WITH_PROBE && IS_CARTESIAN && DISABLED(Z_SAFE_HOMING)
  #error "Z_SAFE_HOMING is recommended when homing with a probe. Enable Z_SAFE_HOMING or comment out this line to continue."
#endif

//
// Warn users of potential endstop/DIAG pin conflicts to prevent homing issues when not using sensorless homing
//
#if !USE_SENSORLESS
  #if ENABLED(USES_DIAG_JUMPERS) && DISABLED(DIAG_JUMPERS_REMOVED)
    #warning "Motherboard DIAG jumpers must be removed when SENSORLESS_HOMING is disabled. (Define DIAG_JUMPERS_REMOVED to suppress this warning.)"
  #elif ENABLED(USES_DIAG_PINS) && DISABLED(DIAG_PINS_REMOVED)
    #warning "Driver DIAG pins must be physically removed unless SENSORLESS_HOMING is enabled. (See https://bit.ly/2ZPRlt0) (Define DIAG_PINS_REMOVED to suppress this warning.)"
  #endif
#endif

#if CANNOT_EMBED_CONFIGURATION
  #warning "Disabled CONFIGURATION_EMBEDDING because the target usually has less flash storage. Define FORCE_CONFIG_EMBED to override."
#endif

#if HAS_LCD_CONTRAST && LCD_CONTRAST_MIN >= LCD_CONTRAST_MAX
  #warning "Contrast cannot be changed when LCD_CONTRAST_MIN >= LCD_CONTRAST_MAX."
#endif

#if PROGRESS_MSG_EXPIRE > 0 && HAS_STATUS_MESSAGE_TIMEOUT
  #warning "It is recommended not to combine PROGRESS_MSG_EXPIRE with STATUS_MESSAGE_TIMEOUT_SEC."
#endif

/**
 * FYSETC/MKS/BTT Mini Panel backlighting
 */
#if EITHER(FYSETC_242_OLED_12864, FYSETC_MINI_12864_2_1) && !ALL(NEOPIXEL_LED, LED_CONTROL_MENU, LED_USER_PRESET_STARTUP, LED_COLOR_PRESETS)
  #warning "Your FYSETC/MKS/BTT Mini Panel works best with NEOPIXEL_LED, LED_CONTROL_MENU, LED_USER_PRESET_STARTUP, and LED_COLOR_PRESETS."
#endif

#if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0) && DISABLED(RGB_LED)
  #warning "Your FYSETC Mini Panel works best with RGB_LED."
#elif EITHER(FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1) && DISABLED(LED_USER_PRESET_STARTUP)
  #warning "Your FYSETC Mini Panel works best with LED_USER_PRESET_STARTUP."
#endif

#if EITHER(FYSETC_242_OLED_12864, FYSETC_MINI_12864) && BOTH(PSU_CONTROL, HAS_COLOR_LEDS) && !LED_POWEROFF_TIMEOUT
  #warning "Your FYSETC display with PSU_CONTROL works best with LED_POWEROFF_TIMEOUT."
#endif

/**
 * Maple environment
 */
#ifdef __STM32F1__
  #warning "Maple build environments are deprecated. Please use a non-Maple build environment. Report issues to the Marlin Firmware project."
#endif

/**
 * Průša MK3/S/+ fan pin reassignment
 */
#if MB(BTT_BTT002_V1_0, EINSY_RAMBO) && DISABLED(NO_MK3_FAN_PINS_WARNING)
  #warning "Define MK3_FAN_PINS to swap hotend and part cooling fan pins. (Define NO_MK3_FAN_PINS_WARNING to suppress this warning.)"
#endif

/**
 * BD Sensor should always include BABYSTEPPING
 */
#if ENABLED(BD_SENSOR) && DISABLED(BABYSTEPPING)
  #warning "BABYSTEPPING is recommended with BD_SENSOR."
#endif
