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
 * Anet board pin assignments
 */

#if defined(__AVR_ATmega1284P__)
#define VARIANT_STANDARD true
#endif

#include "pins_SANGUINOLOLU_12.h"

#undef BOARD_NAME
#define BOARD_NAME "Anet"

#undef LED_PIN
#undef FAN_PIN

#define FAN_PIN 4

#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)
  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7

  #if ENABLED(ADC_KEYPAD)
    #undef BTN_EN1
    #undef BTN_EN2
    #undef BTN_ENC

    #define SERVO0_PIN         27 // free for BLTouch/3D-Touch
    #define LCD_PINS_RS        28
    #define LCD_PINS_ENABLE    29
    #define LCD_PINS_D4        10
    #define LCD_PINS_D5        11
    #define LCD_PINS_D6        16
    #define LCD_PINS_D7        17

    #define BTN_EN1            -1
    #define BTN_EN2            -1
    #define BTN_ENC            -1

    #define ADC_KEYPAD_PIN      1

    #define ENCODER_FEEDRATE_DEADZONE 2
  #elif ENABLED(U8GLIB_ST7920)
    #undef BEEPER_PIN

  #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    // For RepRap Discount (with Anet Adapter wiring)
    #define SERVO0_PIN        27 // free for BLTouch/3D-Touch
    #define BEEPER_PIN        28
    #define LCD_PINS_RS       30
    #define	LCD_PINS_ENABLE   29
    #define LCD_PINS_D4       17
    #define BTN_EN1               11
    #define BTN_EN2               10
  #elif ENABLED(ANET_FULL_GRAPHICS_LCD)
    #define SERVO0_PIN         29 // free for BLTouch/3D-Touch
    #define BEEPER_PIN 17
    #define LCD_PINS_RS        27
    #define LCD_PINS_ENABLE    28
    #define LCD_PINS_D4        30
  #else
    #error "You need to select ANET or RepRap Version"
  #endif

    #define ST7920_DELAY_1 DELAY_0_NOP
    #define ST7920_DELAY_2 DELAY_1_NOP
    #define ST7920_DELAY_3 DELAY_2_NOP

    #ifndef ENCODER_STEPS_PER_MENU_ITEM
      #define ENCODER_STEPS_PER_MENU_ITEM 1
    #endif
    #ifndef ENCODER_PULSES_PER_STEP
      #define ENCODER_PULSES_PER_STEP 4
    #endif
  #endif

#endif


