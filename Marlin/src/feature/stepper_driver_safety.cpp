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

#if HAS_DRIVER_SAFE_POWER_PROTECT

#include "stepper_driver_safety.h"

static uint32_t axis_plug_backward = 0;

void stepper_driver_backward_error(PGM_P str) {
  SERIAL_ERROR_START();
  SERIAL_ECHOPGM_P(str);
  SERIAL_ECHOLNPGM(" driver is backward!");
  ui.status_printf_P(2, PSTR(S_FMT S_FMT), str, GET_TEXT(MSG_DRIVER_BACKWARD));
}

void stepper_driver_backward_check() {

  OUT_WRITE(SAFE_POWER_PIN, LOW);

  #define TEST_BACKWARD(AXIS, BIT) do { \
      SET_INPUT(AXIS##_ENABLE_PIN); \
      OUT_WRITE(AXIS##_STEP_PIN, false); \
      delay(20); \
      if (READ(AXIS##_ENABLE_PIN) == false) { \
        SBI(axis_plug_backward, BIT); \
        stepper_driver_backward_error(PSTR(STRINGIFY(AXIS))); \
      } \
    }while(0)

  #if HAS_X_ENABLE
    TEST_BACKWARD(X, 0);
  #endif
  #if HAS_X2_ENABLE
    TEST_BACKWARD(X2, 1);
  #endif

  #if HAS_Y_ENABLE
    TEST_BACKWARD(Y, 2);
  #endif
  #if HAS_Y2_ENABLE
    TEST_BACKWARD(Y2, 3);
  #endif

  #if HAS_Z_ENABLE
    TEST_BACKWARD(Z, 4);
  #endif
  #if HAS_Z2_ENABLE
    TEST_BACKWARD(Z2, 5);
  #endif
  #if HAS_Z3_ENABLE
    TEST_BACKWARD(Z3, 6);
  #endif
  #if HAS_Z4_ENABLE
    TEST_BACKWARD(Z4, 7);
  #endif

  #if HAS_E0_ENABLE
    TEST_BACKWARD(E0, 8);
  #endif
  #if HAS_E1_ENABLE
    TEST_BACKWARD(E1, 9);
  #endif
  #if HAS_E2_ENABLE
    TEST_BACKWARD(E2, 10);
  #endif
  #if HAS_E3_ENABLE
    TEST_BACKWARD(E3, 11);
  #endif
  #if HAS_E4_ENABLE
    TEST_BACKWARD(E4, 12);
  #endif
  #if HAS_E5_ENABLE
    TEST_BACKWARD(E5, 13);
  #endif
  #if HAS_E6_ENABLE
    TEST_BACKWARD(E6, 14);
  #endif
  #if HAS_E7_ENABLE
    TEST_BACKWARD(E7, 15);
  #endif

  if (!axis_plug_backward)
    WRITE(SAFE_POWER_PIN, HIGH);
}

void stepper_driver_backward_report() {
  if (!axis_plug_backward) return;

  auto _report_if_backward = [](PGM_P axis, uint8_t bit) {
    if (TEST(axis_plug_backward, bit))
      stepper_driver_backward_error(axis);
  };

  #define REPORT_BACKWARD(axis, bit) _report_if_backward(PSTR(STRINGIFY(axis)), bit)

  #if HAS_X_ENABLE
    REPORT_BACKWARD(X, 0);
  #endif
  #if HAS_X2_ENABLE
    REPORT_BACKWARD(X2, 1);
  #endif

  #if HAS_Y_ENABLE
    REPORT_BACKWARD(Y, 2);
  #endif
  #if HAS_Y2_ENABLE
    REPORT_BACKWARD(Y2, 3);
  #endif

  #if HAS_Z_ENABLE
    REPORT_BACKWARD(Z, 4);
  #endif
  #if HAS_Z2_ENABLE
    REPORT_BACKWARD(Z2, 5);
  #endif
  #if HAS_Z3_ENABLE
    REPORT_BACKWARD(Z3, 6);
  #endif
  #if HAS_Z4_ENABLE
    REPORT_BACKWARD(Z4, 7);
  #endif

  #if HAS_E0_ENABLE
    REPORT_BACKWARD(E0, 8);
  #endif
  #if HAS_E1_ENABLE
    REPORT_BACKWARD(E1, 9);
  #endif
  #if HAS_E2_ENABLE
    REPORT_BACKWARD(E2, 10);
  #endif
  #if HAS_E3_ENABLE
    REPORT_BACKWARD(E3, 11);
  #endif
  #if HAS_E4_ENABLE
    REPORT_BACKWARD(E4, 12);
  #endif
  #if HAS_E5_ENABLE
    REPORT_BACKWARD(E5, 13);
  #endif
  #if HAS_E6_ENABLE
    REPORT_BACKWARD(E6, 14);
  #endif
  #if HAS_E7_ENABLE
    REPORT_BACKWARD(E7, 15);
  #endif
}

#endif // HAS_DRIVER_SAFE_POWER_PROTECT
