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

#ifndef HAL_ENDSTOP_INTERRUPTS_H_
#define HAL_ENDSTOP_INTERRUPTS_H_

volatile uint8_t e_hit = 0; // Different from 0 when the endstops should be tested in detail.
                            // Must be reset to 0 by the test function when finished.

// This is what is really done inside the interrupts.
FORCE_INLINE void endstop_ISR_worker( void ) {
  e_hit = 2; // Because the detection of a e-stop hit has a 1 step debouncer it has to be called at least twice.
}

// One ISR for all EXT-Interrupts
void endstop_ISR(void) { endstop_ISR_worker(); }

#ifdef __AVR__

  #include "HAL_AVR/endstop_interrupts.h"

#elif defined(ARDUINO_ARCH_SAM)

  #include "HAL_DUE/endstop_interrupts.h"

#elif IS_32BIT_TEENSY

  #include "HAL_TEENSY35_36/endstop_interrupts.h"

#elif defined(__STM32F1__)

  #include "HAL_STM32F1/endstop_interrupts.h"

#elif defined(STM32F7)

  #include "HAL_STM32F7/endstop_interrupts.h"

#else

  #error Unsupported Platform!

#endif

#endif /* HAL_ENDSTOP_INTERRUPTS_H_ */
