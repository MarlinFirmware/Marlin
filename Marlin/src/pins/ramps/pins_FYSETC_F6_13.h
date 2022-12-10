/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

//
// FYSETC F6 1.3 (and 1.4) pin assignments
// ATmega2560
//

#if NOT_TARGET(__AVR_ATmega2560__)
  #error "Oops! Select 'FYSETC F6' in 'Tools > Board.'"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "FYSETC F6 1.3"
#endif

#define RESET_PIN                          PinC7
#define SPI_FLASH_CS_PIN                   PinD6

//
// Servos
//
#define SERVO0_PIN                         PinB7
#define SERVO1_PIN                         PinB5  // (PS_ON_PIN)
#define SERVO2_PIN                         PinB4  // (FIL_RUNOUT_PIN)
#define SERVO3_PIN                         PinG5  // (RGB_LED_G_PIN)

//
// Limit Switches
//
#define X_MIN_PIN                          PinK1
#define X_MAX_PIN                          PinK2
#define Y_MIN_PIN                          PinJ1
#define Y_MAX_PIN                          PinJ0
#define Z_MIN_PIN                          PinB6
#ifndef Z_MAX_PIN
  #define Z_MAX_PIN                        PinH6
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN              SERVO2_PIN
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  PinH6  // Servos pin
#endif

//
// Steppers
//
#define X_STEP_PIN                         PinF0
#define X_DIR_PIN                          PinF1
#define X_ENABLE_PIN                       PinD7
#ifndef X_CS_PIN
  #define X_CS_PIN                         PinG4
#endif

#define Y_STEP_PIN                         PinF6
#define Y_DIR_PIN                          PinF7
#define Y_ENABLE_PIN                       PinF2
#ifndef Y_CS_PIN
  #define Y_CS_PIN                         PinG2
#endif

#define Z_STEP_PIN                         PinL6
#define Z_DIR_PIN                          PinL1
#define Z_ENABLE_PIN                       PinF4
#ifndef Z_CS_PIN
  #define Z_CS_PIN                         PinJ7
#endif

#define E0_STEP_PIN                        PinA4
#define E0_DIR_PIN                         PinA6
#define E0_ENABLE_PIN                      PinA2
#ifndef E0_CS_PIN
  #define E0_CS_PIN                        PinL2
#endif

#define E1_STEP_PIN                        PinC1
#define E1_DIR_PIN                         PinC3
#define E1_ENABLE_PIN                      PinC7
#ifndef E1_CS_PIN
  #define E1_CS_PIN                        PinC5
#endif

#define E2_STEP_PIN                        PinF5
#define E2_DIR_PIN                         PinF3
#define E2_ENABLE_PIN                      PinG1
#ifndef E2_CS_PIN
  #define E2_CS_PIN                        PinL7
#endif

//
// Sensorless homing DIAG pin is not directly connected to the MCU. Close
// the jumper next to the limit switch socket when using sensorless homing.
//

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Software serial communication pins.
   * At the moment, F6 rx pins are not pc interrupt pins
   */
  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN                PinJ2
  #endif
  #ifndef X_SERIAL_RX_PIN
    #define X_SERIAL_RX_PIN                   -1  // 71
  #endif
  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN                PinJ4
  #endif
  #ifndef Y_SERIAL_RX_PIN
    #define Y_SERIAL_RX_PIN                   -1  // 73
  #endif
  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN                PinE6
  #endif
  #ifndef Z_SERIAL_RX_PIN
    #define Z_SERIAL_RX_PIN                   -1  // 78
  #endif
  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN               PinJ6
  #endif
  #ifndef E0_SERIAL_RX_PIN
    #define E0_SERIAL_RX_PIN                  -1  // 76
  #endif
  #ifndef E1_SERIAL_TX_PIN
    #define E1_SERIAL_TX_PIN               PinD4
  #endif
  #ifndef E1_SERIAL_RX_PIN
    #define E1_SERIAL_RX_PIN                  -1  // 80
  #endif
  #ifndef E2_SERIAL_TX_PIN
    #define E2_SERIAL_TX_PIN               PinD5
  #endif
  #ifndef E2_SERIAL_RX_PIN
    #define E2_SERIAL_RX_PIN                  -1  // 22
  #endif
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                         PinB6  // Analog Input
#define TEMP_1_PIN                         PinB7  // Analog Input
#define TEMP_2_PIN                         PinJ1  // Analog Input
#define TEMP_BED_PIN                       PinJ0  // Analog Input

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                     PinH6  // Analog Input on X+ endstop
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                       PinE3
#define HEATER_1_PIN                       PinH3
#define HEATER_2_PIN                       PinH4
#define HEATER_BED_PIN                     PinH5

