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
 * dogm_lcd_implementation.h
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

#ifndef DOGM_LCD_IMPLEMENTATION_H
#define DOGM_LCD_IMPLEMENTATION_H

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display. These are common LCD 128x64 pixel graphic displays.
 */

#if ENABLED(ULTIPANEL)
  #define BLEN_A 0
  #define BLEN_B 1
  #define BLEN_C 2
  #define EN_A (_BV(BLEN_A))
  #define EN_B (_BV(BLEN_B))
  #define EN_C (_BV(BLEN_C))
  #define LCD_CLICKED (buttons&EN_C)
#endif

#include <U8glib.h>
#include "dogm_bitmaps.h"

#include "ultralcd.h"
#include "ultralcd_st7920_u8glib_rrd.h"
#include "Configuration.h"

#if DISABLED(MAPPER_C2C3) && DISABLED(MAPPER_NON) && ENABLED(USE_BIG_EDIT_FONT)
  #undef USE_BIG_EDIT_FONT
#endif


#if ENABLED(USE_SMALL_INFOFONT)
  #include "dogm_font_data_6x9_marlin.h"
  #define FONT_STATUSMENU_NAME u8g_font_6x9
#else
  #define FONT_STATUSMENU_NAME FONT_MENU_NAME
#endif

#include "dogm_font_data_Marlin_symbols.h"   // The Marlin special symbols
#define FONT_SPECIAL_NAME Marlin_symbols

#if DISABLED(SIMULATE_ROMFONT)
  #if ENABLED(DISPLAY_CHARSET_ISO10646_1)
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_5)
    #include "dogm_font_data_ISO10646_5_Cyrillic.h"
    #define FONT_MENU_NAME ISO10646_5_Cyrillic_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_KANA)
    #include "dogm_font_data_ISO10646_Kana.h"
    #define FONT_MENU_NAME ISO10646_Kana_5x7
  #elif ENABLED(DISPLAY_CHARSET_ISO10646_CN)
    #include "dogm_font_data_ISO10646_CN.h"
    #define FONT_MENU_NAME ISO10646_CN
    #define TALL_FONT_CORRECTION 1
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #endif
#else // SIMULATE_ROMFONT
  #if ENABLED(DISPLAY_CHARSET_HD44780_JAPAN)
    #include "dogm_font_data_HD44780_J.h"
    #define FONT_MENU_NAME HD44780_J_5x7
  #elif ENABLED(DISPLAY_CHARSET_HD44780_WESTERN)
    #include "dogm_font_data_HD44780_W.h"
    #define FONT_MENU_NAME HD44780_W_5x7
  #elif ENABLED(DISPLAY_CHARSET_HD44780_CYRILLIC)
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

#define START_ROW              0

// LCD selection
#if ENABLED(U8GLIB_ST7920)
  //U8GLIB_ST7920_128X64_RRD u8g(0,0,0);
  U8GLIB_ST7920_128X64_RRD u8g(0);
#elif ENABLED(MAKRPANEL)
  // The MaKrPanel display, ST7565 controller as well
  U8GLIB_NHD_C12864_2X u8g(DOGLCD_CS, DOGLCD_A0);
#elif ENABLED(VIKI2) || ENABLED(miniVIKI)
  // Mini Viki and Viki 2.0 LCD, ST7565 controller as well
  U8GLIB_NHD_C12864_2X u8g(DOGLCD_CS, DOGLCD_A0);
#elif ENABLED(U8GLIB_LM6059_AF)
  // Based on the Adafruit ST7565 (http://www.adafruit.com/products/250)
  U8GLIB_LM6059_2X u8g(DOGLCD_CS, DOGLCD_A0);
#elif ENABLED(U8GLIB_SSD1306)
  // Generic support for SSD1306 OLED I2C LCDs
  U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);
#elif ENABLED(MINIPANEL)
  // The MINIPanel display
  U8GLIB_MINI12864_2X u8g(DOGLCD_CS, DOGLCD_A0);
#else
  // for regular DOGM128 display with HW-SPI
  U8GLIB_DOGM128_2X u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0
#endif

