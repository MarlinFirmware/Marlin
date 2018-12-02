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

#include "../../inc/MarlinConfigPre.h"

#if HAS_CHARACTER_LCD

/**
 * ultralcd_HD44780.cpp
 *
 * LCD display implementations for Hitachi HD44780.
 * These are the most common LCD character displays.
 */

#include "ultralcd_HD44780.h"
#include "../ultralcd.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../../module/motion.h"

#if DISABLED(LCD_PROGRESS_BAR) && ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
  #include "../../feature/filwidth.h"
  #include "../../gcode/parser.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/ubl/ubl.h"
#endif

//
// Create LCD instance and chipset-specific information
//

#if ENABLED(LCD_I2C_TYPE_PCF8575)

  LCD_CLASS lcd(LCD_I2C_ADDRESS, LCD_I2C_PIN_EN, LCD_I2C_PIN_RW, LCD_I2C_PIN_RS, LCD_I2C_PIN_D4, LCD_I2C_PIN_D5, LCD_I2C_PIN_D6, LCD_I2C_PIN_D7);

#elif ENABLED(LCD_I2C_TYPE_MCP23017) || ENABLED(LCD_I2C_TYPE_MCP23008)

  LCD_CLASS lcd(LCD_I2C_ADDRESS
    #ifdef DETECT_DEVICE
      , 1
    #endif
  );

#elif ENABLED(LCD_I2C_TYPE_PCA8574)

  LCD_CLASS lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);

#elif ENABLED(SR_LCD_2W_NL)

  // 2 wire Non-latching LCD SR from:
  // https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!shiftregister-connection

  LCD_CLASS lcd(SR_DATA_PIN, SR_CLK_PIN
    #if PIN_EXISTS(SR_STROBE)
      , SR_STROBE_PIN
    #endif
  );

#elif ENABLED(LCM1602)

  LCD_CLASS lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#else

  // Standard direct-connected LCD implementations
  LCD_CLASS lcd(LCD_PINS_RS, LCD_PINS_ENABLE, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7);

#endif

static void createChar_P(const char c, const byte * const ptr) {
  byte temp[8];
  for (uint8_t i = 0; i < 8; i++)
    temp[i] = pgm_read_byte(&ptr[i]);
  lcd.createChar(c, temp);
}

#if ENABLED(LCD_PROGRESS_BAR)
  #define LCD_STR_PROGRESS  "\x03\x04\x05"
#endif

void MarlinUI::set_custom_characters(const HD44780CharSet screen_charset/*=CHARSET_INFO*/) {
  #if DISABLED(LCD_PROGRESS_BAR) && DISABLED(SHOW_BOOTSCREEN)
    UNUSED(screen_charset);
  #endif

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
    #if LCD_INFO_SCREEN_STYLE == 1
      B00000,
      B00100,
      B10010,
      B01001,
      B10010,
      B00100,
      B00000,
      B00000
    #else
      B11100,
      B10000,
      B11000,
      B10111,
      B00101,
      B00110,
      B00101,
      B00000
    #endif
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

  #endif // SDSUPPORT

  #if ENABLED(SHOW_BOOTSCREEN)
    // Set boot screen corner characters
    if (screen_charset == CHARSET_BOOT) {
      for (uint8_t i = 4; i--;)
        createChar_P(i, corner[i]);
    }
    else
  #endif
    { // Info Screen uses 5 special characters
      createChar_P(LCD_STR_BEDTEMP[0], bedTemp);
      createChar_P(LCD_STR_DEGREE[0], degree);
      createChar_P(LCD_STR_THERMOMETER[0], thermometer);
      createChar_P(LCD_STR_FEEDRATE[0], feedrate);
      createChar_P(LCD_STR_CLOCK[0], clock);

      #if ENABLED(LCD_PROGRESS_BAR)
        if (screen_charset == CHARSET_INFO) { // 3 Progress bar characters for info screen
          for (int16_t i = 3; i--;)
            createChar_P(LCD_STR_PROGRESS[i], progress[i]);
        }
        else
      #endif
        {
          createChar_P(LCD_STR_UPLEVEL[0], uplevel);
          #if ENABLED(SDSUPPORT)
            // SD Card sub-menu special characters
            createChar_P(LCD_STR_REFRESH[0], refresh);
            createChar_P(LCD_STR_FOLDER[0], folder);
          #endif
        }
    }

}

void MarlinUI::init_lcd() {

  #if ENABLED(LCD_I2C_TYPE_PCF8575)
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    #ifdef LCD_I2C_PIN_BL
      lcd.setBacklightPin(LCD_I2C_PIN_BL, POSITIVE);
      lcd.setBacklight(HIGH);
    #endif

  #elif ENABLED(LCD_I2C_TYPE_MCP23017)
    lcd.setMCPType(LTI_TYPE_MCP23017);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    update_indicators();

  #elif ENABLED(LCD_I2C_TYPE_MCP23008)
    lcd.setMCPType(LTI_TYPE_MCP23008);
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);

  #elif ENABLED(LCD_I2C_TYPE_PCA8574)
    lcd.init();
    lcd.backlight();

  #else
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  #endif

  set_custom_characters(on_status_screen() ? CHARSET_INFO : CHARSET_MENU);

  lcd.clear();
}

void MarlinUI::clear_lcd() { lcd.clear(); }

