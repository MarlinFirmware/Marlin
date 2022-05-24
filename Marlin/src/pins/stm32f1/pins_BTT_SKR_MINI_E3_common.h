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

// Release PB3/PB4 (E0 STP/DIR) from JTAG pins
#define DISABLE_JTAG

#define USES_DIAG_JUMPERS

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2K
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2K
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

#define E0_ENABLE_PIN                       PD2
#define E0_STEP_PIN                         PB3
#define E0_DIR_PIN                          PB4

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // Analog Input "TH0"
#define TEMP_BED_PIN                        PC3   // Analog Input "TB0"

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC8   // "HE"
#define HEATER_BED_PIN                      PC9   // "HB"

#ifdef SKR_MINI_E3_V2
  #define FAN_PIN                           PC6
#else
  #define FAN_PIN                           PA8   // "FAN0"
#endif

//
// USB connect control
//
#ifdef SKR_MINI_E3_V2
  #define USB_CONNECT_PIN                   PA14
#else
  #define USB_CONNECT_PIN                   PC13
#endif

#define USB_CONNECT_INVERTING              false

/**
 *        SKR Mini E3 V1.0, V1.2                      SKR Mini E3 V2.0
 *                ------                                    ------
 * (BEEPER)  PB5  |10  9 | PB6 (BTN_ENC)    (BEEPER)  PB5  |10  9 | PA15 (BTN_ENC)
 * (BTN_EN1) PA9  | 8  7 | RESET            (BTN_EN1) PA9  | 8  7 | RESET
 * (BTN_EN2) PA10   6  5 | PB9  (LCD_D4)    (BTN_EN2) PA10   6  5 | PB9  (LCD_D4)
 * (LCD_RS)  PB8  | 4  3 | PB7  (LCD_EN)    (LCD_RS)  PB8  | 4  3 | PB15 (LCD_EN)
 *            GND | 2  1 | 5V                          GND | 2  1 | 5V
 *                ------                                    ------
 *                 EXP1                                      EXP1
 */
#ifdef SKR_MINI_E3_V2
  #define EXP1_9                            PA15
  #define EXP1_3                            PB15
#else
  #define EXP1_9                            PB6
  #define EXP1_3                            PB7
#endif

#if HAS_DWIN_E3V2 || IS_DWIN_MARLINUI
  /**
   *        ------                ------                ------
   * (ENT) |10  9 | (BEEP)       |10  9 |              |10  9 |
   *  (RX) | 8  7 |         (RX) | 8  7 | (TX)      RX | 8  7 | TX
   *  (TX)   6  5 |        (ENT)   6  5 | (BEEP)   ENT | 6  5 | BEEP
   *   (B) | 4  3 | (A)      (B) | 4  3 | (A)        B | 4  3 | A
   *   GND | 2  1 | (VCC)    GND | 2  1 | VCC      GND | 2  1 | VCC
   *        ------                ------                ------
   *         EXP1                  DWIN               DWIN (plug)
   *
   * All pins are labeled as printed on DWIN PCB. Connect TX-TX, A-A and so on.
   */

  #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
    #error "CAUTION! Ender-3 V2 display requires a custom cable. See 'pins_BTT_SKR_MINI_E3_common.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
  #endif

  #define BEEPER_PIN                      EXP1_9
  #define BTN_EN1                         EXP1_3
  #define BTN_EN2                           PB8
  #define BTN_ENC                           PB5

