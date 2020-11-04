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

#define ST7789V_MADCTL_MY         0x80 // Row Address Order
#define ST7789V_MADCTL_MX         0x40 // Column Address Order
#define ST7789V_MADCTL_MV         0x20 // Row/Column Exchange
#define ST7789V_MADCTL_ML         0x10 // Vertical Refresh Order
#define ST7789V_MADCTL_BGR        0x08 // RGB-BGR ORDER
#define ST7789V_MADCTL_RGB        0x00
#define ST7789V_MADCTL_MH         0x04 // Horizontal Refresh Order

#define ST7789V_ORIENTATION_UP    ST7789V_MADCTL_MX | ST7789V_MADCTL_MY // 240x320 ; Cable on the upper side
#define ST7789V_ORIENTATION_RIGHT ST7789V_MADCTL_MX | ST7789V_MADCTL_MV // 320x240 ; Cable on the right side
#define ST7789V_ORIENTATION_LEFT  ST7789V_MADCTL_MY | ST7789V_MADCTL_MV // 320x240 ; Cable on the left side
#define ST7789V_ORIENTATION_DOWN  0                                     // 240x320 ; Cable on the lower side

#define ST7789V_ORIENTATION IF_0((TFT_ORIENTATION) & TFT_EXCHANGE_XY, ST7789V_MADCTL_MV) | \
                            IF_0((TFT_ORIENTATION) & TFT_INVERT_X,    ST7789V_MADCTL_MX) | \
                            IF_0((TFT_ORIENTATION) & TFT_INVERT_Y,    ST7789V_MADCTL_MY)

#if !defined(TFT_COLOR) || TFT_COLOR == TFT_COLOR_RGB
  #define ST7789V_COLOR ST7789V_MADCTL_RGB
#elif TFT_COLOR == TFT_COLOR_BGR
  #define ST7789V_COLOR ST7789V_MADCTL_BGR
#endif

#define ST7789V_MADCTL_DATA       (ST7789V_ORIENTATION) | (ST7789V_COLOR)

#define ST7789V_NOP               0x00 // No Operation
#define ST7789V_SWRESET           0x01 // Software reset
#define ST7789V_RDDID             0x04 // Read Display ID
#define ST7789V_RDDST             0x09 // Read Display Status
#define ST7789V_RDDPM             0x0A // Read Display Power Mode
#define ST7789V_RDDMADCTL         0x0B // Read Display MADCTL
#define ST7789V_RDDCOLMOD         0x0C // Read Display Pixel Format
#define ST7789V_RDDIM             0x0D // Read Display Image Mode
#define ST7789V_RDDSM             0x0E // Read Display Signal Mode
#define ST7789V_RDDSDR            0x0F // Read Display Self-Diagnostic Result
#define ST7789V_SLPIN             0x10 // Sleep In
#define ST7789V_SLPOUT            0x11 // Sleep Out
#define ST7789V_PTLON             0x12 // Partial Display Mode On
#define ST7789V_NORON             0x13 // Normal Display Mode On
#define ST7789V_INVOFF            0x20 // Display Inversion Off
#define ST7789V_INVON             0x21 // Display Inversion On
#define ST7789V_GAMSET            0x26 // Gamma Set
#define ST7789V_DISPOFF           0x28 // Display Off
#define ST7789V_DISPON            0x29 // Display On
#define ST7789V_CASET             0x2A // Column Address Set
#define ST7789V_RASET             0x2B // Row Address Set
#define ST7789V_RAMWR             0x2C // Memory Write
#define ST7789V_RAMRD             0x2E // Memory Read
#define ST7789V_PTLAR             0x30 // Partial Area
#define ST7789V_VSCRDEF           0x33 // Vertical Scrolling Definition
#define ST7789V_TEOFF             0x34 // Tearing Effect Line OFF
#define ST7789V_TEON              0x35 // Tearing Effect Line ON
#define ST7789V_MADCTL            0x36 // Memory Data Access Control
#define ST7789V_VSCSAD            0x37 // Vertical Scroll Start Address of RAM
#define ST7789V_IDMOFF            0x38 // Idle Mode Off
#define ST7789V_IDMON             0x39 // Idle Mode On
#define ST7789V_COLMOD            0x3A // Interface Pixel Format
#define ST7789V_WRMEMC            0x3C // Write Memory Continue
#define ST7789V_RDMEMC            0x3E // Read Memory Continue
#define ST7789V_STE               0x44 // Set Tear Scanline
#define ST7789V_GSCAN             0x45 // Get Scanline
#define ST7789V_WRDISBV           0x51 // Write Display Brightness
#define ST7789V_RDDISBV           0x52 // Read Display Brightness
#define ST7789V_WRCTRLD           0x53 // Write CTRL Display
#define ST7789V_RDCTRLD           0x54 // Read CTRL Value Display
#define ST7789V_WRCACE            0x55 // Write Content Adaptive Brightness Control and Color Enhancement
#define ST7789V_RDCABC            0x56 // Read Content Adaptive Brightness Control
#define ST7789V_WRCABCMB          0x5E // Write CABC Minimum Brightness
#define ST7789V_RDCABCMB          0x5F // Read CABC Minimum Brightness
#define ST7789V_RDABCSDR          0x68 // Read Automatic Brightness Control Self-Diagnostic Result
#define ST7789V_RDID1             0xDA // Read ID1 Value
#define ST7789V_RDID2             0xDB // Read ID2 Value
#define ST7789V_RDID3             0xDC // Read ID3 Value

