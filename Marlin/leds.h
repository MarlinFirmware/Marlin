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
 * leds.h - Marlin general RGB LED support
 */

#ifndef __LEDS_H__
#define __LEDS_H__

#include "MarlinConfig.h"

#if ENABLED(NEOPIXEL_LED)
  #include "neopixel.h"
#endif

#define HAS_WHITE_LED (ENABLED(RGBW_LED) || ENABLED(NEOPIXEL_LED) || ENABLED(CASE_LIGHT_USE_NEOPIXEL))

/**
 * LEDcolor type for use with leds.set_color
 */
typedef struct LEDColor {
  uint8_t r, g, b
    #if HAS_WHITE_LED
      , w
      #if ENABLED(NEOPIXEL_LED)
        , i
      #endif
    #endif
  ;
  LEDColor() : r(255), g(255), b(255)
    #if HAS_WHITE_LED
      , w(255)
      #if ENABLED(NEOPIXEL_LED)
        , i(NEOPIXEL_BRIGHTNESS)
      #endif
    #endif
  {}
  LEDColor(uint8_t r, uint8_t g, uint8_t b
    #if HAS_WHITE_LED
      , uint8_t w=0
      #if ENABLED(NEOPIXEL_LED)
        , uint8_t i=NEOPIXEL_BRIGHTNESS
      #endif
    #endif
    ) : r(r), g(g), b(b)
    #if HAS_WHITE_LED
      , w(w)
      #if ENABLED(NEOPIXEL_LED)
        , i(i)
      #endif
    #endif
  {}
  LEDColor(const uint8_t (&rgbw)[4]) : r(rgbw[0]), g(rgbw[1]), b(rgbw[2])
    #if HAS_WHITE_LED
      , w(rgbw[3])
      #if ENABLED(NEOPIXEL_LED)
        , i(NEOPIXEL_BRIGHTNESS)
      #endif
    #endif
  {}
  LEDColor& operator=(const uint8_t (&rgbw)[4]) {
    r = rgbw[0]; g = rgbw[1]; b = rgbw[2];
    #if HAS_WHITE_LED
      w = rgbw[3];
    #endif
    return *this;
  }
  LEDColor& operator=(const LEDColor &right) {
    if (this != &right) memcpy(this, &right, sizeof(LEDColor));
    return *this;
  }
  bool operator==(const LEDColor &right) {
    if (this == &right) return true;
    return 0 == memcmp(this, &right, sizeof(LEDColor));
  }
  bool operator!=(const LEDColor &right) { return !operator==(right); }
  bool is_off() const {
    return 3 > r + g + b
      #if HAS_WHITE_LED
        + w
      #endif
    ;
  }
} LEDColor;

/**
 * Color helpers and presets
 */
#if HAS_WHITE_LED
  #define LEDColorWhite() LEDColor(0, 0, 0, 255)
  #if ENABLED(NEOPIXEL_LED)
    #define MakeLEDColor(R,G,B,W,I) LEDColor(R, G, B, W, I)
  #else
    #define MakeLEDColor(R,G,B,W,I) LEDColor(R, G, B, W)
  #endif
#else
  #define MakeLEDColor(R,G,B,W,I) LEDColor(R, G, B)
  #define LEDColorWhite() LEDColor(255, 255, 255)
#endif
#define LEDColorDefault() LEDColor(LED_DEFAULT_COLOR)
#define LEDColorOff()     LEDColor(  0,   0,   0)
#define LEDColorRed()     LEDColor(255,   0,   0)
#define LEDColorOrange()  LEDColor(255,  80,   0)
#define LEDColorYellow()  LEDColor(255, 255,   0)
#define LEDColorGreen()   LEDColor(  0, 255,   0)
#define LEDColorBlue()    LEDColor(  0,   0, 255)
#define LEDColorIndigo()  LEDColor(  0, 255, 255)
#define LEDColorViolet()  LEDColor(255,   0, 255)

class LEDLights {
public:
  LEDLights() {} // ctor

  static void setup(); // init()

  static void set_color(const LEDColor &color
    #if ENABLED(NEOPIXEL_LED)
      , bool isSequence=false
    #endif
  );

  FORCE_INLINE void set_color(uint8_t r, uint8_t g, uint8_t b
    #if HAS_WHITE_LED
      , uint8_t w=0
      #if ENABLED(NEOPIXEL_LED)
        , uint8_t i=NEOPIXEL_BRIGHTNESS
      #endif
    #endif
    #if ENABLED(NEOPIXEL_LED)
      , bool isSequence=false
    #endif
  ) {
    set_color(MakeLEDColor(r, g, b, w, i)
      #if ENABLED(NEOPIXEL_LED)
        , isSequence
      #endif
    );
  }

  static void set_white();
  FORCE_INLINE static void set_off()   { set_color(LEDColorOff()); }
  FORCE_INLINE static void set_green() { set_color(LEDColorGreen()); }

