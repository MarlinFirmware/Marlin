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

/**
 * TFT_COLOR_UI G-code thumbnail preview
 * Loosely based on DWIN PROUI gcode_preview.cpp
 */

#include "../../inc/MarlinConfig.h"

#if HAS_TFT_GCODE_PREVIEW

#include "ui_common.h"
#include "ui_gcode_preview.h"
#include "base64.h"
#include "image_decoders/image_decoder.h"

#include "../marlinui.h"
#include "../menu/menu.h"  // for menu_file_selector
#include "../../sd/cardreader.h"
#include "../../MarlinCore.h" // for wait_for_user

#define MAX_THUMBNAIL_SIZE 131072  // 128KB for thumbnails

//#define DEBUG_OUT 1
#include "../../core/debug_out.h"

Preview preview;

/**
 * Structure to hold file properties
 */
typedef struct {
  char name[13] = "";         //!< 8.3 + null filename
  uint32_t thumbstart = 0;    //!< File position of selected thumbnail
  uint32_t thumb_data_start = 0;  //!< Position where base64 data starts
  uint32_t thumbsize = 0;     //!< Size of thumbnail data
  uint16_t thumbheight = 0, thumbwidth = 0;  //!< Thumbnail dimensions
  float time = 0;             //!< Estimated print time
  float filament = 0;         //!< Filament used
  float layer = 0;            //!< Layer height
  float width = 0, height = 0, length = 0;  //!< Model dimensions
  uint16_t decoded_thumb[MAX_THUMBNAIL_SIZE / 2];  //!< RGB565 buffer (half size of raw data)
  bool decode_success = false;  //!< Track if JPEG decoding succeeded

  /**
   * Set the filename
   * @param fn Filename string
   */
  void setname(const char * const fn) {
    const uint8_t len = _MIN(sizeof(name) - 1, strlen(fn));
    memcpy(name, fn, len);
    name[len] = '\0';
  }

  /**
   * Clear all file properties
   */
  void clear() {
    name[0] = '\0';
    thumbstart = 0;
    thumb_data_start = 0;
    thumbsize = 0;
    thumbheight = thumbwidth = 0;
    time = 0;
    filament = 0;
    layer = 0;
    height = width = length = 0;
    memset(decoded_thumb, 0, sizeof(decoded_thumb));
    decode_success = false;
  }

} fileprop_t;

/**
 * Structure to hold thumbnail candidate information
 */
typedef struct {
  uint32_t position = 0;      //!< File position of thumbnail
  uint32_t data_start = 0;    //!< Position where base64 data starts
  uint16_t width = 0;         //!< Thumbnail width in pixels
  uint16_t height = 0;        //!< Thumbnail height in pixels
  uint32_t size = 0;          //!< Data size in bytes
  uint32_t area = 0;          //!< width * height for comparison
} thumb_candidate_t;

fileprop_t fileprop;

// Track current button selection for encoder navigation (0 = Cancel, 1 = Confirm)
static uint8_t button_selection = 1; // Default to Confirm

void getValue(const char * const buf, PGM_P const key, float &value) {
  if (value != 0.0f) return;

  const char *posptr = strstr_P(buf, key);
  if (posptr == nullptr) return;

  char num[10] = "";
  for (uint8_t i = 0; i < sizeof(num);) {
    const char c = *posptr;
    if (ISEOL(c) || c == '\0') {
      num[i] = '\0';
      value = atof(num);
      break;
    }
    if (WITHIN(c, '0', '9') || c == '.') num[i++] = c;
    posptr++;
  }
}

