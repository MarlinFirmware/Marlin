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

#include "env_validate.h"

// Onboard I2C EEPROM
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x8000  // 32KB (24C32A)
#define I2C_SCL_PIN                         PB8
#define I2C_SDA_PIN                         PB9

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

//
// Servos
#define SERVO0_PIN                          PB6

//
// Misc. Functions
//
#define LED_PIN                             PA13

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PG6   // X-STOP
#define Y_DIAG_PIN                          PG9   // Y-STOP
#define Z_DIAG_PIN                          PG10  // Z-STOP
#define Z2_DIAG_PIN                         PG11  // Z2-STOP
#define E0_DIAG_PIN                         PG12  // E0DET
#define E1_DIAG_PIN                         PG13  // E1DET
#define E2_DIAG_PIN                         PG14  // E2DET
#define E3_DIAG_PIN                         PG15  // E3DET

// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB7
#endif

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_TO_MIN
    #define X_MAX_PIN                E0_DIAG_PIN  // E0DET
  #else
    #define X_MIN_PIN                E0_DIAG_PIN  // E0DET
  #endif
#elif ENABLED(X_DUAL_ENDSTOPS)
  #ifndef X_MIN_PIN
    #define X_MIN_PIN                 X_DIAG_PIN  // X-STOP
  #endif
  #ifndef X_MAX_PIN
    #define X_MAX_PIN                E0_DIAG_PIN  // E0DET
  #endif
#else
  #define X_STOP_PIN                  X_DIAG_PIN  // X-STOP
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_TO_MIN
    #define Y_MAX_PIN                E1_DIAG_PIN  // E1DET
  #else
    #define Y_MIN_PIN                E1_DIAG_PIN  // E1DET
  #endif
#elif ENABLED(Y_DUAL_ENDSTOPS)
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                 Y_DIAG_PIN  // Y-STOP
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                E1_DIAG_PIN  // E1DET
  #endif
#else
  #define Y_STOP_PIN                  Y_DIAG_PIN  // Y-STOP
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_TO_MIN
    #define Z_MAX_PIN                E2_DIAG_PIN  // PWRDET
  #else
    #define Z_MIN_PIN                E2_DIAG_PIN  // PWRDET
  #endif
#elif ENABLED(Z_MULTI_ENDSTOPS)
  #ifndef Z_MIN_PIN
    #define Z_MIN_PIN                 Z_DIAG_PIN  // Z-STOP
  #endif
  #ifndef Z_MAX_PIN
    #define Z_MAX_PIN                E2_DIAG_PIN  // PWRDET
  #endif
#else
  #define Z_STOP_PIN                  Z_DIAG_PIN  // Z-STOP
#endif

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                      PG12  // E0DET
#define FIL_RUNOUT2_PIN                     PG13  // E1DET
#define FIL_RUNOUT3_PIN                     PG14  // E2DET
#define FIL_RUNOUT4_PIN                     PG15  // E3DET

//
// Power Supply Control
//
#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PE11  // PS-ON
#endif

//
// Power Loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PC0   // PWRDET
#endif

//
// NeoPixel LED
//
#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PB0
#endif

//
// Steppers
//
#define X_STEP_PIN                          PF13  // MOTOR 0
#define X_DIR_PIN                           PF12
#define X_ENABLE_PIN                        PF14
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC4
#endif

#define Y_STEP_PIN                          PG0   // MOTOR 1
#define Y_DIR_PIN                           PG1
#define Y_ENABLE_PIN                        PF15
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PD11
#endif

#define Z_STEP_PIN                          PF11  // MOTOR 2
#define Z_DIR_PIN                           PG3
#define Z_ENABLE_PIN                        PG5
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PC6
#endif

#define Z2_STEP_PIN                         PG4   // MOTOR 3
#define Z2_DIR_PIN                          PC1
#define Z2_ENABLE_PIN                       PA0
#ifndef Z2_CS_PIN
  #define Z2_CS_PIN                         PC7
#endif

#define E0_STEP_PIN                         PF9   // MOTOR 4
#define E0_DIR_PIN                          PF10
#define E0_ENABLE_PIN                       PG2
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PF2
#endif

#define E1_STEP_PIN                         PC13  // MOTOR 5
#define E1_DIR_PIN                          PF0
#define E1_ENABLE_PIN                       PF1
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PE4
#endif

