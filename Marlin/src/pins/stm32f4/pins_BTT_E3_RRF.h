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

#if NOT_TARGET(STM32F4)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BTT E3 RRF"
#endif

// Add-on board for IDEX conversion
//#define BTT_E3_RRF_IDEX_BOARD

// Onboard I2C EEPROM
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE 0x1000                 // 4KB

//
// Servos
//
#define SERVO0_PIN                          PB0   // SERVOS

//
// Limit Switches
//
#define X_STOP_PIN                          PC0   // X-STOP
#define Y_STOP_PIN                          PC1   // Y-STOP
#define Z_STOP_PIN                          PC2   // Z-STOP

#if ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #if X2_USE_ENDSTOP == _XMAX_
    #define X_MAX_PIN                   FPC2_PIN  // X2-STOP
  #elif X2_USE_ENDSTOP == _XMIN_
    #define X_MIN_PIN                   FPC2_PIN  // X2-STOP
  #endif
#endif

//
// Z Probe must be this pin
//
#define Z_MIN_PROBE_PIN                     PC5   // PROBE

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC3   // E0-STOP
#endif

#if !defined(FIL1_RUNOUT2_PIN) && ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #define FIL_RUNOUT2_PIN               FPC3_PIN  // E1-STOP
#endif

//
// Power-loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PE0   // Power Loss Detection: PWR-DET
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PD7
#define X_STEP_PIN                          PD5
#define X_DIR_PIN                           PD4

#define Y_ENABLE_PIN                        PD3
#define Y_STEP_PIN                          PD0
#define Y_DIR_PIN                           PA15

#define Z_ENABLE_PIN                        PD14
#define Z_STEP_PIN                          PC6
#define Z_DIR_PIN                           PC7

#define E0_ENABLE_PIN                       PD10
#define E0_STEP_PIN                         PD12
#define E0_DIR_PIN                          PD13

#if ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #define E1_ENABLE_PIN                 FPC7_PIN  // E1EN
  #define E1_STEP_PIN                   FPC5_PIN  // E1STP
  #define E1_DIR_PIN                    FPC4_PIN  // E1DIR

  #define X2_ENABLE_PIN                FPC13_PIN  // X2EN
  #define X2_STEP_PIN                  FPC11_PIN  // X2STP
  #define X2_DIR_PIN                   FPC10_PIN  // X2DIR
#endif

/**
 * TMC2208/TMC2209 stepper drivers
 */
#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                   PD6
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PD1
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PD15
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PD11
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  #if ENABLED(BTT_E3_RRF_IDEX_BOARD)
    #define X2_SERIAL_TX_PIN           FPC12_PIN  // X2UART
    #define X2_SERIAL_RX_PIN    X2_SERIAL_TX_PIN

    #define E1_SERIAL_TX_PIN            FPC6_PIN  // E1UART
    #define E1_SERIAL_RX_PIN    E1_SERIAL_TX_PIN
  #endif

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PA1   // Analog Input "TB"
#define TEMP_0_PIN                          PA0   // Analog Input "TH0"

#if ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #define TEMP_1_PIN                    FPC9_PIN  // Analog Input "TH1"
  #define PT100_PIN                     FPC8_PIN  // Analog Input "PT100" (INA826)
#endif

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PB4   // "HB"
#define HEATER_0_PIN                        PB3   // "HE0"

#if ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #define HEATER_1_PIN                 FPC16_PIN  // "HE1"
#endif

#define FAN_PIN                             PB5   // "FAN0"

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN                PB6   // "FAN1"
#endif

#if ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #define FAN1_PIN                     FPC15_PIN  // "FAN0" in IDEX board
  #define FAN2_PIN                     FPC14_PIN  // "FAN1" in IDEX board
#else
  //#define FAN1_PIN                        PB6   // "FAN1"
#endif

//
// Misc. Functions
//
#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PB7   // LED driving pin
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PE1   // Power Supply Control
#endif

