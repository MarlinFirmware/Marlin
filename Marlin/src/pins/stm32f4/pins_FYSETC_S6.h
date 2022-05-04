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

#include "env_validate.h"

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "FYSETC S6 supports up to 3 hotends / E-steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "FYSETC S6"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

// Avoid conflict with TIMER_TONE defined in variant
#define STEP_TIMER 10

//
// EEPROM Emulation
//
#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION
  //#define I2C_EEPROM
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#elif ENABLED(I2C_EEPROM)
  #define MARLIN_EEPROM_SIZE              0x1000  // 4K
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                        PA3
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PB14
#define X_MAX_PIN                           PA1
#define Y_MIN_PIN                           PB13
#define Y_MAX_PIN                           PA2
#define Z_MIN_PIN                           PA0
#define Z_MAX_PIN                           PA3

//
// Filament Sensor
// share with X_MAX_PIN
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA1
#endif

//
// Steppers
//
#define X_STEP_PIN                          PE11
#define X_DIR_PIN                           PE10
#ifndef X_ENABLE_PIN
  #define X_ENABLE_PIN                      PE12
#endif
#define X_CS_PIN                            PE7

#define Y_STEP_PIN                          PD8
#define Y_DIR_PIN                           PB12
#define Y_ENABLE_PIN                        PD9
#define Y_CS_PIN                            PE15

#define Z_STEP_PIN                          PD14
#define Z_DIR_PIN                           PD13
#define Z_ENABLE_PIN                        PD15
#define Z_CS_PIN                            PD10

#define E0_STEP_PIN                         PD5
#define E0_DIR_PIN                          PD6
#define E0_ENABLE_PIN                       PD4
#define E0_CS_PIN                           PD7

#define E1_STEP_PIN                         PE6
#define E1_DIR_PIN                          PC13
#define E1_ENABLE_PIN                       PE5
#define E1_CS_PIN                           PC14

#define E2_STEP_PIN                         PE2
#define E2_DIR_PIN                          PE4
#define E2_ENABLE_PIN                       PE3
#define E2_CS_PIN                           PC15

#if HAS_TMC_UART
  //
  // TMC2208/TMC2209 stepper drivers
  //

  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN                 PE9
  #endif
  #ifndef X_SERIAL_RX_PIN
    #define X_SERIAL_RX_PIN                 PE8
  #endif
  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN                 PE14
  #endif
  #ifndef Y_SERIAL_RX_PIN
    #define Y_SERIAL_RX_PIN                 PE13
  #endif
  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN                 PD11
  #endif
  #ifndef Z_SERIAL_RX_PIN
    #define Z_SERIAL_RX_PIN                 PD12
  #endif
  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN                PD3
  #endif
  #ifndef E0_SERIAL_RX_PIN
    #define E0_SERIAL_RX_PIN                PA15
  #endif
  #ifndef E1_SERIAL_TX_PIN
    #define E1_SERIAL_TX_PIN                PC4
  #endif
  #ifndef E1_SERIAL_RX_PIN
    #define E1_SERIAL_RX_PIN                PC5
  #endif
  #ifndef E2_SERIAL_TX_PIN
    #define E2_SERIAL_TX_PIN                PE1
  #endif
  #ifndef E2_SERIAL_RX_PIN
    #define E2_SERIAL_RX_PIN                PE0
  #endif
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0
#define TEMP_1_PIN                          PC1
#define TEMP_2_PIN                          PC2
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN                      PC3
#endif

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      PB3
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN                      PB4
#endif
#ifndef HEATER_2_PIN
  #define HEATER_2_PIN                      PB15
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    PC8
#endif

#ifndef FAN_PIN
  #define FAN_PIN                           PB0
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                          PB1
#endif
#define FAN2_PIN                            PB2

//
// Misc. Functions
//
//#define LED_PIN                           PB14
//#define PS_ON_PIN                         PE11
//#define KILL_PIN                          PC5

