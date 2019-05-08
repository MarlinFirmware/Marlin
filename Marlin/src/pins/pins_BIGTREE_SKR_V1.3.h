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

#ifndef TARGET_LPC1768
  #error "Oops!  Make sure you have the LPC1768 environment selected in your IDE."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "BIGTREE SKR V1.3"
#endif

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//
// Servos
//
#define SERVO0_PIN         P2_00

//
// Limit Switches
//
#define X_MIN_PIN          P1_29
#define X_MAX_PIN          P1_28
#define Y_MIN_PIN          P1_27
#define Y_MAX_PIN          P1_26
#define Z_MIN_PIN          P1_25
#define Z_MAX_PIN          P1_24

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  P1_24
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   P1_28
#endif

//
// Steppers
//
#define X_STEP_PIN         P2_02
#define X_DIR_PIN          P2_06
#define X_ENABLE_PIN       P2_01
#ifndef X_CS_PIN
  #define X_CS_PIN         P1_17
#endif

#define Y_STEP_PIN         P0_19
#define Y_DIR_PIN          P0_20
#define Y_ENABLE_PIN       P2_08
#ifndef Y_CS_PIN
  #define Y_CS_PIN         P1_15
#endif

#define Z_STEP_PIN         P0_22
#define Z_DIR_PIN          P2_11
#define Z_ENABLE_PIN       P0_21
#ifndef Z_CS_PIN
  #define Z_CS_PIN         P1_10
#endif

#define E0_STEP_PIN        P2_13
#define E0_DIR_PIN         P0_11
#define E0_ENABLE_PIN      P2_12
#ifndef E0_CS_PIN
  #define E0_CS_PIN        P1_08
#endif

#define E1_STEP_PIN        P0_01
#define E1_DIR_PIN         P0_00
#define E1_ENABLE_PIN      P0_10
#ifndef E1_CS_PIN
  #define E1_CS_PIN        P1_01
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #define TMC_SW_MOSI      P4_28
  #define TMC_SW_MISO      P0_05
  #define TMC_SW_SCK       P0_04
#endif

#if HAS_DRIVER(TMC2208)
    /**
   * TMC2208 stepper drivers
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
  #define X_SERIAL_TX_PIN  P4_29
  #define X_SERIAL_RX_PIN  P1_17

  #define Y_SERIAL_TX_PIN  P1_16
  #define Y_SERIAL_RX_PIN  P1_15

  #define Z_SERIAL_TX_PIN  P1_14
  #define Z_SERIAL_RX_PIN  P1_10

  #define E0_SERIAL_TX_PIN P1_09
  #define E0_SERIAL_RX_PIN P1_08

  #define E1_SERIAL_TX_PIN P1_04
  #define E1_SERIAL_RX_PIN P1_01

  #define Z2_SERIAL_TX_PIN P1_04
  #define Z2_SERIAL_RX_PIN P1_01

#endif

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_BED_PIN       0   // A0 (T0) - (67) - TEMP_BED_PIN
#define TEMP_0_PIN         1   // A1 (T1) - (68) - TEMP_0_PIN
#define TEMP_1_PIN         2   // A2 (T2) - (69) - TEMP_1_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN       P2_07
#if HOTENDS == 1
  #define FAN1_PIN         P2_04
#else
  #define HEATER_1_PIN     P2_04
#endif
#define FAN_PIN            P2_03
#define HEATER_BED_PIN     P2_05

/*
|               _____                                             _____
|           NC | · · | GND                                    5V | · · | GND
|        RESET | · · | 1.31(SD_DETECT)             (LCD_D7) 1.23 | · · | 1.22 (LCD_D6)
|   (MOSI)0.18 | · · | 3.25(BTN_EN2)               (LCD_D5) 1.21 | · · | 1.20 (LCD_D4)
|  (SD_SS)0.16 | · · | 3.26(BTN_EN1)               (LCD_RS) 1.19 | · · | 1.18 (LCD_EN)
|    (SCK)0.15 | · · | 0.17(MISO)                 (BTN_ENC) 0.28 | · · | 1.30 (BEEPER)
|               ￣￣                                               ￣￣
|               EXP2                                              EXP1
*/
#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN       P1_30   // (37) not 5V tolerant
  #define BTN_ENC          P0_28   // (58) open-drain

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS    P1_22

    #define BTN_EN1        P1_18
    #define BTN_EN2        P1_20

    #define LCD_PINS_ENABLE P1_23
    #define LCD_PINS_D4    P1_21

  #else
    #define LCD_PINS_RS    P1_19

    #define BTN_EN1        P3_26   // (31) J3-2 & AUX-4
    #define BTN_EN2        P3_25   // (33) J3-4 & AUX-4

    #define LCD_PINS_ENABLE P1_18
    #define LCD_PINS_D4    P1_20

    #define LCD_SDSS       P0_16   // (16) J3-7 & AUX-4
    #define SD_DETECT_PIN  P1_31   // (49) (NOT 5V tolerant)

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS    P1_18
      #define DOGLCD_A0    P1_19

      #define LCD_BACKLIGHT_PIN -1

      //#define FORCE_SOFT_SPI    // Use this if default of hardware SPI causes display problems
                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

      #define LCD_RESET_PIN P1_20   // Must be high or open for LCD to operate normally.

      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN P1_21
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN P1_22
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN P1_23
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN    P1_21
      #endif

    #else // !FYSETC_MINI_12864

      #if ENABLED(MKS_MINI_12864)
        #define DOGLCD_CS  P1_21
        #define DOGLCD_A0  P1_22
      #endif

      #if ENABLED(ULTIPANEL)
        #define LCD_PINS_D5 P1_21
        #define LCD_PINS_D6 P1_22
        #define LCD_PINS_D7 P1_23
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

  #define SCK_PIN          P0_15
  #define MISO_PIN         P0_17
  #define MOSI_PIN         P0_18
  #define SS_PIN           P0_16   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    P0_06   // Chip select for "System" SD card

#elif ENABLED(LPC_SD_ONBOARD)

  #if ENABLED(USB_SD_ONBOARD)
    // When sharing the SD card with a PC we want the menu options to
    // mount/unmount the card and refresh it. So we disable card detect.
    #define SHARED_SD_CARD
    #undef SD_DETECT_PIN
    //#define SD_DETECT_PIN  P0_27   // (57) open-drain
  #endif

  #define SCK_PIN          P0_07
  #define MISO_PIN         P0_08
  #define MOSI_PIN         P0_09
  #define SS_PIN           P0_06   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS    P0_06   // Chip select for "System" SD card

#endif

 /**
  * Special pins
  *   P1_30  (37) (NOT 5V tolerant)
  *   P1_31  (49) (NOT 5V tolerant)
  *   P0_27  (57) (Open collector)
  *   P0_28  (58) (Open collector)
  */
