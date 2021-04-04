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
 * Standard Marlin Status Screen bitmaps
 *
 * Use the Marlin Bitmap Converter to make your own:
 * https://marlinfw.org/tools/u8glib/converter.html
 */

#include "../../inc/MarlinConfig.h"
#include "marlinui_DOGM.h"

#define BW(N) ((N + 7) / 8)

#if ENABLED(CUSTOM_STATUS_SCREEN_IMAGE) && DISABLED(STATUS_COMBINE_HEATERS)

  #undef STATUS_HEATERS_X
  #undef STATUS_BED_X
  /**
   * Custom _Statusscreen.h files can define:
   * - A custom logo image
   * - A custom heater bitmap
   * - A custom fan bitmap / animation
   *
   * See the included examples for guidance
   */
  #include "../../../_Statusscreen.h"

  #ifdef STATUS_SCREENWIDTH
    #error "Your custom _Statusscreen.h needs to be converted for Marlin 2.0."
  #endif

#endif

//
// Default Status Screen Heater or Hotends bitmaps
//
#if !STATUS_HEATERS_WIDTH && !STATUS_HOTEND1_WIDTH
  #if ENABLED(STATUS_COMBINE_HEATERS)
    #include "status/combined.h"
  #else
    #if HAS_HOTEND
      #include "status/hotend.h"
    #else
      #define STATUS_HEATERS_HEIGHT 20
    #endif
  #endif
#endif

//
// Laser / Spindle
//
#if !STATUS_CUTTER_WIDTH && HAS_CUTTER
  #include "status/cutter.h"
#endif
#ifndef STATUS_CUTTER_WIDTH
  #define STATUS_CUTTER_WIDTH 0
#endif
  #ifndef STATUS_CUTTER_BYTEWIDTH
    #define STATUS_CUTTER_BYTEWIDTH BW(STATUS_CUTTER_WIDTH)
  #endif

//
// Laser cooler
//
#if !STATUS_COOLER_WIDTH && HAS_COOLER
  #include "status/cooler.h"
#endif
#ifndef STATUS_COOLER_WIDTH
  #define STATUS_COOLER_WIDTH 0
#endif
#ifndef STATUS_COOLER_BYTEWIDTH
  #define STATUS_COOLER_BYTEWIDTH BW(STATUS_COOLER_WIDTH)
#endif

//
// Laser Flowmeter
//
#if !STATUS_FLOWMETER_WIDTH && ENABLED(LASER_COOLANT_FLOW_METER)
  #include "status/cooler.h"
#endif
#ifndef STATUS_FLOWMETER_WIDTH
  #define STATUS_FLOWMETER_WIDTH 0
#endif
#ifndef STATUS_FLOWMETER_BYTEWIDTH
  #define STATUS_FLOWMETER_BYTEWIDTH BW(STATUS_FLOWMETER_WIDTH)
#endif



//
// Bed
//
#if !STATUS_BED_WIDTH && HAS_HEATED_BED && DISABLED(STATUS_COMBINE_HEATERS)
  #include "status/bed.h"
#endif
#ifndef STATUS_BED_WIDTH
  #define STATUS_BED_WIDTH 0
#endif

//
// Chamber
//
#if !STATUS_CHAMBER_WIDTH && HAS_TEMP_CHAMBER && ((HOTENDS <= 4 && !HAS_HEATED_BED) || (HOTENDS <= 3 && HAS_HEATED_BED))
  #include "status/chamber.h"
#endif
#ifndef STATUS_CHAMBER_WIDTH
  #define STATUS_CHAMBER_WIDTH 0
#endif

// Can also be overridden in Configuration_adv.h
// If you can afford it, try the 3-frame fan animation!
// Don't compile in the fan animation with no fan
#if !HAS_FAN0 || (HOTENDS == 5 || (HOTENDS == 4 && BED_OR_CHAMBER) || BOTH(STATUS_COMBINE_HEATERS, HAS_HEATED_CHAMBER))
  #undef STATUS_FAN_FRAMES
#elif !STATUS_FAN_FRAMES
  #define STATUS_FAN_FRAMES 2
#elif STATUS_FAN_FRAMES > 4
  #error "A maximum of 4 fan animation frames is currently supported."
#endif

#if HOTENDS > 4
  #undef STATUS_LOGO_WIDTH
  #undef STATUS_HEATERS_XSPACE
  #define STATUS_HEATERS_XSPACE 24
#endif

