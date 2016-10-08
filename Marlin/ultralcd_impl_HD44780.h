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

#endif //ULTIPANEL

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
  LCD_CLASS lcd(SR_DATA_PIN, SR_CLK_PIN);
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
  static millis_t progress_bar_ms = 0;
  #if PROGRESS_MSG_EXPIRE > 0
    static millis_t expire_status_ms = 0;
  #endif
  #define LCD_STR_PROGRESS  "\x03\x04\x05"
#endif

#if ENABLED(LCD_HAS_STATUS_INDICATORS)
  static void lcd_implementation_update_indicators();
#endif

static void lcd_set_custom_characters(
  #if ENABLED(LCD_PROGRESS_BAR)
    bool info_screen_charset = true
  #endif
) {
  static byte bedTemp[8] = {
    B00000,
    B11111,
    B10101,
    B10001,
    B10101,
    B11111,
    B00000,
    B00000
  }; //thanks Sonny Mounicou
  static byte degree[8] = {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };
  static byte thermometer[8] = {
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B10001,
    B01110
  };
  static byte feedrate[8] = {
    B11100,
    B10000,
    B11000,
    B10111,
    B00101,
    B00110,
    B00101,
    B00000
  }; //thanks Sonny Mounicou
  static byte clock[8] = {
    B00000,
    B01110,
    B10011,
    B10101,
    B10001,
    B01110,
    B00000,
    B00000
  }; //thanks Sonny Mounicou

  lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
  lcd.createChar(LCD_STR_DEGREE[0], degree);
  lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
  lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
  lcd.createChar(LCD_STR_CLOCK[0], clock);

  #if ENABLED(SDSUPPORT)
    static byte uplevel[8] = {
      B00100,
      B01110,
      B11111,
      B00100,
      B11100,
      B00000,
      B00000,
      B00000
    }; //thanks joris
    static byte refresh[8] = {
      B00000,
      B00110,
      B11001,
      B11000,
      B00011,
      B10011,
      B01100,
      B00000,
    }; //thanks joris
    static byte folder[8] = {
      B00000,
      B11100,
      B11111,
      B10001,
      B10001,
      B11111,
      B00000,
      B00000
    }; //thanks joris

    #if ENABLED(LCD_PROGRESS_BAR)
      static byte progress[3][8] = { {
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
      static bool char_mode = false;
      if (info_screen_charset != char_mode) {
        char_mode = info_screen_charset;
        if (info_screen_charset) { // Progress bar characters for info screen
          for (int i = 3; i--;) lcd.createChar(LCD_STR_PROGRESS[i], progress[i]);
        }
        else { // Custom characters for submenus
          lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
          lcd.createChar(LCD_STR_REFRESH[0], refresh);
          lcd.createChar(LCD_STR_FOLDER[0], folder);
        }
      }
    #else
      lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
      lcd.createChar(LCD_STR_REFRESH[0], refresh);
      lcd.createChar(LCD_STR_FOLDER[0], folder);
    #endif

  #endif
}

static void lcd_implementation_init(
  #if ENABLED(LCD_PROGRESS_BAR)
    bool info_screen_charset = true
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
      info_screen_charset
    #endif
  );

  lcd.clear();
}

static void lcd_implementation_clear() { lcd.clear(); }

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
void lcd_printPGM(const char *str) {
  for (; char c = pgm_read_byte(str); ++str) charset_mapper(c);
}

void lcd_print(const char* str) {
  for (uint8_t i = 0; char c = str[i]; ++i) charset_mapper(c);
}

void lcd_print(char c) { charset_mapper(c); }

#if ENABLED(SHOW_BOOTSCREEN)

  void lcd_erase_line(int line) {
    lcd.setCursor(0, line);
    for (int i = 0; i < LCD_WIDTH; i++)
      lcd_print(' ');
  }

  // Scroll the PSTR 'text' in a 'len' wide field for 'time' milliseconds at position col,line
  void lcd_scroll(int col, int line, const char* text, int len, int time) {
    char tmp[LCD_WIDTH + 1] = {0};
    int n = max(lcd_strlen_P(text) - len, 0);
    for (int i = 0; i <= n; i++) {
      strncpy_P(tmp, text + i, min(len, LCD_WIDTH));
      lcd.setCursor(col, line);
      lcd_print(tmp);
      delay(time / max(n, 1));
    }
  }

  static void logo_lines(const char *extra) {
    int indent = (LCD_WIDTH - 8 - lcd_strlen_P(extra)) / 2;
    lcd.setCursor(indent, 0); lcd.print('\x00'); lcd_printPGM(PSTR( "------" ));  lcd.print('\x01');
    lcd.setCursor(indent, 1);                    lcd_printPGM(PSTR("|Marlin|"));  lcd_printPGM(extra);
    lcd.setCursor(indent, 2); lcd.print('\x02'); lcd_printPGM(PSTR( "------" ));  lcd.print('\x03');
  }

  void bootscreen() {
    byte top_left[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00001,
      B00010,
      B00100,
      B00100
    };
    byte top_right[8] = {
      B00000,
      B00000,
      B00000,
      B11100,
      B11100,
      B01100,
      B00100,
      B00100
    };
    byte botom_left[8] = {
      B00100,
      B00010,
      B00001,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    };
    byte botom_right[8] = {
      B00100,
      B01000,
      B10000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    };
    lcd.createChar(0, top_left);
    lcd.createChar(1, top_right);
    lcd.createChar(2, botom_left);
    lcd.createChar(3, botom_right);

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

    lcd_set_custom_characters(
      #if ENABLED(LCD_PROGRESS_BAR)
        false
      #endif
    );
  }

#endif // SHOW_BOOTSCREEN

void lcd_kill_screen() {
  lcd.setCursor(0, 0);
  lcd_print(lcd_status_message);
  #if LCD_HEIGHT < 4
    lcd.setCursor(0, 2);
  #else
    lcd.setCursor(0, 2);
    lcd_printPGM(PSTR(MSG_HALTED));
    lcd.setCursor(0, 3);
  #endif
  lcd_printPGM(PSTR(MSG_PLEASE_RESET));
}

FORCE_INLINE void _draw_axis_label(AxisEnum axis, const char *pstr, bool blink) {
  if (blink)
    lcd_printPGM(pstr);
  else {
    if (!axis_homed[axis])
      lcd.print('?');
    else {
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!axis_known_position[axis])
          lcd.print(' ');
        else
      #endif
      lcd_printPGM(pstr);
    }
  }
}

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

  #define LCD_TEMP_ONLY(T1,T2) \
    lcd.print(itostr3(T1 + 0.5)); \
    lcd.print('/'); \
    lcd.print(itostr3left(T2 + 0.5))

  #define LCD_TEMP(T1,T2,PREFIX) \
    lcd.print(PREFIX); \
    LCD_TEMP_ONLY(T1,T2); \
    lcd_printPGM(PSTR(LCD_STR_DEGREE " ")); \
    if (T2 < 10) lcd.print(' ')

  //
  // Line 1
  //

  lcd.setCursor(0, 0);

  #if LCD_WIDTH < 20

    //
    // Hotend 0 Temperature
    //
    LCD_TEMP_ONLY(thermalManager.degHotend(0), thermalManager.degTargetHotend(0));

    //
    // Hotend 1 or Bed Temperature
    //
    #if HOTENDS > 1 || TEMP_SENSOR_BED != 0

      lcd.setCursor(8, 0);
      #if HOTENDS > 1
        lcd.print(LCD_STR_THERMOMETER[0]);
        LCD_TEMP_ONLY(thermalManager.degHotend(1), thermalManager.degTargetHotend(1));
      #else
        lcd.print(LCD_STR_BEDTEMP[0]);
        LCD_TEMP_ONLY(thermalManager.degBed(), thermalManager.degTargetBed());
      #endif

    #endif // HOTENDS > 1 || TEMP_SENSOR_BED != 0

  #else // LCD_WIDTH >= 20

    //
    // Hotend 0 Temperature
    //
    LCD_TEMP(thermalManager.degHotend(0), thermalManager.degTargetHotend(0), LCD_STR_THERMOMETER[0]);

    //
    // Hotend 1 or Bed Temperature
    //
    #if HOTENDS > 1 || TEMP_SENSOR_BED != 0
      lcd.setCursor(10, 0);
      #if HOTENDS > 1
        LCD_TEMP(thermalManager.degHotend(1), thermalManager.degTargetHotend(1), LCD_STR_THERMOMETER[0]);
      #else
        LCD_TEMP(thermalManager.degBed(), thermalManager.degTargetBed(), LCD_STR_BEDTEMP[0]);
      #endif

    #endif  // HOTENDS > 1 || TEMP_SENSOR_BED != 0

  #endif // LCD_WIDTH >= 20

  //
  // Line 2
  //

  #if LCD_HEIGHT > 2

    bool blink = lcd_blink();

    #if LCD_WIDTH < 20

      #if ENABLED(SDSUPPORT)
        lcd.setCursor(0, 2);
        lcd_printPGM(PSTR("SD"));
        if (IS_SD_PRINTING)
          lcd.print(itostr3(card.percentDone()));
        else
          lcd_printPGM(PSTR("---"));
          lcd.print('%');
      #endif // SDSUPPORT

    #else // LCD_WIDTH >= 20

      lcd.setCursor(0, 1);

      #if HOTENDS > 1 && TEMP_SENSOR_BED != 0

        // If we both have a 2nd extruder and a heated bed,
        // show the heated bed temp on the left,
        // since the first line is filled with extruder temps
        LCD_TEMP(thermalManager.degBed(), thermalManager.degTargetBed(), LCD_STR_BEDTEMP[0]);

      #else
        // Before homing the axis letters are blinking 'X' <-> '?'.
        // When axis is homed but axis_known_position is false the axis letters are blinking 'X' <-> ' '.
        // When everything is ok you see a constant 'X'.

        _draw_axis_label(X_AXIS, PSTR(MSG_X), blink);
        lcd.print(ftostr4sign(current_position[X_AXIS]));

        lcd.print(' ');

        _draw_axis_label(Y_AXIS, PSTR(MSG_Y), blink);
        lcd.print(ftostr4sign(current_position[Y_AXIS]));

      #endif // HOTENDS > 1 || TEMP_SENSOR_BED != 0

    #endif // LCD_WIDTH >= 20

    lcd.setCursor(LCD_WIDTH - 8, 1);
    _draw_axis_label(Z_AXIS, PSTR(MSG_Z), blink);
    lcd.print(ftostr52sp(current_position[Z_AXIS] + 0.00001));

  #endif // LCD_HEIGHT > 2

  //
  // Line 3
  //

  #if LCD_HEIGHT > 3

    lcd.setCursor(0, 2);
    lcd.print(LCD_STR_FEEDRATE[0]);
    lcd.print(itostr3(feedrate_percentage));
    lcd.print('%');

    #if LCD_WIDTH > 19 && ENABLED(SDSUPPORT)

      lcd.setCursor(7, 2);
      lcd_printPGM(PSTR("SD"));
      if (IS_SD_PRINTING)
        lcd.print(itostr3(card.percentDone()));
      else
        lcd_printPGM(PSTR("---"));
      lcd.print('%');

    #endif // LCD_WIDTH > 19 && SDSUPPORT

    lcd.setCursor(LCD_WIDTH - 6, 2);
    lcd.print(LCD_STR_CLOCK[0]);

    char buffer[10];
    duration_t elapsed = print_job_timer.duration();
    elapsed.toDigital(buffer);
    lcd_print(buffer);

  #endif // LCD_HEIGHT > 3

  //
  // Last Line
  // Status Message (which may be a Progress Bar or Filament display)
  //

  lcd.setCursor(0, LCD_HEIGHT - 1);

  #if ENABLED(LCD_PROGRESS_BAR)

    if (card.isFileOpen()) {
      // Draw the progress bar if the message has shown long enough
      // or if there is no message set.
      if (ELAPSED(millis(), progress_bar_ms + PROGRESS_BAR_MSG_TIME) || !lcd_status_message[0]) {
        int tix = (int)(card.percentDone() * (LCD_WIDTH) * 3) / 100,
          cel = tix / 3, rem = tix % 3, i = LCD_WIDTH;
        char msg[LCD_WIDTH + 1], b = ' ';
        msg[i] = '\0';
        while (i--) {
          if (i == cel - 1)
            b = LCD_STR_PROGRESS[2];
          else if (i == cel && rem != 0)
            b = LCD_STR_PROGRESS[rem - 1];
          msg[i] = b;
        }
        lcd.print(msg);
        return;
      }
    } //card.isFileOpen

  #elif ENABLED(FILAMENT_LCD_DISPLAY)

    // Show Filament Diameter and Volumetric Multiplier %
    // After allowing lcd_status_message to show for 5 seconds
    if (ELAPSED(millis(), previous_lcd_status_ms + 5000UL)) {
      lcd_printPGM(PSTR("Dia "));
      lcd.print(ftostr12ns(filament_width_meas));
      lcd_printPGM(PSTR(" V"));
      lcd.print(itostr3(100.0 * volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]));
      lcd.print('%');
      return;
    }

  #endif // FILAMENT_LCD_DISPLAY

  lcd_print(lcd_status_message);
}

