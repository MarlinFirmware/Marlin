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
 * ultralcd_impl_DOGM.h
 *
 * Graphics LCD implementation for 128x64 pixel LCDs by STB for ErikZalm/Marlin
 * Demonstrator: http://www.reprap.org/wiki/STB_Electronics
 * License: http://opensource.org/licenses/BSD-3-Clause
 *
 * With the use of:
 * u8glib by Oliver Kraus
 * https://github.com/olikraus/U8glib_Arduino
 * License: http://opensource.org/licenses/BSD-3-Clause
 */

#ifndef ULTRALCD_IMPL_DOGM_H
#define ULTRALCD_IMPL_DOGM_H

#include <U8glib.h>
#include "u8g_fontutf8.h"

#include "MarlinConfig.h"

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display.
 * These are common LCD 128x64 pixel graphic displays.
 */
#include "ultralcd.h"
#include "ultralcd_st7920_u8glib_rrd.h"
#include "dogm_bitmaps.h"
#include "utility.h"
#include "duration_t.h"


#if ENABLED(SHOW_BOOTSCREEN) && ENABLED(SHOW_CUSTOM_BOOTSCREEN)
  #include "_Bootscreen.h"
#endif

#if ENABLED(USE_SMALL_INFOFONT)
  #include "dogm_font_data_6x9_marlin.h"
  #define FONT_STATUSMENU_NAME u8g_font_6x9
#else
  #define FONT_STATUSMENU_NAME FONT_MENU_NAME
#endif

#include LANGUAGE_DATA_INCL(LCD_LANGUAGE)
#define TALL_FONT_CORRECTION 1

#if DISABLED(SIMULATE_ROMFONT)
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
#else // SIMULATE_ROMFONT
  #if DISPLAY_CHARSET_HD44780 == JAPANESE
    #include "dogm_font_data_HD44780_J.h"
    #define FONT_MENU_NAME HD44780_J_5x7
  #elif DISPLAY_CHARSET_HD44780 == WESTERN
    #include "dogm_font_data_HD44780_W.h"
    #define FONT_MENU_NAME HD44780_W_5x7
  #elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    #include "dogm_font_data_HD44780_C.h"
    #define FONT_MENU_NAME HD44780_C_5x7
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #endif
#endif // SIMULATE_ROMFONT

//#define FONT_STATUSMENU_NAME FONT_MENU_NAME

#define FONT_STATUSMENU 1
#define FONT_SPECIAL 2
#define FONT_MENU_EDIT 3
#define FONT_MENU 4

// DOGM parameters (size in pixels)
#define DOG_CHAR_WIDTH         6
#define DOG_CHAR_HEIGHT        12
#if ENABLED(USE_BIG_EDIT_FONT)
  #define FONT_MENU_EDIT_NAME u8g_font_9x18
  #define DOG_CHAR_WIDTH_EDIT  9
  #define DOG_CHAR_HEIGHT_EDIT 18
  #define LCD_WIDTH_EDIT       14
#else
  #define FONT_MENU_EDIT_NAME FONT_MENU_NAME
  #define DOG_CHAR_WIDTH_EDIT  6
  #define DOG_CHAR_HEIGHT_EDIT 12
  #define LCD_WIDTH_EDIT       22
#endif

#ifndef TALL_FONT_CORRECTION
  #define TALL_FONT_CORRECTION 0
#endif

#define START_COL              0

// LCD selection
#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
  U8GLIB_ST7920_128X64_4X u8g(LCD_PINS_RS);
#elif ENABLED(U8GLIB_ST7920)
  //U8GLIB_ST7920_128X64_RRD u8g(0,0,0);
  U8GLIB_ST7920_128X64_RRD u8g(0);
#elif ENABLED(CARTESIO_UI)
  // The CartesioUI display
  #if DOGLCD_MOSI != -1 && DOGLCD_SCK != -1
    // using SW-SPI
    U8GLIB_DOGM128 u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS, DOGLCD_A0);
  #else
    U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);
  #endif