//
// Provide default Fan Bitmaps
//
#if !STATUS_FAN_WIDTH && STATUS_FAN_FRAMES > 0
  #include "status/fan.h"
#else
  #undef STATUS_FAN_FRAMES
  #define STATUS_FAN_WIDTH 0
#endif

#define _EXTRA_WIDTH (STATUS_FAN_WIDTH + STATUS_CHAMBER_WIDTH + STATUS_BED_WIDTH)

//
// Heater Bitmap X Space Requirements
//
#if !defined(STATUS_HEATERS_XSPACE) && (STATUS_HOTEND1_WIDTH || STATUS_HEATERS_WIDTH)
  #if (HOTENDS == 3 || HOTENDS == 4) && ENABLED(STATUS_COMBINE_HEATERS)
    // If more heaters or they're combined, 3 bytes
    #define STATUS_HEATERS_XSPACE 24
  #elif STATUS_LOGO_WIDTH > (LCD_PIXEL_WIDTH - (_EXTRA_WIDTH) - 26 * (HOTENDS)) // 128 - (20 + 24 + 26) == 58
    // If the logo won't fit at 26 width
    #define STATUS_HEATERS_XSPACE 24
  #else
    #define STATUS_HEATERS_XSPACE 26
  #endif
#endif

#if ENABLED(CUSTOM_STATUS_SCREEN_IMAGE)
  //
  // Disable the logo bitmap if insufficient space
  //
  #if STATUS_HEATERS_XSPACE
    #define _HEATERS_WIDTH (HOTENDS * (STATUS_HEATERS_XSPACE)) // as many hotends as possible
  #elif STATUS_HEATERS_WIDTH
    #define _HEATERS_WIDTH STATUS_HEATERS_WIDTH
  #elif HOTENDS
    #error "Status screen heaters region was not specified."
  #endif
  #if STATUS_LOGO_WIDTH > (LCD_PIXEL_WIDTH - (_EXTRA_WIDTH + _HEATERS_WIDTH))
    #warning "Unable to fit custom Status Screen logo. Disabling."
    #undef STATUS_LOGO_WIDTH
  #endif

  #if !defined(STATUS_HEATERS_X) && ((HAS_HOTEND && STATUS_LOGO_WIDTH && BED_OR_CHAMBER_OR_FAN) || (HOTENDS >= 3 && !BED_OR_CHAMBER_OR_FAN))
    #define _STATUS_HEATERS_X(H,S,N) ((LCD_PIXEL_WIDTH - (H * (S + N)) - (_EXTRA_WIDTH) + (STATUS_LOGO_WIDTH)) / 2)
    #if STATUS_HOTEND1_WIDTH
      #if HOTENDS > 2
        #define STATUS_HEATERS_X _STATUS_HEATERS_X(HOTENDS, STATUS_HOTEND1_WIDTH, 6)
      #else
        #define STATUS_HEATERS_X _STATUS_HEATERS_X(HOTENDS, STATUS_HOTEND1_WIDTH, 4)
      #endif
    #else
      #define STATUS_HEATERS_X _STATUS_HEATERS_X(1, STATUS_HEATERS_WIDTH, 4)
    #endif
  #endif
#endif

//
// Custom Logo Bitmap Properties
//
#ifndef STATUS_LOGO_WIDTH
  #define STATUS_LOGO_WIDTH 0
#endif
#ifndef STATUS_LOGO_BYTEWIDTH
  #define STATUS_LOGO_BYTEWIDTH BW(STATUS_LOGO_WIDTH)
#endif
#if STATUS_LOGO_WIDTH
  #ifndef STATUS_LOGO_X
    #ifdef STATUS_HEATERS_X
      #define STATUS_LOGO_X ((STATUS_HEATERS_X - (STATUS_LOGO_WIDTH) - 1) / 2)
    #else
      #define STATUS_LOGO_X 0
    #endif
  #endif
  #ifndef STATUS_LOGO_HEIGHT
    #define STATUS_LOGO_HEIGHT (sizeof(status_logo_bmp) / (STATUS_LOGO_BYTEWIDTH))
  #endif
  #ifndef STATUS_LOGO_Y
    #define STATUS_LOGO_Y _MAX(0U, (28U - _MIN(28U, STATUS_LOGO_HEIGHT)) / 2U)
  #endif
  static_assert(
    sizeof(status_logo_bmp) == (STATUS_LOGO_BYTEWIDTH) * (STATUS_LOGO_HEIGHT),
    "Status logo bitmap (status_logo_bmp) dimensions don't match data."
  );
