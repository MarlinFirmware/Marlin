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

#ifndef TARGET_STM32F1
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR Mini V1.1"
#endif

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Limit Switches
//
#define X_MIN_PIN          PC2
#define X_MAX_PIN          PA2
#define Y_MIN_PIN          PC1
#define Y_MAX_PIN          PA1
#define Z_MIN_PIN          PC0
#define Z_MAX_PIN          PC3

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  PC3
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   PA2
#endif

//
// Steppers
//
#define X_STEP_PIN         PC6
#define X_DIR_PIN          PC7
#define X_ENABLE_PIN       PB15

#define Y_STEP_PIN         PB13
#define Y_DIR_PIN          PB14
#define Y_ENABLE_PIN       PB12

#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB11
#define Z_ENABLE_PIN       PB2

#define E0_STEP_PIN        PC5
#define E0_DIR_PIN         PB0
#define E0_ENABLE_PIN      PC4

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_BED_PIN       0   // A0 (T0) - (67) - TEMP_BED_PIN
#define TEMP_0_PIN         1   // A1 (T1) - (68) - TEMP_0_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA8
#define FAN_PIN            PC8
#define HEATER_BED_PIN     PC9

/*
|                 _____                                             _____
|             NC | · · | GND                                    5V | · · | GND
|          RESET | · · | PB9 (SD_DETECT)             (LCD_D7) PC14 | · · | PC15 (LCD_D6)
|   (MOSI)   PB5 | · · | PB8 (BTN_EN2)               (LCD_D5)  PB7 | · · | PC13 (LCD_D4)
|  (SD_SS)  PA15 | · · | PD2 (BTN_EN1)               (LCD_RS) PC12 | · · | PB6  (LCD_EN)
|    (SCK)   PB3 | · · | PB4 (MISO)                 (BTN_ENC) PC11 | · · | PC10 (BEEPER)
|                 ￣￣                                               ￣￣  
|                 EXP2                                              EXP1  
*/
#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN       PC10   // (37) not 5V tolerant
  #define BTN_ENC          PC11   // (58) open-drain

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    PC15

    #define BTN_EN1        PD2
    #define BTN_EN2        PB8

    #define LCD_PINS_ENABLE PC14
    #define LCD_PINS_D4    PB7

  #else
    #define LCD_PINS_RS    PC12

    #define BTN_EN1        PD2   // (31) J3-2 & AUX-4
    #define BTN_EN2        PB8   // (33) J3-4 & AUX-4

    #define LCD_PINS_ENABLE PPB6
    #define LCD_PINS_D4    PC13

    #define LCD_SDSS       PA15   // (16) J3-7 & AUX-4
    #define SD_DETECT_PIN  PB9   // (49) (NOT 5V tolerant)

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS    PB6
      #define DOGLCD_A0    PC12
      #define DOGLCD_SCK   PB3
      #define DOGLCD_MOSI  PB5
      #define FORCE_SOFT_SPI

      #define LCD_BACKLIGHT_PIN -1

      #define FORCE_SOFT_SPI      // Use this if default of hardware SPI causes display problems
                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

      #define LCD_RESET_PIN PC13   // Must be high or open for LCD to operate normally.

      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN PB7
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN PC15
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN PC14
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN    PC14
      #endif

    #else // !FYSETC_MINI_12864

      #if ENABLED(MKS_MINI_12864)
        #define DOGLCD_CS  PB7
        #define DOGLCD_A0  PC15
      #endif

      #if ENABLED(ULTIPANEL)
        #define LCD_PINS_D5 PB7
        #define LCD_PINS_D6 PC15
        #define LCD_PINS_D7 PC14
      #endif

    #endif // !FYSETC_MINI_12864

  #endif

#endif // ULTRA_LCD

//
// SD Support
//

#if !ANY(LPC_SD_LCD, LPC_SD_ONBOARD, LPC_SD_CUSTOM_CABLE)
  #undef USB_SD_DISABLED
  #define USB_SD_ONBOARD
  #define LPC_SD_LCD
#endif

#if ENABLED(LPC_SD_LCD)

  #define SCK_PIN          PB3
  #define MISO_PIN         PB4
  #define MOSI_PIN         PB5
  #define SS_PIN           PA15  // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    PA4   // Chip select for "System" SD card

#elif ENABLED(LPC_SD_ONBOARD)

  #if ENABLED(USB_SD_ONBOARD)
    // When sharing the SD card with a PC we want the menu options to
    // mount/unmount the card and refresh it. So we disable card detect.
    #define SHARED_SD_CARD
    #undef SD_DETECT_PIN
    //#define SD_DETECT_PIN  -1   // (57) open-drain
  #endif

  #define SCK_PIN          PA5
  #define MISO_PIN         PA6
  #define MOSI_PIN         PA7
  #define SS_PIN           PA15   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    PA4    // Chip select for "System" SD card

#endif

 /**
  * Special pins
  *   PC10  (37) (NOT 5V tolerant)
  *   PB9  (49) (NOT 5V tolerant)
  *   PC11  (58) (Open collector)
  */
