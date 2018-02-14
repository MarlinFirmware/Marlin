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
 * This feature only works when all used endstop pins can generate an 'external interrupt'.
 *
 * Test whether pins issue interrupts on your board by flashing 'pin_interrupt_test.ino'.
 * (Located in Marlin/buildroot/share/pin_interrupt_test/pin_interrupt_test.ino)
 */

 #ifndef _ENDSTOP_INTERRUPTS_H_
 #define _ENDSTOP_INTERRUPTS_H_

void setup_endstop_interrupts(void) {
  pinMode(ENDSTOP_INTERRUPTS_PIN, INPUT);

  uint8_t pinmask = 0;
  #if HAS_X_MAX
    pinmask |= 1 << GET_PIN(X_MAX_PIN);
  #endif
  #if HAS_X_MIN
    pinmask |= 1 << GET_PIN(X_MIN_PIN);
  #endif
  #if HAS_Y_MAX
    pinmask |= 1 << GET_PIN(Y_MAX_PIN);
  #endif
  #if HAS_Y_MIN
    pinmask |= 1 << GET_PIN(Y_MIN_PIN);
  #endif
  #if HAS_Z_MAX
    pinmask |= 1 << GET_PIN(Z_MAX_PIN);
  #endif
  #if HAS_Z_MIN
    pinmask |= 1 << GET_PIN(Z_MIN_PIN);
  #endif
  #if HAS_Z_MIN_PROBE_PIN
    pinmask |= 1 << GET_PIN(Z_MIN_PROBE_PIN);
  #endif

  // This assumes that all the endstops are on the same port
  set_i2c_register(GET_REGISTER(ENDSTOP_INTERRUPTS_PIN, GPINTENA), pinmask);
  get_i2c_register(GET_REGISTER(X_MIN_PIN, GPIOA));
  
  attachInterrupt(ENDSTOP_INTERRUPTS_PIN, endstop_ISR, CHANGE);
}

#endif //_ENDSTOP_INTERRUPTS_H_
