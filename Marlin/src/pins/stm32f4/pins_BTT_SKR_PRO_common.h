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

#if NOT_TARGET(STM32F4)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#endif

// BigTreeTech driver expansion module https://bit.ly/3ptRRoj
//#define BTT_MOTOR_EXPANSION

#if BOTH(HAS_WIRED_LCD, BTT_MOTOR_EXPANSION)
  #error "It's not possible to have both LCD and motor expansion module on EXP1/EXP2."
#endif

// Use one of these or SDCard-based Emulation will be used
#if NO_EEPROM_SELECTED
  //#define SRAM_EEPROM_EMULATION                 // Use BackSRAM-based EEPROM emulation
  #define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#endif

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

//
// Servos
//
#define SERVO0_PIN                          PA1
#define SERVO1_PIN                          PC9

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PB10  // X-
#define Y_DIAG_PIN                          PE12  // Y-
#define Z_DIAG_PIN                          PG8   // Z-
#define E0_DIAG_PIN                         PE15  // E0
#define E1_DIAG_PIN                         PE10  // E1
#define E2_DIAG_PIN                         PG5   // E2

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_DIR < 0
    #define X_MAX_PIN                       PE15  // E0
  #else
    #define X_MIN_PIN                       PE15  // E0
  #endif
#else
  #define X_MIN_PIN                         PB10  // X-
  #define X_MAX_PIN                         PE15  // E0
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_DIR < 0
    #define Y_MAX_PIN                       PE10  // E1
  #else
    #define Y_MIN_PIN                       PE10  // E1
  #endif
#else
  #define Y_MIN_PIN                         PE12  // Y-
  #define Y_MAX_PIN                         PE10  // E1
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_DIR < 0
    #define Z_MAX_PIN                       PG5   // E2
  #else
    #define Z_MIN_PIN                       PG5   // E2
  #endif
#else
  #define Z_MIN_PIN                         PG8   // Z-
  #define Z_MAX_PIN                         PG5   // E2
#endif

//
// Z Probe must be this pin
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA2
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PE15
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PE10
#endif
#ifndef FIL_RUNOUT3_PIN
  #define FIL_RUNOUT3_PIN                   PG5
#endif

//
// Steppers
//
#define X_STEP_PIN                          PE9
#define X_DIR_PIN                           PF1
#define X_ENABLE_PIN                        PF2
#ifndef X_CS_PIN
  #define X_CS_PIN                          PA15
#endif

#define Y_STEP_PIN                          PE11
#define Y_DIR_PIN                           PE8
#define Y_ENABLE_PIN                        PD7
 #ifndef Y_CS_PIN
  #define Y_CS_PIN                          PB8
#endif

#define Z_STEP_PIN                          PE13
#define Z_DIR_PIN                           PC2
#define Z_ENABLE_PIN                        PC0
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PB9
#endif

#define E0_STEP_PIN                         PE14
#define E0_DIR_PIN                          PA0
#define E0_ENABLE_PIN                       PC3
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PB3
#endif

#define E1_STEP_PIN                         PD15
#define E1_DIR_PIN                          PE7
#define E1_ENABLE_PIN                       PA3
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PG15
#endif

#define E2_STEP_PIN                         PD13
#define E2_DIR_PIN                          PG9
#define E2_ENABLE_PIN                       PF0
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PG12
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PC12
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PC11
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PC10
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
  #define X_SERIAL_TX_PIN                   PC13
  #define X_SERIAL_RX_PIN                   PC13

  #define Y_SERIAL_TX_PIN                   PE3
  #define Y_SERIAL_RX_PIN                   PE3

  #define Z_SERIAL_TX_PIN                   PE1
  #define Z_SERIAL_RX_PIN                   PE1

  #define E0_SERIAL_TX_PIN                  PD4
  #define E0_SERIAL_RX_PIN                  PD4

  #define E1_SERIAL_TX_PIN                  PD1
  #define E1_SERIAL_RX_PIN                  PD1

  #define E2_SERIAL_TX_PIN                  PD6
  #define E2_SERIAL_RX_PIN                  PD6

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PF4   // T1 <-> E0
#define TEMP_1_PIN                          PF5   // T2 <-> E1
#define TEMP_2_PIN                          PF6   // T3 <-> E2
#define TEMP_BED_PIN                        PF3   // T0 <-> Bed

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PB1   // Heater0
#define HEATER_1_PIN                        PD14  // Heater1
#define HEATER_2_PIN                        PB0   // Heater1
#define HEATER_BED_PIN                      PD12  // Hotbed
#define FAN_PIN                             PC8   // Fan0
#define FAN1_PIN                            PE5   // Fan1
#define FAN2_PIN                            PE6

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN               FAN1_PIN
#endif