#if ENABLED(SHOW_BOOTSCREEN)

  void lcd_erase_line(const int16_t line) {
    lcd_moveto(0, line);
    for (uint8_t i = LCD_WIDTH + 1; --i;)
      lcd_put_wchar(' ');
  }

  // Scroll the PSTR 'text' in a 'len' wide field for 'time' milliseconds at position col,line
  void lcd_scroll(const uint8_t col, const uint8_t line, PGM_P const text, const uint8_t len, const int16_t time) {
    uint8_t slen = utf8_strlen_P(text);
    if (slen < len) {
      // Fits into,
      lcd_moveto(col, line);
      lcd_put_u8str_max_P(text, len);
      for (; slen < len; ++slen) lcd_put_wchar(' ');
      safe_delay(time);
    }
    else {
      PGM_P p = text;
      int dly = time / MAX(slen, 1);
      for (uint8_t i = 0; i <= slen; i++) {

        // Go to the correct place
        lcd_moveto(col, line);

        // Print the text
        lcd_put_u8str_max_P(p, len);

        // Fill with spaces
        for (uint8_t ix = slen - i; ix < len; ++ix) lcd_put_wchar(' ');

        // Delay
        safe_delay(dly);

        // Advance to the next UTF8 valid position
        p++;
        while (!START_OF_UTF8_CHAR(pgm_read_byte(p))) p++;
      }
    }
  }

  static void logo_lines(PGM_P const extra) {
    int16_t indent = (LCD_WIDTH - 8 - utf8_strlen_P(extra)) / 2;
    lcd_moveto(indent, 0); lcd_put_wchar('\x00'); lcd_put_u8str_P(PSTR( "------" ));  lcd_put_wchar('\x01');
    lcd_moveto(indent, 1);                        lcd_put_u8str_P(PSTR("|Marlin|"));  lcd_put_u8str_P(extra);
    lcd_moveto(indent, 2); lcd_put_wchar('\x02'); lcd_put_u8str_P(PSTR( "------" ));  lcd_put_wchar('\x03');
  }

  void MarlinUI::show_bootscreen() {
    set_custom_characters(CHARSET_BOOT);
    lcd.clear();

    #define LCD_EXTRA_SPACE (LCD_WIDTH-8)

    #define CENTER_OR_SCROLL(STRING,DELAY) \
      lcd_erase_line(3); \
      if (utf8_strlen(STRING) <= LCD_WIDTH) { \
        lcd_moveto((LCD_WIDTH - utf8_strlen_P(PSTR(STRING))) / 2, 3); \
        lcd_put_u8str_P(PSTR(STRING)); \
        safe_delay(DELAY); \
      } \
      else { \
        lcd_scroll(0, 3, PSTR(STRING), LCD_WIDTH, DELAY); \
      }

    #ifdef STRING_SPLASH_LINE1
      //
      // Show the Marlin logo with splash line 1
      //
      if (LCD_EXTRA_SPACE >= utf8_strlen(STRING_SPLASH_LINE1) + 1) {
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
          #ifdef STRING_SPLASH_LINE3
            CENTER_OR_SCROLL(STRING_SPLASH_LINE3, 1500);
          #endif
        #endif
      }
    #elif defined(STRING_SPLASH_LINE2)
      //
      // Show splash line 2 only, alongside the logo if possible
      //
      if (LCD_EXTRA_SPACE >= utf8_strlen(STRING_SPLASH_LINE2) + 1) {
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
    set_custom_characters(CHARSET_INFO);
    lcd.clear();
  }

#endif // SHOW_BOOTSCREEN

void MarlinUI::draw_kill_screen() {
  lcd_moveto(0, 0);
  lcd_put_u8str(status_message);
  #if LCD_HEIGHT < 4
    lcd_moveto(0, 2);
  #else
    lcd_moveto(0, 2);
    lcd_put_u8str_P(PSTR(MSG_HALTED));
    lcd_moveto(0, 3);
  #endif
  lcd_put_u8str_P(PSTR(MSG_PLEASE_RESET));
}

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink) {
  lcd_put_wchar('X' + uint8_t(axis));
  if (blink)
    lcd_put_u8str(value);
  else {
    if (!TEST(axis_homed, axis))
      while (const char c = *value++) lcd_put_wchar(c <= '.' ? c : '?');
    else {
      #if DISABLED(HOME_AFTER_DEACTIVATE) && DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        if (!TEST(axis_known_position, axis))
          lcd_put_u8str_P(axis == Z_AXIS ? PSTR("      ") : PSTR("    "));
        else
      #endif
          lcd_put_u8str(value);
    }
  }
}

FORCE_INLINE void _draw_heater_status(const int8_t heater, const char prefix, const bool blink) {
  #if HAS_HEATED_BED
    const bool isBed = heater < 0;
    const float t1 = (isBed ? thermalManager.degBed()       : thermalManager.degHotend(heater)),
                t2 = (isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater));
  #else
    const float t1 = thermalManager.degHotend(heater), t2 = thermalManager.degTargetHotend(heater);
  #endif

  if (prefix >= 0) lcd_put_wchar(prefix);

  lcd_put_u8str(itostr3(t1 + 0.5));
  lcd_put_wchar('/');

  #if !HEATER_IDLE_HANDLER
    UNUSED(blink);
  #else
    const bool is_idle = (
      #if HAS_HEATED_BED
        isBed ? thermalManager.is_bed_idle() :
      #endif
      thermalManager.is_heater_idle(heater)
    );

    if (!blink && is_idle) {
      lcd_put_wchar(' ');
      if (t2 >= 10) lcd_put_wchar(' ');
      if (t2 >= 100) lcd_put_wchar(' ');
    }
    else
  #endif
      lcd_put_u8str(itostr3left(t2 + 0.5));

  if (prefix >= 0) {
    lcd_put_wchar(LCD_STR_DEGREE[0]);
    lcd_put_wchar(' ');
    if (t2 < 10) lcd_put_wchar(' ');
  }
}

FORCE_INLINE void _draw_bed_status(const bool blink) {
  _draw_heater_status(-1, (
    #if HAS_LEVELING
      planner.leveling_active && blink ? '_' :
    #endif
    LCD_STR_BEDTEMP[0]
  ), blink);
}

