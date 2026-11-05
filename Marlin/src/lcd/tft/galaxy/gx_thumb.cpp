/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * NOVIN3DP Galaxy UI - G-code QOI thumbnails
 *
 * The slicer writes (OrcaSlicer "48x48/QOI, 150x150/QOI"):
 *   ; thumbnail_QOI begin 150x150 7464
 *   ; cW9pZgAAAJYAAACW...      <- base64 QOI, one "; " prefixed line at a time
 *   ; thumbnail_QOI end
 *
 * The image is decoded while the canvas strip is filled: no image buffer,
 * 256 bytes of QOI state and a 64 byte read buffer.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(NOVIN3DP_GALAXY_UI, HAS_MEDIA)

#include "gx_thumb.h"
#include "gx_core.h"
#include "../tft.h"
#include "../../../sd/cardreader.h"

#define THUMB_SCAN_LIMIT  65536UL   // Only look at the G-code header

typedef struct __attribute__((__packed__)) {
  uint8_t size;
  uint8_t job;
  char name[13];
} thumb_payload_t;

// The file being printed
static MediaFile job_dir;
static bool job_valid = false;
static char job_dos[13], job_long[48];

void gx_thumb_set_job(const char *dosname, const char *longname) {
  job_dir = card.getWorkDir();
  strlcpy(job_dos, dosname, sizeof(job_dos));
  strlcpy(job_long, longname && *longname ? longname : dosname, sizeof(job_long));
  job_valid = true;
}
bool gx_thumb_has_job() { return job_valid; }
bool gx_job_file_ok() {
  if (!job_valid || !card.isMounted()) return false;
  MediaFile dir = job_dir, f;
  const bool ok = f.open(&dir, job_dos, O_READ);
  if (ok) f.close();
  return ok;
}
const char* gx_job_name() { return job_valid ? job_long : card.longest_filename(); }
const char* gx_job_dosname() { return job_dos; }

//
// Decoder state, kept between canvas strips
//
static struct {
  MediaFile file;
  bool active, ok;
  thumb_payload_t key;
  uint16_t row;
  // read buffer
  uint8_t buf[64];
  uint8_t len, pos;
  uint32_t total;
  // base64
  uint32_t bits;
  uint8_t nbits;
  bool line_start;
  // QOI
  uint8_t idx[64][4], px[4], run;
} T;

static int16_t raw_byte() {
  if (T.pos >= T.len) {
    if (T.total >= THUMB_SCAN_LIMIT * 4) return -1;
    const int16_t n = T.file.read(T.buf, sizeof(T.buf));
    if (n <= 0) return -1;
    T.len = n; T.pos = 0; T.total += n;
  }
  return T.buf[T.pos++];
}

// Read one text line (without CR/LF) into dst. Returns false at end of file.
static bool read_line(char *dst, const uint8_t maxlen) {
  uint8_t n = 0;
  for (;;) {
    const int16_t c = raw_byte();
    if (c < 0) { dst[n] = '\0'; return n > 0; }
    if (c == '\n') break;
    if (c == '\r') continue;
    if (n < maxlen - 1) dst[n++] = char(c);
  }
  dst[n] = '\0';
  return true;
}

static int8_t b64_value(const int16_t c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a' + 26;
  if (c >= '0' && c <= '9') return c - '0' + 52;
  if (c == '+') return 62;
  if (c == '/') return 63;
  return -1;
}

// Next decoded byte of the base64 stream, skipping the "; " line prefixes
static int16_t data_byte() {
  while (T.nbits < 8) {
    const int16_t c = raw_byte();
    if (c < 0) return -1;
    if (c == '\n') { T.line_start = true; continue; }
    if (T.line_start && (c == ';' || c == ' ')) continue;
    T.line_start = false;
    const int8_t v = b64_value(c);
    if (v < 0) continue;                        // '=', '\r', anything else
    T.bits = (T.bits << 6) | uint8_t(v);
    T.nbits += 6;
  }
  T.nbits -= 8;
  return (T.bits >> T.nbits) & 0xFF;
}

static bool find_thumbnail(const uint8_t size) {
  char line[48];
  while (read_line(line, sizeof(line))) {
    if (line[0] != ';') {
      if (line[0] && line[0] != ' ') return false;   // G-code started, no thumbnail
      continue;
    }
    const char *p = strstr(line, "thumbnail_QOI begin ");
    if (!p) continue;
    p += 20;
    const int w = atoi(p);
    const char *x = strchr(p, 'x');
    const int h = x ? atoi(x + 1) : 0;
    if (w == size && h == size) return true;
  }
  return false;
}

static bool thumb_start(const thumb_payload_t *key) {
  if (T.active) { T.file.close(); T.active = false; }
  T.key = *key;
  T.row = 0;
  T.ok = false;
  T.len = T.pos = 0;
  T.total = 0;
  T.bits = 0; T.nbits = 0;
  T.line_start = false;

  if (!card.isMounted()) return false;
  MediaFile dir = key->job ? job_dir : card.getWorkDir();
  if (key->job && !job_valid) return false;
  if (!T.file.open(&dir, key->name, O_READ)) return false;
  T.active = true;

  if (!find_thumbnail(key->size)) return false;

  // 14 byte QOI header: "qoif", width, height, channels, colorspace
  uint8_t hdr[14];
  for (uint8_t i = 0; i < 14; i++) {
    const int16_t b = data_byte();
    if (b < 0) return false;
    hdr[i] = b;
  }
  if (hdr[0] != 'q' || hdr[1] != 'o' || hdr[2] != 'i' || hdr[3] != 'f') return false;
  const uint32_t w = (uint32_t(hdr[4]) << 24) | (uint32_t(hdr[5]) << 16) | (uint32_t(hdr[6]) << 8) | hdr[7],
                 h = (uint32_t(hdr[8]) << 24) | (uint32_t(hdr[9]) << 16) | (uint32_t(hdr[10]) << 8) | hdr[11];
  if (w != key->size || h != key->size) return false;    // Square thumbnail of the requested size only
  if (hdr[12] != 3 && hdr[12] != 4) return false;        // Channels: RGB or RGBA
  if (hdr[13] > 1) return false;                         // Colorspace: sRGB or linear

  memset(T.idx, 0, sizeof(T.idx));
  T.px[0] = T.px[1] = T.px[2] = 0; T.px[3] = 255;
  T.run = 0;
  T.ok = true;
  return true;
}

