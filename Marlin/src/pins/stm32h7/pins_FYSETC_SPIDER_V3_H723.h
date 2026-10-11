/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * FYSETC Spider V3.0 H7 (STM32H723VGT6, 25MHz crystal)
 * Schematic, pinout and Klipper config: https://github.com/FYSETC/FYSETC-SPIDER-H7
 *
 * Same form factor and largely the same pinout as the F446 Spider, except:
 *  - One heater MOSFET removed (PB15 is no longer a heater output).
 *  - Two hotend heaters, labeled HEAT1 (PC8) and HEAT2 (PB3).
 *  - Clocked DotStar/NeoPixel header (DATA PD3, CLK PB11).
 */

#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 5
  #error "FYSETC Spider V3 H7 supports up to 2 hotends / 5 E steppers."
#endif

#define BOARD_INFO_NAME      "FYSETC Spider V3 H7"
#define BOARD_WEBSITE_URL    "github.com/FYSETC/FYSETC-SPIDER-H7"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define I2C_EEPROM
  //#define FLASH_EEPROM_EMULATION
#endif

#if ENABLED(I2C_EEPROM)
  // Onboard AT24C32
  #define SOFT_I2C_EEPROM                         // Force the use of Software I2C
  #define I2C_SCL_PIN                       PB8
  #define I2C_SDA_PIN                       PB9
  #define MARLIN_EEPROM_SIZE             0x1000U  // 4K
#elif ENABLED(FLASH_EEPROM_EMULATION)
  #define EEPROM_PAGE_SIZE                0x800U  // 2K
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2K
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                        PA2   // BLTouch header (shared with Y-MAX)
#endif

//
// Limit Switches
//
// Driver DIAG jumpers connect:
//   M0 (X)  -> X-MIN    M3 (E0) -> Z-MAX
//   M1 (Y)  -> Y-MIN    M4 (E1) -> Y-MAX
//   M2 (Z)  -> Z-MIN    M5 (E2) -> X-MAX
//
#define X_MIN_PIN                           PB14
#define X_MAX_PIN                           PA1
#define Y_MIN_PIN                           PB13
#define Y_MAX_PIN                           PA2
#define Z_MIN_PIN                           PA0   // Also on the BLTouch header
#define Z_MAX_PIN                           PA3

//
// Z Probe (when not Z_MIN_PIN)
// Z-MAX connector with selectable probe voltage (V_PROBE)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA3
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA1   // X-MAX
#endif

//
// Steppers
//
#define X_STEP_PIN                          PE11  // M0
#define X_DIR_PIN                           PE10
#define X_ENABLE_PIN                        PE9
#define X_CS_PIN                            PE7

#define Y_STEP_PIN                          PD8   // M1
#define Y_DIR_PIN                           PB12
#define Y_ENABLE_PIN                        PD9
#define Y_CS_PIN                            PE15

#define Z_STEP_PIN                          PD14  // M2
#define Z_DIR_PIN                           PD13
#define Z_ENABLE_PIN                        PD15
#define Z_CS_PIN                            PD10

#define E0_STEP_PIN                         PD5   // M3
#define E0_DIR_PIN                          PD6
#define E0_ENABLE_PIN                       PD4
#define E0_CS_PIN                           PD7

#define E1_STEP_PIN                         PE6   // M4
#define E1_DIR_PIN                          PC13
#define E1_ENABLE_PIN                       PE5
#define E1_CS_PIN                           PC14

#define E2_STEP_PIN                         PE2   // M5
#define E2_DIR_PIN                          PE4
#define E2_ENABLE_PIN                       PE3
#define E2_CS_PIN                           PC15

#define E3_STEP_PIN                         PD12  // M6
#define E3_DIR_PIN                          PC4
#define E3_ENABLE_PIN                       PE8
#define E3_CS_PIN                           PA15