#define E2_STEP_PIN                         PE2   // MOTOR 6
#define E2_DIR_PIN                          PE3
#define E2_ENABLE_PIN                       PD4
#ifndef E2_CS_PIN

  #define E2_CS_PIN                         PE1
#endif

#define E3_STEP_PIN                         PE6   // MOTOR 7
#define E3_DIR_PIN                          PA14
#define E3_ENABLE_PIN                       PE0
#ifndef E3_CS_PIN
  #define E3_CS_PIN                         PD3
#endif

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PF3   // TB
#if TEMP_SENSOR_0 == 20
  #define TEMP_0_PIN                        PF8   // PT100 Connector
#else
  #define TEMP_0_PIN                        PF4   // TH0
#endif
#define TEMP_1_PIN                          PF5   // TH1
#define TEMP_2_PIN                          PF6   // TH2
#define TEMP_3_PIN                          PF7   // TH3

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PA1   // Hotbed
#define HEATER_0_PIN                        PA2   // Heater0
#define HEATER_1_PIN                        PA3   // Heater1
#define HEATER_2_PIN                        PB10  // Heater2
#define HEATER_3_PIN                        PB11  // Heater3

#define FAN_PIN                             PA8   // Fan0
#define FAN1_PIN                            PE5   // Fan1
#define FAN2_PIN                            PD12  // Fan2
#define FAN3_PIN                            PD13  // Fan3
#define FAN4_PIN                            PD14  // Fan4
#define FAN5_PIN                            PD15  // Fan5

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

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PA7
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PA6
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PA5
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

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                   PC4
  #define X_SERIAL_RX_PIN                   PC4

  #define Y_SERIAL_TX_PIN                   PD11
  #define Y_SERIAL_RX_PIN                   PD11

  #define Z_SERIAL_TX_PIN                   PC6
  #define Z_SERIAL_RX_PIN                   PC6

  #define Z2_SERIAL_TX_PIN                  PC7
  #define Z2_SERIAL_RX_PIN                  PC7

  #define E0_SERIAL_TX_PIN                  PF2
  #define E0_SERIAL_RX_PIN                  PF2

  #define E1_SERIAL_TX_PIN                  PE4
  #define E1_SERIAL_RX_PIN                  PE4

  #define E2_SERIAL_TX_PIN                  PE1
  #define E2_SERIAL_RX_PIN                  PE1

  #define E3_SERIAL_TX_PIN                  PD3
  #define E3_SERIAL_RX_PIN                  PD3

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

/**
 *               ______                                              ______
 *           NC | 1  2 | GND                                     5V | 1  2 | GND
 *        RESET | 3  4 | PC15 (SD_DETECT)             (LCD_D7) PE15 | 3  4 | PE14 (LCD_D6)
 *   (MOSI) PA7 | 5  6   PB1  (BTN_EN2)               (LCD_D5) PE13 | 5  6   PE12 (LCD_D4)
 *  (SD_SS) PA4 | 7  8 | PB2  (BTN_EN1)               (LCD_RS) PE10 | 7  8 | PE9  (LCD_EN)
 *    (SCK) PA5 | 9 10 | PA6  (MISO)                 (BTN_ENC)  PE7 | 9 10 | PE8  (BEEPER)
 *               ------                                               -----
 *               EXP2                                                EXP1
 */

#define EXP1_03_PIN                         PE15
#define EXP1_04_PIN                         PE14
#define EXP1_05_PIN                         PE13
#define EXP1_06_PIN                         PE12
#define EXP1_07_PIN                         PE10
#define EXP1_08_PIN                         PE9
#define EXP1_09_PIN                         PE7
#define EXP1_10_PIN                         PE8

#define EXP2_03_PIN                         -1
#define EXP2_04_PIN                         PC15
#define EXP2_05_PIN                         PA7
#define EXP2_06_PIN                         PB2
#define EXP2_07_PIN                         PA4
#define EXP2_08_PIN                         PB1
#define EXP2_09_PIN                         PA5
#define EXP2_10_PIN                         PA6

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

  #define SDSS                              PA4
  #define SD_SS_PIN                         SDSS
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define SD_DETECT_PIN                     PC15

#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "CUSTOM_CABLE is not a supported SDCARD_CONNECTION for this board"
#endif

