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

//#define BOARD_CUSTOM_BUILD_FLAGS -DTONE_CHANNEL=4 -DTONE_TIMER=4 -DTIMER_TONE=4

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BTT SKR Mini E3 V3.0"
#endif

#define USES_DIAG_JUMPERS

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

#define LED_PIN                             PD8

// Onboard I2C EEPROM
#if EITHER(NO_EEPROM_SELECTED, I2C_EEPROM)
  #undef NO_EEPROM_SELECTED
  #define I2C_EEPROM
  #define SOFT_I2C_EEPROM                         // Force the use of Software I2C
  #define I2C_SCL_PIN                       PB6
  #define I2C_SDA_PIN                       PB7
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Servos
//
#define SERVO0_PIN                          PA1   // SERVOS

//
// Limit Switches
//
#define X_STOP_PIN                          PC0   // X-STOP
#define Y_STOP_PIN                          PC1   // Y-STOP
#define Z_STOP_PIN                          PC2   // Z-STOP

//
// Z Probe must be this pin
//
#define Z_MIN_PROBE_PIN                     PC14  // PROBE

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15  // E0-STOP
#endif

//
// Power-loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PC12  // Power Loss Detection: PWR-DET
#endif

#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PA8   // LED driving pin
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PC13  // Power Supply Control
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PB14
#define X_STEP_PIN                          PB13
#define X_DIR_PIN                           PB12

#define Y_ENABLE_PIN                        PB11
#define Y_STEP_PIN                          PB10
#define Y_DIR_PIN                           PB2

#define Z_ENABLE_PIN                        PB1
#define Z_STEP_PIN                          PB0
#define Z_DIR_PIN                           PC5

#define E0_ENABLE_PIN                       PD1
#define E0_STEP_PIN                         PB3
#define E0_DIR_PIN                          PB4

#if HAS_TMC_UART
  /**
   * TMC220x stepper drivers
   * Hardware serial communication ports
   */
  #define X_HARDWARE_SERIAL  MSerial4
  #define Y_HARDWARE_SERIAL  MSerial4
  #define Z_HARDWARE_SERIAL  MSerial4
  #define E0_HARDWARE_SERIAL MSerial4

  // Default TMC slave addresses
  #ifndef X_SLAVE_ADDRESS
    #define X_SLAVE_ADDRESS  0
  #endif
  #ifndef Y_SLAVE_ADDRESS
    #define Y_SLAVE_ADDRESS  2
  #endif
  #ifndef Z_SLAVE_ADDRESS
    #define Z_SLAVE_ADDRESS  1
  #endif
  #ifndef E0_SLAVE_ADDRESS
    #define E0_SLAVE_ADDRESS 3
  #endif
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // Analog Input "TH0"
#define TEMP_BED_PIN                        PC4   // Analog Input "TB0"

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC8   // "HE"
#define HEATER_BED_PIN                      PC9   // "HB"
#define FAN_PIN                             PC6   // "FAN0"
#define FAN1_PIN                            PC7   // "FAN1"
#define FAN2_PIN                            PB15  // "FAN2"

/**
 *              SKR Mini E3 V3.0
 *                 ------
 *             5V | 1  2 | GND
 *  (LCD_EN) PD6  | 3  4 | PB8  (LCD_RS)
 *  (LCD_D4) PB9  | 5  6   PA10 (BTN_EN2)
 *          RESET | 7  8 | PA9  (BTN_EN1)
 * (BTN_ENC) PA15 | 9 10 | PB5  (BEEPER)
 *                 ------ 
 *                  EXP1
 */
#define EXP1_09_PIN                         PA15
#define EXP1_03_PIN                         PD6

#if EITHER(DWIN_CREALITY_LCD, IS_DWIN_MARLINUI)
  /**
   *        ------              ------              ------
   *   VCC | 1  2 | GND    VCC | 1  2 | GND    GND |  2 1 | VCC
   *     A | 3  4 | B        A | 3  4 | B        B |  4 3 | A
   *       | 5  6   TX    BEEP | 5  6   ENT    ENT |  6 5 | BEEP
   *       | 7  8 | RX      TX | 7  8 | RX      RX |  8 7 | TX
   *  BEEP | 9 10 | ENT        | 9 10 |            | 10 9 |
   *        ------              ------              ------
   *         EXP1                DWIN             DWIN (plug)
   *
   * All pins are labeled as printed on DWIN PCB. Connect TX-TX, A-A and so on.
   */

  #error "DWIN_CREALITY_LCD requires a custom cable, see diagram above this line. Comment out this line to continue."

  #define BEEPER_PIN                 EXP1_09_PIN
  #define BTN_EN1                    EXP1_03_PIN
  #define BTN_EN2                           PB8
  #define BTN_ENC                           PB5

