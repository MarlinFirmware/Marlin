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
 * Mightyboard Rev.G and H pin assignments
 * Schematic: not avalable (as rev G and H are not open source)
 * Pins based on the work of https://github.com/Sgail7/Replicator-Revival-Project/
 * Pin number according to Mega extended mega2560ext: .\buildroot\share\PlatformIO\variants\MARLIN_MEGA_EXTENDED\pins_arduino.h
 * Use env:MightyBoard2560 or env:MightyBoard1280 in platformio.ini
 */


#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME      "Mightyboard rev.G/H"
#define DEFAULT_MACHINE_NAME "MB Replicator"

//
// Servos (unconfirmed)
//
// #define SERVO0_PIN                            36  // C1 (1280-EX1)
// #define SERVO1_PIN                            37  // C0 (1280-EX2)
// #define SERVO2_PIN                            40  // G1 (1280-EX3)
// #define SERVO3_PIN                            41  // G0 (1280-EX4)

//
// Limit Switches: X and Y go to max, Z to min 
// 
// #define X_MIN_PIN                             30  // C7
#define X_MAX_PIN                             30  // C7
// #define Y_MIN_PIN                             31  // C6
#define Y_MAX_PIN                             31  // C6
#define Z_MIN_PIN                             32  // C5
// #define Z_MAX_PIN                             42  // L7

//
// Z Probe (when not Z_MIN_PIN)
//
// #ifndef Z_MIN_PROBE_PIN
//   #define Z_MIN_PROBE_PIN                     42
// #endif

//
// Filament Runout Sensor
//
// #ifndef FIL_RUNOUT_PIN
//   #define FIL_RUNOUT_PIN                      49
// #endif
// #ifndef FIL_RUNOUT2_PIN
//   #define FIL_RUNOUT2_PIN                     47
// #endif

//
// Steppers
//
#define X_STEP_PIN                            83  // D6
#define X_DIR_PIN                             38  // D7
#define X_ENABLE_PIN                          81  // D4

#define Y_STEP_PIN                            44  // L5
#define Y_DIR_PIN                             42  // L7
#define Y_ENABLE_PIN                          45  // L4 

#define Z_STEP_PIN                            48  // L1
#define Z_DIR_PIN                             47  // L2
#define Z_ENABLE_PIN                          49  // L0

#define E0_STEP_PIN                           25  // A3
#define E0_DIR_PIN                            24  // A2
#define E0_ENABLE_PIN                         27  // A5

#define E1_STEP_PIN                           22  // A0
#define E1_DIR_PIN                            15  // K7
#define E1_ENABLE_PIN                         23  // A1

//
// I2C Digipots - MCP4018
// Address 5E (2F << 1)
// Set from 0 - 127 with stop bit.
// (Ex. 3F << 1 | 1)
//
#define DIGIPOTS_I2C_SCL                      28  // A6
#define DIGIPOTS_I2C_SDA_X                    82  // D5
#define DIGIPOTS_I2C_SDA_Y                    43  // L6
#define DIGIPOTS_I2C_SDA_Z                    46  // L3
#define DIGIPOTS_I2C_SDA_E0                   26  // A4
#define DIGIPOTS_I2C_SDA_E1                   77  // J7

#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A             0x2F  // unshifted slave address (5E <- 2F << 1)
#endif
#define DIGIPOT_ENABLE_I2C_PULLUPS                // MightyBoard doesn't have hardware I2C pin pull-ups.


// Bed temp sensor pin 
#define TEMP_BED_PIN                           3  // F3

/* 
*  Temperature Sensors
*  Uses ADS1118 as ADC converter to read the hotend temperature sensors
*  SPI for ADS1118 ADC, Uses software SPI
*
*/ 
#define TEMP_0_CS_PIN                         79  // E6
#define TEMP_0_SCK_PIN                        78  // E2
#define TEMP_0_MISO_PIN                       80  // E7
#define TEMP_0_MOSI_PIN                       84  // H2

#define TEMP_1_CS_PIN                          2  // E4
#define TEMP_1_SCK_PIN            TEMP_0_SCK_PIN
#define TEMP_1_MISO_PIN          TEMP_0_MISO_PIN
#define TEMP_1_MOSI_PIN          TEMP_0_MOSI_PIN

//
// FET Pin Mapping - FET A is closest to the input power connector
//

#define MOSFET_A_PIN                           3  // E5
#define MOSFET_B_PIN                           5  // E3
#define MOSFET_C_PIN                           8  // H5
#define MOSFET_D_PIN                           7  // H4
#define MOSFET_E_PIN                           2  // E4
#define MOSFET_F_PIN                           4  // G5

//
// Heaters / Fans (24V)
//

