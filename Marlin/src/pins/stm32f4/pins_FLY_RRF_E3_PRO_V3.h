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
#pragma once
#define BOARD_INFO_NAME "FLY RRF E3 V3 Pro"

#include "env_validate.h"

#if E_STEPPERS > 2
  #error "Mellow only supports 2 hotend / E stepper."
#endif

#if Z2_DRIVER_TYPE && E1_DRIVER_TYPE
 #error "You can't have both z2 and e1 enabled choose only one. Whatever your choise is the wire for the spepper motoer is connected on the e1 port"
#define BOARD_NO_NATIVE_USB
#endif
//
// Servos
//
#define SERVO0_PIN                          PB0

//
// Limit Switches
//
#ifndef X_STOP_PIN
  #define X_STOP_PIN                        PA2
#endif
#ifndef Y_STOP_PIN
  #define Y_STOP_PIN                        PB10
#endif
#ifndef Z_STOP_PIN
  #define Z_STOP_PIN                        PC4
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PC5   // BLTouch IN
#endif
#define LASER                               PB1  // For a laser ???



//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA4	   // E0
#define TEMP_1_PIN                          PA1     // E1
#define TEMP_2_PIN                          PC1     // Board Temp
#define TEMP_BED_PIN                        PA3     // Bed

//
// Heaters
//
#define HEATER_0_PIN                        PA5   // Heater0
#define HEATER_1_PIN                        PC6   // Heater1
#define HEATER_BED_PIN                      PA0   // Hotbed

//Fan
#define FAN_PIN                             PA7   // Fan0 for part cooling
#define FAN1_PIN                            PA6   // Fan1 for extruder
#ifndef USE_CONTROLLER_FAN
 #define FAN2_PIN                           PB6  // Fan2 for the board
#endif
#define FAN3_PIN                            PB7
//
// Power Supply Control
//
#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PD11  // PS-ON
#endif
//
// Power Loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PD10   // PWRDET
#endif


//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                      PD0  // E0DET
#define FIL_RUNOUT2_PIN                     PD1  // E1DET

//
// Steppers
//
#define X_STEP_PIN                          PE3  // MOTOR 0
#define X_DIR_PIN                           PE2
#define X_ENABLE_PIN                        PE5
#ifndef X_CS_PIN
  #define X_CS_PIN                          PE4
#endif

#define Y_STEP_PIN                          PB9   // MOTOR 1
#define Y_DIR_PIN                           PB8
#define Y_ENABLE_PIN                        PE1
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PE0
#endif

#define Z_STEP_PIN                          PA15  // MOTOR 2
#define Z_DIR_PIN                           PD7
#define Z_ENABLE_PIN                        PC2
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PA8
#endif

#define E0_STEP_PIN                         PD4   // MOTOR 4
#define E0_DIR_PIN                          PD3
#define E0_ENABLE_PIN                       PD6
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PD5
#endif

#ifdef E1_DRIVER_TYPE
 #define E1_STEP_PIN                         PC13  // MOTOR 5
 #define E1_DIR_PIN                          PC0
 #define E1_ENABLE_PIN                       PC15
 #ifndef E1_CS_PIN
   #define E1_CS_PIN                         PC14
 #endif
#endif

#ifdef Z2_DRIVER_TYPE
 #define Z2_STEP_PIN                         PC13   // MOTOR 3
 #define Z2_DIR_PIN                          PC0
 #define Z2_ENABLE_PIN                       PC15
 #ifndef Z2_CS_PIN
   #define Z2_CS_PIN                         PC14
 #endif
#endif

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #if HAS_WIRED_LCD
    #define SDCARD_CONNECTION                LCD
  #else
    #define SDCARD_CONNECTION            ONBOARD
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
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

  #define X_SERIAL_TX_PIN                   PE4
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PE0
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PA8
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define Z2_SERIAL_TX_PIN                  PC14
  #define Z2_SERIAL_RX_PIN      Z2_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PD5
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  #define E1_SERIAL_TX_PIN                  PC14
  #define E1_SERIAL_RX_PIN      E1_SERIAL_TX_PIN

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif


