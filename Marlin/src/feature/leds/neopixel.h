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

#ifndef _NEOPIXEL_INCLUDE_
  #error "Always include 'leds.h' and not 'neopixel.h' directly."
#endif

// ------------------------
// Includes
// ------------------------

#include "../../inc/MarlinConfig.h"

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

// ------------------------
// Defines
// ------------------------

#define _NEO_IS_RGB(N) (N == NEO_RGB || N == NEO_RBG || N == NEO_GRB || N == NEO_GBR || N == NEO_BRG || N == NEO_BGR)

#if !_NEO_IS_RGB(NEOPIXEL_TYPE)
  #define HAS_WHITE_LED 1
#endif

#if HAS_WHITE_LED
  #define NEO_WHITE 0, 0, 0, 255
#else
  #define NEO_WHITE 255, 255, 255
#endif

#if defined(NEOPIXEL2_TYPE) && NEOPIXEL2_TYPE != NEOPIXEL_TYPE && DISABLED(NEOPIXEL2_SEPARATE)
  #define MULTIPLE_NEOPIXEL_TYPES 1
#endif

#if EITHER(MULTIPLE_NEOPIXEL_TYPES, NEOPIXEL2_INSERIES)
  #define CONJOINED_NEOPIXEL 1
#endif

// ------------------------
// Types
// ------------------------

typedef value_t(TERN0(NEOPIXEL_LED, NEOPIXEL_PIXELS)) pixel_index_t;

// ------------------------
// Classes
// ------------------------

class Marlin_NeoPixel {
private:
  static Adafruit_NeoPixel adaneo1;
  #if CONJOINED_NEOPIXEL
    static Adafruit_NeoPixel adaneo2;
  #endif

public:
  static pixel_index_t neoindex;

  static void init();
  static void set_color_startup(const uint32_t c);

  static void set_color(const uint32_t c);

  #ifdef NEOPIXEL_BKGD_INDEX_FIRST
    static void set_background_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w);
    static void set_background_color(const uint8_t (&rgbw)[4]) { set_background_color(rgbw[0], rgbw[1], rgbw[2], rgbw[3]); }
    static void reset_background_color();
    static void set_background_off();
  #endif

  static void begin() {
    adaneo1.begin();
    TERN_(CONJOINED_NEOPIXEL, adaneo2.begin());
  }

  static void set_pixel_color(const uint16_t n, const uint32_t c) {
    #if ENABLED(NEOPIXEL2_INSERIES)
      if (n >= NEOPIXEL_PIXELS) adaneo2.setPixelColor(n - (NEOPIXEL_PIXELS), c);
      else adaneo1.setPixelColor(n, c);
    #else
      adaneo1.setPixelColor(n, c);
      TERN_(MULTIPLE_NEOPIXEL_TYPES, adaneo2.setPixelColor(n, c));
    #endif
  }

  static void set_brightness(const uint8_t b) {
    adaneo1.setBrightness(b);
    TERN_(CONJOINED_NEOPIXEL, adaneo2.setBrightness(b));
  }

  static void show() {
    // Some platforms cannot maintain PWM output when NeoPixel disables interrupts for long durations.
    TERN_(HAS_PAUSE_SERVO_OUTPUT, PAUSE_SERVO_OUTPUT());
    adaneo1.show();
    #if PIN_EXISTS(NEOPIXEL2)
      #if CONJOINED_NEOPIXEL
        adaneo2.show();
      #else
        adaneo1.show();
        adaneo1.setPin(NEOPIXEL_PIN);
      #endif
    #endif
    TERN_(HAS_PAUSE_SERVO_OUTPUT, RESUME_SERVO_OUTPUT());
  }

  // Accessors
  static uint16_t pixels() { return adaneo1.numPixels() * TERN1(NEOPIXEL2_INSERIES, 2); }

  static uint32_t pixel_color(const uint16_t n) {
    #if ENABLED(NEOPIXEL2_INSERIES)
      if (n >= NEOPIXEL_PIXELS) return adaneo2.getPixelColor(n - (NEOPIXEL_PIXELS));
    #endif
    return adaneo1.getPixelColor(n);
  }

  static uint8_t brightness() { return adaneo1.getBrightness(); }

  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b OPTARG(HAS_WHITE_LED, uint8_t w)) {
    return adaneo1.Color(r, g, b OPTARG(HAS_WHITE_LED, w));
  }
};

extern Marlin_NeoPixel neo;

// Neo pixel channel 2
#if ENABLED(NEOPIXEL2_SEPARATE)

  #if _NEO_IS_RGB(NEOPIXEL2_TYPE)
    #define NEOPIXEL2_IS_RGB 1
    #define NEO2_WHITE 255, 255, 255
  #else
    #define NEOPIXEL2_IS_RGBW 1
    #define HAS_WHITE_LED2 1      // A white component can be passed for NEOPIXEL2
    #define NEO2_WHITE 0, 0, 0, 255
  #endif

  class Marlin_NeoPixel2 {
  private:
    static Adafruit_NeoPixel adaneo;

  public:
    static pixel_index_t neoindex;

    static void init();
    static void set_color_startup(const uint32_t c);

    static void set_color(const uint32_t c);

    static void begin() { adaneo.begin(); }
    static void set_pixel_color(const uint16_t n, const uint32_t c) { adaneo.setPixelColor(n, c); }
    static void set_brightness(const uint8_t b) { adaneo.setBrightness(b); }
    static void show() {
      adaneo.show();
      adaneo.setPin(NEOPIXEL2_PIN);
    }

    // Accessors
    static uint16_t pixels() { return adaneo.numPixels();}
    static uint32_t pixel_color(const uint16_t n) { return adaneo.getPixelColor(n); }
    static uint8_t brightness() { return adaneo.getBrightness(); }
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b OPTARG(HAS_WHITE_LED2, uint8_t w)) {
      return adaneo.Color(r, g, b OPTARG(HAS_WHITE_LED2, w));
    }
  };

  extern Marlin_NeoPixel2 neo2;

#endif // NEOPIXEL2_SEPARATE

#undef _NEO_IS_RGB
