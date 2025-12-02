#pragma once

#include <stdint.h>
#include <stddef.h>

namespace ImageDecoders {

class JPEGDecoder {
public:
  // Decode JPEG data to RGB565 format
  // Returns true on success, false on failure
  // output_buffer must be large enough for width * height * 2 bytes
  static bool decode(
    const uint8_t *jpeg_data, size_t jpeg_size, uint16_t *output_buffer, uint16_t width, uint16_t height
  );

  // Get the dimensions of a JPEG without decoding
  // Returns true if dimensions were successfully extracted
  static bool getDimensions(
    const uint8_t *jpeg_data, size_t jpeg_size, uint16_t& width, uint16_t& height
  );

  // Helper functions for JPEG parsing
  static bool isValidJPEG(const uint8_t *data, size_t size);
  static bool findSOF0(const uint8_t *data, size_t size, uint16_t& width, uint16_t& height);

};

} // namespace ImageDecoders