#ifndef LCD_PIXEL_WIDTH
  #define LCD_PIXEL_WIDTH 128
#endif
#ifndef LCD_PIXEL_HEIGHT
  #define LCD_PIXEL_HEIGHT 64
#endif

#include "utf_mapper.h"

int lcd_contrast;
static char currentfont = 0;

static void lcd_setFont(char font_nr) {
  switch(font_nr) {
    case FONT_STATUSMENU : {u8g.setFont(FONT_STATUSMENU_NAME); currentfont = FONT_STATUSMENU;}; break;
    case FONT_MENU       : {u8g.setFont(FONT_MENU_NAME); currentfont = FONT_MENU;}; break;
    case FONT_SPECIAL    : {u8g.setFont(FONT_SPECIAL_NAME); currentfont = FONT_SPECIAL;}; break;
    case FONT_MENU_EDIT  : {u8g.setFont(FONT_MENU_EDIT_NAME); currentfont = FONT_MENU_EDIT;}; break;
    break;
  }
}

char lcd_print(char c) {
  if ((c > 0) && (c <= LCD_STR_SPECIAL_MAX)) {
    u8g.setFont(FONT_SPECIAL_NAME);
    u8g.print(c);
    lcd_setFont(currentfont);
    return 1;
  } else {
    return charset_mapper(c);
  }
}

char lcd_print(const char* str) {
  char c;
  int i = 0;
  char n = 0;
  while ((c = str[i++])) {
    n += lcd_print(c);
  }
  return n;
}

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
char lcd_printPGM(const char* str) {
  char c;
  char n = 0;
  while ((c = pgm_read_byte(str++))) {
    n += lcd_print(c);
  }
  return n;
}

#if ENABLED(SHOW_BOOTSCREEN)
  static bool show_bootscreen = true;
#endif

/* Warning: This function is called from interrupt context */
static void lcd_implementation_init() {

  #if defined(LCD_PIN_BL) && LCD_PIN_BL > -1 // Enable LCD backlight
    pinMode(LCD_PIN_BL, OUTPUT);
    digitalWrite(LCD_PIN_BL, HIGH);
  #endif

  #if defined(LCD_PIN_RESET) && LCD_PIN_RESET > -1
    pinMode(LCD_PIN_RESET, OUTPUT);
    digitalWrite(LCD_PIN_RESET, HIGH);
  #endif

  #if DISABLED(MINIPANEL) // setContrast not working for Mini Panel
    u8g.setContrast(lcd_contrast);
  #endif

  // FIXME: remove this workaround
  // Uncomment this if you have the first generation (V1.10) of STBs board
  // pinMode(17, OUTPUT); // Enable LCD backlight
  // digitalWrite(17, HIGH);

  #if ENABLED(LCD_SCREEN_ROT_180)
    u8g.setRot180();  // Rotate screen by 180°
  #endif

  #if ENABLED(SHOW_BOOTSCREEN)
    int offx = (u8g.getWidth() - (START_BMPWIDTH)) / 2;
    #if ENABLED(START_BMPHIGH)
      int offy = 0;
    #else
      int offy = DOG_CHAR_HEIGHT;
    #endif

    int txt1X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE1) - 1) * (DOG_CHAR_WIDTH)) / 2;

    u8g.firstPage();
    do {
      if (show_bootscreen) {
        u8g.drawBitmapP(offx, offy, START_BMPBYTEWIDTH, START_BMPHEIGHT, start_bmp);
        lcd_setFont(FONT_MENU);
        #ifndef STRING_SPLASH_LINE2
          u8g.drawStr(txt1X, u8g.getHeight() - (DOG_CHAR_HEIGHT), STRING_SPLASH_LINE1);
        #else
          int txt2X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE2) - 1) * (DOG_CHAR_WIDTH)) / 2;
          u8g.drawStr(txt1X, u8g.getHeight() - (DOG_CHAR_HEIGHT) * 3 / 2, STRING_SPLASH_LINE1);
          u8g.drawStr(txt2X, u8g.getHeight() - (DOG_CHAR_HEIGHT) * 1 / 2, STRING_SPLASH_LINE2);
        #endif
      }
    } while (u8g.nextPage());

    if (show_bootscreen) {
      delay(1000);
      show_bootscreen = false;
    }
  #endif
}

