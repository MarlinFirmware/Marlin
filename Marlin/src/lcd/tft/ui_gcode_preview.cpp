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

/**
 * TFT_COLOR_UI G-code thumbnail preview
 *
 * Thumbnails are base64-encoded images in the G-code header, written by the slicer as:
 *   ; thumbnail_JPG begin 200x200 12345
 *   ; /9j/4AAQSkZJRgABAQ...
 *   ; thumbnail_JPG end
 * The image is decoded while reading it from the file and drawn straight to the display,
 * so only a few kilobytes of RAM are needed.
 */

#include "../../inc/MarlinConfig.h"

#if HAS_TFT_GCODE_PREVIEW

#include "ui_common.h"
#include "ui_gcode_preview.h"
#include "../menu/menu.h"
#include "../../sd/cardreader.h"
#include "../../libs/numtostr.h"

#if ENABLED(GCODE_PREVIEW_JPEG)
  #include "image_decoders/jpeg/jpeg_decoder.h"
#endif
#if ENABLED(GCODE_PREVIEW_QOI)
  #include "image_decoders/qoi/qoi_decoder.h"
#endif
#if ENABLED(GCODE_PREVIEW_MKS)
  #include "image_decoders/mks/mks_decoder.h"
#endif

Preview preview;

// Thumbnail on the left with details on the right, or details above the thumbnail
#define PREVIEW_H         BUTTON_CONFIRM_Y                      // Above the buttons
#if TFT_WIDTH > TFT_HEIGHT
  #define THUMB_X         0
  #define THUMB_Y         0
  #define THUMB_W         (TFT_WIDTH / 2)
  #define THUMB_H         PREVIEW_H
  #define INFO_X          THUMB_W
  #define INFO_Y          0
  #define INFO_W          (TFT_WIDTH - THUMB_W)
  #define INFO_H          PREVIEW_H
#else
  #define INFO_X          0
  #define INFO_Y          0
  #define INFO_W          TFT_WIDTH
  #define INFO_H          (4 * MENU_LINE_HEIGHT)                // Details above the thumbnail
  #define THUMB_X         0
  #define THUMB_Y         INFO_H
  #define THUMB_W         TFT_WIDTH
  #define THUMB_H         (PREVIEW_H - INFO_H)
#endif

#define INFO_PAD          5                                     // Detail text margin

// Decoded rows go to the second half of the TFT buffer and are drawn from the first half
#define ROW_BUFFER        (tft.buffer + (TFT_BUFFER_WORDS) / 2)
static_assert(THUMB_W <= (TFT_BUFFER_WORDS) / 2, "The G-code preview needs TFT_BUFFER_WORDS of at least twice the thumbnail width.");

enum ThumbFormat : uint8_t { THUMB_NONE, THUMB_JPEG, THUMB_QOI, THUMB_MKS };

static struct {
  char name[FILENAME_LENGTH];   // 8.3 name of the file to print
  ThumbFormat format;           // Format of the chosen thumbnail
  uint32_t data_pos, data_len;  // File position and length of its base64 data
  uint16_t w, h;                // Size it will be drawn
  float time, filament, layer, size_x, size_y, size_z;
  bool drawn;                   // Thumbnail and details are on screen
} fileprop;

//
// Reading the file
//

// Read a line, truncated to fit the buffer. Return false at the end of the file.
static bool read_line(char * const buf, const uint8_t size) {
  int16_t c = card.get();
  if (c < 0) return false;
  uint8_t i = 0;
  for (; c >= 0 && c != '\n'; c = card.get())
    if (c != '\r' && i < size - 1) buf[i++] = c;
  buf[i] = '\0';
  return true;
}

// Get the value of a metadata line like ";TIME:3723"
static void get_value(const char * const line, PGM_P const key, float &value) {
  const size_t len = strlen_P(key);
  if (strncmp_P(line, key, len) == 0) value = atof(line + len);
}