/**                 ------                                      ------
 * (BEEPER) PE11  | 1  2 | PE12 (BTN_ENC)         (MISO) PB4   | 1  2 | PB3  (SCK)
 * (LCD_EN) PA14  | 3  4 | PE6  (LCD_RS)       (BTN_EN1) PA10  | 3  4 | PB2  (SD_SS)
 * (LCD_D4) PE9     5  6 | PE10 (LCD_D5)       (BTN_EN2) PA9     5  6 | PB5  (MOSI)
 * (LCD_D6) PE7   | 7  8 | PE8  (LCD_D7)     (SD_DETECT) PA13  | 7  8 | RESET
 *           GND  | 9 10 | 5V                             GND  | 9 10 | --
 *                ------                                      ------
 *                 EXP1                                        EXP2
 */
#define EXP1_01_PIN                         PE11
#define EXP1_02_PIN                         PE12
#define EXP1_03_PIN                         PA14
#define EXP1_04_PIN                         PE6
#define EXP1_05_PIN                         PE9
#define EXP1_06_PIN                         PE10
#define EXP1_07_PIN                         PE7
#define EXP1_08_PIN                         PE8

#define EXP2_01_PIN                         PB4
#define EXP2_02_PIN                         PB3
#define EXP2_03_PIN                         PA10
#define EXP2_04_PIN                         PB2
#define EXP2_05_PIN                         PA9
#define EXP2_06_PIN                         PB5
#define EXP2_07_PIN                         PA13
#define EXP2_08_PIN                         -1

//
//
// Onboard SD card
// Must use soft SPI because Marlin's default hardware SPI is tied to LCD's EXP2
//
#if SD_CONNECTION_IS(ONBOARD)
  #define SDIO_SUPPORT                            // Use SDIO for onboard SD
  #ifndef SD_DETECT_STATE
    #define SD_DETECT_STATE HIGH
  #elif SD_DETECT_STATE == LOW
    #error "BOARD_BTT_OCTOPUS_V1_0 onboard SD requires SD_DETECT_STATE set to HIGH."
  #endif
  #define SD_DETECT_PIN                     PC14
#elif SD_CONNECTION_IS(LCD)

  #define SDSS                              PB2
  #define SD_SS_PIN                         SDSS
  #define SD_SCK_PIN                        PB3
  #define SD_MISO_PIN                       PB4
  #define SD_MOSI_PIN                       PB5
  #define SD_DETECT_PIN                     PA13

#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "CUSTOM_CABLE is not a supported SDCARD_CONNECTION for this board"
#endif
#if ENABLED(BTT_MOTOR_EXPANSION)
  /**
   *         ------                  ------
   * M3DIAG | 1  2 | M3RX     M3STP | 1  2 | M3DIR
   * M2DIAG | 3  4 | M2RX     M2STP | 3  4 | M2DIR
   * M1DIAG   5  6 | M1RX     M1DIR   5  6 | M1STP
   *   M3EN | 7  8 | M2EN      M1EN | 7  8 | --
   *    GND | 9 10 | --         GND | 9 10 | --
   *        ------                   ------
   *         EXP1                     EXP2
   */

  // M1 on Driver Expansion Module
  #define E4_STEP_PIN                EXP2_06_PIN
  #define E4_DIR_PIN                 EXP2_05_PIN
  #define E4_ENABLE_PIN              EXP2_07_PIN
  #define E4_DIAG_PIN                EXP1_05_PIN
  #define E4_CS_PIN                  EXP1_06_PIN
  #if HAS_TMC_UART
    #define E4_SERIAL_TX_PIN         EXP1_06_PIN
    #define E4_SERIAL_RX_PIN    E4_SERIAL_TX_PIN
  #endif

  // M2 on Driver Expansion Module
  #define E5_STEP_PIN                EXP2_03_PIN
  #define E5_DIR_PIN                 EXP2_04_PIN
  #define E5_ENABLE_PIN              EXP1_08_PIN
  #define E5_DIAG_PIN                EXP1_03_PIN
  #define E5_CS_PIN                  EXP1_04_PIN
  #if HAS_TMC_UART
    #define E5_SERIAL_TX_PIN         EXP1_04_PIN
    #define E5_SERIAL_RX_PIN    E5_SERIAL_TX_PIN
  #endif

  // M3 on Driver Expansion Module
  #define E6_STEP_PIN                EXP2_01_PIN
  #define E6_DIR_PIN                 EXP2_02_PIN
  #define E6_ENABLE_PIN              EXP1_07_PIN
  #define E6_DIAG_PIN                EXP1_01_PIN
  #define E6_CS_PIN                  EXP1_02_PIN
  #if HAS_TMC_UART
    #define E6_SERIAL_TX_PIN         EXP1_02_PIN
    #define E6_SERIAL_RX_PIN    E6_SERIAL_TX_PIN
  #endif

