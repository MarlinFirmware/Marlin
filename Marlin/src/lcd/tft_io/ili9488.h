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

#include "tft_io.h"

#include "../../inc/MarlinConfig.h"

#define ILI9488_MADCTL_MY         0x80 // Row Address Order
#define ILI9488_MADCTL_MX         0x40 // Column Address Order
#define ILI9488_MADCTL_MV         0x20 // Row/Column Exchange
#define ILI9488_MADCTL_ML         0x10 // Vertical Refresh Order
#define ILI9488_MADCTL_BGR        0x08 // RGB-BGR ORDER
#define ILI9488_MADCTL_RGB        0x00
#define ILI9488_MADCTL_MH         0x04 // Horizontal Refresh Order

#define ILI9488_ORIENTATION_UP    ILI9488_MADCTL_MY                                         // 320x480 ; Cable on the upper side
#define ILI9488_ORIENTATION_RIGHT ILI9488_MADCTL_MV                                         // 480x320 ; Cable on the right side
#define ILI9488_ORIENTATION_LEFT  ILI9488_MADCTL_MY | ILI9488_MADCTL_MX | ILI9488_MADCTL_MV // 480x320 ; Cable on the left side
#define ILI9488_ORIENTATION_DOWN  ILI9488_MADCTL_MX                                         // 320x480 ; Cable on the upper side

#define ILI9488_ORIENTATION IF_0((TFT_ORIENTATION) & TFT_EXCHANGE_XY, ILI9488_MADCTL_MV) | \
                            IF_0((TFT_ORIENTATION) & TFT_INVERT_X,    ILI9488_MADCTL_MX) | \
                            IF_0((TFT_ORIENTATION) & TFT_INVERT_Y,    ILI9488_MADCTL_MY)

#if !defined(TFT_COLOR) || TFT_COLOR == TFT_COLOR_BGR
  #define ILI9488_COLOR ILI9488_MADCTL_BGR
#elif TFT_COLOR == TFT_COLOR_RGB
  #define ILI9488_COLOR ILI9488_MADCTL_RGB
#endif

#define ILI9488_MADCTL_DATA       (ILI9488_ORIENTATION) | (ILI9488_COLOR)

#define ILI9488_NOP               0x00 // No Operation
#define ILI9488_SWRESET           0x01 // Software Reset
#define ILI9488_RDDIDIF           0x04 // Read Display Identification Information
#define ILI9488_RDNUMED           0x05 // Read Number of the Errors on DSI
#define ILI9488_RDDST             0x09 // Read Display Status
#define ILI9488_RDDPM             0x0A // Read Display Power Mode
#define ILI9488_RDDMADCTL         0x0B // Read Display MADCTL
#define ILI9488_RDDCOLMOD         0x0C // Read Display COLMOD
#define ILI9488_RDDIM             0x0D // Read Display Image Mode
#define ILI9488_RDDSM             0x0E // Read Display Signal Mode
#define ILI9488_RDDSDR            0x0F // Read Display Self-Diagnostic Result
#define ILI9488_SLPIN             0x10 // Sleep IN
#define ILI9488_SLPOUT            0x11 // Sleep OUT
#define ILI9488_PTLON             0x12 // Partial Mode ON
#define ILI9488_NORON             0x13 // Normal Display Mode ON
#define ILI9488_INVOFF            0x20 // Display Inversion OFF
#define ILI9488_INVON             0x21 // Display Inversion ON
#define ILI9488_ALLPOFF           0x22 // All Pixels OFF
#define ILI9488_ALLPON            0x23 // All Pixels ON
#define ILI9488_DISOFF            0x28 // Display OFF
#define ILI9488_DISON             0x29 // Display ON
#define ILI9488_CASET             0x2A // Column Address Set
#define ILI9488_PASET             0x2B // Page Address Set
#define ILI9488_RAMWR             0x2C // Memory Write
#define ILI9488_RAMRD             0x2E // Memory Read
#define ILI9488_PLTAR             0x30 // Partial Area
#define ILI9488_VSCRDEF           0x33 // Vertical Scrolling Definition
#define ILI9488_TEOFF             0x34 // Tearing Effect Line OFF
#define ILI9488_TEON              0x35 // Tearing Effect Line ON
#define ILI9488_MADCTL            0x36 // Memory Access Control
#define ILI9488_VSCRSADD          0x37 // Vertical Scrolling Start Address
#define ILI9488_IDMOFF            0x38 // Idle Mode OFF
#define ILI9488_IDMON             0x39 // Idle Mode ON
#define ILI9488_COLMOD            0x3A // Interface Pixel Format
#define ILI9488_RAMWRC            0x3C // Memory Write Continue
#define ILI9488_RAMRDRC           0x3E // Memory Read Continue
#define ILI9488_TESLWR            0x44 // Write Tear Scan Line
#define ILI9488_TESLRD            0x45 // Read Scan Line
#define ILI9488_WRDISBV           0x51 // Write Display Brightness Value
#define ILI9488_RDDISBV           0x52 // Read Display Brightness Value
#define ILI9488_WRCTRLD           0x53 // Write Control Display Value
#define ILI9488_RDCTRLD           0x54 // Read Control Display Value
#define ILI9488_WRCABC            0x55 // Write Content Adaptive Brightness Control Value
#define ILI9488_RDCABC            0x56 // Read Content Adaptive Brightness Control Value
#define ILI9488_WRCABCMB          0x5E // Write CABC Minimum Brightness
#define ILI9488_RDCABCMB          0x5F // Read CABC Minimum Brightness
#define ILI9488_RDABCSDR          0x68 // Read Automatic Brightness Control Self-diagnostic Result
#define ILI9488_RDID1             0xDA // Read ID1
#define ILI9488_RDID2             0xDB // Read ID2
#define ILI9488_RDID3             0xDC // Read ID3

