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

#if E_STEPPERS > MAX_E_STEPPERS
  #error "Marlin extruder/hotends limit! Increase MAX_E_STEPPERS to continue."
#elif HOTENDS > 8 || E_STEPPERS > 8
  #error "BIGTREE GTR V1.0 supports up to 8 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME "BTT GTR V1.0"

// Onboard I2C EEPROM
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x2000  // 8KB (24C64 ... 64Kb = 8KB)

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

#define M5_EXTENDER                               // The M5 extender is attached

//
// Servos
//
#define SERVO0_PIN                          PB11  // BLTOUCH
#define SOL0_PIN                            PC7   // Toolchanger

#if ENABLED(TOOL_SENSOR)
  #define TOOL_SENSOR1_PIN                  PH6
  #define TOOL_SENSOR2_PIN                  PI4
  //#define TOOL_SENSOR3_PIN                PF4
#else
  #define PS_ON_PIN                         PH6
#endif

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PF2   // X-
#define Y_DIAG_PIN                          PC13  // Y-
#define Z_DIAG_PIN                          PE0   // Z-
#define E0_DIAG_PIN                         PG14  // X+
#define E1_DIAG_PIN                         PG9   // Y+
#define E2_DIAG_PIN                         PD3   // Z+

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_TO_MIN
    #define X_MAX_PIN                E0_DIAG_PIN  // X+
  #else
    #define X_MIN_PIN                E0_DIAG_PIN  // X+
  #endif
#else
  #define X_MIN_PIN                   X_DIAG_PIN  // X-
  #define X_MAX_PIN                  E0_DIAG_PIN  // X+
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_TO_MIN
    #define Y_MAX_PIN                E1_DIAG_PIN  // Y+
  #else
    #define Y_MIN_PIN                E1_DIAG_PIN  // Y+
  #endif
#else
  #define Y_MIN_PIN                   Y_DIAG_PIN  // Y-
  #define Y_MAX_PIN                  E1_DIAG_PIN  // Y+
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_TO_MIN
    #define Z_MAX_PIN                E2_DIAG_PIN  // Z+
  #else
    #define Z_MIN_PIN                E2_DIAG_PIN  // Z+
  #endif
#else
  #define Z_MIN_PIN                   Z_DIAG_PIN  // Z-
  #define Z_MAX_PIN                  E2_DIAG_PIN  // Z+
#endif

//
// Pins on the extender
//
#if ENABLED(M5_EXTENDER)
  #define X2_STOP_PIN                       PI4   // M5 M1_STOP
  #define Y2_STOP_PIN                       PF12  // M5 M5_STOP
  #define Z2_STOP_PIN                       PF4   // M5 M2_STOP
  #define Z3_STOP_PIN                       PI7   // M5 M4_STOP
  #define Z4_STOP_PIN                       PF6   // M5 M3_STOP
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PH11  // Z Probe must be PH11
#endif

//
// Steppers
//
#define X_STEP_PIN                          PC15
#define X_DIR_PIN                           PF0
#define X_ENABLE_PIN                        PF1
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC14
#endif

#define Y_STEP_PIN                          PE3
#define Y_DIR_PIN                           PE2
#define Y_ENABLE_PIN                        PE4
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PE1
#endif

#define Z_STEP_PIN                          PB8
#define Z_DIR_PIN                           PB7   // PB7
#define Z_ENABLE_PIN                        PB9
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PB5
#endif

#define E0_STEP_PIN                         PG12
#define E0_DIR_PIN                          PG11
#define E0_ENABLE_PIN                       PG13
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PG10
#endif

#define E1_STEP_PIN                         PD6
#define E1_DIR_PIN                          PD5
#define E1_ENABLE_PIN                       PD7
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PD4
#endif

#define E2_STEP_PIN                         PD1
#define E2_DIR_PIN                          PD0
#define E2_ENABLE_PIN                       PD2
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PC12
#endif

#if ENABLED(M5_EXTENDER)

  #define E3_STEP_PIN                       PF3
  #define E3_DIR_PIN                        PG3
  #define E3_ENABLE_PIN                     PF8
  #ifndef E3_CS_PIN
    #define E3_CS_PIN                       PG4
  #endif

  #define E4_STEP_PIN                       PD14
  #define E4_DIR_PIN                        PD11
  #define E4_ENABLE_PIN                     PG2
  #ifndef E4_CS_PIN
    #define E4_CS_PIN                       PE15
  #endif

  #define E5_STEP_PIN                       PE12
  #define E5_DIR_PIN                        PE10
  #define E5_ENABLE_PIN                     PF14
  #ifndef E5_CS_PIN
    #define E5_CS_PIN                       PE7
  #endif

  #define E6_STEP_PIN                       PG0
  #define E6_DIR_PIN                        PG1
  #define E6_ENABLE_PIN                     PE8
  #ifndef E6_CS_PIN
    #define E6_CS_PIN                       PF15
  #endif

  #define E7_STEP_PIN                       PH12
  #define E7_DIR_PIN                        PH15
  #define E7_ENABLE_PIN                     PI0
  #ifndef E7_CS_PIN
    #define E7_CS_PIN                       PH14
  #endif

