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
 * neopixel.cpp
 */

#include "MarlinConfig.h"

#if ENABLED(NEOPIXEL_LED)||ENABLED(CASE_LIGHT_USE_NEOPIXEL_EXCLUSIV)

#include "neopixel.h"

#if ENABLED(NEOPIXEL_STARTUP_TEST)
  #include "utility.h"
#endif

#if ENABLED(CASE_LIGHT_USE_NEOPIXEL_SPLIT)
  Adafruit_NeoPixel pixels(NEOPIXEL_PIXELS + NEOPIXEL_CASE_LIGHT_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800);
#else
  Adafruit_NeoPixel pixels(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800);
#endif
#if ENABLED(CASE_LIGHT_USE_NEOPIXEL_EXCLUSIV)
  Adafruit_NeoPixel pixels_cl(NEOPIXEL_CASE_LIGHT_PIXELS, NEOPIXEL_CASE_LIGHT_PIN, NEOPIXEL_TYPE + NEO_KHZ800);
#endif

void set_neopixel_color(const uint32_t color) {
  for (uint16_t i = 0; i < NEOPIXEL_PIXELS; ++i)
    pixels.setPixelColor(i, color);
  pixels.show();
}

#if ENABLED(CASE_LIGHT_USE_NEOPIXEL)
  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL_SPLIT)
    void set_neopixel_cl_color(const uint32_t color) {
      for (uint16_t i = NEOPIXEL_PIXELS; i < NEOPIXEL_PIXELS + NEOPIXEL_CASE_LIGHT_PIXELS; ++i)
        pixels.setPixelColor(i, color);
      pixels.show();
    }
  #elif ENABLED(CASE_LIGHT_USE_NEOPIXEL_EXCLUSIVE)
    void set_neopixel_cl_color(const uint32_t color) {
      for (uint16_t i = 0; i < NEOPIXEL_CASE_LIGHT_PIXELS; ++i)
        pixels_cl.setPixelColor(i, color);
      pixels_cl.show();
    }
  #else 
    void set_neopixel_cl_color(const uint32_t color) {
      set_neopixel_color(color);
    }
  #endif
#endif
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
	#if ENABLED(CASE_LIGHT_USE_NEOPIXEL_SPLIT)
      set_neopixel_cl_color(pixels.Color(255, 0, 0, 0));  // red
      safe_delay(1000);
      set_neopixel_cl_color(pixels.Color(0, 255, 0, 0));  // green
      safe_delay(1000);
      set_neopixel_cl_color(pixels.Color(0, 0, 255, 0));  // blue
      safe_delay(1000);
	#endif
  #endif
  set_neopixel_color(pixels.Color(NEO_WHITE));       // white
  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL_SPLIT)
  set_neopixel_cl_color(pixels.Color(NEO_WHITE));       // white
  #endif
  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL_EXCLUSIV)
    SET_OUTPUT(NEOPIXEL_CASE_LIGHT_PIN);
    pixels_cl.setBrightness(NEOPIXEL_BRIGHTNESS); // 0 - 255 range
    pixels_cl.begin();
    pixels_cl.show(); // initialize to all off
    #if ENABLED(NEOPIXEL_STARTUP_TEST)
      safe_delay(1000);
      set_neopixel_cl_color(pixels.Color(255, 0, 0, 0));  // red
      safe_delay(1000);
      set_neopixel_cl_color(pixels.Color(0, 255, 0, 0));  // green
      safe_delay(1000);
      set_neopixel_cl_color(pixels.Color(0, 0, 255, 0));  // blue
      safe_delay(1000);
    #endif
      set_neopixel_cl_color(pixels.Color(NEO_WHITE));       // white
   #endif
}

#endif // NEOPIXEL_LED