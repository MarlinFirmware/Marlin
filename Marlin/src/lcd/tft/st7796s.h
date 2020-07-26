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

#define ST7796S_MADCTL_MY         0x80 // Row Address Order
#define ST7796S_MADCTL_MX         0x40 // Column Address Order
#define ST7796S_MADCTL_MV         0x20 // Row/Column Exchange
#define ST7796S_MADCTL_ML         0x10 // Vertical Refresh Order
#define ST7796S_MADCTL_BGR        0x08 // RGB-BGR ORDER
#define ST7796S_MADCTL_RGB        0x00
#define ST7796S_MADCTL_MH         0x04 // Horizontal Refresh Order

#define ST7796S_COLOR_BGR
#define ST7796S_ORIENTATION       ST7796S_MADCTL_MV
#define ST7796S_MADCTL_DATA      (ST7796S_ORIENTATION | TERN(ST7796S_COLOR_BGR, ST7796S_MADCTL_BGR, ST7796S_MADCTL_RGB))

#define ST7796S_NOP               0x00 // No Operation
#define ST7796S_SWRESET           0x01 // Software reset
#define ST7796S_RDDID             0x04 // Read Display ID
#define ST7796S_RDNUMED           0x05 // Read Number of the Errors on DSI
#define ST7796S_RDDST             0x09 // Read Display Status
#define ST7796S_RDDPM             0x0A // Read Display Power Mode
#define ST7796S_RDDMADCTL         0x0B // Read Display MADCTL
#define ST7796S_RDDCOLMOD         0x0C // Read Display Pixel Format
#define ST7796S_RDDIM             0x0D // Read Display Image Mode
#define ST7796S_RDDSM             0x0E // Read Display Signal Status
#define ST7796S_RDDSDR            0x0F // Read Display Self-Diagnostic Result
#define ST7796S_SLPIN             0x10 // Sleep In
#define ST7796S_SLPOUT            0x11 // Sleep Out
#define ST7796S_PTLON             0x12 // Partial Display Mode On
#define ST7796S_NORON             0x13 // Normal Display Mode On
#define ST7796S_INVOFF            0x20 // Display Inversion Off
#define ST7796S_INVON             0x21 // Display Inversion On
#define ST7796S_DISPOFF           0x28 // Display Off
#define ST7796S_DISPON            0x29 // Display On
#define ST7796S_CASET             0x2A // Column Address Set
#define ST7796S_RASET             0x2B // Row Address Set
#define ST7796S_RAMWR             0x2C // Memory Write
#define ST7796S_RAMRD             0x2E // Memory Read
#define ST7796S_PTLAR             0x30 // Partial Area
#define ST7796S_VSCRDEF           0x33 // Vertical Scrolling Definition
#define ST7796S_TEOFF             0x34 // Tearing Effect Line OFF
#define ST7796S_TEON              0x35 // Tearing Effect Line On
#define ST7796S_MADCTL            0x36 // Memory Data Access Control
#define ST7796S_VSCSAD            0x37 // Vertical Scroll Start Address of RAM
#define ST7796S_IDMOFF            0x38 // Idle Mode Off
#define ST7796S_IDMON             0x39 // Idle Mode On
#define ST7796S_COLMOD            0x3A // Interface Pixel Format
#define ST7796S_WRMEMC            0x3C // Write Memory Continue
#define ST7796S_RDMEMC            0x3E // Read Memory Continue
#define ST7796S_STE               0x44 // Set Tear ScanLine
#define ST7796S_GSCAN             0x45 // Get ScanLine
#define ST7796S_WRDISBV           0x51 // Write Display Brightness
#define ST7796S_RDDISBV           0x52 // Read Display Brightness Value
#define ST7796S_WRCTRLD           0x53 // Write CTRL Display
#define ST7796S_RDCTRLD           0x54 // Read CTRL value Display
#define ST7796S_WRCABC            0x55 // Write Adaptive Brightness Control
#define ST7796S_RDCABC            0x56 // Read Content Adaptive Brightness Control
#define ST7796S_WRCABCMB          0x5E // Write CABC Minimum Brightness
#define ST7796S_RDCABCMB          0x5F // Read CABC Minimum Brightness
#define ST7796S_RDFCS             0xAA // Read First Checksum
#define ST7796S_RDCFCS            0xAF // Read Continue Checksum
#define ST7796S_RDID1             0xDA // Read ID1
#define ST7796S_RDID2             0xDB // Read ID2
#define ST7796S_RDID3             0xDC // Read ID3