#endif

//
// Hotend Heater Bitmap starting X position
//
#if !defined(STATUS_HEATERS_X) && (STATUS_HOTEND1_WIDTH || STATUS_HEATERS_WIDTH)
  #if STATUS_LOGO_BYTEWIDTH
    #define STATUS_HEATERS_X (STATUS_LOGO_BYTEWIDTH * 8)
  #elif ((STATUS_CHAMBER_WIDTH || STATUS_FAN_WIDTH) && (STATUS_BED_WIDTH  && STATUS_HOTEND_BITMAPS == 3)) || \
        ((STATUS_CHAMBER_WIDTH || STATUS_FAN_WIDTH  ||  STATUS_BED_WIDTH) && STATUS_HOTEND_BITMAPS == 4)
    #define STATUS_HEATERS_X 5
  #else
    #if BOTH(STATUS_COMBINE_HEATERS, HAS_HEATED_BED) && HOTENDS <= 4
      #define STATUS_HEATERS_X 5
    #else
      #define STATUS_HEATERS_X 8 // Like the included bitmaps
    #endif
  #endif
#endif

#if STATUS_HOTEND1_WIDTH

  //
  // Hotend images. A base hotend image and optional "ON" state image.
  //
  #ifndef STATUS_HOTEND_BITMAPS
    #define STATUS_HOTEND_BITMAPS 1
  #endif

  #ifndef STATUS_HOTEND2_WIDTH
    #define STATUS_HOTEND2_WIDTH STATUS_HOTEND1_WIDTH
  #endif
  #ifndef STATUS_HOTEND3_WIDTH
    #define STATUS_HOTEND3_WIDTH STATUS_HOTEND2_WIDTH
  #endif
  #ifndef STATUS_HOTEND4_WIDTH
    #define STATUS_HOTEND4_WIDTH STATUS_HOTEND3_WIDTH
  #endif
  #ifndef STATUS_HOTEND5_WIDTH
    #define STATUS_HOTEND5_WIDTH STATUS_HOTEND4_WIDTH
  #endif
  #ifndef STATUS_HOTEND6_WIDTH
    #define STATUS_HOTEND6_WIDTH STATUS_HOTEND5_WIDTH
  #endif
  #ifndef STATUS_HOTEND7_WIDTH
    #define STATUS_HOTEND7_WIDTH STATUS_HOTEND6_WIDTH
  #endif
  #ifndef STATUS_HOTEND8_WIDTH
    #define STATUS_HOTEND8_WIDTH STATUS_HOTEND7_WIDTH
  #endif

  constexpr uint8_t status_hotend_width[HOTENDS] = ARRAY_N(HOTENDS, STATUS_HOTEND1_WIDTH, STATUS_HOTEND2_WIDTH, STATUS_HOTEND3_WIDTH, STATUS_HOTEND4_WIDTH, STATUS_HOTEND5_WIDTH, STATUS_HOTEND6_WIDTH, STATUS_HOTEND7_WIDTH, STATUS_HOTEND8_WIDTH);
  #define STATUS_HOTEND_WIDTH(N) status_hotend_width[N]

  #ifndef STATUS_HOTEND1_BYTEWIDTH
    #define STATUS_HOTEND1_BYTEWIDTH BW(STATUS_HOTEND1_WIDTH)
  #endif
  #ifndef STATUS_HOTEND2_BYTEWIDTH
    #define STATUS_HOTEND2_BYTEWIDTH BW(STATUS_HOTEND2_WIDTH)
  #endif
  #ifndef STATUS_HOTEND3_BYTEWIDTH
    #define STATUS_HOTEND3_BYTEWIDTH BW(STATUS_HOTEND3_WIDTH)
  #endif
  #ifndef STATUS_HOTEND4_BYTEWIDTH
    #define STATUS_HOTEND4_BYTEWIDTH BW(STATUS_HOTEND4_WIDTH)
  #endif
  #ifndef STATUS_HOTEND5_BYTEWIDTH
    #define STATUS_HOTEND5_BYTEWIDTH BW(STATUS_HOTEND5_WIDTH)
  #endif
  #ifndef STATUS_HOTEND6_BYTEWIDTH
    #define STATUS_HOTEND6_BYTEWIDTH BW(STATUS_HOTEND6_WIDTH)
  #endif
  #ifndef STATUS_HOTEND7_BYTEWIDTH
    #define STATUS_HOTEND7_BYTEWIDTH BW(STATUS_HOTEND7_WIDTH)
  #endif
  #ifndef STATUS_HOTEND8_BYTEWIDTH
    #define STATUS_HOTEND8_BYTEWIDTH BW(STATUS_HOTEND8_WIDTH)
  #endif

  constexpr uint8_t status_hotend_bytewidth[HOTENDS] = ARRAY_N(HOTENDS, STATUS_HOTEND1_BYTEWIDTH, STATUS_HOTEND2_BYTEWIDTH, STATUS_HOTEND3_BYTEWIDTH, STATUS_HOTEND4_BYTEWIDTH, STATUS_HOTEND5_BYTEWIDTH, STATUS_HOTEND6_BYTEWIDTH, STATUS_HOTEND7_BYTEWIDTH, STATUS_HOTEND8_BYTEWIDTH);
  #define STATUS_HOTEND_BYTEWIDTH(N) status_hotend_bytewidth[N]

  #ifndef STATUS_HOTEND1_X
    #define STATUS_HOTEND1_X STATUS_HEATERS_X
  #endif
  #ifndef STATUS_HOTEND2_X
    #define STATUS_HOTEND2_X STATUS_HOTEND1_X + STATUS_HEATERS_XSPACE
  #endif

  #if HOTENDS > 2
    #ifndef STATUS_HOTEND3_X
      #define STATUS_HOTEND3_X STATUS_HOTEND2_X + STATUS_HEATERS_XSPACE
    #endif
    #ifndef STATUS_HOTEND4_X
      #define STATUS_HOTEND4_X STATUS_HOTEND3_X + STATUS_HEATERS_XSPACE
    #endif
    #ifndef STATUS_HOTEND5_X
      #define STATUS_HOTEND5_X STATUS_HOTEND4_X + STATUS_HEATERS_XSPACE
    #endif
    #ifndef STATUS_HOTEND6_X
      #define STATUS_HOTEND6_X STATUS_HOTEND5_X + STATUS_HEATERS_XSPACE
    #endif
    #ifndef STATUS_HOTEND7_X
      #define STATUS_HOTEND7_X STATUS_HOTEND6_X + STATUS_HEATERS_XSPACE
    #endif
    #ifndef STATUS_HOTEND8_X
      #define STATUS_HOTEND8_X STATUS_HOTEND7_X + STATUS_HEATERS_XSPACE
    #endif

    constexpr uint8_t status_hotend_x[HOTENDS] = ARRAY_N(HOTENDS, STATUS_HOTEND1_X, STATUS_HOTEND2_X, STATUS_HOTEND3_X, STATUS_HOTEND4_X, STATUS_HOTEND5_X, STATUS_HOTEND6_X, STATUS_HOTEND7_X, STATUS_HOTEND8_X);
    #define STATUS_HOTEND_X(N) status_hotend_x[N]
  #elif HAS_MULTI_HOTEND
    #define STATUS_HOTEND_X(N) ((N) ? STATUS_HOTEND2_X : STATUS_HOTEND1_X)
  #else
    #define STATUS_HOTEND_X(N) STATUS_HOTEND1_X
  #endif

  #ifndef STATUS_HOTEND_TEXT_X
    #ifdef STATUS_HOTEND1_TEXT_X
      #ifndef STATUS_HOTEND2_TEXT_X
        #define STATUS_HOTEND2_TEXT_X STATUS_HOTEND1_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      #ifndef STATUS_HOTEND3_TEXT_X
        #define STATUS_HOTEND3_TEXT_X STATUS_HOTEND2_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      #ifndef STATUS_HOTEND4_TEXT_X
        #define STATUS_HOTEND4_TEXT_X STATUS_HOTEND3_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      #ifndef STATUS_HOTEND5_TEXT_X
        #define STATUS_HOTEND5_TEXT_X STATUS_HOTEND4_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      #ifndef STATUS_HOTEND6_TEXT_X
        #define STATUS_HOTEND6_TEXT_X STATUS_HOTEND5_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      #ifndef STATUS_HOTEND7_TEXT_X
        #define STATUS_HOTEND7_TEXT_X STATUS_HOTEND6_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      #ifndef STATUS_HOTEND8_TEXT_X
        #define STATUS_HOTEND8_TEXT_X STATUS_HOTEND7_TEXT_X + STATUS_HEATERS_XSPACE
      #endif
      constexpr uint8_t status_hotend_text_x[] = ARRAY_N(HOTENDS, STATUS_HOTEND1_TEXT_X, STATUS_HOTEND2_TEXT_X, STATUS_HOTEND3_TEXT_X, STATUS_HOTEND4_TEXT_X, STATUS_HOTEND5_TEXT_X, STATUS_HOTEND6_TEXT_X, STATUS_HOTEND7_TEXT_X, STATUS_HOTEND8_TEXT_X);
      #define STATUS_HOTEND_TEXT_X(N) status_hotend_text_x[N]
    #else
      #define STATUS_HOTEND_TEXT_X(N) (STATUS_HOTEND1_X + 6 + (N) * (STATUS_HEATERS_XSPACE))
    #endif
  #endif

  #if STATUS_HOTEND_BITMAPS > 1 && DISABLED(STATUS_HOTEND_NUMBERLESS)
    #define TEST_BITMAP_OFF status_hotend1_a_bmp
    #define TEST_BITMAP_ON  status_hotend1_b_bmp
  #else
    #define TEST_BITMAP_OFF status_hotend_a_bmp
    #define TEST_BITMAP_ON  status_hotend_b_bmp
  #endif
  #ifndef STATUS_HEATERS_HEIGHT
    #define STATUS_HEATERS_HEIGHT (sizeof(TEST_BITMAP_OFF) / (STATUS_HOTEND1_BYTEWIDTH))
  #endif
  #ifndef STATUS_HEATERS_Y
    #define STATUS_HEATERS_Y (20 - (STATUS_HEATERS_HEIGHT))
  #endif

  #define HOTEND0_BITMAP_SIZE (STATUS_HOTEND1_BYTEWIDTH) * (STATUS_HEATERS_HEIGHT)
  static_assert(
    sizeof(TEST_BITMAP_OFF) == HOTEND0_BITMAP_SIZE,
    "Status hotend bitmap (" STRINGIFY(TEST_BITMAP_OFF) ") dimensions don't match data."
  );
  #ifdef STATUS_HOTEND_ANIM
    static_assert(
      sizeof(TEST_BITMAP_ON) == HOTEND0_BITMAP_SIZE,
      "Status hotend bitmaps (" STRINGIFY(TEST_BITMAP_OFF) " and " STRINGIFY(TEST_BITMAP_ON) ") dimensions don't match."
    );
  #endif

