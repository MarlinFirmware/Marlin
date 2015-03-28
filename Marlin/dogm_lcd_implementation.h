/**
 * dogm_lcd_implementation.h
 *
 * Graphics LCD implementation for 128x64 pixel LCDs by STB for ErikZalm/Marlin
 * Demonstrator: http://www.reprap.org/wiki/STB_Electronics
 * License: http://opensource.org/licenses/BSD-3-Clause
 *
 * With the use of:
 * u8glib by Oliver Kraus
 * http://code.google.com/p/u8glib/
 * License: http://opensource.org/licenses/BSD-3-Clause
 */

#ifndef DOGM_LCD_IMPLEMENTATION_H
#define DOGM_LCD_IMPLEMENTATION_H

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display. These are common LCD 128x64 pixel graphic displays.
 */

#ifdef ULTIPANEL
  #define BLEN_A 0
  #define BLEN_B 1
  #define BLEN_C 2
  #define EN_A BIT(BLEN_A)
  #define EN_B BIT(BLEN_B)
  #define EN_C BIT(BLEN_C)
  #define LCD_CLICKED (buttons&EN_C)
#endif

#include <U8glib.h>
#include "DOGMbitmaps.h"

#include "ultralcd.h"
#include "ultralcd_st7920_u8glib_rrd.h"
#include "Configuration.h"

// save 3120 bytes of PROGMEM by commenting out #define USE_BIG_EDIT_FONT
// we don't have a big font for Cyrillic, Kana
#if defined( MAPPER_C2C3 ) || defined( MAPPER_NON )
//  #define USE_BIG_EDIT_FONT
#endif

// If you have spare 2300Byte of progmem and want to use a 
// smaller font on the Info-screen uncomment the next line.
//#define USE_SMALL_INFOFONT
#ifdef USE_SMALL_INFOFONT
  #include "dogm_font_data_6x9_marlin.h"
  #define FONT_STATUSMENU_NAME u8g_font_6x9
#else
  #define FONT_STATUSMENU_NAME FONT_MENU_NAME
#endif

#include "dogm_font_data_Marlin_symbols.h"   // The Marlin special symbols
#define FONT_SPECIAL_NAME Marlin_symbols

#ifndef SIMULATE_ROMFONT
  #if defined( DISPLAY_CHARSET_ISO10646_1 )
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #elif defined( DISPLAY_CHARSET_ISO10646_5 )
    #include "dogm_font_data_ISO10646_5_Cyrillic.h"
    #define FONT_MENU_NAME ISO10646_5_Cyrillic_5x7
  #elif defined( DISPLAY_CHARSET_ISO10646_KANA )
    #include "dogm_font_data_ISO10646_Kana.h"
    #define FONT_MENU_NAME ISO10646_Kana_5x7
  #else // fall-back
    #include "dogm_font_data_ISO10646_1.h"
    #define FONT_MENU_NAME ISO10646_1_5x7
  #endif
#else // SIMULATE_ROMFONT
  #if defined( DISPLAY_CHARSET_HD44780_JAPAN )
    #include "dogm_font_data_HD44780_J.h"
    #define FONT_MENU_NAME HD44780_J_5x7
  #elif defined( DISPLAY_CHARSET_HD44780_WESTERN )
    #include "dogm_font_data_HD44780_W.h"
    #define FONT_MENU_NAME HD44780_W_5x7
  #elif defined( DISPLAY_CHARSET_HD44780_CYRILLIC )
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
#ifdef USE_BIG_EDIT_FONT
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

#define START_ROW              0

/* Custom characters defined in font font_6x10_marlin_symbols */
// \x00 intentionally skipped to avoid problems in strings
#define LCD_STR_REFRESH     "\x01"
#define LCD_STR_FOLDER      "\x02"
#define LCD_STR_ARROW_RIGHT "\x03"
#define LCD_STR_UPLEVEL     "\x04"
#define LCD_STR_CLOCK       "\x05"
#define LCD_STR_FEEDRATE    "\x06"
#define LCD_STR_BEDTEMP     "\x07"
#define LCD_STR_THERMOMETER "\x08"
#define LCD_STR_DEGREE      "\x09"

