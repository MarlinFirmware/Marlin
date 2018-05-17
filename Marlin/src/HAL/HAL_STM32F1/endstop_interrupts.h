/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2017 Victor Perez
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

/**
 * Endstop interrupts for Libmaple STM32F1 based targets.
 *
 * On STM32F, all pins support external interrupt capability.
 * Any pin can be used for external interrupts, but there are some restrictions.
 * At most 16 different external interrupts can be used at one time.
 * Further, you can’t just pick any 16 pins to use. This is because every pin on the STM32
 * connects to what is called an EXTI line, and only one pin per EXTI line can be used for external interrupts at a time
 * Check the Reference Manual of the MCU to confirm which line is used by each pin
 */

/**
 * Endstop Interrupts
 *
 * Without endstop interrupts the endstop pins must be polled continually in
 * the stepper-ISR via endstops.update(), most of the time finding no change.
 * With this feature endstops.update() is called only when we know that at
 * least one endstop has changed state, saving valuable CPU cycles.
 *
 * This feature only works when all used endstop pins can generate an 'external interrupt'.
 *
 * Test whether pins issue interrupts on your board by flashing 'pin_interrupt_test.ino'.
 * (Located in Marlin/buildroot/share/pin_interrupt_test/pin_interrupt_test.ino)
 */

#ifndef _ENDSTOP_INTERRUPTS_H_
#define _ENDSTOP_INTERRUPTS_H_

volatile uint8_t e_hit = 0; // Different from 0 when the endstops should be tested in detail.
                            // Must be reset to 0 by the test function when finished.

// This is what is really done inside the interrupts.
FORCE_INLINE void endstop_ISR_worker( void ) {
  e_hit = 2; // Because the detection of a e-stop hit has a 1 step debouncer it has to be called at least twice.
}

// One ISR for all EXT-Interrupts
void endstop_ISR(void) { endstop_ISR_worker(); }

void setup_endstop_interrupts(void) {
  #if HAS_X_MAX
    SET_INPUT(X_MAX_PIN);
    attachInterrupt(X_MAX_PIN, endstop_ISR, CHANGE); // assign it
  #endif
  #if HAS_X_MIN
    SET_INPUT(X_MIN_PIN);
    attachInterrupt(X_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Y_MAX
    SET_INPUT(Y_MAX_PIN);
    attachInterrupt(Y_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Y_MIN
    SET_INPUT(Y_MIN_PIN);
    attachInterrupt(Y_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MAX
    SET_INPUT(Z_MAX_PIN);
    attachInterrupt(Z_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MIN
    SET_INPUT(Z_MIN_PIN);
    attachInterrupt(Z_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z2_MAX
    SET_INPUT(Z2_MAX_PIN);
    attachInterrupt(Z2_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z2_MIN
    SET_INPUT(Z2_MIN_PIN);
    attachInterrupt(Z2_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MIN_PROBE_PIN
    SET_INPUT(Z_MIN_PROBE_PIN);
    attachInterrupt(Z_MIN_PROBE_PIN, endstop_ISR, CHANGE);
  #endif
}

#endif //_ENDSTOP_INTERRUPTS_H_
