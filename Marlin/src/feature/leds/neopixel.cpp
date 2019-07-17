/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Marlin RGB LED general support
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(NEOPIXEL_LED)

#include "neopixel.h"

#if ENABLED(NEOPIXEL_STARTUP_TEST)
  #include "../../core/utility.h"
#endif

Adafruit_NeoPixel pixels(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800);
#if HAS_TWO_NEOPIXEL
  Adafruit_NeoPixel pixels2(NEOPIXEL_PIXELS, NEOPIXEL2_PIN, NEOPIXEL2_TYPE + NEO_KHZ800);
#endif

#ifdef NEOPIXEL_BKGD_LED_INDEX
  void set_neopixel_color_background() {
    uint8_t background_color[4] = NEOPIXEL_BKGD_COLOR;
    pixels.setPixelColor(NEOPIXEL_BKGD_LED_INDEX, pixels.Color(background_color[0], background_color[1], background_color[2], background_color[3]));
    #if HAS_TWO_NEOPIXEL
      pixels2.setPixelColor(NEOPIXEL_BKGD_LED_INDEX, pixels.Color(background_color[0], background_color[1], background_color[2], background_color[3]));
    #endif
  }
#endif

void set_neopixel_color(const uint32_t color) {
  for (uint16_t i = 0; i < pixels.numPixels(); ++i) {
    #ifdef NEOPIXEL_BKGD_LED_INDEX
      if (i == NEOPIXEL_BKGD_LED_INDEX && color != 0x000000) {
        set_neopixel_color_background();
        continue;
      }
    #endif
    pixels.setPixelColor(i, color);
    #if HAS_TWO_NEOPIXEL
      pixels2.setPixelColor(i, color);
    #endif
  }
  NEOPIXELSHOW;
}

void set_neopixel_color_startup(const uint32_t color) {
  for (uint16_t i = 0; i < pixels.numPixels(); ++i) {
    pixels.setPixelColor(i, color);
    #if HAS_TWO_NEOPIXEL
      pixels2.setPixelColor(i, color);
    #endif
  }
  NEOPIXELSHOW;
}

void setup_neopixel() {
  SET_OUTPUT(NEOPIXEL_PIN);
  pixels.setBrightness(NEOPIXEL_BRIGHTNESS); // 0 - 255 range
  pixels.begin();
  #if HAS_TWO_NEOPIXEL
    pixels2.setBrightness(NEOPIXEL_BRIGHTNESS); // 0 - 255 range
    pixels2.begin();
  #endif
  NEOPIXELSHOW;   // initialize to all off

  #if ENABLED(NEOPIXEL_STARTUP_TEST)
    safe_delay(1000);
    set_neopixel_color_startup(pixels.Color(255, 0, 0, 0));  // red
    safe_delay(1000);
    set_neopixel_color_startup(pixels.Color(0, 255, 0, 0));  // green
    safe_delay(1000);
    set_neopixel_color_startup(pixels.Color(0, 0, 255, 0));  // blue
    safe_delay(1000);
  #endif

  #ifdef NEOPIXEL_BKGD_LED_INDEX
    set_neopixel_color_background();
  #endif

  #if ENABLED(LED_USER_PRESET_STARTUP)
    set_neopixel_color(pixels.Color(LED_USER_PRESET_RED, LED_USER_PRESET_GREEN, LED_USER_PRESET_BLUE, LED_USER_PRESET_WHITE));
  #else
    set_neopixel_color(pixels.Color(0, 0, 0, 0));
  #endif
}

#if 0
bool neopixel_set_led_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w, const uint8_t p) {
  const uint32_t color = pixels.Color(r, g, b, w);
  pixels.setBrightness(p);
  #if DISABLED(NEOPIXEL_IS_SEQUENTIAL)
    set_neopixel_color(color);
    return false;
  #else
    static uint16_t nextLed = 0;
    pixels.setPixelColor(nextLed, color);
    #if HAS_TWO_NEOPIXEL
      pixels2.setPixelColor(nextLed, color);
    #endif
    NEOPIXELSHOW;
    if (++nextLed >= pixels.numPixels()) nextLed = 0;
    return true;
  #endif
}
#endif

#endif // NEOPIXEL_LED
