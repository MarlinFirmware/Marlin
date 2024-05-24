/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// zzcatvs 2024-05-24

#pragma once

#include "env_validate.h"

#if HOTENDS > 5 || E_STEPPERS > 5
  #error "FYSETC SPIDER KING V1.1 supports up to 5 hotends / E-steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "FYSETC SPIDER KING V1.1"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif



#if ANY(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #undef NO_EEPROM_SELECTED
  #ifndef FLASH_EEPROM_EMULATION
    #define FLASH_EEPROM_EMULATION
  #endif
  #define EEPROM_PAGE_SIZE      (0x800UL) // 2K
  #define EEPROM_START_ADDRESS  (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE
#endif


//
// BLTouch and Z Probe
//
#if ENABLED(BLTOUCH)||ENABLED(FIX_MOUNTED_PROBE)
  #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #define Z_MIN_PIN                       PA0//PD0
  #elif !defined(Z_MIN_PROBE_PIN)
    #define Z_MIN_PROBE_PIN                 PA0//PD0
  #endif
#endif

//
// Servos
//
#define SERVO0_PIN                          PA1//PD1

//
// Software SPI pins for TMC stepper drivers
//
#if HAS_TMC_SPI
  #define TMC_USE_SW_SPI
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


//
// Limit Switches
//
#define X_MIN_PIN                           PC5//PA0
#define Y_MIN_PIN                           PC4//PA1
#define Z_MIN_PIN                           PB6
#define X_MAX_PIN                           PB5
#define Y_MAX_PIN                           PF13
#define Z_MAX_PIN                           PF14

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA0
#endif

//
// Steppers
//

// MOT1
#define X_STEP_PIN                          PG7 //D1_STEP 
#define X_DIR_PIN                           PG6 //D1_DIR  
#define X_ENABLE_PIN                        PE11  //D1_EN  
#define X_CS_PIN                            PD2 //D1_CS  

// MOT2
#define X2_STEP_PIN                         PD11  //D2_STEP 
#define X2_DIR_PIN                          PD10 //D2_DIR  
#define X2_ENABLE_PIN                       PG10 //D2_EN   
#define X2_CS_PIN                           PE15 //D2_CS   

// MOT3
#define Y_STEP_PIN                          PG14  //PD14 
#define Y_DIR_PIN                           PG12  //PD13 
#define Y_ENABLE_PIN                        PG15  //PD15 
#define Y_CS_PIN                            PD8 //D3_CS 

// MOT4
#define Z_STEP_PIN                          PD4  //D4_STEP 
#define Z_DIR_PIN                           PD6  //D4_DIR 
#define Z_ENABLE_PIN                        PD5  //D4_EN 
#define Z_CS_PIN                            PD7 //D4_CS 

// MOT5
#define Z2_STEP_PIN                         PE5 //D5_STEP 
#define Z2_DIR_PIN                          PC13  //D5_DIR 
#define Z2_ENABLE_PIN                       PE6 //D5_EN 
#define Z2_CS_PIN                           PC14  //D5_CS 

// MOT6
#define E0_STEP_PIN                         PE3 //D6_STEP 
#define E0_DIR_PIN                          PE4 //D6_DIR 
#define E0_ENABLE_PIN                       PE2 //D6_EN 
#define E0_CS_PIN                           PC15  //D6_CS 

// MOT7
#define E1_STEP_PIN                         PG13 //D7_STEP 
#define E1_DIR_PIN                          PG8 //D7_DIR 
#define E1_ENABLE_PIN                       PG9 //D7_EN 
#define E1_CS_PIN                           PG3 //D7_CS 

// MOT8
#define E2_STEP_PIN                         PE1 //D8_STEP 
#define E2_DIR_PIN                          PE0 //D8_DIR 
#define E2_ENABLE_PIN                       PB2 //D8_EN 
#define E2_CS_PIN                           PD9 //D8_CS 

// MOT9
#define E3_STEP_PIN                         PF4 //D9_STEP 
#define E3_DIR_PIN                          PF3 //D9_DIR 
#define E3_ENABLE_PIN                       PF2 //D9_EN 
#define E3_CS_PIN                           PF5 //D9_CS 

// MOT10
#define E4_STEP_PIN                         PF15 //D10_STEP 
#define E4_DIR_PIN                          PG0 //D10_DIR 
#define E4_ENABLE_PIN                       PG5 //D10_EN 
#define E4_CS_PIN                           PG11  //D10_CS 