// Read the metadata and choose the largest thumbnail that can be drawn
void Preview::load(const char * const filename) {
  fileprop = {};
  strlcpy(fileprop.name, filename, sizeof(fileprop.name));

  card.openFileRead(fileprop.name);
  if (!card.isFileOpen()) return;

  float min_x = 0, min_y = 0, min_z = 0;
  uint32_t best_area = 0;
  char line[96];

  // Keep the largest thumbnail that can be drawn
  auto consider = [&](const ThumbFormat format, const uint32_t data_pos, const uint32_t len, const uint16_t w, const uint16_t h) {
    if (w <= THUMB_W && h <= THUMB_H && uint32_t(w) * h > best_area) {
      best_area = uint32_t(w) * h;
      fileprop.format = format;
      fileprop.data_pos = data_pos;
      fileprop.data_len = len;
      fileprop.w = w;
      fileprop.h = h;
    }
  };

  #if ENABLED(GCODE_PREVIEW_MKS)
    bool seen_simage = false, seen_gimage = false;
  #endif

  // The header ends at the first line that isn't a comment
  for (uint32_t line_pos = 0; read_line(line, sizeof(line)); line_pos = card.getIndex()) {
    if (!line[0]) continue;
    if (line[0] != ';') break;

    #if ENABLED(GCODE_PREVIEW_MKS)
      // MKS images: the first ";simage:" (100x100) or ";;gimage:" (200x200) line starts the image
      if (strncmp_P(line, PSTR(";simage:"), 8) == 0) {
        if (!seen_simage) { seen_simage = true; consider(THUMB_MKS, line_pos, 0, 100, 100); }
        continue;
      }
      if (strncmp_P(line, PSTR(";;gimage:"), 9) == 0) {
        if (!seen_gimage) { seen_gimage = true; consider(THUMB_MKS, line_pos, 0, 200, 200); }
        continue;
      }
    #endif

    // Cura-style metadata
    get_value(line, PSTR(";TIME:"), fileprop.time);
    get_value(line, PSTR(";Filament used:"), fileprop.filament);
    get_value(line, PSTR(";Layer height:"), fileprop.layer);
    get_value(line, PSTR(";MINX:"), min_x);
    get_value(line, PSTR(";MINY:"), min_y);
    get_value(line, PSTR(";MINZ:"), min_z);
    get_value(line, PSTR(";MAXX:"), fileprop.size_x);
    get_value(line, PSTR(";MAXY:"), fileprop.size_y);
    get_value(line, PSTR(";MAXZ:"), fileprop.size_z);

    // "; thumbnail... begin WxH length"
    const char *p = strstr_P(line, PSTR(" begin "));
    if (!p || !strstr_P(line, PSTR("thumbnail"))) continue;
    char *end;
    uint16_t w = strtoul(p + 7, &end, 10);
    if (*end != 'x') continue;
    uint16_t h = strtoul(end + 1, &end, 10);
    const uint32_t len = strtoul(end, nullptr, 10);
    if (!w || !h || !len) continue;

    // Identify the format from the start of the base64 data
    const uint32_t data_pos = card.getIndex();
    if (!read_line(line, sizeof(line))) break;
    const char *data = line;
    while (*data == ';' || *data == ' ') ++data;
    ThumbFormat format = THUMB_NONE;
    #if ENABLED(GCODE_PREVIEW_JPEG)
      if (strncmp_P(data, PSTR("/9j/"), 4) == 0) {
        format = THUMB_JPEG;
        for (uint8_t s = 0; s < 3 && (w > THUMB_W || h > THUMB_H); ++s) { w >>= 1; h >>= 1; } // Up to 1/8 scale
      }
    #endif
    #if ENABLED(GCODE_PREVIEW_QOI)
      if (strncmp_P(data, PSTR("cW9p"), 4) == 0) format = THUMB_QOI;
    #endif

    if (format) consider(format, data_pos, len, w, h);
  }
  card.closefile();

  if (fileprop.size_x) fileprop.size_x -= min_x;
  if (fileprop.size_y) fileprop.size_y -= min_y;
  if (fileprop.size_z) fileprop.size_z -= min_z;
}

#if ANY(GCODE_PREVIEW_JPEG, GCODE_PREVIEW_QOI)
  #define HAS_BASE64_THUMB 1
#endif

#if HAS_BASE64_THUMB

// Base64 decoding of the thumbnail data as it is read
static uint32_t b64_left;   // Base64 characters still to read
static uint16_t b64_bits;
static uint8_t b64_nbits;

static int16_t b64_read() {
  while (b64_nbits < 8) {
    if (!b64_left) return -1;
    const int16_t c = card.get();
    if (c < 0) return -1;
    int8_t v;
    if (WITHIN(c, 'A', 'Z'))      v = c - 'A';
    else if (WITHIN(c, 'a', 'z')) v = c - 'a' + 26;
    else if (WITHIN(c, '0', '9')) v = c - '0' + 52;
    else if (c == '+')            v = 62;
    else if (c == '/')            v = 63;
    else {
      if (c == '=') b64_left = 0;   // Padding ends the data
      continue;                     // Skip line ends and comment marks
    }
    --b64_left;
    b64_bits = b64_bits << 6 | v;
    b64_nbits += 6;
  }
  b64_nbits -= 8;
  return (b64_bits >> b64_nbits) & 0xFF;
}

#endif // HAS_BASE64_THUMB

//
// Drawing
//

// Draw decoded pixels straight to the display through the TFT buffer
static void draw_pixels(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, uint16_t * const pixels) {
  const uint16_t count = w * h;
  while (tft.is_busy()) { /* Wait for the previous block to be sent */ }
  for (uint16_t i = 0; i < count; ++i) tft.buffer[i] = ENDIAN_COLOR(pixels[i]);
  tft.set_window(THUMB_X + x, THUMB_Y + y, THUMB_X + x + w - 1, THUMB_Y + y + h - 1);
  tft.write_sequence(tft.buffer, count);
}