#elif ENABLED(U8GLIB_LM6059_AF)
  // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)
  U8GLIB_LM6059 u8g(DOGLCD_CS, DOGLCD_A0);
#elif ENABLED(MAKRPANEL) || ENABLED(VIKI2) || ENABLED(miniVIKI)
  // The MaKrPanel, Mini Viki, and Viki 2.0, ST7565 controller as well
  U8GLIB_NHD_C12864 u8g(DOGLCD_CS, DOGLCD_A0);
#elif ENABLED(U8GLIB_SSD1306)
  // Generic support for SSD1306 OLED I2C LCDs
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);
#elif ENABLED(U8GLIB_SH1106)
  // Generic support for SH1106 OLED I2C LCDs
  U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_FAST);
#elif ENABLED(MINIPANEL)
  // The MINIPanel display
  U8GLIB_MINI12864 u8g(DOGLCD_CS, DOGLCD_A0);
#else
  // for regular DOGM128 display with HW-SPI
  U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0
#endif

U8GLIB *pu8g = &u8g;

#ifndef LCD_PIXEL_WIDTH
  #define LCD_PIXEL_WIDTH 128
#endif
#ifndef LCD_PIXEL_HEIGHT
  #define LCD_PIXEL_HEIGHT 64
#endif

#include "lcdprint.h"

int lcd_contrast;
static char currentfont = 0;

#if ENABLED(USE_SMALL_INFOFONT) || !DISABLED(SIMULATE_ROMFONT)
#include "dogm_font_data_Marlin_symbols.h"   // The Marlin special symbols
#define FONT_SPECIAL_NAME Marlin_symbols
static void lcd_setFont(char font_nr);
char lcd_print_u(wchar_t c) {
  if ((c > 0) && (c <= LCD_STR_SPECIAL_MAX)) {
    u8g.setFont(FONT_SPECIAL_NAME);
    u8g.print(c);
    lcd_setFont(currentfont);
    return 1;
  } else {
    return lcd_print_wchar(c);
  }
}
#else
// The Marlin special symbols is merged in the ISO10646_1_5x7
#define FONT_SPECIAL_NAME FONT_MENU_NAME
#define lcd_print_u lcd_print_wchar
#endif

static void lcd_setFont(char font_nr) {
  switch(font_nr) {
    case FONT_STATUSMENU : {u8g.setFont(FONT_STATUSMENU_NAME); currentfont = FONT_STATUSMENU;}; break;
    default:
    case FONT_MENU       : {u8g.setFont(FONT_MENU_NAME); currentfont = FONT_MENU;}; break;
    case FONT_SPECIAL    : {u8g.setFont(FONT_SPECIAL_NAME); currentfont = FONT_SPECIAL;}; break;
    case FONT_MENU_EDIT  : {u8g.setFont(FONT_MENU_EDIT_NAME); currentfont = FONT_MENU_EDIT;}; break;
  }
}

