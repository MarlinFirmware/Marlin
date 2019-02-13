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

#if PIN_EXISTS(CHDK) || HAS_PHOTOGRAPH

#include "../../gcode.h"

bool chdk_active; // = false
millis_t chdk_timeout;

/**
 * M240: Trigger a camera by emulating a Canon RC-1
 *       See http://www.doc-diy.net/photo/rc-1_hacked/
 */
void GcodeSuite::M240() {

  #if PIN_EXISTS(CHDK)

    OUT_WRITE(CHDK_PIN, HIGH);
    chdk_timeout = millis() + CHDK_DELAY;
    chdk_active = true;

  #elif HAS_PHOTOGRAPH

    const uint8_t NUM_PULSES = 16;
    const float PULSE_LENGTH = 0.01524;
    for (int i = 0; i < NUM_PULSES; i++) {
      WRITE(PHOTOGRAPH_PIN, HIGH);
      _delay_ms(PULSE_LENGTH);
      WRITE(PHOTOGRAPH_PIN, LOW);
      _delay_ms(PULSE_LENGTH);
    }
    delay(7.33);
    for (int i = 0; i < NUM_PULSES; i++) {
      WRITE(PHOTOGRAPH_PIN, HIGH);
      _delay_ms(PULSE_LENGTH);
      WRITE(PHOTOGRAPH_PIN, LOW);
      _delay_ms(PULSE_LENGTH);
    }

  #endif
}

#endif // CHDK_PIN || HAS_PHOTOGRAPH