#if ENABLED(ULTIPANEL)

  #if ENABLED(LCD_INFO_MENU) || ENABLED(FILAMENT_CHANGE_FEATURE)

    static void lcd_implementation_drawmenu_static(uint8_t row, const char* pstr, bool center=true, bool invert=false, const char *valstr=NULL) {
      UNUSED(invert);
      char c;
      int8_t n = LCD_WIDTH;
      lcd.setCursor(0, row);
      if (center && !valstr) {
        int8_t pad = (LCD_WIDTH - lcd_strlen_P(pstr)) / 2;
        while (--pad >= 0) { lcd.print(' '); n--; }
      }
      while (n > 0 && (c = pgm_read_byte(pstr))) {
        n -= charset_mapper(c);
        pstr++;
      }
      if (valstr) while (n > 0 && (c = *valstr)) {
        n -= charset_mapper(c);
        valstr++;
      }
      while (n-- > 0) lcd.print(' ');
    }

  #endif // LCD_INFO_MENU || FILAMENT_CHANGE_FEATURE

  static void lcd_implementation_drawmenu_generic(bool sel, uint8_t row, const char* pstr, char pre_char, char post_char) {
    char c;
    uint8_t n = LCD_WIDTH - 2;
    lcd.setCursor(0, row);
    lcd.print(sel ? pre_char : ' ');
    while ((c = pgm_read_byte(pstr)) && n > 0) {
      n -= charset_mapper(c);
      pstr++;
    }
    while (n--) lcd.print(' ');
    lcd.print(post_char);
  }

  static void lcd_implementation_drawmenu_setting_edit_generic(bool sel, uint8_t row, const char* pstr, char pre_char, char* data) {
    char c;
    uint8_t n = LCD_WIDTH - 2 - lcd_strlen(data);
    lcd.setCursor(0, row);
    lcd.print(sel ? pre_char : ' ');
    while ((c = pgm_read_byte(pstr)) && n > 0) {
      n -= charset_mapper(c);
      pstr++;
    }
    lcd.print(':');
    while (n--) lcd.print(' ');
    lcd_print(data);
  }
  static void lcd_implementation_drawmenu_setting_edit_generic_P(bool sel, uint8_t row, const char* pstr, char pre_char, const char* data) {
    char c;
    uint8_t n = LCD_WIDTH - 2 - lcd_strlen_P(data);
    lcd.setCursor(0, row);
    lcd.print(sel ? pre_char : ' ');
    while ((c = pgm_read_byte(pstr)) && n > 0) {
      n -= charset_mapper(c);
      pstr++;
    }
    lcd.print(':');
    while (n--) lcd.print(' ');
    lcd_printPGM(data);
  }

  #define lcd_implementation_drawmenu_setting_edit_int3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', itostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float32(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr32(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float43(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr43sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float52(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr52sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_float51(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr51sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_long5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  //Add version for callback functions
  #define lcd_implementation_drawmenu_setting_edit_callback_int3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', itostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr3(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float32(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr32(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float43(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr43sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float52(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr52sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_float51(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr51sign(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_long5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5rj(*(data)))
  #define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  void lcd_implementation_drawedit(const char* pstr, const char* value=NULL) {
    lcd.setCursor(1, 1);
    lcd_printPGM(pstr);
    if (value != NULL) {
      lcd.print(':');
      lcd.setCursor(LCD_WIDTH - lcd_strlen(value), 1);
      lcd_print(value);
    }
  }

  #if ENABLED(SDSUPPORT)

    static void lcd_implementation_drawmenu_sd(bool sel, uint8_t row, const char* pstr, const char* filename, char* longFilename, uint8_t concat, char post_char) {
      UNUSED(pstr);
      char c;
      uint8_t n = LCD_WIDTH - concat;
      lcd.setCursor(0, row);
      lcd.print(sel ? '>' : ' ');
      if (longFilename[0]) {
        filename = longFilename;
        longFilename[n] = '\0';
      }
      while ((c = *filename) && n > 0) {
        n -= charset_mapper(c);
        filename++;
      }
      while (n--) lcd.print(' ');
      lcd.print(post_char);
    }

    static void lcd_implementation_drawmenu_sdfile(bool sel, uint8_t row, const char* pstr, const char* filename, char* longFilename) {
      lcd_implementation_drawmenu_sd(sel, row, pstr, filename, longFilename, 2, ' ');
    }

    static void lcd_implementation_drawmenu_sddirectory(bool sel, uint8_t row, const char* pstr, const char* filename, char* longFilename) {
      lcd_implementation_drawmenu_sd(sel, row, pstr, filename, longFilename, 2, LCD_STR_FOLDER[0]);
    }

  #endif // SDSUPPORT

  #define lcd_implementation_drawmenu_back(sel, row, pstr) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
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

#endif // ULTIPANEL

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

#endif // ULTRALCD_IMPL_HD44780_H
