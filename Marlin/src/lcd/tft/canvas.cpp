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

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_TFT

#include "canvas.h"

uint16_t CANVAS::width, CANVAS::height;
uint16_t CANVAS::startLine, CANVAS::endLine;
uint16_t CANVAS::background_color;
uint16_t *CANVAS::buffer = TFT::buffer;

void CANVAS::New(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  CANVAS::width = width;
  CANVAS::height = height;
  startLine = 0;
  endLine = 0;

  tft.set_window(x, y, x + width - 1, y + height - 1);
}

void CANVAS::Continue() {
  startLine = endLine;
  endLine = TFT_BUFFER_SIZE < width * (height - startLine) ? startLine + TFT_BUFFER_SIZE / width : height;
}

bool CANVAS::ToScreen() {
  tft.write_sequence(buffer, width * (endLine - startLine));
  return endLine == height;
}

void CANVAS::SetBackground(uint16_t color) {
  /* TODO: test and optimize performance */
  /*
  uint32_t count = (endLine - startLine) * width;
  uint16_t *pixel = buffer;
  while (count--)
    *pixel++ = color;
  */
  const uint32_t two_pixels = (((uint32_t )color) << 16) | color;
  uint32_t count = ((endLine - startLine) * width + 1) >> 1;
  uint32_t *pointer = (uint32_t *)buffer;
  while (count--) *pointer++ = two_pixels;
  background_color = color;
}

extern uint16_t gradient(uint16_t colorA, uint16_t colorB, uint16_t factor);

void CANVAS::AddText(uint16_t x, uint16_t y, uint16_t color, uint16_t *string, uint16_t maxWidth) {
  if (endLine < y || startLine > y + GetFontHeight()) return;

  if (maxWidth == 0) maxWidth = width - x;

  uint16_t colors[16];
  uint16_t stringWidth = 0;
  for (uint16_t i = 0 ; *(string + i) ; i++) {
    glyph_t *glyph = Glyph(string + i);
    if (stringWidth + glyph->BBXWidth > maxWidth) break;
    switch (GetFontType()) {
      case FONT_MARLIN_GLYPHS_1BPP:
        AddImage(x + stringWidth + glyph->BBXOffsetX, y + GetFontAscent() - glyph->BBXHeight - glyph->BBXOffsetY, glyph->BBXWidth, glyph->BBXHeight, GREYSCALE1, ((uint8_t *)glyph) + sizeof(glyph_t), &color);
        break;
      case FONT_MARLIN_GLYPHS_2BPP:
        for (uint8_t i = 0; i < 3; i++)
          colors[i] = gradient(color, background_color, ((i+1) << 8) / 3);
        AddImage(x + stringWidth + glyph->BBXOffsetX, y + GetFontAscent() - glyph->BBXHeight - glyph->BBXOffsetY, glyph->BBXWidth, glyph->BBXHeight, GREYSCALE2, ((uint8_t *)glyph) + sizeof(glyph_t), colors);
        break;
    }
    stringWidth += glyph->DWidth;
  }
}

void CANVAS::AddImage(int16_t x, int16_t y, MarlinImage image, uint16_t *colors) {
  uint16_t *data = (uint16_t *)Images[image].data;
  if (!data) return;

  uint16_t image_width = Images[image].width,
           image_height = Images[image].height;
  colorMode_t color_mode = Images[image].colorMode;

  if (color_mode != HIGHCOLOR)
    return AddImage(x, y, image_width, image_height, color_mode, (uint8_t *)data, colors);

  // HIGHCOLOR - 16 bits per pixel

  for (int16_t i = 0; i < image_height; i++) {
    int16_t line = y + i;
    if (line >= startLine && line < endLine) {
      uint16_t *pixel = buffer + x + (line - startLine) * width;
      for (int16_t j = 0; j < image_width; j++) {
        if ((x + j >= 0) && (x + j < width)) *pixel = ENDIAN_COLOR(*data);
        pixel++;
        data++;
      }
    }
    else
      data += image_width;
  }
}

void CANVAS::AddImage(int16_t x, int16_t y, uint8_t image_width, uint8_t image_height, colorMode_t color_mode, uint8_t *data, uint16_t *colors) {
  uint8_t bitsPerPixel;
  switch (color_mode) {
    case GREYSCALE1: bitsPerPixel = 1; break;
    case GREYSCALE2: bitsPerPixel = 2; break;
    case GREYSCALE4: bitsPerPixel = 4; break;
    default: return;
  }

  uint8_t mask = 0xFF >> (8 - bitsPerPixel),
          pixelsPerByte = 8 / bitsPerPixel;

  colors--;

  for (int16_t i = 0; i < image_height; i++) {
    int16_t line = y + i;
    if (line >= startLine && line < endLine) {
      uint16_t *pixel = buffer + x + (line - startLine) * width;
      uint8_t offset = 8 - bitsPerPixel;
      for (int16_t j = 0; j < image_width; j++) {
        if (offset > 8) {
          data++;
          offset = 8 - bitsPerPixel;
        }
        if ((x + j >= 0) && (x + j < width)) {
          const uint8_t color = ((*data) >> offset) & mask;
          if (color) *pixel = *(colors + color);
        }
        pixel++;
        offset -= bitsPerPixel;
      }
      data++;
    }
    else
      data += (image_width + pixelsPerByte - 1) / pixelsPerByte;
  }
}

void CANVAS::AddRectangle(uint16_t x, uint16_t y, uint16_t rectangleWidth, uint16_t rectangleHeight, uint16_t color) {
  if (endLine < y || startLine > y + rectangleHeight) return;

  for (uint16_t i = 0; i < rectangleHeight; i++) {
    uint16_t line = y + i;
    if (line >= startLine && line < endLine) {
      uint16_t *pixel = buffer + x + (line - startLine) * width;
      if (i == 0 || i == rectangleHeight - 1) {
        for (uint16_t j = 0; j < rectangleWidth; j++) *pixel++ = color;
      }
      else {
        *pixel = color;
        pixel += rectangleWidth - 1;
        *pixel = color;
      }
    }
  }
}

void CANVAS::AddBar(uint16_t x, uint16_t y, uint16_t barWidth, uint16_t barHeight, uint16_t color) {
  if (endLine < y || startLine > y + barHeight) return;

  for (uint16_t i = 0; i < barHeight; i++) {
    uint16_t line = y + i;
    if (line >= startLine && line < endLine) {
      uint16_t *pixel = buffer + x + (line - startLine) * width;
      for (uint16_t j = 0; j < barWidth; j++) *pixel++ = color;
    }
  }
}

CANVAS Canvas;

#endif // HAS_GRAPHICAL_TFT
