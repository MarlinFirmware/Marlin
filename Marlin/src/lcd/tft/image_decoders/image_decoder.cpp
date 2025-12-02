/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ANY(HAS_JPEG_DECODER, HAS_PNG_DECODER, HAS_QOI_DECODER)

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

#include "image_decoder.h"

namespace ImageDecoders {

  ImageFormat ImageDecoder::detectFormat(const uint8_t *data, size_t size) {
    #if HAS_JPEG_DECODER
      if (JPEGDecoder::isValidJPEG(data, size)) return ImageFormat::IMG_JPEG;
    #endif
    #if HAS_PNG_DECODER
      if (PNGDecoder::isValidPNG(data, size)) return ImageFormat::IMG_PNG;
    #endif
    #if HAS_QOI_DECODER
      if (QOIDecoder::isValidQOI(data, size)) return ImageFormat::IMG_QOI;
    #endif
    return ImageFormat::IMG_UNKNOWN;
  }

  bool ImageDecoder::decode(
    const uint8_t *image_data, size_t image_size, uint16_t *output_buffer, uint16_t width, uint16_t height
  ) {
    ImageFormat format = detectFormat(image_data, image_size);

    DEBUG_ECHOLNPGM("ImageDecoder: Detected format: ", (int)format, ", data size: ", (int)image_size);

    switch (format) {
      #if HAS_JPEG_DECODER
        case ImageFormat::IMG_JPEG:
          DEBUG_ECHOLNPGM("ImageDecoder: Calling JPEGDecoder::decode()");
          return JPEGDecoder::decode(image_data, image_size, output_buffer, width, height);
      #endif

      #if HAS_PNG_DECODER
        case ImageFormat::IMG_PNG:
          return PNGDecoder::decode(image_data, image_size, output_buffer, width, height);
      #endif

      #if HAS_QOI_DECODER
        case ImageFormat::IMG_QOI:
          return QOIDecoder::decode(image_data, image_size, output_buffer, width, height);
      #endif

      default:
        return false;
    }
  }

  bool ImageDecoder::getDimensions(
    const uint8_t *image_data, size_t image_size, uint16_t& width, uint16_t& height
  ) {
    ImageFormat format = detectFormat(image_data, image_size);

    switch (format) {
      #if HAS_JPEG_DECODER
        case ImageFormat::IMG_JPEG:
          return JPEGDecoder::getDimensions(image_data, image_size, width, height);
      #endif
      #if HAS_PNG_DECODER
        case ImageFormat::IMG_PNG:
          return PNGDecoder::getDimensions(image_data, image_size, width, height);
      #endif
      #if HAS_QOI_DECODER
        case ImageFormat::IMG_QOI:
          return QOIDecoder::getDimensions(image_data, image_size, width, height);
      #endif
      default:
        return false;
    }
  }

} // namespace ImageDecoders

#endif // HAS_JPEG_DECODER || HAS_PNG_DECODER || HAS_QOI_DECODER