int8_t glcd_loopcounter = 0;
int8_t glcd_loops = 0;

static void lcd_implementation_clear() { } // Automatically cleared by Picture Loop

static void _draw_heater_status(int x, int heater) {
  int heaterp1 = heater + 1;
  bool isBed = heaterp1 == 0;
  int y = 17 + (isBed ? 1 : 0);

  static char target_str[EXTRUDERS+1][4];
  static char is_str[EXTRUDERS+1][4];

#if ENABLED(LCD_SCREEN_ROT_180)
  if (glcd_loopcounter == 1 || glcd_loopcounter == 2 || glcd_loopcounter == 4) {
#else
  if (!glcd_loopcounter) {
#endif
    uitoaR10(int((heater >= 0 ? degTargetHotend(heater) : degTargetBed()) + 0.5), target_str[heaterp1], 3);
    uitoaR10(int(heater >= 0 ? degHotend(heater) : degBed()) + 0.5, is_str[heaterp1], 3);
  }
  lcd_setFont(FONT_STATUSMENU);
  u8g.setPrintPos(x, 7);
  lcd_print(target_str[heaterp1]);
  lcd_printPGM(PSTR(LCD_STR_DEGREE));
  u8g.setPrintPos(x, 28);
  lcd_print(is_str[heaterp1]);
  lcd_printPGM(PSTR(LCD_STR_DEGREE));
  if (isBed) {
    #if HAS_TEMP_BED
      u8g.drawBitmapP(x + (4*DOG_CHAR_WIDTH - STATUS_BED_WIDTH)/2, 8, STATUS_BED_BYTEWIDTH, STATUS_BED_HEIGHT, bed_graphic[isHeatingBed()]);
    #endif
  }
  else
    u8g.drawBitmapP(x + (4*DOG_CHAR_WIDTH - STATUS_EXTRUDER_WIDTH)/2, 8, STATUS_EXTRUDER_BYTEWIDTH, STATUS_EXTRUDER_HEIGHT, extruder_graphic[heater][isHeatingHotend(heater)]);
}

static void lcd_implementation_status_screen() {

  bool blink = lcd_blink();

  // storage for pracalculated strings
  static char xpos_str[6];
  static char ypos_str[6];
  static char zpos_str[7];
  static char fr_str[4];
  #if HAS_FAN0
    static int per = 0;
    static char fanper_str[4];
  #endif
  #if ENABLED(SDSUPPORT)
    uint16_t time;
    static char h_str[4];
    static char m_str[3];
    static unsigned int progress_bar = 0;
  #endif
  #if ENABLED(FILAMENT_LCD_DISPLAY)
    static char fila_d_str[5];
    static char fila_f_str[5];
  #endif

  // precalculate all strings in round 0
  if (!glcd_loopcounter) {
    dtostrfMP(current_position[X_AXIS], 5, 2, xpos_str);
    dtostrfMP(current_position[Y_AXIS], 5, 2, ypos_str);
    dtostrfMP(current_position[Z_AXIS], 6, 2, zpos_str);
    uitoaR10(feedrate_multiplier, fr_str, 3);
    #if HAS_FAN0
      per = ((fanSpeeds[0] + 1) * 100) / 256;
      uitoaR10(per, fanper_str, 3);
    #endif
    #if ENABLED(SDSUPPORT)
      time = (((print_job_stop_ms > print_job_start_ms) ? print_job_stop_ms : millis()) - print_job_start_ms) / 60000;
      uitoaR10(time/60, h_str, 2);
      uitoaR10p(time%60, m_str, 2, '0');
      if (IS_SD_PRINTING)
        progress_bar = (unsigned int)(71.f * card.percentDone() / 100.f);
    #endif
    #if ENABLED(FILAMENT_LCD_DISPLAY)
      dtostrfMP(filament_width_meas, 4, 2, fila_d_str);
      itoa(100.0 * volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM], fila_f_str, 10);
    #endif
  }

  // upper half
  #if ENABLED(LCD_SCREEN_ROT_180)
    if ((glcd_loopcounter & ((glcd_loops)>>1))) {
  #else
    if (!(glcd_loopcounter & ((glcd_loops)>>1))) {
  #endif

  u8g.setColorIndex(1); // black on white

  // Symbols menu graphics, animated fan

  // Extruders
  #if HAS_TEMP_BED && HAS_FAN0
    #define E_DIST ((LCD_PIXEL_WIDTH)/(EXTRUDERS+2))
  #elif HAS_TEMP_BED || HAS_FAN0
    #define E_DIST ((LCD_PIXEL_WIDTH)/(EXTRUDERS+1))
  #else
    #define E_DIST ((LCD_PIXEL_WIDTH)/(EXTRUDERS))
  #endif
  #if E_DIST < 4*DOG_CHAR_WIDTH
    #error To much devices in GCLD top row
  #endif
  for (int i = 0; i < EXTRUDERS; i++) _draw_heater_status(E_DIST/2 + i*E_DIST - 2*DOG_CHAR_WIDTH, i);

  // Heatbed
  #if HAS_TEMP_BED
    _draw_heater_status(E_DIST/2 + EXTRUDERS*E_DIST - 2*DOG_CHAR_WIDTH, -1);
  #endif

  // Fan
  lcd_setFont(FONT_STATUSMENU);
  #if HAS_FAN0
    u8g.drawBitmapP(LCD_PIXEL_WIDTH - E_DIST/2 - 2*DOG_CHAR_WIDTH + (4*DOG_CHAR_WIDTH - STATUS_FAN_WIDTH)/2, 2, STATUS_FAN_BYTEWIDTH, STATUS_FAN_HEIGHT, fan_graphic[((per>0) && blink)?0:1]);
    u8g.setPrintPos(LCD_PIXEL_WIDTH - E_DIST/2 - 2*DOG_CHAR_WIDTH, 28);
    if (per) {
      lcd_print(fanper_str);
      lcd_print('%');
    }
    else
    {
      lcd_printPGM(PSTR("---"));
    }
  #endif

  #if ENABLED(USE_SMALL_INFOFONT)
    u8g.drawHLine(0, 30, LCD_PIXEL_WIDTH);
  #else
    u8g.drawHLine(0, 29, LCD_PIXEL_WIDTH, 9);
  #endif
  }

  // X, Y, Z-Coordinates
  // Before homing the axis letters are blinking 'X' <-> '?'.
  // When axis is homed but axis_known_position is false the axis letters are blinking 'X' <-> ' '.
  // When everything is ok you see a constant 'X'.
  #define XYZ_BASELINE 38
  lcd_setFont(FONT_STATUSMENU);

  u8g.setPrintPos(2, XYZ_BASELINE);
  if (blink)
    lcd_printPGM(PSTR(MSG_X));
  else {
    if (!axis_homed[X_AXIS])
      lcd_printPGM(PSTR("?"));
    else {
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[X_AXIS])
          lcd_printPGM(PSTR(" "));
        else
      #endif
      lcd_printPGM(PSTR(MSG_X));
    }
  }
  u8g.drawPixel(8, XYZ_BASELINE - 5);
  u8g.drawPixel(8, XYZ_BASELINE - 3);
  u8g.setPrintPos(10, XYZ_BASELINE);
  lcd_print(xpos_str);

  u8g.setPrintPos(43, XYZ_BASELINE);
  if (blink)
    lcd_printPGM(PSTR(MSG_Y));
  else {
    if (!axis_homed[Y_AXIS])
      lcd_printPGM(PSTR("?"));
    else {
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[Y_AXIS])
          lcd_printPGM(PSTR(" "));
        else
      #endif
      lcd_printPGM(PSTR(MSG_Y));
    }
  }
  u8g.drawPixel(49, XYZ_BASELINE - 5);
  u8g.drawPixel(49, XYZ_BASELINE - 3);
  u8g.setPrintPos(51, XYZ_BASELINE);
  lcd_print(ypos_str);

  u8g.setPrintPos(83, XYZ_BASELINE);
  if (blink)
    lcd_printPGM(PSTR(MSG_Z));
  else {
    if (!axis_homed[Z_AXIS])
      lcd_printPGM(PSTR("?"));
    else {
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[Z_AXIS])
          lcd_printPGM(PSTR(" "));
        else
      #endif
      lcd_printPGM(PSTR(MSG_Z));
    }
  }
  u8g.drawPixel(89, XYZ_BASELINE - 5);
  u8g.drawPixel(89, XYZ_BASELINE - 3);
  u8g.setPrintPos(91, XYZ_BASELINE);
  lcd_print(zpos_str);

  // lower half
  #if ENABLED(LCD_SCREEN_ROT_180)
    if (!(glcd_loopcounter & ((glcd_loops)>>1))) {
  #else
    if ((glcd_loopcounter & ((glcd_loops)>>1))) {
  #endif
  // Feedrate
  lcd_setFont(FONT_MENU);
  u8g.setPrintPos(3, 49);
  lcd_print(LCD_STR_FEEDRATE[0]);
  lcd_setFont(FONT_STATUSMENU);
  u8g.setPrintPos(12, 49);
  lcd_print(fr_str);
  lcd_print('%');

  #if ENABLED(USE_SMALL_INFOFONT)
    u8g.drawHLine(0, 40, LCD_PIXEL_WIDTH);
  #else
    u8g.drawHLine(0, 39, LCD_PIXEL_WIDTH);
  #endif

  #if ENABLED(SDSUPPORT)
    // SD Card Symbol
    u8g.drawFrame(42, 42 - TALL_FONT_CORRECTION, 10, 7);
    u8g.drawPixel(50, 43 - TALL_FONT_CORRECTION);
    u8g.setColorIndex(0); // white on black
    u8g.drawPixel(50, 42 - TALL_FONT_CORRECTION);
    u8g.drawPixel(51, 43 - TALL_FONT_CORRECTION);
    u8g.drawPixel(51, 42 - TALL_FONT_CORRECTION);
    u8g.setColorIndex(1); // black on white
    if (IS_SD_INSERTED) {
      u8g.drawBox(42, 49 - TALL_FONT_CORRECTION, 10, 4);
    }

    // SD Card Progress bar and clock
    lcd_setFont(FONT_STATUSMENU);

    if (IS_SD_PRINTING) {
      // Progress bar frame
      u8g.drawFrame(54, 49, 73, 4 - TALL_FONT_CORRECTION);

      // Progress bar solid part
      u8g.drawBox(55, 50, progress_bar, 2 - (TALL_FONT_CORRECTION));

      if (print_job_start_ms != 0) {
        u8g.setPrintPos(80,48);
        lcd_print(h_str);
        lcd_print(':');
        lcd_print(m_str);
      }
    }
  #endif

  // Status line
  lcd_setFont(FONT_STATUSMENU);
  #if ENABLED(USE_SMALL_INFOFONT)
    u8g.setPrintPos(0, 62);
  #else
    u8g.setPrintPos(0, 63);
  #endif
  #if DISABLED(FILAMENT_LCD_DISPLAY)
    lcd_print(lcd_status_message);
  #else
    if (millis() < previous_lcd_status_ms + 5000) {  //Display both Status message line and Filament display on the last line
      lcd_print(lcd_status_message);
    }
    else {
      lcd_printPGM(PSTR("dia:"));
      lcd_print(fila_d_str);
      lcd_printPGM(PSTR(" factor:"));
      lcd_print(fila_f_str);
      lcd_print('%');
    }
  #endif
  }
}

