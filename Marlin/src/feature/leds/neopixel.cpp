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
 * Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(NEOPIXEL_LED)

#include "neopixel.h"

Adafruit_NeoPixel pixels(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800);

void set_neopixel_color(const uint32_t color) {
  for (uint16_t i = 0; i < pixels.numPixels(); ++i)
    pixels.setPixelColor(i, color);
  pixels.show();
}

void setup_neopixel() {
  SET_OUTPUT(NEOPIXEL_PIN);
  pixels.setBrightness(NEOPIXEL_BRIGHTNESS); // 0 - 255 range
  pixels.begin();
  pixels.show(); // initialize to all off

  #if ENABLED(NEOPIXEL_STARTUP_TEST)
    safe_delay(1000);
    set_neopixel_color(pixels.Color(255, 0, 0, 0));  // red
    safe_delay(1000);
    set_neopixel_color(pixels.Color(0, 255, 0, 0));  // green
    safe_delay(1000);
    set_neopixel_color(pixels.Color(0, 0, 255, 0));  // blue
    safe_delay(1000);
  #endif
  set_neopixel_color(pixels.Color(NEO_WHITE));       // white
}

#endif // NEOPIXEL_LED
