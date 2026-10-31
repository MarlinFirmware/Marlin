/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if HAS_GCODE_THUMBNAIL

#include "gcode_thumbnail.h"
#include "cardreader.h"

#define THUMB_SCAN_LIMIT  (128UL * 1024UL)  // Bytes of the file to search for the block
#define THUMB_LINE_MAX    48                // Longest header line to keep

uint32_t GCodeThumbnail::decoded_size, GCodeThumbnail::remaining;
uint8_t GCodeThumbnail::pending[3], GCodeThumbnail::pending_pos, GCodeThumbnail::pending_end;

// Read one line, keeping at most 'maxlen' - 1 characters. Returns false at end of file.
static bool read_line(char * const line, const uint8_t maxlen) {
  uint8_t n = 0;
  for (;;) {
    const int16_t c = card.get();
    if (c < 0) { line[n] = '\0'; return n > 0; }
    if (c == '\n') break;
    if (c != '\r' && n < maxlen - 1) line[n++] = char(c);
  }
  line[n] = '\0';
  return true;
}

static const char *skip_space(const char *p) { while (*p == ' ') ++p; return p; }

// Parse an unsigned decimal. Advances 'p'. Returns false if there are no digits.
static bool parse_uint(const char *&p, uint32_t &value) {
  if (*p < '0' || *p > '9') return false;
  value = 0;
  while (*p >= '0' && *p <= '9') value = value * 10 + (*p++ - '0');
  return true;
}

static int8_t base64_value(const char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a' + 26;
  if (c >= '0' && c <= '9') return c - '0' + 52;
  if (c == '+' || c == '-') return 62;  // Accept the URL-safe alphabet too
  if (c == '/' || c == '_') return 63;
  return -1;
}

bool GCodeThumbnail::find(PGM_P const tag, const uint16_t w, const uint16_t h, const bool size_is_encoded/*=false*/) {
  decoded_size = remaining = pending_pos = pending_end = 0;

  card.setIndex(0);

  const size_t taglen = strlen_P(tag);
  char line[THUMB_LINE_MAX];

  while (card.getIndex() < THUMB_SCAN_LIMIT && read_line(line, sizeof(line))) {
    const char *p = line;
    if (*p++ != ';') continue;

    p = skip_space(p);
    if (strncmp_P(p, tag, taglen)) continue;
    p = skip_space(p + taglen);
    if (strncmp_P(p, PSTR("begin"), 5)) continue;
    p = skip_space(p + 5);

    // <W><x or *><H>
    uint32_t bw, bh, size = 0;
    if (!parse_uint(p, bw) || (*p != 'x' && *p != '*')) continue;
    ++p;
    if (!parse_uint(p, bh)) continue;
    if (bw != w || bh != h) continue;

    // Size is optional in some formats, but needed here to know where the image ends
    p = skip_space(p);
    if (!parse_uint(p, size) || !size) return false;

    if (size_is_encoded) {
      remaining = size;
      decoded_size = (size / 4) * 3;
    }
    else {
      decoded_size = size;
      remaining = ((size + 2) / 3) * 4;
    }
    return true;
  }

  return false;
}

uint16_t GCodeThumbnail::read(uint8_t * const buf, const uint16_t len) {
  uint16_t out = 0;
  while (out < len) {

    // Hand out bytes decoded earlier but not yet taken
    if (pending_pos < pending_end) {
      buf[out++] = pending[pending_pos++];
      continue;
    }

    if (!remaining) break;

    // Gather four base64 characters, skipping line prefixes and breaks
    uint8_t v[4], got = 0, pad = 0;
    while (got < 4 && remaining) {
      const int16_t c = card.get();
      if (c < 0) { remaining = 0; break; }
      if (c == ';' || c == ' ' || c == '\r' || c == '\n') continue;
      --remaining;
      if (c == '=') { v[got++] = 0; ++pad; continue; }
      const int8_t b = base64_value(char(c));
      if (b < 0) { remaining = 0; break; }  // Not image data
      v[got++] = uint8_t(b);
    }
    if (got < 4) break;

    pending[0] = (v[0] << 2) | (v[1] >> 4);
    pending[1] = (v[1] << 4) | (v[2] >> 2);
    pending[2] = (v[2] << 6) | v[3];
    pending_pos = 0;
    pending_end = 3 - pad;
    if (pad) remaining = 0;               // Padding ends the data
  }
  return out;
}

#endif // HAS_GCODE_THUMBNAIL
