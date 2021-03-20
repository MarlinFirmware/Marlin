/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Endstop Interrupts
 *
 * Without endstop interrupts the endstop pins must be polled continually in
 * the temperature-ISR via endstops.update(), most of the time finding no change.
 * With this feature endstops.update() is called only when we know that at
 * least one endstop has changed state, saving valuable CPU cycles.
 *
 * This feature only works when all used endstop pins can generate an 'external interrupt'.
 *
 * Test whether pins issue interrupts on your board by flashing 'pin_interrupt_test.ino'.
 * (Located in Marlin/buildroot/share/pin_interrupt_test/pin_interrupt_test.ino)
 */

#include "../../module/endstops.h"

// One ISR for all EXT-Interrupts
void endstop_ISR(void) { endstops.update(); }

void setup_endstop_interrupts(void) {
  #if HAS_X_MAX
    #if !LPC1768_PIN_INTERRUPT_M(X_MAX_PIN)
      #error "X_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(X_MAX_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_X_MIN
    #if !LPC1768_PIN_INTERRUPT_M(X_MIN_PIN)
      #error "X_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(X_MIN_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Y_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Y_MAX_PIN)
      #error "Y_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Y_MAX_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Y_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Y_MIN_PIN)
      #error "Y_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Y_MIN_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z_MAX_PIN)
      #error "Z_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z_MAX_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z_MIN_PIN)
      #error "Z_MIN_PIN is not an INTERRUPT capable pin."
    #endif
     attachInterrupt(digitalPinToInterrupt(Z_MIN_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z2_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z2_MAX_PIN)
      #error "Z2_MAX_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z2_MAX_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z2_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z2_MIN_PIN)
      #error "Z2_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z2_MIN_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z3_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z3_MIN_PIN)
      #error "Z3_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z3_MAX_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z3_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z3_MIN_PIN)
      #error "Z3_MIN_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z3_MIN_PIN), endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MIN_PROBE_PIN
    #if !LPC1768_PIN_INTERRUPT_M(Z_MIN_PROBE_PIN)
      #error "Z_MIN_PROBE_PIN is not an INTERRUPT capable pin."
    #endif
    attachInterrupt(digitalPinToInterrupt(Z_MIN_PROBE_PIN), endstop_ISR, CHANGE);
  #endif
}
