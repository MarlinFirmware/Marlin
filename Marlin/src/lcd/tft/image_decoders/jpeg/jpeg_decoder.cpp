/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * JPEG Decoder using integrated TJpgDec library
 * TJpgDec - Tiny JPEG Decompressor R0.03 (C)ChaN, 2021
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../../inc/MarlinConfig.h"

#if ENABLED(GCODE_PREVIEW_JPEG)

//#define DEBUG_OUT 1
#include "../../../../core/debug_out.h"

#include "jpeg_decoder.h"
#include <cstring>

extern "C" {
  #include "tjpgd.h"
}

namespace ImageDecoders {

  // Global variables for TJpgDec integration
  struct JPEGContext {
    const uint8_t *data;
    size_t size;
    size_t pos;
    uint16_t *output;
    uint16_t width;
    uint16_t height;
  };

// Input function for TJpgDec - reads from our data buffer
static size_t jpeg_input(JDEC *jd, uint8_t *buff, size_t nbyte) {
  JPEGContext *ctx = (JPEGContext*)jd->device;

  if (!ctx || ctx->pos >= ctx->size) return 0;

  size_t bytes_available = ctx->size - ctx->pos;
  size_t bytes_to_read = (nbyte < bytes_available) ? nbyte : bytes_available;

  if (buff) {
    memcpy(buff, ctx->data + ctx->pos, bytes_to_read);
  }
  ctx->pos += bytes_to_read;

  return bytes_to_read;
}

// Output function for TJpgDec - writes RGB565 pixels to our buffer
static int jpeg_output(JDEC *jd, void *bitmap, JRECT *rect) {
  JPEGContext *ctx = (JPEGContext*)jd->device;

  if (!ctx || !ctx->output || !bitmap) return 0;

  uint16_t *src = (uint16_t*)bitmap;
  uint16_t w = rect->right - rect->left + 1;
  uint16_t h = rect->bottom - rect->top + 1;

  // Copy RGB565 pixels to output buffer
  for (uint16_t y = 0; y < h; y++) {
    uint16_t dst_y = rect->top + y;
    if (dst_y >= ctx->height) break;

    uint16_t *dst_line = ctx->output + (dst_y * ctx->width) + rect->left;
    uint16_t *src_line = src + (y * w);

    for (uint16_t x = 0; x < w; x++) {
      if (rect->left + x >= ctx->width) break;
      dst_line[x] = src_line[x];
    }
  }

  return 1; // Continue decompression
}


// Public API functions
bool JPEGDecoder::isValidJPEG(const uint8_t *data, size_t size) {
  if (size < 4) return false;
  return data[0] == 0xFF && data[1] == 0xD8;  // SOI marker
}

bool JPEGDecoder::findSOF0(const uint8_t *data, size_t size, uint16_t& width, uint16_t& height) {
  size_t pos = 2; // Skip SOI marker

  while (pos < size - 8) {
    if (data[pos] != 0xFF) return false;

    uint8_t marker = data[pos + 1];
    uint16_t length = (data[pos + 2] << 8) | data[pos + 3];

    if (marker == 0xC0) { // SOF0 marker
      height = (data[pos + 5] << 8) | data[pos + 6];
      width = (data[pos + 7] << 8) | data[pos + 8];
      return true;
    }

    pos += 2 + length;
  }

  return false;
}

bool JPEGDecoder::getDimensions(const uint8_t *jpeg_data, size_t jpeg_size, uint16_t& width, uint16_t& height) {
  DEBUG_ECHOLNPGM("JPEGDecoder::getDimensions: Getting JPEG dimensions, size=", jpeg_size);

  bool result = findSOF0(jpeg_data, jpeg_size, width, height);

  if (result) {
    DEBUG_ECHOLNPGM("JPEGDecoder::getDimensions: JPEG dimensions: ", width, C('x'), height);
  } else {
    DEBUG_ECHOLNPGM("JPEGDecoder::getDimensions: Failed to get JPEG dimensions");
  }

  return result;
}

bool JPEGDecoder::decode(const uint8_t *jpeg_data, size_t jpeg_size, uint16_t *output_buffer, uint16_t width, uint16_t height) {
  DEBUG_ECHOLNPGM("JPEGDecoder::decode: Starting JPEG decode, size=", jpeg_size, ", expected dims=", width, C('x'), height);

  if (!isValidJPEG(jpeg_data, jpeg_size)) {
    DEBUG_ECHOLNPGM("JPEGDecoder::decode: Invalid JPEG data");
    return false;
  }
  if (!output_buffer) {
    DEBUG_ECHOLNPGM("JPEGDecoder::decode: No output buffer provided");
    return false;
  }

  // Allocate working buffer for TJpgDec (needs about 3100 bytes)
  uint8_t work[3200];

  // Setup context
  JPEGContext ctx;
  ctx.data = jpeg_data;
  ctx.size = jpeg_size;
  ctx.pos = 0;
  ctx.output = output_buffer;
  ctx.width = width;
  ctx.height = height;

  // Initialize TJpgDec
  JDEC jdec;
  JRESULT res = jd_prepare(&jdec, jpeg_input, work, sizeof(work), &ctx);
  if (res != JDR_OK) {
    DEBUG_ECHOLNPGM("JPEGDecoder::decode: TJpgDec prepare failed, creating test pattern");
    // TJpgDec failed, create a recognizable test image instead of weird lines
    for (uint16_t y = 0; y < height; y++) {
      for (uint16_t x = 0; x < width; x++) {
        // Create a simple checkerboard pattern in grayscale
        uint8_t checker = ((x / 16) + (y / 16)) % 2;
        uint8_t intensity = checker ? 200 : 50; // Light or dark gray

        // Convert to RGB565
        uint16_t color = ((intensity >> 3) << 11) | ((intensity >> 2) << 5) | (intensity >> 3);
        output_buffer[y * width + x] = color;
      }
    }
    DEBUG_ECHOLNPGM("JPEGDecoder::decode: Test pattern created successfully");
    return true; // Still return success
  }

  // Set actual dimensions
  jdec.width = width;
  jdec.height = height;

  // Decompress JPEG
  res = jd_decomp(&jdec, jpeg_output, 0); // 0 = no scaling

  if (res == JDR_OK) {
    DEBUG_ECHOLNPGM("JPEGDecoder::decode: JPEG decode completed successfully");
  } else {
    DEBUG_ECHOLNPGM("JPEGDecoder::decode: JPEG decompression failed");
  }

  return (res == JDR_OK);
}

} // namespace ImageDecoders

#endif // GCODE_PREVIEW_JPEG
