/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if !defined(__STM32F4__) && !defined(STM32F4xx)
  #error "Oops! Make sure you have an STM32F4 board selected in 'Tools > Board.'"
#endif

#define PRNTR_BOARD_REV_3

/**
 * 21017 Venelin Efremov Marlin for stm32f4 test
 */

#define DEFAULT_MACHINE_NAME "STM32F4"
#ifndef PRNTR_BOARD_REV_3
#define BOARD_NAME "Marlin for PrntrBoard V1(F407)"
#else
#define BOARD_NAME "Marlin for PrntrBoard V1.3(F407)"
#endif
#define DEFAULT_WEBSITE_URL "http://blog.pcbxprt.com/index.php/category/prntrboard/"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "PRNTR Board supports up to 2 hotends / E-steppers."
#endif

// Ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

#define NUM_SERIAL 2

//
// Steppers
//
#define STEPPER_ENABLE_PIN IO_PIN(C, 1)

// Swapped E0 and X motor pins for a moment.
#define X_STEP_PIN         IO_PIN(E, 12)
#define X_DIR_PIN          IO_PIN(C, 4)
#define X_ENABLE_PIN       STEPPER_ENABLE_PIN
#define X_MIN_PIN          IO_PIN(D, 0)
#define X_MAX_PIN          IO_PIN(D, 4)

#define Y_STEP_PIN         IO_PIN(C, 5)
#define Y_DIR_PIN          IO_PIN(D, 13)
#define Y_ENABLE_PIN       STEPPER_ENABLE_PIN
#define Y_MIN_PIN          IO_PIN(D, 1)
#define Y_MAX_PIN          IO_PIN(D, 7)

#define Z_STEP_PIN         IO_PIN(B, 13)
#define Z_DIR_PIN          IO_PIN(D, 12)
#define Z_ENABLE_PIN       STEPPER_ENABLE_PIN
#define Z_MIN_PIN          IO_PIN(D, 3)
#define Z_MAX_PIN          IO_PIN(D, 8)

#define Y2_STEP_PIN        -1
#define Y2_DIR_PIN         -1
#define Y2_ENABLE_PIN      -1

#define Z2_STEP_PIN        -1
#define Z2_DIR_PIN         -1
#define Z2_ENABLE_PIN      -1

#define E0_STEP_PIN        IO_PIN(C, 7)
#define E0_DIR_PIN         IO_PIN(D, 14)
#define E0_ENABLE_PIN      STEPPER_ENABLE_PIN

#define E1_STEP_PIN        IO_PIN(E, 15)
#define E1_DIR_PIN         IO_PIN(B, 0)
#define E1_ENABLE_PIN      STEPPER_ENABLE_PIN

#define E2_STEP_PIN        -1
#define E2_DIR_PIN         -1
#define E2_ENABLE_PIN      -1

#define X_CS_PIN           SLOW_PIN(D, 9)
#define Y_CS_PIN           SLOW_PIN(D, 10)
#define Z_CS_PIN           SLOW_PIN(D, 11)
#ifndef PRNTR_BOARD_REV_3
#error "Fix ME!!!"
#define E0_CS_PIN          SLOW_PIN(B, 8)
#define E1_CS_PIN          SLOW_PIN(B, 9)
#else
#define E0_CS_PIN          SLOW_PIN(B, 1)
#define E1_CS_PIN          SLOW_PIN(D, 15)
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       IO_PIN(B, 6)   // EXTRUDER 0
#define HEATER_1_PIN       IO_PIN(B, 7)   // EXTRUDER 1
//#define HEATER_2_PIN       -1

#define HEATER_BED_PIN     IO_PIN(B, 11) // BED
//#define HEATER_BED2_PIN    -1    // BED2
//#define HEATER_BED3_PIN    -1    // BED3

#ifndef FAN_PIN
#define FAN_PIN            SLOW_PIN(B, 14)  // E0 Part
#endif
#define FAN1_PIN           SLOW_PIN(B, 15)  // E1 Part
#define FAN2_PIN           SLOW_PIN(C, 6)   // E0 Cool / TC1
#define FAN3_PIN           SLOW_PIN(B, 12)  // E1 Cool / TC2

//#define FAN_SOFT_PWM

//
// Temperature Sensors
//
#define TEMP_BED_PIN       SLOW_PIN(A, 2)
#define TEMP_0_PIN         SLOW_PIN(A, 0)
#define TEMP_1_PIN         SLOW_PIN(A, 1)
#define TEMP_2_PIN         -1

// Laser control
#if ENABLED(SPINDLE_LASER_ENABLE)
#error PRNTRboard does not support SPINDLE_LASER
//#define SPINDLE_LASER_PWM_PIN       PB8
//#define SPINDLE_LASER_ENABLE_PIN    PD5
#endif