#elif STATUS_HEATERS_WIDTH

  #ifndef STATUS_HEATERS_XSPACE
    #define STATUS_HEATERS_XSPACE 24
  #endif
  #ifndef STATUS_HOTEND_WIDTH
    #define STATUS_HOTEND_WIDTH(N) 10
  #endif
  #ifndef STATUS_HOTEND_X
    #define STATUS_HOTEND_X(N) (STATUS_HEATERS_X + 2 + (N) * (STATUS_HEATERS_XSPACE))
  #endif
  #ifndef STATUS_HOTEND_TEXT_X
    #if HOTENDS == 4 && STATUS_LOGO_WIDTH
      #define STATUS_HOTEND_TEXT_X(N) (STATUS_HEATERS_X + 6 + (N) * (STATUS_HEATERS_XSPACE))
    #else
      #define STATUS_HOTEND_TEXT_X(N) (STATUS_HEATERS_X + 6 + (N) * (STATUS_HEATERS_XSPACE))
    #endif
  #endif
  #ifndef STATUS_HEATERS_BYTEWIDTH
    #define STATUS_HEATERS_BYTEWIDTH BW(STATUS_HEATERS_WIDTH)
  #endif
  #ifndef STATUS_HEATERS_HEIGHT
    #define STATUS_HEATERS_HEIGHT (sizeof(status_heaters_bmp) / (STATUS_HEATERS_BYTEWIDTH))
  #endif
  #ifndef STATUS_HEATERS_Y
    #define STATUS_HEATERS_Y (20 - (STATUS_HEATERS_HEIGHT))
  #endif

  static_assert(
    sizeof(status_heaters_bmp) == (STATUS_HEATERS_BYTEWIDTH) * (STATUS_HEATERS_HEIGHT),
    "Status heaters bitmap (status_heaters_bmp) dimensions don't match data."
  );

