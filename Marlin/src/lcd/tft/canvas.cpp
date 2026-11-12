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

#if ALL(TFT_COLOR_UI, AUTO_BED_LEVELING_UBL)
  #include "ui_common.h"   // HAS_TFT_TINY_FONT
  #include "tft_tiny_font.h"
#endif

uint16_t Canvas::width, Canvas::height;
uint16_t Canvas::startLine, Canvas::endLine;
uint16_t Canvas::background_color;
uint16_t *Canvas::buffer = TFT::buffer;

#if ENABLED(NOVIN3DP_GALAXY_UI)
  #include "galaxy/gx_core.h"
  uint16_t Canvas::originX, Canvas::originY;
#endif

void Canvas::instantiate(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  width = w;
  height = h;
  startLine = 0;
  endLine = 0;
  #if ENABLED(NOVIN3DP_GALAXY_UI)
    originX = x;
    originY = y;
  #endif

  // The TFT handles DMA within the given canvas rectangle
  // so whatever is drawn will be offset on the screen by x,y.
  tft.set_window(x, y, x + w - 1, y + h - 1);
}

void Canvas::next() {
  startLine = endLine;
  endLine = (TFT_BUFFER_WORDS) < width * (height - startLine) ? startLine + (TFT_BUFFER_WORDS) / width : height;
}

bool Canvas::toScreen() {
  tft.write_sequence(buffer, width * (endLine - startLine));
  return endLine == height;
}

