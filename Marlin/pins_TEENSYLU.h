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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.   If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 *  rev B    11 DEC 2016
 *
 *  The original version of this file did NOT result in a useful program because:
 *   1. The pin numbers assumed that the "#define AT90USBxx_TEENSYPP_ASSIGNMENTS" line
 *      in FASTIO.h was commented out. The only currently available library that 
 *      supports the AT90USBxxx chips is "Teensyduino" from PJRC http://www.pjrc.com .
 *      That library uses a different pin map. The result was a program that couldn't
 *      control the hardware correctly (changing X direction might turn on a heater FET,
 *      etc.) 
 *   2. The silkscreen for the four end stops don't agree with the schematic. Activating
 *      the X endstop would tell the software that the Y endstop just went active.
 *   3. The thermistor inputs also had heater names assigned to them. The result was
 *      thermistor inputs that were set to digital outputs. 
 *
 *  Rev B corrects the above problems by:
 *   1. A section was added that uses the TEENSYPP_ASSIGNMENTS for pin numbers.  The 
 *      flag "AT90USBxx_TEENSYPP_ASSIGNMENTS" in FASTIO selects which section to use.
 *   2. The endstop pin numbers in this file were changed to match the silkscreen.  This
 *      makes it a little confusing when trying to correlate the schematic with the pin
 *      numbers used in this file.
 *   3. The offending heater names were deleted.
 *
 *  At the very end of this file is a table showing the two pin mappings, the signal
 *  names in this file, the signal names in the schematic and the signal names on the
 *  the silkscreen that don't match the schematic.
 *  
 *  To create a useable image for Teensylu do the following:
 *   a) Go to "https://github.com/scwimbush/Printrboard-HID-Arduino-IDE-Support",
 *      click on the "Clone or Download" button and then click on "Download ZIP" button.
 *      This download contains the "Teensyduino" library from PJRC http://www.pjrc.com, a
 *      bootloader that supports 128K downloads and a boards.txt file that is setup to
 *      use the bootloader.
 *   b) Download, if needed, a version of the Arduino IDE that is supported by the
 *      "Teensyduino" library.  Supported versions are listed on the "Teensyduino"
 *      download page.
 *   c) Unzip the file, find the "printrboard" directory and then copy it to the
 *      hardware directory in Arduino.  The Arduino hardware directory will probably be
 *      located in a path similar to this: C:\Program Files (x86)\Arduino\hardware
 *   d) Connect the USBtinyISP to the board.
 *   e) In the Arduino IDE select the Printrboard and select the USBtinyISP programmer.
 *   f) In the Arduino IDE click on "burn bootloader".  Don't worry about the "verify
 *      failed at 1F000" error message.
 *   g) The USBtinyISP programmer is no longer needed.  Remove it from the Teensylu.
 *   h) In FASTIO.h uncomment the "#define AT90USBxx_TEENSYPP_ASSIGNMENTS" line.
 *   i) To upload a sketch do the following:
 *       1. remove the jumper
 *       2. press reset
 *       3. click on the "upload" button in the Arduino IDE
 *       4. wait until the upload finishes (less than a minute)
 *       5. put the jumper back on
 *       6. press the reset button
 *
 */

 /**
  *  SILKSCREEN ERROR
  *
  *  The silkscreen for the endstops do NOT match the schematic.  The silkscreen SHOULD
  *  read (from left to right) X-STOP, Y-STOP, Z-STOP & E-STOP.  The silkscreen actually
  *  reads                     E-STOP, X-STOP, Y-STOP & Z-STOP.
  *
  *  The pin assignments in this file match the silkscreen.
  */


#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "Teensylu"

#define USBCON 1286  // Disable MarlinSerial etc.
#define LARGE_FLASH        true