#else // HOTENDS == 0

  #define STATUS_HOTEND_TEXT_X(N) 0
  #define STATUS_HEATERS_Y 0

#endif

//
// Cutter Bitmap Properties
//
#if HAS_CUTTER
  #if STATUS_CUTTER_WIDTH

    #ifndef STATUS_CUTTER_X
      #define STATUS_CUTTER_X (LCD_PIXEL_WIDTH - (STATUS_CUTTER_BYTEWIDTH + STATUS_CUTTER_BYTEWIDTH) * 8)
    #endif

    #ifndef STATUS_CUTTER_HEIGHT
      #ifdef STATUS_CUTTER_ANIM
        #define STATUS_CUTTER_HEIGHT(S) ((S) ? sizeof(status_cutter_on_bmp) / (STATUS_CUTTER_BYTEWIDTH) : sizeof(status_cutter_bmp) / (STATUS_CUTTER_BYTEWIDTH))
      #else
        #define STATUS_CUTTER_HEIGHT(S) (sizeof(status_cutter_bmp) / (STATUS_CUTTER_BYTEWIDTH))
      #endif
    #endif

    #ifndef STATUS_CUTTER_Y
      #define STATUS_CUTTER_Y(S) 4
    #endif

    #ifndef STATUS_CUTTER_TEXT_X
      #define STATUS_CUTTER_TEXT_X (STATUS_CUTTER_X -1)
    #endif

    #ifndef STATUS_CUTTER_TEXT_Y
      #define STATUS_CUTTER_TEXT_Y 28
    #endif

    static_assert(
      sizeof(status_cutter_bmp) == (STATUS_CUTTER_BYTEWIDTH) * (STATUS_CUTTER_HEIGHT(0)),
      "Status cutter bitmap (status_cutter_bmp) dimensions don't match data."
    );
    #ifdef STATUS_CUTTER_ANIM
      static_assert(
        sizeof(status_cutter_on_bmp) == (STATUS_CUTTER_BYTEWIDTH) * (STATUS_CUTTER_HEIGHT(1)),
        "Status cutter bitmap (status_cutter_on_bmp) dimensions don't match data."
      );
    #endif

  #endif