//
// Misc. Functions
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION                  LCD
#endif

/**
 *               -----                                             -----
 *           NC | 1 2 | GND                                    5V | 1 2 | GND
 *        RESET | 3 4 | PF12(SD_DETECT)             (LCD_D7)  PG7 | 3 4 | PG6  (LCD_D6)
 *   (MOSI)PB15 | 5 6   PF11(BTN_EN2)               (LCD_D5)  PG3 | 5 6   PG2  (LCD_D4)
 *  (SD_SS)PB12 | 7 8 | PG10(BTN_EN1)               (LCD_RS) PD10 | 7 8 | PD11 (LCD_EN)
 *    (SCK)PB13 | 9 10| PB14(MISO)                 (BTN_ENC)  PA8 | 9 10| PG4  (BEEPER)
 *               -----                                             -----
 *               EXP2                                              EXP1
 */

#define EXPA1_03_PIN                        PG7
#define EXPA1_04_PIN                        PG6
#define EXPA1_05_PIN                        PG3
#define EXPA1_06_PIN                        PG2
#define EXPA1_07_PIN                        PD10
#define EXPA1_08_PIN                        PD11
#define EXPA1_09_PIN                        PA8
#define EXPA1_10_PIN                        PG4

#define EXPA2_03_PIN                        -1
#define EXPA2_04_PIN                        PF12
#define EXPA2_05_PIN                        PB15
#define EXPA2_06_PIN                        PF11
#define EXPA2_07_PIN                        PB12
#define EXPA2_08_PIN                        PG10
#define EXPA2_09_PIN                        PB13
#define EXPA2_10_PIN                        PB14

//
// Onboard SD card
// Must use soft SPI because Marlin's default hardware SPI is tied to LCD's EXP2
//
#if SD_CONNECTION_IS(LCD)

  #define SD_DETECT_PIN             EXPA2_04_PIN
  #define SDSS                      EXPA2_07_PIN

#elif SD_CONNECTION_IS(ONBOARD)

  // The SKR Pro's ONBOARD SD interface is on SPI1.
  // Due to a pull resistor on the clock line, it needs to use SPI Data Mode 3 to
  // function with Hardware SPI. This is not currently configurable in the HAL,
  // so force Software SPI to work around this issue.
  #define SOFTWARE_SPI
  #define SDSS                              PA4
  #define SCK_PIN                           PA5
  #define MISO_PIN                          PA6
  #define MOSI_PIN                          PB5
  #define SD_DETECT_PIN                     PB11

#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "CUSTOM_CABLE is not a supported SDCARD_CONNECTION for this board"
#endif

#if ENABLED(BTT_MOTOR_EXPANSION)
  /**
   *               _____                                      _____
   *           NC | · · | GND                             NC | · · | GND
   *           NC | · · | PF12 (M1EN)            (M2EN)  PG7 | · · | PG6  (M3EN)
   * (M1STP) PB15 | · ·   PF11 (M1DIR)           (M1RX)  PG3 | · ·   PG2  (M1DIAG)
   * (M2DIR) PB12 | · · | PG10 (M2STP)           (M2RX) PD10 | · · | PD11 (M2DIAG)
   * (M3DIR) PB13 | · · | PB14 (M3STP)           (M3RX)  PA8 | · · | PG4  (M3DIAG)
   *               -----                                      -----
   *               EXP2                                       EXP1
   */

  // M1 on Driver Expansion Module
  #define E3_STEP_PIN               EXPA2_05_PIN
  #define E3_DIR_PIN                EXPA2_06_PIN
  #define E3_ENABLE_PIN             EXPA2_04_PIN
  #define E3_DIAG_PIN               EXPA1_06_PIN
  #define E3_CS_PIN                 EXPA1_05_PIN
  #if HAS_TMC_UART
    #define E3_SERIAL_TX_PIN        EXPA1_05_PIN
    #define E3_SERIAL_RX_PIN        EXPA1_05_PIN
  #endif

  // M2 on Driver Expansion Module
  #define E4_STEP_PIN               EXPA2_08_PIN
  #define E4_DIR_PIN                EXPA2_07_PIN
  #define E4_ENABLE_PIN             EXPA1_03_PIN
  #define E4_DIAG_PIN               EXPA1_08_PIN
  #define E4_CS_PIN                 EXPA1_07_PIN
  #if HAS_TMC_UART
    #define E4_SERIAL_TX_PIN        EXPA1_07_PIN
    #define E4_SERIAL_RX_PIN        EXPA1_07_PIN
  #endif

  // M3 on Driver Expansion Module
  #define E5_STEP_PIN               EXPA2_10_PIN
  #define E5_DIR_PIN                EXPA2_09_PIN
  #define E5_ENABLE_PIN             EXPA1_04_PIN
  #define E5_DIAG_PIN               EXPA1_10_PIN
  #define E5_CS_PIN                 EXPA1_09_PIN
  #if HAS_TMC_UART
    #define E5_SERIAL_TX_PIN        EXPA1_09_PIN
    #define E5_SERIAL_RX_PIN        EXPA1_09_PIN
  #endif

