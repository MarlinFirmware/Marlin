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

#ifndef ULTRALCD_IMPL_HD44780_H
#define ULTRALCD_IMPL_HD44780_H

/**
 * Implementation of the LCD display routines for a Hitachi HD44780 display.
 * These are the most common LCD character displays.
 */

#include "utility.h"
#include "duration_t.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl.h"

  #if ENABLED(ULTIPANEL)
    #define ULTRA_X_PIXELS_PER_CHAR    5
    #define ULTRA_Y_PIXELS_PER_CHAR    8
    #define ULTRA_COLUMNS_FOR_MESH_MAP 7
    #define ULTRA_ROWS_FOR_MESH_MAP    4

    #define N_USER_CHARS    8

    #define TOP_LEFT      _BV(0)
    #define TOP_RIGHT     _BV(1)
    #define LOWER_LEFT    _BV(2)
    #define LOWER_RIGHT   _BV(3)
  #endif
#endif

extern volatile uint8_t buttons;  //an extended version of the last checked buttons in a bit array.

////////////////////////////////////
// Setup button and encode mappings for each panel (into 'buttons' variable
//
// This is just to map common functions (across different panels) onto the same
// macro name. The mapping is independent of whether the button is directly connected or
// via a shift/i2c register.

#if ENABLED(ULTIPANEL)

  //
  // Setup other button mappings of each panel
  //
  #if ENABLED(LCD_I2C_VIKI)
    #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

    // button and encoder bit positions within 'buttons'
    #define B_LE (BUTTON_LEFT<<B_I2C_BTN_OFFSET)    // The remaining normalized buttons are all read via I2C
    #define B_UP (BUTTON_UP<<B_I2C_BTN_OFFSET)
    #define B_MI (BUTTON_SELECT<<B_I2C_BTN_OFFSET)
    #define B_DW (BUTTON_DOWN<<B_I2C_BTN_OFFSET)
    #define B_RI (BUTTON_RIGHT<<B_I2C_BTN_OFFSET)

    #if BUTTON_EXISTS(ENC)
      // the pause/stop/restart button is connected to BTN_ENC when used
      #define B_ST (EN_C)                            // Map the pause/stop/resume button into its normalized functional name
      #undef LCD_CLICKED
      #define LCD_CLICKED (buttons&(B_MI|B_RI|B_ST)) // pause/stop button also acts as click until we implement proper pause/stop.
    #else
      #undef LCD_CLICKED
      #define LCD_CLICKED (buttons&(B_MI|B_RI))
    #endif

    // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
    #define LCD_HAS_SLOW_BUTTONS

  #elif ENABLED(LCD_I2C_PANELOLU2)

    #if !BUTTON_EXISTS(ENC) // Use I2C if not directly connected to a pin

      #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

      #define B_MI (PANELOLU2_ENCODER_C<<B_I2C_BTN_OFFSET) // requires LiquidTWI2 library v1.2.3 or later

      #undef LCD_CLICKED
      #define LCD_CLICKED (buttons & B_MI)

      // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
      #define LCD_HAS_SLOW_BUTTONS

    #endif

  #elif DISABLED(NEWPANEL) // old style ULTIPANEL
    // Shift register bits correspond to buttons:
    #define BL_LE 7   // Left
    #define BL_UP 6   // Up
    #define BL_MI 5   // Middle
    #define BL_DW 4   // Down
    #define BL_RI 3   // Right
    #define BL_ST 2   // Red Button
    #define B_LE (_BV(BL_LE))
    #define B_UP (_BV(BL_UP))
    #define B_MI (_BV(BL_MI))
    #define B_DW (_BV(BL_DW))
    #define B_RI (_BV(BL_RI))
    #define B_ST (_BV(BL_ST))
    #define LCD_CLICKED ((buttons & B_MI) || (buttons & B_ST))
  #endif

#endif // ULTIPANEL

////////////////////////////////////
// Create LCD class instance and chipset-specific information
#if ENABLED(LCD_I2C_TYPE_PCF8575)
  // note: these are register mapped pins on the PCF8575 controller not Arduino pins
  #define LCD_I2C_PIN_BL  3
  #define LCD_I2C_PIN_EN  2
  #define LCD_I2C_PIN_RW  1
  #define LCD_I2C_PIN_RS  0
  #define LCD_I2C_PIN_D4  4
  #define LCD_I2C_PIN_D5  5
  #define LCD_I2C_PIN_D6  6
  #define LCD_I2C_PIN_D7  7

  #include <Wire.h>
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  #define LCD_CLASS LiquidCrystal_I2C
  LCD_CLASS lcd(LCD_I2C_ADDRESS, LCD_I2C_PIN_EN, LCD_I2C_PIN_RW, LCD_I2C_PIN_RS, LCD_I2C_PIN_D4, LCD_I2C_PIN_D5, LCD_I2C_PIN_D6, LCD_I2C_PIN_D7);

#elif ENABLED(LCD_I2C_TYPE_MCP23017)
  //for the LED indicators (which maybe mapped to different things in lcd_implementation_update_indicators())
  #define LED_A 0x04 //100
  #define LED_B 0x02 //010
  #define LED_C 0x01 //001

  #define LCD_HAS_STATUS_INDICATORS

  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2
  #if ENABLED(DETECT_DEVICE)
    LCD_CLASS lcd(LCD_I2C_ADDRESS, 1);
  #else
    LCD_CLASS lcd(LCD_I2C_ADDRESS);
  #endif

#elif ENABLED(LCD_I2C_TYPE_MCP23008)
  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2
  #if ENABLED(DETECT_DEVICE)
    LCD_CLASS lcd(LCD_I2C_ADDRESS, 1);
  #else
    LCD_CLASS lcd(LCD_I2C_ADDRESS);
  #endif

#elif ENABLED(LCD_I2C_TYPE_PCA8574)
  #include <LiquidCrystal_I2C.h>
  #define LCD_CLASS LiquidCrystal_I2C
  LCD_CLASS lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);

// 2 wire Non-latching LCD SR from:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!shiftregister-connection
#elif ENABLED(SR_LCD_2W_NL)
  extern "C" void __cxa_pure_virtual() { while (1); }
  #include <LCD.h>
  #include <LiquidCrystal_SR.h>
  #define LCD_CLASS LiquidCrystal_SR
  #if PIN_EXISTS(SR_STROBE)
    LCD_CLASS lcd(SR_DATA_PIN, SR_CLK_PIN, SR_STROBE_PIN);
  #else
    LCD_CLASS lcd(SR_DATA_PIN, SR_CLK_PIN);
  #endif
#elif ENABLED(LCM1602)
  #include <Wire.h>
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  #define LCD_CLASS LiquidCrystal_I2C
  LCD_CLASS lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#else
  // Standard directly connected LCD implementations
  #include <LiquidCrystal.h>
  #define LCD_CLASS LiquidCrystal
  LCD_CLASS lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7); //RS,Enable,D4,D5,D6,D7
#endif

#include "utf_mapper.h"