#endif

//
// Chamber Bitmap Properties
//
#ifndef STATUS_CHAMBER_BYTEWIDTH
  #define STATUS_CHAMBER_BYTEWIDTH BW(STATUS_CHAMBER_WIDTH)
#endif
#if STATUS_CHAMBER_WIDTH

  #ifndef STATUS_CHAMBER_X
    #define STATUS_CHAMBER_X (LCD_PIXEL_WIDTH - (STATUS_CHAMBER_BYTEWIDTH + STATUS_FAN_BYTEWIDTH) * 8)
  #endif

  #ifndef STATUS_CHAMBER_HEIGHT
    #ifdef STATUS_CHAMBER_ANIM
      #define STATUS_CHAMBER_HEIGHT(S) ((S) ? sizeof(status_chamber_on_bmp) / (STATUS_CHAMBER_BYTEWIDTH) : sizeof(status_chamber_bmp) / (STATUS_CHAMBER_BYTEWIDTH))
    #else
      #define STATUS_CHAMBER_HEIGHT(S) (sizeof(status_chamber_bmp) / (STATUS_CHAMBER_BYTEWIDTH))
    #endif
  #endif

  #ifndef STATUS_CHAMBER_Y
    #define STATUS_CHAMBER_Y(S) (20 - STATUS_CHAMBER_HEIGHT(S))
  #endif

  #ifndef STATUS_CHAMBER_TEXT_X
    #define STATUS_CHAMBER_TEXT_X (STATUS_CHAMBER_X + 11)
  #endif

  static_assert(
    sizeof(status_chamber_bmp) == (STATUS_CHAMBER_BYTEWIDTH) * (STATUS_CHAMBER_HEIGHT(0)),
    "Status chamber bitmap (status_chamber_bmp) dimensions don't match data."
  );
  #ifdef STATUS_CHAMBER_ANIM
    static_assert(
      sizeof(status_chamber_on_bmp) == (STATUS_CHAMBER_BYTEWIDTH) * (STATUS_CHAMBER_HEIGHT(1)),
      "Status chamber bitmap (status_chamber_on_bmp) dimensions don't match data."
    );
  #endif

#endif