bool Preview::hasPreview() {
  DEBUG_ECHOLNPGM("G-code preview: hasPreview called, filename: ", card.filename, ", processed: ", processed);

  // If already processed, just return whether we have valid data
  if (processed) {
    DEBUG_ECHOLNPGM("G-code preview: Already processed (processed=", processed, "), returning valid status");
    return valid();
  }

  DEBUG_ECHOLNPGM("G-code preview: SD mounted: ", card.isMounted());

  const char * const tbstart = PSTR(" begin ");
  const char *posptr = nullptr;
  uint32_t indx = 0;
  uint32_t prev_indx = 0;
  float tmp = 0;

  // Array to hold thumbnail candidates
  thumb_candidate_t candidates[10];  // Support up to 10 thumbnails
  uint8_t candidate_count = 0;

  fileprop.clear();
  fileprop.setname(card.filename);

  card.openFileRead(card.filename);
  DEBUG_ECHOLNPGM("G-code preview: file opened, isOpen: ", card.isFileOpen());

  char buf[256];
  uint8_t nbyte = 1;
  bool found_gcode = false;
  // First pass: read through the file to find ALL thumbnails and parse all metadata
  // Scan until we find a line starting with M or G (G-code commands)
  while (nbyte > 0 && !found_gcode && candidate_count < 10) {
    nbyte = card.read(buf, sizeof(buf) - 1);
    if (nbyte >= 0) {
      buf[nbyte] = '\0';
      // Parse metadata regardless of whether we've found thumbnail yet
      getValue(buf, PSTR(";TIME:"), fileprop.time);
      getValue(buf, PSTR(";Filament used:"), fileprop.filament);
      getValue(buf, PSTR(";Layer height:"), fileprop.layer);
      getValue(buf, PSTR(";MINX:"), tmp);
      getValue(buf, PSTR(";MAXX:"), fileprop.width);
      fileprop.width -= tmp;
      tmp = 0;
      getValue(buf, PSTR(";MINY:"), tmp);
      getValue(buf, PSTR(";MAXY:"), fileprop.length);
      fileprop.length -= tmp;
      tmp = 0;
      getValue(buf, PSTR(";MINZ:"), tmp);
      getValue(buf, PSTR(";MAXZ:"), fileprop.height);

      // Check if we've reached G-code commands (lines starting with M or G)
      const char *line_start = buf;
      while (*line_start && !found_gcode) {
        // Skip leading whitespace
        while (*line_start == ' ' || *line_start == '\t') line_start++;
        if (*line_start == 'M' || *line_start == 'G') {
          found_gcode = true;
          break;
        }
        // Find next line
        line_start = strchr(line_start, '\n');
        if (!line_start) break;
        line_start++; // Skip the \n
      }

      // Look for ALL thumbnails in the file
      const char *search_start = buf;
      while ((posptr = strstr_P(search_start, tbstart)) != nullptr && candidate_count < 10) {
        // Check if " begin " is followed by valid dimensions (digits x digits)
        const char * dim_start = posptr + strlen_P(tbstart);
        bool valid_dimensions = false;
        if (isdigit(*dim_start)) {
          const char * x_pos = dim_start;
          while (isdigit(*x_pos)) x_pos++;
          if (*x_pos == 'x') {
            x_pos++;
            if (isdigit(*x_pos)) {
              valid_dimensions = true;
            }
          }
        }

        if (valid_dimensions) {
          // Parse dimensions for this candidate
          uint32_t saved_index = card.getIndex();
          uint32_t candidate_pos = indx + (posptr - buf);
          card.setIndex(candidate_pos + strlen_P(tbstart));

          char temp_linebuf[64];
          uint8_t i = 0;
          for (i = 0; i < sizeof(temp_linebuf) - 1; i++) {
            const char c = card.get();
            if (ISEOL(c) || c == '\0') { temp_linebuf[i] = '\0'; break; }
            temp_linebuf[i] = c;
          }

          uint16_t temp_width = 0, temp_height = 0;
          uint32_t temp_size = 0;
          char *xpos = strchr(temp_linebuf, 'x');
          char *spos = strchr(temp_linebuf, ' ');
          if (xpos && spos && xpos < spos) {
            *xpos = '\0';
            temp_width = atoi(temp_linebuf);
            *spos = '\0';
            temp_height = atoi(xpos + 1);
            temp_size = atoi(spos + 1);
          }

          // Store this candidate if it has valid dimensions
          if (temp_width > 0 && temp_height > 0 && temp_size > 0) {
            candidates[candidate_count].position = candidate_pos;
            candidates[candidate_count].data_start = card.getIndex();
            candidates[candidate_count].width = temp_width;
            candidates[candidate_count].height = temp_height;
            candidates[candidate_count].size = temp_size;
            candidates[candidate_count].area = temp_width * temp_height;
            candidate_count++;

            DEBUG_ECHOLNPGM("G-code preview: Found thumbnail candidate ", candidate_count, " at position ", candidate_pos, ", dimensions ", temp_width, C('x'), temp_height, ", size ", temp_size, ", area ", temp_width * temp_height);
          }

          card.setIndex(saved_index);  // Restore index
        }

        // Continue searching from after this match
        search_start = posptr + 1;
      }

      indx += _MAX(10, nbyte - (signed)strlen_P(tbstart));
      if (indx <= prev_indx) break;
      prev_indx = indx;
      card.setIndex(indx);
    }
    else
      break;
  }

  DEBUG_ECHOLNPGM("G-code preview: Found ", candidate_count, " thumbnail candidates");

  if (!found_gcode) {
    DEBUG_ECHOLNPGM("G-code preview: Scanned entire file header without finding G-code commands");
  }

  // Select the best thumbnail (largest that fits within display limits)
  uint32_t best_position = 0;
  uint8_t best_index = 0;
  uint16_t best_width = 0, best_height = 0;
  uint32_t best_size = 0;

  for (uint8_t i = 0; i < candidate_count; i++) {
    uint16_t w = candidates[i].width;
    uint16_t h = candidates[i].height;

    // Check if this thumbnail fits within our display limits
    if (w <= GCODE_PREVIEW_THUMB_WIDTH && h <= GCODE_PREVIEW_THUMB_HEIGHT) {
      // If this is the first valid candidate, or larger than previous best
      if (best_position == 0 || candidates[i].area > (best_width * best_height)) {
        best_position = candidates[i].position;
        best_index = i;
        best_width = w;
        best_height = h;
        best_size = candidates[i].size;
        DEBUG_ECHOLNPGM("G-code preview: Selected thumbnail candidate ", i+1, " (area ", candidates[i].area, ")");
      }
    } else {
      DEBUG_ECHOLNPGM("G-code preview: Skipping thumbnail candidate ", i+1, " - too large (", w, C('x'), h, " > ", GCODE_PREVIEW_THUMB_WIDTH, C('x'), GCODE_PREVIEW_THUMB_HEIGHT, ")");
    }
  }

  if (!best_position) {
    card.closefile();
    ui.set_status_P(PSTR("No suitable thumbnail found"));
    DEBUG_ECHOLNPGM("G-code preview: No suitable thumbnail found");
    return false;
  }

  // Use the best thumbnail
  fileprop.thumbstart = best_position;
  fileprop.thumb_data_start = candidates[best_index].data_start;
  fileprop.thumbwidth = best_width;
  fileprop.thumbheight = best_height;
  fileprop.thumbsize = best_size;

  DEBUG_ECHOLNPGM("G-code preview: Selected best thumbnail at position ", fileprop.thumbstart, ", dimensions ", fileprop.thumbwidth, C('x'), fileprop.thumbheight, ", size ", fileprop.thumbsize);

  // Exit if there isn't a thumbnail
  if (!fileprop.thumbsize) {
    card.closefile();
    ui.set_status_P(PSTR("Invalid Thumbnail Size"));
    return false;
  }

  // Set index to the start of the base64 data
  card.setIndex(fileprop.thumb_data_start);

  uint8_t buf64[fileprop.thumbsize + 1];
  uint16_t nread = 0;
  while (nread < fileprop.thumbsize) {
    const uint8_t c = card.get();
    if (!ISEOL(c) && c != ';' && c != ' ')
      buf64[nread++] = c;
  }
  card.closefile();
  buf64[nread] = '\0';

  DEBUG_ECHOLNPGM("G-code preview: nread = ", nread, ", buf64 first 20: ", String((char*)buf64).substring(0, 20));

  // For TFT, decode to a buffer and store for display
  // Assuming we can use a static buffer or allocate
  static uint8_t thumbdata[MAX_THUMBNAIL_SIZE];  // Reserve space for the JPEG thumbnail
  fileprop.thumbsize = decode_base64(buf64, thumbdata);

  DEBUG_ECHOLNPGM("G-code preview: Base64 decoded size: ", fileprop.thumbsize, ", first bytes: ", thumbdata[0], " ", thumbdata[1], " ", thumbdata[2], " ", thumbdata[3]);

  // Detect image format
  #if DEBUG_OUT
    const char *format_name = "UNKNOWN";
    if (TERN0(GCODE_PREVIEW_JPEG, ImageDecoders::JPEGDecoder::isValidJPEG(thumbdata, fileprop.thumbsize)))
      format_name = "JPEG";
    else if (TERN0(GCODE_PREVIEW_PNG, ImageDecoders::PNGDecoder::isValidPNG(thumbdata, fileprop.thumbsize)))
      format_name = "PNG";
    else if (TERN0(GCODE_PREVIEW_QOI, ImageDecoders::QOIDecoder::isValidQOI(thumbdata, fileprop.thumbsize)))
      format_name = "QOI";
    DEBUG_ECHOLNPGM("G-code preview: Detected image format: ", format_name);
  #endif

  // Get actual dimensions from JPEG header
  uint16_t jpeg_width, jpeg_height;
  if (ImageDecoders::ImageDecoder::getDimensions(thumbdata, fileprop.thumbsize, jpeg_width, jpeg_height)) {
    DEBUG_ECHOLNPGM("G-code preview: JPEG dimensions from header: ", jpeg_width, C('x'), jpeg_height);
    // Use JPEG dimensions instead of G-code dimensions
    fileprop.thumbwidth = jpeg_width;
    fileprop.thumbheight = jpeg_height;
  }
  else {
    DEBUG_ECHOLNPGM("G-code preview: Failed to get JPEG dimensions");
  }

  // Now decode the image data to RGB565
  bool decode_success = false;
  size_t required_bytes = (size_t)fileprop.thumbwidth * fileprop.thumbheight * 2;
  if (required_bytes <= sizeof(fileprop.decoded_thumb)) {
    decode_success = ImageDecoders::ImageDecoder::decode(
      thumbdata, fileprop.thumbsize,
      fileprop.decoded_thumb, fileprop.thumbwidth, fileprop.thumbheight
    );
  }
  else {
    DEBUG_ECHOLNPGM("G-code preview: Image too large for buffer: ", fileprop.thumbwidth, C('x'), fileprop.thumbheight,
           " needs ", required_bytes, " bytes, buffer has ", sizeof(fileprop.decoded_thumb), " bytes");
  }

  fileprop.decode_success = decode_success;

  DEBUG_ECHOLNPGM("G-code preview: JPEG decode result: success=", decode_success, ", buffer size needed=", required_bytes, ", actual buffer size=", sizeof(fileprop.decoded_thumb));

  if (!fileprop.decode_success) {
    DEBUG_ECHOLNPGM("G-code preview: Failed to decode thumbnail image");
    // Continue anyway - we can still show metadata
  }
  else {
    // Debug: Print first few decoded pixels
    DEBUG_ECHOPGM("G-code preview: First 10 decoded pixels: ");
    for (int i = 0; i < 10 && i < fileprop.thumbwidth * fileprop.thumbheight; i++) {
      DEBUG_ECHO(fileprop.decoded_thumb[i], HEX);
      DEBUG_CHAR(' ');
    }
    DEBUG_EOL();
  }

  DEBUG_ECHOLNPGM("G-code preview: Thumbnail found and decoded, size: ", fileprop.thumbsize, ", decode success: ", fileprop.decode_success);

  // Mark as processed to prevent repeated file I/O
  processed = true;
  DEBUG_ECHOLNPGM("G-code preview: Setting processed = true");

  return true;
}