#define E4_STEP_PIN                         PE1   // M7
#define E4_DIR_PIN                          PE0
#define E4_ENABLE_PIN                       PC5
#define E4_CS_PIN                           PD11

//
// SPI pins for TMC2130/5160 stepper drivers
//
#ifndef TMC_USE_SW_SPI
  #define TMC_USE_SW_SPI
#endif
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SPI_MOSI
    #define TMC_SPI_MOSI                    PE14
  #endif
  #ifndef TMC_SPI_MISO
    #define TMC_SPI_MISO                    PE13
  #endif
  #ifndef TMC_SPI_SCK
    #define TMC_SPI_SCK                     PE12
  #endif
#endif

#if HAS_TMC_UART
  //
  // TMC2208/TMC2209 stepper drivers (single-wire UART on the CS pin)
  //
  #define X_SERIAL_TX_PIN                   PE7
  #define Y_SERIAL_TX_PIN                   PE15
  #define Z_SERIAL_TX_PIN                   PD10
  #define E0_SERIAL_TX_PIN                  PD7
  #define E1_SERIAL_TX_PIN                  PC14
  #define E2_SERIAL_TX_PIN                  PC15
  #define E3_SERIAL_TX_PIN                  PA15
  #define E4_SERIAL_TX_PIN                  PD11

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0   // T0
#define TEMP_1_PIN                          PC1   // T1
#define TEMP_2_PIN                          PC2   // T2
#define TEMP_3_PIN                          PC3   // T3
#define TEMP_4_PIN                          PB1   // T4
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN                      PB0   // TB
#endif

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      PC8   // HEAT1
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN                      PB3   // HEAT2
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    PB4   // BED-OUT
#endif

// FAN0-FAN2 are not on timer channels
#define FAN_SOFT_PWM_REQUIRED

#ifndef FAN0_PIN
  #define FAN0_PIN                          PA13  // SWDIO
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                          PA14  // SWCLK
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN                          PB2
#endif

// FAN3-FAN5 double as the 12/24V RGB LED strip header (G/R/B)
#if ANY(RGB_LED, RGBW_LED)
  #ifndef RGB_LED_R_PIN
    #define RGB_LED_R_PIN                   PB6   // FAN4
  #endif
  #ifndef RGB_LED_G_PIN
    #define RGB_LED_G_PIN                   PB5   // FAN3
  #endif
  #ifndef RGB_LED_B_PIN
    #define RGB_LED_B_PIN                   PB7   // FAN5
  #endif
#else
  #ifndef FAN3_PIN
    #define FAN3_PIN                        PB5
  #endif
  #ifndef FAN4_PIN
    #define FAN4_PIN                        PB6
  #endif
  #ifndef FAN5_PIN
    #define FAN5_PIN                        PB7
  #endif
#endif

/**
 *         ------                 ------
 *   PC9  | 1  2 | PA8      PA6  | 1  2 | PA5
 *   PC11 | 3  4 | PD2      PC6  | 3  4 | PA4
 *   PC10   5  6 | PC12     PC7    5  6 | PA7
 *   PD0  | 7  8 | PD1      PB10 | 7  8 | RESET
 *    GND | 9 10 | 5V        GND | 9 10 | 5V
 *         ------                 ------
 *          EXP1                   EXP2
 *
 * EXP1-7 (PD0) and EXP1-8 (PD1) are shared with the onboard CAN transceiver (CAN_RX / CAN_TX).
 * The transceiver drives PD0, so displays that use EXP1-7 as an output may not work correctly.
 */
#define EXP1_01_PIN                         PC9   // BEEP
#define EXP1_02_PIN                         PA8   // BTN_ENC
#define EXP1_03_PIN                         PC11  // LCD_EN
#define EXP1_04_PIN                         PD2   // LCD_RS
#define EXP1_05_PIN                         PC10  // LCD_D4
#define EXP1_06_PIN                         PC12  // LCD_D5
#define EXP1_07_PIN                         PD0   // LCD_D6 / CAN_RX
#define EXP1_08_PIN                         PD1   // LCD_D7 / CAN_TX