// Initialize or re-initializw the LCD
static void lcd_implementation_init() {

  #if PIN_EXISTS(LCD_BACKLIGHT) // Enable LCD backlight
    OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
  #endif

  #if PIN_EXISTS(LCD_RESET)
    OUT_WRITE(LCD_RESET_PIN, HIGH);
  #endif

  #if DISABLED(MINIPANEL) // setContrast not working for Mini Panel
    u8g.setContrast(lcd_contrast);
  #endif

  #if ENABLED(LCD_SCREEN_ROT_90)
    u8g.setRot90();   // Rotate screen by 90°
  #elif ENABLED(LCD_SCREEN_ROT_180)
    u8g.setRot180();  // Rotate screen by 180°
  #elif ENABLED(LCD_SCREEN_ROT_270)
    u8g.setRot270();  // Rotate screen by 270°
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    static bool show_bootscreen = true;

    #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
      if (show_bootscreen) {
        u8g.firstPage();
        do {
          u8g.drawBitmapP(
            (128 - (CUSTOM_BOOTSCREEN_BMPWIDTH))  /2,
            ( 64 - (CUSTOM_BOOTSCREEN_BMPHEIGHT)) /2,
            CEILING(CUSTOM_BOOTSCREEN_BMPWIDTH, 8), CUSTOM_BOOTSCREEN_BMPHEIGHT, custom_start_bmp);
        } while (u8g.nextPage());
        safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT);
      }
    #endif // SHOW_CUSTOM_BOOTSCREEN

    int offx = (u8g.getWidth() - (START_BMPWIDTH)) / 2;

    #if ENABLED(START_BMPHIGH)
      int offy = 0;
    #else
      int offy = DOG_CHAR_HEIGHT;
    #endif

    int txt1X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE1) - 1) * (DOG_CHAR_WIDTH)) / 2;

    if (show_bootscreen) {
      u8g.firstPage();
      do {
        u8g.drawBitmapP(offx, offy, START_BMPBYTEWIDTH, START_BMPHEIGHT, start_bmp);
        lcd_setFont(FONT_MENU);
        #ifndef STRING_SPLASH_LINE2
          u8g.drawStr(txt1X, u8g.getHeight() - (DOG_CHAR_HEIGHT), STRING_SPLASH_LINE1);
        #else
          int txt2X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE2) - 1) * (DOG_CHAR_WIDTH)) / 2;
          u8g.drawStr(txt1X, u8g.getHeight() - (DOG_CHAR_HEIGHT) * 3 / 2, STRING_SPLASH_LINE1);
          u8g.drawStr(txt2X, u8g.getHeight() - (DOG_CHAR_HEIGHT) * 1 / 2, STRING_SPLASH_LINE2);
        #endif
      } while (u8g.nextPage());
    }

    show_bootscreen = false;

  #endif // SHOW_BOOTSCREEN

    uxg_SetUtf8Fonts (g_fontinfo, NUM_ARRAY(g_fontinfo));
}

// The kill screen is displayed for unrecoverable conditions
void lcd_kill_screen() {
  lcd_setFont(FONT_MENU);
  lcd_moveto(0, u8g.getHeight()/4*1);
  lcd_print(lcd_status_message);
  lcd_moveto(0, u8g.getHeight()/4*2);
  lcd_printPGM(PSTR(MSG_HALTED));
  lcd_moveto(0, u8g.getHeight()/4*3);
  lcd_printPGM(PSTR(MSG_PLEASE_RESET));
}

static void lcd_implementation_clear() { } // Automatically cleared by Picture Loop

//
// Status Screen
//

FORCE_INLINE void _draw_centered_temp(int temp, int x, int y) {
  int degsize = 6 * (temp >= 100 ? 3 : temp >= 10 ? 2 : 1); // number's pixel width
  lcd_moveto(x - (18 - degsize) / 2, y + TALL_FONT_CORRECTION); // move left if shorter
  lcd_print(itostr3(temp));
  lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
}

FORCE_INLINE void _draw_heater_status(int x, int heater) {
  #if HAS_TEMP_BED
    bool isBed = heater < 0;
  #else
    const bool isBed = false;
  #endif

  _draw_centered_temp((isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater)) + 0.5, x, 7);

  _draw_centered_temp((isBed ? thermalManager.degBed() : thermalManager.degHotend(heater)) + 0.5, x, 28);

  int h = isBed ? 7 : 8,
      y = isBed ? 18 : 17;
  if (isBed ? thermalManager.isHeatingBed() : thermalManager.isHeatingHotend(heater)) {
    u8g.setColorIndex(0); // white on black
    u8g.drawBox(x + h, y, 2, 2);
    u8g.setColorIndex(1); // black on white
  }
  else {
    u8g.drawBox(x + h, y, 2, 2);
  }
}

