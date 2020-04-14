/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
 * Endstop interrupts for ATMEL SAMD51 based targets.
 *
 * On SAMD51, all pins support external interrupt capability.
 * Any pin can be used for external interrupts, but there are some restrictions.
 * At most 16 different external interrupts can be used at one time.
 * Further, you can’t just pick any 16 pins to use. This is because every pin on the SAMD51
 * connects to what is called an EXTINT line, and only one pin per EXTINT line can be used for external
 * interrupts at a time
 */

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

#define MATCH_EILINE(P1,P2)     (P1 != P2 && PIN_TO_EILINE(P1) == PIN_TO_EILINE(P2))
#if HAS_X_MAX
  #define MATCH_X_MAX_EILINE(P) MATCH_EILINE(P, X_MAX_PIN)
#else
  #define MATCH_X_MAX_EILINE(P) false
#endif
#if HAS_X_MIN
  #define MATCH_X_MIN_EILINE(P) MATCH_EILINE(P, X_MIN_PIN)
#else
  #define MATCH_X_MIN_EILINE(P) false
#endif
#if HAS_Y_MAX
   #define MATCH_Y_MAX_EILINE(P) MATCH_EILINE(P, Y_MAX_PIN)
#else
   #define MATCH_Y_MAX_EILINE(P) false
#endif
#if HAS_Y_MIN
  #define MATCH_Y_MIN_EILINE(P) MATCH_EILINE(P, Y_MIN_PIN)
#else
  #define MATCH_Y_MIN_EILINE(P) false
#endif
#if HAS_Z_MAX
   #define MATCH_Z_MAX_EILINE(P) MATCH_EILINE(P, Z_MAX_PIN)
#else
  #define MATCH_Z_MAX_EILINE(P) false
#endif
#if HAS_Z_MIN
  #define MATCH_Z_MIN_EILINE(P) MATCH_EILINE(P, Z_MIN_PIN)
#else
  #define MATCH_Z_MIN_EILINE(P) false
#endif
#if HAS_Z2_MAX
  #define MATCH_Z2_MAX_EILINE(P) MATCH_EILINE(P, Z2_MAX_PIN)
#else
  #define MATCH_Z2_MAX_EILINE(P) false
#endif
#if HAS_Z2_MIN
  #define MATCH_Z2_MIN_EILINE(P) MATCH_EILINE(P, Z2_MIN_PIN)
#else
  #define MATCH_Z2_MIN_EILINE(P) false
#endif
#if HAS_Z3_MAX
  #define MATCH_Z3_MAX_EILINE(P) MATCH_EILINE(P, Z3_MAX_PIN)
#else
  #define MATCH_Z3_MAX_EILINE(P) false
#endif
#if HAS_Z3_MIN
  #define MATCH_Z3_MIN_EILINE(P) MATCH_EILINE(P, Z3_MIN_PIN)
#else
  #define MATCH_Z3_MIN_EILINE(P) false
#endif
#if HAS_Z_MIN_PROBE_PIN
  #define MATCH_Z_MIN_PROBE_EILINE(P)   MATCH_EILINE(P, Z_MIN_PROBE_PIN)
#else
  #define MATCH_Z_MIN_PROBE_EILINE(P) false
#endif
#define AVAILABLE_EILINE(P)     (PIN_TO_EILINE(P) != -1                                 \
                                 && !MATCH_X_MAX_EILINE(P) && !MATCH_X_MIN_EILINE(P)    \
                                 && !MATCH_Y_MAX_EILINE(P) && !MATCH_Y_MIN_EILINE(P)    \
                                 && !MATCH_Z_MAX_EILINE(P) && !MATCH_Z_MIN_EILINE(P)    \
                                 && !MATCH_Z2_MAX_EILINE(P) && !MATCH_Z2_MIN_EILINE(P)  \
                                 && !MATCH_Z3_MAX_EILINE(P) && !MATCH_Z3_MIN_EILINE(P)  \
                                 && !MATCH_Z_MIN_PROBE_EILINE(P))

// One ISR for all EXT-Interrupts
void endstop_ISR() { endstops.update(); }

void setup_endstop_interrupts() {
  #if HAS_X_MAX
    #if !AVAILABLE_EILINE(X_MAX_PIN)
      static_assert(false, "X_MAX_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(X_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_X_MIN
    #if !AVAILABLE_EILINE(X_MIN_PIN)
      static_assert(false, "X_MIN_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(X_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Y_MAX
    #if !AVAILABLE_EILINE(Y_MAX_PIN)
      static_assert(false, "Y_MAX_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Y_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Y_MIN
    #if !AVAILABLE_EILINE(Y_MIN_PIN)
      static_assert(false, "Y_MIN_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Y_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MAX
    #if !AVAILABLE_EILINE(Z_MAX_PIN)
      static_assert(false, "Z_MAX_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MIN
    #if !AVAILABLE_EILINE(Z_MIN_PIN)
      static_assert(false, "Z_MIN_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z2_MAX
    #if !AVAILABLE_EILINE(Z2_MAX_PIN)
      static_assert(false, "Z2_MAX_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z2_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z2_MIN
    #if !AVAILABLE_EILINE(Z2_MIN_PIN)
      static_assert(false, "Z2_MIN_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z2_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z3_MAX
    #if !AVAILABLE_EILINE(Z3_MAX_PIN)
      static_assert(false, "Z3_MAX_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z3_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z3_MIN
    #if !AVAILABLE_EILINE(Z3_MIN_PIN)
      static_assert(false, "Z3_MIN_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z3_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if HAS_Z_MIN_PROBE_PIN
    #if !AVAILABLE_EILINE(Z_MIN_PROBE_PIN)
      static_assert(false, "Z_MIN_PROBE_PIN has no EXTINT line available.");
    #endif
    attachInterrupt(Z_MIN_PROBE_PIN, endstop_ISR, CHANGE);
  #endif
}
