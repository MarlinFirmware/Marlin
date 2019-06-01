/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef __STM32F1__
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

/**
 * Marlin for STM32F103RC test
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR MINI V1.1"
#endif

// Ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define X_MAX_PIN          PA2
#define Y_MAX_PIN          PA1
#define Z_MAX_PIN          PC3
#define X_MIN_PIN          PC2
#define Y_MIN_PIN          PC1
#define Z_MIN_PIN          PC0

//
// Steppers
//
#define X_ENABLE_PIN       PB15
#define X_STEP_PIN         PC6
#define X_DIR_PIN          PC7

#define Y_ENABLE_PIN       PB12
#define Y_STEP_PIN         PB13
#define Y_DIR_PIN          PB14

#define Z_ENABLE_PIN       PB2
#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB11

#define E0_ENABLE_PIN      PC4
#define E0_STEP_PIN        PC5
#define E0_DIR_PIN         PB0

#if HAS_DRIVER(TMC2208) || HAS_DRIVER(TMC2209)
    /**
   * TMC2208 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  #define X_HARDWARE_SERIAL  Serial1
  #define Y_HARDWARE_SERIAL  Serial1
  #define Z_HARDWARE_SERIAL  Serial1
  #define E0_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN  PB9
  #define X_SERIAL_RX_PIN  PB9

 /*#define Y_SERIAL_TX_PIN  PB9
  #define Y_SERIAL_RX_PIN  PB9

  #define Z_SERIAL_TX_PIN  PB9
  #define Z_SERIAL_RX_PIN  PB9

  #define E0_SERIAL_TX_PIN PB9
  #define E0_SERIAL_RX_PIN PB9*/

#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA8   // EXTRUDER 1
#define HEATER_BED_PIN     PC9   // BED

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // ANALOG NUMBERING
#define TEMP_BED_PIN       PB1   // ANALOG NUMBERING


/*
|                    _____                                             _____
|                5V | · · | GND                                    NC | · · | GND
|     (LCD_D7) PC14 | · · | PC15 (LCD_D6)                       RESET | · · | PB9(SD_DETECT)
|     (LCD_D5) PB7  | · · | PC13 (LCD_D4)                  (MOSI)PB5  | · · | PB8(BTN_EN2)
|     (LCD_RS) PC12 | · · | PB6 (LCD_EN)                  (SD_SS)PA15 | · · | PD2(BTN_EN1)
|    (BTN_ENC) PC11 | · · | PC10 (BEEPER)                   (SCK)PB3  | · · | PB4(MISO)
|                    ￣￣                                               ￣￣  
|                    EXP1                                              EXP2
*/
#if ENABLED(ULTRA_LCD)

  //EXP-1
  #if ENABLED(ULTIPANEL)
    #define LCD_PINS_D7    PC14
    #define LCD_PINS_D6    PC15
    #define LCD_PINS_D5    PB7
  #endif
  #define LCD_PINS_D4      PC13
  #define LCD_PINS_RS      PC12
  #define LCD_PINS_ENABLE  PB6
  #define BTN_ENC          PC11
  #define BEEPER_PIN       PC10

  //EXP-2
  #define SD_DETECT_PIN    PB9
  #define BTN_EN2          PB8
  #define LCD_SDSS         PA15
  #define BTN_EN1          PD2  

#endif // ULTRA_LCD



//#define USB_SD_DISABLED
#define USB_SD_ONBOARD        // Provide the onboard SD card to the host as a USB mass storage device

//#define LPC_SD_LCD            // Marlin uses the SD drive attached to the LCD
#define LPC_SD_ONBOARD        // Marlin uses the SD drive on the control board

#if ENABLED(LPC_SD_LCD)

  #define SCK_PIN          PB3
  #define MISO_PIN         PB4
  #define MOSI_PIN         PB5
  #define SS_PIN           PA15   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    PA4   // Chip select for "System" SD card

#elif ENABLED(LPC_SD_ONBOARD)

  #if ENABLED(USB_SD_ONBOARD)
    // When sharing the SD card with a PC we want the menu options to
    // mount/unmount the card and refresh it. So we disable card detect.
    #define SHARED_SD_CARD
    #undef SD_DETECT_PIN
    //#define SD_DETECT_PIN  PA3           // redefine detect pin onboard tf card
  #endif

  #define SCK_PIN          PA5
  #define MISO_PIN         PA6
  #define MOSI_PIN         PA7
  #define SS_PIN           PA4   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    PA4   // Chip select for "System" SD card

#endif

