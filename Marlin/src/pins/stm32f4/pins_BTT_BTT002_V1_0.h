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

#ifndef TARGET_STM32F4
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "BIGTREE BTT002 V1.0 supports up to 1 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "BIGTREE Btt002 1.0"

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
#define FLASH_EEPROM_EMULATION                    // Use Flash-based EEPROM emulation

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Limit Switches
//
#define X_STOP_PIN                          PD3
#define Y_STOP_PIN                          PD2
#define Z_STOP_PIN                          PD1   // Shares J4 connector with PC3

//
// Z Probe must be this pin
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PD1
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA15
#endif

//
// Power Loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PD4
#endif

//
// Steppers
//
#define X_STEP_PIN                          PA9
#define X_DIR_PIN                           PA10
#define X_ENABLE_PIN                        PA8
#ifndef X_CS_PIN
  #define X_CS_PIN                          PE2
#endif

#define Y_STEP_PIN                          PC8
#define Y_DIR_PIN                           PC9
#define Y_ENABLE_PIN                        PC7
 #ifndef Y_CS_PIN
  #define Y_CS_PIN                          PE3
#endif

#define Z_STEP_PIN                          PD15
#define Z_DIR_PIN                           PC6
#define Z_ENABLE_PIN                        PD14
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PE4
#endif

#define E0_STEP_PIN                         PD12
#define E0_DIR_PIN                          PD13
#define E0_ENABLE_PIN                       PD11
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PD7
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PB15
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PB14
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PB13
  #endif
#endif

#if HAS_TMC_UART
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
  #define X_SERIAL_TX_PIN                   PE2
  #define X_SERIAL_RX_PIN                   PE2

  #define Y_SERIAL_TX_PIN                   PE3
  #define Y_SERIAL_RX_PIN                   PE3

  #define Z_SERIAL_TX_PIN                   PE4
  #define Z_SERIAL_RX_PIN                   PE4

  #define E0_SERIAL_TX_PIN                  PD7
  #define E0_SERIAL_RX_PIN                  PD7

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA2   // T0 <-> E0
#define TEMP_1_PIN                          PA0   // T1 <-> E1
#define TEMP_BED_PIN                        PA1   // T2 <-> Bed
#define TEMP_PROBE_PIN                      PC3   // Shares J4 connector with PD1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PE6   // Heater0
#define HEATER_BED_PIN                      PE5   // Hotbed
#define FAN_PIN                             PB8   // Fan1
#define FAN1_PIN                            PB9   // Fan0

// HAL SPI1 pins
#define CUSTOM_SPI_PINS
#if ENABLED(CUSTOM_SPI_PINS)
  #define SCK_PIN                           PA5   // SPI1 SCLK
  #define SS_PIN                            PA4   // SPI1 SSEL
  #define MISO_PIN                          PA6   // SPI1 MISO
  #define MOSI_PIN                          PA7   // SPI1 MOSI
#endif

//
// Misc. Functions
//
#define SDSS                                PA4

/**
 * -------------------------------------BTT002 V1.0-----------------------------------------------
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
  #define BEEPER_PIN                        PE7
  #define BTN_ENC                           PB1

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS                     PE12

    #define BTN_EN1                         PE9
    #define BTN_EN2                         PE10

    #define LCD_PINS_ENABLE                 PE13
    #define LCD_PINS_D4                     PE11

  #else

    #define LCD_PINS_RS                     PE8

    #define BTN_EN1                         PC5
    #define BTN_EN2                         PB0
    #define SD_DETECT_PIN                   PC4

    #define LCD_SDSS                        PA4

    #define LCD_PINS_ENABLE                 PE9
    #define LCD_PINS_D4                     PE10

    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PE11
      #define LCD_PINS_D6                   PE12
      #define LCD_PINS_D7                   PE13
    #endif

  #endif

  // Alter timing for graphical display
  #if HAS_GRAPHICAL_LCD
    #define BOARD_ST7920_DELAY_1 DELAY_NS(96)
    #define BOARD_ST7920_DELAY_2 DELAY_NS(48)
    #define BOARD_ST7920_DELAY_3 DELAY_NS(600)
  #endif

#endif // HAS_SPI_LCD

//
// RGB LEDs
//
#ifndef RGB_LED_R_PIN
  #define RGB_LED_R_PIN                     PB5
#endif
#ifndef RGB_LED_G_PIN
  #define RGB_LED_G_PIN                     PB4
#endif
#ifndef RGB_LED_B_PIN
  #define RGB_LED_B_PIN                     PB3
#endif
#ifndef RGB_LED_W_PIN
  #define RGB_LED_W_PIN                     -1
#endif
