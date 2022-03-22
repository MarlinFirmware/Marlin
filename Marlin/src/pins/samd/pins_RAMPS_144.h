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

/**
 * AGCM4 with RAMPS v1.4.4 pin assignments
 */

#if NOT_TARGET(ARDUINO_GRAND_CENTRAL_M4)
  #error "Oops! Select 'Adafruit Grand Central M4' in 'Tools > Board.'"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "AGCM4 RAMPS 1.4.4"
#endif

//
// Servos
//
#define SERVO0_PIN                            11
#define SERVO1_PIN                             6
#define SERVO2_PIN                             5
#define SERVO3_PIN                             4

//
// EEPROM
//
//#define QSPI_EEPROM                             // Use AGCM4 onboard QSPI EEPROM (Uses 4K of RAM)
#define I2C_EEPROM                                // EEPROM on I2C-0
#define MARLIN_EEPROM_SIZE                0x8000  // 32K (24lc256)

//
// Limit Switches
//
#define X_MIN_PIN                              3
#define X_MAX_PIN                              2
#define Y_MIN_PIN                             14
#define Y_MAX_PIN                             15
#define Z_MIN_PIN                             18
#define Z_MAX_PIN                             19

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     18
#endif

//
// Steppers
//
#define X_STEP_PIN                            67  // Mega/Due:54 - AGCM4:67
#define X_DIR_PIN                             68  // Mega/Due:55 - AGCM4:68
#define X_ENABLE_PIN                          38
#ifndef X_CS_PIN
  #define X_CS_PIN                            47
#endif

#define Y_STEP_PIN                            73  // Mega/Due:60 - AGCM4:73
#define Y_DIR_PIN                             74  // Mega/Due:61 - AGCM4:74
#define Y_ENABLE_PIN                          69  // Mega/Due:56 - AGCM4:69
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            45
#endif

#define Z_STEP_PIN                            46
#define Z_DIR_PIN                             48
#define Z_ENABLE_PIN                          54  // Mega/Due:62 - AGCM4:54
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            32
#endif

#define E0_STEP_PIN                           26
#define E0_DIR_PIN                            28
#define E0_ENABLE_PIN                         24
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           43
#endif

#define E1_STEP_PIN                           36
#define E1_DIR_PIN                            34
#define E1_ENABLE_PIN                         30
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           22
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            13
#if TEMP_SENSOR_BED
  #define TEMP_BED_PIN                        14
#else
  #define TEMP_1_PIN                          14
#endif
#define TEMP_CHAMBER_PIN                      15

//
// Heaters / Fans
//
#define HEATER_0_PIN                          10
#if TEMP_SENSOR_BED
  #define HEATER_BED_PIN                       8
#else
  #define HEATER_1_PIN                         8
#endif
#define FAN_PIN                                9
#define FAN1_PIN                               7
#define FAN2_PIN                              12

//
// Misc. Functions
//
#define LED_PIN                               13

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                         5  // Analog Input on AUX2
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      70
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                           39
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !defined(CASE_LIGHT_PIN) && !defined(SPINDLE_LASER_ENA_PIN)
  #if NUM_SERVOS <= 1                             // Prefer the servo connector
    #define CASE_LIGHT_PIN                     6  // Hardware PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER && !defined(SPINDLE_LASER_ENA_PIN)
  #if !NUM_SERVOS                                 // Use servo connector if possible
    #define SPINDLE_LASER_ENA_PIN              4  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN              6  // Hardware PWM
    #define SPINDLE_DIR_PIN                    5
  #else
    #error "No auto-assignable Spindle/Laser pins available."
  #endif
#endif