#define LCD_STR_SPECIAL_MAX '\x09'
// Maximum here is 0x1f because 0x20 is ' ' (space) and the normal charsets begin.
// Better stay below 0x10 because DISPLAY_CHARSET_HD44780_WESTERN begins here.

int lcd_contrast;

// LCD selection
#ifdef U8GLIB_ST7920
//U8GLIB_ST7920_128X64_RRD u8g(0,0,0);
U8GLIB_ST7920_128X64_RRD u8g(0);
#elif defined(MAKRPANEL)
// The MaKrPanel display, ST7565 controller as well
U8GLIB_NHD_C12864 u8g(DOGLCD_CS, DOGLCD_A0);
#elif defined(VIKI2) || defined(miniVIKI)
// Mini Viki and Viki 2.0 LCD, ST7565 controller as well
U8GLIB_NHD_C12864 u8g(DOGLCD_CS, DOGLCD_A0);
#else
// for regular DOGM128 display with HW-SPI
U8GLIB_DOGM128 u8g(DOGLCD_CS, DOGLCD_A0);  // HW-SPI Com: CS, A0
#endif

#include "utf_mapper.h"

char currentfont = 0;

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

char lcd_print(char* str) {
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

static void lcd_implementation_init()
{
  #ifdef LCD_PIN_BL // Enable LCD backlight
    pinMode(LCD_PIN_BL, OUTPUT);
	  digitalWrite(LCD_PIN_BL, HIGH);
  #endif

  u8g.setContrast(lcd_contrast);	
	// FIXME: remove this workaround
  // Uncomment this if you have the first generation (V1.10) of STBs board
	// pinMode(17, OUTPUT);	// Enable LCD backlight
	// digitalWrite(17, HIGH);
  
#ifdef LCD_SCREEN_ROT_90
	u8g.setRot90();   // Rotate screen by 90°
#elif defined(LCD_SCREEN_ROT_180)
	u8g.setRot180();	// Rotate screen by 180°
#elif defined(LCD_SCREEN_ROT_270)
	u8g.setRot270();	// Rotate screen by 270°
#endif
	
  // Show splashscreen
  int offx = (u8g.getWidth() - START_BMPWIDTH) / 2;
  #ifdef START_BMPHIGH
    int offy = 0;
  #else
    int offy = DOG_CHAR_HEIGHT;
  #endif

  int txt1X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE1) - 1)*DOG_CHAR_WIDTH) / 2;

	u8g.firstPage();
	do {
    u8g.drawBitmapP(offx, offy, START_BMPBYTEWIDTH, START_BMPHEIGHT, start_bmp);
    lcd_setFont(FONT_MENU);
    #ifndef STRING_SPLASH_LINE2
      u8g.drawStr(txt1X, u8g.getHeight() - DOG_CHAR_HEIGHT, STRING_SPLASH_LINE1);
    #else
      int txt2X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE2) - 1)*DOG_CHAR_WIDTH) / 2;
      u8g.drawStr(txt1X, u8g.getHeight() - DOG_CHAR_HEIGHT*3/2, STRING_SPLASH_LINE1);
      u8g.drawStr(txt2X, u8g.getHeight() - DOG_CHAR_HEIGHT*1/2, STRING_SPLASH_LINE2);
    #endif
	} while (u8g.nextPage());
}

static void lcd_implementation_clear() { } // Automatically cleared by Picture Loop

