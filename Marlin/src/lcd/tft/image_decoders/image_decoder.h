#pragma once

#include <stdint.h>
#include <stddef.h>

#include "../ui_common.h"  // For decoder defines

#if HAS_JPEG_DECODER
#include "jpeg/jpeg_decoder.h"
#endif

#if HAS_PNG_DECODER
#include "png/png_decoder.h"
#endif

#if HAS_QOI_DECODER
#include "qoi/qoi_decoder.h"
#endif

namespace ImageDecoders {

enum class ImageFormat {
  IMG_JPEG,
  IMG_PNG,
  IMG_QOI,
  IMG_UNKNOWN
};

// Unified interface for image decoding
class ImageDecoder {
public:
  // Detect image format from data
  static ImageFormat detectFormat(const uint8_t* data, size_t size);

  // Decode image data to RGB565 format
  // Returns true on success, false on failure
  // output_buffer must be large enough for width * height * 2 bytes
  static bool decode(const uint8_t* image_data, size_t image_size,
                     uint16_t* output_buffer, uint16_t width, uint16_t height);

  // Get dimensions without decoding
  static bool getDimensions(const uint8_t* image_data, size_t image_size,
                           uint16_t& width, uint16_t& height);
};

} // namespace ImageDecoders
