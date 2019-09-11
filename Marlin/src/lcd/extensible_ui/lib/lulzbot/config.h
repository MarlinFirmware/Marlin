/************
 * config.h *
 ************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

#include "compat.h"

// Define the display board used (see "ftdi_eve_boards.h" for definitions)

//#define LCD_FTDI_VM800B35A        // FTDI 3.5" 320x240 with FT800
//#define LCD_4DSYSTEMS_4DLCD_FT843 // 4D Systems 4.3" 480x272
//#define LCD_HAOYU_FT800CB         // Haoyu with 4.3" or 5" 480x272
//#define LCD_HAOYU_FT810CB         // Haoyu with 5" 800x480
//#define LCD_ALEPHOBJECTS_CLCD_UI  // Aleph Objects Color LCD User Interface

// Leave the following commented out to use a board's default resolution.
// If you have changed the LCD panel, you may override the resolution
// below (see "ftdi_eve_resolutions.h" for definitions):

//#define TOUCH_UI_320x240
//#define TOUCH_UI_480x272
//#define TOUCH_UI_800x480

// Define the printer interface or pins used (see "ui_pin_mappings.h" for definitions):

//#define CR10_TFT_PINMAP
//#define AO_EXP1_DEPRECATED_PINMAP  // UltraLCD EXP1 connector, old AlephObject's wiring
//#define AO_EXP1_PINMAP  // UltraLCD EXP1 connector, new AlephObject's wiring
//#define AO_EXP2_PINMAP  // UltraLCD EXP2 connector, new AlephObject's wiring
//#define OTHER_PIN_LAYOUT

// Otherwise. Define all the pins manually:

#ifdef OTHER_PIN_LAYOUT
    // Select interfacing pins, the following pin specifiers are supported:
    //
    //     ARDUINO_DIGITAL_1  - Arduino pin via digitalWrite/digitalRead
    //     AVR_A1             - Fast AVR port access via PORTA/PINA/DDRA
    //     1                  - When compiling Marlin, use Marlin pin IDs.

    // The pins for CS and MOD_RESET (PD) must be chosen.
    #define CLCD_MOD_RESET                      9
    #define CLCD_SPI_CS                        10

    // If using software SPI, specify pins for SCLK, MOSI, MISO
    //#define CLCD_USE_SOFT_SPI
    #ifdef CLCD_USE_SOFT_SPI
        #define CLCD_SOFT_SPI_MOSI             11
        #define CLCD_SOFT_SPI_MISO             12
        #define CLCD_SOFT_SPI_SCLK             13
    #endif
#endif

// Defines how to orient the display. An inverted (i.e. upside-down) display
// is supported on the FT800. The FT810 or better also support a portrait
// and mirrored orientation.
//#define TOUCH_UI_INVERTED
//#define TOUCH_UI_PORTRAIT
//#define TOUCH_UI_MIRRORED

// Enable UTF8 rendering capabilities.
//#define TOUCH_UI_USE_UTF8
#ifdef  TOUCH_UI_USE_UTF8
  #define TOUCH_UI_UTF8_WESTERN_CHARSET
#endif

// Use a numeric passcode for "Parental lock".
// This is a recommended for smaller displays.
//#define TOUCH_UI_PASSCODE

// The timeout (in ms) to return to the status screen from sub-menus
//#define LCD_TIMEOUT_TO_STATUS 15000

// Enable this to debug the event framework
//#define UI_FRAMEWORK_DEBUG

// Enable the developer's menu and screens
//#define DEVELOPER_SCREENS

// Maximum feed rate for manual extrusion (mm/s)
#define MAX_MANUAL_FEEDRATE {50*60, 50*60, 4*60, 60}

// Sets the SPI speed in Hz

#define SPI_FREQUENCY 8000000 >> SPI_SPEED
