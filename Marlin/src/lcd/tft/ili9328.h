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

#define ILI9328_DRVCTL_SM         0x0400
#define ILI9328_DRVCTL_SS         0x0100 // Select the shift direction of outputs from the source driver. 0 - from S1 to S720 / 1 - from S720 to S1

#define ILI9328_GATE_SCANCTL_GS   0x8000 // Sets the direction of scan by the gate driver in the range determined by SCN[4:0] and NL[4:0].

#define ILI9328_ETMOD_TRI         0x8000
#define ILI9328_ETMOD_DFM         0x4000
#define ILI9328_ETMOD_BGR         0x1000 // RGB-BGR ORDER
#define ILI9328_ETMOD_RGB         0x0000
#define ILI9328_ETMOD_ORG         0x0080
#define ILI9328_ETMOD_ID1         0x0020 // 0 - Vertical Decrement / 1 - Vertical Increment
#define ILI9328_ETMOD_ID0         0x0010 // 0 - Horizontal Decrement / 1 - Horizontal Increment
#define ILI9328_ETMOD_AM          0x0008 // 0 - Horizontal / 1 - Vertical

// MKS Robin TFT v1.1 - 320x240 ; Cable on the left side
#define ILI9328_DRVCTL_DATA       ILI9328_DRVCTL_SS
#define ILI9328_ETMOD_DATA        ILI9328_ETMOD_BGR | ILI9328_ETMOD_ID1 | ILI9328_ETMOD_ID0 | ILI9328_ETMOD_AM

#define ILI9328_RDDID             0x00 // ID code - 0x9328
#define ILI9328_DRVCTL            0x01 // Driver Output Control
#define ILI9328_LCDCTL            0x02 // LCD Driving Wave Control
#define ILI9328_ETMOD             0x03 // Entry Mode - Control the GRAM update direction
#define ILI9328_RESIZECTL         0x04 // Resizing Control Register
#define ILI9328_DISCTRL1          0x07 // Display Control 1
#define ILI9328_DISCTRL2          0x08 // Display Control 2
#define ILI9328_DISCTRL3          0x09 // Display Control 3
#define ILI9328_DISCTRL4          0x0A // Display Control 4
#define ILI9328_RGBCTRL1          0x0C // RGB Display Interface Control 1
#define ILI9328_FMARKERPOS        0x0D // Frame Marker Position
#define ILI9328_RGBCTRL2          0x0F // RGB Display Interface Control 2
#define ILI9328_PWCTRL1           0x10 // Power Control 1
#define ILI9328_PWCTRL2           0x11 // Power Control 2
#define ILI9328_PWCTRL3           0x12 // Power Control 3
#define ILI9328_PWCTRL4           0x13 // Power Control 4

// With landscape screen orientation 'Horizontal' is Y and 'Vertical' is X
#define ILI9328_HASET             0x20 // GRAM Horizontal Address Set (0-255)
#define ILI9328_VASET             0x21 // GRAM Vertical Address Set (0-511)
#define ILI9328_RAMWR             0x22 // Write data to GRAM
#define ILI9328_RAMRD             0x22 // Read Data from GRAM

#define ILI9328_PWCTRL7           0x29 // Power Control 7
#define ILI9328_FRMCTR            0x2B // Frame Rate and Color Control
#define ILI9328_GAMCTRL1          0x30 // Gamma Control
#define ILI9328_GAMCTRL2          0x31 // Gamma Control
#define ILI9328_GAMCTRL3          0x32 // Gamma Control
#define ILI9328_GAMCTRL4          0x35 // Gamma Control
#define ILI9328_GAMCTRL5          0x36 // Gamma Control
#define ILI9328_GAMCTRL6          0x37 // Gamma Control
#define ILI9328_GAMCTRL7          0x38 // Gamma Control
#define ILI9328_GAMCTRL8          0x39 // Gamma Control
#define ILI9328_GAMCTRL9          0x3C // Gamma Control
#define ILI9328_GAMCTRLA          0x3D // Gamma Control

// With landscape screen orientation 'Horizontal' is Y and 'Vertical' is X
#define ILI9328_HASTART           0x50 // Horizontal Address Start Position (0-255)
#define ILI9328_HAEND             0x51 // Horizontal Address End Position (0-255)
#define ILI9328_VASTART           0x52 // Vertical Address Start Position (0-511)
#define ILI9328_VAEND             0x53 // Vertical Address End Position (0-511)

