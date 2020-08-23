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
#pragma once

/**
 * NeoPixel support
 */

// ------------------------
// Includes
// ------------------------

#include "../../inc/MarlinConfig.h"

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

// ------------------------
// Defines
// ------------------------

#if defined(NEOPIXEL2_TYPE) && NEOPIXEL2_TYPE != NEOPIXEL_TYPE
  #define MULTIPLE_NEOPIXEL_TYPES 1
#endif

#if NEOPIXEL_TYPE == NEO_RGB || NEOPIXEL_TYPE == NEO_RBG || NEOPIXEL_TYPE == NEO_GRB || NEOPIXEL_TYPE == NEO_GBR || NEOPIXEL_TYPE == NEO_BRG || NEOPIXEL_TYPE == NEO_BGR
  #define NEOPIXEL_IS_RGB 1
#else
  #define NEOPIXEL_IS_RGBW 1
#endif

#if NEOPIXEL_IS_RGB
  #define NEO_WHITE 255, 255, 255, 0
#else
  #define NEO_WHITE 0, 0, 0, 255
#endif

// ------------------------
// Function prototypes
// ------------------------

class Marlin_NeoPixel {
private:
  static Adafruit_NeoPixel adaneo1
    #if EITHER(MULTIPLE_NEOPIXEL_TYPES, NEOPIXEL2_INSERIES)
      , adaneo2
    #endif
  ;
  static int8_t neoindex;

public:
  static void init();
  static void set_color_startup(const uint32_t c);

  static void set_color(const uint32_t c);

  FORCE_INLINE static void set_neo_index(const int8_t neoIndex) { neoindex = neoIndex; }
  FORCE_INLINE static int8_t get_neo_index() { return neoindex; }

  #ifdef NEOPIXEL_BKGD_LED_INDEX
    static void set_color_background();
  #endif

  static inline void begin() {
    adaneo1.begin();
    #if ENABLED(NEOPIXEL2_INSERIES)
      adaneo2.begin();
    #else
      TERN_(MULTIPLE_NEOPIXEL_TYPES, adaneo2.begin());
    #endif
  }

  static inline void set_pixel_color(const uint16_t n, const uint32_t c) {
    #if ENABLED(NEOPIXEL2_INSERIES)
      if (n >= NEOPIXEL_PIXELS) adaneo2.setPixelColor(n - (NEOPIXEL_PIXELS), c);
      else adaneo1.setPixelColor(n, c);
    #else
      adaneo1.setPixelColor(n, c);
      TERN_(MULTIPLE_NEOPIXEL_TYPES, adaneo2.setPixelColor(n, c));
    #endif
  }

  static inline void set_brightness(const uint8_t b) {
    adaneo1.setBrightness(b);
    #if ENABLED(NEOPIXEL2_INSERIES)
      adaneo2.setBrightness(b);
    #else
      TERN_(MULTIPLE_NEOPIXEL_TYPES, adaneo2.setBrightness(b));
    #endif
  }

  static inline void show() {
    adaneo1.show();
    #if PIN_EXISTS(NEOPIXEL2)
      #if EITHER(MULTIPLE_NEOPIXEL_TYPES, NEOPIXEL2_INSERIES)
        adaneo2.show();
      #else
        adaneo1.setPin(NEOPIXEL2_PIN);
        adaneo1.show();
        adaneo1.setPin(NEOPIXEL_PIN);
      #endif
    #endif
  }

  #if 0
    bool set_led_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w, const uint8_t p);
  #endif

  // Accessors
  static inline uint16_t pixels() { TERN(NEOPIXEL2_INSERIES, return adaneo1.numPixels() * 2, return adaneo1.numPixels()); }
  static inline uint8_t brightness() { return adaneo1.getBrightness(); }
  static inline uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return adaneo1.Color(r, g, b, w);
  }
};

extern Marlin_NeoPixel neo;
