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

#define MARLIN_VERSION "1.0.2"

/**
* Implementation of the LCD display routines for a DOGM128 graphic display. These are common LCD 128x64 pixel graphic displays.
**/

#ifdef ULTIPANEL
#define BLEN_A 0
#define BLEN_B 1
#define BLEN_C 2
#define EN_A (1<<BLEN_A)
#define EN_B (1<<BLEN_B)
#define EN_C (1<<BLEN_C)
#define encrot0 0
#define encrot1 2
#define encrot2 3
#define encrot3 1
#define LCD_CLICKED (buttons&EN_C)
#endif

#include <U8glib.h>
#include "DOGMbitmaps.h"
#include "dogm_font_data_marlin.h"
#include "ultralcd.h"
#include "ultralcd_st7920_u8glib_rrd.h"

/* Russian language not supported yet, needs custom font

#ifdef LANGUAGE_RU
#include "LiquidCrystalRus.h"
#define LCD_CLASS LiquidCrystalRus
#else
#include <LiquidCrystal.h>
#define LCD_CLASS LiquidCrystal
#endif
*/

#define USE_BIG_EDIT_FONT                // save 3120 bytes of PROGMEM by commenting out this line
#define FONT_STATUSMENU u8g_font_6x9
#define FONT_MENU u8g_font_6x10_marlin

// DOGM parameters (size in pixels)
#define DOG_CHAR_WIDTH         6
#define DOG_CHAR_HEIGHT        12
#ifdef USE_BIG_EDIT_FONT
  #define FONT_MENU_EDIT u8g_font_9x18
  #define DOG_CHAR_WIDTH_EDIT  9
  #define DOG_CHAR_HEIGHT_EDIT 18
  #define LCD_WIDTH_EDIT       14
#else
  #define FONT_MENU_EDIT u8g_font_6x10_marlin
  #define DOG_CHAR_WIDTH_EDIT  6
  #define DOG_CHAR_HEIGHT_EDIT 12
  #define LCD_WIDTH_EDIT       22
#endif

#define START_ROW              0

/* Custom characters defined in font font_6x10_marlin.c */
#define LCD_STR_DEGREE      "\xB0"
#define LCD_STR_REFRESH     "\xF8"
#define LCD_STR_FOLDER      "\xF9"
#define LCD_STR_ARROW_RIGHT "\xFA"
#define LCD_STR_UPLEVEL     "\xFB"
#define LCD_STR_CLOCK       "\xFC"
#define LCD_STR_FEEDRATE    "\xFD"
#define LCD_STR_BEDTEMP     "\xFE"
#define LCD_STR_THERMOMETER "\xFF"

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
    u8g.setFont(FONT_MENU);
    #ifndef STRING_SPLASH_LINE2
      u8g.drawStr(txt1X, u8g.getHeight() - DOG_CHAR_HEIGHT, STRING_SPLASH_LINE1);
    #else
      int txt2X = (u8g.getWidth() - (sizeof(STRING_SPLASH_LINE2) - 1)*DOG_CHAR_WIDTH) / 2;
      u8g.drawStr(txt1X, u8g.getHeight() - DOG_CHAR_HEIGHT*3/2, STRING_SPLASH_LINE1);
      u8g.drawStr(txt2X, u8g.getHeight() - DOG_CHAR_HEIGHT*1/2, STRING_SPLASH_LINE2);
    #endif
	} while(u8g.nextPage());
}

static void lcd_implementation_clear() { } // Automatically cleared by Picture Loop

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
static void lcd_printPGM(const char* str) {
  char c;
  while ((c = pgm_read_byte(str++))) u8g.print(c);
}

