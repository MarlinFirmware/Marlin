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

#ifndef STM32F4
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "RUMBA32 supports up to 3 hotends / E-steppers."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "FYSETC_S6"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

// change the prio to 3 , 2 is for software serial
//#define TEMP_TIMER_IRQ_PRIO 3

//
// EEPROM Emulation
//
#define FLASH_EEPROM_EMULATION
//#define SRAM_EEPROM_EMULATION
//#define I2C_EEPROM
#ifdef I2C_EEPROM
  #undef E2END // Defined in Arduino Core STM32 to be used with EEPROM emulation. This board uses a real EEPROM.
  #define E2END 0xFFF // 4KB
#endif

//
// Servos
//
#define SERVO0_PIN         PA3

//
// Limit Switches
//
#define X_MIN_PIN          PB14
#define X_MAX_PIN          PA1
#define Y_MIN_PIN          PB13
#define Y_MAX_PIN          PA2
#define Z_MIN_PIN          PA0
#define Z_MAX_PIN          PA3

//
// Filament Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PA1
#endif

//
// Steppers
//
#define X_STEP_PIN         PE11
#define X_DIR_PIN          PE10
#define X_ENABLE_PIN       PE12
#define X_CS_PIN           PE7

#define Y_STEP_PIN         PD8
#define Y_DIR_PIN          PB12
#define Y_ENABLE_PIN       PD9
#define Y_CS_PIN           PE15

#define Z_STEP_PIN         PD14
#define Z_DIR_PIN          PD13
#define Z_ENABLE_PIN       PD15
#define Z_CS_PIN           PD10

#define E0_STEP_PIN        PD5
#define E0_DIR_PIN         PD6
#define E0_ENABLE_PIN      PD4
#define E0_CS_PIN          PD7

#define E1_STEP_PIN        PE6
#define E1_DIR_PIN         PC13
#define E1_ENABLE_PIN      PE5
#define E1_CS_PIN          PC14

#define E2_STEP_PIN        PE2
#define E2_DIR_PIN         PE4
#define E2_ENABLE_PIN      PE3
#define E2_CS_PIN          PC15

#if HAS_TMC220x
  //
  // TMC2208/TMC2209 stepper drivers
  //

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN  PE9
  #define X_SERIAL_RX_PIN  PE8

  #define Y_SERIAL_TX_PIN  PE14
  #define Y_SERIAL_RX_PIN  PE13

  #define Z_SERIAL_TX_PIN  PD11
  #define Z_SERIAL_RX_PIN  PD12

  #define E0_SERIAL_TX_PIN PD3
  #define E0_SERIAL_RX_PIN PA15

  #define E1_SERIAL_TX_PIN PC4
  #define E1_SERIAL_RX_PIN PC5

  #define E2_SERIAL_TX_PIN PE1
  #define E2_SERIAL_RX_PIN PE0
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         PC0
#define TEMP_1_PIN         PC1
#define TEMP_2_PIN         PC2
#define TEMP_BED_PIN       PC3

//
// Heaters / Fans
//
#define HEATER_0_PIN       PB3
#define HEATER_1_PIN       PB4
#define HEATER_2_PIN       PB15
#define HEATER_BED_PIN     PC8

#define FAN_PIN            PB0
#define FAN1_PIN           PB1
#define FAN2_PIN           PB2

//
// SPI
//
#define SCK_PIN            PA5
#define MISO_PIN           PA6
#define MOSI_PIN           PA7

//
// Misc. Functions
//
//#define LED_PIN            PB14
//#define BTN_PIN            PC10
//#define PS_ON_PIN          PE11
//#define KILL_PIN           PC5

#define SDSS               PA4
#define SD_DETECT_PIN  	   PB10

//
// LCD / Controller
//
#if HAS_SPI_LCD
  #define BEEPER_PIN       PC9
  #define BTN_ENC          PA8

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PD0

    #define BTN_EN1        PC11
    #define BTN_EN2        PC10

    #define LCD_PINS_ENABLE PD1
    #define LCD_PINS_D4    PC12

    // CR10_Stock Display needs a different delay setting on SKR PRO v1.1, so undef it here.
    // It will be defined again at the #HAS_GRAPHICAL_LCD section below.
    #undef ST7920_DELAY_1
    #undef ST7920_DELAY_2
    #undef ST7920_DELAY_3

  #else

    #define LCD_PINS_RS    PD2

    #define BTN_EN1        PC6
    #define BTN_EN2        PC7

    #define LCD_SDSS       PA4

    #define LCD_PINS_ENABLE PC11
    #define LCD_PINS_D4    PC10

    #if ENABLED(FYSETC_MINI_12864)
     // See https://wiki.fysetc.com/Mini12864_Panel
      #define DOGLCD_CS    PC11
      #define DOGLCD_A0    PD2
      #if ENABLED(FYSETC_GENERIC_12864_1_1)
        #define LCD_BACKLIGHT_PIN PD0
      #endif
      #define LCD_RESET_PIN PC10   // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN PC12
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN PD0
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN PD1
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN    PC12
      #endif
    #endif // !FYSETC_MINI_12864

    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5  PC12
      #define LCD_PINS_D6  PD0
      #define LCD_PINS_D7  PD1
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

#ifndef RGB_LED_R_PIN
  #define RGB_LED_R_PIN    PB6
#endif
#ifndef RGB_LED_G_PIN
  #define RGB_LED_G_PIN    PB5
#endif
#ifndef RGB_LED_B_PIN
  #define RGB_LED_B_PIN    PB7
#endif
#ifndef RGB_LED_W_PIN
  #define RGB_LED_W_PIN    -1
#endif
