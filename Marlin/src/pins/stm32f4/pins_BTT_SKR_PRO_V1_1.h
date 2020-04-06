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

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "BIGTREE SKR Pro V1.1 supports up to 3 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "BTT SKR Pro V1.1"

#include "pins_BTT_SKR_PRO_common.h"
extern int home5x;
// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation

//
// Servos
//
#define SERVO0_PIN                          PA1

//
// Limit Switches
//
#define X_MIN_PIN                           PB10
#if ENABLED (CNC_5X)
  #define E_MIN_PIN                         PE15
#else
  #define X_MAX_PIN                         PE15  
#endif
#define Y_MIN_PIN                           PE12
#if ENABLED (CNC_5X)
  #define Z_MIN_M167_PIN                    PE10
  #define E_MAX_PIN                         PE6
#else
  #define Y_MAX_PIN                         PE10
#endif
#define Z_MIN_PIN                           PG8
#define Z_MAX_PIN                           PG5



//
// Z Probe must be this pins
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA2
#endif

//
// Steppers
//
#define X_STEP_PIN                          PE9
#define X_DIR_PIN                           PF1
#define X_ENABLE_PIN                        PF2
#ifndef X_CS_PIN
  #define X_CS_PIN                          PA15
#endif

#define Y_STEP_PIN                          PE11
#define Y_DIR_PIN                           PE8
#define Y_ENABLE_PIN                        PD7
 #ifndef Y_CS_PIN
  #define Y_CS_PIN                          PB8
#endif

#define Z_STEP_PIN                          PE13
#define Z_DIR_PIN                           PC2
#define Z_ENABLE_PIN                        PC0
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PB9
#endif

#define E0_STEP_PIN                         PE14
#if ENABLED(CNC_5X)
  #define E0_DIR_PIN                      PD14  //Heater1 PIN for Workaround
  #define EA_DIR_PIN                      PA0   //PIN for direct direction change on M168
#else 
  #define E0_DIR_PIN                        PA0
#endif
#define E0_ENABLE_PIN                       PC3
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PB3
#endif

#define E1_STEP_PIN                         PD15
#if ENABLED(CNC_5X)
  #define E1_DIR_PIN                        PB0 //Heater2 PIN for Workaround
  #define EB_DIR_PIN                        PE7 //PIN for direct direction change on M168
#else
  #define E1_DIR_PIN                        PE7
#endif
#define E1_ENABLE_PIN                       PA3
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PG15
#endif

#define E2_STEP_PIN                         PD13
#define E2_DIR_PIN                          PG9
#define E2_ENABLE_PIN                       PF0
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PG12
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PC12
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PC11
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PC10
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

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                   PC13
  #define X_SERIAL_RX_PIN                   PC13

  #define Y_SERIAL_TX_PIN                   PE3
  #define Y_SERIAL_RX_PIN                   PE3

  #define Z_SERIAL_TX_PIN                   PE1
  #define Z_SERIAL_RX_PIN                   PE1

  #define E0_SERIAL_TX_PIN                  PD4
  #define E0_SERIAL_RX_PIN                  PD4

  #define E1_SERIAL_TX_PIN                  PD1
  #define E1_SERIAL_RX_PIN                  PD1

  #define E2_SERIAL_TX_PIN                  PD6
  #define E2_SERIAL_RX_PIN                  PD6

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PF4   // T1 <-> E0
#define TEMP_1_PIN                          PF5   // T2 <-> E1
#define TEMP_2_PIN                          PF6   // T3 <-> E2
#define TEMP_BED_PIN                        PF3   // T0 <-> Bed

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // Heater0
#if DISABLED(CNC_5X)
#define HEATER_1_PIN                        PD14 // Heater1
#define HEATER_2_PIN                        PB0  // Heater2
#endif
#define HEATER_BED_PIN                      PD12  // Hotbed
#define FAN_PIN                             PC8   // Fan0
#define FAN1_PIN                            PE5   // Fan1
#if DISABLED(CNC_5X)
#define FAN2_PIN                            PE6   // Fan2
#endif
//
// Misc. Functions
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION                  LCD
#endif

//
// Onboard SD card
//   NOT compatible with LCD
//
#if SDCARD_CONNECTION == ONBOARD && !HAS_SPI_LCD
  #define SOFTWARE_SPI                            // Use soft SPI for onboard SD
  #define SDSS                              PA4
  #define SCK_PIN                           PA5
  #define MISO_PIN                          PA6
  #define MOSI_PIN                          PB5
#else
  #define SDSS                              PB12
#endif

/**
 *               _____                                             _____
 *           NC | · · | GND                                    5V | · · | GND
 *        RESET | · · | PF12(SD_DETECT)             (LCD_D7)  PG7 | · · | PG6  (LCD_D6)
 *   (MOSI)PB15 | · · | PF11(BTN_EN2)               (LCD_D5)  PG3 | · · | PG2  (LCD_D4)
 *  (SD_SS)PB12 | · · | PG10(BTN_EN1)               (LCD_RS) PD10 | · · | PD11 (LCD_EN)
 *    (SCK)PB13 | · · | PB14(MISO)                 (BTN_ENC)  PA8 | · · | PG4  (BEEPER)
 *               ￣￣                                               ￣￣
 *               EXP2                                              EXP1
 */

//
// Have I2C external EEPROM  (AT24C256)
//
#define I2C_EXT_EEPROM

#if ENABLED(I2C_EXT_EEPROM)
  #define I2C_EEPROM
  #ifdef E2END
     #undef E2END
  #endif
  #define E2END 0x7FFF // EEPROM end address AT24C256 (32kB)
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
  #if HAS_CUTTER && !defined(SPINDLE_LASER_ENA_PIN)
    #define SPINDLE_LASER_ENA_PIN           TEMP_2_PIN    // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN           SERVO0_PIN   // Hardware PWM
    #define SPINDLE_DIR_PIN                 PA2   
  #endif


//
// LCDs and Controllers
//
#if HAS_SPI_LCD
  #define BEEPER_PIN                        PG4
  #define BTN_ENC                           PA8

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS                     PG6

    #define BTN_EN1                         PD11
    #define BTN_EN2                         PG2

    #define LCD_PINS_ENABLE                 PG7
    #define LCD_PINS_D4                     PG3

    // CR10_Stock Display needs a different delay setting on SKR PRO v1.1, so undef it here.
    // It will be defined again at the #HAS_GRAPHICAL_LCD section below.
    #undef ST7920_DELAY_1
    #undef ST7920_DELAY_2
    #undef ST7920_DELAY_3

  #else

    #define LCD_PINS_RS                     PD10

    #define BTN_EN1                         PG10
    #define BTN_EN2                         PF11
    #define SD_DETECT_PIN                   PF12

    #define LCD_SDSS                        PB12

    #define LCD_PINS_ENABLE                 PD11
    #define LCD_PINS_D4                     PG2

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS                     PD11
      #define DOGLCD_A0                     PD10
      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN                 PG2   // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN             PG3
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN             PG6
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN             PG7
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN                PG3
      #endif
    #endif // !FYSETC_MINI_12864

    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PG3
      #define LCD_PINS_D6                   PG6
      #define LCD_PINS_D7                   PG7
    #endif

  #endif

  // Alter timing for graphical display
  #if HAS_GRAPHICAL_LCD
    #define BOARD_ST7920_DELAY_1 DELAY_NS(96)
    #define BOARD_ST7920_DELAY_2 DELAY_NS(48)
    #define BOARD_ST7920_DELAY_3 DELAY_NS(600)
  #endif

#endif // HAS_SPI_LCD