/**
 *               BTT E3 RRF
 *                 -----
 *             5V | 1 2 | GND
 *  (LCD_EN) PE11 | 3 4 | PB1  (LCD_RS)
 *  (LCD_D4) PE10 | 5 6   PB2  (BTN_EN2)
 *          RESET | 7 8 | PE7  (BTN_EN1)
 * (BTN_ENC) PE9  | 9 10| PE8  (BEEPER)
 *                 -----
 *                 EXP1
 */

#if HAS_WIRED_LCD

  #if ENABLED(CR10_STOCKDISPLAY)

    #define BEEPER_PIN                      PE8
    #define BTN_ENC                         PE9

    #define BTN_EN1                         PE7
    #define BTN_EN2                         PB2

    #define LCD_PINS_RS                     PB1
    #define LCD_PINS_ENABLE                 PE11
    #define LCD_PINS_D4                     PE10

  #elif ENABLED(ZONESTAR_LCD)                     // ANET A8 LCD Controller - Must convert to 3.3V - CONNECTING TO 5V WILL DAMAGE THE BOARD!

    #error "CAUTION! ZONESTAR_LCD requires wiring modifications. See 'pins_BTT_E3_RRF.h' for details. Comment out this line to continue."

    #define LCD_PINS_RS                     PE10
    #define LCD_PINS_ENABLE                 PE9
    #define LCD_PINS_D4                     PB1
    #define LCD_PINS_D5                     PB2
    #define LCD_PINS_D6                     PE7
    #define LCD_PINS_D7                     PE8
    #define ADC_KEYPAD_PIN                  PB0   // Repurpose servo pin for ADC - CONNECTING TO 5V WILL DAMAGE THE BOARD!

  #elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

    #define BTN_ENC                         PE9
    #define BTN_EN1                         PE7
    #define BTN_EN2                         PB2

    #define DOGLCD_CS                       PB1
    #define DOGLCD_A0                       PE10
    #define DOGLCD_SCK                      PE8
    #define DOGLCD_MOSI                     PE11

    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN               -1

  #elif IS_TFTGLCD_PANEL

    #if ENABLED(TFTGLCD_PANEL_SPI)

      #error "CAUTION! TFTGLCD_PANEL_SPI requires wiring modifications. See 'pins_BTT_E3_RRF.h' for details. Comment out this line to continue."

      /**
       * TFTGLCD_PANEL_SPI display pinout
       *
       *               Board                                      Display
       *               -----                                       -----
       *           5V | 1 2 | GND                (SPI1-MISO) MISO | 1 2 | SCK   (SPI1-SCK)
       * (FREE)  PE11 | 3 4 | PB1  (LCD_CS)      (PE7)     LCD_CS | 3 4 | SD_CS (PB2)
       * (FREE)  PE10 | 5 6 | PB2  (SD_CS)                 (FREE) | 5 6 | MOSI  (SPI1-MOSI)
       *        RESET | 7 8 | PE7  (MOD_RESET)   (PE8)     SD_DET | 7 8 | (FREE)
       * (BEEPER) PE9 | 9 10| PE8  (SD_DET)                   GND | 9 10| 5V
       *               -----                                       -----
       *                EXP1                                        EXP1
       *
       * Needs custom cable:
       *
       *    Board   Adapter   Display
       *           _________
       *   EXP1-1 ----------- EXP1-10
       *   EXP1-2 ----------- EXP1-9
       *   SPI1-4 ----------- EXP1-6
       *   EXP1-4 ----------- FREE
       *   SPI1-3 ----------- EXP1-2
       *   EXP1-6 ----------- EXP1-4
       *   EXP1-7 ----------- FREE
       *   EXP1-8 ----------- EXP1-3
       *   SPI1-1 ----------- EXP1-1
       *  EXP1-10 ----------- EXP1-7
       */

      #define TFTGLCD_CS                    PE7

    #endif

  #else
    #error "Only CR10_STOCKDISPLAY, ZONESTAR_LCD, ENDER2_STOCKDISPLAY, MKS_MINI_12864, and TFTGLCD_PANEL_(SPI|I2C) are currently supported on the BTT_E3_RRF."
  #endif

  // Alter timing for graphical display
  #if ENABLED(U8GLIB_ST7920)
    #define BOARD_ST7920_DELAY_1              96
    #define BOARD_ST7920_DELAY_2              48
    #define BOARD_ST7920_DELAY_3             600
  #endif

