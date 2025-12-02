/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * QOI Decoder using integrated qoi.h library
 * qoi - The Quite OK Image format for fast, lossless image compression
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../../inc/MarlinConfig.h"

#if ENABLED(GCODE_PREVIEW_QOI)

//#define DEBUG_OUT 1
#include "../../../../core/debug_out.h"

// Configure qoi.h for embedded use
#define QOI_IMPLEMENTATION
#define QOI_NO_STDIO
#define QOI_MALLOC(sz) malloc(sz)
#define QOI_FREE(p) free(p)

#include "qoi.h"
#include "qoi_decoder.h"
#include <stdlib.h>

namespace ImageDecoders {

bool QOIDecoder::decode(
  const uint8_t *qoi_data, size_t qoi_size, uint16_t *output_buffer, uint16_t width, uint16_t height
) {
  DEBUG_ECHOLNPGM("QOIDecoder::decode: Starting QOI decode, size=", qoi_size, ", expected dims=", width, C('x'), height);

  if (!isValidQOI(qoi_data, qoi_size)) {
    DEBUG_ECHOLNPGM("QOIDecoder::decode: Invalid QOI data");
    return false;
  }

  // Decode QOI image using qoi.h
  qoi_desc desc;
  // Request RGB or RGBA output (4 channels to get full color data)
  unsigned char *pixels = (unsigned char*)qoi_decode(qoi_data, qoi_size, &desc, 0);

  if (!pixels) {
    DEBUG_ECHOLNPGM("QOIDecoder::decode: qoi_decode failed");
    return false;
  }

  DEBUG_ECHOLNPGM("QOIDecoder::decode: QOI decoded successfully, actual dims=", desc.width, C('x'), desc.height, ", channels=", desc.channels);

  // Verify dimensions match
  if (desc.width != width || desc.height != height) {
    DEBUG_ECHOLNPGM("QOIDecoder::decode: Dimension mismatch: got ", desc.width, C('x'), desc.height, ", expected ", width, C('x'), height);
    QOI_FREE(pixels);
    return false;
  }

  DEBUG_ECHOLNPGM("QOIDecoder::decode: Converting to RGB565, channels=", desc.channels);

  // Convert decoded pixels to RGB565
  const uint32_t total_pixels = width * height;
  const uint8_t channels = desc.channels;

  for (uint32_t i = 0; i < total_pixels; i++) {
    uint8_t r = pixels[i * channels + 0];
    uint8_t g = pixels[i * channels + 1];
    uint8_t b = pixels[i * channels + 2];

    // Convert RGB888 to RGB565
    output_buffer[i] = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
  }

  QOI_FREE(pixels);
  DEBUG_ECHOLNPGM("QOIDecoder::decode: QOI decode completed successfully");
  return true;
}

bool QOIDecoder::getDimensions(
  const uint8_t *qoi_data, size_t qoi_size, uint16_t& width, uint16_t& height
) {
  DEBUG_ECHOLNPGM("QOIDecoder::getDimensions: Getting QOI dimensions, size=", qoi_size);

  if (!isValidQOI(qoi_data, qoi_size) || qoi_size < 14) {
    DEBUG_ECHOLNPGM("QOIDecoder::getDimensions: Invalid QOI data or size too small");
    return false;
  }

  // We can read the header manually since it's just 14 bytes
  // Width and height are big-endian 4-byte integers at offset 4 and 8
  width = ((uint32_t)qoi_data[4] << 24) | ((uint32_t)qoi_data[5] << 16) |
          ((uint32_t)qoi_data[6] << 8) | qoi_data[7];
  height = ((uint32_t)qoi_data[8] << 24) | ((uint32_t)qoi_data[9] << 16) |
           ((uint32_t)qoi_data[10] << 8) | qoi_data[11];

  DEBUG_ECHOLNPGM("QOIDecoder::getDimensions: QOI dimensions: ", width, C('x'), height);
  return true;
}

bool QOIDecoder::isValidQOI(const uint8_t *data, size_t size) {
  if (size < 14) return false;
  // Check QOI magic "qoif"
  return data[0] == 'q' && data[1] == 'o' && data[2] == 'i' && data[3] == 'f';
}

} // namespace ImageDecoders

#endif // GCODE_PREVIEW_QOI