#define ST7789V_RAMCTRL           0xB0 // RAM Control
#define ST7789V_RGBCTRL           0xB1 // RGB Interface Control
#define ST7789V_PORCTRL           0xB2 // Porch Setting
#define ST7789V_FRCTRL1           0xB3 // Frame Rate Control 1 (In partial mode/ idle colors)
#define ST7789V_GCTRL             0xB7 // Gate Control
#define ST7789V_DGMEN             0xBA // Digital Gamma Enable
#define ST7789V_VCOMS             0xBB // VCOM Setting
#define ST7789V_LCMCTRL           0xC0 // LCM Control
#define ST7789V_IDSET             0xC1 // ID Code Setting
#define ST7789V_VDVVRHEN          0xC2 // VDV and VRH Command Enable
#define ST7789V_VRHS              0xC3 // VRH Set
#define ST7789V_VDVS              0xC4 // VDV Set
#define ST7789V_VCMOFSET          0xC5 // VCOM Offset Set
#define ST7789V_FRCTRL2           0xC6 // Frame Rate Control in Normal Mode
#define ST7789V_CABCCTRL          0xC7 // CABC Control
#define ST7789V_REGSEL1           0xC8 // Register Value Selection 1
#define ST7789V_REGSEL2           0xCA // Register Value Selection 2
#define ST7789V_PWMFRSEL          0xCC // PWM Frequency Selection
#define ST7789V_PWCTRL1           0xD0 // Power Control 1
#define ST7789V_VAPVANEN          0xD2 // Enable VAP/VAN signal output
#define ST7789V_CMD2EN            0xDF // Command 2 Enable
#define ST7789V_PVGAMCTRL         0xE0 // Positive Voltage Gamma Control
#define ST7789V_NVGAMCTRL         0xE1 // Negative Voltage Gamma Control
#define ST7789V_DGMLUTR           0xE2 // Digital Gamma Look-up Table for Red
#define ST7789V_DGMLUTB           0xE3 // Digital Gamma Look-up Table for Blue
#define ST7789V_GATECTRL          0xE4 // Gate Control
#define ST7789V_SPI2EN            0xE7 // SPI2 Enable
#define ST7789V_PWCTRL2           0xE8 // Power Control 2
#define ST7789V_EQCTRL            0xE9 // Equalize time control
#define ST7789V_PROMCTRL          0xEC // Program Mode Control
#define ST7789V_PROMEN            0xFA // Program Mode Enable
#define ST7789V_NVMSET            0xFC // NVM Setting
#define ST7789V_PROMACT           0xFE // Program action

static const uint16_t st7789v_init[] = {
  DATASIZE_8BIT,
  ESC_REG(ST7789V_SWRESET), ESC_DELAY(100),
  ESC_REG(ST7789V_SLPOUT), ESC_DELAY(20),

  ESC_REG(ST7789V_PORCTRL), 0x000C, 0x000C, 0x0000, 0x0033, 0x0033,
  ESC_REG(ST7789V_GCTRL), 0x0035,
  ESC_REG(ST7789V_VCOMS), 0x001F,
  ESC_REG(ST7789V_LCMCTRL), 0x002C,
  ESC_REG(ST7789V_VDVVRHEN), 0x0001, 0x00C3,
  ESC_REG(ST7789V_VDVS), 0x0020,
  ESC_REG(ST7789V_FRCTRL2), 0x000F,
  ESC_REG(ST7789V_PWCTRL1), 0x00A4, 0x00A1,

  ESC_REG(ST7789V_MADCTL), ST7789V_MADCTL_DATA,
  ESC_REG(ST7789V_COLMOD), 0x0055,

  ESC_REG(ST7789V_NORON),
  ESC_REG(ST7789V_DISPON),
  ESC_END
};
