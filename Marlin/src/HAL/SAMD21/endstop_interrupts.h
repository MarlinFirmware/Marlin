/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * SAMD21 HAL developed by Bart Meijer (brupje)
 * Based on SAMD51 HAL by Giuliano Zaro (AKA GMagician)
 */

/**
 * Endstop interrupts for ATMEL SAMD21 based targets.
 *
 * On SAMD21, all pins support external interrupt capability.
 * Any pin can be used for external interrupts, but there are some restrictions.
 * At most 16 different external interrupts can be used at one time.
 * Further, you can’t just pick any 16 pins to use. This is because every pin on the SAMD21
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

#define MATCH_EILINE(P1,P2) (P1 != P2 && PIN_TO_EILINE(P1) == PIN_TO_EILINE(P2))
#define MATCH_X_MAX_EILINE(P)  TERN0(USE_X_MAX,  DEFER4(MATCH_EILINE)(P, X_MAX_PIN))
#define MATCH_X_MIN_EILINE(P)  TERN0(USE_X_MIN,  DEFER4(MATCH_EILINE)(P, X_MIN_PIN))
#define MATCH_Y_MAX_EILINE(P)  TERN0(USE_Y_MAX,  DEFER4(MATCH_EILINE)(P, Y_MAX_PIN))
#define MATCH_Y_MIN_EILINE(P)  TERN0(USE_Y_MIN,  DEFER4(MATCH_EILINE)(P, Y_MIN_PIN))
#define MATCH_Z_MAX_EILINE(P)  TERN0(USE_Z_MAX,  DEFER4(MATCH_EILINE)(P, Z_MAX_PIN))
#define MATCH_Z_MIN_EILINE(P)  TERN0(USE_Z_MIN,  DEFER4(MATCH_EILINE)(P, Z_MIN_PIN))
#define MATCH_I_MAX_EILINE(P)  TERN0(USE_I_MAX,  DEFER4(MATCH_EILINE)(P, I_MAX_PIN))
#define MATCH_I_MIN_EILINE(P)  TERN0(USE_I_MIN,  DEFER4(MATCH_EILINE)(P, I_MIN_PIN))
#define MATCH_J_MAX_EILINE(P)  TERN0(USE_J_MAX,  DEFER4(MATCH_EILINE)(P, J_MAX_PIN))
#define MATCH_J_MIN_EILINE(P)  TERN0(USE_J_MIN,  DEFER4(MATCH_EILINE)(P, J_MIN_PIN))
#define MATCH_K_MAX_EILINE(P)  TERN0(USE_K_MAX,  DEFER4(MATCH_EILINE)(P, K_MAX_PIN))
#define MATCH_K_MIN_EILINE(P)  TERN0(USE_K_MIN,  DEFER4(MATCH_EILINE)(P, K_MIN_PIN))
#define MATCH_U_MAX_EILINE(P)  TERN0(USE_U_MAX,  DEFER4(MATCH_EILINE)(P, U_MAX_PIN))
#define MATCH_U_MIN_EILINE(P)  TERN0(USE_U_MIN,  DEFER4(MATCH_EILINE)(P, U_MIN_PIN))
#define MATCH_V_MAX_EILINE(P)  TERN0(USE_V_MAX,  DEFER4(MATCH_EILINE)(P, V_MAX_PIN))
#define MATCH_V_MIN_EILINE(P)  TERN0(USE_V_MIN,  DEFER4(MATCH_EILINE)(P, V_MIN_PIN))
#define MATCH_W_MAX_EILINE(P)  TERN0(USE_W_MAX,  DEFER4(MATCH_EILINE)(P, W_MAX_PIN))
#define MATCH_W_MIN_EILINE(P)  TERN0(USE_W_MIN,  DEFER4(MATCH_EILINE)(P, W_MIN_PIN))
#define MATCH_X2_MAX_EILINE(P) TERN0(USE_X2_MAX, DEFER4(MATCH_EILINE)(P, X2_MAX_PIN))
#define MATCH_X2_MIN_EILINE(P) TERN0(USE_X2_MIN, DEFER4(MATCH_EILINE)(P, X2_MIN_PIN))
#define MATCH_Y2_MAX_EILINE(P) TERN0(USE_Y2_MAX, DEFER4(MATCH_EILINE)(P, Y2_MAX_PIN))
#define MATCH_Y2_MIN_EILINE(P) TERN0(USE_Y2_MIN, DEFER4(MATCH_EILINE)(P, Y2_MIN_PIN))
#define MATCH_Z2_MAX_EILINE(P) TERN0(USE_Z2_MAX, DEFER4(MATCH_EILINE)(P, Z2_MAX_PIN))
#define MATCH_Z2_MIN_EILINE(P) TERN0(USE_Z2_MIN, DEFER4(MATCH_EILINE)(P, Z2_MIN_PIN))
#define MATCH_Z3_MAX_EILINE(P) TERN0(USE_Z3_MAX, DEFER4(MATCH_EILINE)(P, Z3_MAX_PIN))
#define MATCH_Z3_MIN_EILINE(P) TERN0(USE_Z3_MIN, DEFER4(MATCH_EILINE)(P, Z3_MIN_PIN))
#define MATCH_Z4_MAX_EILINE(P) TERN0(USE_Z4_MAX, DEFER4(MATCH_EILINE)(P, Z4_MAX_PIN))
#define MATCH_Z4_MIN_EILINE(P) TERN0(USE_Z4_MIN, DEFER4(MATCH_EILINE)(P, Z4_MIN_PIN))
#define MATCH_Z_MIN_PROBE_EILINE(P) TERN0(USE_Z_MIN_PROBE, DEFER4(MATCH_EILINE)(P, Z_MIN_PROBE_PIN))

#define AVAILABLE_EILINE(P) ( PIN_TO_EILINE(P) != -1    \
  && !MATCH_X_MAX_EILINE(P) && !MATCH_X_MIN_EILINE(P)   \
  && !MATCH_Y_MAX_EILINE(P) && !MATCH_Y_MIN_EILINE(P)   \
  && !MATCH_Z_MAX_EILINE(P) && !MATCH_Z_MIN_EILINE(P)   \
  && !MATCH_I_MAX_EILINE(P) && !MATCH_I_MIN_EILINE(P)   \
  && !MATCH_J_MAX_EILINE(P) && !MATCH_J_MIN_EILINE(P)   \
  && !MATCH_K_MAX_EILINE(P) && !MATCH_K_MIN_EILINE(P)   \
  && !MATCH_U_MAX_EILINE(P) && !MATCH_U_MIN_EILINE(P)   \
  && !MATCH_V_MAX_EILINE(P) && !MATCH_V_MIN_EILINE(P)   \
  && !MATCH_W_MAX_EILINE(P) && !MATCH_W_MIN_EILINE(P)   \
  && !MATCH_X2_MAX_EILINE(P) && !MATCH_X2_MIN_EILINE(P) \
  && !MATCH_Y2_MAX_EILINE(P) && !MATCH_Y2_MIN_EILINE(P) \
  && !MATCH_Z2_MAX_EILINE(P) && !MATCH_Z2_MIN_EILINE(P) \
  && !MATCH_Z3_MAX_EILINE(P) && !MATCH_Z3_MIN_EILINE(P) \
  && !MATCH_Z4_MAX_EILINE(P) && !MATCH_Z4_MIN_EILINE(P) \
  && !MATCH_Z_MIN_PROBE_EILINE(P) )

// One ISR for all EXT-Interrupts
void endstop_ISR() { endstops.update(); }

void setup_endstop_interrupts() {
  #define _ATTACH(P) attachInterrupt(P, endstop_ISR, CHANGE)
  #if USE_X_MAX
    #if !AVAILABLE_EILINE(X_MAX_PIN)
      #error "X_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(X_MAX_PIN);
  #endif
  #if USE_X_MIN
    #if !AVAILABLE_EILINE(X_MIN_PIN)
      #error "X_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(X_MIN_PIN);
  #endif
  #if USE_Y_MAX
    #if !AVAILABLE_EILINE(Y_MAX_PIN)
      #error "Y_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Y_MAX_PIN);
  #endif
  #if USE_Y_MIN
    #if !AVAILABLE_EILINE(Y_MIN_PIN)
      #error "Y_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Y_MIN_PIN);
  #endif
  #if USE_Z_MAX
    #if !AVAILABLE_EILINE(Z_MAX_PIN)
      #error "Z_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z_MAX_PIN);
  #endif
  #if USE_Z_MIN
    #if !AVAILABLE_EILINE(Z_MIN_PIN)
      #error "Z_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z_MIN_PIN);
  #endif
  #if USE_X2_MAX
    #if !AVAILABLE_EILINE(X2_MAX_PIN)
      #error "X2_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(X2_MAX_PIN);
  #endif
  #if USE_X2_MIN
    #if !AVAILABLE_EILINE(X2_MIN_PIN)
      #error "X2_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(X2_MIN_PIN);
  #endif
  #if USE_Y2_MAX
    #if !AVAILABLE_EILINE(Y2_MAX_PIN)
      #error "Y2_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Y2_MAX_PIN);
  #endif
  #if USE_Y2_MIN
    #if !AVAILABLE_EILINE(Y2_MIN_PIN)
      #error "Y2_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Y2_MIN_PIN);
  #endif
  #if USE_Z2_MAX
    #if !AVAILABLE_EILINE(Z2_MAX_PIN)
      #error "Z2_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z2_MAX_PIN);
  #endif
  #if USE_Z2_MIN
    #if !AVAILABLE_EILINE(Z2_MIN_PIN)
      #error "Z2_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z2_MIN_PIN);
  #endif
  #if USE_Z3_MAX
    #if !AVAILABLE_EILINE(Z3_MAX_PIN)
      #error "Z3_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z3_MAX_PIN);
  #endif
  #if USE_Z3_MIN
    #if !AVAILABLE_EILINE(Z3_MIN_PIN)
      #error "Z3_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z3_MIN_PIN);
  #endif
  #if USE_Z4_MAX
    #if !AVAILABLE_EILINE(Z4_MAX_PIN)
      #error "Z4_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z4_MAX_PIN);
  #endif
  #if USE_Z4_MIN
    #if !AVAILABLE_EILINE(Z4_MIN_PIN)
      #error "Z4_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z4_MIN_PIN);
  #endif
  #if USE_Z_MIN_PROBE
    #if !AVAILABLE_EILINE(Z_MIN_PROBE_PIN)
      #error "Z_MIN_PROBE_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    _ATTACH(Z_MIN_PROBE_PIN);
  #endif
  #if USE_I_MAX
    #if !AVAILABLE_EILINE(I_MAX_PIN)
      #error "I_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(I_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_I_MIN
    #if !AVAILABLE_EILINE(I_MIN_PIN)
      #error "I_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(I_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_J_MAX
    #if !AVAILABLE_EILINE(J_MAX_PIN)
      #error "J_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(J_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_J_MIN
    #if !AVAILABLE_EILINE(J_MIN_PIN)
      #error "J_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(J_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_K_MAX
    #if !AVAILABLE_EILINE(K_MAX_PIN)
      #error "K_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(K_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_K_MIN
    #if !AVAILABLE_EILINE(K_MIN_PIN)
      #error "K_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(K_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_U_MAX
    #if !AVAILABLE_EILINE(U_MAX_PIN)
      #error "U_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(U_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_U_MIN
    #if !AVAILABLE_EILINE(U_MIN_PIN)
      #error "U_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(U_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_V_MAX
    #if !AVAILABLE_EILINE(V_MAX_PIN)
      #error "V_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(V_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_V_MIN
    #if !AVAILABLE_EILINE(V_MIN_PIN)
      #error "V_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(V_MIN_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_W_MAX
    #if !AVAILABLE_EILINE(W_MAX_PIN)
      #error "W_MAX_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(W_MAX_PIN, endstop_ISR, CHANGE);
  #endif
  #if USE_W_MIN
    #if !AVAILABLE_EILINE(W_MIN_PIN)
      #error "W_MIN_PIN has no EXTINT line available. Disable ENDSTOP_INTERRUPTS_FEATURE to continue."
    #endif
    attachInterrupt(W_MIN_PIN, endstop_ISR, CHANGE);
  #endif
}
