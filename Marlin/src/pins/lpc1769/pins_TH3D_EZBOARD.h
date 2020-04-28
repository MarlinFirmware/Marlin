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

/**
 * TH3D EZBoard pin assignments
 */

#ifndef MCU_LPC1769
  #error "Oops! Make sure you have the LPC1769 environment selected in your IDE."
#endif

#define BOARD_INFO_NAME   "TH3D EZBoard"
#define BOARD_WEBSITE_URL "th3dstudio.com"

//
// Servos
//
#define SERVO0_PIN                         P2_04

//
// Limit Switches
//
#define X_STOP_PIN                         P1_24
#define Y_STOP_PIN                         P1_25
#define Z_STOP_PIN                         P1_26

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                   P1_27
#endif

//
// Steppers
//
#define X_STEP_PIN                         P2_00
#define X_DIR_PIN                          P1_16
#define X_ENABLE_PIN                       P1_17

#define Y_STEP_PIN                         P2_01
#define Y_DIR_PIN                          P1_10
#define Y_ENABLE_PIN                       P1_09

#define Z_STEP_PIN                         P2_02
#define Z_DIR_PIN                          P1_15
#define Z_ENABLE_PIN                       P1_14

#define E0_STEP_PIN                        P2_03
#define E0_DIR_PIN                         P1_04
#define E0_ENABLE_PIN                      P1_08

#define E1_STEP_PIN                        P2_08
#define E1_DIR_PIN                         P2_13
#define E1_ENABLE_PIN                      P4_29

#if HAS_TMC_UART
  //
  // TMC220x stepper drivers
  // Software serial
  //
  #define X_SERIAL_TX_PIN                  P0_04
  #define X_SERIAL_RX_PIN                  P0_05
  #define Y_SERIAL_TX_PIN                  P0_10
  #define Y_SERIAL_RX_PIN                  P0_11
  #define Z_SERIAL_TX_PIN                  P0_19
  #define Z_SERIAL_RX_PIN                  P0_20
  #define E0_SERIAL_TX_PIN                 P0_22
  #define E0_SERIAL_RX_PIN                 P0_21

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Temp Sensors
//  3.3V max when defined as an Analog Input!
//
#if TEMP_SENSOR_0 == 20                           // PT100 Adapter
  #define TEMP_0_PIN                    P0_02_A7  // Analog Input
#else
  #define TEMP_0_PIN                    P0_23_A0  // Analog Input P0_23
#endif

#define TEMP_BED_PIN                    P0_24_A1  // Analog Input P0_24
#define TEMP_1_PIN                      P0_25_A2  // Analog Input P0_25

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #define FILWIDTH_PIN                  P0_26_A3  // Analog Input P0_26
#else
  #define TEMP_2_PIN                    P0_26_A3  // Analog Input P0_26
#endif

//
// Heaters / Fans
//
#define HEATER_BED_PIN                     P2_05
#define HEATER_0_PIN                       P2_07
#ifndef FAN_PIN
  #define FAN_PIN                          P2_06
#endif
#define FAN1_PIN                           P1_22

//
// Auto fans
//
#define AUTO_FAN_PIN                       P1_22  // FET 3
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif

//
// SD Card
//

#define SDCARD_CONNECTION                ONBOARD

#define SCK_PIN                            P0_07
#define MISO_PIN                           P0_08
#define MOSI_PIN                           P0_09
#define ONBOARD_SD_CS_PIN                  P0_06
#define SS_PIN                 ONBOARD_SD_CS_PIN

//
// LCD / Controller
//

/**
 *                  _____
 *              5V | · · | GND
 *  (LCD_EN) P0_18 | · · | P0_16 (LCD_RS)
 *  (LCD_D4) P0_15 | · · | P3_25 (BTN_EN2)
 *   (RESET) P2_11 | · · | P3_26 (BTN_EN1)
 * (BTN_ENC) P1_30 | · · | P1_31 (BEEPER)
 *                  -----
 *                  EXP1
 *
 * LCD_PINS_D5, D6, and D7 are not present in the EXP1 connector, and will need to be
 * defined to use the REPRAP_DISCOUNT_SMART_CONTROLLER.
 *
 * A remote SD card is currently not supported because the pins routed to the EXP2
 * connector are shared with the onboard SD card.
 *
 */

#if ENABLED(CR10_STOCKDISPLAY)
  #define BEEPER_PIN                       P1_31
  #define BTN_EN1                          P3_26
  #define BTN_EN2                          P3_25
  #define BTN_ENC                          P1_30
  #define LCD_PINS_RS                      P0_16
  #define LCD_PINS_ENABLE                  P0_18
  #define LCD_PINS_D4                      P0_15
  #define KILL_PIN                         P2_11
#elif HAS_SPI_LCD
  #error "Only the CR10_STOCKDISPLAY is supported with TH3D EZBoard."
#endif