#define EXP2_01_PIN                         PA6   // MISO
#define EXP2_02_PIN                         PA5   // SCK
#define EXP2_03_PIN                         PC6   // BTN_EN1
#define EXP2_04_PIN                         PA4   // SS
#define EXP2_05_PIN                         PC7   // BTN_EN2
#define EXP2_06_PIN                         PA7   // MOSI
#define EXP2_07_PIN                         PB10  // CD
#define EXP2_08_PIN                         -1    // RESET

//
// SPI / SD Card (onboard TF card shares SPI1 with EXP2)
//
#define SD_SCK_PIN                   EXP2_02_PIN
#define SD_MISO_PIN                  EXP2_01_PIN
#define SD_MOSI_PIN                  EXP2_06_PIN

#define SD_SS_PIN                    EXP2_04_PIN
#define SD_DETECT_PIN                EXP2_07_PIN

//
// LCD / Controller
//
#if ENABLED(FYSETC_242_OLED_12864)

  #define BTN_EN1                    EXP1_01_PIN
  #define BTN_EN2                    EXP1_08_PIN
  #define BTN_ENC                    EXP1_02_PIN

  #define BEEPER_PIN                 EXP2_03_PIN

  #define LCD_PINS_DC                EXP1_06_PIN
  #define LCD_PINS_RS                EXP2_05_PIN  // LCD_RST
  #define DOGLCD_CS                  EXP1_04_PIN
  #define DOGLCD_MOSI                EXP1_05_PIN
  #define DOGLCD_SCK                 EXP1_03_PIN
  #define DOGLCD_A0                  LCD_PINS_DC
  #define FORCE_SOFT_SPI

  #define KILL_PIN                          -1    // NC
  #define NEOPIXEL_PIN               EXP1_07_PIN

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_01_PIN
  #define BTN_ENC                    EXP1_02_PIN

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP1_03_PIN
    #define BTN_EN2                  EXP1_05_PIN

    #define LCD_PINS_EN              EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

  #else

    #define LCD_PINS_RS              EXP1_04_PIN

    #define BTN_EN1                  EXP2_03_PIN
    #define BTN_EN2                  EXP2_05_PIN

    #define LCD_SDSS_PIN             EXP2_04_PIN

    #define LCD_PINS_EN              EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

    #if ENABLED(FYSETC_MINI_12864)
      // See https://wiki.fysetc.com/Mini12864_Panel
      #define DOGLCD_CS              EXP1_03_PIN
      #define DOGLCD_A0              EXP1_04_PIN
      #if ENABLED(FYSETC_GENERIC_12864_1_1)
        #define LCD_BACKLIGHT_PIN    EXP1_07_PIN
      #endif
      #define LCD_RESET_PIN          EXP1_05_PIN  // Must be high or open for LCD to operate normally.
      #if ANY(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN      EXP1_06_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN      EXP1_07_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN      EXP1_08_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         EXP1_06_PIN
      #endif
    #endif

    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_06_PIN
      #define LCD_PINS_D6            EXP1_07_PIN
      #define LCD_PINS_D7            EXP1_08_PIN
      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif
    #endif

  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1               96
  #define BOARD_ST7920_DELAY_2               48
  #define BOARD_ST7920_DELAY_3              640
#endif

//
// NeoPixel LED (5V RGB header, level-shifted DATA PD3 / CLK PB11)
// Some FYSETC displays use an EXP pin for NeoPixels, so use the onboard header as the 2nd strip.
//
#if ANY(FYSETC_242_OLED_12864, FYSETC_MINI_12864_2_1)
  #ifndef NEOPIXEL2_PIN
    #define NEOPIXEL2_PIN                   PD3
  #endif
#elif !defined(NEOPIXEL_PIN)
  #define NEOPIXEL_PIN                      PD3
#endif