#if HAS_TMC_UART
  //
  // TMC2208/TMC2209 stepper drivers
  //
  #define X_SERIAL_TX_PIN                   PD2
  #define X_SERIAL_RX_PIN                   PD2

  #define X2_SERIAL_TX_PIN                  PE15
  #define X2_SERIAL_RX_PIN                  PE15

  #define Y_SERIAL_TX_PIN                   PD8
  #define Y_SERIAL_RX_PIN                   PD8

  #define Z_SERIAL_TX_PIN                   PD7
  #define Z_SERIAL_RX_PIN                   PD7

  #define Z2_SERIAL_TX_PIN                  PC14
  #define Z2_SERIAL_RX_PIN                  PC14

  #define E0_SERIAL_TX_PIN                  PC15
  #define E0_SERIAL_RX_PIN                  PC15

  #define E1_SERIAL_TX_PIN                  PG3
  #define E1_SERIAL_RX_PIN                  PG3

  #define E2_SERIAL_TX_PIN                  PD9
  #define E2_SERIAL_RX_PIN                  PD9
    
  #define E3_SERIAL_TX_PIN                  PF5
  #define E3_SERIAL_RX_PIN                  PF5

  #define E4_SERIAL_TX_PIN                  PG11
  #define E4_SERIAL_RX_PIN                  PG11

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1
#define TEMP_1_PIN                          PF9
#define TEMP_2_PIN                          PC3_C
#define TEMP_3_PIN                          PC2_C
#define TEMP_4_PIN                          PC0
//#define TEMP_CHAMBER_PIN                    PC0
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN                      PF10
#endif

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      PB4//PC5
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN                      PB0//PC4
#endif
#ifndef HEATER_2_PIN
  #define HEATER_2_PIN                      PD13  //N_OUT2
#endif
#ifndef HEATER_3_PIN
  #define HEATER_3_PIN                      PC8 //N_OUT3
#endif
#ifndef HEATER_4_PIN
  #define HEATER_4_PIN                      PA15  //N_OUT4
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    PE10  //BED_OUT
#endif

#ifndef FAN0_PIN
  #define FAN0_PIN                           PE8 //FAN0
#endif
#ifndef FAN1_PIN
  #define FAN1_PIN                          PE9 //FAN0
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN                          PD15  //FAN2
#endif
#ifndef FAN3_PIN
  #define FAN3_PIN                          PD12  //FAN3
#endif
#ifndef FAN4_PIN
  #define FAN4_PIN                          PD14  //FAN4
#endif

//
// Misc. Functions
//
//#define LED_PIN                           PB14
//#define PS_ON_PIN                         PE11
//#define KILL_PIN                          PC5

/**
 *          ------                ------                 ------            
 *     PA2 |10  9 | PA3      PA6 |10  9 | PA5       PC9 |10  9 | PA8  
 *      -1 | 8  7 | PB10     PC6 | 8  7 | PA4      PC12 | 8  7 | PD0    
 *     PA6   6  5 | PA7      PC7 | 6  5   PA7      PC10 | 6  5   PC11
 *     PA5 | 4  3 | PA4     PB10 | 4  3 | RESET     PG4 | 4  3 | PE7 
 *     GND | 2  1 | 5V       GND | 2  1 | 5V        GND | 2  1 | 5V  
 *          ------                ------                 ------ 
 *           EXP3                  EXP2                   EXP1 
 */
#define EXP1_03_PIN                         PE7//LCD_D7
#define EXP1_04_PIN                         PG4//PE8//LCD_D6
#define EXP1_05_PIN                         PC11//LCD_D5
#define EXP1_06_PIN                         PC10//LCD_D4
#define EXP1_07_PIN                         PD0//PA15//LCD_RS
#define EXP1_08_PIN                         PC12//LCD_EN
#define EXP1_09_PIN                         PA8//BTN_ENC
#define EXP1_10_PIN                         PC9//BEEP

#define EXP2_03_PIN                         -1//RESET
#define EXP2_04_PIN                         PB10//CD
#define EXP2_05_PIN                         PA7//MOSI
#define EXP2_06_PIN                         PC7//BTN_EN2
#define EXP2_07_PIN                         PA4//SS
#define EXP2_08_PIN                         PC6//BTN_EN1
#define EXP2_09_PIN                         PA5//SCK
#define EXP2_10_PIN                         PA6//MISO

