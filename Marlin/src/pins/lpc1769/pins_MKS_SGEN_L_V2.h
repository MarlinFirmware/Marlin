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

/**
 * MKS SGen pin assignments
 */

#if NOT_TARGET(MCU_LPC1769)
  #error "Oops! Make sure you have the LPC1769 environment selected in your IDE."
#endif

#define BOARD_INFO_NAME   "MKS SGEN_L V2"
#define BOARD_WEBSITE_URL "github.com/makerbase-mks"

//
// EEPROM, MKS SGEN_L V2.0 hardware has 4K EEPROM on the board
//
#if NO_EEPROM_SELECTED
  //#define SDCARD_EEPROM_EMULATION
  //#define I2C_EEPROM                            // AT24C32
  #define FLASH_EEPROM_EMULATION
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Servos
//
#define SERVO0_PIN                         P1_23  // SERVO P1.23
#define SERVO1_PIN                         P2_00  // SERVO P2.0

//
// Trinamic Stallguard pins, can connect or disconnect by jumpers cap on the board
//
#define X_DIAG_PIN                         P1_29  // X-
#define Y_DIAG_PIN                         P1_27  // Y-
#define Z_DIAG_PIN                         P1_25  // Z-
#define E0_DIAG_PIN                        P1_28  // X+
#define E1_DIAG_PIN                        P1_26  // Y+

//
// Limit Switches
//
#if X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_DIR < 0
    #define X_MAX_PIN                      P1_28  // X+
  #else
    #define X_MIN_PIN                      P1_28  // X+
  #endif
#else
  #define X_MIN_PIN                        P1_29  // X-
  #define X_MAX_PIN                        P1_28  // X+
#endif

#if Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_DIR < 0
    #define Y_MAX_PIN                      P1_26  // Y+
  #else
    #define Y_MIN_PIN                      P1_26  // Y+
  #endif
#else
  #define Y_MIN_PIN                        P1_27  // Y-
  #define Y_MAX_PIN                        P1_26  // Y+
#endif

#if Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_DIR < 0
    #define Z_MAX_PIN                      P1_24  // Z+
  #else
    #define Z_MIN_PIN                      P1_24  // Z+
  #endif
#else
  #define Z_MIN_PIN                        P1_25  // Z-
  #define Z_MAX_PIN                        P1_24  // Z+
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  P1_24
#endif

//
// Steppers
//
#define X_STEP_PIN                         P2_02
#define X_DIR_PIN                          P2_03
#define X_ENABLE_PIN                       P2_01
#ifndef X_CS_PIN
  #define X_CS_PIN                         P1_01
#endif

#define Y_STEP_PIN                         P0_19
#define Y_DIR_PIN                          P0_20
#define Y_ENABLE_PIN                       P2_08
#ifndef Y_CS_PIN
  #define Y_CS_PIN                         P1_08
#endif

#define Z_STEP_PIN                         P0_22
#define Z_DIR_PIN                          P2_11
#define Z_ENABLE_PIN                       P0_21
#ifndef Z_CS_PIN
  #define Z_CS_PIN                         P1_10
#endif

#define E0_STEP_PIN                        P2_13
#define E0_DIR_PIN                         P0_11
#define E0_ENABLE_PIN                      P2_12
#ifndef E0_CS_PIN
  #define E0_CS_PIN                        P1_15
#endif

#define E1_STEP_PIN                        P1_09
#define E1_DIR_PIN                         P1_14
#define E1_ENABLE_PIN                      P0_10
#ifndef E1_CS_PIN
  #define E1_CS_PIN                        P1_17
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                    P1_16
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                    P0_05
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                     P0_04
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                  P1_01
  #define X_SERIAL_RX_PIN                  P1_01
  #define Y_SERIAL_TX_PIN                  P1_08
  #define Y_SERIAL_RX_PIN                  P1_08
  #define Z_SERIAL_TX_PIN                  P1_10
  #define Z_SERIAL_RX_PIN                  P1_10
  #define E0_SERIAL_TX_PIN                 P1_15
  #define E0_SERIAL_RX_PIN                 P1_15
  #define E1_SERIAL_TX_PIN                 P1_17
  #define E1_SERIAL_RX_PIN                 P1_17

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif // HAS_TMC_UART

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//
#define TEMP_0_PIN                      P0_23_A0  // Analog Input A0 (TH1)
#define TEMP_BED_PIN                    P0_24_A1  // Analog Input A1 (TB)
#define TEMP_1_PIN                      P0_25_A2  // Analog Input A2 (TH2)
#define TEMP_2_PIN                      P0_26_A3  // Analog Input A3 (P0.26, No pull up)