//
// Cooler Bitmap Properties
//
#if HAS_COOLER
  #if STATUS_COOLER_WIDTH

    #ifndef STATUS_COOLER_X
      #define STATUS_COOLER_X (LCD_PIXEL_WIDTH - (STATUS_COOLER_BYTEWIDTH + STATUS_FAN_BYTEWIDTH + STATUS_CUTTER_BYTEWIDTH) * 8)
    #endif

    #ifndef STATUS_COOLER_HEIGHT
      #define STATUS_COOLER_HEIGHT(S) (sizeof(status_cooler_bmp1) / (STATUS_COOLER_BYTEWIDTH))
    #endif

    #ifndef STATUS_COOLER_Y
      #define STATUS_COOLER_Y(S) (18 - STATUS_COOLER_HEIGHT(S))
    #endif

    #ifndef STATUS_COOLER_TEXT_X
      #define STATUS_COOLER_TEXT_X (STATUS_COOLER_X + 8)
    #endif

    static_assert(
      sizeof(status_cooler_bmp1) == (STATUS_COOLER_BYTEWIDTH) * (STATUS_COOLER_HEIGHT(0)),
      "Status cooler bitmap (status_cooler_bmp1) dimensions don't match data."
    );
    #ifdef STATUS_COOLER_ANIM
      static_assert(
        sizeof(status_cooler_bmp2) == (STATUS_COOLER_BYTEWIDTH) * (STATUS_COOLER_HEIGHT(1)),
        "Status cooler bitmap (status_cooler_bmp2) dimensions don't match data."
      );
    #endif

  #endif
#endif

//
//  Flowmeter Bitmap Properties
//
#if ENABLED(LASER_COOLANT_FLOW_METER)
  #if STATUS_FLOWMETER_WIDTH

    #ifndef STATUS_FLOWMETER_X
      #define STATUS_FLOWMETER_X (LCD_PIXEL_WIDTH - (STATUS_FLOWMETER_BYTEWIDTH + STATUS_FAN_BYTEWIDTH + STATUS_CUTTER_BYTEWIDTH + STATUS_COOLER_BYTEWIDTH) * 8)
    #endif

    #ifndef STATUS_FLOWMETER_HEIGHT
      #define STATUS_FLOWMETER_HEIGHT(S) (sizeof(status_flowmeter_bmp1) / (STATUS_FLOWMETER_BYTEWIDTH))
    #endif

    #ifndef STATUS_FLOWMETER_Y
      #define STATUS_FLOWMETER_Y(S) (20 - STATUS_FLOWMETER_HEIGHT(S))
    #endif

    #ifndef STATUS_FLOWMETER_TEXT_X
      #define STATUS_FLOWMETER_TEXT_X (STATUS_FLOWMETER_X + 8)
    #endif

    static_assert(
      sizeof(status_flowmeter_bmp1) == (STATUS_FLOWMETER_BYTEWIDTH) * STATUS_FLOWMETER_HEIGHT(0),
      "Status flowmeter bitmap (status_flowmeter_bmp1) dimensions don't match data."
    );
    #ifdef STATUS_COOLER_ANIM
      static_assert(
        sizeof(status_flowmeter_bmp2) == (STATUS_FLOWMETER_BYTEWIDTH) * STATUS_FLOWMETER_HEIGHT(1),
        "Status flowmeter bitmap (status_flowmeter_bmp2) dimensions don't match data."
      );
    #endif
  #endif
#endif

//
// Bed Bitmap Properties
//
#ifndef STATUS_BED_BYTEWIDTH
  #define STATUS_BED_BYTEWIDTH BW(STATUS_BED_WIDTH)
#endif
#if STATUS_BED_WIDTH && !STATUS_HEATERS_WIDTH

  #ifndef STATUS_BED_X
    #define STATUS_BED_X (LCD_PIXEL_WIDTH - (STATUS_CHAMBER_BYTEWIDTH + STATUS_FAN_BYTEWIDTH + STATUS_BED_BYTEWIDTH) * 8)
  #endif

  #ifndef STATUS_BED_HEIGHT
    #ifdef STATUS_BED_ANIM
      #define STATUS_BED_HEIGHT(S) ((S) ? sizeof(status_bed_on_bmp) / (STATUS_BED_BYTEWIDTH) : sizeof(status_bed_bmp) / (STATUS_BED_BYTEWIDTH))
    #else
      #define STATUS_BED_HEIGHT(S) (sizeof(status_bed_bmp) / (STATUS_BED_BYTEWIDTH))
    #endif
  #endif

  #ifndef STATUS_BED_Y
    #define STATUS_BED_Y(S) (20 - STATUS_BED_HEIGHT(S))
  #endif

  #ifndef STATUS_BED_TEXT_X
    #define STATUS_BED_TEXT_X (STATUS_BED_X + 9)
  #endif

  static_assert(
    sizeof(status_bed_bmp) == (STATUS_BED_BYTEWIDTH) * (STATUS_BED_HEIGHT(0)),
    "Status bed bitmap (status_bed_bmp) dimensions don't match data."
  );
  #ifdef STATUS_BED_ANIM
    static_assert(
      sizeof(status_bed_on_bmp) == (STATUS_BED_BYTEWIDTH) * (STATUS_BED_HEIGHT(1)),
      "Status bed bitmap (status_bed_on_bmp) dimensions don't match data."
    );
  #endif
