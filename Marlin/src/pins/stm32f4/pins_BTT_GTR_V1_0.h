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
#elif HOTENDS > 8 || E_STEPPERS > 8
  #error "BIGTREE GTR V1.0 supports up to 8 hotends / E-steppers."
#elif HOTENDS > MAX_EXTRUDERS || E_STEPPERS > MAX_EXTRUDERS
  #error "Marlin extruder/hotends limit! Increase MAX_EXTRUDERS to continue."
#endif

#define BOARD_INFO_NAME "BIGTREE GTR 1.0"

// Use one of these or SDCard-based Emulation will be used
#if NO_EEPROM_SELECTED
  //#define I2C_EEPROM
  //#define SRAM_EEPROM_EMULATION                 // Use BackSRAM-based EEPROM emulation
  //#define FLASH_EEPROM_EMULATION                // Use Flash-based EEPROM emulation
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#endif

#define TP                                        // Enable to define servo and probe pins

//
// Servos
//
#if ENABLED(TP)
  #define SERVO0_PIN                        PB11
#endif

#define PS_ON_PIN                           PH6

//
// Limit Switches
//
#define X_MIN_PIN                           PF2
#define X_MAX_PIN                           PG14
#define Y_MIN_PIN                           PC13
#define Y_MAX_PIN                           PG9
#define Z_MIN_PIN                           PE0
#define Z_MAX_PIN                           PD3

//
// Pins on the extender
//
//#define X_MIN_PIN                         PI4
//#define X2_MIN_PIN                        PF12
//#define Y_MIN_PIN                         PF4
//#define Y2_MIN_PIN                        PI7
//#define Z_MIN_PIN                         PF6

#if ENABLED(TP) && !defined(Z_MIN_PROBE_PIN)
  #define Z_MIN_PROBE_PIN                   PH11  // Z Probe must be PH11
#endif

//
// Steppers
//
#define X_STEP_PIN                          PC15
#define X_DIR_PIN                           PF0
#define X_ENABLE_PIN                        PF1
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC14
#endif

#define Y_STEP_PIN                          PE3
#define Y_DIR_PIN                           PE2
#define Y_ENABLE_PIN                        PE4
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PE1
#endif

#define Z_STEP_PIN                          PB8
#define Z_DIR_PIN                           PB7   // PB7
#define Z_ENABLE_PIN                        PB9
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PB5
#endif

#define E0_STEP_PIN                         PG12
#define E0_DIR_PIN                          PG11
#define E0_ENABLE_PIN                       PG13
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PG10
#endif

#define E1_STEP_PIN                         PD6
#define E1_DIR_PIN                          PD5
#define E1_ENABLE_PIN                       PD7
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PD4
#endif

#define E2_STEP_PIN                         PD1
#define E2_DIR_PIN                          PD0
#define E2_ENABLE_PIN                       PD2
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PC12
#endif

#define E3_STEP_PIN                         PF3
#define E3_DIR_PIN                          PG3
#define E3_ENABLE_PIN                       PF8
#ifndef E3_CS_PIN
  #define E3_CS_PIN                         PG4
#endif

#define E4_STEP_PIN                         PD14
#define E4_DIR_PIN                          PD11
#define E4_ENABLE_PIN                       PG2
#ifndef E4_CS_PIN
  #define E4_CS_PIN                         PE15
#endif

#define E5_STEP_PIN                         PE12
#define E5_DIR_PIN                          PE10
#define E5_ENABLE_PIN                       PF14
#ifndef E5_CS_PIN
  #define E5_CS_PIN                         PE7
#endif

#define E6_STEP_PIN                         PG0
#define E6_DIR_PIN                          PG1
#define E6_ENABLE_PIN                       PE8
#ifndef E6_CS_PIN
  #define E6_CS_PIN                         PF15
#endif

#define E7_STEP_PIN                         PH12
#define E7_DIR_PIN                          PH15
#define E7_ENABLE_PIN                       PI0
#ifndef E7_CS_PIN
  #define E7_CS_PIN                         PH14
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PG15
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PB6
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PB3
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial
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
  //#define E5_HARDWARE_SERIAL Serial1
  //#define E6_HARDWARE_SERIAL Serial1
  //#define E7_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                   PC14
  #define X_SERIAL_RX_PIN                   PC14

  #define Y_SERIAL_TX_PIN                   PE1
  #define Y_SERIAL_RX_PIN                   PE1

  #define Z_SERIAL_TX_PIN                   PB5
  #define Z_SERIAL_RX_PIN                   PB5

  #define E0_SERIAL_TX_PIN                  PG10
  #define E0_SERIAL_RX_PIN                  PG10

  #define E1_SERIAL_TX_PIN                  PD4
  #define E1_SERIAL_RX_PIN                  PD4

  #define E2_SERIAL_TX_PIN                  PC12
  #define E2_SERIAL_RX_PIN                  PC12

  #define E3_SERIAL_TX_PIN                  PG4
  #define E3_SERIAL_RX_PIN                  PG4

  #define E4_SERIAL_TX_PIN                  PE15
  #define E4_SERIAL_RX_PIN                  PE15

  #define E5_SERIAL_TX_PIN                  PE7
  #define E5_SERIAL_RX_PIN                  PE7

  #define E6_SERIAL_TX_PIN                  PF15
  #define E6_SERIAL_RX_PIN                  PF15

  #define E7_SERIAL_TX_PIN                  PH14
  #define E7_SERIAL_RX_PIN                  PH14

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // T1 <-> E0
#define TEMP_1_PIN                          PC2   // T2 <-> E1
#define TEMP_2_PIN                          PC3   // T3 <-> E2

