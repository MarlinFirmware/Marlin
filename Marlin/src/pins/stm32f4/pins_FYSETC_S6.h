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
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
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

  //
  // Software serial
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
#define TEMP_BED_PIN                        PC3

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

#define FAN_PIN                             PB0
#define FAN1_PIN                            PB1
#define FAN2_PIN                            PB2

//
// SPI
//
#define SD_SCK_PIN                          PA5
#define SD_MISO_PIN                         PA6
#define SD_MOSI_PIN                         PA7

//
// Misc. Functions
//
//#define LED_PIN                           PB14
//#define BTN_PIN                           PC10
//#define PS_ON_PIN                         PE11
//#define KILL_PIN                          PC5

#define SDSS                                PA4
#define SD_DETECT_PIN                       PB10

//
// LCD / Controller
//
#if ENABLED(FYSETC_242_OLED_12864)

  #define BTN_EN1                           PC9
  #define BTN_EN2                           PD1
  #define BTN_ENC                           PA8

  #define BEEPER_PIN                        PC6

  #define LCD_PINS_DC                       PC12
  #define LCD_PINS_RS                       PC7   // LCD_RST
  #define DOGLCD_CS                         PD2
  #define DOGLCD_MOSI                       PC10
  #define DOGLCD_SCK                        PC11
  #define DOGLCD_A0                  LCD_PINS_DC
  #define FORCE_SOFT_SPI

  #define KILL_PIN                          -1    // NC
  #define NEOPIXEL_PIN                      PD0

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                        PC9
  #define BTN_ENC                           PA8

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS                     PD0

    #define BTN_EN1                         PC11
    #define BTN_EN2                         PC10

    #define LCD_PINS_ENABLE                 PD1
    #define LCD_PINS_D4                     PC12

  #else

    #define LCD_PINS_RS                     PD2

    #define BTN_EN1                         PC6
    #define BTN_EN2                         PC7

    #define LCD_SDSS                        PA4

    #define LCD_PINS_ENABLE                 PC11
    #define LCD_PINS_D4                     PC10

    #if ENABLED(FYSETC_MINI_12864)
      // See https://wiki.fysetc.com/Mini12864_Panel
      #define DOGLCD_CS                     PC11
      #define DOGLCD_A0                     PD2
      #if ENABLED(FYSETC_GENERIC_12864_1_1)
        #define LCD_BACKLIGHT_PIN           PD0
      #endif
      #define LCD_RESET_PIN                 PC10  // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN             PC12
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN             PD0
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN             PD1
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN                PC12
      #endif
    #endif

    #if IS_ULTIPANEL
      #define LCD_PINS_D5                   PC12
      #define LCD_PINS_D6                   PD0
      #define LCD_PINS_D7                   PD1
      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif
    #endif

  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if HAS_MARLINUI_U8GLIB
  #ifndef BOARD_ST7920_DELAY_1
    #define BOARD_ST7920_DELAY_1  DELAY_NS(96)
  #endif
  #ifndef BOARD_ST7920_DELAY_2
    #define BOARD_ST7920_DELAY_2  DELAY_NS(48)
  #endif
  #ifndef BOARD_ST7920_DELAY_3
    #define BOARD_ST7920_DELAY_3 DELAY_NS(640)
  #endif
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
