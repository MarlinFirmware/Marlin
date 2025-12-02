/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * PNG Decoder using integrated stb_image library
 * stb_image - v2.28 - public domain image loader
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../../inc/MarlinConfig.h"

#if HAS_PNG_DECODER

//#define DEBUG_OUT 1
#include "../../../../core/debug_out.h"

#include "png_decoder.h"
#include <stdlib.h>
#include <string.h>

// Use stb_image for PNG decoding - single-header, minimal, well-tested
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#define STBI_NO_FAILURE_STRINGS  // Save some flash space
#define STBI_ASSERT(x)  // Disable asserts for embedded system

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "stb_image.h"
#pragma GCC diagnostic pop

namespace ImageDecoders {

bool PNGDecoder::decode(
  const uint8_t *png_data, size_t png_size, uint16_t *output_buffer, uint16_t width, uint16_t height
) {
  DEBUG_ECHOLNPGM("PNGDecoder::decode: Starting PNG decode with stb_image, size=", png_size, ", expected dims=", width, "x", height);

  int img_width, img_height, channels;

  // Decode PNG - request RGB or RGBA
  unsigned char *img_data = stbi_load_from_memory(png_data, png_size, &img_width, &img_height, &channels, 0);

  if (!img_data) {
    DEBUG_ECHOLNPGM("PNGDecoder::decode: stbi_load_from_memory failed");
    return false;
  }

  DEBUG_ECHOLNPGM("PNGDecoder::decode: PNG decoded successfully, actual dims=", img_width, "x", img_height, ", channels=", channels);

  // Check dimensions match expected
  if (img_width != width || img_height != height) {
    DEBUG_ECHOLNPGM("PNGDecoder::decode: Dimension mismatch: got ", img_width, "x", img_height, ", expected ", width, "x", height);
    stbi_image_free(img_data);
    return false;
  }

  DEBUG_ECHOLNPGM("PNGDecoder::decode: Converting to RGB565, channels=", channels);

  // Convert to RGB565
  const unsigned char* src = img_data;
  uint16_t* dst = output_buffer;

  for (uint16_t y = 0; y < height; y++) {
    for (uint16_t x = 0; x < width; x++) {
      uint8_t r = *src++;
      uint8_t g = *src++;
      uint8_t b = *src++;

      // Skip alpha if present
      if (channels == 4) src++;
      else if (channels == 1) {
        // Grayscale - duplicate to RGB
        g = r;
        b = r;
      }

      // Convert RGB888 to RGB565
      uint16_t rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
      *dst++ = rgb565;
    }
  }

  DEBUG_ECHOLNPGM("PNGDecoder::decode: RGB conversion completed successfully");
  stbi_image_free(img_data);
  DEBUG_ECHOLNPGM("PNGDecoder::decode: PNG decode completed successfully");
  return true;
}

bool PNGDecoder::getDimensions(
  const uint8_t *png_data, size_t png_size, uint16_t& width, uint16_t& height
) {
  int img_width, img_height, channels;

  // Just get image info without decoding the whole image
  int result = stbi_info_from_memory(png_data, png_size, &img_width, &img_height, &channels);

  if (!result) {
    DEBUG_ECHOLNPGM("PNGDecoder::getDimensions: stbi_info_from_memory failed");
    return false;
  }

  width = (uint16_t)img_width;
  height = (uint16_t)img_height;

  DEBUG_ECHOLNPGM("PNGDecoder::getDimensions: PNG dimensions: ", width, "x", height, ", channels=", channels);
  return true;
}

bool PNGDecoder::isValidPNG(const uint8_t *data, size_t size) {
  if (size < 8) return false;
  return data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47 &&
         data[4] == 0x0D && data[5] == 0x0A && data[6] == 0x1A && data[7] == 0x0A;
}

bool PNGDecoder::findIHDR(const uint8_t *data, size_t size, uint16_t& width, uint16_t& height) {
  return getDimensions(data, size, width, height);
}

} // namespace ImageDecoders

#endif // HAS_PNG_DECODER
