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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#ifndef MCU_LPC1768
  #error "Oops! Make sure you have the LPC1768 environment selected in your IDE."
#endif

#define BOARD_INFO_NAME "GMARSH X6 REV1"

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Enable 12MHz clock output on P1.27 pin to sync TMC2208 chip clocks
//
#define LPC1768_ENABLE_CLKOUT_12M

//
// Servos
//
#define SERVO0_PIN                         P1_26  // PWM1[6]
#define SERVO1_PIN                         P1_18  // PWM1[1]

//
// Limit Switches
//
#define X_MIN_PIN                          P0_00
#define X_MAX_PIN                          P0_01
#define Y_MIN_PIN                          P0_10
#define Y_MAX_PIN                          P0_21
#define Z_MIN_PIN                          P2_13
#define Z_MAX_PIN                          P2_22

//
// Steppers
//

#define X_STEP_PIN                         P1_01
#define X_DIR_PIN                          P1_04
#define X_ENABLE_PIN                       P0_26

#define Y_STEP_PIN                         P1_10
#define Y_DIR_PIN                          P1_14
#define Y_ENABLE_PIN                       P1_08

#define Z_STEP_PIN                         P1_17
#define Z_DIR_PIN                          P4_29
#define Z_ENABLE_PIN                       P1_15

#define E0_STEP_PIN                        P0_05
#define E0_DIR_PIN                         P2_00
#define E0_ENABLE_PIN                      P4_28

#define E1_STEP_PIN                        P2_03
#define E1_DIR_PIN                         P2_04
#define E1_ENABLE_PIN                      P2_01

#define E2_STEP_PIN                        P2_07
#define E2_DIR_PIN                         P2_08
#define E2_ENABLE_PIN                      P2_05

//
// TMC2208 UART pins
//
#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                  P1_00
  #define X_SERIAL_RX_PIN                  P1_00
  #define Y_SERIAL_TX_PIN                  P1_09
  #define Y_SERIAL_RX_PIN                  P1_09
  #define Z_SERIAL_TX_PIN                  P1_16
  #define Z_SERIAL_RX_PIN                  P1_16
  #define E0_SERIAL_TX_PIN                 P0_04
  #define E0_SERIAL_RX_PIN                 P0_04
  #define E1_SERIAL_TX_PIN                 P2_02
  #define E1_SERIAL_RX_PIN                 P2_02
  #define E2_SERIAL_TX_PIN                 P2_06
  #define E2_SERIAL_RX_PIN                 P2_06

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#else
  #error "TMC2208 UART configuration is required for GMarsh X6."
#endif

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN                      P0_24_A1  // AD0[0] on P0_23
#define TEMP_BED_PIN                    P0_23_A0  // AD0[1] on P0_24

//
// Heaters / Fans
//
#define HEATER_BED_PIN                     P1_19  // Not a PWM pin, software PWM required
#define HEATER_0_PIN                       P3_26  // PWM1[3]
#define FAN_PIN                            P3_25  // Part cooling fan - connected to PWM1[2]
#define E0_AUTO_FAN_PIN                    P0_27  // Extruder cooling fan

//
// Misc. Functions
//
#define LED_PIN                            P1_31

//
// LCD
//
#if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
  #define BEEPER_PIN                       P0_19
  #define BTN_EN1                          P1_23
  #define BTN_EN2                          P1_24
  #define BTN_ENC                          P1_25
  #define LCD_PINS_RS                      P0_20
  #define LCD_PINS_ENABLE                  P0_21
  #define LCD_PINS_D4                      P2_11
  #define LCD_PINS_D5                      P0_22
  #define LCD_PINS_D6                      P1_29
  #define LCD_PINS_D7                      P1_28
#endif

//
// SD Support
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION                  LCD
#endif

#define ONBOARD_SD_CS_PIN                  P0_06  // Chip select for "System" SD card

#if SD_CONNECTION_IS(LCD)
  #define SCK_PIN                          P0_15
  #define MISO_PIN                         P0_17
  #define MOSI_PIN                         P0_18
  #define SS_PIN                           P0_16
#elif SD_CONNECTION_IS(ONBOARD)
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN                    P0_27
  #define SCK_PIN                          P0_07
  #define MISO_PIN                         P0_08
  #define MOSI_PIN                         P0_09
  #define SS_PIN               ONBOARD_SD_CS_PIN
#endif
