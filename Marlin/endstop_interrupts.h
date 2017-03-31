/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Endstop Interrupts
 *
 * Without endstop interrupts the endstop pins must be polled continually in
 * the stepper-ISR via endstops.update(), most of the time finding no change.
 * With this feature endstops.update() is called only when we know that at
 * least one endstop has changed state, saving valuable CPU cycles.
 *
 * This feature only works when all used endstop pins can generate either an
 * 'external interrupt' or a 'pin change interrupt'.
 *
 * Test whether pins issue interrupts on your board by flashing 'pin_interrupt_test.ino'.
 * (Located in Marlin/buildroot/share/pin_interrupt_test/pin_interrupt_test.ino)
 */

#ifndef _ENDSTOP_INTERRUPTS_H_
#define _ENDSTOP_INTERRUPTS_H_

#include "macros.h"

/**
 * Patch for pins_arduino.h (...\Arduino\hardware\arduino\avr\variants\mega\pins_arduino.h)
 *
 * These macros for the Arduino MEGA do not include the two connected pins on Port J (D13, D14).
 * So we extend them here because these are the normal pins for Y_MIN and Y_MAX on RAMPS.
 * There are more PCI-enabled processor pins on Port J, but they are not connected to Arduino MEGA.
 */
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #undef  digitalPinToPCICR
  #define digitalPinToPCICR(p)    ( WITHIN(p, 10, 15) || \
                                    WITHIN(p, 50, 53) || \
                                    WITHIN(p, 62, 69) ? &PCICR : (uint8_t*)0 )
  #undef  digitalPinToPCICRbit
  #define digitalPinToPCICRbit(p) ( WITHIN(p, 10, 13) || WITHIN(p, 50, 53) ? 0 : \
                                    WITHIN(p, 14, 15) ? 1 : \
                                    WITHIN(p, 62, 69) ? 2 : \
                                    0 )
  #undef  digitalPinToPCMSK
  #define digitalPinToPCMSK(p)    ( WITHIN(p, 10, 13) || WITHIN(p, 50, 53) ? &PCMSK0 : \
                                    WITHIN(p, 14, 15) ? &PCMSK1 : \
                                    WITHIN(p, 62, 69) ? &PCMSK2 : \
                                    (uint8_t *)0 )
  #undef  digitalPinToPCMSKbit
  #define digitalPinToPCMSKbit(p) ( WITHIN(p, 10, 13) ? ((p) - 6) : \
                                    (p) == 14 || (p) == 51 ? 2 : \
                                    (p) == 15 || (p) == 52 ? 1 : \
                                    (p) == 50 ? 3 : \
                                    (p) == 53 ? 0 : \
                                    WITHIN(p, 62, 69) ? ((p) - 62) : \
                                    0 )
#endif

volatile uint8_t e_hit = 0; // Different from 0 when the endstops should be tested in detail.
                            // Must be reset to 0 by the test function when finished.

// Install Pin change interrupt for a pin. Can be called multiple times.
void pciSetup(byte pin) {
  SBI(*digitalPinToPCMSK(pin), digitalPinToPCMSKbit(pin));  // enable pin
  SBI(PCIFR, digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  SBI(PCICR, digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

// This is what is really done inside the interrupts.
FORCE_INLINE void endstop_ISR_worker( void ) {
  e_hit = 2; // Because the detection of a e-stop hit has a 1 step debouncer it has to be called at least twice.
}

// Use one Routine to handle each group
// One ISR for all EXT-Interrupts
void endstop_ISR(void) { endstop_ISR_worker(); }

// Handlers for pin change interrupts
#ifdef PCINT0_vect
  ISR(PCINT0_vect) { endstop_ISR_worker(); }
#endif

#ifdef PCINT1_vect
  ISR(PCINT1_vect) { endstop_ISR_worker(); }
#endif

#ifdef PCINT2_vect
  ISR(PCINT2_vect) { endstop_ISR_worker(); }
#endif

#ifdef PCINT3_vect
  ISR(PCINT3_vect) { endstop_ISR_worker(); }
#endif

void setup_endstop_interrupts( void ) {

  #if HAS_X_MAX
    #if (digitalPinToInterrupt(X_MAX_PIN) != NOT_AN_INTERRUPT) // if pin has an external interrupt
      attachInterrupt(digitalPinToInterrupt(X_MAX_PIN), endstop_ISR, CHANGE); // assign it
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(X_MAX_PIN) != NULL, "X_MAX_PIN is not interrupt-capable"); // if pin has no pin change interrupt - error
      pciSetup(X_MAX_PIN);                                                            // assign it
    #endif
  #endif

  #if HAS_X_MIN
    #if (digitalPinToInterrupt(X_MIN_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(X_MIN_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(X_MIN_PIN) != NULL, "X_MIN_PIN is not interrupt-capable");
      pciSetup(X_MIN_PIN);
    #endif
  #endif

  #if HAS_Y_MAX
    #if (digitalPinToInterrupt(Y_MAX_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Y_MAX_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Y_MAX_PIN) != NULL, "Y_MAX_PIN is not interrupt-capable");
      pciSetup(Y_MAX_PIN);
    #endif
  #endif

  #if HAS_Y_MIN
    #if (digitalPinToInterrupt(Y_MIN_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Y_MIN_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Y_MIN_PIN) != NULL, "Y_MIN_PIN is not interrupt-capable");
      pciSetup(Y_MIN_PIN);
    #endif
  #endif

  #if HAS_Z_MAX
    #if (digitalPinToInterrupt(Z_MAX_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Z_MAX_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Z_MAX_PIN) != NULL, "Z_MAX_PIN is not interrupt-capable");
      pciSetup(Z_MAX_PIN);
    #endif
  #endif

  #if HAS_Z_MIN
    #if (digitalPinToInterrupt(Z_MIN_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Z_MIN_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Z_MIN_PIN) != NULL, "Z_MIN_PIN is not interrupt-capable");
      pciSetup(Z_MIN_PIN);
    #endif
  #endif

  #if HAS_Z2_MAX
    #if (digitalPinToInterrupt(Z2_MAX_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Z2_MAX_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Z2_MAX_PIN) != NULL, "Z2_MAX_PIN is not interrupt-capable");
      pciSetup(Z2_MAX_PIN);
    #endif
  #endif

  #if HAS_Z2_MIN
    #if (digitalPinToInterrupt(Z2_MIN_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Z2_MIN_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Z2_MIN_PIN) != NULL, "Z2_MIN_PIN is not interrupt-capable");
      pciSetup(Z2_MIN_PIN);
    #endif
  #endif

  #if HAS_Z_MIN_PROBE_PIN
    #if (digitalPinToInterrupt(Z_MIN_PROBE_PIN) != NOT_AN_INTERRUPT)
      attachInterrupt(digitalPinToInterrupt(Z_MIN_PROBE_PIN), endstop_ISR, CHANGE);
    #else
      // Not all used endstop/probe -pins can raise interrupts. Please deactivate ENDSTOP_INTERRUPTS or change the pin configuration!
      static_assert(digitalPinToPCICR(Z_MIN_PROBE_PIN) != NULL, "Z_MIN_PROBE_PIN is not interrupt-capable");
      pciSetup(Z_MIN_PROBE_PIN);
    #endif
  #endif

  // If we arrive here without raising an assertion, each pin has either an EXT-interrupt or a PCI.
}

#endif //_ENDSTOP_INTERRUPTS_H_