#endif // BTT_MOTOR_EXPANSION

//
// LCDs and Controllers
//
#if IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS              EXPA2_08_PIN
  #endif

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                EXPA1_10_PIN
  #define BTN_ENC                   EXPA1_09_PIN

  #if ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS             EXPA1_04_PIN

    #define BTN_EN1                 EXPA1_08_PIN
    #define BTN_EN2                 EXPA1_06_PIN

    #define LCD_PINS_ENABLE         EXPA1_03_PIN
    #define LCD_PINS_D4             EXPA1_05_PIN

    // CR10_STOCKDISPLAY default timing is too fast
    #undef BOARD_ST7920_DELAY_1
    #undef BOARD_ST7920_DELAY_2
    #undef BOARD_ST7920_DELAY_3

  #elif ENABLED(MKS_MINI_12864)

    #define DOGLCD_A0               EXPA1_04_PIN
    #define DOGLCD_CS               EXPA1_05_PIN
    #define BTN_EN1                 EXPA2_08_PIN
    #define BTN_EN2                 EXPA2_06_PIN

  #else

    #define LCD_PINS_RS             EXPA1_07_PIN

    #define BTN_EN1                 EXPA2_08_PIN
    #define BTN_EN2                 EXPA2_06_PIN

    #define LCD_PINS_ENABLE         EXPA1_08_PIN
    #define LCD_PINS_D4             EXPA1_06_PIN

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS             EXPA1_08_PIN
      #define DOGLCD_A0             EXPA1_07_PIN
      //#define LCD_BACKLIGHT_PIN           -1
      #define LCD_RESET_PIN         EXPA1_06_PIN  // Must be high or open for LCD to operate normally.
      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN     EXPA1_05_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN     EXPA1_04_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN     EXPA1_03_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN        EXPA1_05_PIN
      #endif
    #endif // !FYSETC_MINI_12864

    #if IS_ULTIPANEL
      #define LCD_PINS_D5           EXPA1_05_PIN
      #define LCD_PINS_D6           EXPA1_04_PIN
      #define LCD_PINS_D7           EXPA1_03_PIN

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if HAS_MARLINUI_U8GLIB
  #ifndef BOARD_ST7920_DELAY_1
    #define BOARD_ST7920_DELAY_1    DELAY_NS(96)
  #endif
  #ifndef BOARD_ST7920_DELAY_2
    #define BOARD_ST7920_DELAY_2    DELAY_NS(48)
  #endif
  #ifndef BOARD_ST7920_DELAY_3
    #define BOARD_ST7920_DELAY_3   DELAY_NS(600)
  #endif
#endif

//
// WIFI
//

/**
 *          -----
 *      TX | 1 2 | GND      Enable PG1   // Must be high for module to run
 *  Enable | 3 4 | GPIO2    Reset  PG0   // active low, probably OK to leave floating
 *   Reset | 5 6 | GPIO0    GPIO2  PF15  // must be high (ESP3D software configures this with a pullup so OK to leave as floating)
 *    3.3V | 7 8 | RX       GPIO0  PF14  // Leave as unused (ESP3D software configures this with a pullup so OK to leave as floating)
 *          -----
 *            W1
 */
#define ESP_WIFI_MODULE_COM                    6  // Must also set either SERIAL_PORT or SERIAL_PORT_2 to this
#define ESP_WIFI_MODULE_BAUDRATE        BAUDRATE  // Must use same BAUDRATE as SERIAL_PORT & SERIAL_PORT_2
#define ESP_WIFI_MODULE_RESET_PIN           PG0
#define ESP_WIFI_MODULE_ENABLE_PIN          PG1
#define ESP_WIFI_MODULE_GPIO0_PIN           PF14
#define ESP_WIFI_MODULE_GPIO2_PIN           PF15
