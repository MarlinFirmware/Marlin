/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * feature/pancakebot.cpp - PancakeBot customizations
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(PANCAKEBOT)

#include "pancakebot.h"
#include "../sd/cardreader.h"

PancakeBot pancakebot;

/**
 * Set up the LEDs. The Working LED (blue) shows SD printing and
 * the SD Error LED (red) shows that no SD card is mounted.
 */
void PancakeBot::init() {
  OUT_WRITE(PANCAKEBOT_WORKING_LED_PIN, LOW);
  OUT_WRITE(PANCAKEBOT_SD_ERROR_LED_PIN, LOW);
}

void PancakeBot::idle() {
  WRITE(PANCAKEBOT_WORKING_LED_PIN, card.isStillPrinting());
  WRITE(PANCAKEBOT_SD_ERROR_LED_PIN, !card.isMounted());
}

#endif // PANCAKEBOT
