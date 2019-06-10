/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR mini V1.1"
#endif

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define U_MIN_PIN          -1
#define V_MIN_PIN          -1
#define W_MIN_PIN          -1
#define X_STOP_PIN         PC2
#define Y_STOP_PIN         PC1
#define Z_STOP_PIN         PC0

//
// Steppers
//

/**
 * TODO: Currently using same Enable pin for all steppers.
 */

#define X_STEP_PIN         PC6
#define X_DIR_PIN          PC7
#define X_ENABLE_PIN       PB15
//#ifndef X_CS_PIN
//  #define X_CS_PIN         PC10
//#endif

#define Y_STEP_PIN         PB13
#define Y_DIR_PIN          PB14
#define Y_ENABLE_PIN       PB12
//#ifndef Y_CS_PIN
//  #define Y_CS_PIN         PC11
//#endif

#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB11
#define Z_ENABLE_PIN       PB2
//#ifndef Z_CS_PIN
//  #define Z_CS_PIN         PC12
//#endif

#define E0_STEP_PIN        PC5
#define E0_DIR_PIN         PB0
#define E0_ENABLE_PIN      PC4
//#ifndef E0_CS_PIN
//  #define E0_CS_PIN        PC13
//#endif

//#define E1_STEP_PIN        PC8
//#define E1_DIR_PIN         PC9
//#define E1_ENABLE_PIN      PA8

//#define E2_STEP_PIN        PC10
//#define E2_DIR_PIN         PC11
//#define E2_ENABLE_PIN      PA8

#if ENABLED(TMC_USE_SW_SPI)
  #define TMC_SW_SCK       PB3
  #define TMC_SW_MISO      PB4
  #define TMC_SW_MOSI      PB5
#endif

//
// Misc. Functions
//

//#define LED_PIN            PD2

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA8   // EXTRUDER 1
//#define HEATER_1_PIN       PD2
#define  FAN_PIN           PC8
#define HEATER_BED_PIN     PC9   // BED

//
// Temperature Sensors
//
#define TEMP_BED_PIN         PB1   // Analog Input
#define TEMP_0_PIN           PA0   // Analog Input
//#define TEMP_1_PIN           PA3   // Analog Input (onboard SD_DETECT_PIN)

//
// LCD Pins
//

/**
 *               _____                                             _____
 *           NC | · · | GND                                    5V | · · | GND
 *        RESET | · · | 1.31(SD_DETECT)             (LCD_D7) 1.23 | · · | 1.22 (LCD_D6)
 *   (MOSI)0.18 | · · | 3.25(BTN_EN2)               (LCD_D5) 1.21 | · · | 1.20 (LCD_D4)
 *  (SD_SS)0.16 | · · | 3.26(BTN_EN1)               (LCD_RS) 1.19 | · · | 1.18 (LCD_EN)
 *    (SCK)0.15 | · · | 0.17(MISO)                 (BTN_ENC) 0.28 | · · | 1.30 (BEEPER)
 *               ￣￣                                               ￣￣
 *               EXP2                                              EXP1
 */

#if ENABLED(ULTRA_LCD)

  #define BEEPER_PIN       PC10
  #define BTN_ENC          PC11
  #define LCD_PINS_RS      PC12

  #define BTN_EN1          PD2
  #define BTN_EN2          PB8
  #define SD_DETECT_PIN    PB9

  //#define LCD_SDSS         P0_16   // (16) J3-7 & AUX-4

  #define LCD_PINS_ENABLE  PB6
  #define LCD_PINS_D4      PC13

  #if ENABLED(ULTIPANEL)
    #define LCD_PINS_D5    PB7
    #define LCD_PINS_D6    PC15
    #define LCD_PINS_D7    PC14
  #endif

#endif // ULTRA_LCD

//#define _SPI1                   // if using spi1, please open it or enable spi3

#ifdef _SPI1
  #define SDSS             PA4  // SPI1片选
#else
  #define SDSS             PA15 // SPI3片选
  //#define DISABLE_DEBUG
  #define DISABLE_JTAG
#endif

//#define USB_SD_DISABLED
//#define USB_SD_ONBOARD        // Provide the onboard SD card to the host as a USB mass storage device

//#define LPC_SD_LCD            // Marlin uses the SD drive attached to the LCD
//#define STM32_SD_ONBOARD      // Marlin uses the SD drive on the control board

#if ENABLED(LPC_SD_LCD)

  #define SCK_PIN          PB3
  #define MISO_PIN         PB4
  #define MOSI_PIN         PB5
  #define SS_PIN           PA15  // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    PA4   // Chip select for "System" SD card

#elif ENABLED(STM32_SD_ONBOARD)

  #if ENABLED(USB_SD_ONBOARD)
    // When sharing the SD card with a PC we want the menu options to
    // mount/unmount the card and refresh it. So we disable card detect.
    #define SHARED_SD_CARD
    #undef SD_DETECT_PIN           // redefine detect pin onboard tf card
    #define SD_DETECT_PIN  PA3
    #if TEMP_1_PIN == PA3
      #error "TEMP_1_PIN and SD_DETECT_PIN are in conflict."
    #endif
  #endif

  #define SCK_PIN          PA5
  #define MISO_PIN         PA6
  #define MOSI_PIN         PA7
  #define SS_PIN           PA15   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    PA4    // Chip select for "System" SD card

#endif
