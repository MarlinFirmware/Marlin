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

#include "tft.h"

#include "../../inc/MarlinConfig.h"

#define ILI9341_MADCTL_MY         0x80 // Row Address Order
#define ILI9341_MADCTL_MX         0x40 // Column Address Order
#define ILI9341_MADCTL_MV         0x20 // Row/Column Exchange
#define ILI9341_MADCTL_ML         0x10 // Vertical Refresh Order
#define ILI9341_MADCTL_BGR        0x08 // RGB-BGR ORDER
#define ILI9341_MADCTL_RGB        0x00
#define ILI9341_MADCTL_MH         0x04 // Horizontal Refresh Order

#define ILI9341_ORIENTATION_UP    ILI9341_MADCTL_MY	                                        // 240x320 ; Cable on the upper side
#define ILI9341_ORIENTATION_RIGHT ILI9341_MADCTL_MV                                         // 320x240 ; Cable on the right side
#define ILI9341_ORIENTATION_LEFT  ILI9341_MADCTL_MY | ILI9341_MADCTL_MX | ILI9341_MADCTL_MV // 320x240 ; Cable on the left side
#define ILI9341_ORIENTATION_DOWN  ILI9341_MADCTL_MX	                                        // 240x320 ; Cable on the upper side

#define ILI9341_COLOR_BGR
#define ILI9341_ORIENTATION       ILI9341_ORIENTATION_LEFT
#define ILI9341_MADCTL_DATA      (ILI9341_ORIENTATION | TERN(ILI9341_COLOR_BGR, ILI9341_MADCTL_BGR, ILI9341_MADCTL_RGB))

#define ILI9341_NOP               0x00 // No Operation
#define ILI9341_SWRESET           0x01 // Software Reset
#define ILI9341_RDDIDIF           0x04 // Read display identification information
#define ILI9341_RDDST             0x09 // Read Display Status
#define ILI9341_RDDPM             0x0A // Read Display Power Mode
#define ILI9341_RDDMADCTL         0x0B // Read Display MADCTL
#define ILI9341_RDDCOLMOD         0x0C // Read Display Pixel Format
#define ILI9341_RDDIM             0x0D // Read Display Image Format
#define ILI9341_RDDSM             0x0E // Read Display Signal Mode
#define ILI9341_RDDSDR            0x0F // Read Display Self-Diagnostic Result
#define ILI9341_SPLIN             0x10 // Enter Sleep Mode
#define ILI9341_SLPOUT            0x11 // Sleep Out
#define ILI9341_PTLON             0x12 // Partial Mode ON
#define ILI9341_NORON             0x13 // Normal Display Mode ON
#define ILI9341_DINVOFF           0x20 // Display Inversion OFF
#define ILI9341_DINVON            0x21 // Display Inversion ON
#define ILI9341_GAMSET            0x26 // Gamma Set
#define ILI9341_DISPOFF           0x28 // Display OFF
#define ILI9341_DISPON            0x29 // Display ON
#define ILI9341_CASET             0x2A // Column Address Set
#define ILI9341_PASET             0x2B // Page Address Set
#define ILI9341_RAMWR             0x2C // Memory Write
#define ILI9341_RGBSET            0x2D // Color Set
#define ILI9341_RAMRD             0x2E // Memory Read
#define ILI9341_PLTAR             0x30 // Partial Area
#define ILI9341_VSCRDEF           0x33 // Vertical Scrolling Definition
#define ILI9341_TEOFF             0x34 // Tearing Effect Line OFF
#define ILI9341_TEON              0x35 // Tearing Effect Line ON
#define ILI9341_MADCTL            0x36 // Memory Access Control
#define ILI9341_VSCRSADD          0x37 // Vertical Scrolling Start Address
#define ILI9341_IDMOFF            0x38 // Idle Mode OFF
#define ILI9341_IDMON             0x39 // Idle Mode ON
#define ILI9341_PIXSET            0x3A // COLMOD: Pixel Format Set
#define ILI9341_WRMEMC            0x3C // Write Memory Continue
#define ILI9341_RDMEMC            0x3E // Read Memory Continue
#define ILI9341_STE               0x44 // Set Tear Scanline
#define ILI9341_GSCAN             0x45 // Get Scanline
#define ILI9341_WRDISBV           0x51 // Write Display Brightness
#define ILI9341_RDDISBV           0x52 // Read Display Brightness
#define ILI9341_WRCTRLD           0x53 // Write CTRL Display
#define ILI9341_RDCTRLD           0x54 // Read CTRL Display
#define ILI9341_WRCABC            0x55 // Write Content Adaptive Brightness Control
#define ILI9341_RDCABC            0x56 // Read Content Adaptive Brightness Control
#define ILI9341_WRCABCMB          0x5E // Write CABC Minimum Brightness / Backlight Control 1
#define ILI9341_RDCABCMB          0x5F // Read CABC Minimum Brightness / Backlight Control 1
#define ILI9341_RDID1             0xDA // Read ID1
#define ILI9341_RDID2             0xDB // Read ID2
#define ILI9341_RDID3             0xDC // Read ID3