#define FAN_PIN                            PinL5
#define FAN1_PIN                           PinL4
#define FAN2_PIN                           PinL3

//
// Misc. Functions
//
#define SDSS                               PinB0
#define LED_PIN                            PinB7
#define KILL_PIN                           PinG0

#ifndef PS_ON_PIN
  #define PS_ON_PIN                   SERVO1_PIN
#endif

/**
 *               -----                                             -----
 *       5V/D41 | · · | GND                                    5V | · · | GND
 *        RESET | · · | D49 (SD_DETECT)             (LCD_D7)  D29 | · · | D27  (LCD_D6)
 *   (MOSI) D51 | · ·   D33 (BTN_EN2)               (LCD_D5)  D25 | · ·   D23  (LCD_D4)
 *  (SD_SS) D53 | · · | D31 (BTN_EN1)               (LCD_RS)  D16 | · · | D17  (LCD_EN)
 *    (SCK) D52 | · · | D50 (MISO)                 (BTN_ENC)  D35 | · · | D37  (BEEPER)
 *               -----                                             -----
 *               EXP2                                              EXP1
 */

//
// LCDs and Controllers
//
#define SD_DETECT_PIN                      PinL0

#if ENABLED(FYSETC_242_OLED_12864)
  #define BTN_EN1                          PinC0
  #define BTN_EN2                          PinA7
  #define BTN_ENC                          PinC2
  #define BEEPER_PIN                       PinC6

  #define LCD_PINS_DC                      PinA3
  #define LCD_PINS_RS                      PinC4
  #define DOGLCD_CS                        PinH1
  #define DOGLCD_MOSI                      PinA1
  #define DOGLCD_SCK                       PinH0
  #define DOGLCD_A0                  LCD_PINS_DC

  #undef KILL_PIN
  #define NEOPIXEL_PIN                     PinA5

#else
  #define BEEPER_PIN                       PinC0

  #if ENABLED(FYSETC_MINI_12864)
    //
    // See https://wiki.fysetc.com/Mini12864_Panel/
    //
    #define DOGLCD_A0                      PinH1
    #define DOGLCD_CS                      PinH0

    #if ENABLED(FYSETC_GENERIC_12864_1_1)
      #define LCD_BACKLIGHT_PIN            PinA5
    #endif

    #define LCD_RESET_PIN                     23  // Must be high or open for LCD to operate normally.
                                  // Seems to work best if left open.

    #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
      #ifndef RGB_LED_R_PIN
        #define RGB_LED_R_PIN              PinA3
      #endif
      #ifndef RGB_LED_G_PIN
        #define RGB_LED_G_PIN              PinA5
      #endif
      #ifndef RGB_LED_B_PIN
        #define RGB_LED_B_PIN              PinA7
      #endif
    #elif ENABLED(FYSETC_MINI_12864_2_1)
      #define NEOPIXEL_PIN                 PinA3
    #endif

  #elif HAS_MARLINUI_U8GLIB || HAS_MARLINUI_HD44780

    #define LCD_PINS_RS                    PinH1
    #define LCD_PINS_ENABLE                PinH0
    #define LCD_PINS_D4                    PinA1
    #define LCD_PINS_D5                    PinA3
    #define LCD_PINS_D6                    PinA5
    #define LCD_PINS_D7                    PinA7

    #if ENABLED(MKS_MINI_12864)
      #define DOGLCD_CS                    PinA3
      #define DOGLCD_A0                    PinA5
    #endif

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif

  #endif

  #if IS_NEWPANEL
    #define BTN_EN1                        PinC6
    #define BTN_EN2                        PinC4
    #define BTN_ENC                        PinC2
  #endif
#endif

#ifndef RGB_LED_R_PIN
  #define RGB_LED_R_PIN                    PinE5
#endif
#ifndef RGB_LED_G_PIN
  #define RGB_LED_G_PIN                    PinG5
#endif
#ifndef RGB_LED_B_PIN
  #define RGB_LED_B_PIN                    PinH6
#endif
#ifndef RGB_LED_W_PIN
  #define RGB_LED_W_PIN                       -1
#endif
