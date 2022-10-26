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
  #define BOARD_INFO_NAME "BTT Manta M8P V1.0"
#endif

#define USES_DIAG_JUMPERS

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define SDCARD_EEPROM_EMULATION
  //#define FLASH_EEPROM_EMULATION
  //#ifndef MARLIN_EEPROM_SIZE
  //  #define MARLIN_EEPROM_SIZE 0x800U             // 2K
  //#endif
  #undef NO_EEPROM_SELECTED
#endif

//
// Servos
//
#define SERVO0_PIN                          PB1   // SERVOS

//
// Probe enable
//
#if ENABLED(PROBE_ENABLE_DISABLE)
  #ifndef PROBE_ENABLE_PIN
    #define PROBE_ENABLE_PIN         SERVO0_PIN
  #endif
#endif

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PF3   // X-STOP
#define Y_DIAG_PIN                          PF4   // Y-STOP
#define Z_DIAG_PIN                          PF5   // Z-STOP
#define Z2_DIAG_PIN                         PC0   // Z2-STOP
#define E0_DIAG_PIN                         PC1   // E0DET
#define E1_DIAG_PIN                         PC2   // E1DET

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB2   // PF6 for Proximity switch
#endif

//
// Check for additional used endstop pins
//
#if HAS_EXTRA_ENDSTOPS
  #define _ENDSTOP_IS_ANY(ES) X2_USE_ENDSTOP == ES || Y2_USE_ENDSTOP == ES || Z2_USE_ENDSTOP == ES || Z3_USE_ENDSTOP == ES || Z4_USE_ENDSTOP == ES
  #if _ENDSTOP_IS_ANY(_XMIN_) || _ENDSTOP_IS_ANY(_XMAX_)
    #define NEEDS_X_MINMAX                     1
  #endif
  #if _ENDSTOP_IS_ANY(_YMIN_) || _ENDSTOP_IS_ANY(_YMAX_)
    #define NEEDS_Y_MINMAX                     1
  #endif
  #undef _ENDSTOP_IS_ANY
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
#elif EITHER(DUAL_X_CARRIAGE, NEEDS_X_MINMAX)
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
#elif NEEDS_Y_MINMAX
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                 Y_DIAG_PIN  // Y-STOP
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                E1_DIAG_PIN  // E1DET
  #endif
#else
  #define Y_STOP_PIN                  Y_DIAG_PIN  // Y-STOP
#endif

#define Z_STOP_PIN                    Z_DIAG_PIN  // Z-STOP

#undef NEEDS_X_MINMAX
#undef NEEDS_Y_MINMAX

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                      PC1   // E0DET
#define FIL_RUNOUT2_PIN                     PC2   // E1DET

#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PC3   // Power Supply Control
#endif

//
// Steppers
//
#define X_STEP_PIN                          PE2   // MOTOR 1
#define X_DIR_PIN                           PB4
#define X_ENABLE_PIN                        PC11
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC10
#endif

#define Y_STEP_PIN                          PF12  // MOTOR 2
#define Y_DIR_PIN                           PF11
#define Y_ENABLE_PIN                        PB3
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PF13
#endif

#define Z_STEP_PIN                          PD7   // MOTOR 3
#define Z_DIR_PIN                           PD6
#define Z_ENABLE_PIN                        PF10
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PF9
#endif

#define Z2_STEP_PIN                         PD3   // MOTOR 4
#define Z2_DIR_PIN                          PD2
#define Z2_ENABLE_PIN                       PD5
#ifndef Z2_CS_PIN
  #define Z2_CS_PIN                         PD4
#endif

#define E0_STEP_PIN                         PC9   // MOTOR 5
#define E0_DIR_PIN                          PC8
#define E0_ENABLE_PIN                       PD1
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PD0
#endif

#define E1_STEP_PIN                         PA10  // MOTOR 6
#define E1_DIR_PIN                          PD15
#define E1_ENABLE_PIN                       PA15
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PF8
#endif

#define E2_STEP_PIN                         PD12  // MOTOR 7
#define E2_DIR_PIN                          PD11
#define E2_ENABLE_PIN                       PD14
#ifndef E2_CS_PIN

  #define E2_CS_PIN                         PD13
#endif