static void _draw_heater_status(int x, int heater) {
  bool isBed = heater < 0;
  int y = 17 + (isBed ? 1 : 0);
  u8g.setFont(FONT_STATUSMENU);
  u8g.setPrintPos(x,6);
  u8g.print(itostr3(int((heater >= 0 ? degTargetHotend(heater) : degTargetBed()) + 0.5)));
  lcd_printPGM(PSTR(LCD_STR_DEGREE " "));
  u8g.setPrintPos(x,27);
  u8g.print(itostr3(int(heater >= 0 ? degHotend(heater) : degBed()) + 0.5));
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
    u8g.setFont(FONT_STATUSMENU);
 
    if (IS_SD_PRINTING) {
      // Progress bar solid part
      u8g.drawBox(55, 50, (unsigned int)(71.f * card.percentDone() / 100.f), 2);
    }

    u8g.setPrintPos(80,47);
    if (starttime != 0) {
      uint16_t time = (millis() - starttime) / 60000;
      u8g.print(itostr2(time/60));
      u8g.print(':');
      u8g.print(itostr2(time%60));
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
  u8g.setFont(FONT_STATUSMENU);
  u8g.setPrintPos(104,27);
  #if defined(FAN_PIN) && FAN_PIN > -1
    int per = ((fanSpeed + 1) * 100) / 256;
    if (per) {
      u8g.print(itostr3(per));
      u8g.print("%");
    }
    else
  #endif
    {
      u8g.print("---");
    }

  // X, Y, Z-Coordinates
  u8g.setFont(FONT_STATUSMENU);
  u8g.drawBox(0,29,128,10);
  u8g.setColorIndex(0); // white on black
  u8g.setPrintPos(2,37);
  u8g.print("X");
  u8g.drawPixel(8,33);
  u8g.drawPixel(8,35);
  u8g.setPrintPos(10,37);
  u8g.print(ftostr31ns(current_position[X_AXIS]));
  u8g.setPrintPos(43,37);
  lcd_printPGM(PSTR("Y"));
  u8g.drawPixel(49,33);
  u8g.drawPixel(49,35);
  u8g.setPrintPos(51,37);
  u8g.print(ftostr31ns(current_position[Y_AXIS]));
  u8g.setPrintPos(83,37);
  u8g.print("Z");
  u8g.drawPixel(89,33);
  u8g.drawPixel(89,35);
  u8g.setPrintPos(91,37);
  u8g.print(ftostr31(current_position[Z_AXIS]));
  u8g.setColorIndex(1); // black on white
 
  // Feedrate
  u8g.setFont(FONT_MENU);
  u8g.setPrintPos(3,49);
  u8g.print(LCD_STR_FEEDRATE[0]);
  u8g.setFont(FONT_STATUSMENU);
  u8g.setPrintPos(12,48);
  u8g.print(itostr3(feedmultiply));
  u8g.print('%');

  // Status line
  u8g.setFont(FONT_STATUSMENU);
  u8g.setPrintPos(0,61);
  #ifndef FILAMENT_LCD_DISPLAY
    u8g.print(lcd_status_message);
  #else
    if (millis() < message_millis + 5000) {  //Display both Status message line and Filament display on the last line
      u8g.print(lcd_status_message);
    }
    else {
      lcd_printPGM(PSTR("dia:"));
      u8g.print(ftostr12ns(filament_width_meas));
      lcd_printPGM(PSTR(" factor:"));
      u8g.print(itostr3(extrudemultiply));
      u8g.print('%');
    }
  #endif
}

static void lcd_implementation_mark_as_selected(uint8_t row, char pr_char) {
  if ((pr_char == '>') || (pr_char == LCD_STR_UPLEVEL[0] )) {
    u8g.setColorIndex(1);  // black on white
    u8g.drawBox (0, row*DOG_CHAR_HEIGHT + 3, 128, DOG_CHAR_HEIGHT);
    u8g.setColorIndex(0);  // following text must be white on black
  }
  else {
    u8g.setColorIndex(1); // unmarked text is black on white
  }
  u8g.setPrintPos(START_ROW * DOG_CHAR_WIDTH, (row + 1) * DOG_CHAR_HEIGHT);
}

static void lcd_implementation_drawmenu_generic(uint8_t row, const char* pstr, char pre_char, char post_char) {
  char c;
  uint8_t n = LCD_WIDTH - 2;

  lcd_implementation_mark_as_selected(row, pre_char);

  while((c = pgm_read_byte(pstr))) {
    u8g.print(c);
    pstr++;
    n--;
  }
  while(n--) u8g.print(' ');
  u8g.print(post_char);
  u8g.print(' ');
}

static void _drawmenu_setting_edit_generic(uint8_t row, const char* pstr, char pre_char, const char* data, bool pgm) {
  char c;
  uint8_t n = LCD_WIDTH - 2 - (pgm ? lcd_strlen_P(data) : (lcd_strlen((char*)data)));

  lcd_implementation_mark_as_selected(row, pre_char);

  while( (c = pgm_read_byte(pstr))) {
    u8g.print(c);
    pstr++;
    n--;
  }
  u8g.print(':');
  while(n--) u8g.print(' ');
  if (pgm) { lcd_printPGM(data); } else { u8g.print(data); }
}

#define lcd_implementation_drawmenu_setting_edit_generic(row, pstr, pre_char, data) _drawmenu_setting_edit_generic(row, pstr, pre_char, data, false)
#define lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, pre_char, data) _drawmenu_setting_edit_generic(row, pstr, pre_char, data, true)

#define lcd_implementation_drawmenu_setting_edit_int3_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_int3(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float43_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float43(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5_selected(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool_selected(row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_bool(row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

//Add version for callback functions
#define lcd_implementation_drawmenu_setting_edit_callback_int3_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_int3(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float43_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float43(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5_selected(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5(row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(row, pstr, ' ', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_bool_selected(row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
#define lcd_implementation_drawmenu_setting_edit_callback_bool(row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(row, pstr, ' ', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

void lcd_implementation_drawedit(const char* pstr, char* value) {
  uint8_t rows = 1;
  uint8_t lcd_width = LCD_WIDTH;
  uint8_t char_width = DOG_CHAR_WIDTH;

  #ifdef USE_BIG_EDIT_FONT
    if (lcd_strlen_P(pstr) <= LCD_WIDTH_EDIT - 1) {
      u8g.setFont(FONT_MENU_EDIT);
      lcd_width = LCD_WIDTH_EDIT + 1;
      char_width = DOG_CHAR_WIDTH_EDIT;
      if (lcd_strlen_P(pstr) >= LCD_WIDTH_EDIT - lcd_strlen(value)) rows = 2;
    }
    else {
      u8g.setFont(FONT_MENU);
    }
  #endif

  if (lcd_strlen_P(pstr) > LCD_WIDTH - 2 - lcd_strlen(value)) rows = 2;

  const float kHalfChar = DOG_CHAR_HEIGHT_EDIT / 2;
  float rowHeight = u8g.getHeight() / (rows + 1); // 1/(rows+1) = 1/2 or 1/3

  u8g.setPrintPos(0, rowHeight + kHalfChar);
  lcd_printPGM(pstr);
  u8g.print(':');
  u8g.setPrintPos((lcd_width-1-lcd_strlen(value)) * char_width, rows * rowHeight + kHalfChar);
  u8g.print(value);
}

static void _drawmenu_sd(uint8_t row, const char* pstr, const char* filename, char * const longFilename, bool isDir, bool isSelected) {
  char c;
  uint8_t n = LCD_WIDTH - 1;

  if (longFilename[0] != '\0') {
    filename = longFilename;
    longFilename[n] = '\0';
  }

  lcd_implementation_mark_as_selected(row, ((isSelected) ? '>' : ' '));

  if (isDir) u8g.print(LCD_STR_FOLDER[0]);
  while((c = *filename) != '\0') {
    u8g.print(c);
    filename++;
    n--;
  }
  while(n--) u8g.print(' ');
}

#define lcd_implementation_drawmenu_sdfile_selected(row, pstr, filename, longFilename) _drawmenu_sd(row, pstr, filename, longFilename, false, true)
#define lcd_implementation_drawmenu_sdfile(row, pstr, filename, longFilename) _drawmenu_sd(row, pstr, filename, longFilename, false, false)
#define lcd_implementation_drawmenu_sddirectory_selected(row, pstr, filename, longFilename) _drawmenu_sd(row, pstr, filename, longFilename, true, true)
#define lcd_implementation_drawmenu_sddirectory(row, pstr, filename, longFilename) _drawmenu_sd(row, pstr, filename, longFilename, true, false)

#define lcd_implementation_drawmenu_back_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_back(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_submenu(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode_selected(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_gcode(row, pstr, gcode) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')
#define lcd_implementation_drawmenu_function_selected(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(row, pstr, data) lcd_implementation_drawmenu_generic(row, pstr, ' ', ' ')

static void lcd_implementation_quick_feedback() {
  #if BEEPER > -1
    SET_OUTPUT(BEEPER);
    for(int8_t i=0; i<10; i++) {
      WRITE(BEEPER,HIGH);
      delay(3);
      WRITE(BEEPER,LOW);
      delay(3);
    }
  #endif
}

#endif //__DOGM_LCD_IMPLEMENTATION_H