#if HAS_PRINT_PROGRESS

  FORCE_INLINE void _draw_print_progress() {
    const uint8_t progress = ui.get_progress();
    lcd_put_u8str_P(PSTR(
      #if ENABLED(SDSUPPORT)
        "SD"
      #elif ENABLED(LCD_SET_PROGRESS_MANUALLY)
        "P:"
      #endif
    ));
    if (progress)
      lcd_put_u8str(itostr3(progress));
    else
      lcd_put_u8str_P(PSTR("---"));
    lcd_put_wchar('%');
  }

#endif

#if ENABLED(LCD_PROGRESS_BAR)

  void MarlinUI::draw_progress_bar(const uint8_t percent) {
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
    lcd_put_u8str(msg);
  }

#endif // LCD_PROGRESS_BAR

void MarlinUI::draw_status_message(const bool blink) {

  lcd_moveto(0, LCD_HEIGHT - 1);

  #if ENABLED(LCD_PROGRESS_BAR)

    // Draw the progress bar if the message has shown long enough
    // or if there is no message set.
    if (ELAPSED(millis(), progress_bar_ms + PROGRESS_BAR_MSG_TIME) || !has_status()) {
      const uint8_t progress = get_progress();
      if (progress > 2) return draw_progress_bar(progress);
    }

  #elif ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)

    // Alternate Status message and Filament display
    if (ELAPSED(millis(), next_filament_display)) {
      lcd_put_u8str_P(PSTR("Dia "));
      lcd_put_u8str(ftostr12ns(filament_width_meas));
      lcd_put_u8str_P(PSTR(" V"));
      lcd_put_u8str(itostr3(100.0 * (
          parser.volumetric_enabled
            ? planner.volumetric_area_nominal / planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
            : planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
        )
      ));
      lcd_put_wchar('%');
      return;
    }

  #endif // FILAMENT_LCD_DISPLAY && SDSUPPORT

  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    static bool last_blink = false;

    // Get the UTF8 character count of the string
    uint8_t slen = utf8_strlen(status_message);

    // If the string fits into the LCD, just print it and do not scroll it
    if (slen <= LCD_WIDTH) {

      // The string isn't scrolling and may not fill the screen
      lcd_put_u8str(status_message);

      // Fill the rest with spaces
      while (slen < LCD_WIDTH) {
        lcd_put_wchar(' ');
        ++slen;
      }
    }
    else {
      // String is larger than the available space in screen.

      // Get a pointer to the next valid UTF8 character
      const char *stat = status_message + status_scroll_offset;

      // Get the string remaining length
      const uint8_t rlen = utf8_strlen(stat);

      // If we have enough characters to display
      if (rlen >= LCD_WIDTH) {
        // The remaining string fills the screen - Print it
        lcd_put_u8str_max(stat, LCD_WIDTH);
      }
      else {

        // The remaining string does not completely fill the screen
        lcd_put_u8str_max(stat, LCD_WIDTH);               // The string leaves space
        uint8_t chars = LCD_WIDTH - rlen;                 // Amount of space left in characters

        lcd_put_wchar('.');                               // Always at 1+ spaces left, draw a dot
        if (--chars) {                                    // Draw a second dot if there's space
          lcd_put_wchar('.');
          if (--chars)
            lcd_put_u8str_max(status_message, chars); // Print a second copy of the message
        }
      }
      if (last_blink != blink) {
        last_blink = blink;

        // Adjust by complete UTF8 characters
        if (status_scroll_offset < slen) {
          status_scroll_offset++;
          while (!START_OF_UTF8_CHAR(status_message[status_scroll_offset]))
            status_scroll_offset++;
        }
        else
          status_scroll_offset = 0;
      }
    }
  #else
    UNUSED(blink);

    // Get the UTF8 character count of the string
    uint8_t slen = utf8_strlen(status_message);

    // Just print the string to the LCD
    lcd_put_u8str_max(status_message, LCD_WIDTH);

    // Fill the rest with spaces if there are missing spaces
    while (slen < LCD_WIDTH) {
      lcd_put_wchar(' ');
      ++slen;
    }
  #endif
}

/**
 *  LCD_INFO_SCREEN_STYLE 0 : Classic Status Screen
 *
 *  16x2   |000/000 B000/000|
 *         |0123456789012345|
 *
 *  16x4   |000/000 B000/000|
 *         |SD---%  Z 000.00|
 *         |F---%     T--:--|
 *         |0123456789012345|
 *
 *  20x2   |T000/000° B000/000° |
 *         |01234567890123456789|
 *
 *  20x4   |T000/000° B000/000° |
 *         |X 000 Y 000 Z000.000|
 *         |F---%  SD---% T--:--|
 *         |01234567890123456789|
 *
 *  LCD_INFO_SCREEN_STYLE 1 : Prusa-style Status Screen
 *
 *  |T000/000°  Z 000.00 |
 *  |B000/000°  F---%    |
 *  |SD---%     T--:--   |
 *  |01234567890123456789|
 *
 *  |T000/000°  Z 000.00 |
 *  |T000/000°  F---%    |
 *  |B000/000°  SD---%   |
 *  |01234567890123456789|
 */