static void lcd_implementation_mark_as_selected(uint8_t row, bool isSelected) {
  if (isSelected) {
    u8g.setColorIndex(1);  // black on white
    u8g.drawBox(0, row * (DOG_CHAR_HEIGHT) + 3 - (TALL_FONT_CORRECTION), LCD_PIXEL_WIDTH, DOG_CHAR_HEIGHT);
    u8g.setColorIndex(0);  // following text must be white on black
  }
  else {
    u8g.setColorIndex(1); // unmarked text is black on white
  }
  u8g.setPrintPos((START_ROW) * (DOG_CHAR_WIDTH), (row + 1) * (DOG_CHAR_HEIGHT));
}

static void lcd_implementation_drawmenu_generic(bool isSelected, uint8_t row, const char* pstr, char pre_char, char post_char) {
  char c;
  uint8_t n = LCD_WIDTH - 2;

  lcd_implementation_mark_as_selected(row, isSelected);

  while (c = pgm_read_byte(pstr)) {
    n -= lcd_print(c);
    pstr++;
  }
  while (n--) lcd_print(' ');
  u8g.setPrintPos(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH), (row + 1) * (DOG_CHAR_HEIGHT));
  lcd_print(post_char);
  lcd_print(' ');
}

static void _drawmenu_setting_edit_generic(bool isSelected, uint8_t row, const char* pstr, const char* data, bool pgm) {
  char c;
  uint8_t vallen = (pgm ? lcd_strlen_P(data) : (lcd_strlen((char*)data)));
  uint8_t n = LCD_WIDTH - 2 - vallen;

  lcd_implementation_mark_as_selected(row, isSelected);

  while (c = pgm_read_byte(pstr)) {
    n -= lcd_print(c);
    pstr++;
  }
  lcd_print(':');
  while (n--) lcd_print(' ');
  u8g.setPrintPos(LCD_PIXEL_WIDTH - (DOG_CHAR_WIDTH) * vallen, (row + 1) * (DOG_CHAR_HEIGHT));
  if (pgm)  lcd_printPGM(data);  else  lcd_print((char*)data);
}