  #if ENABLED(LED_COLOR_PRESETS)
    FORCE_INLINE static void set_default()  { set_color(LEDColorDefault()); }
    FORCE_INLINE static void set_red()      { set_color(LEDColorRed()); }
    FORCE_INLINE static void set_orange()   { set_color(LEDColorOrange()); }
    FORCE_INLINE static void set_yellow()   { set_color(LEDColorYellow()); }
    FORCE_INLINE static void set_blue()     { set_color(LEDColorBlue()); }
    FORCE_INLINE static void set_indigo()   { set_color(LEDColorIndigo()); }
    FORCE_INLINE static void set_violet()   { set_color(LEDColorViolet()); }
  #endif

  #if ENABLED(LED_CONTROL_MENU)
    static LEDColor color; // last non-off color
    static bool lights_on; // the last set color was "on"
    static void toggle();  // swap "off" with color
    FORCE_INLINE static void update() { set_color(color); }
  #endif
};


#if ENABLED(CASE_LIGHT_USE_NEOPIXEL_EXCLUSIVE) || ENABLED(CASE_LIGHT_USE_NEOPIXEL_SPLIT) 
  typedef struct CaselightColor {
    uint8_t r, g, b, w, i;
    CaselightColor() : r(255), g(255), b(255), w(255), i(NEOPIXEL_BRIGHTNESS) {}
    CaselightColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0, uint8_t i=NEOPIXEL_BRIGHTNESS ) : r(r), g(g), b(b), w(w), i(i) {}
    CaselightColor(const uint8_t (&rgbw)[4]) : r(rgbw[0]), g(rgbw[1]), b(rgbw[2]), w(rgbw[3]), i(NEOPIXEL_BRIGHTNESS) {}
    CaselightColor& operator=(const uint8_t (&rgbw)[4]) {
      r = rgbw[0]; g = rgbw[1]; b = rgbw[2]; w = rgbw[3];
      return *this;
    }
    CaselightColor& operator=(const CaselightColor &right) {
      if (this != &right) memcpy(this, &right, sizeof(CaselightColor));
      return *this;
    }
    bool operator==(const CaselightColor &right) {
      if (this == &right) return true;
      return 0 == memcmp(this, &right, sizeof(CaselightColor));
    }
    bool operator!=(const CaselightColor &right) { return !operator==(right); }
    bool is_off() const {
      return 3 > r + g + b + w ;
    }
  } CaselightColor;
  /**
   * Color helpers and presets
   */
 
  #define MakeCaselightColor(R,G,B,W,I) CaselightColor(R, G, B, W, I)
  #define CaselightColorDefault() CaselightColor(CASE_LIGHT_NEOPIXEL_COLOR)
  #define CaselightColorWhite() CaselightColor(0, 0, 0, 255)
  #define CaselightColorOff()     CaselightColor(  0,   0,   0,  0)
  #define CaselightColorRed()     CaselightColor(255,   0,   0)
  #define CaselightColorOrange()  CaselightColor(255,  80,   0)
  #define CaselightColorYellow()  CaselightColor(255, 255,   0)
  #define CaselightColorGreen()   CaselightColor(  0, 255,   0)
  #define CaselightColorBlue()    CaselightColor(  0,   0, 255)
  #define CaselightColorIndigo()  CaselightColor(  0, 255, 255)
  #define CaselightColorViolet()  CaselightColor(255,   0, 255)

  class CaselightLights {
    public:
      CaselightLights() {} 
      static void setup(); // init()
      static void set_color(const CaselightColor &color, bool isSequence=false );
      FORCE_INLINE void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0, uint8_t i=NEOPIXEL_BRIGHTNESS, bool isSequence=false) {
        set_color(MakeCaselightColor(r, g, b, w, i) , isSequence );
      }
      static void set_white();
      FORCE_INLINE static void set_off()   { set_color(CaselightColorOff()); }
      FORCE_INLINE static void set_default()  { set_color(CaselightColorDefault()); }
      #if ENABLED(CASE_LIGHT_COLOR_PRESETS)
        static const CaselightColor defaultCaselightColor;
        FORCE_INLINE static void set_red()      { set_color(CaselightColorRed()); }
        FORCE_INLINE static void set_orange()   { set_color(CaselightColorOrange()); }
        FORCE_INLINE static void set_yellow()   { set_color(CaselightColorYellow()); }
        FORCE_INLINE static void set_blue()     { set_color(CaselightColorBlue()); }
        FORCE_INLINE static void set_green() { set_color(CaselightColorGreen()); }
        FORCE_INLINE static void set_indigo()   { set_color(CaselightColorIndigo()); }
        FORCE_INLINE static void set_violet()   { set_color(CaselightColorViolet()); }
      #endif
      #if ENABLED(CASE_LIGHT_COLOR_CONTROL_MENU) 
        static CaselightColor color; // last non-off color
        static bool lights_on; // the last set color was "on"
        static void toggle();  // swap "off" with color
        FORCE_INLINE static void update() { set_color(color); }
      #endif
  };
  extern CaselightLights caselight;
#endif
extern LEDLights leds;

#endif // __LEDS_H__