FORCE_INLINE void _draw_axis_label(AxisEnum axis, const char *pstr, bool blink) {
  if (blink)
    lcd_printPGM(pstr);
  else {
    if (!axis_homed[axis])
      lcd_print_u('?');
    else {
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[axis])
          lcd_print_u(' ');
        else
      #endif
      lcd_printPGM(pstr);
    }
  }
}

//#define DOGM_SD_PERCENT

static void lcd_implementation_status_screen() {
  u8g.setColorIndex(1); // black on white

  bool blink = lcd_blink();

  // Symbols menu graphics, animated fan
  u8g.drawBitmapP(9, 1, STATUS_SCREENBYTEWIDTH, STATUS_SCREENHEIGHT,
    #if HAS_FAN0
      blink && fanSpeeds[0] ? status_screen0_bmp : status_screen1_bmp
    #else
      status_screen0_bmp
    #endif
  );

  // Status Menu Font for SD info, Heater status, Fan, XYZ
  lcd_setFont(FONT_STATUSMENU);

  #if ENABLED(SDSUPPORT)
    // SD Card Symbol
    u8g.drawBox(42, 42 - (TALL_FONT_CORRECTION), 8, 7);
    u8g.drawBox(50, 44 - (TALL_FONT_CORRECTION), 2, 5);
    u8g.drawFrame(42, 49 - (TALL_FONT_CORRECTION), 10, 4);
    u8g.drawPixel(50, 43 - (TALL_FONT_CORRECTION));

    // Progress bar frame
    u8g.drawFrame(54, 49, 73, 4 - (TALL_FONT_CORRECTION));

    // SD Card Progress bar and clock
    if (IS_SD_PRINTING) {
      // Progress bar solid part
      u8g.drawBox(55, 50, (unsigned int)(71 * card.percentDone() * 0.01), 2 - (TALL_FONT_CORRECTION));
    
      #if ENABLED(DOGM_SD_PERCENT)
        // Percent complete
        u8g.setPrintPos(55, 48);
        lcd_print(itostr3(card.percentDone()));
        lcd_print_u('%');
      #endif
    }

    char buffer[10];
    duration_t elapsed = print_job_timer.duration();
    bool has_days = (elapsed.value > 60*60*24L);
    elapsed.toDigital(buffer, has_days);

    #if DISABLED(DOGM_SD_PERCENT)
      #define SD_DURATION_X 71
    #else
      #define SD_DURATION_X 89
    #endif

    lcd_moveto (SD_DURATION_X + (has_days ? 0 : 9), 48);
    lcd_print(buffer);

  #endif

  // Extruders
  HOTEND_LOOP() _draw_heater_status(5 + e * 25, e);

  // Heated bed
  #if HOTENDS < 4 && HAS_TEMP_BED
    _draw_heater_status(81, -1);
  #endif

  // Fan
  lcd_moveto(104, 27);
  #if HAS_FAN0
    int per = ((fanSpeeds[0] + 1) * 100) / 256;
    if (per) {
      lcd_print(itostr3(per));
      lcd_print_u('%');
    }
  #endif

  // X, Y, Z-Coordinates
  // Before homing the axis letters are blinking 'X' <-> '?'.
  // When axis is homed but axis_known_position is false the axis letters are blinking 'X' <-> ' '.
  // When everything is ok you see a constant 'X'.
  #define XYZ_BASELINE (38 + TALL_FONT_CORRECTION)

  #if ENABLED(USE_SMALL_INFOFONT)
    u8g.drawBox(0, 30, LCD_PIXEL_WIDTH, 10);
  #else
    u8g.drawBox(0, 30, LCD_PIXEL_WIDTH, 9);
  #endif
  u8g.setColorIndex(0); // white on black

  lcd_moveto(2, XYZ_BASELINE);
  _draw_axis_label(X_AXIS, PSTR(MSG_X), blink);
  lcd_moveto(10, XYZ_BASELINE);
  lcd_print(ftostr4sign(current_position[X_AXIS]));

  lcd_moveto(43, XYZ_BASELINE);
  _draw_axis_label(Y_AXIS, PSTR(MSG_Y), blink);
  lcd_moveto(51, XYZ_BASELINE);
  lcd_print(ftostr4sign(current_position[Y_AXIS]));

  lcd_moveto(83, XYZ_BASELINE);
  _draw_axis_label(Z_AXIS, PSTR(MSG_Z), blink);
  lcd_moveto(91, XYZ_BASELINE);
  lcd_print(ftostr52sp(current_position[Z_AXIS] + 0.00001));

  u8g.setColorIndex(1); // black on white

  // Feedrate
  lcd_setFont(FONT_MENU);
  lcd_moveto(3, 49);
  lcd_print_u(LCD_STR_FEEDRATE[0]);

  lcd_setFont(FONT_STATUSMENU);
  lcd_moveto(12, 49);
  lcd_print(itostr3(feedrate_percentage));
  lcd_print_u('%');

  // Status line
  #if ENABLED(USE_SMALL_INFOFONT)
    lcd_moveto(0, 62);
  #else
    lcd_moveto(0, 63);
  #endif
  #if DISABLED(FILAMENT_LCD_DISPLAY)
    lcd_print(lcd_status_message);
  #else
    if (PENDING(millis(), previous_lcd_status_ms + 5000UL)) {  //Display both Status message line and Filament display on the last line
      lcd_print(lcd_status_message);
    }
    else {
      lcd_printPGM(PSTR("dia:"));
      lcd_print(ftostr12ns(filament_width_meas));
      lcd_printPGM(PSTR(" factor:"));
      lcd_print(itostr3(100.0 * volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]));
      lcd_print_u('%');
    }
  #endif
}