/**
 *         ------                 ------
 *   PC9  |10  9 | PA8      PA6  |10  9 | PA5
 *   PC11 | 8  7 | PD2      PC6  | 8  7 | PA4
 *   PC10   6  5 | PC12     PC7    6  5 | PA7
 *   PD0  | 4  3 | PD1      PB10 | 4  3 | RESET
 *    GND | 2  1 | 5V        GND | 2  1 | 5V
 *         ------                 ------
 *          EXP1                   EXP2
 */
#define EXP1_03_PIN                         PD1
#define EXP1_04_PIN                         PD0
#define EXP1_05_PIN                         PC12
#define EXP1_06_PIN                         PC10
#define EXP1_07_PIN                         PD2
#define EXP1_08_PIN                         PC11
#define EXP1_09_PIN                         PA8
#define EXP1_10_PIN                         PC9

#define EXP2_03_PIN                         -1    // RESET
#define EXP2_04_PIN                         PB10
#define EXP2_05_PIN                         PA7
#define EXP2_06_PIN                         PC7
#define EXP2_07_PIN                         PA4
#define EXP2_08_PIN                         PC6
#define EXP2_09_PIN                         PA5
#define EXP2_10_PIN                         PA6

//
// SPI / SD Card
//
#define SD_SCK_PIN                   EXP2_09_PIN
#define SD_MISO_PIN                  EXP2_10_PIN
#define SD_MOSI_PIN                  EXP2_05_PIN

#define SDSS                         EXP2_07_PIN
#define SD_DETECT_PIN                EXP2_04_PIN

//
// LCD / Controller
//
#if ENABLED(FYSETC_242_OLED_12864)

  #define BTN_EN1                    EXP1_10_PIN
  #define BTN_EN2                    EXP1_03_PIN
  #define BTN_ENC                    EXP1_09_PIN

  #define BEEPER_PIN                 EXP2_08_PIN

  #define LCD_PINS_DC                EXP1_05_PIN
  #define LCD_PINS_RS                EXP2_06_PIN  // LCD_RST
  #define DOGLCD_CS                  EXP1_07_PIN
  #define DOGLCD_MOSI                EXP1_06_PIN
  #define DOGLCD_SCK                 EXP1_08_PIN
  #define DOGLCD_A0                  LCD_PINS_DC
  #define FORCE_SOFT_SPI

  #define KILL_PIN                          -1    // NC
  #define NEOPIXEL_PIN               EXP1_04_PIN

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_10_PIN
  #define BTN_ENC                    EXP1_09_PIN

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS              EXP1_04_PIN

    #define BTN_EN1                  EXP1_08_PIN
    #define BTN_EN2                  EXP1_06_PIN

    #define LCD_PINS_ENABLE          EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

  #else

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP2_08_PIN
    #define BTN_EN2                  EXP2_06_PIN

    #define LCD_SDSS                 EXP2_07_PIN

    #define LCD_PINS_ENABLE          EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

    #if ENABLED(FYSETC_MINI_12864)
      // See https://wiki.fysetc.com/Mini12864_Panel
      #define DOGLCD_CS              EXP1_08_PIN
      #define DOGLCD_A0              EXP1_07_PIN
      #if ENABLED(FYSETC_GENERIC_12864_1_1)
        #define LCD_BACKLIGHT_PIN    EXP1_04_PIN
      #endif
      #define LCD_RESET_PIN          EXP1_06_PIN  // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN      EXP1_05_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN      EXP1_04_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN      EXP1_03_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         EXP1_05_PIN
      #endif
    #endif

    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_05_PIN
      #define LCD_PINS_D6            EXP1_04_PIN
      #define LCD_PINS_D7            EXP1_03_PIN
      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif
    #endif

  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                96
  #define BOARD_ST7920_DELAY_2                48
  #define BOARD_ST7920_DELAY_3               640
#endif

#ifndef RGB_LED_R_PIN
  #define RGB_LED_R_PIN                     PB6
#endif
#ifndef RGB_LED_G_PIN
  #define RGB_LED_G_PIN                     PB5
#endif
#ifndef RGB_LED_B_PIN
  #define RGB_LED_B_PIN                     PB7
#endif
#ifndef RGB_LED_W_PIN
  #define RGB_LED_W_PIN                     -1
#endif