static void _draw_heater_status(int x, int heater) {
  bool isBed = heater < 0;
  int y = 17 + (isBed ? 1 : 0);

  lcd_setFont(FONT_STATUSMENU);
  u8g.setPrintPos(x,7);
  lcd_print(itostr3(int((heater >= 0 ? degTargetHotend(heater) : degTargetBed()) + 0.5)));
  lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
  u8g.setPrintPos(x,28);
  lcd_print(itostr3(int(heater >= 0 ? degHotend(heater) : degBed()) + 0.5));

  lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
  if (!isHeatingHotend(0)) {
    u8g.drawBox(x+7,y,2,2);
  }
  else {
    u8g.setColorIndex(0); // white on black
    u8g.drawBox(x+7,y,2,2);
    u8g.setColorIndex(1); // black on white
  }
}

static void lcd_implementation_status_screen() {

  static unsigned char fan_rot = 0;
 
  u8g.setColorIndex(1); // black on white

  // Symbols menu graphics, animated fan
  u8g.drawBitmapP(9,1,STATUS_SCREENBYTEWIDTH,STATUS_SCREENHEIGHT, (blink % 2) && fanSpeed ? status_screen0_bmp : status_screen1_bmp);
 
  #ifdef SDSUPPORT
    // SD Card Symbol
    u8g.drawBox(42,42,8,7);
    u8g.drawBox(50,44,2,5);
    u8g.drawFrame(42,49,10,4);
    u8g.drawPixel(50,43);

    // Progress bar frame
    u8g.drawFrame(54,49,73,4);

    // SD Card Progress bar and clock
    lcd_setFont(FONT_STATUSMENU);
 
    if (IS_SD_PRINTING) {
      // Progress bar solid part
      u8g.drawBox(55, 50, (unsigned int)(71.f * card.percentDone() / 100.f), 2);
    }

    u8g.setPrintPos(80,48);
    if (starttime != 0) {
      uint16_t time = (millis() - starttime) / 60000;
      lcd_print(itostr2(time/60));
      lcd_print(':');
      lcd_print(itostr2(time%60));
    }
    else {
      lcd_printPGM(PSTR("--:--"));
    }
  #endif
 
  // Extruders
  for (int i=0; i<EXTRUDERS; i++) _draw_heater_status(6 + i * 25, i);

  // Heatbed
  if (EXTRUDERS < 4) _draw_heater_status(81, -1);
 
  // Fan
  lcd_setFont(FONT_STATUSMENU);
  u8g.setPrintPos(104,27);
  #if defined(FAN_PIN) && FAN_PIN > -1
    int per = ((fanSpeed + 1) * 100) / 256;
    if (per) {

      lcd_print(itostr3(per));
      lcd_print('%');
    }
    else
  #endif
    {
      lcd_printPGM(PSTR("---"));
    }

  // X, Y, Z-Coordinates
  #define XYZ_BASELINE 38
  lcd_setFont(FONT_STATUSMENU);

  #ifdef USE_SMALL_INFOFONT
    u8g.drawBox(0,30,128,10);
  #else
    u8g.drawBox(0,30,128,9);
  #endif
  u8g.setColorIndex(0); // white on black
  u8g.setPrintPos(2,XYZ_BASELINE);
  lcd_print('X');
  u8g.drawPixel(8,XYZ_BASELINE - 5);
  u8g.drawPixel(8,XYZ_BASELINE - 3);
  u8g.setPrintPos(10,XYZ_BASELINE);
  lcd_print(ftostr31ns(current_position[X_AXIS]));
  u8g.setPrintPos(43,XYZ_BASELINE);
  lcd_print('Y');
  u8g.drawPixel(49,XYZ_BASELINE - 5);
  u8g.drawPixel(49,XYZ_BASELINE - 3);
  u8g.setPrintPos(51,XYZ_BASELINE);
  lcd_print(ftostr31ns(current_position[Y_AXIS]));
  u8g.setPrintPos(83,XYZ_BASELINE);
  lcd_print('Z');
  u8g.drawPixel(89,XYZ_BASELINE - 5);
  u8g.drawPixel(89,XYZ_BASELINE - 3);
  u8g.setPrintPos(91,XYZ_BASELINE);
  lcd_print(ftostr31(current_position[Z_AXIS]));
  u8g.setColorIndex(1); // black on white
 
  // Feedrate
  lcd_setFont(FONT_MENU);
  u8g.setPrintPos(3,49);
  lcd_print(LCD_STR_FEEDRATE[0]);
  lcd_setFont(FONT_STATUSMENU);
  u8g.setPrintPos(12,49);
  lcd_print(itostr3(feedmultiply));
  lcd_print('%');

  // Status line
  lcd_setFont(FONT_STATUSMENU);
  #ifdef USE_SMALL_INFOFONT
  u8g.setPrintPos(0,62);
  #else
  u8g.setPrintPos(0,63);
  #endif
  #ifndef FILAMENT_LCD_DISPLAY
    lcd_print(lcd_status_message);
  #else
    if (millis() < message_millis + 5000) {  //Display both Status message line and Filament display on the last line
      lcd_print(lcd_status_message);
    }
    else {
      lcd_printPGM(PSTR("dia:"));
      lcd_print(ftostr12ns(filament_width_meas));
      lcd_printPGM(PSTR(" factor:"));
      lcd_print(itostr3(extrudemultiply));
      lcd_print('%');
    }
  #endif
}

