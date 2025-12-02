#pragma once

#include <stdint.h>
#include <stddef.h>

namespace ImageDecoders {

class PNGDecoder {
public:
  // Decode PNG data to RGB565 format
  // Returns true on success, false on failure
  // output_buffer must be large enough for width * height * 2 bytes
  static bool decode(
    const uint8_t *png_data, size_t png_size, uint16_t *output_buffer, uint16_t width, uint16_t height
  );

  // Get the dimensions of a PNG without decoding
  // Returns true if dimensions were successfully extracted
  static bool getDimensions(
    const uint8_t *png_data, size_t png_size, uint16_t& width, uint16_t& height
  );

  // Helper functions for PNG parsing
  static bool isValidPNG(const uint8_t *data, size_t size);
  static bool findIHDR(const uint8_t *data, size_t size, uint16_t& width, uint16_t& height);

private:
};

} // namespace ImageDecoders