void MarlinUI::draw_status_screen() {

  const bool blink = get_blink();
  lcd_moveto(0, 0);

  #if LCD_INFO_SCREEN_STYLE == 0

    // ========== Line 1 ==========

    #if LCD_WIDTH < 20

      //
      // Hotend 0 Temperature
      //
      _draw_heater_status(0, -1, blink);

      //
      // Hotend 1 or Bed Temperature
      //
      #if HOTENDS > 1
        lcd_moveto(8, 0);
        lcd_put_wchar(LCD_STR_THERMOMETER[0]);
        _draw_heater_status(1, -1, blink);
      #elif HAS_HEATED_BED
        lcd_moveto(8, 0);
        lcd_put_wchar(LCD_STR_BEDTEMP[0]);
        _draw_heater_status(-1, -1, blink);
      #endif

    #else // LCD_WIDTH >= 20

      //
      // Hotend 0 Temperature
      //
      _draw_heater_status(0, LCD_STR_THERMOMETER[0], blink);

      //
      // Hotend 1 or Bed Temperature
      //
      #if HOTENDS > 1
        lcd_moveto(10, 0);
        _draw_heater_status(1, LCD_STR_THERMOMETER[0], blink);
      #elif HAS_HEATED_BED
        lcd_moveto(10, 0);
        _draw_bed_status(blink);
      #endif

    #endif // LCD_WIDTH >= 20

    // ========== Line 2 ==========

    #if LCD_HEIGHT > 2

      #if LCD_WIDTH < 20

        #if HAS_PRINT_PROGRESS
          lcd_moveto(0, 2);
          _draw_print_progress();
        #endif

      #else // LCD_WIDTH >= 20

        lcd_moveto(0, 1);

        // If the first line has two extruder temps,
        // show more temperatures on the next line

        #if HOTENDS > 2 || (HOTENDS > 1 && HAS_HEATED_BED)

          #if HOTENDS > 2
            _draw_heater_status(2, LCD_STR_THERMOMETER[0], blink);
            lcd_moveto(10, 1);
          #endif

          _draw_bed_status(blink);

        #else // HOTENDS <= 2 && (HOTENDS <= 1 || !HAS_HEATED_BED)

          _draw_axis_value(X_AXIS, ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])), blink);

          lcd_put_wchar(' ');

          _draw_axis_value(Y_AXIS, ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])), blink);

        #endif // HOTENDS <= 2 && (HOTENDS <= 1 || !HAS_HEATED_BED)

      #endif // LCD_WIDTH >= 20

      lcd_moveto(LCD_WIDTH - 8, 1);
      _draw_axis_value(Z_AXIS, ftostr52sp(LOGICAL_Z_POSITION(current_position[Z_AXIS])), blink);

      #if HAS_LEVELING && !HAS_HEATED_BED
        lcd_put_wchar(planner.leveling_active || blink ? '_' : ' ');
      #endif

    #endif // LCD_HEIGHT > 2

    // ========== Line 3 ==========

    #if LCD_HEIGHT > 3

      lcd_moveto(0, 2);
      lcd_put_wchar(LCD_STR_FEEDRATE[0]);
      lcd_put_u8str(itostr3(feedrate_percentage));
      lcd_put_wchar('%');

      char buffer[14];
      duration_t elapsed = print_job_timer.duration();
      const uint8_t len = elapsed.toDigital(buffer),
                    timepos = LCD_WIDTH - len - 1;
      lcd_moveto(timepos, 2);
      lcd_put_wchar(LCD_STR_CLOCK[0]);
      lcd_put_u8str(buffer);

      #if LCD_WIDTH >= 20
        lcd_moveto(timepos - 7, 2);
        #if HAS_PRINT_PROGRESS
          _draw_print_progress();
        #else
          char c;
          int per;
          #if HAS_FAN0
            if (blink) {
              c = 'F';
              per = ((int(fan_speed[0]) + 1) * 100) / 256;
            }
            else
          #endif
            {
              c = 'E';
              per = planner.flow_percentage[0];
            }
          lcd_put_wchar(c);
          lcd_put_u8str(itostr3(per));
          lcd_put_wchar('%');
        #endif
      #endif

    #endif // LCD_HEIGHT > 3

  #elif LCD_INFO_SCREEN_STYLE == 1

    // ========== Line 1 ==========

    //
    // Hotend 0 Temperature
    //
    _draw_heater_status(0, LCD_STR_THERMOMETER[0], blink);

    //
    // Z Coordinate
    //
    lcd_moveto(LCD_WIDTH - 9, 0);
    _draw_axis_value(Z_AXIS, ftostr52sp(LOGICAL_Z_POSITION(current_position[Z_AXIS])), blink);

    #if HAS_LEVELING && (HOTENDS > 1 || !HAS_HEATED_BED)
      lcd_moveto(LCD_WIDTH - 1, 0);
      lcd_put_wchar(planner.leveling_active || blink ? '_' : ' ');
    #endif

    // ========== Line 2 ==========

    //
    // Hotend 1 or Bed Temperature
    //
    lcd_moveto(0, 1);
    #if HOTENDS > 1
      _draw_heater_status(1, LCD_STR_THERMOMETER[0], blink);
    #elif HAS_HEATED_BED
      _draw_bed_status(blink);
    #endif

    lcd_moveto(LCD_WIDTH - 9, 1);
    lcd_put_wchar(LCD_STR_FEEDRATE[0]);
    lcd_put_u8str(itostr3(feedrate_percentage));
    lcd_put_wchar('%');

    // ========== Line 3 ==========

    //
    // SD Percent, Hotend 2, or Bed
    //
    lcd_moveto(0, 2);
    #if HOTENDS > 2
      _draw_heater_status(2, LCD_STR_THERMOMETER[0], blink);
    #elif HOTENDS > 1 && HAS_HEATED_BED
      _draw_bed_status(blink);
    #elif HAS_PRINT_PROGRESS
      #define DREW_PRINT_PROGRESS
      _draw_print_progress();
    #endif

    //
    // Elapsed Time or SD Percent
    //
    lcd_moveto(LCD_WIDTH - 9, 2);
    #if HAS_PRINT_PROGRESS && !defined(DREW_PRINT_PROGRESS)
      _draw_print_progress();
    #else
      duration_t elapsed = print_job_timer.duration();
      char buffer[14];
      (void)elapsed.toDigital(buffer);
      lcd_put_wchar(LCD_STR_CLOCK[0]);
      lcd_put_u8str(buffer);
    #endif

  #endif // LCD_INFO_SCREEN_STYLE 1

  // ========= Last Line ========

  //
  // Status Message (which may be a Progress Bar or Filament display)
  //
  draw_status_message(blink);
}