static void lcd_implementation_mark_as_selected(uint8_t row, bool isSelected) {
  if (isSelected) {
    u8g.setColorIndex(1);  // black on white
    u8g.drawBox(0, row * DOG_CHAR_HEIGHT + 3, 128, DOG_CHAR_HEIGHT);
    u8g.setColorIndex(0);  // following text must be white on black
  }
  else {
    u8g.setColorIndex(1); // unmarked text is black on white
  }
  u8g.setPrintPos(START_ROW * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);
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
  lcd_print(post_char);
  lcd_print(' ');
}

static void _drawmenu_setting_edit_generic(bool isSelected, uint8_t row, const char* pstr, const char* data, bool pgm) {
  char c;
  uint8_t n = LCD_WIDTH - 2 - (pgm ? lcd_strlen_P(data) : (lcd_strlen((char*)data)));

  lcd_implementation_mark_as_selected(row, isSelected);

  while (c = pgm_read_byte(pstr)) {
    n -= lcd_print(c);
    pstr++;
  }
  lcd_print(':');
  while (n--) lcd_print(' ');
  if (pgm) { lcd_printPGM(data); } else { lcd_print((char *)data); }
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

void lcd_implementation_drawedit(const char* pstr, char* value) {
  uint8_t rows = 1;
  uint8_t lcd_width = LCD_WIDTH, char_width = DOG_CHAR_WIDTH;
  uint8_t vallen = lcd_strlen(value);

  #ifdef USE_BIG_EDIT_FONT
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

  const float kHalfChar = DOG_CHAR_HEIGHT_EDIT / 2;
  float rowHeight = u8g.getHeight() / (rows + 1); // 1/(rows+1) = 1/2 or 1/3

  u8g.setPrintPos(0, rowHeight + kHalfChar);
  lcd_printPGM(pstr);
  lcd_print(':');
  u8g.setPrintPos((lcd_width - 1 - vallen) * char_width, rows * rowHeight + kHalfChar);
  lcd_print(value);
}

static void _drawmenu_sd(bool isSelected, uint8_t row, const char* pstr, const char* filename, char * const longFilename, bool isDir) {
  char c;
  uint8_t n = LCD_WIDTH - 1;

  if (longFilename[0]) {
    filename = longFilename;
    longFilename[n] = '\0';
  }

  lcd_implementation_mark_as_selected(row, isSelected);

  if (isDir) lcd_print(LCD_STR_FOLDER[0]);
  while (c = *filename) {
    n -= lcd_print(c);
    filename++;
  }
  while (n--) lcd_print(' ');
}

#define lcd_implementation_drawmenu_sdfile(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, false)
#define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, filename, longFilename) _drawmenu_sd(sel, row, pstr, filename, longFilename, true)

#define lcd_implementation_drawmenu_back(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

#endif //__DOGM_LCD_IMPLEMENTATION_H