void Preview::drawFromSD() {
  DEBUG_ECHOLNPGM("G-code preview: drawFromSD called");

  // Follow the TFT queue pattern like other screens
  tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);

  if (!valid()) {
    // Handle no preview - draw a test message
    DEBUG_ECHOLNPGM("G-code preview: no valid preview available");
    tft.add_text(20, 100, COLOR_WHITE, "Preview not available");
    tft.add_text(20, 120, COLOR_WHITE, "File: ");
    tft.add_text(80, 120, COLOR_WHITE, card.filename);

    // Draw thumbnail placeholder area
    #if ENABLED(TFT_COLOR_UI_PORTRAIT)
      // Portrait mode: thumbnail below text
      const int thumb_x = (TFT_WIDTH - GCODE_PREVIEW_THUMB_WIDTH) / 2;
      const int thumb_y = GCODE_PREVIEW_THUMB_Y;
    #else
      // Landscape mode: thumbnail on left
      const int thumb_x = 0;
      const int thumb_y = (TFT_HEIGHT - GCODE_PREVIEW_THUMB_HEIGHT) / 2;
    #endif
    tft.add_bar(thumb_x, thumb_y, GCODE_PREVIEW_THUMB_WIDTH, GCODE_PREVIEW_THUMB_HEIGHT, COLOR_RED);
    tft.add_text(thumb_x + 20, thumb_y + GCODE_PREVIEW_THUMB_HEIGHT / 2 - 10, COLOR_WHITE, "No thumbnail");

    // Draw metadata text
    char buf[100];
    int text_x = GCODE_PREVIEW_TEXT_X;
    int text_y = GCODE_PREVIEW_TEXT_Y_START;

    #if ENABLED(TFT_COLOR_UI_PORTRAIT)
      // Portrait mode: compact single-line format
      // Always show estimated time
      tft.add_text(text_x, text_y, COLOR_WHITE, "Estimated time:");
      if (fileprop.time > 0) {
        sprintf(buf, "%i:%02i", (uint16_t)fileprop.time / 3600, ((uint16_t)fileprop.time % 3600) / 60);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }
      text_y += 30;

      // Always show filament used
      tft.add_text(text_x, text_y, COLOR_WHITE, "Filament used:");
      if (fileprop.filament > 0) {
        sprintf(buf, "%.2f m", fileprop.filament);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }
      text_y += 30;

      // Always show layer height
      tft.add_text(text_x, text_y, COLOR_WHITE, "Layer height:");
      if (fileprop.layer > 0) {
        sprintf(buf, "%.2f mm", fileprop.layer);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }
      text_y += 30;

      // Always show volume
      tft.add_text(text_x, text_y, COLOR_WHITE, "Volume:");
      if (fileprop.width > 0 && fileprop.length > 0 && fileprop.height > 0) {
        sprintf(buf, "%.1fx%.1fx%.1f mm", fileprop.width, fileprop.length, fileprop.height);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }

    #else // TFT_COLOR_UI_LANDSCAPE

      // Landscape mode: two-line format, text on right
      // Always show estimated time
      tft.add_text(text_x, text_y, COLOR_WHITE, "Estimated time:");
      if (fileprop.time > 0) {
        sprintf(buf, "%i:%02i", (uint16_t)fileprop.time / 3600, ((uint16_t)fileprop.time % 3600) / 60);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
      text_y += 60;

      // Always show filament used
      tft.add_text(text_x, text_y, COLOR_WHITE, "Filament used:");
      if (fileprop.filament > 0) {
        sprintf(buf, "%.2f m", fileprop.filament);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
      text_y += 60;

      // Always show layer height
      tft.add_text(text_x, text_y, COLOR_WHITE, "Layer height:");
      if (fileprop.layer > 0) {
        sprintf(buf, "%.2f mm", fileprop.layer);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
      text_y += 60;

      // Always show volume
      tft.add_text(text_x, text_y, COLOR_WHITE, "Volume:");
      if (fileprop.width > 0 && fileprop.length > 0 && fileprop.height > 0) {
        sprintf(buf, "%.1fx%.1fx%.1f mm", fileprop.width, fileprop.length, fileprop.height);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
    #endif // TFT_COLOR_UI_LANDSCAPE

  }
  else { // valid()

    // Valid preview available
    char buf[100];

    #if ENABLED(TFT_COLOR_UI_PORTRAIT)
      // Portrait mode: text at top, thumbnail below
      int text_x = GCODE_PREVIEW_TEXT_X;
      int text_y = GCODE_PREVIEW_TEXT_Y_START;

      // Draw metadata in compact single-line format
      tft.add_text(text_x, text_y, COLOR_WHITE, "Estimated time:");
      if (fileprop.time > 0) {
        sprintf(buf, "%i:%02i", (uint16_t)fileprop.time / 3600, ((uint16_t)fileprop.time % 3600) / 60);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }
      text_y += 30;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Filament used:");
      if (fileprop.filament > 0) {
        sprintf(buf, "%.2f m", fileprop.filament);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }
      text_y += 30;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Layer height:");
      if (fileprop.layer > 0) {
        sprintf(buf, "%.2f mm", fileprop.layer);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }
      text_y += 30;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Volume:");
      if (fileprop.width > 0 && fileprop.length > 0 && fileprop.height > 0) {
        sprintf(buf, "%.1fx%.1fx%.1f mm", fileprop.width, fileprop.length, fileprop.height);
        tft.add_text(text_x + 120, text_y, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x + 120, text_y, COLOR_RED, "Not available");
      }

      // Draw thumbnail below text
      int thumb_x = (TFT_WIDTH - GCODE_PREVIEW_THUMB_WIDTH) / 2;  // Center horizontally
      int thumb_y = GCODE_PREVIEW_THUMB_Y;
      show(thumb_x, thumb_y);

    #else // TFT_COLOR_UI_LANDSCAPE
      // Landscape mode: thumbnail on left, text on right
      int thumb_x = 0;  // Hard left
      int thumb_y = (TFT_HEIGHT - fileprop.thumbheight) / 2;  // Centered vertically

      // Draw metadata text to the right of the thumbnail
      int text_x = GCODE_PREVIEW_TEXT_X;
      int text_y = GCODE_PREVIEW_TEXT_Y_START;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Estimated time:");
      if (fileprop.time > 0) {
        sprintf(buf, "%i:%02i", (uint16_t)fileprop.time / 3600, ((uint16_t)fileprop.time % 3600) / 60);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
      text_y += 60;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Filament used:");
      if (fileprop.filament > 0) {
        sprintf(buf, "%.2f m", fileprop.filament);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
      text_y += 60;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Layer height:");
      if (fileprop.layer > 0) {
        sprintf(buf, "%.2f mm", fileprop.layer);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }
      text_y += 60;

      tft.add_text(text_x, text_y, COLOR_WHITE, "Volume:");
      if (fileprop.width > 0 && fileprop.length > 0 && fileprop.height > 0) {
        sprintf(buf, "%.1fx%.1fx%.1f mm", fileprop.width, fileprop.length, fileprop.height);
        tft.add_text(text_x, text_y + 25, COLOR_WHITE, buf);
      } else {
        tft.add_text(text_x, text_y + 25, COLOR_RED, "Not available");
      }

      // Draw thumbnail area
      show(thumb_x, thumb_y);
    #endif // TFT_COLOR_UI_LANDSCAPE
  }

  // Always draw both buttons at the bottom of the screen
  const int button_y = GCODE_PREVIEW_BUTTON_Y,
            button1_x = GCODE_PREVIEW_BUTTON1_X,
            button2_x = GCODE_PREVIEW_BUTTON2_X;

  // Handle encoder input for button selection
  const int8_t encoder_delta = ui.get_encoder_delta();
  if (encoder_delta) {
    button_selection = encoder_delta > 0 ? 1 : 0; // Positive = Confirm, Negative = Cancel
    DEBUG_ECHOLNPGM("G-code preview: Encoder moved, selection now: ", button_selection ? "Confirm" : "Cancel");
  }

  // Draw buttons with highlighting for encoder selection
  const uint16_t confirm_color = (button_selection == 1) ? COLOR_WHITE : COLOR_LIME;
  const uint16_t cancel_color = (button_selection == 0) ? COLOR_WHITE : COLOR_RED;
  tft.add_image(button1_x, button_y, imgConfirm, confirm_color);
  tft.add_image(button2_x, button_y, imgCancel, cancel_color);

  // Add touch controls for the buttons
  #if ENABLED(TOUCH_SCREEN)
    touch.add_control(CONFIRM, button1_x, button_y, 64, 64);  // Confirm button
    touch.add_control(CANCEL, button2_x, button_y, 64, 64);  // Cancel button
  #endif

  tft.queue.sync();

  // Check for user input (non-blocking)
  if (ui.use_click()) {
    // For encoder-only systems, set ui.selection based on current button_selection
    #if DISABLED(TOUCH_SCREEN)
      ui.set_selection(button_selection == 1);
    #endif

    // Handle the user response
    if (ui.selection) {
      // User pressed CONFIRM - start printing
      DEBUG_ECHOLNPGM("G-code preview: User confirmed - starting print");
      card.openAndPrintFile(card.filename);
      ui.return_to_status();
    } else {
      // User pressed CANCEL - go back to SD card file browser
      DEBUG_ECHOLNPGM("G-code preview: User cancelled - returning to SD card file browser");
      ui.goto_screen(menu_file_selector);
    }
  }
}

void Preview::invalidate() {
  DEBUG_ECHOLNPGM("G-code preview: invalidate called, resetting processed flag");
  fileprop.thumbsize = 0;
  processed = false;
}

bool Preview::valid() {
  return !!fileprop.thumbsize;
}

void Preview::show(const int x, const int y) {
  DEBUG_ECHOLNPGM("G-code preview: show() called at (%d,%d), thumbsize: %d, width: %d, height: %d\n", x, y, fileprop.thumbsize, fileprop.thumbwidth, fileprop.thumbheight);

  // Display the decoded thumbnail if available
  if (fileprop.decode_success) {
    // Use the new add_pixels method to display raw RGB565 data
    tft.add_pixels(x, y, fileprop.thumbwidth, fileprop.thumbheight, fileprop.decoded_thumb);

    // Debug: Print first few decoded pixels
    DEBUG_ECHOPGM("G-code preview: First 10 decoded pixels: ");
    for (int i = 0; i < 10 && i < fileprop.thumbwidth * fileprop.thumbheight; i++) {
      DEBUG_ECHO(fileprop.decoded_thumb[i], HEX);
      DEBUG_CHAR(' ');
    }
    DEBUG_EOL();
  }
  else {
    // Fallback: Fill the thumbnail area with a visible color
    tft.add_bar(x, y, fileprop.thumbwidth, fileprop.thumbheight, COLOR_RED);

    char buf[50];
    sprintf(buf, "Decode failed");
    tft.add_text((TFT_WIDTH - 200) / 2, y + fileprop.thumbheight / 2 - 10, COLOR_WHITE, buf);
  }

  // Draw a border around the thumbnail area
  //tft.add_rectangle(x, y, fileprop.thumbwidth, fileprop.thumbheight, COLOR_WHITE);
}

#endif // HAS_TFT_GCODE_PREVIEW
