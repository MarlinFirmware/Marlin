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

#include "MarlinConfig.h"
#include "types.h"
#include "enum.h"
//#include "duration_t.h"
#include "utility.h"


extern volatile uint8_t buttons;  //an extended version of the last checked buttons in a bit array.
void lcd_implementation_clear();
void lcd_implementation_drawmenu_static(const uint8_t row, const char* pstr, const bool center=true, const bool invert=false, const char *valstr=NULL);
void lcd_implementation_status_screen();
void lcd_implementation_drawmenu_generic(const bool sel, const uint8_t row, const char* pstr, const char pre_char, const char post_char);
void lcd_implementation_drawedit(const char* pstr, const char* const value=NULL);
void lcd_implementation_drawmenu_setting_edit_generic_P(const bool sel, const uint8_t row, const char* pstr, const char pre_char, const char* const data);

void lcd_implementation_init(
  #if ENABLED(LCD_PROGRESS_BAR)
    const bool info_screen_charset = true
  #endif
);


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

#elif ENABLED(LCD_I2C_TYPE_MCP23017)
  //for the LED indicators (which maybe mapped to different things in lcd_implementation_update_indicators())
  #define LED_A 0x04 //100
  #define LED_B 0x02 //010
  #define LED_C 0x01 //001

  #define LCD_HAS_STATUS_INDICATORS

  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2

#elif ENABLED(LCD_I2C_TYPE_MCP23008)
  #include <Wire.h>
  #include <LiquidTWI2.h>
  #define LCD_CLASS LiquidTWI2

#elif ENABLED(LCD_I2C_TYPE_PCA8574)
  #include <LiquidCrystal_I2C.h>
  #define LCD_CLASS LiquidCrystal_I2C

// 2 wire Non-latching LCD SR from:
// https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/schematics#!shiftregister-connection
#elif ENABLED(SR_LCD_2W_NL)
  extern "C" void __cxa_pure_virtual() { while (1); }
  #include <LCD.h>
  #include <LiquidCrystal_SR.h>
  #define LCD_CLASS LiquidCrystal_SR
#elif ENABLED(LCM1602)
  #include <Wire.h>
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  #define LCD_CLASS LiquidCrystal_I2C
#else
  // Standard directly connected LCD implementations
  #include <LiquidCrystal.h>
  #define LCD_CLASS LiquidCrystal
#endif

#include "utf_mapper.h"

#if ENABLED(LCD_PROGRESS_BAR)
  extern millis_t progress_bar_ms;
  #if PROGRESS_MSG_EXPIRE > 0
    extern millis_t expire_status_ms;
  #endif
  #define LCD_STR_PROGRESS  "\x03\x04\x05"
#endif

#if ENABLED(LCD_HAS_STATUS_INDICATORS)
  static void lcd_implementation_update_indicators();
#endif


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


  #define DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(_type, _name, _strFunc) \
    void lcd_implementation_drawmenu_setting_edit_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...) { \
      lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', _strFunc(*(data))); \
    } \
    void lcd_implementation_drawmenu_setting_edit_callback_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...) { \
      lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', _strFunc(*(data))); \
    } \
    void lcd_implementation_drawmenu_setting_edit_accessor_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type (*pget)(), void (*pset)(_type), ...) { \
      lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, '>', _strFunc(pget())); \
    } \
    typedef void _name##_void



  #define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
  #define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
  #define lcd_implementation_drawmenu_setting_edit_accessor_bool(sel, row, pstr, pstr2, pget, pset, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))


  #define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_UPLEVEL_CHAR, LCD_UPLEVEL_CHAR)
  #define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', LCD_STR_ARROW_RIGHT[0])
  #define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')
  #define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, '>', ' ')

  #if ENABLED(LCD_HAS_SLOW_BUTTONS)

    extern millis_t next_button_update_ms;

  #endif // LCD_HAS_SLOW_BUTTONS

//#endif // ULTIPANEL

#ifdef AUTO_BED_LEVELING_UBL
    // These are just place holders for the 20x4 LCD work that
    // is coming up very soon.   Soon this will morph into the
    // real code.
    void lcd_return_to_status();       
    void _lcd_ubl_plot_HD44780(uint8_t x_plot, uint8_t y_plot);

#endif // AUTO_BED_LEVELING_UBL

extern LCD_CLASS lcd;

/*
 #define DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(_type, _name, _strFunc) \
    inline void lcd_implementation_drawmenu_setting_edit_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...);\
    inline void lcd_implementation_drawmenu_setting_edit_callback_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type * const data, ...);\
    inline void lcd_implementation_drawmenu_setting_edit_accessor_ ## _name (const bool sel, const uint8_t row, const char* pstr, const char* pstr2, _type (*pget)(), void (*pset)(_type), ...);\
    typedef void _name##_void

  #define lcd_implementation_drawmenu_setting_edit_bool(sel, row, pstr, pstr2, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
  #define lcd_implementation_drawmenu_setting_edit_callback_bool(sel, row, pstr, pstr2, data, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))
  #define lcd_implementation_drawmenu_setting_edit_accessor_bool(sel, row, pstr, pstr2, pget, pset, callback) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, '>', (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF))

  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(int, int3, itostr3);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float3, ftostr3);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float32, ftostr32);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float43, ftostr43sign);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float5, ftostr5rj);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float51, ftostr51sign);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float52, ftostr52sign);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(float, float62, ftostr62rj);
  DEFINE_LCD_IMPLEMENTATION_DRAWMENU_SETTING_EDIT_TYPE(unsigned long, long5, ftostr5rj);
*/

#endif // ULTRALCD_IMPL_HD44780_H