#if !ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)   // use Marlin traditional pin assignments instead of Teensy++


  //
  // Limit Switche definitions that match the schematic
  //
  //#define X_STOP_PIN              13  // 25 Teensy++ pin #
  //#define Y_STOP_PIN              14  // 26 Teensy++ pin #
  //#define Z_STOP_PIN              15  // 27 Teensy++ pin #
  //#define E_STOP_PIN              36  // 36 Teensy++ pin #


  //
  // Limit Switch definitions that match the silkscreen
  //
  #define X_STOP_PIN              14  // 26 Teensy++ pin #
  #define Y_STOP_PIN              15  // 27 Teensy++ pin #
  #define Z_STOP_PIN              36  // 36 Teensy++ pin #
  //#define E_STOP_PIN              13  // 25 Teensy++ pin #

  //
  // Steppers
  //
  #define X_STEP_PIN               0  // 28 Teensy++ pin #
  #define X_DIR_PIN                1  // 29 Teensy++ pin #
  #define X_ENABLE_PIN            39  // 19 Teensy++ pin #

  #define Y_STEP_PIN               2  // 30 Teensy++ pin #
  #define Y_DIR_PIN                3  // 31 Teensy++ pin #
  #define Y_ENABLE_PIN            38  // 18 Teensy++ pin #

  #define Z_STEP_PIN               4  // 32 Teensy++ pin #
  #define Z_DIR_PIN                5  // 33 Teensy++ pin #
  #define Z_ENABLE_PIN            23  // 17 Teensy++ pin #

  #define E0_STEP_PIN              6  // 34 Teensy++ pin #
  #define E0_DIR_PIN               7  // 35 Teensy++ pin #
  #define E0_ENABLE_PIN           19  // 13 Teensy++ pin #


  // Temperature Sensors

  #define TEMP_0_PIN               7  // Analog Input (Extruder)
  #define TEMP_BED_PIN             6  // Analog Input (Bed)

  //
  // Heaters / Fans
  //
  #define HEATER_0_PIN            21  // Extruder   15 Teensy++ pin #
  #define HEATER_BED_PIN          20  // 14 Teensy++ pin #

  #define FAN_PIN                 22  // 16 Teensy++ pin #

  //
  // Misc. Functions
  //
  #define SDSS                     8  // 20 Teensy++ pin #

  //
  // LCD / Controller
  //
  #if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

    #define BEEPER_PIN       -1

    #if ENABLED(LCD_I2C_PANELOLU2)
      #define BTN_EN1                 27  // 3 Teensy++ pin #
      #define BTN_EN2                 26  // 2 Teensy++ pin #
      #define BTN_ENC                 43  // 41 Teensy++ pin #
      #define SDSS                    40  // use SD card on Panelolu2   38 Teensy++ pin #
    #endif // LCD_I2C_PANELOLU2

    #define SD_DETECT_PIN    -1

  #endif // ULTRA_LCD && NEWPANEL

  // added 9 DEC 2016
  #define SPINDLE_SPEED_PIN       12  // 24 Teensy++ pin #
  #define SPINDLE_ENABLE_PIN      41  // should have a pullup resistor on this pin  39 Teensy++ pin #
  #define SPINDLE_DIR_PIN         42  // 40 Teensy++ pin #

  // misc
  #define CASE_LIGHT_PIN          24  // 0 Teensy++ pin #

#else    // use Teensyduino Teensy++2.0 pin assignments instead of Marlin traditional.

  //
  // Limit Switche definitions that match the schematic
  //
  //#define X_STOP_PIN              25  // 13 AT90USB… pin #
  //#define Y_STOP_PIN              26  // 14 AT90USB… pin #
  //#define Z_STOP_PIN              27  // 15 AT90USB… pin #
  //#define E_STOP_PIN              36  // 36 AT90USB… pin #

  //
  // Limit Switche definitions that match the silkscreen
  //
  #define X_STOP_PIN              26  // 14 AT90USB… pin #
  #define Y_STOP_PIN              27  // 15 AT90USB… pin #
  #define Z_STOP_PIN              36  // 36 AT90USB… pin #
  //#define E_STOP_PIN              25  // 13 AT90USB… pin #

  //
  // Steppers
  //
  #define X_STEP_PIN              28  // 0 AT90USB… pin #
  #define X_DIR_PIN               29  // 1 AT90USB… pin #
  #define X_ENABLE_PIN            19  // 39 AT90USB… pin #

  #define Y_STEP_PIN              30  // 2 AT90USB… pin #
  #define Y_DIR_PIN               31  // 3 AT90USB… pin #
  #define Y_ENABLE_PIN            18  // 38 AT90USB… pin #

  #define Z_STEP_PIN              32  // 4 AT90USB… pin #
  #define Z_DIR_PIN               33  // 5 AT90USB… pin #
  #define Z_ENABLE_PIN            17  // 23 AT90USB… pin #

  #define E0_STEP_PIN             34  // 6 AT90USB… pin #
  #define E0_DIR_PIN              35  // 7 AT90USB… pin #
  #define E0_ENABLE_PIN           13  // 19 AT90USB… pin #

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN              7  // Analog Input (Extruder)
  #define TEMP_BED_PIN            6  // Analog Input (Bed)

  //
  // Heaters / Fans
  //
  #define HEATER_0_PIN            15  // Extruder   21 AT90USB… pin #
  #define HEATER_BED_PIN          14  // 20 AT90USB… pin #


  #define FAN_PIN                 16  // 22 AT90USB… pin #


  //
  // Misc. Functions
  //
  #define SDSS                    20  // 8 AT90USB… pin #

  //
  // LCD / Controller
  //
  #if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

    #define BEEPER_PIN       -1

    #if ENABLED(LCD_I2C_PANELOLU2)
      #define BTN_EN1                  3  // 27 AT90USB… pin #
      #define BTN_EN2                  2  // 26 AT90USB… pin #
      #define BTN_ENC                 41  // 43 AT90USB… pin #
      #define SDSS                    38  // use SD card on Panelolu2   40 AT90USB… pin #
    #endif // LCD_I2C_PANELOLU2

    #define SD_DETECT_PIN    -1

  #endif // ULTRA_LCD && NEWPANEL

  // added 9 DEC 2016
  #define SPINDLE_SPEED_PIN       24  // 12 AT90USB… pin #
  #define SPINDLE_ENABLE_PIN      39  // should have a pullup resistor on this pin   41 AT90USB… pin #
  #define SPINDLE_DIR_PIN         40  // 42 AT90USB… pin #

  // misc
  #define CASE_LIGHT_PIN           0  // 24 AT90USB… pin #

  #endif  // Teensy++ pin map