#define ILI9488_IFMODE            0xB0 // Interface Mode Control
#define ILI9488_FRMCTR1           0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9488_FRMCTR2           0xB2 // Frame Rate Control (In Idle Mode/8 Colors)
#define ILI9488_FRMCTR3           0xB3 // Frame Rate Control (In Partial Mode/Full Colors)
#define ILI9488_INVTR             0xB4 // Display Inversion Control
#define ILI9488_PRCTR             0xB5 // Blanking Porch Control
#define ILI9488_DISCTRL           0xB6 // Display Function Control
#define ILI9488_ETMOD             0xB7 // Entry Mode Set
#define ILI9488_CECTRL1           0xB9 // Color Enhancement Control 1
#define ILI9488_CECTRL2           0xBA // Color Enhancement Control 2
#define ILI9488_HSLCTRL           0xBE // HS Lanes Control
#define ILI9488_PWCTRL1           0xC0 // Power Control 1
#define ILI9488_PWCTRL2           0xC1 // Power Control 2
#define ILI9488_PWCTRL3           0xC2 // Power Control 3 (For Normal Mode)
#define ILI9488_PWCTRL4           0xC3 // Power Control 4 (For Idle Mode)
#define ILI9488_PWCTRL5           0xC4 // Power Control 5 (For Partial Mode)
#define ILI9488_VMCTRL            0xC5 // VCOM Control
#define ILI9488_CABCCTRL1         0xC6 // CABC Control 1
#define ILI9488_CABCCTRL2         0xC8 // CABC Control 2
#define ILI9488_CABCCTRL3         0xC9 // CABC Control 3
#define ILI9488_CABCCTRL4         0xCA // CABC Control 4
#define ILI9488_CABCCTRL5         0xCB // CABC Control 5
#define ILI9488_CABCCTRL6         0xCC // CABC Control 6
#define ILI9488_CABCCTRL7         0xCD // CABC Control 7
#define ILI9488_CABCCTRL8         0xCE // CABC Control 8
#define ILI9488_CABCCTRL9         0xCF // CABC Control 9
#define ILI9488_NVMWR             0xD0 // NV Memory Write
#define ILI9488_NVMPKEY           0xD1 // NV Memory Protection Key
#define ILI9488_RDNVM             0xD2 // NV Memory Status Read
#define ILI9488_RDID4             0xD3 // Read ID4 - 0x009488
#define ILI9488_ADJCTL1           0xD7 // Adjust Control 1
#define ILI9488_RDIDV             0xD8 // Read ID Version
#define ILI9488_PGAMCTRL          0xE0 // Positive Gamma Control
#define ILI9488_NGAMCTRL          0xE1 // Negative Gamma Control
#define ILI9488_DGAMCTRL1         0xE2 // Ditigal Gamma Control 1
#define ILI9488_DGAMCTRL2         0xE3 // Ditigal Gamma Control 2
#define ILI9488_SETIMAGE          0xE9 // Set Image Function
#define ILI9488_ADJCTL2           0xF2 // Adjust Control 2
#define ILI9488_ADJCTL3           0xF7 // Adjust Control 3
#define ILI9488_ADJCTL4           0xF8 // Adjust Control 4
#define ILI9488_ADJCTL5           0xF9 // Adjust Control 5
#define ILI9488_RDEXTC            0xFB // Read EXTC command is SPI mode
#define ILI9488_ADJCTL6           0xFC // Adjust Control 6
#define ILI9488_ADJCTL7           0xFF // Adjust Control 7

static const uint16_t ili9488_init[] = {
  DATASIZE_8BIT,
  ESC_REG(ILI9488_SWRESET), ESC_DELAY(120),
  ESC_REG(ILI9488_SLPOUT), ESC_DELAY(20),

  ESC_REG(ILI9488_MADCTL), ILI9488_MADCTL_DATA,
  ESC_REG(ILI9488_COLMOD), 0x0055,

  /* Gamma Correction. */
  ESC_REG(ILI9488_PGAMCTRL), 0x0000, 0x0003, 0x0009, 0x0008, 0x0016, 0x000A, 0x003F, 0x0078, 0x004C, 0x0009, 0x000A, 0x0008, 0x0016, 0x001A, 0x000F,
  ESC_REG(ILI9488_NGAMCTRL), 0x0000, 0x0016, 0x0019, 0x0003, 0x000F, 0x0005, 0x0032, 0x0045, 0x0046, 0x0004, 0x000E, 0x000D, 0x0035, 0x0037, 0x000F,

  ESC_REG(ILI9488_NORON),
  ESC_REG(ILI9488_DISON),
  ESC_END
};