static void thumb_stop() {
  if (T.active) { T.file.close(); T.active = false; }
}

// Decode one pixel. Returns false on a read error.
static bool qoi_pixel() {
  if (T.run) { T.run--; return true; }
  const int16_t b1 = data_byte();
  if (b1 < 0) return false;
  if (b1 == 0xFE) {
    for (uint8_t i = 0; i < 3; i++) { const int16_t v = data_byte(); if (v < 0) return false; T.px[i] = v; }
  }
  else if (b1 == 0xFF) {
    for (uint8_t i = 0; i < 4; i++) { const int16_t v = data_byte(); if (v < 0) return false; T.px[i] = v; }
  }
  else switch (b1 >> 6) {
    case 0: memcpy(T.px, T.idx[b1], 4); break;                                                         // INDEX
    case 1: T.px[0] += ((b1 >> 4) & 3) - 2; T.px[1] += ((b1 >> 2) & 3) - 2; T.px[2] += (b1 & 3) - 2; break;  // DIFF
    case 2: {                                                                                           // LUMA
      const int16_t b2 = data_byte();
      if (b2 < 0) return false;
      const int8_t vg = (b1 & 0x3F) - 32;
      T.px[0] += vg - 8 + ((b2 >> 4) & 0x0F);
      T.px[1] += vg;
      T.px[2] += vg - 8 + (b2 & 0x0F);
    } break;
    case 3: T.run = b1 & 0x3F; break;                                                                    // RUN
  }
  memcpy(T.idx[(T.px[0] * 3 + T.px[1] * 5 + T.px[2] * 7 + T.px[3] * 11) & 63], T.px, 4);
  return true;
}

// Fallback: the file icon, centered
static void draw_fallback(uint16_t *buffer, const uint16_t cw, const uint16_t startLine, const uint16_t endLine, const int16_t x, const int16_t y, const uint8_t size) {
  const uint8_t isz = size >= 80 ? 80 : (size >= 48 ? 48 : 32);
  const uint8_t *data = gx_icon_data(GXI_FILE, isz);
  if (data) gx_icon_render(buffer, cw, startLine, endLine, x + (size - isz) / 2, y + (size - isz) / 2, data, isz, GX_C_DIM);
}

static void thumb_render(uint16_t *buffer, uint16_t cw, uint16_t startLine, uint16_t endLine, int16_t x, int16_t y, const uint8_t *payload) {
  const thumb_payload_t *key = (const thumb_payload_t *)payload;
  const int16_t size = key->size;
  if (int16_t(endLine) <= y || int16_t(startLine) >= y + size) return;   // Not in this strip

  // Restart when this is a new image, the image starts in this strip, or the strip doesn't
  // continue where we stopped. A failed image is not retried on its later strips:
  // a file without a thumbnail would otherwise be re-opened and re-scanned for every strip.
  const bool same = memcmp(&T.key, key, sizeof(thumb_payload_t)) == 0;
  const int16_t first = _MAX(int16_t(startLine) - y, int16_t(0));
  if (!same || first == 0 || (T.ok && int16_t(T.row) != first)) {
    if (!thumb_start(key)) { thumb_stop(); T.ok = false; }
    for (; T.ok && int16_t(T.row) < first; T.row++)   // skip rows above this strip
      for (int16_t c = 0; c < size; c++) if (!qoi_pixel()) { T.ok = false; thumb_stop(); break; }
  }

  if (!T.ok) { draw_fallback(buffer, cw, startLine, endLine, x, y, size); return; }

  for (; int16_t(T.row) < size && y + int16_t(T.row) < int16_t(endLine); T.row++) {
    uint16_t *row = buffer + (y + T.row - startLine) * cw;
    for (int16_t c = 0; c < size; c++) {
      if (!qoi_pixel()) { T.ok = false; thumb_stop(); return; }
      if (T.px[3] >= 128 && WITHIN(x + c, 0, cw - 1))
        row[x + c] = ENDIAN_COLOR(uint16_t(((T.px[0] & 0xF8) << 8) | ((T.px[1] & 0xFC) << 3) | (T.px[2] >> 3)));
    }
  }
  if (int16_t(T.row) >= size) thumb_stop();
}

void gx_thumb(const int16_t x, const int16_t y, const uint8_t size, const char *dosname, const bool job) {
  thumb_payload_t p;
  p.size = size;
  p.job = job;
  strlcpy(p.name, dosname ? dosname : "", sizeof(p.name));
  tft.add_custom(x, y, (void *)thumb_render, &p, sizeof(p));
}

#endif // NOVIN3DP_GALAXY_UI && HAS_MEDIA
