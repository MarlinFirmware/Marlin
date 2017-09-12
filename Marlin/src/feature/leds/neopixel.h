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
 * Neopixel support
 */

#ifndef __NEOPIXEL_H__
#define __NEOPIXEL_H__

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

void setup_neopixel();
bool neopixel_set_led_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w, const bool isSequence);

extern Adafruit_NeoPixel pixels;

#endif // __NEOPIXEL_H__
