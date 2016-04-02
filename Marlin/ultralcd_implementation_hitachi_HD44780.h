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

#ifndef ULTRALCD_IMPLEMENTATION_HITACHI_HD44780_H
#define ULTRALCD_IMPLEMENTATION_HITACHI_HD44780_H

/**
* Implementation of the LCD display routines for a Hitachi HD44780 display. These are common LCD character displays.
**/

extern volatile uint8_t buttons;  //an extended version of the last checked buttons in a bit array.

////////////////////////////////////
// Setup button and encode mappings for each panel (into 'buttons' variable
//
// This is just to map common functions (across different panels) onto the same
// macro name. The mapping is independent of whether the button is directly connected or
// via a shift/i2c register.

#if ENABLED(ULTIPANEL)
  // All UltiPanels might have an encoder - so this is always be mapped onto first two bits
  #define BLEN_B 1
  #define BLEN_A 0

  #define EN_B (_BV(BLEN_B)) // The two encoder pins are connected through BTN_EN1 and BTN_EN2
  #define EN_A (_BV(BLEN_A))

  #if BUTTON_EXISTS(ENC)
    // encoder click is directly connected
    #define BLEN_C 2
    #define EN_C (_BV(BLEN_C))
  #endif

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

    #if BUTTON_EXISTS(ENC)

      #undef LCD_CLICKED
      #define LCD_CLICKED (buttons&EN_C)

    #else // Read through I2C if not directly connected to a pin

      #define B_I2C_BTN_OFFSET 3 // (the first three bit positions reserved for EN_A, EN_B, EN_C)

      #define B_MI (PANELOLU2_ENCODER_C<<B_I2C_BTN_OFFSET) // requires LiquidTWI2 library v1.2.3 or later

      #undef LCD_CLICKED
      #define LCD_CLICKED (buttons&B_MI)

      // I2C buttons take too long to read inside an interrupt context and so we read them during lcd_update
      #define LCD_HAS_SLOW_BUTTONS

    #endif

  #elif ENABLED(REPRAPWORLD_KEYPAD)
    // define register bit values, don't change it
    #define BLEN_REPRAPWORLD_KEYPAD_F3 0
    #define BLEN_REPRAPWORLD_KEYPAD_F2 1
    #define BLEN_REPRAPWORLD_KEYPAD_F1 2
    #define BLEN_REPRAPWORLD_KEYPAD_UP 6
    #define BLEN_REPRAPWORLD_KEYPAD_RIGHT 4
    #define BLEN_REPRAPWORLD_KEYPAD_MIDDLE 5
    #define BLEN_REPRAPWORLD_KEYPAD_DOWN 3
    #define BLEN_REPRAPWORLD_KEYPAD_LEFT 7

    #define REPRAPWORLD_BTN_OFFSET 0 // bit offset into buttons for shift register values

    #define EN_REPRAPWORLD_KEYPAD_F3 (_BV(BLEN_REPRAPWORLD_KEYPAD_F3+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_F2 (_BV(BLEN_REPRAPWORLD_KEYPAD_F2+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_F1 (_BV(BLEN_REPRAPWORLD_KEYPAD_F1+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_UP (_BV(BLEN_REPRAPWORLD_KEYPAD_UP+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_RIGHT (_BV(BLEN_REPRAPWORLD_KEYPAD_RIGHT+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_MIDDLE (_BV(BLEN_REPRAPWORLD_KEYPAD_MIDDLE+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_DOWN (_BV(BLEN_REPRAPWORLD_KEYPAD_DOWN+REPRAPWORLD_BTN_OFFSET))
    #define EN_REPRAPWORLD_KEYPAD_LEFT (_BV(BLEN_REPRAPWORLD_KEYPAD_LEFT+REPRAPWORLD_BTN_OFFSET))

    //#define LCD_CLICKED ((buttons&EN_C) || (buttons&EN_REPRAPWORLD_KEYPAD_F1))
    //#define REPRAPWORLD_KEYPAD_MOVE_Y_DOWN (buttons&EN_REPRAPWORLD_KEYPAD_DOWN)
    //#define REPRAPWORLD_KEYPAD_MOVE_Y_UP (buttons&EN_REPRAPWORLD_KEYPAD_UP)
    //#define REPRAPWORLD_KEYPAD_MOVE_HOME (buttons&EN_REPRAPWORLD_KEYPAD_MIDDLE)

  #elif ENABLED(NEWPANEL)
    #define LCD_CLICKED (buttons&EN_C)

  #else // old style ULTIPANEL
    //bits in the shift register that carry the buttons for:
    // left up center down right red(stop)
    #define BL_LE 7
    #define BL_UP 6
    #define BL_MI 5
    #define BL_DW 4
    #define BL_RI 3
    #define BL_ST 2

    //automatic, do not change
    #define B_LE (_BV(BL_LE))
    #define B_UP (_BV(BL_UP))
    #define B_MI (_BV(BL_MI))
    #define B_DW (_BV(BL_DW))
    #define B_RI (_BV(BL_RI))
    #define B_ST (_BV(BL_ST))

    #define LCD_CLICKED (buttons&(B_MI|B_ST))
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