#define TEMP_3_PIN                          PA3   // T4 <-> E3
#define TEMP_4_PIN                          PF9   // T5 <-> E4
#define TEMP_5_PIN                          PF10  // T6 <-> E5
#define TEMP_6_PIN                          PF7   // T7 <-> E6
#define TEMP_7_PIN                          PF5   // T8 <-> E7

#define TEMP_BED_PIN                        PC0   // T0 <-> Bed

// SPI for Max6675 or Max31855 Thermocouple
// Uses a separate SPI bus
// If you have a two-way thermocouple, you can customize two THERMO_CSx_PIN pins (x:1~2)

#define THERMO_SCK_PIN                      PI1   // SCK
#define THERMO_DO_PIN                       PI2   // MISO
#define THERMO_CS1_PIN                      PH9   // CS1
#define THERMO_CS2_PIN                      PH2   // CS2

#define MAX6675_SS_PIN            THERMO_CS1_PIN
#define MAX6675_SS2_PIN           THERMO_CS2_PIN
#define MAX6675_SCK_PIN           THERMO_SCK_PIN
#define MAX6675_DO_PIN             THERMO_DO_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // Heater0
#define HEATER_1_PIN                        PA1   // Heater1
#define HEATER_2_PIN                        PB0   // Heater2

#define HEATER_3_PIN                        PD15  // Heater3
#define HEATER_4_PIN                        PD13  // Heater4
#define HEATER_5_PIN                        PD12  // Heater5
#define HEATER_6_PIN                        PE13  // Heater6
#define HEATER_7_PIN                        PI6   // Heater7

#define HEATER_BED_PIN                      PA2   // Hotbed

#define FAN_PIN                             PE5   // Fan0
#define FAN1_PIN                            PE6   // Fan1
#define FAN2_PIN                            PC8   // Fan2

#define FAN3_PIN                            PI5   // Fan3
#define FAN4_PIN                            PE9   // Fan4
#define FAN5_PIN                            PE11  // Fan5
//#define FAN6_PIN                          PC9   // Fan6
//#define FAN7_PIN                          PE14  // Fan7

//
// By default the onboard SD (SPI1) is enabled
//
#define CUSTOM_SPI_PINS
#if DISABLED(CUSTOM_SPI_PINS)
  #define SDSS                              PB12
#endif

// HAL SPI1 pins group
#if ENABLED(CUSTOM_SPI_PINS)
  #define SDSS                              PA4
  #define SD_DETECT_PIN                     PC4
  #define LCD_SDSS                          PA4

  #define SCK_PIN                           PA5
  #define MISO_PIN                          PA6
  #define MOSI_PIN                          PA7
  #define SS_PIN                            PA4   // Chip select for SD card used by Marlin
#endif

/**
 *               _____                                             _____
 *           NC | · · | GND                                    5V | · · | GND
 *        RESET | · · | PB10(SD_DETECT)             (LCD_D7)  PG5 | · · | PG6  (LCD_D6)
 *   (MOSI)PB15 | · · | PH10(BTN_EN2)               (LCD_D5)  PG7 | · · | PG8  (LCD_D4)
 *  (SD_SS)PB12 | · · | PD10(BTN_EN1)               (LCD_RS)  PA8 | · · | PC10 (LCD_EN)
 *    (SCK)PB13 | · · | PB14(MISO)                 (BTN_ENC) PA15 | · · | PC11  (BEEPER)
 *               ￣￣                                               ￣￣
 *               EXP2                                              EXP1
 */

//
// LCDs and Controllers
//
#if HAS_SPI_LCD
  #define BEEPER_PIN                        PC11
  #define BTN_ENC                           PA15

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS                     PA8

    #define BTN_EN1                         PD10
    #define BTN_EN2                         PH10

    #define LCD_PINS_ENABLE                 PG7
    #define LCD_PINS_D4                     PG8

    //#undef ST7920_DELAY_1
    //#undef ST7920_DELAY_2
    //#undef ST7920_DELAY_3

  #else

    #define LCD_PINS_RS                     PA8

    #define BTN_EN1                         PD10
    #define BTN_EN2                         PH10

    #if DISABLED(CUSTOM_SPI_PINS)
      #define SD_DETECT_PIN                 PB10
      #define LCD_SDSS                      PB12
    #endif

    #define LCD_PINS_ENABLE                 PC10
    #define LCD_PINS_D4                     PG8

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS                     PC10
      #define DOGLCD_A0                     PA8
      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN                 PG8   // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN             PG7
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN             PG6
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN             PG5
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN                PF13
      #endif
    #endif // !FYSETC_MINI_12864

    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PG7
      #define LCD_PINS_D6                   PG6
      #define LCD_PINS_D7                   PG5
    #endif

  #endif

  // Alter timing for graphical display
  #if HAS_GRAPHICAL_LCD
    #define BOARD_ST7920_DELAY_1 DELAY_NS(96)
    #define BOARD_ST7920_DELAY_2 DELAY_NS(48)
    #define BOARD_ST7920_DELAY_3 DELAY_NS(600)
  #endif

  //#define DOGLCD_CS                       PB12
  //#define DOGLCD_A0                       PA8
  //#define LCD_PINS_DC                     PB14
  //#define DOGLCD_MOSI                     PB15

#endif // HAS_SPI_LCD

#undef TP