#define E3_STEP_PIN                         PD10  // MOTOR 8
#define E3_DIR_PIN                          PD8
#define E3_ENABLE_PIN                       PD9
#ifndef E3_CS_PIN
  #define E3_CS_PIN                         PC7
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
  #define X_SERIAL_TX_PIN                   PC10
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PF13
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PF9
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define Z2_SERIAL_TX_PIN                  PD4
  #define Z2_SERIAL_RX_PIN      Z2_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PD0
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  #define E1_SERIAL_TX_PIN                  PF8
  #define E1_SERIAL_RX_PIN      E1_SERIAL_TX_PIN

  #define E2_SERIAL_TX_PIN                  PD13
  #define E2_SERIAL_RX_PIN      E2_SERIAL_TX_PIN

  #define E3_SERIAL_TX_PIN                  PC7
  #define E3_SERIAL_RX_PIN      E3_SERIAL_TX_PIN

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PA0   // TB
#define TEMP_0_PIN                          PA1   // TH0
#define TEMP_1_PIN                          PA2   // TH1
#define TEMP_2_PIN                          PA3   // TH2
#define TEMP_3_PIN                          PA4   // TH3

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PB7   // Hotbed
#define HEATER_0_PIN                        PE3   // Heater0
#define HEATER_1_PIN                        PB5   // Heater1
#define HEATER_2_PIN                        PB6   // Heater2
#define HEATER_3_PIN                        PE1   // Heater3

#define FAN_PIN                             PE6   // Fan0
#define FAN1_PIN                            PE0   // Fan1
#define FAN2_PIN                            PC12  // Fan2
#define FAN3_PIN                            PE5   // Fan3

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

/**
 *                ------                                    ------
 * (BEEPER) PE9  | 1  2 | PE10 (BTN_ENC)  (MISO)      PB14 | 1  2 | PB13 (SCK)
 * (LCD_EN) PE11 | 3  4 | PE12 (LCD_RS)   (BTN_EN1)   PF7  | 3  4 | PB12 (SD_SS)
 * (LCD_D4) PE13 | 5  6   PE14 (LCD_D5)   (BTN_EN2)   PE7  | 5  6   PB11 (MOSI)
 * (LCD_D6) PE15 | 7  8 | PB10 (LCD_D7)   (SD_DETECT) PE8  | 7  8 | RESET
 *           GND | 9 10 | 5V                           GND | 9 10 | --
 *                ------                                    ------
 *                 EXP1                                      EXP2
 */
#define EXP1_01_PIN                         PE9
#define EXP1_02_PIN                         PE10
#define EXP1_03_PIN                         PE11
#define EXP1_04_PIN                         PE12
#define EXP1_05_PIN                         PE13
#define EXP1_06_PIN                         PE14
#define EXP1_07_PIN                         PE15
#define EXP1_08_PIN                         PB10

#define EXP2_01_PIN                         PB14
#define EXP2_02_PIN                         PB13
#define EXP2_03_PIN                         PF7
#define EXP2_04_PIN                         PB12
#define EXP2_05_PIN                         PE7
#define EXP2_06_PIN                         PB11
#define EXP2_07_PIN                         PE8
#define EXP2_08_PIN                         -1

//
// Onboard SD card
// Must use soft SPI because Marlin's default hardware SPI is tied to LCD's EXP2
//
#if SD_CONNECTION_IS(LCD)
  #define SDSS                       EXP2_04_PIN
  #define SD_SS_PIN                         SDSS
  #define SD_SCK_PIN                 EXP2_02_PIN
  #define SD_MISO_PIN                EXP2_01_PIN
  #define SD_MOSI_PIN                EXP2_06_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
#elif SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                     PE8
  #define SD_SCK_PIN                        PB13
  #define SD_MISO_PIN                       PB14
  #define SD_MOSI_PIN                       PB11
  #define ONBOARD_SD_CS_PIN                 PB14  // Chip select for "System" SD card
  #define SD_SS_PIN            ONBOARD_SD_CS_PIN
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

//
// LCDs and Controllers
//
#if IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS               EXP2_03_PIN
  #endif

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_01_PIN
  #define BTN_ENC                    EXP1_02_PIN

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP1_03_PIN
    #define BTN_EN2                  EXP1_05_PIN

    #define LCD_PINS_ENABLE          EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

  #elif ENABLED(MKS_MINI_12864)

    #define DOGLCD_A0                EXP1_07_PIN
    #define DOGLCD_CS                EXP1_06_PIN
    #define BTN_EN1                  EXP2_03_PIN
    #define BTN_EN2                  EXP2_05_PIN

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

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #ifndef BOARD_ST7920_DELAY_1
    #define BOARD_ST7920_DELAY_1             120
  #endif
  #ifndef BOARD_ST7920_DELAY_2
    #define BOARD_ST7920_DELAY_2              80
  #endif
  #ifndef BOARD_ST7920_DELAY_3
    #define BOARD_ST7920_DELAY_3             580
  #endif
#endif

//
// NeoPixel LED
//
#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PC6
#endif

#ifndef NEOPIXEL2_PIN
  #define NEOPIXEL2_PIN                     PA9
#endif
