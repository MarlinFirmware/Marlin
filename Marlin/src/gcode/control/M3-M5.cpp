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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if HAS_CUTTER

#include "../gcode.h"
#include "../../feature/spindle_laser.h"
#include "../../module/stepper.h"

/**
 * Laser:
 *
 *  M3 - Laser ON/Power (Ramped power)
 *  M4 - Laser ON/Power (Continuous power)
 *
 *    S<power> - Set power. S0 will turn the laser off.
 *    O<ocr>   - Set power and OCR
 *
 * Spindle:
 *
 *  M3 - Spindle ON (Clockwise)
 *  M4 - Spindle ON (Counter-clockwise)
 *
 *    S<power> - Set power. S0 will turn the spindle off.
 *    O<ocr>   - Set power and OCR
 *
 *  If no PWM pin is defined then M3/M4 just turns it on.
 *
 *  At least 12.8KHz (50Hz * 256) is needed for Spindle PWM.
 *  Hardware PWM is required on AVR. ISRs are too slow.
 *
 * NOTE: WGM for timers 3, 4, and 5 must be either Mode 1 or Mode 5.
 *       No other settings give a PWM signal that goes from 0 to 5 volts.
 *
 *       The system automatically sets WGM to Mode 1, so no special
 *       initialization is needed.
 *
 *       WGM bits for timer 2 are automatically set by the system to
 *       Mode 1. This produces an acceptable 0 to 5 volt signal.
 *       No special initialization is needed.
 *
 * NOTE: A minimum PWM frequency of 50 Hz is needed. All prescaler
 *       factors for timers 2, 3, 4, and 5 are acceptable.
 *
 *  SPINDLE_LASER_ENA_PIN needs an external pullup or it may power on
 *  the spindle/laser during power-up or when connecting to the host
 *  (usually goes through a reset which sets all I/O pins to tri-state)
 *
 *  PWM duty cycle goes from 0 (off) to 255 (always on).
 */
void GcodeSuite::M3_M4(const bool is_M4) {

  #if ENABLED(SPINDLE_FEATURE)
    planner.synchronize();   // Wait for movement to complete before changing power
  #endif

  cutter.set_direction(is_M4);

  #if ENABLED(SPINDLE_LASER_PWM)
    if (parser.seenval('O'))
      cutter.set_ocr_power(parser.value_byte()); // The OCR is a value from 0 to 255 (uint8_t)
    else
      cutter.set_power(parser.intval('S', 255));
  #else
    cutter.set_enabled(true);
  #endif
}

/**
 * M5 - Cutter OFF
 */
void GcodeSuite::M5() {
  #if ENABLED(SPINDLE_FEATURE)
    planner.synchronize();
  #endif
  cutter.set_enabled(false);
}

#endif // HAS_CUTTER
