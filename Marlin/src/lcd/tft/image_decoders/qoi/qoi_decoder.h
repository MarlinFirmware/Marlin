#pragma once

#include <stdint.h>
#include <stddef.h>

namespace ImageDecoders {

class QOIDecoder {
public:
  // Decode QOI data to RGB565 format
  // Returns true on success, false on failure
  // output_buffer must be large enough for width * height * 2 bytes
  static bool decode(
    const uint8_t *qoi_data, size_t qoi_size, uint16_t *output_buffer, uint16_t width, uint16_t height
  );

  // Get the dimensions of a QOI without decoding
  // Returns true if dimensions were successfully extracted
  static bool getDimensions(
    const uint8_t *qoi_data, size_t qoi_size, uint16_t& width, uint16_t& height
  );

  // Helper functions for QOI parsing
  static bool isValidQOI(const uint8_t *data, size_t size);
};

} // namespace ImageDecoders
