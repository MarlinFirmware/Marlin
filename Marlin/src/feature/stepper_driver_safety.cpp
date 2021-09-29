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

  #define _TEST_BACKWARD(AXIS, BIT) do { \
      SET_INPUT(AXIS##_ENABLE_PIN); \
      OUT_WRITE(AXIS##_STEP_PIN, false); \
      delay(20); \
      if (READ(AXIS##_ENABLE_PIN) == false) { \
        SBI(axis_plug_backward, BIT); \
        stepper_driver_backward_error(PSTR(STRINGIFY(AXIS))); \
      } \
    }while(0)

  #define TEST_BACKWARD(AXIS, BIT) TERN_(HAS_##AXIS##_ENABLE, _TEST_BACKWARD(AXIS, BIT))

  TEST_BACKWARD(X,   0);
  TEST_BACKWARD(X2,  1);

  TEST_BACKWARD(Y,   2);
  TEST_BACKWARD(Y2,  3);

  TEST_BACKWARD(Z,   4);
  TEST_BACKWARD(Z2,  5);
  TEST_BACKWARD(Z3,  6);
  TEST_BACKWARD(Z4,  7);

  TEST_BACKWARD(I,   8);
  TEST_BACKWARD(J,   9);
  TEST_BACKWARD(K,  10);

  TEST_BACKWARD(E0, 11);
  TEST_BACKWARD(E1, 12);
  TEST_BACKWARD(E2, 13);
  TEST_BACKWARD(E3, 14);
  TEST_BACKWARD(E4, 15);
  TEST_BACKWARD(E5, 16);
  TEST_BACKWARD(E6, 17);
  TEST_BACKWARD(E7, 18);

  if (!axis_plug_backward)
    WRITE(SAFE_POWER_PIN, HIGH);
}

void stepper_driver_backward_report() {
  if (!axis_plug_backward) return;

  auto _report_if_backward = [](PGM_P axis, uint8_t bit) {
    if (TEST(axis_plug_backward, bit))
      stepper_driver_backward_error(axis);
  };

  #define REPORT_BACKWARD(axis, bit) TERN_(HAS_##axis##_ENABLE, _report_if_backward(PSTR(STRINGIFY(axis)), bit))

  REPORT_BACKWARD(X,   0);
  REPORT_BACKWARD(X2,  1);

  REPORT_BACKWARD(Y,   2);
  REPORT_BACKWARD(Y2,  3);

  REPORT_BACKWARD(Z,   4);
  REPORT_BACKWARD(Z2,  5);
  REPORT_BACKWARD(Z3,  6);
  REPORT_BACKWARD(Z4,  7);

  REPORT_BACKWARD(I,   8);
  REPORT_BACKWARD(J,   9);
  REPORT_BACKWARD(K,  10);

  REPORT_BACKWARD(E0, 11);
  REPORT_BACKWARD(E1, 12);
  REPORT_BACKWARD(E2, 13);
  REPORT_BACKWARD(E3, 14);
  REPORT_BACKWARD(E4, 15);
  REPORT_BACKWARD(E5, 16);
  REPORT_BACKWARD(E6, 17);
  REPORT_BACKWARD(E7, 18);
}

#endif // HAS_DRIVER_SAFE_POWER_PROTECT