static bool draw_thumbnail() {
  if (!fileprop.format) return false;

  card.openFileRead(fileprop.name);
  if (!card.isFileOpen()) return false;
  card.setIndex(fileprop.data_pos);
  #if HAS_BASE64_THUMB
    b64_left = fileprop.data_len;
    b64_nbits = 0;
  #endif

  tft.queue.sync();   // The TFT buffer is free once the queue is empty

  bool ok = false;
  switch (fileprop.format) {
    #if ENABLED(GCODE_PREVIEW_JPEG)
      case THUMB_JPEG: ok = ImageDecoders::JPEGDecoder::draw(b64_read, draw_pixels, THUMB_W, THUMB_H); break;
    #endif
    #if ENABLED(GCODE_PREVIEW_QOI)
      case THUMB_QOI:  ok = ImageDecoders::QOIDecoder::draw(b64_read, draw_pixels, THUMB_W, THUMB_H, ROW_BUFFER); break;
    #endif
    #if ENABLED(GCODE_PREVIEW_MKS)
      case THUMB_MKS:  ok = ImageDecoders::MKSDecoder::draw(card.get, draw_pixels, fileprop.w, THUMB_W, THUMB_H, COLOR_BACKGROUND, ROW_BUFFER); break;
    #endif
    default: break;
  }
  while (tft.is_busy()) { /* Wait for the last block to be sent */ }
  card.closefile();
  return ok;
}

// Add a "label: value" line
static void add_detail(uint16_t &y, FSTR_P const label, const char * const value) {
  tft_string.set(label);
  tft_string.add(F(": "));
  tft.add_text(INFO_PAD, y, COLOR_MENU_TEXT, tft_string);
  const uint16_t vx = INFO_PAD + tft_string.width();
  tft_string.set(value ?: "-");
  tft.add_text(vx, y, COLOR_MENU_VALUE, tft_string, INFO_W - INFO_PAD - vx);
  y += MENU_LINE_HEIGHT;
}

// Skip the padding in a fixed-width number
static const char* trim(const char *s) { while (*s == ' ') ++s; return s; }

static void draw_details() {
  tft.canvas(INFO_X, INFO_Y, INFO_W, INFO_H);
  tft.set_background(COLOR_BACKGROUND);

  uint16_t y = MENU_TEXT_Y;
  char value[24];

  // Print time as h:mm
  const uint32_t mins = uint32_t(fileprop.time) / 60;
  sprintf_P(value, PSTR("%lu:%02u"), (unsigned long)(mins / 60), unsigned(mins % 60));
  add_detail(y, GET_TEXT_F(MSG_PREVIEW_TIME), fileprop.time ? value : nullptr);

  if (fileprop.filament) { strcpy(value, trim(ftostr42_52(fileprop.filament))); strcat_P(value, PSTR("m")); }
  add_detail(y, GET_TEXT_F(MSG_FILAMENT), fileprop.filament ? value : nullptr);

  if (fileprop.layer) { strcpy(value, trim(ftostr12ns(fileprop.layer))); strcat_P(value, PSTR("mm")); }
  add_detail(y, GET_TEXT_F(MSG_PREVIEW_LAYER), fileprop.layer ? value : nullptr);

  const bool has_size = fileprop.size_x && fileprop.size_y && fileprop.size_z;
  if (has_size) {
    // Whole millimeters
    sprintf_P(value, PSTR("%ux%ux%umm"), unsigned(LROUND(fileprop.size_x)), unsigned(LROUND(fileprop.size_y)), unsigned(LROUND(fileprop.size_z)));
  }
  add_detail(y, GET_TEXT_F(MSG_PREVIEW_SIZE), has_size ? value : nullptr);
}

static void draw_screen(const bool yes) {
  // Only the buttons change after the first draw
  if (!fileprop.drawn) {
    fileprop.drawn = true;
    if (!draw_thumbnail()) {
      tft.canvas(THUMB_X, THUMB_Y, THUMB_W, THUMB_H);
      tft.set_background(COLOR_BACKGROUND);
      tft_string.set(GET_TEXT_F(MSG_PREVIEW_NO_THUMBNAIL));
      tft.add_text(tft_string.center(THUMB_W), (THUMB_H - tft_string.font_height()) / 2, COLOR_MENU_TEXT, tft_string);
    }
    draw_details();
  }

  TERN_(TOUCH_SCREEN, touch.clear());
  // Without a touch screen the buttons show the encoder selection
  add_control(BUTTON_CANCEL_X, BUTTON_CANCEL_Y, TERN(TOUCH_SCREEN, CANCEL_ITEM, NONE), imgCancel, true, yes ? HALF(COLOR_CONTROL_CANCEL) : COLOR_CONTROL_CANCEL);
  add_control(BUTTON_CONFIRM_X, BUTTON_CONFIRM_Y, TERN(TOUCH_SCREEN, CONFIRM, NONE), imgConfirm, true, yes ? COLOR_CONTROL_CONFIRM : HALF(COLOR_CONTROL_CONFIRM));
}

void Preview::screen() {
  ui.defer_status_screen();
  const bool yes = ui.update_selection(), got_click = ui.use_click();
  if (got_click) {
    if (yes) {
      card.openAndPrintFile(fileprop.name);
      ui.return_to_status();
      ui.reset_status();
    }
    else
      ui.goto_previous_screen();
  }
  else if (ui.should_draw())
    draw_screen(yes);
}

#endif // HAS_TFT_GCODE_PREVIEW
