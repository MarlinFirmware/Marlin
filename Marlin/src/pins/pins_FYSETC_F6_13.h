/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// Fysetc F6 pin assignments
//

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'FYSETC F6' in 'Tools > Board.'"
#endif

#if ENABLED(SD_DETECT_INVERTED)
  //#error "SD_DETECT_INVERTED must be disabled for the FYSETC_F6_13 board."
#endif

#define BOARD_NAME "FYSETC F6 1.3"

#define RESET_PIN          30
#define SPI_FLASH_CS       83

//
// Servos
//
#define SERVO0_PIN         13
#define SERVO1_PIN         11    // (PS_ON_PIN)
#define SERVO2_PIN         10    // (FIL_RUNOUT_PIN)
#define SERVO3_PIN          4    // (RGB_LED_G_PIN)

//
// Limit Switches
//
#define X_MIN_PIN          63
#define X_MAX_PIN          64
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          12
#define Z_MAX_PIN           9

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   SERVO2_PIN
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN   9   // Servos pin
#endif

//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#ifndef X_CS_PIN
  #define X_CS_PIN         70
#endif

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#ifndef Y_CS_PIN
  #define Y_CS_PIN         39
#endif

#define Z_STEP_PIN         43
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       58
#ifndef Z_CS_PIN
  #define Z_CS_PIN         74
#endif

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#ifndef E0_CS_PIN
  #define E0_CS_PIN        47
#endif

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#ifndef E1_CS_PIN
  #define E1_CS_PIN        32
#endif

#define E2_STEP_PIN        59
#define E2_DIR_PIN         57
#define E2_ENABLE_PIN      40
#ifndef E2_CS_PIN
  #define E2_CS_PIN        42
#endif

//
// Sensorless homing DIAG pin is not directly connected to the MCU. Close
// the jumper next to the limit switch socket when using sensorless homing.
//

#if HAS_DRIVER(TMC2208)
  // Software serial
  #define X_SERIAL_RX_PIN  71
  #define X_SERIAL_TX_PIN  72
  #define Y_SERIAL_RX_PIN  73
  #define Y_SERIAL_TX_PIN  75
  #define Z_SERIAL_RX_PIN  78
  #define Z_SERIAL_TX_PIN  79
  #define E0_SERIAL_RX_PIN 76
  #define E0_SERIAL_TX_PIN 77
  #define E1_SERIAL_RX_PIN 80
  #define E1_SERIAL_TX_PIN 81
  #define E2_SERIAL_RX_PIN 22
  #define E2_SERIAL_TX_PIN 82
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         12   // Analog Input
#define TEMP_1_PIN         13   // Analog Input
#define TEMP_2_PIN         14   // Analog Input
#define TEMP_BED_PIN       15   // Analog Input

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN      9   // Analog Input on X+ endstop
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        5
#define HEATER_1_PIN        6
#define HEATER_2_PIN        7
#define HEATER_BED_PIN      8

#define FAN_PIN            44
#define FAN1_PIN           45
#define FAN2_PIN           46

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define KILL_PIN           41

#ifndef PS_ON_PIN
  #define PS_ON_PIN        SERVO1_PIN
#endif

/**
 *               -----                                             -----
 *       5V/D41 | · · | GND                                    5V | · · | GND
 *        RESET | · · | D49 (SD_DETECT)             (LCD_D7)  D29 | · · | D27  (LCD_D6)
 *   (MOSI) D51 | · · | D33 (BTN_EN2)               (LCD_D5)  D25 | · · | D23  (LCD_D4)
 *  (SD_SS) D53 | · · | D31 (BTN_EN1)               (LCD_RS)  D16 | · · | D17  (LCD_EN)
 *    (SCK) D52 | · · | D50 (MISO)                 (BTN_ENC)  D35 | · · | D37  (BEEPER)
 *               -----                                             -----
 *               EXP2                                              EXP1
 */

//
// LCDs and Controllers
//
#define BEEPER_PIN         37
#define SD_DETECT_PIN      49

#if ENABLED(FYSETC_MINI_12864)
  //
  // See https://wiki.fysetc.com/Mini12864_Panel/?fbclid=IwAR1FyjuNdVOOy9_xzky3qqo_WeM5h-4gpRnnWhQr_O1Ef3h0AFnFXmCehK8
  //
  #define DOGLCD_A0        16
  #define DOGLCD_CS        17

  #define LCD_BACKLIGHT_PIN -1
  #define KILL_PIN         41

  #define LCD_RESET_PIN    23   // Must be high or open for LCD to operate normally.
                                // Seems to work best if left open.

  #define FYSETC_MINI_12864_REV_1_2
  //#define FYSETC_MINI_12864_REV_2_0
  //#define FYSETC_MINI_12864_REV_2_1
  #if EITHER(FYSETC_MINI_12864_REV_1_2, FYSETC_MINI_12864_REV_2_0)
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN 25
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN 27
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN 29
    #endif
  #elif defined(FYSETC_MINI_12864_REV_2_1)
    #define NEOPIXEL_LED
    #define NEOPIXEL_TYPE   NEO_GRB  // NEO_GRBW / NEO_GRB - four/three channel driver type (defined in Adafruit_NeoPixel.h)
    #define NEOPIXEL_PIN    25       // LED driving pin on motherboard 4 => D4 (EXP2-5 on Printrboard) / 30 => PC7 (EXP3-13 on Rumba)
    #define NEOPIXEL_PIXELS  3       // Number of LEDs in the strip
    #define NEOPIXEL_IS_SEQUENTIAL   // Sequential display for temperature change - LED by LED. Disable to change all LEDs at once.
    #define NEOPIXEL_BRIGHTNESS 127  // Initial brightness (0-255)
    #define NEOPIXEL_STARTUP_TEST    // Cycle through colors at startup
  #else
    #error "Either FYSETC_MINI_12864_REV_1_2, FYSETC_MINI_12864_REV_2_0 or FYSETC_MINI_12864_REV_2_1 must be defined"
  #endif

  #if !defined(LED_USER_PRESET_STARTUP) && EITHER(FYSETC_MINI_12864_REV_2_0, FYSETC_MINI_12864_REV_2_1)
    #error "LED_USER_PRESET_STARTUP must be enabled when using FYSETC_MINI_12864 REV 2.0 and later"
  #endif

#elif HAS_GRAPHICAL_LCD

  #define LCD_PINS_RS      16
  #define LCD_PINS_ENABLE  17
  #define LCD_PINS_D4      23
  #define LCD_PINS_D5      25
  #define LCD_PINS_D6      27
  #define LCD_PINS_D7      29

  #if ENABLED(MKS_MINI_12864)
    #define DOGLCD_CS      25
    #define DOGLCD_A0      27
  #endif

#endif

#if ENABLED(NEWPANEL)
  #define BTN_EN1          31
  #define BTN_EN2          33
  #define BTN_ENC          35
#endif

#ifndef RGB_LED_R_PIN
  #define RGB_LED_R_PIN     3
#endif
#ifndef RGB_LED_G_PIN
  #define RGB_LED_G_PIN     4
#endif
#ifndef RGB_LED_B_PIN
  #define RGB_LED_B_PIN     9
#endif
#ifndef RGB_LED_W_PIN
  #define RGB_LED_W_PIN    -1
#endif