#endif

//
// Fan Bitmap Properties
//
#ifndef STATUS_FAN_BYTEWIDTH
  #define STATUS_FAN_BYTEWIDTH BW(STATUS_FAN_WIDTH)
#endif
#if STATUS_FAN_FRAMES
  #ifndef STATUS_FAN_X
    #define STATUS_FAN_X (LCD_PIXEL_WIDTH - (STATUS_FAN_BYTEWIDTH) * 8)
  #endif
  #ifndef STATUS_FAN_Y
    #define STATUS_FAN_Y 1
  #endif
  #ifndef STATUS_FAN_TEXT_X
    #define STATUS_FAN_TEXT_X (STATUS_FAN_X - 1)
  #endif
  #ifndef STATUS_FAN_TEXT_Y
    #define STATUS_FAN_TEXT_Y 28
  #endif
  #ifndef STATUS_FAN_HEIGHT
    #define STATUS_FAN_HEIGHT (sizeof(status_fan0_bmp) / (STATUS_FAN_BYTEWIDTH))
  #endif
  #define FAN_BMP_SIZE (STATUS_FAN_BYTEWIDTH) * (STATUS_FAN_HEIGHT)
  static_assert(sizeof(status_fan0_bmp) == FAN_BMP_SIZE, "Status fan bitmap (status_fan0_bmp) dimensions don't match data.");
  #if STATUS_FAN_FRAMES > 1
    static_assert(sizeof(status_fan1_bmp) == FAN_BMP_SIZE, "Status fan bitmap (status_fan1_bmp) dimensions don't match data.");
    #if STATUS_FAN_FRAMES > 2
      static_assert(sizeof(status_fan2_bmp) == FAN_BMP_SIZE, "Status fan bitmap (status_fan2_bmp) dimensions don't match data.");
      #if STATUS_FAN_FRAMES > 3
        static_assert(sizeof(status_fan3_bmp) == FAN_BMP_SIZE, "Status fan bitmap (status_fan3_bmp) dimensions don't match data.");
      #endif
    #endif
  #endif
#endif

#if STATUS_LOGO_WIDTH && ENABLED(CUSTOM_STATUS_SCREEN_IMAGE)
  #define DO_DRAW_LOGO 1
#endif
#if HOTENDS > 0
  #define DO_DRAW_HOTENDS 1
#endif
#if HAS_HEATED_BED && HOTENDS <= 4
  #define DO_DRAW_BED 1
#endif
#if HAS_CUTTER && !DO_DRAW_BED
  #define DO_DRAW_CUTTER 1
#endif
#if HAS_COOLER
  #define DO_DRAW_COOLER 1
#endif
#if ENABLED(LASER_COOLANT_FLOW_METER)
  #define DO_DRAW_FLOWMETER 1
#endif

#if HAS_TEMP_CHAMBER && STATUS_CHAMBER_WIDTH && HOTENDS <= 4
  #define DO_DRAW_CHAMBER 1
#endif
#if HAS_FAN0 && STATUS_FAN_WIDTH && HOTENDS <= 4 && defined(STATUS_FAN_FRAMES)
  #define DO_DRAW_FAN 1
#endif
#if BOTH(HAS_HOTEND, STATUS_HOTEND_ANIM)
  #define ANIM_HOTEND 1
#endif
#if BOTH(DO_DRAW_BED, STATUS_BED_ANIM)
  #define ANIM_BED 1
#endif
#if BOTH(DO_DRAW_CHAMBER, STATUS_CHAMBER_ANIM)
  #define ANIM_CHAMBER 1
#endif
#if BOTH(DO_DRAW_CUTTER, STATUS_CUTTER_ANIM)
  #define ANIM_CUTTER 1
#endif
#if BOTH(DO_DRAW_COOLER, STATUS_COOLER_ANIM)
  #define ANIM_COOLER 1
#endif
#if BOTH(DO_DRAW_FLOWMETER, STATUS_FLOWMETER_ANIM)
  #define ANIM_FLOWMETER 1
#endif
#if ANIM_HOTEND || ANIM_BED || ANIM_CHAMBER || ANIM_CUTTER
  #define ANIM_HBCC 1
#endif
