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
 * Mightyboard Rev.E pin assignments
 * also works for Rev D boards. It's all rev E despite what the silk screen says
 */

/**
 * Rev B  2 JAN 2017
 *
 *  Added pin definitions for:
 *    M3, M4 & M5 spindle control commands
 *    case light
 *
 *  Corrected pin assignment for EX2_HEAT_PIN pin. Changed it from 9 to 11.  The port
 *  number (B5) agrees with the schematic but B5 is assigned to logical pin 11.
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME      "Mightyboard"
#define DEFAULT_MACHINE_NAME "MB Replicator"

#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Servos
//
#define SERVO0_PIN                            PinC1  // C1 (1280-EX1)
#define SERVO1_PIN                            PinC0  // C0 (1280-EX2)
#define SERVO2_PIN                            PinG1  // G1 (1280-EX3)
#define SERVO3_PIN                            PinG0  // G0 (1280-EX4)

//
// Limit Switches
//
#define X_MIN_PIN                             PinL0  // L0
#define X_MAX_PIN                             PinL1  // L1
#define Y_MIN_PIN                             PinL2  // L2
#define Y_MAX_PIN                             PinL3  // L3
#define Z_MIN_PIN                             PinL6  // L6
#define Z_MAX_PIN                             PinL7  // L7

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinL7
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinL0
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                     PinL2
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinF1  // F1
#define X_DIR_PIN                             PinF0  // F0
#define X_ENABLE_PIN                          PinF2  // F2

#define Y_STEP_PIN                            PinF5  // F5
#define Y_DIR_PIN                             PinF4  // F4
#define Y_ENABLE_PIN                          PinF6  // F6

#define Z_STEP_PIN                            PinK1  // K1
#define Z_DIR_PIN                             PinK0  // K0
#define Z_ENABLE_PIN                          PinK2  // K2

#define E0_STEP_PIN                           PinA3  // A3
#define E0_DIR_PIN                            PinA2  // A2
#define E0_ENABLE_PIN                         PinA4  // A4

#define E1_STEP_PIN                           PinA7  // A7
#define E1_DIR_PIN                            PinA6  // A6
#define E1_ENABLE_PIN                         PinG2  // G2

//
// I2C Digipots - MCP4018
// Address 5E (2F << 1)
// Set from 0 - 127 with stop bit.
// (Ex. 3F << 1 | 1)
//
#define DIGIPOTS_I2C_SCL                      PinJ5  // J5
#define DIGIPOTS_I2C_SDA_X                    PinF3  // F3
#define DIGIPOTS_I2C_SDA_Y                    PinF7  // F7
#define DIGIPOTS_I2C_SDA_Z                    PinK3  // K3
#define DIGIPOTS_I2C_SDA_E0                   PinA5  // A5
#define DIGIPOTS_I2C_SDA_E1                   PinJ6  // J6

#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A             0x2F  // unshifted slave address (5E <- 2F << 1)
#endif
#define DIGIPOT_ENABLE_I2C_PULLUPS                // MightyBoard doesn't have hardware I2C pin pull-ups.

//
// Temperature Sensors
//
// K7 - 69 / ADC15 - 15
#define TEMP_BED_PIN                          PinJ0

// SPI for MAX Thermocouple
// Uses a separate SPI bus
//
//  3 E5 DO (SO)
//  5 E3 CS1
//  2 E4 CS2
// 78 E2 SCK
//
#define TEMP_0_CS_PIN                         PinE3  // E3
#define TEMP_0_SCK_PIN                        PinE2  // E2
#define TEMP_0_MISO_PIN                       PinE5  // E5
//#define TEMP_0_MOSI_PIN                    ...  // For MAX31865

#define TEMP_1_CS_PIN                         PinE4  // E4
#define TEMP_1_SCK_PIN            TEMP_0_SCK_PIN
#define TEMP_1_MISO_PIN          TEMP_0_MISO_PIN
//#define TEMP_1_MOSI_PIN        TEMP_0_MOSI_PIN

//
// FET Pin Mapping - FET 1 is closest to the input power connector
//

#define MOSFET_1_PIN                          PinH3  // Plug EX1 Pin 1-2 -> PH3 #15 -> Logical 06
#define MOSFET_2_PIN                          PinH4  // Plug EX1 Pin 3-4 -> PH4 #16 -> Logical 07
#define MOSFET_3_PIN                          PinB5  // Plug EX2 1-2 -> PB5 #24 -> Logical 11
#define MOSFET_4_PIN                          PinB6  // Plug EX2 3-4 -> PB6 #25 -> Logical 12
#define MOSFET_5_PIN                          PinL4  // Plug HBD 1-2 -> PL4 #39 -> Logical 45
#define MOSFET_6_PIN                          PinL5  // Plug Extra 1-2 -> PL5 #40 -> Logical 44 (FET not soldered in all boards)

//
// Heaters / Fans (24V)
//

#define HEATER_0_PIN                MOSFET_1_PIN // EX1
#define HEATER_1_PIN                MOSFET_3_PIN // EX2
#define HEATER_BED_PIN              MOSFET_5_PIN // HBP

// EX1 FAN (Automatic Fans are disabled by default in Configuration_adv.h - comment that out for auto fans)
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN           MOSFET_2_PIN
#else
  #ifndef FAN_PIN
    #define FAN_PIN                 MOSFET_2_PIN
  #endif
#endif
// EX2 FAN (Automatic Fans are disabled by default in Configuration_adv.h - comment that out for auto fans)
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN           MOSFET_4_PIN
#else
  #ifndef FAN1_PIN
    #define FAN1_PIN                MOSFET_4_PIN
  #endif
#endif

//
// Misc. Functions
//
#define LED_PIN                               PinB7  // B7
#define CUTOFF_RESET_PIN                      PinH1  // H1
#define CUTOFF_TEST_PIN                       PinH0  // H0
#define CUTOFF_SR_CHECK_PIN                   PinG4  // G4 (TOSC1)

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #if IS_RRD_FG_SC

    #define LCD_PINS_RS                       PinC4  // C4: LCD-STROBE
    #define LCD_PINS_ENABLE                   PinJ2  // J2: LEFT
    #define LCD_PINS_D4                       PinC2  // C2: LCD-CLK
    #define LCD_PINS_D5                       PinC5  // C5: RLED
    #define LCD_PINS_D6                       PinC3  // C3: LCD-DATA
    #define LCD_PINS_D7                       PinC6  // C6: GLED

    #define BTN_EN2                           PinJ4  // J4, UP
    #define BTN_EN1                           PinJ3  // J3, DOWN
    // STOP button connected as KILL_PIN
    #define KILL_PIN                          PinJ1  // J1, RIGHT (not connected)

    #ifndef FORCE_ONBOARD_BEEPER
      #define BEEPER_PIN                      PinH5  // H5, SD_WP
    #endif

    // Onboard leds
    #define STAT_LED_RED_PIN          SERVO0_PIN  // C1 (1280-EX1, DEBUG2)
    #define STAT_LED_BLUE_PIN         SERVO1_PIN  // C0 (1280-EX2, DEBUG3)

  #else
    // Replicator uses a 3-wire SR controller with HD44780
    #define SR_DATA_PIN                       PinC3  // C3
    #define SR_CLK_PIN                        PinC2  // C2
    #define SR_STROBE_PIN                     PinC4  // C4

    #define BTN_UP                            PinJ4  // J4
    #define BTN_DOWN                          PinJ3  // J3
    #define BTN_LEFT                          PinJ2  // J2
    #define BTN_RIGHT                         PinJ1  // J1

    // Disable encoder
    #undef BTN_EN1
    #undef BTN_EN2

    #define STAT_LED_RED_PIN                  PinC5  // C5
    #define STAT_LED_BLUE_PIN                 PinC6  // C6 (Actually green)

  #endif

  #define BTN_CENTER                          PinJ0  // J0
  #define BTN_ENC                     BTN_CENTER

#endif // HAS_WIRED_LCD

#ifndef BEEPER_PIN
  #define BEEPER_PIN                         PinG5  // G5
#endif

//
// SD Card
//
#define SDSS                                  PinB0  // B0
#define SD_DETECT_PIN                         PinH6  // H6

#if HAS_TMC_UART
  /**
   * TMC220x stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  #define X_HARDWARE_SERIAL Serial2
  #define Y_HARDWARE_SERIAL Serial1

  /**
   * Software serial
   */

  #define X_SERIAL_TX_PIN                     PinH1
  #define X_SERIAL_RX_PIN                     PinH0

  #define Y_SERIAL_TX_PIN                     PinD3
  #define Y_SERIAL_RX_PIN                     PinD2

  #define Z_SERIAL_TX_PIN                     PinG0
  #define Z_SERIAL_RX_PIN                     PinK4

  #define E0_SERIAL_TX_PIN                    PinG1
  #define E0_SERIAL_RX_PIN                    PinK5

  #define E1_SERIAL_TX_PIN                    PinC0
  #define E1_SERIAL_RX_PIN                    PinK6

#endif
