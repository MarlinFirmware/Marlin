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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SPINDLE_LASER_ENABLE)

#include "../gcode.h"
#include "../../module/stepper.h"

/**
 * M3: Spindle Clockwise
 * M4: Spindle Counter-clockwise
 *
 *  S0 turns off spindle.
 *
 *  If no speed PWM output is defined then M3/M4 just turns it on.
 *
 *  At least 12.8KHz (50Hz * 256) is needed for spindle PWM.
 *  Hardware PWM is required. ISRs are too slow.
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
 *  SPINDLE_LASER_ENABLE_PIN needs an external pullup or it may power on
 *  the spindle/laser during power-up or when connecting to the host
 *  (usually goes through a reset which sets all I/O pins to tri-state)
 *
 *  PWM duty cycle goes from 0 (off) to 255 (always on).
 */

// Wait for spindle to come up to speed
inline void delay_for_power_up() { gcode.dwell(SPINDLE_LASER_POWERUP_DELAY); }

// Wait for spindle to stop turning
inline void delay_for_power_down() { gcode.dwell(SPINDLE_LASER_POWERDOWN_DELAY); }

/**
 * ocr_val_mode() is used for debugging and to get the points needed to compute the RPM vs ocr_val line
 *
 * it accepts inputs of 0-255
 */

inline void ocr_val_mode() {
  uint8_t spindle_laser_power = parser.value_byte();
  WRITE(SPINDLE_LASER_ENABLE_PIN, SPINDLE_LASER_ENABLE_INVERT); // turn spindle on (active low)
  if (SPINDLE_LASER_PWM_INVERT) spindle_laser_power = 255 - spindle_laser_power;
  analogWrite(SPINDLE_LASER_PWM_PIN, spindle_laser_power);
}

void GcodeSuite::M3_M4(bool is_M3) {

  planner.synchronize();   // wait until previous movement commands (G0/G0/G2/G3) have completed before playing with the spindle
  #if SPINDLE_DIR_CHANGE
    const bool rotation_dir = (is_M3 != SPINDLE_INVERT_DIR);
    if (SPINDLE_STOP_ON_DIR_CHANGE \
       && READ(SPINDLE_LASER_ENABLE_PIN) == SPINDLE_LASER_ENABLE_INVERT \
       && READ(SPINDLE_DIR_PIN) != rotation_dir
    ) {
      WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);  // turn spindle off
      delay_for_power_down();
    }
    WRITE(SPINDLE_DIR_PIN, rotation_dir);
  #endif

  /**
   * Our final value for ocr_val is an unsigned 8 bit value between 0 and 255 which usually means uint8_t.
   * Went to uint16_t because some of the uint8_t calculations would sometimes give 1000 0000 rather than 1111 1111.
   * Then needed to AND the uint16_t result with 0x00FF to make sure we only wrote the byte of interest.
   */
  #if ENABLED(SPINDLE_LASER_PWM)
    if (parser.seen('O')) ocr_val_mode();
    else {
      const float spindle_laser_power = parser.floatval('S');
      if (spindle_laser_power == 0) {
        WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);                                    // turn spindle off (active low)
        analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);                           // only write low byte
        delay_for_power_down();
      }
      else {
        int16_t ocr_val = (spindle_laser_power - (SPEED_POWER_INTERCEPT)) * (1.0 / (SPEED_POWER_SLOPE));  // convert RPM to PWM duty cycle
        NOMORE(ocr_val, 255);                                                                             // limit to max the Atmel PWM will support
        if (spindle_laser_power <= SPEED_POWER_MIN)
          ocr_val = (SPEED_POWER_MIN - (SPEED_POWER_INTERCEPT)) * (1.0 / (SPEED_POWER_SLOPE));            // minimum setting
        if (spindle_laser_power >= SPEED_POWER_MAX)
          ocr_val = (SPEED_POWER_MAX - (SPEED_POWER_INTERCEPT)) * (1.0 / (SPEED_POWER_SLOPE));            // limit to max RPM
        if (SPINDLE_LASER_PWM_INVERT) ocr_val = 255 - ocr_val;
        WRITE(SPINDLE_LASER_ENABLE_PIN, SPINDLE_LASER_ENABLE_INVERT);                                     // turn spindle on (active low)
        analogWrite(SPINDLE_LASER_PWM_PIN, ocr_val & 0xFF);                                               // only write low byte
        delay_for_power_up();
      }
    }
  #else
    WRITE(SPINDLE_LASER_ENABLE_PIN, SPINDLE_LASER_ENABLE_INVERT); // turn spindle on (active low) if spindle speed option not enabled
    delay_for_power_up();
  #endif
}

/**
 * M5 turn off spindle
 */
void GcodeSuite::M5() {
  planner.synchronize();
  WRITE(SPINDLE_LASER_ENABLE_PIN, !SPINDLE_LASER_ENABLE_INVERT);
  #if ENABLED(SPINDLE_LASER_PWM)
    analogWrite(SPINDLE_LASER_PWM_PIN, SPINDLE_LASER_PWM_INVERT ? 255 : 0);
  #endif
  delay_for_power_down();
}

#endif // SPINDLE_LASER_ENABLE
