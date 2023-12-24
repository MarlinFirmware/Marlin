/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_HC32

#include "endstop_interrupts.h"
#include "../../module/endstops.h"
#include <Arduino.h>

#define ENDSTOP_IRQ_PRIORITY DDL_IRQ_PRIORITY_06

//
// IRQ handler
//
void endstopIRQHandler() {
  bool flag = false;

  // Check all irq flags
  #define CHECK(name) TERN_(USE_##name, flag |= checkIRQFlag(name##_PIN, /*clear*/ true))

  CHECK(X_MAX);
  CHECK(X_MIN);

  CHECK(Y_MAX);
  CHECK(Y_MIN);

  CHECK(Z_MAX);
  CHECK(Z_MIN);

  CHECK(Z2_MAX);
  CHECK(Z2_MIN);

  CHECK(Z3_MAX);
  CHECK(Z3_MIN);

  CHECK(Z_MIN_PROBE);

  // Update endstops
  if (flag) endstops.update();

  #undef CHECK
}

//
// HAL functions
//
void setup_endstop_interrupts() {
  #define SETUP(name) TERN_(USE_##name, attachInterrupt(name##_PIN, endstopIRQHandler, CHANGE); setInterruptPriority(name##_PIN, ENDSTOP_IRQ_PRIORITY))

  SETUP(X_MAX);
  SETUP(X_MIN);

  SETUP(Y_MAX);
  SETUP(Y_MIN);

  SETUP(Z_MAX);
  SETUP(Z_MIN);

  SETUP(Z2_MAX);
  SETUP(Z2_MIN);

  SETUP(Z3_MAX);
  SETUP(Z3_MIN);

  SETUP(Z_MIN_PROBE);

  #undef SETUP
}

// Ensure 1 - 10 IRQs are registered
// Disable some endstops if you encounter this error
#define ENDSTOPS_INTERRUPTS_COUNT COUNT_ENABLED(USE_X_MAX, USE_X_MIN, USE_Y_MAX, USE_Y_MIN, USE_Z_MAX, USE_Z_MIN, USE_Z2_MAX, USE_Z2_MIN, USE_Z3_MAX, USE_Z3_MIN, USE_Z_MIN_PROBE)
#if ENDSTOPS_INTERRUPTS_COUNT > 10
  #error "Too many endstop interrupts! HC32F460 only supports 10 endstop interrupts."
#elif ENDSTOPS_INTERRUPTS_COUNT == 0
  #error "No endstop interrupts are enabled! Comment out this line to continue."
#endif

#endif // ARDUINO_ARCH_HC32