#elif HAS_WIRED_LCD

  #if ENABLED(CR10_STOCKDISPLAY)

    #define BEEPER_PIN                      PB5
    #define BTN_ENC                  EXP1_09_PIN

    #define BTN_EN1                         PA9
    #define BTN_EN2                         PA10

    #define LCD_PINS_RS                     PB8
    #define LCD_PINS_ENABLE          EXP1_03_PIN
    #define LCD_PINS_D4                     PB9

  #elif ENABLED(ZONESTAR_LCD)                     // ANET A8 LCD Controller - Must convert to 3.3V - CONNECTING TO 5V WILL DAMAGE THE BOARD!

    #error "CAUTION! ZONESTAR_LCD requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. Comment out this line to continue."

    #define LCD_PINS_RS                     PB9
    #define LCD_PINS_ENABLE          EXP1_09_PIN
    #define LCD_PINS_D4                     PB8
    #define LCD_PINS_D5                     PA10
    #define LCD_PINS_D6                     PA9
    #define LCD_PINS_D7                     PB5
    #define ADC_KEYPAD_PIN                  PA1   // Repurpose servo pin for ADC - CONNECTING TO 5V WILL DAMAGE THE BOARD!

  #elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

    #define BTN_ENC                  EXP1_09_PIN
    #define BTN_EN1                         PA9
    #define BTN_EN2                         PA10

    #define DOGLCD_CS                       PB8
    #define DOGLCD_A0                       PB9
    #define DOGLCD_SCK                      PB5
    #define DOGLCD_MOSI              EXP1_03_PIN

    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN               -1

  #elif IS_TFTGLCD_PANEL

    #if ENABLED(TFTGLCD_PANEL_SPI)

      #error "CAUTION! TFTGLCD_PANEL_SPI requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. Comment out this line to continue."

      /**
       * TFTGLCD_PANEL_SPI display pinout
       *
       *               Board                                        Display
       *               ------                                       ------
       *           5V | 1  2 | GND                (SPI1-MISO) MISO | 1  2 | SCK   (SPI1-SCK)
       * (FREE)   PB7 | 3  4 | PB8  (LCD_CS)      (PA9)     LCD_CS | 3  4 | SD_CS (PA10)
       * (FREE)   PB9 | 5  6 | PA10 (SD_CS)                 (FREE) | 5  6 | MOSI  (SPI1-MOSI)
       *        RESET | 7  8 | PA9  (MOD_RESET)   (PB5)     SD_DET | 7  8 | (FREE)
       * (BEEPER) PB6 | 9 10 | PB5  (SD_DET)                   GND | 9 10 | 5V
       *               ------                                       ------
       *                EXP1                                         EXP1
       *
       * Needs custom cable:
       *
       *    Board             Display
       *
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

      #define TFTGLCD_CS                    PA9

    #endif

  #else
    #error "Only CR10_STOCKDISPLAY, ZONESTAR_LCD, ENDER2_STOCKDISPLAY, MKS_MINI_12864, and TFTGLCD_PANEL_(SPI|I2C) are currently supported on the BIGTREE_SKR_MINI_E3."
  #endif

#endif // HAS_WIRED_LCD

#if BOTH(TOUCH_UI_FTDI_EVE, LCD_FYSETC_TFT81050)

  #error "CAUTION! LCD_FYSETC_TFT81050 requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. Comment out this line to continue."

  /**
   * FYSETC TFT TFT81050 display pinout
   *
   *               Board                                        Display
   *               ------                                       ------
   *           5V | 1  2 | GND                (SPI1-MISO) MISO | 1  2 | SCK   (SPI1-SCK)
   * (FREE)   PB7 | 3  4 | PB8  (LCD_CS)      (PA9)  MOD_RESET | 3  4 | SD_CS (PA10)
   * (FREE)   PB9 | 5  6 | PA10 (SD_CS)       (PB8)     LCD_CS | 5  6 | MOSI  (SPI1-MOSI)
   *        RESET | 7  8 | PA9  (MOD_RESET)   (PB5)     SD_DET | 7  8 | RESET
   * (BEEPER) PB6 | 9 10 | PB5  (SD_DET)                   GND | 9 10 | 5V
   *               ------                                       ------
   *                EXP1                                         EXP1
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

  #define BEEPER_PIN                 EXP1_09_PIN

  #define CLCD_MOD_RESET                    PA9
  #define CLCD_SPI_CS                       PB8

#endif // TOUCH_UI_FTDI_EVE && LCD_FYSETC_TFT81050

//
// SD Support
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                     PC3
#elif SD_CONNECTION_IS(LCD) && (BOTH(TOUCH_UI_FTDI_EVE, LCD_FYSETC_TFT81050) || IS_TFTGLCD_PANEL)
  #define SD_DETECT_PIN                     PB5
  #define SD_SS_PIN                         PA10
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "SD CUSTOM_CABLE is not compatible with SKR Mini E3."
#endif

#define ONBOARD_SPI_DEVICE                     1  // SPI1 -> used only by HAL/STM32F1...
#define ONBOARD_SD_CS_PIN                   PA4   // Chip select for "System" SD card

#define ENABLE_SPI1
#define SDSS                   ONBOARD_SD_CS_PIN
#define SD_SS_PIN              ONBOARD_SD_CS_PIN
#define SD_SCK_PIN                          PA5
#define SD_MISO_PIN                         PA6
#define SD_MOSI_PIN                         PA7