#endif // HAS_WIRED_LCD

#if BOTH(TOUCH_UI_FTDI_EVE, LCD_FYSETC_TFT81050)

  #error "CAUTION! LCD_FYSETC_TFT81050 requires wiring modifications. See 'pins_BTT_E3_RRF.h' for details. Comment out this line to continue."

  /** FYSETC TFT TFT81050 display pinout
   *
   *               Board                                      Display
   *               -----                                       -----
   *           5V | 1 2 | GND                (SPI1-MISO) MISO | 1 2 | SCK   (SPI1-SCK)
   * (FREE)  PE11 | 3 4 | PB1  (LCD_CS)      (PE7)  MOD_RESET | 3 4 | SD_CS (PB2)
   * (FREE)  PE10 | 5 6 | PB2  (SD_CS)       (PB1)     LCD_CS | 5 6 | MOSI  (SPI1-MOSI)
   *        RESET | 7 8 | PE7  (MOD_RESET)   (PE8)     SD_DET | 7 8 | RESET
   * (BEEPER) PE9 | 9 10| PE8  (SD_DET)                   GND | 9 10| 5V
   *               -----                                       -----
   *                EXP1                                        EXP1
   *
   * Needs custom cable:
   *
   *    Board   Adapter   Display
   *           _________
   *   EXP1-1 ----------- EXP1-10
   *   EXP1-2 ----------- EXP1-9
   *   SPI1-4 ----------- EXP1-6
   *   EXP1-4 ----------- EXP1-5
   *   SPI1-3 ----------- EXP1-2
   *   EXP1-6 ----------- EXP1-4
   *   EXP1-7 ----------- EXP1-8
   *   EXP1-8 ----------- EXP1-3
   *   SPI1-1 ----------- EXP1-1
   *  EXP1-10 ----------- EXP1-7
   */

  #define CLCD_SPI_BUS                         1  // SPI1 connector

  #define BEEPER_PIN                        PE9

  #define CLCD_MOD_RESET                    PE7
  #define CLCD_SPI_CS                       PB1

#endif // TOUCH_UI_FTDI_EVE && LCD_FYSETC_TFT81050

//
// SD Support
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SDIO_SUPPORT                            // Use SDIO for onboard SD
  #define SDIO_D0_PIN                       PC8
  #define SDIO_D1_PIN                       PC9
  #define SDIO_D2_PIN                       PC10
  #define SDIO_D3_PIN                       PC11
  #define SDIO_CK_PIN                       PC12
  #define SDIO_CMD_PIN                      PD2

  //#define SDIO_CLOCK                  48000000
  #define SD_DETECT_PIN                     PC4
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "SD CUSTOM_CABLE is not compatible with BTT E3 RRF."
#endif

//
// WIFI
//

#define ESP_WIFI_MODULE_COM                    3  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
#define ESP_WIFI_MODULE_BAUDRATE        BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
#define ESP_WIFI_MODULE_RESET_PIN           PA4
#define ESP_WIFI_MODULE_ENABLE_PIN          PA5
#define ESP_WIFI_MODULE_GPIO0_PIN           PA6

#if ENABLED(BTT_E3_RRF_IDEX_BOARD)
  #define FPC2_PIN                          PB11
  #define FPC3_PIN                          PB10
  #define FPC4_PIN                          PE12
  #define FPC5_PIN                          PE13
  #define FPC6_PIN                          PE14
  #define FPC7_PIN                          PE15
  #define FPC8_PIN                          PA3
  #define FPC9_PIN                          PA2
  #define FPC10_PIN                         PA8
  #define FPC11_PIN                         PC15
  #define FPC12_PIN                         PC14
  #define FPC13_PIN                         PC13
  #define FPC14_PIN                         PE6
  #define FPC15_PIN                         PE5
  #define FPC16_PIN                         PE4
  #define FPC17_PIN                         PE3
#endif