#if ENABLED(LCD_PROGRESS_BAR)
  static millis_t progress_bar_ms = 0;     // Start millis of the current progress bar cycle
  #if PROGRESS_MSG_EXPIRE > 0
    static millis_t expire_status_ms = 0;  // millis at which to expire the status message
  #endif
  #define LCD_STR_PROGRESS  "\x03\x04\x05"
#endif

#if ENABLED(LCD_HAS_STATUS_INDICATORS)
  static void lcd_implementation_update_indicators();
#endif

static void createChar_P(const char c, const byte * const ptr) {
  byte temp[8];
  for (uint8_t i = 0; i < 8; i++)
    temp[i] = pgm_read_byte(&ptr[i]);
  lcd.createChar(c, temp);
}

#define CHARSET_MENU 0
#define CHARSET_INFO 1
#define CHARSET_BOOT 2

static void lcd_set_custom_characters(
  #if ENABLED(LCD_PROGRESS_BAR) || ENABLED(SHOW_BOOTSCREEN)
    const uint8_t screen_charset=CHARSET_INFO
  #endif
) {
  // CHARSET_BOOT
  #if ENABLED(SHOW_BOOTSCREEN)
    const static PROGMEM byte corner[4][8] = { {
      B00000,
      B00000,
      B00000,
      B00000,
      B00001,
      B00010,
      B00100,
      B00100
    }, {
      B00000,
      B00000,
      B00000,
      B11100,
      B11100,
      B01100,
      B00100,
      B00100
    }, {
      B00100,
      B00010,
      B00001,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    }, {
      B00100,
      B01000,
      B10000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    } };
  #endif // SHOW_BOOTSCREEN

  // CHARSET_INFO
  const static PROGMEM byte bedTemp[8] = {
    B00000,
    B11111,
    B10101,
    B10001,
    B10101,
    B11111,
    B00000,
    B00000
  };

  const static PROGMEM byte degree[8] = {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };

  const static PROGMEM byte thermometer[8] = {
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B10001,
    B01110
  };

  const static PROGMEM byte uplevel[8] = {
    B00100,
    B01110,
    B11111,
    B00100,
    B11100,
    B00000,
    B00000,
    B00000
  };

  const static PROGMEM byte feedrate[8] = {
    B11100,
    B10000,
    B11000,
    B10111,
    B00101,
    B00110,
    B00101,
    B00000
  };

  const static PROGMEM byte clock[8] = {
    B00000,
    B01110,
    B10011,
    B10101,
    B10001,
    B01110,
    B00000,
    B00000
  };

  #if ENABLED(SDSUPPORT)

    // CHARSET_MENU
    const static PROGMEM byte refresh[8] = {
      B00000,
      B00110,
      B11001,
      B11000,
      B00011,
      B10011,
      B01100,
      B00000,
    };
    const static PROGMEM byte folder[8] = {
      B00000,
      B11100,
      B11111,
      B10001,
      B10001,
      B11111,
      B00000,
      B00000
    };

    #if ENABLED(LCD_PROGRESS_BAR)

      // CHARSET_INFO
      const static PROGMEM byte progress[3][8] = { {
        B00000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B00000
      }, {
        B00000,
        B10100,
        B10100,
        B10100,
        B10100,
        B10100,
        B10100,
        B00000
      }, {
        B00000,
        B10101,
        B10101,
        B10101,
        B10101,
        B10101,
        B10101,
        B00000
      } };

    #endif // LCD_PROGRESS_BAR

  #endif // SDSUPPORT

  #if ENABLED(SHOW_BOOTSCREEN) || ENABLED(LCD_PROGRESS_BAR)
    static uint8_t char_mode = CHARSET_MENU;
    #define CHAR_COND (screen_charset != char_mode)
  #else
    #define CHAR_COND true
  #endif

  if (CHAR_COND) {
    #if ENABLED(SHOW_BOOTSCREEN) || ENABLED(LCD_PROGRESS_BAR)
      char_mode = screen_charset;
      #if ENABLED(SHOW_BOOTSCREEN)
        // Set boot screen corner characters
        if (screen_charset == CHARSET_BOOT) {
          for (uint8_t i = 4; i--;)
            createChar_P(i, corner[i]);
        }
        else
      #endif
    #endif
        { // Info Screen uses 5 special characters
          createChar_P(LCD_BEDTEMP_CHAR, bedTemp);
          createChar_P(LCD_DEGREE_CHAR, degree);
          createChar_P(LCD_STR_THERMOMETER[0], thermometer);
          createChar_P(LCD_FEEDRATE_CHAR, feedrate);
          createChar_P(LCD_CLOCK_CHAR, clock);

          #if ENABLED(SDSUPPORT)
            #if ENABLED(LCD_PROGRESS_BAR)
              if (screen_charset == CHARSET_INFO) { // 3 Progress bar characters for info screen
                for (int16_t i = 3; i--;)
                  createChar_P(LCD_STR_PROGRESS[i], progress[i]);
              }
              else
            #endif
              { // SD Card sub-menu special characters
                createChar_P(LCD_UPLEVEL_CHAR, uplevel);
                createChar_P(LCD_STR_REFRESH[0], refresh);
                createChar_P(LCD_STR_FOLDER[0], folder);
              }
          #else
            // With no SD support, only need the uplevel character
            createChar_P(LCD_UPLEVEL_CHAR, uplevel);
          #endif
        }
  }
}

static void lcd_implementation_init(
  #if ENABLED(LCD_PROGRESS_BAR)
    const uint8_t screen_charset=CHARSET_INFO
  #endif
) {

  #if ENABLED(LCD_I2C_TYPE_PCF8575)
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    #ifdef LCD_I2C_PIN_BL
      lcd.setBacklightPin(LCD_I2C_PIN_BL, POSITIVE);
      lcd.setBacklight(HIGH);
    #endif

  #elif ENABLED(LCD_I2C_TYPE_MCP23017)
    lcd.setMCPType(LTI_TYPE_MCP23017);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    lcd_implementation_update_indicators();

  #elif ENABLED(LCD_I2C_TYPE_MCP23008)
    lcd.setMCPType(LTI_TYPE_MCP23008);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);

  #elif ENABLED(LCD_I2C_TYPE_PCA8574)
    lcd.init();
    lcd.backlight();

  #else
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  #endif

  lcd_set_custom_characters(
    #if ENABLED(LCD_PROGRESS_BAR)
      screen_charset
    #endif
  );

  lcd.clear();
}

void lcd_implementation_clear() { lcd.clear(); }

void lcd_print(const char c) { charset_mapper(c); }

void lcd_print(const char *str) { while (*str) lcd.print(*str++); }
void lcd_printPGM(const char *str) { while (const char c = pgm_read_byte(str)) lcd.print(c), ++str; }

void lcd_print_utf(const char *str, uint8_t n=LCD_WIDTH) {
  char c;
  while (n && (c = *str)) n -= charset_mapper(c), ++str;
}

void lcd_printPGM_utf(const char *str, uint8_t n=LCD_WIDTH) {
  char c;
  while (n && (c = pgm_read_byte(str))) n -= charset_mapper(c), ++str;
}