#define ILI9341_IFMODE            0xB0 // RGB Interface Signal Control
#define ILI9341_FRMCTR1           0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2           0xB2 // Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3           0xB3 // Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVTR             0xB4 // Display Inversion Control
#define ILI9341_PRCTR             0xB5 // Blanking Porch Control
#define ILI9341_DISCTRL           0xB6 // Display Function Control
#define ILI9341_ETMOD             0xB7 // Entry Mode Set
#define ILI9341_BLCTL1            0xB8 // Backlight Control 1
#define ILI9341_BLCTL2            0xB9 // Backlight Control 2
#define ILI9341_BLCTL3            0xBA // Backlight Control 3
#define ILI9341_BLCTL4            0xBB // Backlight Control 4
#define ILI9341_BLCTL5            0xBC // Backlight Control 5
#define ILI9341_BLCTL7            0xBE // Backlight Control 7
#define ILI9341_BLCTL8            0xBF // Backlight Control 8
#define ILI9341_PWCTRL1           0xC0 // Power Control 1
#define ILI9341_PWCTRL2           0xC1 // Power Control 2
#define ILI9341_VMCTRL1           0xC5 // VCOM Control 1
#define ILI9341_VMCTRL2           0xC7 // VCOM Control 2
#define ILI9341_PWCTRLA           0xCB // Power control A
#define ILI9341_PWCTRLB           0xCF // Power control B
#define ILI9341_NVMWR             0xD0 // NV Memory Write
#define ILI9341_NVMPKEY           0xD1 // NV Memory Protection Key
#define ILI9341_RDNVM             0xD2 // NV Memory Status Read
#define ILI9341_RDID4             0xD3 // Read ID4 - 0x009341
#define ILI9341_PGAMCTRL          0xE0 // Positive Gamma Correction
#define ILI9341_NGAMCTRL          0xE1 // Negative Gamma Correction
#define ILI9341_DGAMCTRL1         0xE2 // Digital Gamma Control 1
#define ILI9341_DGAMCTRL2         0xE3 // Digital Gamma Control 2
#define ILI9341_DRVTCTLA1         0xE8 // Driver timing control A
#define ILI9341_DRVTCTLA2         0xE9 // Driver timing control A
#define ILI9341_DRVTCTLB          0xEA // Driver timing control B
#define ILI9341_PONSEQCTL         0xED // Power on sequence control
#define ILI9341_EN3G              0xF2 // Enable 3G - 3 gamma control
#define ILI9341_IFCTL             0xF6 // Interface Control
#define ILI9341_PUMPRCTL          0xF7 // Pump ratio control


static const uint16_t ili9341_init[] = {
  DATASIZE_8BIT,
  ESC_REG(ILI9341_SWRESET), ESC_DELAY(100),
  ESC_REG(ILI9341_SLPOUT), ESC_DELAY(20),
/*
  ESC_REG(ILI9341_PWCTRLA), 0x0039, 0x002C, 0x0000, 0x0034, 0x0002, // Power control A
  ESC_REG(ILI9341_PWCTRLB), 0x0000, 0x00C1, 0x0030,                 // Power control B
  ESC_REG(ILI9341_DRVTCTLA1), 0x0085, 0x0000, 0x0078,               // Driver timing control A
  ESC_REG(ILI9341_DRVTCTLB), 0x0000, 0x0000,                        // Driver timing control B
  ESC_REG(ILI9341_PONSEQCTL), 0x0064, 0x0003, 0x0012, 0x0081,       // Power on sequence control
  ESC_REG(ILI9341_DISCTRL), 0x0008, 0x0082, 0x0027,                 // Display Function Control
  ESC_REG(ILI9341_PUMPRCTL), 0x0020,                                // Pump ratio control
  ESC_REG(ILI9341_VMCTRL1), 0x003E, 0x0028,                         // VCOM Control 1
  ESC_REG(ILI9341_VMCTRL2), 0x0086,                                 // VCOM Control 2
  ESC_REG(ILI9341_FRMCTR1), 0x0000, 0x0018,                         // Frame Rate Control (In Normal Mode/Full Colors)
  ESC_REG(ILI9341_PWCTRL1), 0x0023,                                 // Power Control 1
  ESC_REG(ILI9341_PWCTRL2), 0x0010,                                 // Power Control 2
*/
  ESC_REG(ILI9341_MADCTL), ILI9341_MADCTL_DATA,
  ESC_REG(ILI9341_PIXSET), 0x0055,

  /* Gamma Correction */
  ESC_REG(ILI9341_EN3G), 0x0000,                 // 3Gamma Function Disable
  ESC_REG(ILI9341_GAMSET), 0x0001,               // Gamma curve selected
  ESC_REG(ILI9341_PGAMCTRL), 0x000F, 0x0031, 0x002B, 0x000C, 0x000E, 0x0008, 0x004E, 0x00F1, 0x0037, 0x0007, 0x0010, 0x0003, 0x000E, 0x0009, 0x0000,
  ESC_REG(ILI9341_NGAMCTRL), 0x0000, 0x000E, 0x0014, 0x0003, 0x0011, 0x0007, 0x0031, 0x00C1, 0x0048, 0x0008, 0x000F, 0x000C, 0x0031, 0x0036, 0x000F,

  ESC_REG(ILI9341_NORON),
  ESC_REG(ILI9341_DISPON),
  ESC_END
};