#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PG15
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PB6
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PB3
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
  //#define E3_HARDWARE_SERIAL Serial1  // M5 MOTOR 1
  //#define E4_HARDWARE_SERIAL Serial1  // M5 MOTOR 2
  //#define E5_HARDWARE_SERIAL Serial1  // M5 MOTOR 3
  //#define E6_HARDWARE_SERIAL Serial1  // M5 MOTOR 4
  //#define E7_HARDWARE_SERIAL Serial1  // M5 MOTOR 5

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                   PC14
  #define X_SERIAL_RX_PIN                   PC14

  #define Y_SERIAL_TX_PIN                   PE1
  #define Y_SERIAL_RX_PIN                   PE1

  #define Z_SERIAL_TX_PIN                   PB5
  #define Z_SERIAL_RX_PIN                   PB5

  #define E0_SERIAL_TX_PIN                  PG10
  #define E0_SERIAL_RX_PIN                  PG10

  #define E1_SERIAL_TX_PIN                  PD4
  #define E1_SERIAL_RX_PIN                  PD4

  #define E2_SERIAL_TX_PIN                  PC12
  #define E2_SERIAL_RX_PIN                  PC12

  #if ENABLED(M5_EXTENDER)
    #define E3_SERIAL_TX_PIN                PG4
    #define E3_SERIAL_RX_PIN                PG4

    #define E4_SERIAL_TX_PIN                PE15
    #define E4_SERIAL_RX_PIN                PE15

    #define E5_SERIAL_TX_PIN                PE7
    #define E5_SERIAL_RX_PIN                PE7

    #define E6_SERIAL_TX_PIN                PF15
    #define E6_SERIAL_RX_PIN                PF15

    #define E7_SERIAL_TX_PIN                PH14
    #define E7_SERIAL_RX_PIN                PH14
  #endif

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // T1 <-> E0
#define TEMP_1_PIN                          PC2   // T2 <-> E1
#define TEMP_2_PIN                          PC3   // T3 <-> E2

#if ENABLED(M5_EXTENDER)
  #define TEMP_3_PIN                        PA3   // M5 TEMP1
  #define TEMP_4_PIN                        PF9   // M5 TEMP2
  #define TEMP_5_PIN                        PF10  // M5 TEMP3
  #define TEMP_6_PIN                        PF7   // M5 TEMP4
  #define TEMP_7_PIN                        PF5   // M5 TEMP5
#endif

#define TEMP_BED_PIN                        PC0   // T0 <-> Bed

// SPI for Max6675 or Max31855 Thermocouple
// Uses a separate SPI bus
// If you have a two-way thermocouple, you can customize two THERMO_CSx_PIN pins (x:1~2)

#define THERMO_SCK_PIN                      PI1   // SCK
#define THERMO_DO_PIN                       PI2   // MISO
#define THERMO_CS1_PIN                      PH9   // GTR K-TEMP
#define THERMO_CS2_PIN                      PH2   // M5 K-TEMP

#define MAX6675_SS_PIN            THERMO_CS1_PIN
#define MAX6675_SS2_PIN           THERMO_CS2_PIN
#define MAX6675_SCK_PIN           THERMO_SCK_PIN
#define MAX6675_DO_PIN             THERMO_DO_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // Heater0
#define HEATER_1_PIN                        PA1   // Heater1
#define HEATER_2_PIN                        PB0   // Heater2

#if ENABLED(M5_EXTENDER)
  #define HEATER_3_PIN                      PD15  // M5 HEAT1
  #define HEATER_4_PIN                      PD13  // M5 HEAT2
  #define HEATER_5_PIN                      PD12  // M5 HEAT3
  #define HEATER_6_PIN                      PE13  // M5 HEAT4
  #define HEATER_7_PIN                      PI6   // M5 HEAT5
#endif

#define HEATER_BED_PIN                      PA2   // Hotbed

#define FAN_PIN                             PE5   // Fan0
#define FAN1_PIN                            PE6   // Fan1
#define FAN2_PIN                            PC8   // Fan2

#if ENABLED(M5_EXTENDER)
  #define FAN3_PIN                          PI5   // M5 FAN1
  #define FAN4_PIN                          PE9   // M5 FAN2
  #define FAN5_PIN                          PE11  // M5 FAN3
  //#define FAN6_PIN                        PC9   // M5 FAN4
  //#define FAN7_PIN                        PE14  // M5 FAN5
#endif

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

//
// By default the LCD SD (SPI2) is enabled
// Onboard SD is on a completely separate SPI bus, and requires
// overriding pins to access.
//
#if SD_CONNECTION_IS(LCD)

  #define SD_DETECT_PIN              EXP2_04_PIN
  #define SDSS                       EXP2_07_PIN

#elif SD_CONNECTION_IS(ONBOARD)

  #define SDSS                              PA4
  #define SD_SS_PIN                         SDSS
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define SD_DETECT_PIN                     PC4