//
// TMC software SPI
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                       58  // Mega/Due:66 - AGCM4:58
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                       44
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                        56  // Mega/Due:64 - AGCM4:56
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

  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN                   47
  #endif
  #ifndef X_SERIAL_RX_PIN
    #define X_SERIAL_RX_PIN      X_SERIAL_TX_PIN
  #endif
  #ifndef X2_SERIAL_TX_PIN
    #define X2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef X2_SERIAL_RX_PIN
    #define X2_SERIAL_RX_PIN    X2_SERIAL_TX_PIN
  #endif

  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN                   45
  #endif
  #ifndef Y_SERIAL_RX_PIN
    #define Y_SERIAL_RX_PIN      Y_SERIAL_TX_PIN
  #endif
  #ifndef Y2_SERIAL_TX_PIN
    #define Y2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef Y2_SERIAL_RX_PIN
    #define Y2_SERIAL_RX_PIN    Y2_SERIAL_TX_PIN
  #endif

  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN                   32
  #endif
  #ifndef Z_SERIAL_RX_PIN
    #define Z_SERIAL_RX_PIN      Z_SERIAL_TX_PIN
  #endif
  #ifndef Z2_SERIAL_TX_PIN
    #define Z2_SERIAL_TX_PIN                  22
  #endif
  #ifndef Z2_SERIAL_RX_PIN
    #define Z2_SERIAL_RX_PIN    Z2_SERIAL_TX_PIN
  #endif

  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN                  43
  #endif
  #ifndef E0_SERIAL_RX_PIN
    #define E0_SERIAL_RX_PIN    E0_SERIAL_TX_PIN
  #endif
  #ifndef E1_SERIAL_TX_PIN
    #define E1_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E1_SERIAL_RX_PIN
    #define E1_SERIAL_RX_PIN    E1_SERIAL_TX_PIN
  #endif
#endif

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SDSS                                83
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN                       95
#else
  #define SDSS                                53
#endif

//////////////////////////
// LCDs and Controllers //
//////////////////////////

