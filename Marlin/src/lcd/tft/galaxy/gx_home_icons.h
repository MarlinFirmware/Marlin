/**
 * NOVIN3DP Galaxy UI - full color home screen icons (Microsoft Fluent Emoji, MIT License)
 * 8-bit indexed images: each pixel is an index into a 256 entry RGB565 + alpha palette.
 */
#pragma once

#include <stdint.h>

typedef struct {
  uint8_t width, height;
  const uint16_t *colors;   // RGB565 per palette entry
  const uint8_t  *alpha;    // 0..255 per palette entry (0 = transparent)
  const uint8_t  *index;    // width * height palette indices
} gx_pal_image_t;

extern const gx_pal_image_t gx_home_icons[3];   // prepare, settings, print

// Queue a palette image into the current canvas at (x, y)
void gx_pal_image(const int16_t x, const int16_t y, const gx_pal_image_t &img);
