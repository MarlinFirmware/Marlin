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

/**
 * MKS Robin Lite 3 (STM32F103RCT6) board pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS Robin Lite3 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "ERYONE THINKER MINI"
#endif
#define BOARD_WEBSITE_URL "github.com/eryone"

//#define DISABLE_DEBUG
#define DISABLE_JTAG
//#define ENABLE_SPI3
#define FLASH_EEPROM_EMULATION
#define FAN_SOFT_PWM

 #undef EEPROM_PAGE_SIZE
 #undef EEPROM_START_ADDRESS
 #undef STM32_FLASH_SIZE
#define STM32_FLASH_SIZE 512
#define EEPROM_PAGE_SIZE     uint16(0x800) // 2KB
#define EEPROM_START_ADDRESS uint32(0x8000000 + (STM32_FLASH_SIZE) * 1024 - 2 * EEPROM_PAGE_SIZE)
#undef E2END
#define E2END                (EEPROM_PAGE_SIZE - 1) // 2KB

//
// Servos
//
#define SERVO0_PIN         PA12

//
// Limit Switches
// 
#define X_STOP_PIN         PD8
#define Y_STOP_PIN         PD15
#define Z_MIN_PIN          PA11//PA9
//#define Z_MAX_PIN          PB1

//
// Steppers
//
#define X_STEP_PIN         PB15
#define X_DIR_PIN          PB14
#define X_ENABLE_PIN       PD10

#define Y_STEP_PIN         PD14
#define Y_DIR_PIN          PD13
#define Y_ENABLE_PIN       PC6

#define Z_STEP_PIN         PC8
#define Z_DIR_PIN          PC7
#define Z_ENABLE_PIN       PA8

#define E0_STEP_PIN        PE13
#define E0_DIR_PIN         PE14
#define E0_ENABLE_PIN      PB13

//#define E1_STEP_PIN        PD13
//#define E1_DIR_PIN         PD12
//#define E1_ENABLE_PIN      PC6

//
// Heaters 0,1 / Fans / Bed
//
#define HEATER_0_PIN       PD11
//#define HEATER_1_PIN       PD4
#define FAN_PIN            PB5
//#define FAN1_PIN            PB9
#define HEATER_BED_PIN     PD12
// #define PS_ON_PIN         PB9
//
#define E0_AUTO_FAN_PIN PB4
#define TEMP_COOLER_PIN PB9

// Trinamic Software Serial
//
/**
 * TMC220x stepper drivers
 * Hardware serial communication ports.
 */
#if HAS_TMC_UART
  #define X_HARDWARE_SERIAL  MSerial4
  #define Y_HARDWARE_SERIAL  MSerial4
  #define Z_HARDWARE_SERIAL  MSerial4
  #define E0_HARDWARE_SERIAL MSerial4
#endif
//
// Temperature Sensors
//
#define TEMP_BED_PIN       PC2  //TB
#define TEMP_0_PIN         PC1  //TH1
//#define TEMP_1_PIN         PC3  //TH2
#define TEMP_CHAMBER_PIN   PC3
#define FIL_RUNOUT_PIN     PA10 // MT_DET

//
// LCD Pins
//
#if HAS_WIRED_LCD

  #define BEEPER_PIN       PE12
  #define BTN_ENC          PE11
  #define LCD_PINS_ENABLE  PE10
  #define LCD_PINS_RS      PE9
  #define BTN_EN1          PE4
  #define BTN_EN2          PE3

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)

    #define LCD_BACKLIGHT_PIN -1
    #define LCD_RESET_PIN  -1
    #define DOGLCD_A0      PC4
    #define DOGLCD_CS      PA7
    #define DOGLCD_SCK     PB13
    #define DOGLCD_MOSI    PB15

    // Required for MKS_MINI_12864 with this board
    #define MKS_LCD12864B
    #undef SHOW_BOOTSCREEN

  #else // !MKS_MINI_12864

    #define LCD_PINS_D4    PE8
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5  PE7
      #define LCD_PINS_D6  PB2
      #define LCD_PINS_D7  PB1
    #endif

  #endif // !MKS_MINI_12864

#endif // HAS_SPI_LCD

//
// SD Card
//
#define ENABLE_SPI1
#define SD_DETECT_PIN         PA4
#define SD_SCK_PIN            PA5
#define SD_MISO_PIN           PA6
#define SD_MOSI_PIN           PA7
#define SD_SS_PIN             PC4

#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(125)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(125)
#endif