#if ENABLED(SHOW_BOOTSCREEN)

  void lcd_erase_line(const int16_t line) {
    lcd.setCursor(0, line);
    for (uint8_t i = LCD_WIDTH + 1; --i;)
      lcd.write(' ');
  }

  // Scroll the PSTR 'text' in a 'len' wide field for 'time' milliseconds at position col,line
  void lcd_scroll(const int16_t col, const int16_t line, const char* const text, const int16_t len, const int16_t time) {
    char tmp[LCD_WIDTH + 1] = {0};
    int16_t n = max(lcd_strlen_P(text) - len, 0);
    for (int16_t i = 0; i <= n; i++) {
      strncpy_P(tmp, text + i, min(len, LCD_WIDTH));
      lcd.setCursor(col, line);
      lcd_print(tmp);
      delay(time / max(n, 1));
    }
  }

  static void logo_lines(const char* const extra) {
    int16_t indent = (LCD_WIDTH - 8 - lcd_strlen_P(extra)) / 2;
    lcd.setCursor(indent, 0); lcd.print('\x00'); lcd_printPGM(PSTR( "------" ));  lcd.write('\x01');
    lcd.setCursor(indent, 1);                    lcd_printPGM(PSTR("|Marlin|"));  lcd_printPGM(extra);
    lcd.setCursor(indent, 2); lcd.write('\x02'); lcd_printPGM(PSTR( "------" ));  lcd.write('\x03');
  }

  void lcd_bootscreen() {
    lcd_set_custom_characters(CHARSET_BOOT);
    lcd.clear();

    #define LCD_EXTRA_SPACE (LCD_WIDTH-8)

    #define CENTER_OR_SCROLL(STRING,DELAY) \
      lcd_erase_line(3); \
      if (strlen(STRING) <= LCD_WIDTH) { \
        lcd.setCursor((LCD_WIDTH - lcd_strlen_P(PSTR(STRING))) / 2, 3); \
        lcd_printPGM(PSTR(STRING)); \
        safe_delay(DELAY); \
      } \
      else { \
        lcd_scroll(0, 3, PSTR(STRING), LCD_WIDTH, DELAY); \
      }

    #ifdef STRING_SPLASH_LINE1
      //
      // Show the Marlin logo with splash line 1
      //
      if (LCD_EXTRA_SPACE >= strlen(STRING_SPLASH_LINE1) + 1) {
        //
        // Show the Marlin logo, splash line1, and splash line 2
        //
        logo_lines(PSTR(" " STRING_SPLASH_LINE1));
        #ifdef STRING_SPLASH_LINE2
          CENTER_OR_SCROLL(STRING_SPLASH_LINE2, 2000);
        #else
          safe_delay(2000);
        #endif
      }
      else {
        //
        // Show the Marlin logo with splash line 1
        // After a delay show splash line 2, if it exists
        //
        #ifdef STRING_SPLASH_LINE2
          #define _SPLASH_WAIT_1 1500
        #else
          #define _SPLASH_WAIT_1 2000
        #endif
        logo_lines(PSTR(""));
        CENTER_OR_SCROLL(STRING_SPLASH_LINE1, _SPLASH_WAIT_1);
        #ifdef STRING_SPLASH_LINE2
          CENTER_OR_SCROLL(STRING_SPLASH_LINE2, 1500);
        #endif
      }
    #elif defined(STRING_SPLASH_LINE2)
      //
      // Show splash line 2 only, alongside the logo if possible
      //
      if (LCD_EXTRA_SPACE >= strlen(STRING_SPLASH_LINE2) + 1) {
        logo_lines(PSTR(" " STRING_SPLASH_LINE2));
        safe_delay(2000);
      }
      else {
        logo_lines(PSTR(""));
        CENTER_OR_SCROLL(STRING_SPLASH_LINE2, 2000);
      }
    #else
      //
      // Show only the Marlin logo
      //
      logo_lines(PSTR(""));
      safe_delay(2000);
    #endif

    lcd.clear();
    safe_delay(100);
    lcd_set_custom_characters();
    lcd.clear();
  }

#endif // SHOW_BOOTSCREEN

void lcd_kill_screen() {
  lcd.setCursor(0, 0);
  lcd_print_utf(lcd_status_message);
  #if LCD_HEIGHT < 4
    lcd.setCursor(0, 2);
  #else
    lcd.setCursor(0, 2);
    lcd_printPGM(PSTR(MSG_HALTED));
    lcd.setCursor(0, 3);
  #endif
  lcd_printPGM(PSTR(MSG_PLEASE_RESET));
}

FORCE_INLINE void _draw_axis_label(const AxisEnum axis, const char* const pstr, const bool blink) {
  if (blink)
    lcd_printPGM(pstr);
  else {
    if (!axis_homed[axis])
      lcd.write('?');
    else {
      #if DISABLED(HOME_AFTER_DEACTIVATE) && DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[axis])
          lcd.write(' ');
        else
      #endif
          lcd_printPGM(pstr);
    }
  }
}

FORCE_INLINE void _draw_heater_status(const int8_t heater, const char prefix, const bool blink) {
  #if TEMP_SENSOR_BED
    const bool isBed = heater < 0;
  #else
    constexpr bool isBed = false;
  #endif

  const float t1 = (isBed ? thermalManager.degBed()       : thermalManager.degHotend(heater)),
              t2 = (isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater));

  if (prefix >= 0) lcd.print(prefix);

  lcd.print(itostr3(t1 + 0.5));
  lcd.write('/');

  #if !HEATER_IDLE_HANDLER
    UNUSED(blink);
  #else
    const bool is_idle = (!isBed ? thermalManager.is_heater_idle(heater) :
      #if HAS_TEMP_BED
        thermalManager.is_bed_idle()
      #else
        false
      #endif
    );

    if (!blink && is_idle) {
      lcd.write(' ');
      if (t2 >= 10) lcd.write(' ');
      if (t2 >= 100) lcd.write(' ');
    }
    else
  #endif
      lcd.print(itostr3left(t2 + 0.5));

  if (prefix >= 0) {
    lcd.print((char)LCD_DEGREE_CHAR);
    lcd.write(' ');
    if (t2 < 10) lcd.write(' ');
  }
}

#if ENABLED(LCD_PROGRESS_BAR)

  inline void lcd_draw_progress_bar(const uint8_t percent) {
    const int16_t tix = (int16_t)(percent * (LCD_WIDTH) * 3) / 100,
              cel = tix / 3,
              rem = tix % 3;
    uint8_t i = LCD_WIDTH;
    char msg[LCD_WIDTH + 1], b = ' ';
    msg[LCD_WIDTH] = '\0';
    while (i--) {
      if (i == cel - 1)
        b = LCD_STR_PROGRESS[2];
      else if (i == cel && rem != 0)
        b = LCD_STR_PROGRESS[rem - 1];
      msg[i] = b;
    }
    lcd.print(msg);
  }

#endif // LCD_PROGRESS_BAR