void Canvas::setBackground(uint16_t color) {
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

void Canvas::addText(uint16_t x, uint16_t y, uint16_t color, uint16_t *string, uint16_t maxWidth) {
  if (endLine < y || startLine > y + getFontHeight()) return;

  if (maxWidth == 0) maxWidth = width - x;

  uint16_t colors[16];
  uint16_t stringWidth = 0;
  if (getFontType() == FONT_MARLIN_GLYPHS_2BPP) {
    for (uint8_t i = 0; i < 3; i++) {
      colors[i] = gradient(ENDIAN_COLOR(color), ENDIAN_COLOR(background_color), ((i+1) << 8) / 3);
      colors[i] = ENDIAN_COLOR(colors[i]);
    }
  }
  for (uint16_t i = 0 ; *(string + i) ; i++) {
    glyph_t *pGlyph = glyph(string + i);
    if (stringWidth + pGlyph->bbxWidth > maxWidth) break;
    switch (getFontType()) {
      case FONT_MARLIN_GLYPHS_1BPP:
        addImage(x + stringWidth + pGlyph->bbxOffsetX, y + getFontAscent() - pGlyph->bbxHeight - pGlyph->bbxOffsetY, pGlyph->bbxWidth, pGlyph->bbxHeight, GREYSCALE1, ((uint8_t *)pGlyph) + sizeof(glyph_t), &color);
        break;
      case FONT_MARLIN_GLYPHS_2BPP:
        #if ENABLED(NOVIN3DP_GALAXY_UI)
          // Blend antialiased edges with the pixels underneath (galaxy, panels)
          addGlyphBlend(x + stringWidth + pGlyph->bbxOffsetX, y + getFontAscent() - pGlyph->bbxHeight - pGlyph->bbxOffsetY, pGlyph->bbxWidth, pGlyph->bbxHeight, ((uint8_t *)pGlyph) + sizeof(glyph_t), color);
        #else
          addImage(x + stringWidth + pGlyph->bbxOffsetX, y + getFontAscent() - pGlyph->bbxHeight - pGlyph->bbxOffsetY, pGlyph->bbxWidth, pGlyph->bbxHeight, GREYSCALE2, ((uint8_t *)pGlyph) + sizeof(glyph_t), colors);
        #endif
        break;
    }
    stringWidth += pGlyph->dWidth;
  }
}

#if HAS_TFT_TINY_FONT

  // Draw text in the tiny digits font. Like addText(), but independent of the loaded menu font.
  void Canvas::addTinyText(uint16_t x, uint16_t y, uint16_t color, uint16_t *string) {
    if (endLine < y || startLine > y + TinyFont::height()) return;

    uint16_t colors[3];
    for (uint8_t i = 0; i < 3; i++) {
      colors[i] = gradient(ENDIAN_COLOR(color), ENDIAN_COLOR(background_color), ((i+1) << 8) / 3);
      colors[i] = ENDIAN_COLOR(colors[i]);
    }
    uint16_t stringWidth = 0;
    for (uint16_t i = 0; string[i]; i++) {
      const glyph_t * const pGlyph = TinyFont::glyph(string[i]);
      if (!pGlyph) continue;
      addImage(x + stringWidth + pGlyph->bbxOffsetX, y + TinyFont::ascent() - pGlyph->bbxHeight - pGlyph->bbxOffsetY,
               pGlyph->bbxWidth, pGlyph->bbxHeight, GREYSCALE2, ((uint8_t *)pGlyph) + sizeof(glyph_t), colors);
      stringWidth += pGlyph->dWidth;
    }
  }

#endif // HAS_TFT_TINY_FONT

void Canvas::addImage(int16_t x, int16_t y, MarlinImage image, uint16_t *colors) {
  uint16_t *data = (uint16_t *)images[image].data;
  if (!data) return;

  const uint16_t image_width = images[image].width,
                image_height = images[image].height;
  colorMode_t color_mode = images[image].colorMode;

  if (color_mode == HIGHCOLOR) {
    // HIGHCOLOR - 16 bits per pixel
    uint16_t yc = y <= startLine ? 0 : (y - startLine) * width;
    for (int16_t i = 0; i < image_height && y < endLine; i++, y++) {
      if (y >= startLine) {
        uint16_t *pixel = buffer + x + yc;
        yc += width;
        int16_t cx = x;
        for (int16_t j = 0; j < image_width && cx < width; j++, cx++, pixel++, data++) {
          if (cx < 0) continue;
          uint16_t color = ENDIAN_COLOR(*data);
          if (color == 0x0001) color = COLOR_BACKGROUND;
          *pixel = color;
        }
      }
      else
        data += image_width;
    }
    return;
  }

  #if ENABLED(COMPACT_MARLIN_BOOT_LOGO)

    static struct {
      bool has_rle_state = false;
      int16_t dstx, dsty, srcx, srcy;
      uint32_t rle_offset;
    } rle_state;

    #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
      static MarlinImage last_image = noImage;
    #endif

    // RLE16 HIGHCOLOR - 16 bits per pixel
    if (color_mode == RLE16) {
      uint8_t *bytedata = (uint8_t *)images[image].data;
      if (!bytedata) return;

      #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)           // Reset RLE state if a different image
        if (image != last_image) {
          rle_state.has_rle_state = false;
          last_image = image;
        }
      #endif

      // Loop through the image data advancing the row and column as needed
      int16_t srcy = 0, srcx = 0,                   // Image data line / column index
              dsty = y, dstx = x;                   // Destination line / column index

      uint16_t color = 0;                           // Persist the last fetched color value
      if (rle_state.has_rle_state) {                // do we have RLE position data?
        rle_state.has_rle_state = false;            // invalidate stored RLE state
        dstx = rle_state.dstx;                      // restore required states
        dsty = rle_state.dsty;
        srcx = rle_state.srcx;
        srcy = rle_state.srcy;
        bytedata = (uint8_t *)images[image].data + rle_state.rle_offset;  // Restart decode from here instead of the start of data
      }

      bool done = false;
      while (!done) {
        if (dsty >= endLine - 1 || srcy >= image_height - 1) { // Store state?
          rle_state.dstx = dstx;                    // Save required states
          rle_state.dsty = dsty;
          rle_state.srcx = srcx;
          rle_state.srcy = srcy;
          rle_state.rle_offset = bytedata - (uint8_t *)images[image].data;; // Keep these for skipping full RLE decode on future iterations
        }

        uint8_t count = *bytedata++;                // Get the count byte
        const bool uniq = bool(count & 0x80);       // >= 128 is a distinct run; < 128 is a repeat run
        count = (count & 0x7F) + 1;                 // Actual count is 7-bit plus 1

        bool getcol = true;                         // Get at least one color word
        while (count--) {                           // Emit 'count' pixels

          if (getcol) {
            getcol = uniq;                          // Keep getting colors if not RLE
            const uint16_t msb = *bytedata++,       // Color most-significant bits
                           lsb = *bytedata++;       // Color least-significant bits
            color = ENDIAN_COLOR((msb << 8) | lsb); // Color with proper endianness
            if (color == 0x0001) color = COLOR_BACKGROUND; // 0x0001 is "transparent"
          }

          if (WITHIN(dsty, startLine, endLine - 1)) { // Dest pixel Y within the segment?
            if (WITHIN(dstx, 0, width - 1)) {       // Dest pixel X within the canvas?
              uint16_t * const pixel = buffer + dstx + (dsty - startLine) * width;
              *pixel = color;                       // Store the color in the pixel
            }
          }

          ++srcx; ++dstx;                           // Advance the pixel column
          if (srcx >= image_width) {                // Past the right edge of the source image?
            ++srcy; ++dsty;                         // Advance to the next line
            srcx = 0; dstx = x;                     // May be shifted within the canvas, but usually not
            if (dsty >= endLine || srcy >= image_height) { // Done with the segment or the image?
              done = true;                          // Set a flag to end the loop...
              rle_state.has_rle_state = true;       // RLE state is stored
              break;                                // ...and break out of while(count--)
            }
          }
        }
      }
      return;
    }
  #endif // COMPACT_MARLIN_BOOT_LOGO

  addImage(x, y, image_width, image_height, color_mode, (uint8_t *)data, colors);
}

void Canvas::addImage(int16_t x, int16_t y, uint8_t image_width, uint8_t image_height, colorMode_t color_mode, uint8_t *data, uint16_t *colors) {
  uint8_t bitsPerPixel;
  switch (color_mode) {
    case GREYSCALE1: bitsPerPixel = 1; break;
    case GREYSCALE2: bitsPerPixel = 2; break;
    case GREYSCALE4: bitsPerPixel = 4; break;
    default: return;
  }
  const uint8_t obase = 8 - bitsPerPixel, mask = 0xFF >> obase, pixelsPerByte = 8 / bitsPerPixel;
  const uintptr_t span = (image_width + pixelsPerByte - 1) / pixelsPerByte;

  colors--;                                                 // Color 1 is at index 0

  uint16_t yc = y <= startLine ? 0 : (y - startLine) * width; // Multiple of width as y offset
  for (int16_t i = 0; i < image_height && y < endLine; i++, y++) { // Loop through image lines
    if (y >= startLine) {                                   // Within the canvas?
      uint16_t *pixel = buffer + x + yc;                    // Pixel address of line at x pos
      yc += width;                                          // + is faster than *
      int8_t offset = obase;                                // Bit offset of incoming pixel
      for (int16_t j = 0; j < image_width; j++, pixel++) {  // Loop through image pixels
        if (offset < 0) { data++; offset = obase; }         // Got all pixels in the byte? next byte.
        if (WITHIN(x + j, 0, width - 1)) {                  // Within the canvas?
          const uint8_t ci = ((*data) >> offset) & mask;    // Shift the color index to low bits
          if (ci) *pixel = colors[ci];                      // Draw a solid pixel with the indexed color
        }
        offset -= bitsPerPixel;                             // Subtract bits used for the pixel
      }
      data++;                                               // New line, so new set of pixels
    }
    else
      data += span;                                         // Skip line outside the canvas
  }
}

void Canvas::addRect(uint16_t x, uint16_t y, uint16_t rectangleWidth, uint16_t rectangleHeight, uint16_t color) {
  if (endLine < y || startLine > y + rectangleHeight) return;           // Nothing to draw?

  uint16_t yc = y <= startLine ? 0 : (y - startLine) * width;           // Multiple of width as y offset
  for (uint16_t i = 0; i < rectangleHeight && y < endLine; i++, y++) {  // Loop over the rect height
    if (y >= startLine) {                                               // Within the canvas?
      uint16_t *pixel = buffer + x + yc;                                // Pixel address of line at x pos
      yc += width;                                                      // + is faster than *
      if (i == 0 || i == rectangleHeight - 1) {                         // Top or bottom line?
        for (uint16_t j = 0; j < rectangleWidth; j++) *pixel++ = color; // Fill the width. (No transparency)
      }
      else {
        pixel[0] = color;                                               // Left line
        pixel[rectangleWidth - 1] = color;                              // Right line
      }
    }
  }
}

void Canvas::addBar(uint16_t x, uint16_t y, uint16_t barWidth, uint16_t barHeight, uint16_t color) {
  if (endLine < y || startLine > y + barHeight) return;           // Nothing to draw?

  uint16_t yc = y <= startLine ? 0 : (y - startLine) * width;     // Multiple of width as y offset
  for (uint16_t i = 0; i < barHeight && y < endLine; i++, y++) {  // Loop over the bar height
    if (y >= startLine) {                                         // Within the canvas?
      uint16_t *pixel = buffer + x + yc;                          // Pixel address of line at x pos
      yc += width;                                                // + is faster than *
      for (uint16_t j = 0; j < barWidth; j++) *pixel++ = color;   // Fill the width. (No transparency)
    }
  }
}

#if ENABLED(NOVIN3DP_GALAXY_UI)

  void Canvas::setGalaxyBackground() {
    for (uint16_t line = startLine; line < endLine; line++)
      gx_fill_galaxy_row(buffer + (line - startLine) * width, originX, originY + line, width);
    background_color = gx_galaxy_avg(originY + (startLine + endLine) / 2);
  }

  void Canvas::addGlyphBlend(int16_t x, int16_t y, uint8_t glyphWidth, uint8_t glyphHeight, const uint8_t *data, uint16_t color) {
    static constexpr uint8_t alpha[4] = { 0, 96, 176, 255 };
    const uint16_t rawColor = ENDIAN_COLOR(color);
    for (int16_t i = 0; i < glyphHeight; i++) {
      const int16_t line = y + i;
      const uint8_t *row = data + i * ((glyphWidth + 3) >> 2);
      if (!WITHIN(line, startLine, endLine - 1)) continue;
      uint16_t *pixel = buffer + x + (line - startLine) * width;
      for (int16_t j = 0; j < glyphWidth; j++, pixel++) {
        const uint8_t a = (row[j >> 2] >> (6 - ((j & 3) << 1))) & 0x03;
        if (!a || !WITHIN(x + j, 0, width - 1)) continue;
        *pixel = a == 3 ? color : ENDIAN_COLOR(gx_blend(ENDIAN_COLOR(*pixel), rawColor, alpha[a]));
      }
    }
  }

  // Horizontal inset of a rounded corner on panel row i (0..h-1)
  static uint8_t corner_inset(const uint8_t r, const int16_t i, const uint16_t h) {
    int16_t d = 0;
    if (i < r) d = r - i;
    else if (i >= int16_t(h) - r) d = i - (int16_t(h) - r) + 1;
    if (d <= 0) return 0;
    const float dy = d - 0.5f;
    return uint8_t(r - sqrtf(float(r * r) - dy * dy) + 0.5f);
  }

  void Canvas::addPanel(int16_t x, int16_t y, uint16_t panelWidth, uint16_t panelHeight, uint16_t fill, uint16_t border, uint8_t radius, uint8_t flags) {
    if (endLine <= y || startLine >= y + panelHeight) return;
    const uint16_t eFill = ENDIAN_COLOR(fill), eBorder = ENDIAN_COLOR(border);
    for (int16_t i = 0; i < int16_t(panelHeight); i++) {
      const int16_t line = y + i;
      if (!WITHIN(line, startLine, endLine - 1)) continue;
      uint16_t *row = buffer + (line - startLine) * width;
      const uint8_t inset = corner_inset(radius, i, panelHeight);
      const int16_t x0 = x + inset, x1 = x + panelWidth - 1 - inset;

      if (flags & (GXP_BLEND | GXP_SOLID)) {
        for (int16_t px = _MAX(x0, int16_t(0)); px <= _MIN(x1, int16_t(width - 1)); px++)
          row[px] = (flags & GXP_SOLID) ? eFill : ENDIAN_COLOR(gx_half(ENDIAN_COLOR(row[px]), fill));
      }

      if (flags & GXP_BORDER) {
        if (i == 0 || i == int16_t(panelHeight) - 1) {
          for (int16_t px = _MAX(x0, int16_t(0)); px <= _MIN(x1, int16_t(width - 1)); px++) row[px] = eBorder;
        }
        else {
          uint8_t nb = 0;
          if (i > 0) NOLESS(nb, corner_inset(radius, i - 1, panelHeight));
          if (i < int16_t(panelHeight) - 1) NOLESS(nb, corner_inset(radius, i + 1, panelHeight));
          const int16_t span = _MAX(int16_t(0), int16_t(nb) - int16_t(inset) - 1);
          for (int16_t k = 0; k <= span; k++) {
            if (WITHIN(x0 + k, 0, width - 1)) row[x0 + k] = eBorder;
            if (WITHIN(x1 - k, 0, width - 1)) row[x1 - k] = eBorder;
          }
        }
      }
    }
  }

  void Canvas::addBitmap(int16_t x, int16_t y, uint8_t bitmapWidth, uint8_t bitmapHeight, const uint8_t *data, uint16_t color) {
    if (!data || endLine <= y || startLine >= y + bitmapHeight) return;
    const uint16_t rowBytes = (bitmapWidth + 7) >> 3, eColor = ENDIAN_COLOR(color);
    for (int16_t i = 0; i < bitmapHeight; i++) {
      const int16_t line = y + i;
      if (!WITHIN(line, startLine, endLine - 1)) continue;
      const uint8_t *src = data + i * rowBytes;
      uint16_t *row = buffer + (line - startLine) * width;
      for (int16_t j = 0; j < bitmapWidth; j++)
        if ((src[j >> 3] & (0x80 >> (j & 7))) && WITHIN(x + j, 0, width - 1)) row[x + j] = eColor;
    }
  }

#endif // NOVIN3DP_GALAXY_UI

Canvas tftCanvas;

#endif // HAS_GRAPHICAL_TFT
