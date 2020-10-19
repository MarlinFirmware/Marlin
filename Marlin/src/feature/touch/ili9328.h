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

// ILI9328 Registers (X/Y reversed for Marlin landscape use vs datasheet V/H)
// cf. https://cdn-shop.adafruit.com/datasheets/ILI9328.pdf

#define ILI9328_YSET               0x20 /* GRAM Y Address Set (0-255) */
#define ILI9328_XSET               0x21 /* GRAM X Address Set (0-511) */
#define ILI9328_GRAM               0x22 /* Write Data to GRAM (or read) */
#define ILI9328_RASET_MIN          0x50 /* RAM Address Position X */
#define ILI9328_RASET_MAX          0x51
#define ILI9328_CASET_MIN          0x52 /* RAM Address Position Y */
#define ILI9328_CASET_MAX          0x53

void ili9328_Init(void);
void ili9328_DisplayOn(void);
void ili9328_WriteRam(void);
void ili9328_SetCursor(uint16_t Xpos, uint16_t Ypos);
void ili9328_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax = 319, uint16_t Ymax = 239);

