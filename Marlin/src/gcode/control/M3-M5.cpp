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
 *  M4 - Laser ON/Power (Ramped power)
 *  M3I enables continuous inline power and it is processed by the planner. Move blocks are
 *  calculated and each block power buffer value is set there. The Stepper ISR then processes the blocks inline.
 *  Within inline mode M3 S-Values will set the power for the next moves e.g. G1 X10 Y10 powers on with the last S-Value
 *  M3I must be already set when using planner synced M3 inline S-Values (LASER_POWER_SYNC)   
 *  M4I sets dynamic mode which takes the currently set feedrate and calculates a laser power OCR value
 *  M5I clears inline mode and set power to 0
 *  M5 sets the power output to 0 but leaves inline mode on.
 *
 * Spindle:
 *  M3 - Spindle ON (Clockwise)
 *  M4 - Spindle ON (Counter-clockwise)
 *
 * Parameters:
 *  S<power> - Set power. S0 will turn the spindle/laser off.
 *
 *  If no PWM pin is defined then M3/M4 just turns it on or off.
 *
 *  At least 12.8kHz (50Hz * 256) is needed for Spindle PWM.
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
  if (cutter.cutter_mode == CUTTER_MODE_STANDARD)
    planner.synchronize();   // Wait for previous movement commands (G0/G1/G2/G3) to complete before changing power

  #if ENABLED(LASER_FEATURE)
    if (parser.seen('I')) {
      cutter.cutter_mode = is_M4 ? CUTTER_MODE_DYNAMIC : CUTTER_MODE_CONTINUOUS;
      cutter.inline_power(0);
      cutter.set_enabled(true);
    }
  #endif

  auto get_s_power = [] {
    if (parser.seen('S')) {
      #if ENABLED(LASER_POWER_TRAP)
        cutter.unitPower = parser.value_float();
      #else
        cutter.unitPower = cutter.power_to_range(parser.value_float());
      #endif
      cutter.menuPower = cutter.unitPower;
    }
    else if (cutter.cutter_mode == CUTTER_MODE_STANDARD) 
      cutter.menuPower = cutter.unitPower = cutter.cpwr_to_upwr(SPEED_POWER_STARTUP);

    // PWM not implied, power converted to OCR from unit definition and on/off if not PWM.
    cutter.power = TERN(SPINDLE_LASER_USE_PWM, cutter.upower_to_ocr(cutter.unitPower), cutter.unitPower > 0 ? 255 : 0);
    return cutter.unitPower;
  };

  if (cutter.cutter_mode == CUTTER_MODE_CONTINUOUS || cutter.cutter_mode == CUTTER_MODE_DYNAMIC) {  // Laser power in inline mode
    #if ENABLED(LASER_FEATURE)
      planner.laser_inline.status.isPowered = true;                                                 // M3 or M4 is powered either way
      get_s_power();                                                                                // Update cutter.power if seen
      #if ENABLED(LASER_POWER_SYNC)
        // With power sync we only set power so it does not effect queued inline power sets
        planner.buffer_sync_block(BLOCK_FLAG_LASER_PWR);                                            // Send the flag, queueing inline power
      #else
        planner.synchronize();
        cutter.inline_power(cutter.power);
      #endif
    #endif
  }
  else {
    cutter.set_enabled(true);
    get_s_power();
    #if ENABLED(SPINDLE_SERVO)
      cutter.apply_power(cutter.unitPower);
    #else
      cutter.apply_power(TERN(SPINDLE_LASER_USE_PWM, cutter.upower_to_ocr(cutter.unitPower), cutter.unitPower > 0 ? 255 : 0));
    #endif
    TERN_(SPINDLE_CHANGE_DIR, cutter.set_reverse(is_M4));
  }
}

/**
 * M5 - Cutter OFF (when moves are complete)
 */
void GcodeSuite::M5() {
  planner.synchronize();
  cutter.power = 0;
  cutter.apply_power(cutter.power);             // M5 kills power in either mode but if it's in inline it will be still be the active mode.
  if (cutter.cutter_mode != CUTTER_MODE_STANDARD) {
    if (parser.seen('I')) {
      TERN_(LASER_FEATURE, cutter.inline_power(cutter.power));
      cutter.set_enabled(false);                  // Needs to happen while we are in inline mode to clear inline power.
      cutter.cutter_mode = CUTTER_MODE_STANDARD;  // Switch from inline to standard mode.
    }
  }
  cutter.set_enabled(false);                      // Disable enable output setting
}

#endif // HAS_CUTTER