/**
Possible status screens:
16x2   |000/000 B000/000|
       |0123456789012345|

16x4   |000/000 B000/000|
       |SD100%  Z 000.00|
       |F100%     T--:--|
       |0123456789012345|

20x2   |T000/000D B000/000D |
       |01234567890123456789|

20x4   |T000/000D B000/000D |
       |X 000 Y 000 Z 000.00|
       |F100%  SD100% T--:--|
       |01234567890123456789|

20x4   |T000/000D B000/000D |
       |T000/000D   Z 000.00|
       |F100%  SD100% T--:--|
       |01234567890123456789|
*/
static void lcd_implementation_status_screen() {
  const bool blink = lcd_blink();

  //
  // Line 1
  //

  lcd.setCursor(0, 0);

  #if LCD_WIDTH < 20

    //
    // Hotend 0 Temperature
    //
    _draw_heater_status(0, -1, blink);

    //
    // Hotend 1 or Bed Temperature
    //
    #if HOTENDS > 1 || TEMP_SENSOR_BED

      lcd.setCursor(8, 0);
      #if HOTENDS > 1
        lcd.print((char)LCD_STR_THERMOMETER[0]);
        _draw_heater_status(1, -1, blink);
      #else
        lcd.print((char)LCD_BEDTEMP_CHAR);
        _draw_heater_status(-1, -1, blink);
      #endif

    #endif // HOTENDS > 1 || TEMP_SENSOR_BED

  #else // LCD_WIDTH >= 20

    //
    // Hotend 0 Temperature
    //
    _draw_heater_status(0, LCD_STR_THERMOMETER[0], blink);

    //
    // Hotend 1 or Bed Temperature
    //
    #if HOTENDS > 1 || TEMP_SENSOR_BED
      lcd.setCursor(10, 0);
      #if HOTENDS > 1
        _draw_heater_status(1, LCD_STR_THERMOMETER[0], blink);
      #else
        _draw_heater_status(-1, (
          #if HAS_LEVELING
            planner.leveling_active && blink ? '_' :
          #endif
          LCD_BEDTEMP_CHAR
        ), blink);
      #endif

    #endif // HOTENDS > 1 || TEMP_SENSOR_BED != 0

  #endif // LCD_WIDTH >= 20

  //
  // Line 2
  //

  #if LCD_HEIGHT > 2

    #if LCD_WIDTH < 20

      #if ENABLED(SDSUPPORT)
        lcd.setCursor(0, 2);
        lcd_printPGM(PSTR("SD"));
        if (IS_SD_PRINTING)
          lcd.print(itostr3(card.percentDone()));
        else
          lcd_printPGM(PSTR("---"));
          lcd.write('%');
      #endif // SDSUPPORT

    #else // LCD_WIDTH >= 20

      lcd.setCursor(0, 1);

      // If the first line has two extruder temps,
      // show more temperatures on the next line
      // instead of 

      #if HOTENDS > 2 || (HOTENDS > 1 && TEMP_SENSOR_BED)

        #if HOTENDS > 2
          _draw_heater_status(2, LCD_STR_THERMOMETER[0], blink);
          lcd.setCursor(10, 1);
        #endif

        _draw_heater_status(-1, (
          #if HAS_LEVELING
            planner.leveling_active && blink ? '_' :
          #endif
          LCD_BEDTEMP_CHAR
        ), blink);

      #else // HOTENDS <= 2 && (HOTENDS <= 1 || !TEMP_SENSOR_BED)
        // Before homing the axis letters are blinking 'X' <-> '?'.
        // When axis is homed but axis_known_position is false the axis letters are blinking 'X' <-> ' '.
        // When everything is ok you see a constant 'X'.

        _draw_axis_label(X_AXIS, PSTR(MSG_X), blink);
        lcd.print(ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])));

        lcd.write(' ');

        _draw_axis_label(Y_AXIS, PSTR(MSG_Y), blink);
        lcd.print(ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])));

      #endif // HOTENDS <= 2 && (HOTENDS <= 1 || !TEMP_SENSOR_BED)

    #endif // LCD_WIDTH >= 20

    lcd.setCursor(LCD_WIDTH - 8, 1);
    _draw_axis_label(Z_AXIS, PSTR(MSG_Z), blink);
    lcd.print(ftostr52sp(FIXFLOAT(current_position[Z_AXIS])));

    #if HAS_LEVELING && !TEMP_SENSOR_BED
      lcd.write(planner.leveling_active || blink ? '_' : ' ');
    #endif

  #endif // LCD_HEIGHT > 2

  //
  // Line 3
  //

  #if LCD_HEIGHT > 3

    lcd.setCursor(0, 2);
    lcd.print((char)LCD_FEEDRATE_CHAR);
    lcd.print(itostr3(feedrate_percentage));
    lcd.write('%');

    #if LCD_WIDTH >= 20 && ENABLED(SDSUPPORT)

      lcd.setCursor(7, 2);
      lcd_printPGM(PSTR("SD"));
      if (IS_SD_PRINTING)
        lcd.print(itostr3(card.percentDone()));
      else
        lcd_printPGM(PSTR("---"));
      lcd.write('%');

    #endif // LCD_WIDTH >= 20 && SDSUPPORT

    char buffer[10];
    duration_t elapsed = print_job_timer.duration();
    uint8_t len = elapsed.toDigital(buffer);

    lcd.setCursor(LCD_WIDTH - len - 1, 2);
    lcd.print((char)LCD_CLOCK_CHAR);
    lcd_print(buffer);

  #endif // LCD_HEIGHT > 3

  //
  // Last Line
  // Status Message (which may be a Progress Bar or Filament display)
  //

  lcd.setCursor(0, LCD_HEIGHT - 1);

  #if ENABLED(LCD_PROGRESS_BAR)

    // Draw the progress bar if the message has shown long enough
    // or if there is no message set.
    #if DISABLED(LCD_SET_PROGRESS_MANUALLY)
      const uint8_t progress_bar_percent = card.percentDone();
    #endif
    if (progress_bar_percent > 2 && (ELAPSED(millis(), progress_bar_ms + PROGRESS_BAR_MSG_TIME) || !lcd_status_message[0]))
      return lcd_draw_progress_bar(progress_bar_percent);

  #elif ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)

    // Show Filament Diameter and Volumetric Multiplier %
    // After allowing lcd_status_message to show for 5 seconds
    if (ELAPSED(millis(), previous_lcd_status_ms + 5000UL)) {
      lcd_printPGM(PSTR("Dia "));
      lcd.print(ftostr12ns(filament_width_meas));
      lcd_printPGM(PSTR(" V"));
      lcd.print(itostr3(100.0 * (
          parser.volumetric_enabled
            ? planner.volumetric_area_nominal / planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
            : planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
        )
      ));
      lcd.write('%');
      return;
    }

  #endif // FILAMENT_LCD_DISPLAY && SDSUPPORT

  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    static bool last_blink = false;
    const uint8_t slen = lcd_strlen(lcd_status_message);
    const char *stat = lcd_status_message + status_scroll_pos;
    if (slen <= LCD_WIDTH)
      lcd_print_utf(stat);                                      // The string isn't scrolling
    else {
      if (status_scroll_pos <= slen - LCD_WIDTH)
        lcd_print_utf(stat);                                    // The string fills the screen
      else {
        uint8_t chars = LCD_WIDTH;
        if (status_scroll_pos < slen) {                         // First string still visible
          lcd_print_utf(stat);                                  // The string leaves space
          chars -= slen - status_scroll_pos;                    // Amount of space left
        }
        lcd.write('.');                                         // Always at 1+ spaces left, draw a dot
        if (--chars) {
          if (status_scroll_pos < slen + 1)                     // Draw a second dot if there's space
            --chars, lcd.write('.');
          if (chars) lcd_print_utf(lcd_status_message, chars);  // Print a second copy of the message
        }
      }
      if (last_blink != blink) {
        last_blink = blink;
        // Skip any non-printing bytes
        if (status_scroll_pos < slen) while (!PRINTABLE(lcd_status_message[status_scroll_pos])) status_scroll_pos++;
        if (++status_scroll_pos >= slen + 2) status_scroll_pos = 0;
      }
    }
  #else
    lcd_print_utf(lcd_status_message);
  #endif
}