#define HEATER_0_PIN                MOSFET_A_PIN  // E5
#define HEATER_1_PIN                MOSFET_B_PIN  // E3 ?* 
#define HEATER_BED_PIN              MOSFET_C_PIN  // H5

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN           MOSFET_D_PIN
#elif !defined(FAN0_PIN)
  #define FAN0_PIN                  MOSFET_D_PIN
#endif

#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN           MOSFET_E_PIN
#elif !defined(FAN1_PIN)
  #define FAN1_PIN                  MOSFET_E_PIN
#endif

//
// Misc. Functions
// PCA9632 LED


//#define LED_PIN                               13  // B7
//#define CUTOFF_RESET_PIN                      16  // H1
//#define CUTOFF_TEST_PIN                       17  // H0
//#define CUTOFF_SR_CHECK_PIN                   70  // G4 (TOSC1)

//
// LCD / Controller
//

#if HAS_WIRED_LCD

  #if IS_RRD_FG_SC

    #define LCD_PINS_RS                       33  // C4: LCD-STROBE
    #define LCD_PINS_EN                       72  // J2: LEFT
    #define LCD_PINS_D4                       35  // C2: LCD-CLK
    #define LCD_PINS_D5                       32  // C5: RLED
    #define LCD_PINS_D6                       34  // C3: LCD-DATA
    #define LCD_PINS_D7                       31  // C6: GLED

    #define BTN_EN2                           75  // J4, UP
    #define BTN_EN1                           73  // J3, DOWN
    // STOP button connected as KILL_PIN
    #define KILL_PIN                          14  // J1, RIGHT (not connected)

    #define BEEPER_PIN                         8  // H5, SD_WP

    // Onboard leds
    #define STAT_LED_RED_PIN          SERVO0_PIN  // C1 (1280-EX1, DEBUG2)
    #define STAT_LED_BLUE_PIN         SERVO1_PIN  // C0 (1280-EX2, DEBUG3)

  #else
    // Replicator 2 and 2X uses a HD44780 SPI display, pins: mosi, sclk, miso (not used),  missing: latch, click, power
    #define SPI_SOFT_MOSI_PIN                 37  // C0
    #define SPI_SOFT_SCLK_PIN                 36  // C1
    #define SPI_SOFT_MISO_PIN                 33  // C4

    #define BTN_LATCH_PIN                     34  // C3
    #define BTN_CLICK_PIN                     39  // G2

    #define BTN_UP_PIN                        76  // J5
    #define BTN_DOWN_PIN                      75  // J4
    #define BTN_LEFT_PIN                      77  // J6/J13

    #define LCD_POWER_PIN                     29  // A7  

    #define LCD_LED1_PIN                      35  // C2

    // #define BTN_RIGHT_PIN                     14  // J1 ?    
        
    // Leave Old pins defined for now 
    #define SR_DATA_PIN                       37  // C0 *
    #define SR_CLK_PIN                        36  // C1 *
    #define SR_STROBE_PIN                     33  // C4 *

    // #define BTN_UP                            75  // J5
    // #define BTN_DOWN                          73  // J4
    // #define BTN_LEFT                          72  // J6/J13
    // #define BTN_RIGHT                         14  // J1
    
    // Disable encoder
    #undef BTN_EN1
    #undef BTN_EN2

    #define BEEPER_PIN                            6  // H3

    // #define STAT_LED_RED_PIN                  32  // C2 *
    // #define STAT_LED_BLUE_PIN                 31  // C6 (Actually green)

  #endif

  // #define BTN_CENTER                          15  // J6/J13 *
  // #define BTN_ENC                     BTN_CENTER

#endif // HAS_WIRED_LCD

//
// SD Card
// Not sure about these 
// #define SD_SS_PIN                             53  // B0
// #define SD_DETECT_PIN                          9  // H6

// #if HAS_TMC_UART
//   /**
//    * TMC220x stepper drivers
//    *
//    * Hardware serial communication ports.
//    * If undefined software serial is used according to the pins below
//    */
//   #define X_HARDWARE_SERIAL Serial2
//   #define Y_HARDWARE_SERIAL Serial1

//   /**
//    * Software serial
//    */

//   #define X_SERIAL_TX_PIN                     16
//   #define X_SERIAL_RX_PIN                     17

//   #define Y_SERIAL_TX_PIN                     18
//   #define Y_SERIAL_RX_PIN                     19

//   #define Z_SERIAL_TX_PIN                     41
//   #define Z_SERIAL_RX_PIN                     66

//   #define E0_SERIAL_TX_PIN                    40
//   #define E0_SERIAL_RX_PIN                    67

//   #define E1_SERIAL_TX_PIN                    37
//   #define E1_SERIAL_RX_PIN                    68

// #endif