#if HAS_LCD_MENU

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      if (row < LCD_HEIGHT) {
        lcd_moveto(LCD_WIDTH - 9, row);
        _draw_heater_status(extruder, LCD_STR_THERMOMETER[0], ui.get_blink());
      }
    }

  #endif // ADVANCED_PAUSE_FEATURE

  void draw_menu_item_static(const uint8_t row, PGM_P pstr, const bool center/*=true*/, const bool invert/*=false*/, const char *valstr/*=NULL*/) {
    UNUSED(invert);
    int8_t n = LCD_WIDTH;
    lcd_moveto(0, row);
    if (center && !valstr) {
      int8_t pad = (LCD_WIDTH - utf8_strlen_P(pstr)) / 2;
      while (--pad >= 0) { lcd_put_wchar(' '); n--; }
    }
    n -= lcd_put_u8str_max_P(pstr, n);
    if (valstr) n -= lcd_put_u8str_max(valstr, n);
    for (; n > 0; --n) lcd_put_wchar(' ');
  }

  void draw_menu_item(const bool sel, const uint8_t row, PGM_P pstr, const char pre_char, const char post_char) {
    uint8_t n = LCD_WIDTH - 2;
    lcd_moveto(0, row);
    lcd_put_wchar(sel ? pre_char : ' ');
    n -= lcd_put_u8str_max_P(pstr, n);
    for (; n; --n) lcd_put_wchar(' ');
    lcd_put_wchar(post_char);
  }

  void _draw_menu_item_edit(const bool sel, const uint8_t row, PGM_P pstr, const char* const data, const bool pgm) {
    uint8_t n = LCD_WIDTH - 2 - (pgm ? utf8_strlen_P(data) : utf8_strlen(data));
    lcd_moveto(0, row);
    lcd_put_wchar(sel ? LCD_STR_ARROW_RIGHT[0] : ' ');
    n -= lcd_put_u8str_max_P(pstr, n);
    lcd_put_wchar(':');
    for (; n; --n) lcd_put_wchar(' ');
    if (pgm) lcd_put_u8str_P(data); else lcd_put_u8str(data);
  }

  void draw_edit_screen(PGM_P const pstr, const char* const value/*=NULL*/) {
    lcd_moveto(1, 1);
    lcd_put_u8str_P(pstr);
    if (value != NULL) {
      lcd_put_wchar(':');
      int len = utf8_strlen(value);
      const uint8_t valrow = (utf8_strlen_P(pstr) + 1 + len + 1) > (LCD_WIDTH - 2) ? 2 : 1;   // Value on the next row if it won't fit
      lcd_moveto((LCD_WIDTH - 1) - (len + 1), valrow);                                        // Right-justified, padded by spaces
      lcd_put_wchar(' ');                                                                     // Overwrite char if value gets shorter
      lcd_put_u8str(value);
    }
  }

  #if ENABLED(SDSUPPORT)

    void draw_sd_menu_item(const bool sel, const uint8_t row, PGM_P const pstr, CardReader &theCard, const bool isDir) {
      UNUSED(pstr);

      lcd_moveto(0, row);
      lcd_put_wchar(sel ? LCD_STR_ARROW_RIGHT[0] : ' ');
      constexpr uint8_t maxlen = LCD_WIDTH - 2;
      uint8_t n = maxlen - lcd_put_u8str_max(ui.scrolled_filename(theCard, maxlen, row, sel), maxlen);
      for (; n; --n) lcd_put_wchar(' ');
      lcd_put_wchar(isDir ? LCD_STR_FOLDER[0] : ' ');
    }

  #endif // SDSUPPORT

  #if ENABLED(LCD_HAS_STATUS_INDICATORS)

    static void MarlinUI::update_indicators() {
      // Set the LEDS - referred to as backlights by the LiquidTWI2 library
      static uint8_t ledsprev = 0;
      uint8_t leds = 0;

      if (thermalManager.degTargetBed() > 0) leds |= LED_A;

      if (thermalManager.degTargetHotend(0) > 0) leds |= LED_B;

      #if FAN_COUNT > 0
        if (0
          #if HAS_FAN0
            || fan_speed[0]
          #endif
          #if HAS_FAN1
            || fan_speed[1]
          #endif
          #if HAS_FAN2
            || fan_speed[2]
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

    #define HD44780_CHAR_WIDTH    5
    #define HD44780_CHAR_HEIGHT   8
    #define MESH_MAP_COLS         7
    #define MESH_MAP_ROWS         4

    #define CHAR_LINE_TOP         0
    #define CHAR_LINE_BOT         1
    #define CHAR_EDGE_L           2
    #define CHAR_EDGE_R           3
    #define CHAR_UL_UL            4
    #define CHAR_LR_UL            5
    #define CHAR_UL_LR            6
    #define CHAR_LR_LR            7

    #define TOP_LEFT         _BV(0)
    #define TOP_RIGHT        _BV(1)
    #define LOWER_LEFT       _BV(2)
    #define LOWER_RIGHT      _BV(3)

    /**
     * Possible map screens:
     *
     * 16x2   |X000.00  Y000.00|
     *        |(00,00)  Z00.000|
     *
     * 20x2   | X:000.00  Y:000.00 |
     *        | (00,00)   Z:00.000 |
     *
     * 16x4   |+-------+(00,00)|
     *        ||       |X000.00|
     *        ||       |Y000.00|
     *        |+-------+Z00.000|
     *
     * 20x4   | +-------+  (00,00) |
     *        | |       |  X:000.00|
     *        | |       |  Y:000.00|
     *        | +-------+  Z:00.000|
     */

    typedef struct {
      uint8_t custom_char_bits[HD44780_CHAR_HEIGHT];
    } custom_char;

    typedef struct {
      uint8_t column, row,
              x_pixel_offset, y_pixel_offset,
              x_pixel_mask;
    } coordinate;

    void add_edges_to_custom_char(custom_char &custom, const coordinate &ul, const coordinate &lr, const coordinate &brc, const uint8_t cell_location);
    FORCE_INLINE static void clear_custom_char(custom_char * const cc) { ZERO(cc->custom_char_bits); }

    coordinate pixel_location(int16_t x, int16_t y) {
      coordinate ret_val;
      int16_t xp, yp, r, c;

      x++; y++; // +1 because lines on the left and top

      c = x / (HD44780_CHAR_WIDTH);
      r = y / (HD44780_CHAR_HEIGHT);

      ret_val.column = c;
      ret_val.row    = r;

      xp = x - c * (HD44780_CHAR_WIDTH);                                    // Get the pixel offsets into the character cell
      xp = HD44780_CHAR_WIDTH - 1 - xp;                                     // Column within relevant character cell (0 on the right)
      yp = y - r * (HD44780_CHAR_HEIGHT);

      ret_val.x_pixel_mask   = _BV(xp);
      ret_val.x_pixel_offset = xp;
      ret_val.y_pixel_offset = yp;
      return ret_val;
    }

    inline coordinate pixel_location(const uint8_t x, const uint8_t y) { return pixel_location((int16_t)x, (int16_t)y); }

    void prep_and_put_map_char(custom_char &chrdata, const coordinate &ul, const coordinate &lr, const coordinate &brc, const uint8_t cl, const char c, const uint8_t x, const uint8_t y) {
      add_edges_to_custom_char(chrdata, ul, lr, brc, cl);
      lcd.createChar(c, (uint8_t*)&chrdata);
      lcd_moveto(x, y);
      lcd_put_wchar(c);
    }

    void MarlinUI::ubl_plot(const uint8_t x, const uint8_t inverted_y) {

      #if LCD_WIDTH >= 20
        #define _LCD_W_POS 12
        #define _PLOT_X 1
        #define _MAP_X 3
        #define _LABEL(C,X,Y) lcd_moveto(X, Y); lcd_put_u8str(C)
        #define _XLABEL(X,Y) _LABEL("X:",X,Y)
        #define _YLABEL(X,Y) _LABEL("Y:",X,Y)
        #define _ZLABEL(X,Y) _LABEL("Z:",X,Y)
      #else
        #define _LCD_W_POS 8
        #define _PLOT_X 0
        #define _MAP_X 1
        #define _LABEL(X,Y,C) lcd_moveto(X, Y); lcd_put_wchar(C)
        #define _XLABEL(X,Y) _LABEL('X',X,Y)
        #define _YLABEL(X,Y) _LABEL('Y',X,Y)
        #define _ZLABEL(X,Y) _LABEL('Z',X,Y)
      #endif

      #if LCD_HEIGHT <= 3   // 16x2 or 20x2 display

        /**
         * Show X and Y positions
         */
        _XLABEL(_PLOT_X, 0);
        lcd_put_u8str(ftostr52(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x]))));

        _YLABEL(_LCD_W_POS, 0);
        lcd_put_u8str(ftostr52(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[inverted_y]))));

        lcd_moveto(_PLOT_X, 0);

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

        clear_lcd();

        x_map_pixels = (HD44780_CHAR_WIDTH) * (MESH_MAP_COLS) - 2;          // Minus 2 because we are drawing a box around the map
        y_map_pixels = (HD44780_CHAR_HEIGHT) * (MESH_MAP_ROWS) - 2;

        pixels_per_x_mesh_pnt = x_map_pixels / (GRID_MAX_POINTS_X);
        pixels_per_y_mesh_pnt = y_map_pixels / (GRID_MAX_POINTS_Y);

        if (pixels_per_x_mesh_pnt >= HD44780_CHAR_WIDTH) {                  // There are only 2 custom characters available, so the X
          pixels_per_x_mesh_pnt = HD44780_CHAR_WIDTH;                       // Size of the mesh point needs to fit within them independent
          suppress_x_offset = 1;                                            // Of where the starting pixel is located.
        }

        if (pixels_per_y_mesh_pnt >= HD44780_CHAR_HEIGHT) {                 // There are only 2 custom characters available, so the Y
          pixels_per_y_mesh_pnt = HD44780_CHAR_HEIGHT;                      // Size of the mesh point needs to fit within them independent
          suppress_y_offset = 1;                                            // Of where the starting pixel is located.
        }

        x_map_pixels = pixels_per_x_mesh_pnt * (GRID_MAX_POINTS_X);         // Now we have the right number of pixels to make both
        y_map_pixels = pixels_per_y_mesh_pnt * (GRID_MAX_POINTS_Y);         // Directions fit nicely

        right_edge   = pixels_per_x_mesh_pnt * (GRID_MAX_POINTS_X) + 1;     // Find location of right edge within the character cell
        bottom_line  = pixels_per_y_mesh_pnt * (GRID_MAX_POINTS_Y) + 1;     // Find location of bottome line within the character cell

        n_rows = bottom_line / (HD44780_CHAR_HEIGHT) + 1;
        n_cols = right_edge / (HD44780_CHAR_WIDTH) + 1;

        for (i = 0; i < n_cols; i++) {
          lcd_moveto(i, 0);
          lcd_put_wchar(CHAR_LINE_TOP);                                     // Box Top line
          lcd_moveto(i, n_rows - 1);
          lcd_put_wchar(CHAR_LINE_BOT);                                     // Box Bottom line
        }

        for (j = 0; j < n_rows; j++) {
          lcd_moveto(0, j);
          lcd_put_wchar(CHAR_EDGE_L);                                       // Box Left edge
          lcd_moveto(n_cols - 1, j);
          lcd_put_wchar(CHAR_EDGE_R);                                       // Box Right edge
        }

        /**
         * If the entire 4th row is not in use, do not put vertical bars all the way down to the bottom of the display
         */

        k = pixels_per_y_mesh_pnt * (GRID_MAX_POINTS_Y) + 2;
        l = (HD44780_CHAR_HEIGHT) * n_rows;
        if (l > k && l - k >= (HD44780_CHAR_HEIGHT) / 2) {
          lcd_moveto(0, n_rows - 1);                                        // Box Left edge
          lcd_put_wchar(' ');
          lcd_moveto(n_cols - 1, n_rows - 1);                               // Box Right edge
          lcd_put_wchar(' ');
        }

        clear_custom_char(&new_char);
        new_char.custom_char_bits[0] = 0b11111U;                            // Char #0 is used for the box top line
        lcd.createChar(CHAR_LINE_TOP, (uint8_t*)&new_char);

        clear_custom_char(&new_char);
        k = (GRID_MAX_POINTS_Y) * pixels_per_y_mesh_pnt + 1;                // Row of pixels for the bottom box line
        l = k % (HD44780_CHAR_HEIGHT);                                      // Row within relevant character cell
        new_char.custom_char_bits[l] = 0b11111U;                            // Char #1 is used for the box bottom line
        lcd.createChar(CHAR_LINE_BOT, (uint8_t*)&new_char);

        clear_custom_char(&new_char);
        for (j = 0; j < HD44780_CHAR_HEIGHT; j++)
          new_char.custom_char_bits[j] = 0b10000U;                          // Char #2 is used for the box left edge
        lcd.createChar(CHAR_EDGE_L, (uint8_t*)&new_char);

        clear_custom_char(&new_char);
        m = (GRID_MAX_POINTS_X) * pixels_per_x_mesh_pnt + 1;                // Column of pixels for the right box line
        n = m % (HD44780_CHAR_WIDTH);                                       // Column within relevant character cell
        i = HD44780_CHAR_WIDTH - 1 - n;                                     // Column within relevant character cell (0 on the right)
        for (j = 0; j < HD44780_CHAR_HEIGHT; j++)
          new_char.custom_char_bits[j] = (uint8_t)_BV(i);                   // Char #3 is used for the box right edge
        lcd.createChar(CHAR_EDGE_R, (uint8_t*)&new_char);

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

        clear_custom_char(&new_char);
        const uint8_t ypix = MIN(upper_left.y_pixel_offset + pixels_per_y_mesh_pnt, HD44780_CHAR_HEIGHT);
        for (j = upper_left.y_pixel_offset; j < ypix; j++) {
          i = upper_left.x_pixel_mask;
          for (k = 0; k < pixels_per_x_mesh_pnt; k++) {
            new_char.custom_char_bits[j] |= i;
            i >>= 1;
          }
        }

        prep_and_put_map_char(new_char, upper_left, lower_right, bottom_right_corner, TOP_LEFT, CHAR_UL_UL, upper_left.column, upper_left.row);

        /**
         * Next, check for two side by side character cells being used to display the Mesh Point
         * If found...  do the right hand character cell next.
         */
        if (upper_left.column == lower_right.column - 1) {
          l = upper_left.x_pixel_offset;
          clear_custom_char(&new_char);
          for (j = upper_left.y_pixel_offset; j < ypix; j++) {
            i = _BV(HD44780_CHAR_WIDTH - 1);                                // Fill in the left side of the right character cell
            for (k = 0; k < pixels_per_x_mesh_pnt - 1 - l; k++) {
              new_char.custom_char_bits[j] |= i;
              i >>= 1;
            }
          }
          prep_and_put_map_char(new_char, upper_left, lower_right, bottom_right_corner, TOP_RIGHT, CHAR_LR_UL, lower_right.column, upper_left.row);
        }

        /**
         * Next, check for two character cells stacked on top of each other being used to display the Mesh Point
         */
        if (upper_left.row == lower_right.row - 1) {
          l = HD44780_CHAR_HEIGHT - upper_left.y_pixel_offset;              // Number of pixel rows in top character cell
          k = pixels_per_y_mesh_pnt - l;                                    // Number of pixel rows in bottom character cell
          clear_custom_char(&new_char);
          for (j = 0; j < k; j++) {
            i = upper_left.x_pixel_mask;
            for (m = 0; m < pixels_per_x_mesh_pnt; m++) {                   // Fill in the top side of the bottom character cell
              new_char.custom_char_bits[j] |= i;
              if (!(i >>= 1)) break;
            }
          }
          prep_and_put_map_char(new_char, upper_left, lower_right, bottom_right_corner, LOWER_LEFT, CHAR_UL_LR, upper_left.column, lower_right.row);
        }

        /**
         * Next, check for four character cells being used to display the Mesh Point.  If that is
         * what is here, we work to fill in the character cell that is down one and to the right one
         * from the upper_left character cell.
         */

        if (upper_left.column == lower_right.column - 1 && upper_left.row == lower_right.row - 1) {
          l = HD44780_CHAR_HEIGHT - upper_left.y_pixel_offset;              // Number of pixel rows in top character cell
          k = pixels_per_y_mesh_pnt - l;                                    // Number of pixel rows in bottom character cell
          clear_custom_char(&new_char);
          for (j = 0; j < k; j++) {
            l = upper_left.x_pixel_offset;
            i = _BV(HD44780_CHAR_WIDTH - 1);                                // Fill in the left side of the right character cell
            for (m = 0; m < pixels_per_x_mesh_pnt - 1 - l; m++) {           // Fill in the top side of the bottom character cell
              new_char.custom_char_bits[j] |= i;
              i >>= 1;
            }
          }
          prep_and_put_map_char(new_char, upper_left, lower_right, bottom_right_corner, LOWER_RIGHT, CHAR_LR_LR, lower_right.column, lower_right.row);
        }

      #endif

      /**
       * Print plot position
       */
      lcd_moveto(_LCD_W_POS, 0);
      lcd_put_wchar('(');
      lcd_put_u8str(itostr3(x));
      lcd_put_wchar(',');
      lcd_put_u8str(itostr3(inverted_y));
      lcd_put_wchar(')');

      #if LCD_HEIGHT <= 3   // 16x2 or 20x2 display

        /**
         * Print Z values
         */
        _ZLABEL(_LCD_W_POS, 1);
        if (!isnan(ubl.z_values[x][inverted_y]))
          lcd_put_u8str(ftostr43sign(ubl.z_values[x][inverted_y]));
        else
          lcd_put_u8str_P(PSTR(" -----"));

      #else                 // 16x4 or 20x4 display

        /**
         * Show all values at right of screen
         */
        _XLABEL(_LCD_W_POS, 1);
        lcd_put_u8str(ftostr52(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x]))));
        _YLABEL(_LCD_W_POS, 2);
        lcd_put_u8str(ftostr52(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[inverted_y]))));

        /**
         * Show the location value
         */
        _ZLABEL(_LCD_W_POS, 3);
        if (!isnan(ubl.z_values[x][inverted_y]))
          lcd_put_u8str(ftostr43sign(ubl.z_values[x][inverted_y]));
        else
          lcd_put_u8str_P(PSTR(" -----"));

      #endif // LCD_HEIGHT > 3
    }

    void add_edges_to_custom_char(custom_char &custom, const coordinate &ul, const coordinate &lr, const coordinate &brc, const uint8_t cell_location) {
      uint8_t i, k;
      int16_t n_rows = lr.row    - ul.row    + 1,
              n_cols = lr.column - ul.column + 1;

      /**
       * Check if Top line of box needs to be filled in
       */

      if (ul.row == 0 && (cell_location & (TOP_LEFT|TOP_RIGHT))) {   // Only fill in the top line for the top character cells

        if (n_cols == 1) {
          if (ul.column != brc.column)
            custom.custom_char_bits[0] = 0xFF;                              // Single column in middle
          else
            for (i = brc.x_pixel_offset; i < HD44780_CHAR_WIDTH; i++)       // Single column on right side
              SBI(custom.custom_char_bits[0], i);
        }
        else if ((cell_location & TOP_LEFT) || lr.column != brc.column)     // Multiple column in the middle or with right cell in middle
          custom.custom_char_bits[0] = 0xFF;
        else
          for (i = brc.x_pixel_offset; i < HD44780_CHAR_WIDTH; i++)
            SBI(custom.custom_char_bits[0], i);
      }

      /**
       * Check if left line of box needs to be filled in
       */
      if (cell_location & (TOP_LEFT|LOWER_LEFT)) {
        if (ul.column == 0) {                                               // Left column of characters on LCD Display
          k = ul.row == brc.row ? brc.y_pixel_offset : HD44780_CHAR_HEIGHT; // If it isn't the last row... do the full character cell
          for (i = 0; i < k; i++)
            SBI(custom.custom_char_bits[i], HD44780_CHAR_WIDTH - 1);
        }
      }

      /**
       * Check if bottom line of box needs to be filled in
       */

      // Single row of mesh plot cells
      if (n_rows == 1 /* && (cell_location & (TOP_LEFT|TOP_RIGHT)) */ && ul.row == brc.row) {
        if (n_cols == 1)                                                    // Single row, single column case
          k = ul.column == brc.column ? brc.x_pixel_mask : 0x01;
        else if (cell_location & TOP_RIGHT)                                 // Single row, multiple column case
          k = lr.column == brc.column ? brc.x_pixel_mask : 0x01;
        else                                                                // Single row, left of multiple columns
          k = 0x01;
        while (k < _BV(HD44780_CHAR_WIDTH)) {
          custom.custom_char_bits[brc.y_pixel_offset] |= k;
          k <<= 1;
        }
      }

      // Double row of characters on LCD Display
      // And this is a bottom custom character
      if (n_rows == 2 && (cell_location & (LOWER_LEFT|LOWER_RIGHT)) && lr.row == brc.row) {
        if (n_cols == 1)                                                    // Double row, single column case
          k = ul.column == brc.column ? brc.x_pixel_mask : 0x01;
        else if (cell_location & LOWER_RIGHT)                               // Double row, multiple column case
          k = lr.column == brc.column ? brc.x_pixel_mask : 0x01;
        else                                                                // Double row, left of multiple columns
          k = 0x01;
        while (k < _BV(HD44780_CHAR_WIDTH)) {
          custom.custom_char_bits[brc.y_pixel_offset] |= k;
          k <<= 1;
        }
      }

      /**
       * Check if right line of box needs to be filled in
       */

      // Nothing to do if the lower right part of the mesh pnt isn't in the same column as the box line
      if (lr.column == brc.column) {
        // This mesh point is in the same character cell as the right box line
        if (ul.column == brc.column || (cell_location & (TOP_RIGHT|LOWER_RIGHT))) {
          // If not the last row... do the full character cell
          k = ul.row == brc.row ? brc.y_pixel_offset : HD44780_CHAR_HEIGHT;
          for (i = 0; i < k; i++) custom.custom_char_bits[i] |= brc.x_pixel_mask;
        }
      }
    }

  #endif // AUTO_BED_LEVELING_UBL

#endif // HAS_LCD_MENU

#endif // HAS_CHARACTER_LCD