#endif // BTT_MOTOR_EXPANSION

//
// LCDs and Controllers
//
#if IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS               EXP2_03_PIN
  #endif

#elif HAS_DWIN_E3V2 || IS_DWIN_MARLINUI
  /**
   *        ------                 ------            ---
   *       | 1  2 |               | 1  2 |            1 |
   *       | 3  4 |            RX | 3  4 | TX       | 2 | RX
   *   ENT   5  6 | BEEP      ENT   5  6 | BEEP     | 3 | TX
   *     B | 7  8 | A           B | 7  8 | A        | 4 |
   *   GND | 9 10 | VCC       GND | 9 10 | VCC        5 |
   *        ------                 ------            ---
   *         EXP1                   DWIN             TFT
   *
   * DWIN pins are labeled as printed on DWIN PCB. GND, VCC, A, B, ENT & BEEP can be connected in the same
   * orientation as the existing plug/DWIN to EXP1. TX/RX need to be connected to the TFT port, with TX->RX, RX->TX.
   */

  #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
    #error "CAUTION! Ender-3 V2 display requires a custom cable. See 'pins_BTT_OCTOPUS_V1_common.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
  #endif

  #define BEEPER_PIN                 EXP1_06_PIN
  #define BTN_EN1                    EXP1_08_PIN
  #define BTN_EN2                    EXP1_07_PIN
  #define BTN_ENC                    EXP1_05_PIN

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_01_PIN
  #define BTN_ENC                    EXP1_02_PIN

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP1_03_PIN
    #define BTN_EN2                  EXP1_05_PIN

    #define LCD_PINS_ENABLE          EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

  #else

    #define LCD_PINS_RS              EXP1_04_PIN

    #define BTN_EN1                  EXP2_03_PIN
    #define BTN_EN2                  EXP2_05_PIN

    #define LCD_PINS_ENABLE          EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS              EXP1_03_PIN
      #define DOGLCD_A0              EXP1_04_PIN
      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN          EXP1_05_PIN  // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN      EXP1_06_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN      EXP1_07_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN      EXP1_08_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         EXP1_06_PIN
      #endif
    #endif // !FYSETC_MINI_12864

    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_06_PIN
      #define LCD_PINS_D6            EXP1_07_PIN
      #define LCD_PINS_D7            EXP1_08_PIN

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif
#endif  // HAS_WIRED_LCD
// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1               120
  #define BOARD_ST7920_DELAY_2                80
  #define BOARD_ST7920_DELAY_3               580
#endif

#if HAS_SPI_TFT
  #define TFT_CS_PIN                 EXP2_04_PIN
  #define TFT_A0_PIN                 EXP2_07_PIN
  #define TFT_SCK_PIN                EXP2_02_PIN
  #define TFT_MISO_PIN               EXP2_01_PIN
  #define TFT_MOSI_PIN               EXP2_06_PIN

  #define TOUCH_INT_PIN              EXP1_07_PIN
  #define TOUCH_MISO_PIN             EXP1_06_PIN
  #define TOUCH_MOSI_PIN             EXP1_03_PIN
  #define TOUCH_SCK_PIN              EXP1_05_PIN
  #define TOUCH_CS_PIN               EXP1_04_PIN

  #define BTN_EN1                    EXP2_03_PIN
  #define BTN_EN2                    EXP2_05_PIN
  #define BTN_ENC                    EXP1_02_PIN
#endif

//
// NeoPixel LED
//
#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PB11
#endif