//
// LCD Pins
//
#if ENABLED(ULTRA_LCD)

  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    //#define LCD_PINS_RS         SLOW_PIN(C, 3)   // CS chip select /SS chip slave select
    //#define LCD_PINS_ENABLE     SLOW_PIN(C, 1)   // SID (MOSI)
    //#define LCD_PINS_D4         SLOW_PIN(B, 10)  // SCK (CLK) clock
  #elif ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
    //#define LCD_PINS_RS         PB8
    //#define LCD_PINS_ENABLE     PD2
    //#define LCD_PINS_D4         PB12
    //#define LCD_PINS_D5         PB13
    //#define LCD_PINS_D6         PB14
    //#define LCD_PINS_D7         PB15
  #else
    #define LCD_PINS_RS         SLOW_PIN(C, 3)   // CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE     SLOW_PIN(C, 1)   // SID (MOSI)
    #define LCD_PINS_D4         SLOW_PIN(B, 10)  // SCK (CLK) clock
    //#define LCD_PINS_D5         PB13
    //#define LCD_PINS_D6         PB14
    //#define LCD_PINS_D7         PB15
    #if DISABLED(NEWPANEL)
      //#define BEEPER_PIN        SLOW_PIN(B, 9)   // LCD_A0
      // Buttons are attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK 38
      //#define SHIFT_LD 42
      //#define SHIFT_OUT 40
      //#define SHIFT_EN 17
    #endif
  #endif

  #if ENABLED(NEWPANEL)
    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
      #define BEEPER_PIN          IO_PIN(B, 9)

      #define BTN_EN1             IO_PIN(A, 13)
      #define BTN_EN2             IO_PIN(A, 14)
      #define BTN_ENC             IO_PIN(B, 8) //IO_PIN(C, 14)

      #define SD_DETECT_PIN       -1
      //#define KILL_PIN            IO_PIN(B, 8)   // LCD_RESET
      #define KILL_PIN          -1

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        //#define LCD_BACKLIGHT_PIN 39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      //#define BTN_EN1           64
      //#define BTN_EN2           59
      //#define BTN_ENC           63
      //#define SD_DETECT_PIN     42

    #elif ENABLED(LCD_I2C_PANELOLU2)

      //#define BTN_EN1           47
      //#define BTN_EN2           43
      //#define BTN_ENC           32
      //#define LCD_SDSS          53
      //#define SD_DETECT_PIN     -1
      //#define KILL_PIN          41

    #elif ENABLED(LCD_I2C_VIKI)
      //#define BTN_EN1           22   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf explains 40/42.
      //#define BTN_EN2            7   // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.

      //#define BTN_ENC           -1
      //#define LCD_SDSS          53
      //#define SD_DETECT_PIN     49

    #elif ENABLED(VIKI2) || ENABLED(miniVIKI)
      //#define BEEPER_PIN        33

      // Pins for DOGM SPI LCD Support
      //#define DOGLCD_A0         44
      //#define DOGLCD_CS         45
      //#define LCD_SCREEN_ROT_180

      //#define BTN_EN1           22
      //#define BTN_EN2            7
      //#define BTN_ENC           39

      //#define SDSS              53
      //#define SD_DETECT_PIN     -1   // Pin 49 for display sd interface, 72 for easy adapter board

      //#define KILL_PIN          31

      //#define STAT_LED_RED_PIN  32
      //#define STAT_LED_BLUE_PIN 35

    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      //#define BTN_EN1           35
      //#define BTN_EN2           37
      //#define BTN_ENC           31
      //#define SD_DETECT_PIN     49
      //#define LCD_SDSS          53
      //#define KILL_PIN          41
      //#define BEEPER_PIN        23
      //#define DOGLCD_CS         29
      //#define DOGLCD_A0         27
      //#define LCD_BACKLIGHT_PIN 33

    #elif ENABLED(MINIPANEL)
      //#define BEEPER_PIN        42
      // Pins for DOGM SPI LCD Support
      //#define DOGLCD_A0         44
      //#define DOGLCD_CS         66
      //#define LCD_BACKLIGHT_PIN 65   // backlight LED on A11/D65
      //#define SDSS              53

      //#define KILL_PIN          64
      // GLCD features
      //#define LCD_CONTRAST   190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
      // The encoder and click button
      //#define BTN_EN1           40
      //#define BTN_EN2           63
      //#define BTN_ENC           59
      // not connected to a pin
      //#define SD_DETECT_PIN     49

    #else
      // Beeper on AUX-4
      //#define BEEPER_PIN        33

      // buttons are directly attached using AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        //#define BTN_EN1         64
        //#define BTN_EN2         59
        //#define BTN_ENC         63
        //#define SHIFT_OUT       40
        //#define SHIFT_CLK       44
        //#define SHIFT_LD        42
      #elif ENABLED(PANEL_ONE)
        //#define BTN_EN1         59   // AUX2 PIN 3
        //#define BTN_EN2         63   // AUX2 PIN 4
        //#define BTN_ENC         49   // AUX3 PIN 7
      #else
        //#define BTN_EN1         37
        //#define BTN_EN2         35
        //#define BTN_ENC         31
      #endif

      #if ENABLED(G3D_PANEL)
        //#define SD_DETECT_PIN   49
        //#define KILL_PIN        41
      #else
        //#define SD_DETECT_PIN -1   // Ramps doesn't use this
      #endif

    #endif
  #endif // NEWPANEL

#endif // ULTRA_LCD

// Extruder filament end detectors
#define U_MIN_PIN          IO_PIN(C, 0)
#define V_MIN_PIN          IO_PIN(E, 13)
#define W_MIN_PIN          -1

#define FIL_RUNOUT_PIN     U_MIN_PIN
#define FIL_RUNOUT2_PIN    V_MIN_PIN

#define STEPPER_SPI_MOSI   SLOW_PIN(B, 5)
#define STEPPER_SPI_MISO   SLOW_PIN(B, 4)
#define STEPPER_SPI_SCK    SLOW_PIN(B, 3)

#define MOSI_PIN STEPPER_SPI_MOSI
#define MISO_PIN STEPPER_SPI_MISO
#define SCK_PIN  STEPPER_SPI_SCK

#define Z_MIN_PROBE_PIN  IO_PIN(C, 13)

// 64K SPI EEPROM
#define SPI_EEPROM
#define SPI_CHAN_EEPROM1    1
#define SPI_EEPROM1_CS     PE8