#if ENABLED(SHOW_BOOTSCREEN)
  static void bootscreen();
  static bool show_bootscreen = true;
#endif

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
    bool progress_bar_set = true
  #endif
) {
  byte bedTemp[8] = {
    B00000,
    B11111,
    B10101,
    B10001,
    B10101,
    B11111,
    B00000,
    B00000
  }; //thanks Sonny Mounicou
  byte degree[8] = {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
  };
  byte thermometer[8] = {
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B10001,
    B01110
  };
  byte uplevel[8] = {
    B00100,
    B01110,
    B11111,
    B00100,
    B11100,
    B00000,
    B00000,
    B00000
  }; //thanks joris
  byte refresh[8] = {
    B00000,
    B00110,
    B11001,
    B11000,
    B00011,
    B10011,
    B01100,
    B00000,
  }; //thanks joris
  byte folder[8] = {
    B00000,
    B11100,
    B11111,
    B10001,
    B10001,
    B11111,
    B00000,
    B00000
  }; //thanks joris
  byte feedrate[8] = {
    B11100,
    B10000,
    B11000,
    B10111,
    B00101,
    B00110,
    B00101,
    B00000
  }; //thanks Sonny Mounicou
  byte clock[8] = {
    B00000,
    B01110,
    B10011,
    B10101,
    B10001,
    B01110,
    B00000,
    B00000
  }; //thanks Sonny Mounicou

  #if ENABLED(LCD_PROGRESS_BAR)
    static bool char_mode = false;
    byte progress[3][8] = { {
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
    if (progress_bar_set != char_mode) {
      char_mode = progress_bar_set;
      lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
      lcd.createChar(LCD_STR_DEGREE[0], degree);
      lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
      lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
      lcd.createChar(LCD_STR_CLOCK[0], clock);
      if (progress_bar_set) {
        // Progress bar characters for info screen
        for (int i = 3; i--;) lcd.createChar(LCD_STR_PROGRESS[i], progress[i]);
      }
      else {
        // Custom characters for submenus
        lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
        lcd.createChar(LCD_STR_REFRESH[0], refresh);
        lcd.createChar(LCD_STR_FOLDER[0], folder);
      }
    }
  #else
    lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
    lcd.createChar(LCD_STR_DEGREE[0], degree);
    lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
    lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
    lcd.createChar(LCD_STR_REFRESH[0], refresh);
    lcd.createChar(LCD_STR_FOLDER[0], folder);
    lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
    lcd.createChar(LCD_STR_CLOCK[0], clock);
  #endif
}

static void lcd_implementation_init(
  #if ENABLED(LCD_PROGRESS_BAR)
    bool progress_bar_set = true
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

  #if ENABLED(SHOW_BOOTSCREEN)
    if (show_bootscreen) bootscreen();
  #endif

  lcd_set_custom_characters(
    #if ENABLED(LCD_PROGRESS_BAR)
      progress_bar_set
    #endif
  );

  lcd.clear();
}

static void lcd_implementation_clear() { lcd.clear(); }

/* Arduino < 1.0.0 is missing a function to print PROGMEM strings, so we need to implement our own */
char lcd_printPGM(const char* str) {
  char c, n = 0;
  while ((c = pgm_read_byte(str++))) n += charset_mapper(c);
  return n;
}

char lcd_print(const char* str) {
  char c, n = 0;
  unsigned char i = 0;
  while ((c = str[i++])) n += charset_mapper(c);
  return n;
}

unsigned lcd_print(char c) { return charset_mapper(c); }

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

  static void bootscreen() {
    show_bootscreen = false;
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

    #define TEXT_SCREEN_LOGO_SHIFT ((LCD_WIDTH/2) - 4)
    lcd.setCursor(TEXT_SCREEN_LOGO_SHIFT, 0); lcd.print('\x00'); lcd_printPGM(PSTR( "------" ));  lcd.print('\x01');
    lcd.setCursor(TEXT_SCREEN_LOGO_SHIFT, 1);                    lcd_printPGM(PSTR("|Marlin|"));
    lcd.setCursor(TEXT_SCREEN_LOGO_SHIFT, 2); lcd.print('\x02'); lcd_printPGM(PSTR( "------" ));  lcd.print('\x03');

    delay(2000);

    #ifdef STRING_SPLASH_LINE1
      lcd_erase_line(3);
      lcd_scroll(0, 3, PSTR(STRING_SPLASH_LINE1), LCD_WIDTH, 1000);
    #endif

    #ifdef STRING_SPLASH_LINE2
      lcd_erase_line(3);
      lcd_scroll(0, 3, PSTR(STRING_SPLASH_LINE2), LCD_WIDTH, 1000);
    #endif
  }

#endif // SHOW_BOOTSCREEN

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
    LCD_TEMP_ONLY(degHotend(0), degTargetHotend(0));

    //
    // Hotend 1 or Bed Temperature
    //
    #if EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

      lcd.setCursor(8, 0);
      #if EXTRUDERS > 1
        lcd.print(LCD_STR_THERMOMETER[0]);
        LCD_TEMP_ONLY(degHotend(1), degTargetHotend(1));
      #else
        lcd.print(LCD_STR_BEDTEMP[0]);
        LCD_TEMP_ONLY(degBed(), degTargetBed());
      #endif

    #endif // EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

  #else // LCD_WIDTH >= 20

    //
    // Hotend 0 Temperature
    //
    LCD_TEMP(degHotend(0), degTargetHotend(0), LCD_STR_THERMOMETER[0]);

    //
    // Hotend 1 or Bed Temperature
    //
    #if EXTRUDERS > 1 || TEMP_SENSOR_BED != 0
      lcd.setCursor(10, 0);
      #if EXTRUDERS > 1
        LCD_TEMP(degHotend(1), degTargetHotend(1), LCD_STR_THERMOMETER[0]);
      #else
        LCD_TEMP(degBed(), degTargetBed(), LCD_STR_BEDTEMP[0]);
      #endif

    #endif  // EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

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

      #if EXTRUDERS > 1 && TEMP_SENSOR_BED != 0

        // If we both have a 2nd extruder and a heated bed,
        // show the heated bed temp on the left,
        // since the first line is filled with extruder temps
        LCD_TEMP(degBed(), degTargetBed(), LCD_STR_BEDTEMP[0]);

      #else
        // Before homing the axis letters are blinking 'X' <-> '?'.
        // When axis is homed but axis_known_position is false the axis letters are blinking 'X' <-> ' '.
        // When everything is ok you see a constant 'X'.

        if (blink)
          lcd_printPGM(PSTR("X"));
        else {
          if (!axis_homed[X_AXIS])
            lcd_printPGM(PSTR("?"));
          else
            #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
              if (!axis_known_position[X_AXIS])
                lcd_printPGM(PSTR(" "));
              else
            #endif
            lcd_printPGM(PSTR("X"));
        }

        lcd.print(ftostr4sign(current_position[X_AXIS]));

        lcd_printPGM(PSTR(" "));
        if (blink)
          lcd_printPGM(PSTR("Y"));
        else {
          if (!axis_homed[Y_AXIS])
            lcd_printPGM(PSTR("?"));
          else
            #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
              if (!axis_known_position[Y_AXIS])
                lcd_printPGM(PSTR(" "));
              else
            #endif
            lcd_printPGM(PSTR("Y"));
        }
        lcd.print(ftostr4sign(current_position[Y_AXIS]));

      #endif // EXTRUDERS > 1 || TEMP_SENSOR_BED != 0

    #endif // LCD_WIDTH >= 20

    lcd.setCursor(LCD_WIDTH - 8, 1);
    if (blink)
      lcd_printPGM(PSTR("Z"));
    else {
      if (!axis_homed[Z_AXIS])
        lcd_printPGM(PSTR("?"));
      else
        #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
          if (!axis_known_position[Z_AXIS])
            lcd_printPGM(PSTR(" "));
          else
        #endif
        lcd_printPGM(PSTR("Z"));
    }
    lcd.print(ftostr32sp(current_position[Z_AXIS] + 0.00001));

  #endif // LCD_HEIGHT > 2

  //
  // Line 3
  //

  #if LCD_HEIGHT > 3

    lcd.setCursor(0, 2);
    lcd.print(LCD_STR_FEEDRATE[0]);
    lcd.print(itostr3(feedrate_multiplier));
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
    if (print_job_start_ms != 0) {
      uint16_t time = (((print_job_stop_ms > print_job_start_ms)
                       ? print_job_stop_ms : millis()) - print_job_start_ms) / 60000;
      lcd.print(itostr2(time / 60));
      lcd.print(':');
      lcd.print(itostr2(time % 60));
    }
    else {
      lcd_printPGM(PSTR("--:--"));
    }

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
      if (millis() >= progress_bar_ms + PROGRESS_BAR_MSG_TIME || !lcd_status_message[0]) {
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
    if (millis() >= previous_lcd_status_ms + 5000) {
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

static void lcd_implementation_drawmenu_generic(bool sel, uint8_t row, const char* pstr, char pre_char, char post_char) {
  char c;
  uint8_t n = LCD_WIDTH - 2;
  lcd.setCursor(0, row);
  lcd.print(sel ? pre_char : ' ');
  while ((c = pgm_read_byte(pstr)) && n > 0) {
    n -= lcd_print(c);
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
    n -= lcd_print(c);
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
    n -= lcd_print(c);
    pstr++;
  }
  lcd.print(':');
  while (n--) lcd.print(' ');
  lcd_printPGM(data);
}

#define lcd_implementation_drawmenu_setting_edit_int3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float3(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float32(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float43(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float52(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_float51(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_long5(sel, row, pstr, pstr2, data, minValue, maxValue) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

//Add version for callback functions
#define lcd_implementation_drawmenu_setting_edit_callback_int3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', itostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float3(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr3(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float32(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr32(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float43(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr43(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float52(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr52(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_float51(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr51(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_long5(sel, row, pstr, pstr2, data, minValue, maxValue, callback) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', ftostr5(*(data)))
#define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

void lcd_implementation_drawedit(const char* pstr, const char* value) {
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
    char c;
    uint8_t n = LCD_WIDTH - concat;
    lcd.setCursor(0, row);
    lcd.print(sel ? '>' : ' ');
    if (longFilename[0]) {
      filename = longFilename;
      longFilename[n] = '\0';
    }
    while ((c = *filename) && n > 0) {
      n -= lcd_print(c);
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

#endif //SDSUPPORT

#define lcd_implementation_drawmenu_back(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL[0], LCD_STR_UPLEVEL[0])
#define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
#define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
#define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

#if ENABLED(LCD_HAS_STATUS_INDICATORS)

  static void lcd_implementation_update_indicators() {
    // Set the LEDS - referred to as backlights by the LiquidTWI2 library
    static uint8_t ledsprev = 0;
    uint8_t leds = 0;

    if (target_temperature_bed > 0) leds |= LED_A;

    if (target_temperature[0] > 0) leds |= LED_B;

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

    #if EXTRUDERS > 1
      if (target_temperature[1] > 0) leds |= LED_C;
    #endif

    if (leds != ledsprev) {
      lcd.setBacklight(leds);
      ledsprev = leds;
    }

  }

#endif // LCD_HAS_STATUS_INDICATORS

#if ENABLED(LCD_HAS_SLOW_BUTTONS)

  extern millis_t next_button_update_ms;

  static uint8_t lcd_implementation_read_slow_buttons() {
    #if ENABLED(LCD_I2C_TYPE_MCP23017)
      // Reading these buttons this is likely to be too slow to call inside interrupt context
      // so they are called during normal lcd_update
      uint8_t slow_bits = lcd.readButtons() << B_I2C_BTN_OFFSET;
      #if ENABLED(LCD_I2C_VIKI)
        if ((slow_bits & (B_MI | B_RI)) && millis() < next_button_update_ms) // LCD clicked
          slow_bits &= ~(B_MI | B_RI); // Disable LCD clicked buttons if screen is updated
      #endif // LCD_I2C_VIKI
      return slow_bits;
    #endif // LCD_I2C_TYPE_MCP23017
  }

#endif // LCD_HAS_SLOW_BUTTONS

#endif // ULTRALCD_IMPLEMENTATION_HITACHI_HD44780_H