#define EXP3_03_PIN                         EXP2_07_PIN//SS
#define EXP3_04_PIN                         EXP2_09_PIN//SCK
#define EXP3_05_PIN                         EXP2_05_PIN//MOSI
#define EXP3_06_PIN                         EXP2_10_PIN//MISO
#define EXP3_07_PIN                         EXP2_04_PIN//CD
#define EXP3_08_PIN                         -1//3V3
#define EXP3_09_PIN                         PA3//LCD_RX/RX
#define EXP3_10_PIN                         PA2//LCD_TX/TX

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

  #define BEEPER_PIN                 EXP2_08_PIN

  #define BTN_EN1                    EXP1_10_PIN
  #define BTN_EN2                    EXP1_03_PIN
  #define BTN_ENC                    EXP1_09_PIN

  #define LCD_PINS_DC                EXP1_05_PIN
  #define LCD_PINS_RS                EXP2_06_PIN  // LCD_RST
  #define DOGLCD_CS                  EXP1_07_PIN
  #define DOGLCD_MOSI                EXP1_06_PIN
  #define DOGLCD_SCK                 EXP1_08_PIN
  #define DOGLCD_A0                  LCD_PINS_DC
  #define FORCE_SOFT_SPI

  //#define KILL_PIN                          -1    // NC
  #define NEOPIXEL_PIN               EXP1_04_PIN

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_10_PIN
  #define BTN_ENC                    EXP1_09_PIN

  #define LCD_PINS_RS                EXP1_07_PIN

  #define BTN_EN1                    EXP2_08_PIN
  #define BTN_EN2                    EXP2_06_PIN

  #define LCD_SDSS                   EXP2_07_PIN

  #define LCD_PINS_EN                EXP1_08_PIN
  #define LCD_PINS_D4                EXP1_06_PIN

  #if ENABLED(FYSETC_MINI_12864)
    // See https://wiki.fysetc.com/Mini12864_Panel
    #define DOGLCD_CS                EXP1_08_PIN
    #define DOGLCD_A0                EXP1_07_PIN
    #if ENABLED(FYSETC_GENERIC_12864_1_1)
      #define LCD_BACKLIGHT_PIN      EXP1_04_PIN
    #endif
    #define LCD_RESET_PIN            EXP1_06_PIN  // Must be high or open for LCD to operate normally.
    #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
      #ifndef RGB_LED_R_PIN
        #define RGB_LED_R_PIN        EXP1_05_PIN
      #endif
      #ifndef RGB_LED_G_PIN
        #define RGB_LED_G_PIN        EXP1_04_PIN
      #endif
      #ifndef RGB_LED_B_PIN
        #define RGB_LED_B_PIN        EXP1_03_PIN
      #endif
    #elif ENABLED(FYSETC_MINI_12864_2_1)
      #define NEOPIXEL_PIN           EXP1_05_PIN
    #endif
  #endif

  #if IS_ULTIPANEL
    #define LCD_PINS_D5              EXP1_05_PIN
    #define LCD_PINS_D6              EXP1_04_PIN
    #define LCD_PINS_D7              EXP1_03_PIN
    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif
  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                96
  #define BOARD_ST7920_DELAY_2                48
  #define BOARD_ST7920_DELAY_3               640
#endif

/**
 *                      -------
 *            GND | 9  |       | 8 | 3.3V
 *  (ESP-CS) PB12 | 10 |       | 7 | PB15 (ESP-MOSI)
 *           3.3V | 11 |       | 6 | PB14 (ESP-MISO)
 * (ESP-IO0)  PD7 | 12 |       | 5 | PB13 (ESP-CLK)
 * (ESP-IO4) PD10 | 13 |       | 4 | --
 *             -- | 14 |       | 3 | PE15 (ESP-EN)
 *  (ESP-RX)  PD8 | 15 |       | 2 | --
 *  (ESP-TX)  PD9 | 16 |       | 1 | PE14 (ESP-RST)
 *                      -------
 *                       WIFI
 */
#define ESP_WIFI_MODULE_COM                    1  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
#define ESP_WIFI_MODULE_BAUDRATE        BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
#define ESP_WIFI_MODULE_RESET_PIN            PB3
#define ESP_WIFI_MODULE_ENABLE_PIN           PD1//PC8
#define ESP_WIFI_MODULE_GPIO0_PIN            PG2//PB4
#define ESP_WIFI_MODULE_GPIO4_PIN            PG1//PB7

//
// NeoPixel LED
//
//#define NEOPIXEL_PIN                     PD3
#ifndef NEOPIXEL2_PIN
  #define NEOPIXEL2_PIN                     PD3 //Neo-pixel
#endif