#if HAS_WIRED_LCD

  //
  // LCD Display output pins
  //
  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

    // TO TEST
    //#define LCD_PINS_RS                     49  // CS chip select /SS chip slave select
    //#define LCD_PINS_ENABLE                 51  // SID (MOSI)
    //#define LCD_PINS_D4                     52  // SCK (CLK) clock

  #elif BOTH(IS_NEWPANEL, PANEL_ONE)

    // TO TEST
    //#define LCD_PINS_RS                     40
    //#define LCD_PINS_ENABLE                 42
    //#define LCD_PINS_D4                     57  // Mega/Due:65 - AGCM4:57
    //#define LCD_PINS_D5                     58  // Mega/Due:66 - AGCM4:58
    //#define LCD_PINS_D6                     44
    //#define LCD_PINS_D7                     56  // Mega/Due:64 - AGCM4:56

  #else

    #if ENABLED(CR10_STOCKDISPLAY)

      // TO TEST
      //#define LCD_PINS_RS                   27
      //#define LCD_PINS_ENABLE               29
      //#define LCD_PINS_D4                   25

      #if !IS_NEWPANEL
        // TO TEST
        //#define BEEPER_PIN                  37
      #endif

    #elif ENABLED(ZONESTAR_LCD)

      // TO TEST
      //#define LCD_PINS_RS                   56  // Mega/Due:64 - AGCM4:56
      //#define LCD_PINS_ENABLE               44
      //#define LCD_PINS_D4                   55  // Mega/Due:63 - AGCM4:55
      //#define LCD_PINS_D5                   40
      //#define LCD_PINS_D6                   42
      //#define LCD_PINS_D7                   57  // Mega/Due:65 - AGCM4:57

    #else

      #if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
        // TO TEST
        //#define LCD_PINS_DC                 25  // Set as output on init
        //#define LCD_PINS_RS                 27  // Pull low for 1s to init
        // DOGM SPI LCD Support
        //#define DOGLCD_CS                   16
        //#define DOGLCD_MOSI                 17
        //#define DOGLCD_SCK                  23
        //#define DOGLCD_A0          LCD_PINS_DC
      #else
        #define LCD_PINS_RS                   16
        #define LCD_PINS_ENABLE               17
        #define LCD_PINS_D4                   23
        #define LCD_PINS_D5                   25
        #define LCD_PINS_D6                   27
      #endif

      #define LCD_PINS_D7                     29

      #if !IS_NEWPANEL
        #define BEEPER_PIN                    33
      #endif

    #endif

    #if !IS_NEWPANEL
      // Buttons attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK_PIN                 38
      //#define SHIFT_LD_PIN                  42
      //#define SHIFT_OUT_PIN                 40
      //#define SHIFT_EN_PIN                  17
    #endif

  #endif

  //
  // LCD Display input pins
  //
  #if IS_NEWPANEL

    #if IS_RRD_SC

      #define BEEPER_PIN                      37

      #if ENABLED(CR10_STOCKDISPLAY)
        // TO TEST
        //#define BTN_EN1                     17
        //#define BTN_EN2                     23
      #else
        #define BTN_EN1                       31
        #define BTN_EN2                       33
        #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
          #define BTN_ENC_EN         LCD_PINS_D7  // Detect the presence of the encoder
        #endif
      #endif

      #define BTN_ENC                         35
      #ifndef SD_DETECT_PIN
        #define SD_DETECT_PIN                 49
      #endif
      #define KILL_PIN                        41

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        //#define LCD_BACKLIGHT_PIN           39  // TO TEST
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      // TO TEST
      //#define BTN_EN1                       56  // Mega/Due:64 - AGCM4:56
      //#define BTN_EN2                       72  // Mega/Due:59 - AGCM4:72
      //#define BTN_ENC                       55
      //#define SD_DETECT_PIN                 42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      // TO TEST
      //#define BTN_EN1                       47
      //#define BTN_EN2                       43
      //#define BTN_ENC                       32
      //#define LCD_SDSS                    SDSS
      //#define KILL_PIN                      41

    #elif ENABLED(LCD_I2C_VIKI)

      // TO TEST
      //#define BTN_EN1                       40  // https://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      //#define BTN_EN2                       42
      //#define BTN_ENC                       -1

      //#define LCD_SDSS                    SDSS
      //#define SD_DETECT_PIN                 49

    #elif ANY(VIKI2, miniVIKI)

      // TO TEST
      //#define DOGLCD_CS                     45
      //#define DOGLCD_A0                     44

      //#define BEEPER_PIN                    33
      //#define STAT_LED_RED_PIN              32
      //#define STAT_LED_BLUE_PIN             35

      //#define BTN_EN1                       22
      //#define BTN_EN2                        7
      //#define BTN_ENC                       39

      //#define SD_DETECT_PIN                 -1  // Pin 49 for display SD interface, 72 for easy adapter board
      //#define KILL_PIN                      31

      //#define LCD_SCREEN_ROTATE            180  // 0, 90, 180, 270

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)

      // TO TEST
      //#define DOGLCD_CS                     29
      //#define DOGLCD_A0                     27

      //#define BEEPER_PIN                    23
      //#define LCD_BACKLIGHT_PIN             33

      //#define BTN_EN1                       35
      //#define BTN_EN2                       37
      //#define BTN_ENC                       31

      //#define LCD_SDSS                    SDSS
      //#define SD_DETECT_PIN                 49
      //#define KILL_PIN                      41

    #elif EITHER(MKS_MINI_12864, FYSETC_MINI_12864)

      // TO TEST
      //#define BEEPER_PIN                    37
      //#define BTN_ENC                       35
      //#define SD_DETECT_PIN                 49

      //#ifndef KILL_PIN
      //  #define KILL_PIN         41
      //#endif

      #if ENABLED(MKS_MINI_12864)

        // TO TEST
        //#define DOGLCD_A0                   27
        //#define DOGLCD_CS                   25

        // not connected to a pin
        //#define LCD_BACKLIGHT_PIN           57  // backlight LED on A11/D? (Mega/Due:65 - AGCM4:57)

        //#define BTN_EN1                     31
        //#define BTN_EN2                     33

        //#define LCD_SCREEN_ROTATE          180  // 0, 90, 180, 270

      #elif ENABLED(FYSETC_MINI_12864)

        // From https://wiki.fysetc.com/Mini12864_Panel/?fbclid=IwAR1FyjuNdVOOy9_xzky3qqo_WeM5h-4gpRnnWhQr_O1Ef3h0AFnFXmCehK8

        // TO TEST
        //#define DOGLCD_A0                   16
        //#define DOGLCD_CS                   17

        //#define BTN_EN1                     33
        //#define BTN_EN2                     31

        //#define FORCE_SOFT_SPI                  // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

        //#define LCD_RESET_PIN               23  // Must be high or open for LCD to operate normally.

        #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
          #ifndef RGB_LED_R_PIN
            // TO TEST
            //#define RGB_LED_R_PIN           25
          #endif
          #ifndef RGB_LED_G_PIN
            // TO TEST
            //#define RGB_LED_G_PIN           27
          #endif
          #ifndef RGB_LED_B_PIN
            // TO TEST
            //#define RGB_LED_B_PIN           29
          #endif
        #elif ENABLED(FYSETC_MINI_12864_2_1)
          // TO TEST
          //#define NEOPIXEL_PIN              25
        #endif

      #endif

    #elif ENABLED(MINIPANEL)

      // TO TEST
      //#define BEEPER_PIN                    42
      // not connected to a pin
      //#define LCD_BACKLIGHT_PIN             57  // backlight LED on A11/D? (Mega/Due:65 - AGCM4:57)

      //#define DOGLCD_A0                     44
      //#define DOGLCD_CS                     58  // Mega/Due:66 - AGCM4:58

      //#define BTN_EN1                       40
      //#define BTN_EN2                       55  // Mega/Due:63 - AGCM4:55
      //#define BTN_ENC                       72  // Mega/Due:59 - AGCM4:72

      //#define SD_DETECT_PIN                 49
      //#define KILL_PIN                      56  // Mega/Due:64 - AGCM4:56

      //#define LCD_SCREEN_ROTATE            180  // 0, 90, 180, 270

    #elif ENABLED(ZONESTAR_LCD)

      // TO TEST
      //#define ADC_KEYPAD_PIN                12

    #elif ENABLED(AZSMZ_12864)

      // TO TEST

    #else

      // Beeper on AUX-4
      //#define BEEPER_PIN                    33

      // Buttons are directly attached to AUX-2
      #if IS_RRW_KEYPAD
        // TO TEST
        //#define SHIFT_OUT_PIN               40
        //#define SHIFT_CLK_PIN               44
        //#define SHIFT_LD_PIN                42
        //#define BTN_EN1                     56  // Mega/Due:64 - AGCM4:56
        //#define BTN_EN2                     72  // Mega/Due:59 - AGCM4:72
        //#define BTN_ENC                     55  // Mega/Due:63 - AGCM4:55
      #elif ENABLED(PANEL_ONE)
        // TO TEST
        //#define BTN_EN1                     72  // AUX2 PIN 3 (Mega/Due:59 - AGCM4:72)
        //#define BTN_EN2                     55  // AUX2 PIN 4 (Mega/Due:63 - AGCM4:55)
        //#define BTN_ENC                     49  // AUX3 PIN 7
      #else
        // TO TEST
        //#define BTN_EN1                     37
        //#define BTN_EN2                     35
        //#define BTN_ENC                     31
      #endif

      #if ENABLED(G3D_PANEL)
        // TO TEST
        //#define SD_DETECT_PIN               49
        //#define KILL_PIN                    41
      #endif

    #endif
  #endif // IS_NEWPANEL

#endif // HAS_WIRED_LCD
