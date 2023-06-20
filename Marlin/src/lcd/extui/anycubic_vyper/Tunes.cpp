/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * lcd/extui/anycubic_vyper/Tunes.cpp
 */

/***********************************************************************
 * A Utility to play tunes using the buzzer in the printer controller. *
 * See Tunes.h for note and tune definitions.                          *
 ***********************************************************************/

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_VYPER)

#include "Tunes.h"
#include "../ui_api.h"

namespace Anycubic {

  void PlayTune(const uint8_t beeperPin, const uint16_t *tune, const uint8_t speed=1) {
    uint8_t pos = 1;
    uint16_t wholenotelen = tune[0] / speed;
    do {
      uint16_t freq = tune[pos];
      uint16_t notelen = wholenotelen / tune[pos + 1];

      ::tone(beeperPin, freq, notelen);
      ExtUI::delay_ms(notelen);
      pos += 2;

      if (pos >= MAX_TUNE_LENGTH) break;
    } while (tune[pos] != n_END);
  }

}

#endif // ANYCUBIC_LCD_VYPER