#if ENABLED(ULTIPANEL)

  // Set the colors for a menu item based on whether it is selected
  static void lcd_implementation_mark_as_selected(uint8_t row, bool isSelected) {
    if (isSelected) {
      u8g.setColorIndex(1);  // black on white
      u8g.drawBox(0, row * (DOG_CHAR_HEIGHT) + 3 - (TALL_FONT_CORRECTION), LCD_PIXEL_WIDTH, DOG_CHAR_HEIGHT);
      u8g.setColorIndex(0);  // following text must be white on black
    }
    else {
      u8g.setColorIndex(1); // unmarked text is black on white
    }
    lcd_moveto((START_COL) * (DOG_CHAR_WIDTH), (row + 1) * (DOG_CHAR_HEIGHT));
  }

  #if ENABLED(LCD_INFO_MENU) || ENABLED(FILAMENT_CHANGE_FEATURE)

    // Draw a static line of text in the same idiom as a menu item
    static void lcd_implementation_drawmenu_static(uint8_t row, const char* pstr, bool center=true, bool invert=false, const char* valstr=NULL) {

      lcd_implementation_mark_as_selected(row, invert);

      char c;
      int8_t n = LCD_WIDTH - (START_COL);

      n = n * DOG_CHAR_WIDTH;
      n = n - lcd_printstr_P(pstr, n);
      lcd_printstr(valstr, n);
    }

  #endif // LCD_INFO_MENU || FILAMENT_CHANGE_FEATURE

  // Draw a generic menu item
  static void lcd_implementation_drawmenu_generic(bool isSelected, uint8_t row, const char* pstr, char pre_char, char post_char) {
    UNUSED(pre_char);

    char c;
    uint8_t n = LCD_WIDTH - (START_COL) - 2;

    lcd_implementation_mark_as_selected(row, isSelected);

    n = n * DOG_CHAR_WIDTH;
    lcd_printstr_P(pstr, n);
    lcd_moveto(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH), (row + 1) * (DOG_CHAR_HEIGHT));

    lcd_print_u(post_char);
    lcd_print_u(' ');
  }

  // Macros for specific types of menu items
  #define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
  #define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
  #define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
  #define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

  // Draw a menu item with an editable value
  static void _drawmenu_setting_edit_generic(bool isSelected, uint8_t row, const char* pstr, const char* data, bool pgm) {
    char c;
    uint8_t vallen = (pgm ? lcd_strlen_P(data) : (lcd_strlen((char*)data)));
    uint8_t n = LCD_WIDTH - (START_COL) - 2 - vallen;

    lcd_implementation_mark_as_selected(row, isSelected);

    n = n * DOG_CHAR_WIDTH;
    lcd_printstr_P(pstr, n);
    lcd_moveto(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH) * vallen, (row + 1) * (DOG_CHAR_HEIGHT));

    if (pgm)  lcd_printPGM(data);  else  lcd_print((char*)data);
  }

  // Macros for edit items
  #define lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, false)
  #define lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, true)

  #define lcd_implementation_drawmenu_setting_edit_int3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, itostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float32(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr32(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float43(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr43sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float52(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr52sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float51(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr51sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_long5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  #define lcd_implementation_drawmenu_setting_edit_callback_int3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, itostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float32(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr32(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float43(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr43sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float52(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr52sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float51(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr51sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_long5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  void lcd_implementation_drawedit(const char* pstr, const char* value=NULL) {
    uint8_t rows = 1;
    uint8_t lcd_width = LCD_WIDTH - (START_COL), char_width = DOG_CHAR_WIDTH;
    uint8_t vallen = lcd_strlen(value);

    #if ENABLED(USE_BIG_EDIT_FONT)
      if (uxg_GetUtf8StrPixelWidthP (pu8g->getU8g(), pstr) < (DOG_CHAR_WIDTH * LCD_WIDTH_EDIT)) {
        lcd_setFont(FONT_MENU_EDIT);
        lcd_width = LCD_WIDTH_EDIT + 1;
        char_width = DOG_CHAR_WIDTH_EDIT;
        if (lcd_strlen_P(pstr) >= LCD_WIDTH_EDIT - vallen) rows = 2;
      }
      else {
        lcd_setFont(FONT_MENU);
      }
    #endif

    if (uxg_GetUtf8StrPixelWidthP (pu8g->getU8g(), pstr) > DOG_CHAR_WIDTH * (LCD_WIDTH - 2 - vallen)) rows = 2;

    const float kHalfChar = (DOG_CHAR_HEIGHT_EDIT) / 2;
    float rowHeight = u8g.getHeight() / (rows + 1); // 1/(rows+1) = 1/2 or 1/3

    lcd_moveto(0, rowHeight + kHalfChar);
    lcd_printPGM(pstr);
    if (value != NULL) {
      lcd_print_u(':');
      lcd_moveto((lcd_width - 1 - vallen) * char_width, rows * rowHeight + kHalfChar);
      lcd_print(value);
    }
  }

  #if ENABLED(SDSUPPORT)

    static void _drawmenu_sd(bool isSelected, uint8_t row, const char* pstr, const char* filename, char* const longFilename, bool isDir) {
      UNUSED(pstr);
      char c;
      uint8_t n = LCD_WIDTH - (START_COL) - 1;

      if (longFilename[0]) {
        filename = longFilename;
        longFilename[n] = '\0';
      }

      lcd_implementation_mark_as_selected(row, isSelected);

      if (isDir) lcd_print_u (LCD_STR_FOLDER[0]);

      n = n * DOG_CHAR_WIDTH;
      lcd_printstr(filename, n);
    }

    #define lcd_implementation_drawmenu_sdfile(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, false)
    #define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, true)

  #endif // SDSUPPORT

#endif // ULTIPANEL

#endif //__ULTRALCD_IMPL_DOGM_H
