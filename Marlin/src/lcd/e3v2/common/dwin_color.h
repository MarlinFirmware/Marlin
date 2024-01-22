/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// Extended and default UI Colors
#define RGB(R,G,B)  (R << 11) | (G << 5) | (B) // R: 0..31, G: 0..63, B: 0..31
#define GetRColor(color) ((color >> 11) & 0x1F)
#define GetGColor(color) ((color >>  5) & 0x3F)
#define GetBColor(color) ((color >>  0) & 0x1F)

// RGB565 colors: https://rgbcolorpicker.com/565
#define COLOR_WHITE         RGB(0x1F, 0x3F, 0x1F)
#define COLOR_YELLOW        RGB(0x1F, 0x3F, 0x00)
#define COLOR_RED           RGB(0x1F, 0x00, 0x00)
#define COLOR_ERROR_RED     RGB(0x16, 0x00, 0x00)  // Error!
#define COLOR_BG_RED        RGB(0x1E, 0x00, 0x0F)  // Red background color
#define COLOR_BG_WINDOW     RGB(0x06, 0x0F, 0x08)  // Popup background color
#define COLOR_BG_BLUE       RGB(0x02, 0x09, 0x05)  // Dark blue background color
#define COLOR_BG_BLACK      RGB(0x01, 0x02, 0x01)  // Black background color
#define COLOR_ICONBLUE      RGB(0x08, 0x2F, 0x1A)  // Lighter blue that matches icons/accents
#define COLOR_POPUP_TEXT    RGB(0x1A, 0x35, 0x1A)  // Popup font background color
#define COLOR_LINE          RGB(0x07, 0x13, 0x0A)  // Split line color
#define COLOR_RECTANGLE     RGB(0x1D, 0x31, 0x0F)  // Blue square cursor color
#define COLOR_PERCENT       RGB(0x1F, 0x31, 0x09)  // Percentage color
#define COLOR_BARFILL       RGB(0x02, 0x07, 0x04)  // Fill color of progress bar
#define COLOR_SELECT        RGB(0x06, 0x1D, 0x1B)  // Selected color
