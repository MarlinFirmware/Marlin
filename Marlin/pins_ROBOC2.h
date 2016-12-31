/**
   Marlin 3D Printer Firmware
   Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]

   Based on Sprinter and grbl.
   Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
#error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "ROBOC2"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "RoboC2"
#endif

#ifndef DEFAULT_SOURCE_CODE_URL
  #define DEFAULT_SOURCE_CODE_URL "https://github.com/Robo3D/Marlin-C2"
#endif

#define IS_ROBOC2

#define LARGE_FLASH true


#define SERVO1_PIN          6
#define SERVO2_PIN          5
#define SERVO3_PIN          4

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#ifndef X_MAX_PIN
#define X_MAX_PIN           2
#endif

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          15
#define Y_MAX_PIN          14

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          19
#define Z_MAX_PIN          18

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FILWIDTH_PIN        -1 // ANALOG NUMBERING NOT USING IN C2

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
// define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
#define FILRUNOUT_PIN      45
#endif


#define CONTROLLERFAN_PIN  -1 // Pin used for the fan to cool controller
#define FAN_PIN             7
#define PS_ON_PIN          12


#define HEATER_0_PIN       10   // EXTRUDER 1
#define HEATER_1_PIN       9   // EXTRUDER 2 (FAN On Sprinter)

#define TEMP_0_PIN         13   // ANALOG NUMBERING default 13
#define TEMP_1_PIN         15   // ANALOG NUMBERING default 15p

#define HEATER_BED_PIN      -1    // BED

#define TEMP_BED_PIN       -1   // ANALOG NUMBERING default 14

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)

  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    #define LCD_PINS_RS     49 //CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE 51 //SID (MOSI)
    #define LCD_PINS_D4     52 //SCK (CLK) clock
  #elif ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
    #define LCD_PINS_RS 40
    #define LCD_PINS_ENABLE 42
    #define LCD_PINS_D4 65
    #define LCD_PINS_D5 66
    #define LCD_PINS_D6 44
    #define LCD_PINS_D7 64
  #else
    #define LCD_PINS_RS 16
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29
    #if DISABLED(NEWPANEL)
      #define BEEPER_PIN 33
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
      #define BEEPER_PIN 37

      #define BTN_EN1 31
      #define BTN_EN2 33
      #define BTN_ENC 35

      #define SD_DETECT_PIN 49
      #define KILL_PIN 41

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        #define LCD_PIN_BL 39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
      #define BTN_EN1 64
      #define BTN_EN2 59
      #define BTN_ENC 63
      #define SD_DETECT_PIN 42
    #elif ENABLED(LCD_I2C_PANELOLU2)
      #define BTN_EN1 47  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 43
      #define BTN_ENC 32
      #define LCD_SDSS 53
      #define SD_DETECT_PIN -1
      #define KILL_PIN 41
    #elif ENABLED(LCD_I2C_VIKI)
      #define BTN_EN1 22  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 7   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf
                          // tells about 40/42.
                          // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.
      #define BTN_ENC -1
      #define LCD_SDSS 53
      #define SD_DETECT_PIN 49
    #elif ENABLED(VIKI2) || ENABLED(miniVIKI)
      #define BEEPER_PIN       33

      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0        44
      #define DOGLCD_CS        45
      #define LCD_SCREEN_ROT_180

      #define BTN_EN1          22
      #define BTN_EN2           7
      #define BTN_ENC          39

      #define SDSS             53
      #define SD_DETECT_PIN    -1  // Pin 49 for display sd interface, 72 for easy adapter board

      #define KILL_PIN         31

      #if ENABLED(TEMP_STAT_LEDS)
        #define STAT_LED_RED   32
        #define STAT_LED_BLUE  35
      #endif
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #define BTN_EN1 35  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 37
      #define BTN_ENC 31
      #define SD_DETECT_PIN 49
      #define LCD_SDSS 53
      #define KILL_PIN 41
      #define BEEPER_PIN 23
      #define DOGLCD_CS 29
      #define DOGLCD_A0 27
      #define LCD_PIN_BL 33
    #elif ENABLED(MINIPANEL)
      #define BEEPER_PIN 42
      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0  44
      #define DOGLCD_CS  66
      #define LCD_PIN_BL 65 // backlight LED on A11/D65
      #define SDSS   53

      #define KILL_PIN 64
      // GLCD features
      //#define LCD_CONTRAST 190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
      //The encoder and click button
      #define BTN_EN1 40
      #define BTN_EN2 63
      #define BTN_ENC 59
      //not connected to a pin
      #define SD_DETECT_PIN 49

    #else

      // Beeper on AUX-4
      #define BEEPER_PIN 33

      // buttons are directly attached using AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        #define BTN_EN1 64 // encoder
        #define BTN_EN2 59 // encoder
        #define BTN_ENC 63 // enter button
        #define SHIFT_OUT 40 // shift register
        #define SHIFT_CLK 44 // shift register
        #define SHIFT_LD 42 // shift register
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1 59 // AUX2 PIN 3
        #define BTN_EN2 63 // AUX2 PIN 4
        #define BTN_ENC 49 // AUX3 PIN 7
      #else
        #define BTN_EN1 37
        #define BTN_EN2 35
        #define BTN_ENC 31  // the click
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN 49
        #define KILL_PIN 41
      #else
        //        #define SD_DETECT_PIN -1  // Ramps doesn't use this
      #endif

    #endif
  #endif // NEWPANEL

#endif // ULTRA_LCD
