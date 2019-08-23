/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef TARGET_STM32F4
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "BIGTREE SKR Pro V1.1 supports up to 3 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "BIGTREE Btt002 1.0"

#define SRAM_EEPROM_EMULATION

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Servos
//
#define SERVO0_PIN         PC3

//
// Limit Switches
//
#define X_MIN_PIN          PD3
#define X_MAX_PIN          PD3
#define Y_MIN_PIN          PD2
#define Y_MAX_PIN          PD2
#define Z_MIN_PIN          PD1
#define Z_MAX_PIN          PD1

//
// Z Probe must be this pins  ##
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  PD1
#endif

//
// Steppers
//
#define X_STEP_PIN         PA9
#define X_DIR_PIN          PA10
#define X_ENABLE_PIN       PA8
#ifndef X_CS_PIN
  #define X_CS_PIN         PE2
#endif

#define Y_STEP_PIN         PC8
#define Y_DIR_PIN          PC9
#define Y_ENABLE_PIN       PC7
 #ifndef Y_CS_PIN
  #define Y_CS_PIN         PE3
#endif

#define Z_STEP_PIN         PD15
#define Z_DIR_PIN          PC6
#define Z_ENABLE_PIN       PD14
#ifndef Z_CS_PIN
  #define Z_CS_PIN         PE4
#endif

#define E0_STEP_PIN        PD12
#define E0_DIR_PIN         PD13
#define E0_ENABLE_PIN      PD11
#ifndef E0_CS_PIN
  #define E0_CS_PIN        PD7
#endif

/*
//SKR_PRO_V1.1
#define E1_STEP_PIN        PD15
#define E1_DIR_PIN         PE7
#define E1_ENABLE_PIN      PA3
#ifndef E1_CS_PIN
  #define E1_CS_PIN        PG15
#endif

#define E2_STEP_PIN        PD13
#define E2_DIR_PIN         PG9
#define E2_ENABLE_PIN      PF0
#ifndef E2_CS_PIN
  #define E2_CS_PIN        PG12
#endif
*/
//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    PB15
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    PB14
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     PB13
  #endif
#endif

#if HAS_TMC220x
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL Serial1
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
  // Software serial  ##
  //
  #define X_SERIAL_TX_PIN  PE2
  #define X_SERIAL_RX_PIN  PE2

  #define Y_SERIAL_TX_PIN  PE3
  #define Y_SERIAL_RX_PIN  PE3

  #define Z_SERIAL_TX_PIN  PE4
  #define Z_SERIAL_RX_PIN  PE4

  #define E0_SERIAL_TX_PIN PD7
  #define E0_SERIAL_RX_PIN PD7

  //#define E1_SERIAL_TX_PIN PD1
  //#define E1_SERIAL_RX_PIN PD1

  //#define E2_SERIAL_TX_PIN PD6
  //#define E2_SERIAL_RX_PIN PD6
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA2   // T1 <-> E0
#define TEMP_1_PIN         PA0   // T2 <-> E1
//#define TEMP_2_PIN         PC2   // T3 <-> E2  SKR_PRO
#define TEMP_BED_PIN       PA1   // T0 <-> Bed

//
// Heaters / Fans
//
#define HEATER_0_PIN       PE6  // Heater0
//#define HEATER_1_PIN       PD14  // Heater1
//#define HEATER_2_PIN       PB0   // Heater1
#define HEATER_BED_PIN     PE5   // Hotbed
#define FAN_PIN            PB9   // Fan0
#define FAN1_PIN           PB8   // Fan1
//#define FAN2_PIN           PE6   // Fan2

// HAL SPI pins group
#define SCK_PIN            PA5   // SPI SCLK
#define MYSSEL             PA4   // SPI SSEL
#define MISO_PIN           PA6   // SPI MISO
#define MOSI_PIN           PA7   // SPI MOSI

//
// Misc. Functions
//
#define SDSS               PA4

/**
 * -------------------------------------SKR_MK3-----------------------------------------------
 *               _____                                             _____                      |
 *          PA3 | · · | GND                                    5V | · · | GND                 |
 *       NRESET | · · | PC4(SD_DET)                 (LCD_D7) PE13 | · · | PE12  (LCD_D6)      |
 *   (MOSI)PA7  | · · | PB0(BTN_EN2)                (LCD_D5) PE11 | · · | PE10  (LCD_D4)      |
 *  (SD_SS)PA4  | · · | PC5(BTN_EN1)                (LCD_RS) PE8  | · · | PE9   (LCD_EN)      |
 *    (SCK)PA5  | · · | PA6(MISO)                  (BTN_ENC) PB1  | · · | PE7   (BEEPER)      |
 *               ￣￣                                               ￣￣                       |
 *               EXP2                                              EXP1                       |
 * ---------------------------------------------------------------------------------------------
 */
//
// LCDs and Controllers
//
#if HAS_SPI_LCD
  #define BEEPER_PIN       PE7
  #define BTN_ENC          PB1

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PE12

    #define BTN_EN1        PE9
    #define BTN_EN2        PE10

    #define LCD_PINS_ENABLE PE13
    #define LCD_PINS_D4    PE11

  #else

    #define LCD_PINS_RS    PE8

    #define BTN_EN1        PC5
    #define BTN_EN2        PB0
    #define SD_DETECT_PIN  PC4

    #define LCD_SDSS       PA4

    #define LCD_PINS_ENABLE PE9
    #define LCD_PINS_D4    PE10

    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5  PE11
      #define LCD_PINS_D6  PE12
      #define LCD_PINS_D7  PE13
    #endif

  #endif

  // Alter timing for graphical display
  #if HAS_GRAPHICAL_LCD
    #ifndef ST7920_DELAY_1
      #define ST7920_DELAY_1 DELAY_NS(96)
    #endif
    #ifndef ST7920_DELAY_2
      #define ST7920_DELAY_2 DELAY_NS(48)
    #endif
    #ifndef ST7920_DELAY_3
      #define ST7920_DELAY_3 DELAY_NS(600)
    #endif
  #endif

#endif // HAS_SPI_LCD
