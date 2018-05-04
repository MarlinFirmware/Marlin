/***************
 * ui_config.h *
 ***************/

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

#ifndef _UI_CONFIG_H_
#define _UI_CONFIG_H_

#include "../Marlin.h"

#if defined(FTDI_EVE_TOUCHSCREEN)
    // Uncomment the following to use hardware SPI.
    //#define USE_ARDUINO_HW_SPI

    // By default, the FTDI driver can repurpose the
    // pins defined for ULTRALCD. However, they can be
    // changed here.

    #ifndef USE_ARDUINO_HW_SPI
        #define CLCD_SOFT_SPI_SCLK             LCD_PINS_D7
        #define CLCD_SOFT_SPI_MOSI             LCD_PINS_D6
        #define CLCD_SOFT_SPI_CS               LCD_PINS_D5
        #define CLCD_SOFT_SPI_MISO             LCD_PINS_RS
        #define CLCD_MOD_RESET                 LCD_PINS_D4     // Module reset
        #define CLCD_AUX_0                     LCD_PINS_ENABLE
        #define CLCD_AUX_1                     BTN_ENC
        #define CLCD_AUX_2                     BEEPER_PIN
    #endif

    // Define whether an FT800 or FT810+ chip is being used
    //#define USE_FTDI_FT800
    #define USE_FTDI_FT810

    // Define whether to use a portrait orientation (requires FT810 or above).
    // If not defined, landscape orientation will be used.
    #define USE_PORTRAIT_ORIENTATION
    #define FLIP_UPSIDE_DOWN

    // Define the display resolution
    #define LCD_480x272
    #define LCD_800x480

    // The following needs to be defined for this display type.
    #define USE_UI_FRAMEWORK
#endif

#endif _UI_CONFIG_H_