#define ILI9328_GATE_SCANCTL1     0x60 // Gate Scan Control
#define ILI9328_GATE_SCANCTL2     0x61 // Gate Scan Control
#define ILI9328_GATE_SCANCTL3     0x6A // Gate Scan Control

#define ILI9328_PLTPOS1           0x80 // Partial Image 1 Display Position
#define ILI9328_PLTSTART1         0x81 // Partial Image 1 RAM Start Address
#define ILI9328_PLTEND1           0x82 // Partial Image 1 RAM End Address
#define ILI9328_PLTPOS2           0x83 // Partial Image 2 Display Position
#define ILI9328_PLTSTART2         0x84 // Partial Image 2 RAM Start Address
#define ILI9328_PLTEND2           0x85 // Partial Image 2 RAM End Address

#define ILI9328_IFCTL1            0x90 // Panel Interface Control 1
#define ILI9328_IFCTL2            0x92 // Panel Interface Control 2
#define ILI9328_IFCTL4            0x95 // Panel Interface Control 4
#define ILI9328_IFCTL5            0x97 // Panel Interface Control 5

#define ILI9328_OTPWR             0xA1 // OTP VCM Programming Control
#define ILI9328_RDOTP             0xA2 // OTP VCM Status and Enable
#define ILI9328_OTPPKEY           0xA5 // OTP Programming ID Key


static const uint16_t ili9328_init[] = {
  DATASIZE_16BIT,
  ESC_REG(ILI9328_DRVCTL), ILI9328_DRVCTL_DATA,
  ESC_REG(ILI9328_LCDCTL), 0x0400,              // LCD Driving Wave Control
  ESC_REG(ILI9328_ETMOD), ILI9328_ETMOD_DATA,

  ESC_REG(ILI9328_RESIZECTL), 0x0000,
  ESC_REG(ILI9328_DISCTRL2), 0x0202,
  ESC_REG(ILI9328_DISCTRL3), 0x0000,
  ESC_REG(ILI9328_DISCTRL4), 0x0000,
  ESC_REG(ILI9328_RGBCTRL1), 0x0000,
  ESC_REG(ILI9328_FMARKERPOS), 0x0000,
  ESC_REG(ILI9328_RGBCTRL2), 0x0000,
  ESC_REG(ILI9328_PWCTRL1), 0x0000,
  ESC_REG(ILI9328_PWCTRL2), 0x0007,
  ESC_REG(ILI9328_PWCTRL3), 0x0000,
  ESC_REG(ILI9328_PWCTRL4), 0x0000,
  ESC_REG(ILI9328_DISCTRL1), 0x0001,
  ESC_DELAY(200),
  ESC_REG(ILI9328_PWCTRL1), 0x1690,
  ESC_REG(ILI9328_PWCTRL2), 0x0227,
  ESC_DELAY(50),
  ESC_REG(ILI9328_PWCTRL3), 0x008C,
  ESC_DELAY(50),
  ESC_REG(ILI9328_PWCTRL4), 0x1500,
  ESC_REG(ILI9328_PWCTRL7), 0x0004,
  ESC_REG(ILI9328_FRMCTR), 0x000D,
  ESC_DELAY(50),
  ESC_REG(ILI9328_GATE_SCANCTL1), 0x2700,
  ESC_REG(ILI9328_GATE_SCANCTL2), 0x0001,
  ESC_REG(ILI9328_GATE_SCANCTL3), 0x0000,
  ESC_REG(ILI9328_PLTPOS1), 0x0000,
  ESC_REG(ILI9328_PLTSTART1), 0x0000,
  ESC_REG(ILI9328_PLTEND1), 0x0000,
  ESC_REG(ILI9328_PLTPOS2), 0x0000,
  ESC_REG(ILI9328_PLTSTART2), 0x0000,
  ESC_REG(ILI9328_PLTEND2), 0x0000,
  ESC_REG(ILI9328_IFCTL1), 0x0010,
  ESC_REG(ILI9328_IFCTL2), 0x0600,
  ESC_REG(ILI9328_DISCTRL1), 0x0133,
  ESC_END
};
