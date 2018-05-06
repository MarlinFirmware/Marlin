/**********************
 * ftdi_eve_timings.h *
 **********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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

#ifndef _FTDI_EVE_TIMINGS_H_
#define _FTDI_EVE_TIMINGS_H_

#include "../ui_config.h"

// GLOBAL LCD REGISTER SET VALUES FOR WQVGA 480x272 DISPLAY

/*
 * Settings for the Aleph Objects Color LCD User Interface 4.3" (Prototype)  480x272, SPI, FT810 (HDA430T-6S-WV)
 *                  Haoyu Electronics, 4.3" Graphical LCD Touchscreen,       480x272, SPI, FT800 (FT800CB-HY43B)
 *                  Haoyu Electronics,   5" Graphical LCD Touchscreen,       480x272, SPI, FT800 (FT800CB-HY50B)
 *                  4D Systems,        4.3" Embedded SPI Display             480x272, SPI, FT800 (4DLCD-FT843)
 *
 *    http://www.hotmcu.com/43-graphical-lcd-touchscreen-480x272-spi-ft800-p-111.html?cPath=6_16
 *    http://www.hotmcu.com/5-graphical-lcd-touchscreen-480x272-spi-ft800-p-124.html?cPath=6_16
 *    http://www.4dsystems.com.au/product/4DLCD_FT843/
 *
 * Datasheet:
 *
 *    http://www.hantronix.com/files/data/1278363262430-3.pdf
 *    http://www.haoyuelectronics.com/Attachment/HY43-LCD/LCD%20DataSheet.pdf
 *    http://www.haoyuelectronics.com/Attachment/HY5-LCD-HD/KD50G21-40NT-A1.pdf
 *    http://www.4dsystems.com.au/productpages/4DLCD-FT843/downloads/FT843-4.3-Display_datasheet_R_1_2.pdf
 *
 */
namespace FTDI_LCD_480x272 {
  const int Vsync0  =    0;
  const int Vsync1  =   10;
  const int Voffset =   12;
  const int Vcycle  =  292;
  const int Hsync0  =    0;
  const int Hsync1  =   41;
  const int Hoffset =   43;
  const int Hcycle  =  548;
  const int Hsize   =  480;
  const int Vsize   =  272;
  const int Pclkpol =    1;
  const int Swizzle =    0;
  const int Pclk    =    5;
  const int Clksel  = 0x44;

  #if defined(USE_PORTRAIT_ORIENTATION)
    const uint32_t default_transform_a = 0xFFFFFEA3;
    const uint32_t default_transform_b = 0xFFFFB05D;
    const uint32_t default_transform_c = 0x012C5254;
    const uint32_t default_transform_d = 0xFFFF8303;
    const uint32_t default_transform_e = 0xFFFFFDF8;
    const uint32_t default_transform_f = 0x01F32984;
  #else
    const uint32_t default_transform_a = 0x00008100;
    const uint32_t default_transform_b = 0x00000000;
    const uint32_t default_transform_c = 0xFFF18000;
    const uint32_t default_transform_d = 0x00000000;
    const uint32_t default_transform_e = 0xFFFFB100;
    const uint32_t default_transform_f = 0x0120D000;
  #endif
}

// GLOBAL LCD REGISTER SET VALUES FOR 800x480 DISPLAY

/*
 * Settings for the Haoyu Electronics, 5" Graphical LCD Touchscreen, 800x480, SPI, FT810
 *
 *    http://www.hotmcu.com/5-graphical-lcd-touchscreen-800x480-spi-ft810-p-286.html
 *
 * Datasheet:
 *
 *    http://www.haoyuelectronics.com/Attachment/HY5-LCD-HD/KD50G21-40NT-A1.pdf
 *
 */
namespace FTDI_LCD_800x480 {
  const int Vsync0  =    0;
  const int Vsync1  =   13;
  const int Voffset =   16;
  const int Vcycle  =  525;
  const int Hsync0  =    0;
  const int Hsync1  =   40;
  const int Hoffset =   88;
  const int Hcycle  =  928;
  const int Hsize   =  800;
  const int Vsize   =  480;
  const int Pclkpol =    1;
  const int Swizzle =    0;
  const int Pclk    =    2;
  const int Clksel  = 0x45;

  const uint32_t default_transform_a = 0x0000012B;
  const uint32_t default_transform_b = 0xFFFF7BFC;
  const uint32_t default_transform_c = 0x01F2443A;
  const uint32_t default_transform_d = 0xFFFF25ED;
  const uint32_t default_transform_e = 0x000000F9;
  const uint32_t default_transform_f = 0x033F61DA;
}

#if defined(LCD_800x480)
  using namespace FTDI_LCD_800x480;
#elif defined(LCD_480x272)
  using namespace FTDI_LCD_480x272;
#else
  #error Invalid or no resolution specified for the LCD panel.
#endif

#endif // _FTDI_EVE_TIMINGS_H_





