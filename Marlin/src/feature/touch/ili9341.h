/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <stdint.h>

// ILI9341 Registers
// cf. https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf

#define ILI9341_SWRESET            0x01 /* Software Reset */
#define ILI9341_SLEEP_IN           0x10
#define ILI9341_SLEEP_OUT          0x11
#define ILI9341_PARTIAL_DISPLAY    0x12
#define ILI9341_DISPLAY_INVERSION  0x21
#define ILI9341_GAMSET             0x26 /* Gamma Set */
#define ILI9341_DISPLAY_OFF        0x28
#define ILI9341_DISPLAY_ON         0x29
#define ILI9341_WRITE_RAM          0x2C
#define ILI9341_READ_RAM           0x2E
#define ILI9341_CASET              0x2A
#define ILI9341_RASET              0x2B /* Page Address Set (PASET) */
#define ILI9341_VSCRDEF            0x33 /* Vertical Scroll Definition */
#define ILI9341_VSCSAD             0x37 /* Vertical Scroll Start Address of RAM */
#define ILI9341_TEARING_EFFECT     0x35
#define ILI9341_NORMAL_DISPLAY     0x36
#define ILI9341_IDLE_MODE_OFF      0x38
#define ILI9341_IDLE_MODE_ON       0x39
#define ILI9341_COLOR_MODE         0x3A /* Pixel Format Set (COLMOD) */
#define ILI9341_FRMCTRL            0xB1 /* Frame Rate Control (Normal colors) */
#define ILI9341_BLKING_PORCH_CTRL  0xB5
#define ILI9341_POWER_CTRL1        0xC0 /* PWCTRL1 Set the GVDD level */
#define ILI9341_POWER_CTRL2        0xC1 /* PWCTRL2 Factor in the step-up circuit */
#define ILI9341_VCOM_CTRL1         0xC5 /* VMCTRL1 VCOMH voltage */
#define ILI9341_VCOM_CTRL2         0xC7 /* VMCTRL2 VCOM offset voltage */
#define ILI9341_LCD_ID             0xD3 /* ID register for the ILI9341 */
#define ILI9341_GMCTRP1            0xE0 /* Positive Gamma Correction */
#define ILI9341_GMCTRN1            0xE1 /* Negative Gamma Correction */

void ILI9341_Init();
void ILI9341_DisplayOn();
void ILI9341_WriteRam();
void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos);
void ILI9341_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax = 319, uint16_t Ymax = 239);

/*
  ??? 0xEF, 3, 0x03, 0x80, 0x02,
  ??? 0xCF, 3, 0x00, 0xC1, 0x30,
  ??? 0xED, 4, 0x64, 0x03, 0x12, 0x81,
  ??? 0xE8, 3, 0x85, 0x00, 0x78,
  ??? 0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  ??? 0xF7, 1, 0x20,
  ??? 0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTRL1 , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTRL2 , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTRL1 , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTRL2 , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTRL , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMSET  , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  */
