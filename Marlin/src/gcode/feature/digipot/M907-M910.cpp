/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM || ENABLED(DIGIPOT_I2C) || ENABLED(DAC_STEPPER_CURRENT)

#include "../../gcode.h"

#if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
  #include "../../../module/stepper.h"
#endif

#if ENABLED(DIGIPOT_I2C)
  #include "../../../feature/digipot/digipot.h"
#endif

#if ENABLED(DAC_STEPPER_CURRENT)
  #include "../../../feature/dac/stepper_dac.h"
#endif

/**
 * M907: Set digital trimpot motor current using axis codes X, Y, Z, E, B, S
 */
void GcodeSuite::M907() {
  #if HAS_DIGIPOTSS

    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.digipot_current(i, parser.value_int());
    if (parser.seen('B')) stepper.digipot_current(4, parser.value_int());
    if (parser.seen('S')) for (uint8_t i = 0; i <= 4; i++) stepper.digipot_current(i, parser.value_int());

  #elif HAS_MOTOR_CURRENT_PWM

    #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
      if (parser.seen('X')) stepper.digipot_current(0, parser.value_int());
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
      if (parser.seen('Z')) stepper.digipot_current(1, parser.value_int());
    #endif
    #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
      if (parser.seen('E')) stepper.digipot_current(2, parser.value_int());
    #endif

  #endif

  #if ENABLED(DIGIPOT_I2C)
    // this one uses actual amps in floating point
    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) digipot_i2c_set_current(i, parser.value_float());
    // for each additional extruder (named B,C,D,E..., channels 4,5,6,7...)
    for (uint8_t i = NUM_AXIS; i < DIGIPOT_I2C_NUM_CHANNELS; i++) if (parser.seen('B' + i - (NUM_AXIS))) digipot_i2c_set_current(i, parser.value_float());
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT)
    if (parser.seen('S')) {
      const float dac_percent = parser.value_float();
      for (uint8_t i = 0; i <= 4; i++) dac_current_percent(i, dac_percent);
    }
    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) dac_current_percent(i, parser.value_float());
  #endif
}

#if HAS_DIGIPOTSS || ENABLED(DAC_STEPPER_CURRENT)

  /**
   * M908: Control digital trimpot directly (M908 P<pin> S<current>)
   */
  void GcodeSuite::M908() {
    #if HAS_DIGIPOTSS
      stepper.digitalPotWrite(
        parser.intval('P'),
        parser.intval('S')
      );
    #endif
    #if ENABLED(DAC_STEPPER_CURRENT)
      dac_current_raw(
        parser.byteval('P', -1),
        parser.ushortval('S', 0)
      );
    #endif
  }

#endif // HAS_DIGIPOTSS || DAC_STEPPER_CURRENT

#if ENABLED(DAC_STEPPER_CURRENT)

  void GcodeSuite::M909() { dac_print_values(); }
  void GcodeSuite::M910() { dac_commit_eeprom(); }

#endif // DAC_STEPPER_CURRENT

#endif // HAS_DIGIPOTSS || DAC_STEPPER_CURRENT || HAS_MOTOR_CURRENT_PWM || DIGIPOT_I2C