/**
 *
 *                      alternate  TEENSYPP       AT90USB…             name on
 *  names in this file  functions  pin numbering  pin numbering  port  silkscreen  schematic names
 *  X_STEP_PIN                     D28            D0             A0                X-STEP
 *  X_DIR_PIN                      D29            D1             A1                X-DIR
 *  Y_STEP_PIN                     D30            D2             A2                Y-STEP
 *  Y_DIR_PIN                      D31            D3             A3                Y-DIR
 *  Z_STEP_PIN                     D32            D4             A4                Z-STEP
 *  Z_DIR_PIN                      D33            D5             A5                Z-DIR
 *  E0_STEP_PIN                    D34            D6             A6                E-STEP
 *  E0_DIR_PIN                     D35            D7             A7                E-DIR
 *  SDSS                           D20            D8             B0                PB0-SS
 *                                 D21            D9             B1                SCLK
 *                                 D22            D10            B2                MOSI
 *                                 D23            D11            B3                MISO
 *  SPINDLE_SPEED_PIN   PWM2A      D24            D12            B4                EXP-PWM
 *  E_STOP_PIN                     D25            D13            B5    E-STOP      X-STOP
 *  X_STOP_PIN                     D26            D14            B6    X-STOP      Y-STOP
 *  Y_STOP_PIN                     D27            D15            B7    Y-STOP      Z-STOP
 *                                 D10            D16            C0                PC0
 *                                 D11            D17            C1                PC1
 *                                 D12            D18            C2                PC2
 *  E0_ENABLE_PIN                  D13            D19            C3                E-ENABLE
 *  HEATER_BED_PIN      PWM3C      D14            D20            C4                HOTBED
 *  HEATER_0_PIN        PWM3B      D15            D21            C5                HOTEND
 *  FAN_PIN             PWM3A      D16            D22            C6                PWM-FAN
 *  Z_ENABLE_PIN                   D17            D23            C7                Z-ENABLE
 *  CASE_LIGHT_PIN      PWM0B      D0             D24            D0                PD0-SCL
 *                      PWM2B      D1             D25            D1                PD1-SDA
 *  BTN_EN2                        D2             D26            D2                PD2-RX1
 *  BTN_EN1                        D3             D27            D3                PD3-TX1
 *                                 D4             D28            D4                PD4
 *                                 D5             D29            D5                PD5
 *                                 D6             D30            D6                PD6
 *                                 D7             D31            D7                PD7
 *                                 D8             D32            E0                PE0
 *                                 D9             D33            E1                PE1
 *                                 not assigned   D34            E2                ALE (PULL DOWN TO GROUND, JP32 TO +5)
 *                                 not assigned   D35            E3                to JP34
 *  Z_STOP_PIN                     D36            D36            E4    Z-STOP      E-STOP
 *                                 D37            D37            E5                PE5
 *  Y_ENABLE_PIN                   D18            D38            E6                Y-ENABLE
 *  X_ENABLE_PIN                   D19            D39            E7                X-ENABLE
 *  SDSS                A0         D38            D40            F0                A0
 *  SPINDLE_ENABLE_PIN  A1         D39            D41            F1                A1
 *  SPINDLE_DIR_PIN     A2         D40            D42            F2                A2
 *  BTN_ENC             A3         D41            D43            F3                A3
 *                      A4         D42            D44            F4                A4
 *                      A5         D43            D45            F5                A5
 *  TEMP_BED_PIN        A6         D44            D46            F6                B-THERM
 *  TEMP_0_PIN          A7         D45            D47            F7                E-THERM
 *
 */