#define ST7796S_IFMODE            0xB0 // Interface Mode Control
#define ST7796S_FRMCTR1           0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ST7796S_FRMCTR2           0xB2 // Frame Rate Control 2 (In Idle Mode/8 colors)
#define ST7796S_FRMCTR3           0xB3 // Frame Rate Control 3(In Partial Mode/Full Colors)
#define ST7796S_DIC               0xB4 // Display Inversion Control
#define ST7796S_BPC               0xB5 // Blanking Porch Control
#define ST7796S_DFC               0xB6 // Display Function Control
#define ST7796S_EM                0xB7 // Entry Mode Set
#define ST7796S_PWR1              0xC0 // Power Control 1
#define ST7796S_PWR2              0xC1 // Power Control 2
#define ST7796S_PWR3              0xC2 // Power Control 3
#define ST7796S_VCMPCTL           0xC5 // VCOM Control
#define ST7796S_VCMOST            0xC6 // VCOM Offset Register
#define ST7796S_NVMADW            0xD0 // NVM Address/Data Write
#define ST7796S_NVMBPROG          0xD1 // NVM Byte Program
#define ST7796S_NVMSTRD           0xD2 // NVM Status Read
#define ST7796S_RDID4             0xD3 // Read ID4
#define ST7796S_PGC               0xE0 // Positive Gamma Control
#define ST7796S_NGC               0xE1 // Negative Gamma Control
#define ST7796S_DGC1              0xE2 // Digital Gamma Control 1
#define ST7796S_DGC2              0xE3 // Digital Gamma Control 2
#define ST7796S_DOCA              0xE8 // Display Output Ctrl Adjust
#define ST7796S_CSCON             0xF0 // Command Set Control
#define ST7796S_SPIRC             0xFB // SPI Read Control

static const uint16_t st7796s_init[] = {
  DATASIZE_8BIT,
  ESC_REG(ST7796S_SWRESET), ESC_DELAY(100),
  ESC_REG(ST7796S_SLPOUT), ESC_DELAY(20),

  ESC_REG(ST7796S_CSCON), 0x00C3,  // enable command 2 part I
  ESC_REG(ST7796S_CSCON), 0x0096,  // enable command 2 part II

  ESC_REG(ST7796S_MADCTL), ST7796S_MADCTL_DATA,
  ESC_REG(ST7796S_COLMOD), 0x0055,

  ESC_REG(ST7796S_DIC), 0x0001,  // 1-dot inversion
  ESC_REG(ST7796S_EM), 0x00C6,

  ESC_REG(ST7796S_PWR2), 0x0015,
  ESC_REG(ST7796S_PWR3), 0x00AF,
  ESC_REG(ST7796S_VCMPCTL), 0x0022,
  ESC_REG(ST7796S_VCMOST), 0x0000,
  ESC_REG(ST7796S_DOCA), 0x0040, 0x008A, 0x0000, 0x0000, 0x0029, 0x0019, 0x00A5, 0x0033,

  /* Gamma Correction. */
  ESC_REG(ST7796S_PGC), 0x00F0,	0x0004, 0x0008, 0x0009, 0x0008, 0x0015,	0x002F,	0x0042,	0x0046,	0x0028,	0x0015,	0x0016,	0x0029,	0x002D,
  ESC_REG(ST7796S_NGC), 0x00F0,	0x0004,	0x0009,	0x0009,	0x0008,	0x0015,	0x002E,	0x0046,	0x0046,	0x0028,	0x0015,	0x0015,	0x0029,	0x002D,

  ESC_REG(ST7796S_NORON),
  ESC_REG(ST7796S_WRCTRLD), 0x0024,
  ESC_REG(ST7796S_CSCON), 0x003C,  // disable command 2 part I
  ESC_REG(ST7796S_CSCON), 0x0069,  // disable command 2 part II
  ESC_REG(ST7796S_DISPON),
  ESC_END
};

static const uint16_t lerdge_st7796s_init[] = {
  DATASIZE_8BIT,
	ESC_REG(ST7796S_CSCON), 0x00C3,  // enable command 2 part I
	ESC_REG(ST7796S_CSCON), 0x0096,  // enable command 2 part II

	ESC_REG(ST7796S_MADCTL), ST7796S_MADCTL_DATA,
  ESC_REG(ST7796S_COLMOD), 0x0055,

	ESC_REG(ST7796S_DIC), 0x0001,  // 1-dot inversion
	ESC_REG(ST7796S_EM), 0x00C6,

	ESC_REG(ST7796S_PWR2), 0x0015,
	ESC_REG(ST7796S_PWR3), 0x00AF,
	ESC_REG(0xC3), 0x0009,       // Register not documented in datasheet
	ESC_REG(ST7796S_VCMPCTL), 0x0022,
	ESC_REG(ST7796S_VCMOST), 0x0000,
	ESC_REG(ST7796S_DOCA), 0x0040, 0x008A, 0x0000, 0x0000, 0x0029, 0x0019, 0x00A5, 0x0033,

  /* Gamma Correction. */
  ESC_REG(ST7796S_PGC), 0x00F0,	0x0004, 0x0008, 0x0009, 0x0008, 0x0015,	0x002F,	0x0042,	0x0046,	0x0028,	0x0015,	0x0016,	0x0029,	0x002D,
	ESC_REG(ST7796S_NGC), 0x00F0,	0x0004,	0x0009,	0x0009,	0x0008,	0x0015,	0x002E,	0x0046,	0x0046,	0x0028,	0x0015,	0x0015,	0x0029,	0x002D,

	ESC_REG(ST7796S_INVON),      // Display inversion ON
	ESC_REG(ST7796S_WRCTRLD), 0x0024,
	ESC_REG(ST7796S_CSCON), 0x003C,  // disable command 2 part I
	ESC_REG(ST7796S_CSCON), 0x0069,  // disable command 2 part II
	ESC_REG(ST7796S_DISPON),
  ESC_END
};
