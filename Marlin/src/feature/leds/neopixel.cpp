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
  pixels.setBrightness(NEOPIXEL_BRIGHTNESS); // 0 - 255 range
  pixels.begin();
  pixels.show(); // initialize to all off

  #if ENABLED(NEOPIXEL_STARTUP_TEST)
    delay(2000);
    set_neopixel_color(pixels.Color(255, 0, 0, 0));  // red
    delay(2000);
    set_neopixel_color(pixels.Color(0, 255, 0, 0));  // green
    delay(2000);
    set_neopixel_color(pixels.Color(0, 0, 255, 0));  // blue
    delay(2000);
  #endif
  set_neopixel_color(pixels.Color(0, 0, 0, 255));    // white
}

bool neopixel_set_led_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w, const uint8_t p) {
  const uint32_t color = pixels.Color(r, g, b, w);
  pixels.setBrightness(p);
  #if !ENABLED(NEOPIXEL_IS_SEQUENTIAL)
    set_neopixel_color(color);
    return false;
  #else
    static uint16_t nextLed = 0;
    pixels.setPixelColor(nextLed, color);
    pixels.show();
    if (++nextLed >= pixels.numPixels()) nextLed = 0;
    return true;
  #endif
}

#endif // NEOPIXEL_LED