#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "CUSTOM_CABLE is not a supported SDCARD_CONNECTION for this board"
#endif

/**
 *               ------                                          ------
 *           NC | 1  2 | GND                                 5V | 1  2 | GND
 *        RESET | 3  4 | PB10 (SD_DETECT)         (LCD_D7)  PG5 | 3  4 | PG6  (LCD_D6)
 *  (MOSI) PB15 | 5  6 | PH10 (BTN_EN2)           (LCD_D5)  PG7 | 5  6 | PG8  (LCD_D4)
 * (SD_SS) PB12 | 7  8 | PD10 (BTN_EN1)           (LCD_RS)  PA8 | 7  8 | PC10 (LCD_EN)
 *   (SCK) PB13 | 9 10 | PB14 (MISO)             (BTN_ENC) PA15 | 9 10 | PC11 (BEEPER)
 *               ------                                          ------
 *                EXP2                                            EXP1
 */
#define EXP1_03_PIN                         PG5
#define EXP1_04_PIN                         PG6
#define EXP1_05_PIN                         PG7
#define EXP1_06_PIN                         PG8
#define EXP1_07_PIN                         PA8
#define EXP1_08_PIN                         PC10
#define EXP1_09_PIN                         PA15
#define EXP1_10_PIN                         PC11

#define EXP2_04_PIN                         PB10
#define EXP2_05_PIN                         PB15
#define EXP2_06_PIN                         PH10
#define EXP2_07_PIN                         PB12
#define EXP2_08_PIN                         PD10
#define EXP2_09_PIN                         PB13
#define EXP2_10_PIN                         PB14

//
// LCDs and Controllers
//
#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI)

  #define TFT_CS_PIN                 EXP2_07_PIN
  #define TFT_A0_PIN                 EXP2_04_PIN
  #define TFT_SCK_PIN                EXP2_09_PIN
  #define TFT_MISO_PIN               EXP2_10_PIN
  #define TFT_MOSI_PIN               EXP2_05_PIN

  #define TOUCH_INT_PIN              EXP1_04_PIN
  #define TOUCH_MISO_PIN             EXP1_05_PIN
  #define TOUCH_MOSI_PIN             EXP1_08_PIN
  #define TOUCH_SCK_PIN              EXP1_06_PIN
  #define TOUCH_CS_PIN               EXP1_07_PIN
  #define BTN_ENC                    EXP1_09_PIN
  #define BTN_EN1                    EXP2_08_PIN
  #define BTN_EN2                    EXP2_06_PIN

#elif HAS_WIRED_LCD
  #define BEEPER_PIN                 EXP1_10_PIN
  #define BTN_ENC                    EXP1_09_PIN

  #if ENABLED(CR10_STOCKDISPLAY)
    #define LCD_PINS_RS              EXP1_04_PIN

    #define BTN_EN1                  EXP1_08_PIN
    #define BTN_EN2                  EXP1_06_PIN

    #define LCD_PINS_ENABLE          EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

    // CR10_STOCKDISPLAY default timing is too fast
    #undef BOARD_ST7920_DELAY_1
    #undef BOARD_ST7920_DELAY_2
    #undef BOARD_ST7920_DELAY_3

  #elif ENABLED(MKS_MINI_12864)
    #define DOGLCD_A0                EXP1_04_PIN
    #define DOGLCD_CS                EXP1_05_PIN
    #define BTN_EN1                  EXP2_08_PIN
    #define BTN_EN2                  EXP2_06_PIN

    #if SD_CONNECTION_IS(ONBOARD)
      #define SOFTWARE_SPI
    #endif
  #else

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP2_08_PIN
    #define BTN_EN2                  EXP2_06_PIN

    #define LCD_PINS_ENABLE          EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS              EXP1_08_PIN
      #define DOGLCD_A0              EXP1_07_PIN

      #if SD_CONNECTION_IS(ONBOARD)
        #define SOFTWARE_SPI
      #endif

      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN          EXP1_06_PIN  // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN      EXP1_05_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN      EXP1_04_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN      EXP1_03_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         EXP1_05_PIN
      #endif
    #endif // !FYSETC_MINI_12864

    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_05_PIN
      #define LCD_PINS_D6            EXP1_04_PIN
      #define LCD_PINS_D7            EXP1_03_PIN

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif

  // Alter timing for graphical display
  #if HAS_MARLINUI_U8GLIB
    #ifndef BOARD_ST7920_DELAY_1
      #define BOARD_ST7920_DELAY_1 DELAY_NS(96)
    #endif
    #ifndef BOARD_ST7920_DELAY_2
      #define BOARD_ST7920_DELAY_2 DELAY_NS(48)
    #endif
    #ifndef BOARD_ST7920_DELAY_3
      #define BOARD_ST7920_DELAY_3 DELAY_NS(600)
    #endif
  #endif

#endif // HAS_WIRED_LCD

#undef TP
#undef M5_EXTENDER