#define lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, false)
#define lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, true)

#define lcd_implementation_drawmenu_setting_edit_int3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float43(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

//Add version for callback functions
#define lcd_implementation_drawmenu_setting_edit_callback_int3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float43(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

void lcd_implementation_drawedit(const char* pstr, const char* value) {
  uint8_t rows = 1;
  uint8_t lcd_width = LCD_WIDTH, char_width = DOG_CHAR_WIDTH;
  uint8_t vallen = lcd_strlen(value);

  #if ENABLED(USE_BIG_EDIT_FONT)
    if (lcd_strlen_P(pstr) <= LCD_WIDTH_EDIT - 1) {
      lcd_setFont(FONT_MENU_EDIT);
      lcd_width = LCD_WIDTH_EDIT + 1;
      char_width = DOG_CHAR_WIDTH_EDIT;
      if (lcd_strlen_P(pstr) >= LCD_WIDTH_EDIT - vallen) rows = 2;
    }
    else {
      lcd_setFont(FONT_MENU);
    }
  #endif

  if (lcd_strlen_P(pstr) > LCD_WIDTH - 2 - vallen) rows = 2;

  const float kHalfChar = (DOG_CHAR_HEIGHT_EDIT) / 2;
  float rowHeight = u8g.getHeight() / (rows + 1); // 1/(rows+1) = 1/2 or 1/3

  u8g.setPrintPos(0, rowHeight + kHalfChar);
  lcd_printPGM(pstr);
  if (value != NULL) {
    lcd_print(':');
    u8g.setPrintPos((lcd_width - 1 - vallen) * char_width, rows * rowHeight + kHalfChar);
    lcd_print(value);
  }
}

#if ENABLED(SDSUPPORT)

  static void _drawmenu_sd(bool isSelected, uint8_t row, const char* pstr, const char* filename, char* const longFilename, bool isDir) {
    char c;
    uint8_t n = LCD_WIDTH - 1;

    if (longFilename[0]) {
      filename = longFilename;
      longFilename[n] = '\0';
    }

    lcd_implementation_mark_as_selected(row, isSelected);

    if (isDir) lcd_print(LCD_STR_FOLDER[0]);
    while ((c = *filename)) {
      n -= lcd_print(c);
      filename++;
    }
    while (n--) lcd_print(' ');
  }

  #define lcd_implementation_drawmenu_sdfile(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, false)
  #define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, true)

#endif //SDSUPPORT

#define lcd_implementation_drawmenu_back(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

#endif //__DOGM_LCD_IMPLEMENTATION_H