//
// Heaters / Fans
//
#define HEATER_BED_PIN                     P2_05
#define HEATER_0_PIN                       P2_07
#if HAS_MULTI_HOTEND
  #ifndef HEATER_1_PIN
    #define HEATER_1_PIN                   P2_06
  #endif
#else
  #ifndef FAN2_PIN
    #define FAN2_PIN                       P2_06  // HE1 for FAN3
  #endif
#endif
#ifndef FAN_PIN
  #define FAN_PIN                          P2_04  // FAN1
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                         P1_04  // FAN2
#endif

//
// Misc. Functions
//
#define LED_PIN                            P1_18  // Used as a status indicator
#define LED2_PIN                           P1_19
#define LED3_PIN                           P1_20
#define LED4_PIN                           P1_21

/**
 *                _____                                            _____
 * (BEEPER) 1.31 | · · | 1.30 (BTN_ENC)          (MISO)       0.8 | · · | 0.7  (SD_SCK)
 * (LCD_EN) 0.18 | · · | 0.16 (LCD_RS)           (BTN_EN1)   3.25 | · · | 0.28 (SD_CS2)
 * (LCD_D4) 0.15 | · ·| 0.17 (LCD_D5)            (BTN_EN2)   3.26 | · ·|  0.9 (SD_MOSI)
 * (LCD_D6)  1.0 | · · | 1.22 (LCD_D7)           (SD_DETECT) 0.27 | · · | RST
 *           GND | · · | 5V                                   GND | · · | NC
 *                -----                                            -----
 *                EXP1                                             EXP2
 */

   #if ENABLED(ZONESTAR_12864LCD)
   #define	LCDSCREEN_NAME		"ZONESTAR LCD12864"
#define LCD_PINS_RS 		P0_16 	//7 CS make sure for zonestar zm3e4!
#define LCD_PINS_ENABLE 	P0_15		//6 DATA make sure for zonestar zm3e4!
#define LCD_PINS_D4 		P0_18    //8 SCK make sure for zonestar zm3e4!
#define LCD_PINS_D5 		-1    	//mosi
#define LCD_PINS_D6 		-1      
#define LCD_PINS_D7 		-1
#define LCD_RESET_PIN       -1

#define BEEPER_PIN          P1_31
#define KILL_PIN          	-1
#define BTN_EN1 			P0_17
#define BTN_EN2 			P1_00
#define BTN_ENC 			P1_22
	
#define DOGLCD_A0           -1
#define DOGLCD_CS           -1
#define DOGLCD_MOSI         -1
#define DOGLCD_SCK          -1
#endif

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#define ONBOARD_SD_CS_PIN                  P0_06  // Chip select for "System" SD card

#if SD_CONNECTION_IS(LCD) || SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                    P0_27
  #define SCK_PIN                          P0_07
  #define MISO_PIN                         P0_08
  #define MOSI_PIN                         P0_09
  #if SD_CONNECTION_IS(ONBOARD)
    #define SS_PIN             ONBOARD_SD_CS_PIN
  #else
    #define SS_PIN                         P0_28
  #endif
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

//
// Other Pins
//
//#define PIN_P0_02                        P0_02  // AUX1 (Interrupt Capable/ADC/Serial Port 0)
//#define PIN_P0_03                        P0_03  // AUX1 (Interrupt Capable/ADC/Serial Port 0)
//#define PS_ON_PIN                        P1_23  // SERVO P1.23
