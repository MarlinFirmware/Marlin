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

#include "../../inc/MarlinConfig.h"

#if HAS_CUTTER

#include "../gcode.h"
#include "../../feature/spindle_laser.h"
#include "../../module/stepper.h"

/**
 * Laser:
 *  M3 - Laser ON/Power (Ramped power)
 *  M4 - Laser ON/Power (Continuous power)
 *
 * Spindle:
 *  M3 - Spindle ON (Clockwise)
 *  M4 - Spindle ON (Counter-clockwise)
 *
 * Parameters:
 *  S<power> - Set power. S0 will turn the spindle/laser off, except in relative mode.
 *  O<ocr>   - Set power and OCR (oscillator count register)
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
  auto get_s_power = [] {
    if (parser.seenval('S')) {
      const float spwr = parser.value_float();
      #if ENABLED(SPINDLE_SERVO)
        cutter.unitPower = spwr;
      #else
        cutter.unitPower = TERN(SPINDLE_LASER_PWM,
                              cutter.power_to_range(cutter_power_t(round(spwr))),
                              spwr > 0 ? 255 : 0);
      #endif
    }
    else
      cutter.unitPower = cutter.cpwr_to_upwr(SPEED_POWER_STARTUP);
    return cutter.unitPower;
  };

  #if ENABLED(LASER_POWER_INLINE)
    if (parser.seen('I') == DISABLED(LASER_POWER_INLINE_INVERT)) {
      // Laser power in inline mode
      cutter.inline_direction(is_M4); // Should always be unused
      #if ENABLED(SPINDLE_LASER_PWM)
        if (parser.seen('O')) {
          cutter.unitPower = cutter.power_to_range(parser.value_byte(), 0);
          cutter.inline_ocr_power(cutter.unitPower); // The OCR is a value from 0 to 255 (uint8_t)
        }
        else
          cutter.inline_power(cutter.upower_to_ocr(get_s_power()));
      #else
        cutter.set_inline_enabled(true);
      #endif
      return;
    }
    // Non-inline, standard case
    cutter.inline_disable(); // Prevent future blocks re-setting the power
  #endif

  planner.synchronize();   // Wait for previous movement commands (G0/G0/G2/G3) to complete before changing power
  cutter.set_reverse(is_M4);

  #if ENABLED(SPINDLE_LASER_PWM)
    if (parser.seenval('O')) {
      cutter.unitPower = cutter.power_to_range(parser.value_byte(), 0);
      cutter.set_ocr_power(cutter.unitPower); // The OCR is a value from 0 to 255 (uint8_t)
    }
    else
      cutter.set_power(cutter.upower_to_ocr(get_s_power()));
  #elif ENABLED(SPINDLE_SERVO)
    cutter.set_power(get_s_power());
  #else
    cutter.set_enabled(true);
  #endif
  cutter.menuPower = cutter.unitPower;
}

/**
 * M5 - Cutter OFF (when moves are complete)
 */
void GcodeSuite::M5() {
  #if ENABLED(LASER_POWER_INLINE)
    if (parser.seen('I') == DISABLED(LASER_POWER_INLINE_INVERT)) {
      cutter.set_inline_enabled(false); // Laser power in inline mode
      return;
    }
    // Non-inline, standard case
    cutter.inline_disable(); // Prevent future blocks re-setting the power
  #endif
  planner.synchronize();
  cutter.set_enabled(false);
  cutter.menuPower = cutter.unitPower;
}

#endif // HAS_CUTTER