#if ENABLED(ULTIPANEL)

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    static void lcd_implementation_hotend_status(const uint8_t row) {
      if (row < LCD_HEIGHT) {
        lcd.setCursor(LCD_WIDTH - 9, row);
        _draw_heater_status(active_extruder, LCD_STR_THERMOMETER[0], lcd_blink());
      }
    }

  #endif // ADVANCED_PAUSE_FEATURE

  static void lcd_implementation_drawmenu_static(const uint8_t row, const char* pstr, const bool center=true, const bool invert=false, const char *valstr=NULL) {
    UNUSED(invert);
    char c;
    int8_t n = LCD_WIDTH;
    lcd.setCursor(0, row);
    if (center && !valstr) {
      int8_t pad = (LCD_WIDTH - lcd_strlen_P(pstr)) / 2;
      while (--pad >= 0) { lcd.write(' '); n--; }
    }
    while (n > 0 && (c = pgm_read_byte(pstr))) {
      n -= charset_mapper(c);
      pstr++;
    }
    if (valstr) while (n > 0 && (c = *valstr)) {
      n -= charset_mapper(c);
      valstr++;
    }
    while (n-- > 0) lcd.write(' ');
  }

  static void lcd_implementation_drawmenu_generic(const bool sel, const uint8_t row, const char* pstr, const char pre_char, const char post_char) {
    char c;
    uint8_t n = LCD_WIDTH - 2;
    lcd.setCursor(0, row);
    lcd.print(sel ? pre_char : ' ');
    while ((c = pgm_read_byte(pstr)) && n > 0) {
      n -= charset_mapper(c);
      pstr++;
    }
    while (n--) lcd.write(' ');
    lcd.print(post_char);
  }

  static void lcd_implementation_drawmenu_setting_edit_generic(const bool sel, const uint8_t row, const char* pstr, const char pre_char, const char* const data) {
    char c;
    uint8_t n = LCD_WIDTH - 2 - lcd_strlen(data);
    lcd.setCursor(0, row);
    lcd.print(sel ? pre_char : ' ');
    while ((c = pgm_read_byte(pstr)) && n > 0) {
      n -= charset_mapper(c);
      pstr++;
    }
    lcd.write(':');
    while (n--) lcd.write(' ');
    lcd_print(data);
  }
  static void lcd_implementation_drawmenu_setting_edit_generic_P(const bool sel, const uint8_t row, const char* pstr, const char pre_char, const char* const data) {
    char c;
    uint8_t n = LCD_WIDTH - 2 - lcd_strlen_P(data);
    lcd.setCursor(0, row);
    lcd.print(sel ? pre_char : ' ');
    while ((c = pgm_read_byte(pstr)) && n > 0) {
      n -= charset_mapper(c);
      pstr++;
    }
    lcd.write(':');
    while (n--) lcd.write(' ');
    lcd_printPGM(data);
  }

  #define DRAWMENU_SETTING_EDIT_GENERIC(_src) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', _src)
  #define DRAW_BOOL_SETTING(sel, row, pstr, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  void lcd_implementation_drawedit(const char* pstr, const char* const value=NULL) {
    lcd.setCursor(1, 1);
    lcd_printPGM(pstr);
    if (value != NULL) {
      lcd.write(':');
      const uint8_t valrow = (lcd_strlen_P(pstr) + 1 + lcd_strlen(value) + 1) > (LCD_WIDTH - 2) ? 2 : 1;  // Value on the next row if it won't fit
      lcd.setCursor((LCD_WIDTH - 1) - (lcd_strlen(value) + 1), valrow);                                   // Right-justified, padded by spaces
      lcd.write(' ');                                                                                     // overwrite char if value gets shorter
      lcd_print(value);
    }
  }

  #if ENABLED(SDSUPPORT)

    static void lcd_implementation_drawmenu_sd(const bool sel, const uint8_t row, const char* const pstr, const char* filename, char* const longFilename, const uint8_t concat, const char post_char) {
      UNUSED(pstr);
      lcd.setCursor(0, row);
      lcd.print(sel ? '>' : ' ');

      uint8_t n = LCD_WIDTH - concat;
      const char *outstr = longFilename[0] ? longFilename : filename;
      if (longFilename[0]) {
        #if ENABLED(SCROLL_LONG_FILENAMES)
          if (sel) {
            uint8_t name_hash = row;
            for (uint8_t l = FILENAME_LENGTH; l--;)
              name_hash = ((name_hash << 1) | (name_hash >> 7)) ^ filename[l];  // rotate, xor
            if (filename_scroll_hash != name_hash) {                            // If the hash changed...
              filename_scroll_hash = name_hash;                                 // Save the new hash
              filename_scroll_max = max(0, lcd_strlen(longFilename) - n);  // Update the scroll limit
              filename_scroll_pos = 0;                                          // Reset scroll to the start
              lcd_status_update_delay = 8;                                      // Don't scroll right away
            }
            outstr += filename_scroll_pos;
          }
        #else
          longFilename[n] = '\0'; // cutoff at screen edge
        #endif
      }

      char c;
      while (n && (c = *outstr)) {
        n -= charset_mapper(c);
        ++outstr;
      }
      while (n) { --n; lcd.write(' '); }

      lcd.print(post_char);
    }

    static void lcd_implementation_drawmenu_sdfile(const bool sel, const uint8_t row, const char* pstr, const char* filename, char* const longFilename) {
      lcd_implementation_drawmenu_sd(sel, row, pstr, filename, longFilename, 2, ' ');
    }

    static void lcd_implementation_drawmenu_sddirectory(const bool sel, const uint8_t row, const char* pstr, const char* filename, char* const longFilename) {
      lcd_implementation_drawmenu_sd(sel, row, pstr, filename, longFilename, 2, LCD_STR_FOLDER[0]);
    }

  #endif // SDSUPPORT

  #define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_UPLEVEL_CHAR, LCD_UPLEVEL_CHAR)
  #define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
  #define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
  #define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

  #if ENABLED(LCD_HAS_SLOW_BUTTONS)

    extern millis_t next_button_update_ms;

    static uint8_t lcd_implementation_read_slow_buttons() {
      #if ENABLED(LCD_I2C_TYPE_MCP23017)
        // Reading these buttons this is likely to be too slow to call inside interrupt context
        // so they are called during normal lcd_update
        uint8_t slow_bits = lcd.readButtons() << B_I2C_BTN_OFFSET;
        #if ENABLED(LCD_I2C_VIKI)
          if ((slow_bits & (B_MI | B_RI)) && PENDING(millis(), next_button_update_ms)) // LCD clicked
            slow_bits &= ~(B_MI | B_RI); // Disable LCD clicked buttons if screen is updated
        #endif // LCD_I2C_VIKI
        return slow_bits;
      #endif // LCD_I2C_TYPE_MCP23017
    }

  #endif // LCD_HAS_SLOW_BUTTONS

  #if ENABLED(LCD_HAS_STATUS_INDICATORS)

    static void lcd_implementation_update_indicators() {
      // Set the LEDS - referred to as backlights by the LiquidTWI2 library
      static uint8_t ledsprev = 0;
      uint8_t leds = 0;

      if (thermalManager.degTargetBed() > 0) leds |= LED_A;

      if (thermalManager.degTargetHotend(0) > 0) leds |= LED_B;

      #if FAN_COUNT > 0
        if (0
          #if HAS_FAN0
            || fanSpeeds[0]
          #endif
          #if HAS_FAN1
            || fanSpeeds[1]
          #endif
          #if HAS_FAN2
            || fanSpeeds[2]
          #endif
        ) leds |= LED_C;
      #endif // FAN_COUNT > 0

      #if HOTENDS > 1
        if (thermalManager.degTargetHotend(1) > 0) leds |= LED_C;
      #endif

      if (leds != ledsprev) {
        lcd.setBacklight(leds);
        ledsprev = leds;
      }
    }

  #endif // LCD_HAS_STATUS_INDICATORS

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    /**
      Possible map screens:

      16x2   |X000.00  Y000.00|
             |(00,00)  Z00.000|

      20x2   | X:000.00  Y:000.00 |
             | (00,00)   Z:00.000 |

      16x4   |+-------+(00,00)|
             ||       |X000.00|
             ||       |Y000.00|
             |+-------+Z00.000|

      20x4   | +-------+  (00,00) |
             | |       |  X:000.00|
             | |       |  Y:000.00|
             | +-------+  Z:00.000|
    */

    typedef struct {
      uint8_t custom_char_bits[ULTRA_Y_PIXELS_PER_CHAR];
    } custom_char;

    typedef struct {
      uint8_t column, row;
      uint8_t y_pixel_offset, x_pixel_offset;
      uint8_t x_pixel_mask;
    } coordinate;

    void add_edges_to_custom_char(custom_char * const custom, coordinate * const ul, coordinate * const lr, coordinate * const brc, const uint8_t cell_location);
    FORCE_INLINE static void clear_custom_char(custom_char * const cc) { ZERO(cc->custom_char_bits); }

    /*
    // This debug routine should be deleted by anybody that sees it.  It doesn't belong here
    // But I'm leaving it for now until we know the 20x4 Radar Map is working right.
    // We may need it again if any funny lines show up on the mesh points.
    void dump_custom_char(char *title, custom_char *c) {
      SERIAL_PROTOCOLLN(title);
      for (uint8_t j = 0; j < 8; j++) {
        for (uint8_t i = 7; i >= 0; i--)
          SERIAL_PROTOCOLCHAR(TEST(c->custom_char_bits[j], i) ? '1' : '0');
        SERIAL_EOL();
      }
      SERIAL_EOL();
    }
    //*/

    coordinate pixel_location(int16_t x, int16_t y) {
      coordinate ret_val;
      int16_t xp, yp, r, c;

      x++; y++; // +1 because lines on the left and top

      c = x / (ULTRA_X_PIXELS_PER_CHAR);
      r = y / (ULTRA_Y_PIXELS_PER_CHAR);

      ret_val.column = c;
      ret_val.row    = r;

      xp = x - c * (ULTRA_X_PIXELS_PER_CHAR);   // get the pixel offsets into the character cell
      xp = ULTRA_X_PIXELS_PER_CHAR - 1 - xp;    // column within relevant character cell (0 on the right)
      yp = y - r * (ULTRA_Y_PIXELS_PER_CHAR);

      ret_val.x_pixel_mask   = _BV(xp);
      ret_val.x_pixel_offset = xp;
      ret_val.y_pixel_offset = yp;
      return ret_val;
    }

    inline coordinate pixel_location(const uint8_t x, const uint8_t y) { return pixel_location((int16_t)x, (int16_t)y); }

    void lcd_implementation_ubl_plot(const uint8_t x, const uint8_t inverted_y) {

      #if LCD_WIDTH >= 20
        #define _LCD_W_POS 12
        #define _PLOT_X 1
        #define _MAP_X 3
        #define _LABEL(C,X,Y) lcd.setCursor(X, Y); lcd.print(C)
        #define _XLABEL(X,Y) _LABEL("X:",X,Y)
        #define _YLABEL(X,Y) _LABEL("Y:",X,Y)
        #define _ZLABEL(X,Y) _LABEL("Z:",X,Y)
      #else
        #define _LCD_W_POS 8
        #define _PLOT_X 0
        #define _MAP_X 1
        #define _LABEL(X,Y,C) lcd.setCursor(X, Y); lcd.write(C)
        #define _XLABEL(X,Y) _LABEL('X',X,Y)
        #define _YLABEL(X,Y) _LABEL('Y',X,Y)
        #define _ZLABEL(X,Y) _LABEL('Z',X,Y)
      #endif

      #if LCD_HEIGHT <= 3   // 16x2 or 20x2 display

        /**
         * Show X and Y positions
         */
        _XLABEL(_PLOT_X, 0);
        lcd.print(ftostr32(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x]))));

        _YLABEL(_LCD_W_POS, 0);
        lcd.print(ftostr32(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[inverted_y]))));

        lcd.setCursor(_PLOT_X, 0);

      #else // 16x4 or 20x4 display

        coordinate upper_left, lower_right, bottom_right_corner;
        custom_char new_char;
        uint8_t i, j, k, l, m, n, n_rows, n_cols, y,
                bottom_line, right_edge,
                x_map_pixels, y_map_pixels,
                pixels_per_x_mesh_pnt, pixels_per_y_mesh_pnt,
                suppress_x_offset = 0, suppress_y_offset = 0;

        y = GRID_MAX_POINTS_Y - inverted_y - 1;

        upper_left.column  = 0;
        upper_left.row     = 0;
        lower_right.column = 0;
        lower_right.row    = 0;

        lcd_implementation_clear();

        x_map_pixels = (ULTRA_X_PIXELS_PER_CHAR) * (ULTRA_COLUMNS_FOR_MESH_MAP) - 2;  // minus 2 because we are drawing a box around the map
        y_map_pixels = (ULTRA_Y_PIXELS_PER_CHAR) * (ULTRA_ROWS_FOR_MESH_MAP) - 2;

        pixels_per_x_mesh_pnt = x_map_pixels / (GRID_MAX_POINTS_X);
        pixels_per_y_mesh_pnt = y_map_pixels / (GRID_MAX_POINTS_Y);

        if (pixels_per_x_mesh_pnt >= ULTRA_X_PIXELS_PER_CHAR) {         // There are only 2 custom characters available, so the X
          pixels_per_x_mesh_pnt = ULTRA_X_PIXELS_PER_CHAR;              // size of the mesh point needs to fit within them independent
          suppress_x_offset = 1;                                        // of where the starting pixel is located.
        }

        if (pixels_per_y_mesh_pnt >= ULTRA_Y_PIXELS_PER_CHAR) {         // There are only 2 custom characters available, so the Y
          pixels_per_y_mesh_pnt = ULTRA_Y_PIXELS_PER_CHAR;              // size of the mesh point needs to fit within them independent
          suppress_y_offset = 1;                                        // of where the starting pixel is located.
        }

        x_map_pixels = pixels_per_x_mesh_pnt * (GRID_MAX_POINTS_X);     // now we have the right number of pixels to make both
        y_map_pixels = pixels_per_y_mesh_pnt * (GRID_MAX_POINTS_Y);     // directions fit nicely

        right_edge = pixels_per_x_mesh_pnt * (GRID_MAX_POINTS_X) + 1;   // find location of right edge within the character cell
        bottom_line= pixels_per_y_mesh_pnt * (GRID_MAX_POINTS_Y) + 1;   // find location of bottome line within the character cell

        n_rows = bottom_line / (ULTRA_Y_PIXELS_PER_CHAR) + 1;
        n_cols = right_edge / (ULTRA_X_PIXELS_PER_CHAR) + 1;

        for (i = 0; i < n_cols; i++) {
          lcd.setCursor(i, 0);
          lcd.print((char)0x00);                     // top line of the box

          lcd.setCursor(i, n_rows - 1);
          lcd.write(0x01);                           // bottom line of the box
        }

        for (j = 0; j < n_rows; j++) {
          lcd.setCursor(0, j);
          lcd.write(0x02);                           // Left edge of the box
          lcd.setCursor(n_cols - 1, j);
          lcd.write(0x03);                           // right edge of the box
        }

        /**
         * If the entire 4th row is not in use, do not put vertical bars all the way down to the bottom of the display
         */

        k = pixels_per_y_mesh_pnt * (GRID_MAX_POINTS_Y) + 2;
        l = (ULTRA_Y_PIXELS_PER_CHAR) * n_rows;
        if (l > k && l - k >= (ULTRA_Y_PIXELS_PER_CHAR) / 2) {
          lcd.setCursor(0, n_rows - 1);            // left edge of the box
          lcd.write(' ');
          lcd.setCursor(n_cols - 1, n_rows - 1);   // right edge of the box
          lcd.write(' ');
        }

        clear_custom_char(&new_char);
        new_char.custom_char_bits[0] = 0B11111U;              // char #0 is used for the top line of the box
        lcd.createChar(0, (uint8_t*)&new_char);

        clear_custom_char(&new_char);
        k = (GRID_MAX_POINTS_Y) * pixels_per_y_mesh_pnt + 1;  // row of pixels for the bottom box line
        l = k % (ULTRA_Y_PIXELS_PER_CHAR);                    // row within relevant character cell
        new_char.custom_char_bits[l] = 0B11111U;              // char #1 is used for the bottom line of the box
        lcd.createChar(1, (uint8_t*)&new_char);

        clear_custom_char(&new_char);
        for (j = 0; j < ULTRA_Y_PIXELS_PER_CHAR; j++)
          new_char.custom_char_bits[j] = 0B10000U;            // char #2 is used for the left edge of the box
        lcd.createChar(2, (uint8_t*)&new_char);

        clear_custom_char(&new_char);
        m = (GRID_MAX_POINTS_X) * pixels_per_x_mesh_pnt + 1;  // Column of pixels for the right box line
        n = m % (ULTRA_X_PIXELS_PER_CHAR);                    // Column within relevant character cell
        i = ULTRA_X_PIXELS_PER_CHAR - 1 - n;                  // Column within relevant character cell (0 on the right)
        for (j = 0; j < ULTRA_Y_PIXELS_PER_CHAR; j++)
          new_char.custom_char_bits[j] = (uint8_t)_BV(i);     // Char #3 is used for the right edge of the box
        lcd.createChar(3, (uint8_t*)&new_char);

        i = x * pixels_per_x_mesh_pnt - suppress_x_offset;
        j = y * pixels_per_y_mesh_pnt - suppress_y_offset;
        upper_left = pixel_location(i, j);

        k = (x + 1) * pixels_per_x_mesh_pnt - 1 - suppress_x_offset;
        l = (y + 1) * pixels_per_y_mesh_pnt - 1 - suppress_y_offset;
        lower_right = pixel_location(k, l);

        bottom_right_corner = pixel_location(x_map_pixels, y_map_pixels);

        /**
         * First, handle the simple case where everything is within a single character cell.
         * If part of the Mesh Plot is outside of this character cell, we will follow up
         * and deal with that next.
         */

        //dump_custom_char("at entry:", &new_char);

        clear_custom_char(&new_char);
        const uint8_t ypix = min(upper_left.y_pixel_offset + pixels_per_y_mesh_pnt, ULTRA_Y_PIXELS_PER_CHAR);
        for (j = upper_left.y_pixel_offset; j < ypix; j++) {
          i = upper_left.x_pixel_mask;
          for (k = 0; k < pixels_per_x_mesh_pnt; k++) {
            new_char.custom_char_bits[j] |= i;
            i >>= 1;
          }
        }
        //dump_custom_char("after loops:", &new_char);

        add_edges_to_custom_char(&new_char, &upper_left, &lower_right, &bottom_right_corner, TOP_LEFT);
        //dump_custom_char("after add edges", &new_char);
        lcd.createChar(4, (uint8_t*)&new_char);

        lcd.setCursor(upper_left.column, upper_left.row);
        lcd.write(0x04);
        //dump_custom_char("after lcd update:", &new_char);

        /**
         * Next, check for two side by side character cells being used to display the Mesh Point
         * If found...  do the right hand character cell next.
         */
        if (upper_left.column == lower_right.column - 1) {
          l = upper_left.x_pixel_offset;
          clear_custom_char(&new_char);
          for (j = upper_left.y_pixel_offset; j < ypix; j++) {
            i = _BV(ULTRA_X_PIXELS_PER_CHAR - 1);                  // Fill in the left side of the right character cell
            for (k = 0; k < pixels_per_x_mesh_pnt - 1 - l; k++) {
              new_char.custom_char_bits[j] |= i;
              i >>= 1;
            }
          }
          add_edges_to_custom_char(&new_char, &upper_left, &lower_right, &bottom_right_corner, TOP_RIGHT);

          lcd.createChar(5, (uint8_t *) &new_char);

          lcd.setCursor(lower_right.column, upper_left.row);
          lcd.write(0x05);
        }

        /**
         * Next, check for two character cells stacked on top of each other being used to display the Mesh Point
         */
        if (upper_left.row == lower_right.row - 1) {
          l = ULTRA_Y_PIXELS_PER_CHAR - upper_left.y_pixel_offset;  // Number of pixel rows in top character cell
          k = pixels_per_y_mesh_pnt - l;                            // Number of pixel rows in bottom character cell
          clear_custom_char(&new_char);
          for (j = 0; j < k; j++) {
            i = upper_left.x_pixel_mask;
            for (m = 0; m < pixels_per_x_mesh_pnt; m++) {           // Fill in the top side of the bottom character cell
              new_char.custom_char_bits[j] |= i;
              if (!(i >>= 1)) break;
            }
          }
          add_edges_to_custom_char(&new_char, &upper_left, &lower_right, &bottom_right_corner, LOWER_LEFT);
          lcd.createChar(6, (uint8_t *) &new_char);

          lcd.setCursor(upper_left.column, lower_right.row);
          lcd.write(0x06);
        }

        /**
         * Next, check for four character cells being used to display the Mesh Point.  If that is
         * what is here, we work to fill in the character cell that is down one and to the right one
         * from the upper_left character cell.
         */

        if (upper_left.column == lower_right.column - 1 && upper_left.row == lower_right.row - 1) {
          l = ULTRA_Y_PIXELS_PER_CHAR - upper_left.y_pixel_offset;   // Number of pixel rows in top character cell
          k = pixels_per_y_mesh_pnt - l;                             // Number of pixel rows in bottom character cell
          clear_custom_char(&new_char);
          for (j = 0; j < k; j++) {
            l = upper_left.x_pixel_offset;
            i = _BV(ULTRA_X_PIXELS_PER_CHAR - 1);                    // Fill in the left side of the right character cell
            for (m = 0; m < pixels_per_x_mesh_pnt - 1 - l; m++) {    // Fill in the top side of the bottom character cell
              new_char.custom_char_bits[j] |= i;
              i >>= 1;
            }
          }
          add_edges_to_custom_char(&new_char, &upper_left, &lower_right, &bottom_right_corner, LOWER_RIGHT);
          lcd.createChar(7, (uint8_t*)&new_char);

          lcd.setCursor(lower_right.column, lower_right.row);
          lcd.write(0x07);
        }

      #endif

      /**
       * Print plot position
       */
      lcd.setCursor(_LCD_W_POS, 0);
      lcd.write('(');
      lcd.print(x);
      lcd.write(',');
      lcd.print(inverted_y);
      lcd.write(')');

      #if LCD_HEIGHT <= 3   // 16x2 or 20x2 display

        /**
         * Print Z values
         */
        _ZLABEL(_LCD_W_POS, 1);
        if (!isnan(ubl.z_values[x][inverted_y]))
          lcd.print(ftostr43sign(ubl.z_values[x][inverted_y]));
        else
          lcd_printPGM(PSTR(" -----"));

      #else                 // 16x4 or 20x4 display

        /**
         * Show all values at right of screen
         */
        _XLABEL(_LCD_W_POS, 1);
        lcd.print(ftostr32(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x]))));
        _YLABEL(_LCD_W_POS, 2);
        lcd.print(ftostr32(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[inverted_y]))));

        /**
         * Show the location value
         */
        _ZLABEL(_LCD_W_POS, 3);
        if (!isnan(ubl.z_values[x][inverted_y]))
          lcd.print(ftostr43sign(ubl.z_values[x][inverted_y]));
        else
          lcd_printPGM(PSTR(" -----"));

      #endif // LCD_HEIGHT > 3
    }

    void add_edges_to_custom_char(custom_char * const custom, coordinate * const ul, coordinate * const lr, coordinate * const brc, uint8_t cell_location) {
      uint8_t i, k;
      int16_t n_rows = lr->row    - ul->row    + 1,
              n_cols = lr->column - ul->column + 1;

      /**
       * Check if Top line of box needs to be filled in
       */
      if (ul->row == 0 && ((cell_location & TOP_LEFT) || (cell_location & TOP_RIGHT))) {   // Only fill in the top line for the top character cells

        if (n_cols == 1) {
          if (ul->column != brc->column)
            custom->custom_char_bits[0] = 0xFF;                             // Single column in middle
          else
            for (i = brc->x_pixel_offset; i < ULTRA_X_PIXELS_PER_CHAR; i++) // Single column on right side
              SBI(custom->custom_char_bits[0], i);
        }
        else if ((cell_location & TOP_LEFT) || lr->column != brc->column)   // Multiple column in the middle or with right cell in middle
          custom->custom_char_bits[0] = 0xFF;
        else
          for (i = brc->x_pixel_offset; i < ULTRA_X_PIXELS_PER_CHAR; i++)
            SBI(custom->custom_char_bits[0], i);
      }

      /**
       * Check if left line of box needs to be filled in
       */
      if ((cell_location & TOP_LEFT) || (cell_location & LOWER_LEFT)) {
        if (ul->column == 0) {                                              // Left column of characters on LCD Display
          k = ul->row == brc->row ? brc->y_pixel_offset : ULTRA_Y_PIXELS_PER_CHAR; // If it isn't the last row... do the full character cell
          for (i = 0; i < k; i++)
            SBI(custom->custom_char_bits[i], ULTRA_X_PIXELS_PER_CHAR - 1);
        }
      }

      /**
       * Check if bottom line of box needs to be filled in
       */

      // Single row of mesh plot cells
      if (n_rows == 1 /* && (cell_location == TOP_LEFT || cell_location == TOP_RIGHT) */ && ul->row == brc->row) {
        if (n_cols == 1)                                                    // Single row, single column case
          k = ul->column == brc->column ? brc->x_pixel_mask : 0x01;
        else if (cell_location & TOP_RIGHT)                                 // Single row, multiple column case
          k = lr->column == brc->column ? brc->x_pixel_mask : 0x01;
        else                                                                // Single row, left of multiple columns
          k = 0x01;
        while (k < _BV(ULTRA_X_PIXELS_PER_CHAR)) {
          custom->custom_char_bits[brc->y_pixel_offset] |= k;
          k <<= 1;
        }
      }

      // Double row of characters on LCD Display
      // And this is a bottom custom character
      if (n_rows == 2 && (cell_location == LOWER_LEFT || cell_location == LOWER_RIGHT) && lr->row == brc->row) {
        if (n_cols == 1)                                                  // Double row, single column case
          k = ul->column == brc->column ? brc->x_pixel_mask : 0x01;
        else if (cell_location & LOWER_RIGHT)                             // Double row, multiple column case
          k = lr->column == brc->column ? brc->x_pixel_mask : 0x01;
        else                                                              // Double row, left of multiple columns
          k = 0x01;
        while (k < _BV(ULTRA_X_PIXELS_PER_CHAR)) {
          custom->custom_char_bits[brc->y_pixel_offset] |= k;
          k <<= 1;
        }
      }

      /**
       * Check if right line of box needs to be filled in
       */
      // Nothing to do if the lower right part of the mesh pnt isn't in the same column as the box line
      if (lr->column == brc->column) {
        // This mesh point is in the same character cell as the right box line
        if (ul->column == brc->column || (cell_location & TOP_RIGHT) || (cell_location & LOWER_RIGHT)) {
          // If not the last row... do the full character cell
          k = ul->row == brc->row ? brc->y_pixel_offset : ULTRA_Y_PIXELS_PER_CHAR;
          for (i = 0; i < k; i++) custom->custom_char_bits[i] |= brc->x_pixel_mask;
        }
      }
    }

  #endif // AUTO_BED_LEVELING_UBL

#endif // ULTIPANEL

#endif // ULTRALCD_IMPL_HD44780_H
