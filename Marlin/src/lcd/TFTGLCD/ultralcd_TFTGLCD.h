/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Implementation of the LCD display routines for a TFT GLCD displays with external controller.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(TFTGLCD_ADAPTER)

#include "../../libs/duration_t.h"

////////////////////////////////////
// Setup button and encode mappings for each panel (into 'buttons' variable
//
// This is just to map common functions (across different panels) onto the same
// macro name. The mapping is independent of whether the button is directly connected or
// via a shift/i2c register.

////////////////////////////////////
// Create LCD class instance and chipset-specific information
class TFTGLCD {
  private:
  public:
    TFTGLCD();
    void clear_buffer();
    void setCursor(uint8_t col, uint8_t row);
    void write(char c);
    void print(const char *line);
    void print_line();
    void print_screen();
    void redraw_screen();
    void setContrast(uint16_t contrast);
};

#include "../fontutils.h"
#include "../lcdprint.h"

#if ENABLED(HAS_SLOW_BUTTONS)
//use panel encoder - free old encoder pins
  #ifdef BTN_EN1
    #undef  BTN_EN1
    #define BTN_EN1     -1
  #endif
  #ifdef BTN_EN2
    #undef  BTN_EN2
    #define BTN_EN2     -1
  #endif
  #ifdef BTN_ENC
    #undef  BTN_ENC
    #define BTN_ENC     -1
  #endif
#endif

#ifndef EN_C
  #define  EN_C 4 //for click
#endif

#endif
