/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
void endstop_ISR() { endstops.update(); }

void setup_endstop_interrupts() {
  #define _ATTACH(P) attachInterrupt(digitalPinToInterrupt(P), endstop_ISR, CHANGE)
  #define LPC1768_PIN_INTERRUPT_M(pin) ((pin >> 0x5 & 0x7) == 0 || (pin >> 0x5 & 0x7) == 2)

  #if HAS_X_MAX
    #if !LPC1768_PIN_INTERRUPT_M(X_MAX_PIN)
      #error "X_MAX_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(X_MAX_PIN);
  #endif
  #if HAS_X_MIN
    #if !LPC1768_PIN_INTERRUPT_M(X_MIN_PIN)
      #error "X_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(X_MIN_PIN);
  #endif
  #if HAS_Y_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Y_MAX_PIN)
      #error "Y_MAX_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Y_MAX_PIN);
  #endif
  #if HAS_Y_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Y_MIN_PIN)
      #error "Y_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Y_MIN_PIN);
  #endif
  #if HAS_Z_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z_MAX_PIN)
      #error "Z_MAX_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z_MAX_PIN);
  #endif
  #if HAS_Z_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z_MIN_PIN)
      #error "Z_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
     _ATTACH(Z_MIN_PIN);
  #endif
  #if HAS_Z2_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z2_MAX_PIN)
      #error "Z2_MAX_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z2_MAX_PIN);
  #endif
  #if HAS_Z2_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z2_MIN_PIN)
      #error "Z2_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z2_MIN_PIN);
  #endif
  #if HAS_Z3_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z3_MAX_PIN)
      #error "Z3_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z3_MAX_PIN);
  #endif
  #if HAS_Z3_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z3_MIN_PIN)
      #error "Z3_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z3_MIN_PIN);
  #endif
  #if HAS_Z4_MAX
    #if !LPC1768_PIN_INTERRUPT_M(Z4_MAX_PIN)
      #error "Z4_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z4_MAX_PIN);
  #endif
  #if HAS_Z4_MIN
    #if !LPC1768_PIN_INTERRUPT_M(Z4_MIN_PIN)
      #error "Z4_MIN_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z4_MIN_PIN);
  #endif
  #if HAS_Z_MIN_PROBE_PIN
    #if !LPC1768_PIN_INTERRUPT_M(Z_MIN_PROBE_PIN)
      #error "Z_MIN_PROBE_PIN is not INTERRUPT-capable. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z_MIN_PROBE_PIN);
  #endif
  #if HAS_I_MAX
    #if !LPC1768_PIN_INTERRUPT_M(I_MAX_PIN)
      #error "I_MAX_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(I_MAX_PIN);
  #elif HAS_I_MIN
    #if !LPC1768_PIN_INTERRUPT_M(I_MIN_PIN)
      #error "I_MIN_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(I_MIN_PIN);
  #endif
  #if HAS_J_MAX
    #if !LPC1768_PIN_INTERRUPT_M(J_MAX_PIN)
      #error "J_MAX_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(J_MAX_PIN);
  #elif HAS_J_MIN
    #if !LPC1768_PIN_INTERRUPT_M(J_MIN_PIN)
      #error "J_MIN_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(J_MIN_PIN);
  #endif
  #if HAS_K_MAX
    #if !LPC1768_PIN_INTERRUPT_M(K_MAX_PIN)
      #error "K_MAX_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(K_MAX_PIN);
  #elif HAS_K_MIN
    #if !LPC1768_PIN_INTERRUPT_M(K_MIN_PIN)
      #error "K_MIN_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(K_MIN_PIN);
  #endif
  #if HAS_U_MAX
    #if !LPC1768_PIN_INTERRUPT_M(U_MAX_PIN)
      #error "U_MAX_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(U_MAX_PIN);
  #elif HAS_U_MIN
    #if !LPC1768_PIN_INTERRUPT_M(U_MIN_PIN)
      #error "U_MIN_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(U_MIN_PIN);
  #endif
  #if HAS_V_MAX
    #if !LPC1768_PIN_INTERRUPT_M(V_MAX_PIN)
      #error "V_MAX_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(V_MAX_PIN);
  #elif HAS_V_MIN
    #if !LPC1768_PIN_INTERRUPT_M(V_MIN_PIN)
      #error "V_MIN_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(V_MIN_PIN);
  #endif
  #if HAS_W_MAX
    #if !LPC1768_PIN_INTERRUPT_M(W_MAX_PIN)
      #error "W_MAX_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(W_MAX_PIN);
  #elif HAS_W_MIN
    #if !LPC1768_PIN_INTERRUPT_M(W_MIN_PIN)
      #error "W_MIN_PIN is not INTERRUPT-capable."
    #endif
    _ATTACH(W_MIN_PIN);
  #endif
}