#elif HAS_WIRED_LCD

  #if ENABLED(CR10_STOCKDISPLAY)

    #define BEEPER_PIN                      PB5
    #define BTN_ENC                       EXP1_9

    #define BTN_EN1                         PA9
    #define BTN_EN2                         PA10

    #define LCD_PINS_RS                     PB8
    #define LCD_PINS_ENABLE               EXP1_3
    #define LCD_PINS_D4                     PB9

  #elif ENABLED(ZONESTAR_LCD)                     // ANET A8 LCD Controller - Must convert to 3.3V - CONNECTING TO 5V WILL DAMAGE THE BOARD!

    #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
      #error "CAUTION! ZONESTAR_LCD requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
    #endif

    #define LCD_PINS_RS                     PB9
    #define LCD_PINS_ENABLE               EXP1_9
    #define LCD_PINS_D4                     PB8
    #define LCD_PINS_D5                     PA10
    #define LCD_PINS_D6                     PA9
    #define LCD_PINS_D7                     PB5
    #define ADC_KEYPAD_PIN                  PA1   // Repurpose servo pin for ADC - CONNECTING TO 5V WILL DAMAGE THE BOARD!

  #elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

    #define BTN_ENC                       EXP1_9
    #define BTN_EN1                         PA9
    #define BTN_EN2                         PA10

    #define DOGLCD_CS                       PB8
    #define DOGLCD_A0                       PB9
    #define DOGLCD_SCK                      PB5
    #define DOGLCD_MOSI                   EXP1_3

    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN               -1

  #elif IS_TFTGLCD_PANEL

    #if ENABLED(TFTGLCD_PANEL_SPI)

      #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
        #error "CAUTION! TFTGLCD_PANEL_SPI requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
      #endif

      /**
       * TFTGLCD_PANEL_SPI display pinout
       *
       *                   Board                        Display
       *                   ------                        ------
       * (SD_DET)    PB5  |10  9 | PB6 (BEEPER)      5V |10  9 | GND
       * (MOD_RESET) PA9  | 8  7 | RESET             -- | 8  7 | (SD_DET)
       * (SD_CS)     PA10   6  5 | PB9          (MOSI)  | 6  5 | --
       * (LCD_CS)    PB8  | 4  3 | PB7          (SD_CS) | 4  3 | (LCD_CS)
       *              GND | 2  1 | 5V           (SCK)   | 2  1 | (MISO)
       *                   ------                        ------
       *                    EXP1                          EXP1
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

  #elif ENABLED(FYSETC_MINI_12864_2_1)

    #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
      #error "CAUTION! FYSETC_MINI_12864_2_1 / MKS_MINI_12864_V3 / BTT_MINI_12864_V1 requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
    #endif

    /**
     * FYSETC_MINI_12864_2_1 / MKS_MINI_12864_V3 / BTT_MINI_12864_V1 display pinout
     *
     *       Board                      Display
     *       ------                     ------
     * PB5  |10  9 | PA15       (BEEP) |10  9 | BTN_ENC
     * PA9  | 8  7 | RESET      LCD_CS | 8  7 | LCD A0
     * PA10 | 6  5 | PB9       LCD_RST | 6  5 | RED
     * PB8  | 4  3 | PB15      (GREEN) | 4  3 | (BLUE)
     * GND  | 2  1 | 5V            GND | 2  1 | 5V
     *       ------                     ------
     *        EXP1                       EXP1
     *
     *            ---                   ------
     *       RST | 1 |          (MISO) |10  9 | SCK
     * (RX2) PA2 | 2 |         BTN_EN1 | 8  7 | (SS)
     * (TX2) PA3 | 3 |         BTN_EN2 | 6  5 | MOSI
     *       GND | 4 |            (CD) | 4  3 | (RST)
     *        5V | 5 |           (GND) | 2  1 | (KILL)
     *            ---                   ------
     *            TFT                    EXP2
     *
     * Needs custom cable:
     *
     *    Board             Display
     *
     *   EXP1-1 ----------- EXP1-1
     *   EXP1-2 ----------- EXP1-2
     *   EXP1-3 ----------- EXP2-6
     *   EXP1-4 ----------- EXP1-5
     *   EXP1-5 ----------- EXP2-8
     *   EXP1-6 ----------- EXP1-6
     *   EXP1-8 ----------- EXP1-8
     *   EXP1-9 ----------- EXP1-9
     *  EXP1-10 ----------- EXP1-7
     *
     *    TFT-2 ----------- EXP2-9
     *    TFT-3 ----------- EXP2-5
     *
     * for backlight configuration see steps 2 (V2.1) and 3 in https://wiki.fysetc.com/Mini12864_Panel/
     */

    #define LCD_PINS_RS                     PA9     // CS
    #define LCD_PINS_ENABLE                 PA3     // MOSI
    #define LCD_BACKLIGHT_PIN               -1
    #define NEOPIXEL_PIN                    PB8
    #define LCD_CONTRAST                    255
    #define LCD_RESET_PIN                   PA10

    #define DOGLCD_CS                       PA9
    #define DOGLCD_A0                       PB5
    #define DOGLCD_SCK                      PA2
    #define DOGLCD_MOSI                     PA3

    #define BTN_ENC                         PA15
    #define BTN_EN1                         PB9
    #define BTN_EN2                         PB15

    #define FORCE_SOFT_SPI

  #else
    #error "Only CR10_STOCKDISPLAY, ZONESTAR_LCD, ENDER2_STOCKDISPLAY, MKS_MINI_12864, TFTGLCD_PANEL_(SPI|I2C), FYSETC_MINI_12864_2_1, MKS_MINI_12864_V3, and BTT_MINI_12864_V1 are currently supported on the BIGTREE_SKR_MINI_E3."
  #endif

#endif // HAS_WIRED_LCD

#if BOTH(TOUCH_UI_FTDI_EVE, LCD_FYSETC_TFT81050)

  #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
    #error "CAUTION! LCD_FYSETC_TFT81050 requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
  #endif

  /**
   * FYSETC TFT TFT81050 display pinout
   *
   *                   Board                            Display
   *                   ------                           ------
   * (SD_DET)    PB5  |10  9 | PB6 (BEEPER)         5V |10  9 | GND
   * (MOD_RESET) PA9  | 8  7 | RESET           (RESET) | 8  7 | (SD_DET)
   * (SD_CS)     PA10   6  5 | PB9             (MOSI)  | 6  5 | (LCD_CS)
   * (LCD_CS)    PB8  | 4  3 | PB7             (SD_CS) | 4  3 | (MOD_RESET)
   *              GND | 2  1 | 5V              (SCK)   | 2  1 | (MISO)
   *                   ------                           ------
   *                    EXP1                             EXP1
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

  #define BEEPER_PIN                      EXP1_9

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
  #define SD_DETECT_PIN                     PC4
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
#define SD_SCK_PIN                          PA5
#define SD_MISO_PIN                         PA6
#define SD_MOSI_PIN                         PA7
