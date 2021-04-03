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
#include "../inc/MarlinConfig.h"
#include "../lcd/marlinui.h"

#if DRIVER_SAFE_POWER_PROTECT

#include "driver_anti_reverse_protection.h"

#define MSG_ERROR_ANTI(axis) #axis"-Axis anti pluged"

#define ANTI_PLUG_DETECT(axis, bit)   do {\
                                    SET_INPUT(axis##_ENABLE_PIN);\
                                    OUT_WRITE(axis##_STEP_PIN, false);\
                                    delay(20);\
                                    if (READ(axis##_ENABLE_PIN) == false) {\
                                      SBI(axis_anti_pluged, bit);\
                                      SERIAL_ERROR_MSG(MSG_ERROR_ANTI(axis));\
                                    }\
                                  }while(0)

#define TEST_ANTI_PLUG(axis, bit)   if (TEST(axis_anti_pluged, bit)) {\
                                      SERIAL_ERROR_MSG(MSG_ERROR_ANTI(axis));\
                                      LCD_MESSAGEPGM_P(MSG_ERROR_ANTI(axis));\
                                    }


static uint32_t axis_anti_pluged = 0;

void stepper_driver_anti_plug_detect() {

  OUT_WRITE(SAFE_POWER_PIN, LOW);

  #if HAS_X_ENABLE
    ANTI_PLUG_DETECT(X, 0);
  #endif
  #if HAS_X2_ENABLE
    ANTI_PLUG_DETECT(X2, 1);
  #endif

  #if HAS_Y_ENABLE
    ANTI_PLUG_DETECT(Y, 2);
  #endif
  #if HAS_Y2_ENABLE
    ANTI_PLUG_DETECT(Y2, 3);
  #endif

  #if HAS_Z_ENABLE
    ANTI_PLUG_DETECT(Z, 4);
  #endif
  #if HAS_Z2_ENABLE
    ANTI_PLUG_DETECT(Z2, 5);
  #endif
  #if HAS_Z3_ENABLE
    ANTI_PLUG_DETECT(Z3, 6);
  #endif
  #if HAS_Z4_ENABLE
    ANTI_PLUG_DETECT(Z4, 7);
  #endif

  #if HAS_E0_ENABLE
    ANTI_PLUG_DETECT(E0, 8);
  #endif
  #if HAS_E1_ENABLE
    ANTI_PLUG_DETECT(E1, 9);
  #endif
  #if HAS_E2_ENABLE
    ANTI_PLUG_DETECT(E2, 10);
  #endif
  #if HAS_E3_ENABLE
    ANTI_PLUG_DETECT(E3, 11);
  #endif
  #if HAS_E4_ENABLE
    ANTI_PLUG_DETECT(E4, 12);
  #endif
  #if HAS_E5_ENABLE
    ANTI_PLUG_DETECT(E5, 13);
  #endif
  #if HAS_E6_ENABLE
    ANTI_PLUG_DETECT(E6, 14);
  #endif
  #if HAS_E7_ENABLE
    ANTI_PLUG_DETECT(E7, 15);
  #endif

  if (axis_anti_pluged == 0) {
    WRITE(SAFE_POWER_PIN, HIGH);
  }
}

void test_anti_plug() {
  if (axis_anti_pluged == 0) return;

  #if HAS_X_ENABLE
    TEST_ANTI_PLUG(X, 0);
  #endif
  #if HAS_X2_ENABLE
    TEST_ANTI_PLUG(X2, 1);
  #endif

  #if HAS_Y_ENABLE
    TEST_ANTI_PLUG(Y, 2);
  #endif
  #if HAS_Y2_ENABLE
    TEST_ANTI_PLUG(Y2, 3);
  #endif

  #if HAS_Z_ENABLE
    TEST_ANTI_PLUG(Z, 4);
  #endif
  #if HAS_Z2_ENABLE
    TEST_ANTI_PLUG(Z2, 5);
  #endif
  #if HAS_Z3_ENABLE
    TEST_ANTI_PLUG(Z3, 6);
  #endif
  #if HAS_Z4_ENABLE
    TEST_ANTI_PLUG(Z4, 7);
  #endif

  #if HAS_E0_ENABLE
    TEST_ANTI_PLUG(E0, 8);
  #endif
  #if HAS_E1_ENABLE
    TEST_ANTI_PLUG(E1, 9);
  #endif
  #if HAS_E2_ENABLE
    TEST_ANTI_PLUG(E2, 10);
  #endif
  #if HAS_E3_ENABLE
    TEST_ANTI_PLUG(E3, 11);
  #endif
  #if HAS_E4_ENABLE
    TEST_ANTI_PLUG(E4, 12);
  #endif
  #if HAS_E5_ENABLE
    TEST_ANTI_PLUG(E5, 13);
  #endif
  #if HAS_E6_ENABLE
    TEST_ANTI_PLUG(E6, 14);
  #endif
  #if HAS_E7_ENABLE
    TEST_ANTI_PLUG(E7, 15);
  #endif
}

#endif // DRIVER_SAFE_POWER_PROTECT