#if ENABLED(BTT_MOTOR_EXPANSION)
  /**
   *         ______                        ______
   *     NC | 1  2 | GND               NC | 1  2 | GND
   *     NC | 3  4 | M1EN            M2EN | 3  4 | M3EN
   *  M1STP | 5  6   M1DIR           M1RX | 5  6   M1DIAG
   *  M2DIR | 7  8 | M2STP           M2RX | 7  8 | M2DIAG
   *  M3DIR | 9 10 | M3STP           M3RX | 9 10 | M3DIAG
   *         ------                        ------
   *         EXP2                         EXP1
   */

  // M1 on Driver Expansion Module
  #define E4_STEP_PIN                EXP2_05_PIN
  #define E4_DIR_PIN                 EXP2_06_PIN
  #define E4_ENABLE_PIN              EXP2_04_PIN
  #define E4_DIAG_PIN                EXP1_06_PIN
  #define E4_CS_PIN                  EXP1_05_PIN
  #if HAS_TMC_UART
    #define E4_SERIAL_TX_PIN         EXP1_05_PIN
    #define E4_SERIAL_RX_PIN         EXP1_05_PIN
  #endif

  // M2 on Driver Expansion Module
  #define E5_STEP_PIN                EXP2_08_PIN
  #define E5_DIR_PIN                 EXP2_07_PIN
  #define E5_ENABLE_PIN              EXP1_03_PIN
  #define E5_DIAG_PIN                EXP1_08_PIN
  #define E5_CS_PIN                  EXP1_07_PIN
  #if HAS_TMC_UART
    #define E5_SERIAL_TX_PIN         EXP1_07_PIN
    #define E5_SERIAL_RX_PIN         EXP1_07_PIN
  #endif

  // M3 on Driver Expansion Module
  #define E6_STEP_PIN                EXP2_10_PIN
  #define E6_DIR_PIN                 EXP2_09_PIN
  #define E6_ENABLE_PIN              EXP1_04_PIN
  #define E6_DIAG_PIN                EXP1_10_PIN
  #define E6_CS_PIN                  EXP1_09_PIN
  #if HAS_TMC_UART
    #define E6_SERIAL_TX_PIN         EXP1_09_PIN
    #define E6_SERIAL_RX_PIN         EXP1_09_PIN
  #endif

#endif // BTT_MOTOR_EXPANSION

//
// LCDs and Controllers
//
#if IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS               EXP2_08_PIN
  #endif

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

  #else

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP2_08_PIN
    #define BTN_EN2                  EXP2_06_PIN

    #define LCD_PINS_ENABLE          EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS              EXP1_08_PIN
      #define DOGLCD_A0              EXP1_07_PIN
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
#endif  // HAS_WIRED_LCD

// Alter timing for graphical display
#if HAS_MARLINUI_U8GLIB
  #ifndef BOARD_ST7920_DELAY_1
    #define BOARD_ST7920_DELAY_1   DELAY_NS(120)  // DELAY_NS(96)
  #endif
  #ifndef BOARD_ST7920_DELAY_2
    #define BOARD_ST7920_DELAY_2   DELAY_NS(80)   // DELAY_NS(48)
  #endif
  #ifndef BOARD_ST7920_DELAY_3
    #define BOARD_ST7920_DELAY_3   DELAY_NS(580)  // DELAY_NS(600)
  #endif
#endif

//
// WIFI
//

/**
 *                      -------
 *            GND | 9  |       | 8 | 3.3V
 *  (ESP-CS) PB12 | 10 |       | 7 | PB15 (ESP-MOSI)
 *           3.3V | 11 |       | 6 | PB14 (ESP-MISO)
 * (ESP-IO0)  PD7 | 12 |       | 5 | PB13 (ESP-CLK)
 * (ESP-IO4) PD10 | 13 |       | 4 | NC
 *             NC | 14 |       | 3 | PE15 (ESP-EN)
 *  (ESP-RX)  PD8 | 15 |       | 2 | NC
 *  (ESP-TX)  PD9 | 16 |       | 1 | PE14 (ESP-RST)
 *                      -------
 *                       WIFI
 */
#define ESP_WIFI_MODULE_COM                    3  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
#define ESP_WIFI_MODULE_BAUDRATE        BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
#define ESP_WIFI_MODULE_RESET_PIN           PG7
#define ESP_WIFI_MODULE_ENABLE_PIN          PG8
#define ESP_WIFI_MODULE_GPIO0_PIN           PD7
#define ESP_WIFI_MODULE_GPIO4_PIN           PD10
