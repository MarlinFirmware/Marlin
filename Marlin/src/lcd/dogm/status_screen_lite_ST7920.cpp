/**
 * Lightweight Status Screen for the RepRapDiscount Full
 * Graphics Smart Controller (ST7920-based 128x64 LCD)
 *
 * (c) 2017 Aleph Objects, Inc.
 *
 * The code in this page is free software: you can
 * redistribute it and/or modify it under the terms of the GNU
 * General Public License (GNU GPL) as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.  The code is distributed WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
 *
 */

/**
 * Implementation of a Status Screen for the RepRapDiscount
 * Full Graphics Smart Controller using native ST7920 commands
 * instead of U8Glib.
 *
 * This alternative Status Screen makes use of the built-in character
 * generation capabilities of the ST7920 to update the Status Screen
 * with less SPI traffic and CPU use. In particular:
 *
 *  - The fan and bed animations are handled using custom characters
 *    that are stored in CGRAM. This allows for the animation to be
 *    updated by writing a single character to the text-buffer (DDRAM).
 *
 *  - All the information in the Status Screen is text that is written
 *    to DDRAM, so the work of generating the bitmaps is offloaded to
 *    the ST7920 rather than being render by U8Glib on the MCU.
 *
 *  - The graphics buffer (GDRAM) is only used for static graphics
 *    elements (nozzle and feedrate bitmaps) and for the progress
 *    bar, so updates are sporadic.
 */

//
// status_screen_lite_ST7920.cpp
// Lightweight Status Screen for Graphical Display
//

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(LIGHTWEIGHT_UI)

#include "status_screen_lite_ST7920.h"

#include "../ultralcd.h"
#include "../fontutils.h"
#include "../lcdprint.h"
#include "../../libs/duration_t.h"
#include "../../module/motion.h"
#include "../../module/printcounter.h"
#include "../../module/temperature.h"

#if ENABLED(SDSUPPORT)
  #include "../../sd/cardreader.h"
#endif

#define BUFFER_WIDTH   256
#define BUFFER_HEIGHT  32

#define DDRAM_LINE_1   0x00
#define DDRAM_LINE_2   0x10
#define DDRAM_LINE_3   0x08
#define DDRAM_LINE_4   0x18

ST7920_Lite_Status_Screen::st7920_state_t ST7920_Lite_Status_Screen::current_bits;

void ST7920_Lite_Status_Screen::cmd(const uint8_t cmd) {
  if (!current_bits.synced || !current_bits.cmd) {
    current_bits.synced = true;
    current_bits.cmd    = true;
    sync_cmd();
  }
  write_byte(cmd);
}

void ST7920_Lite_Status_Screen::begin_data() {
  if (!current_bits.synced || current_bits.cmd) {
    current_bits.synced = true;
    current_bits.cmd    = false;
    sync_dat();
  }
}

void ST7920_Lite_Status_Screen::write_str(const char *str) {
  while (*str) write_byte(*str++);
}

void ST7920_Lite_Status_Screen::write_str(const char *str, uint8_t len) {
  while (*str && len--) write_byte(*str++);
}

void ST7920_Lite_Status_Screen::write_str_P(PGM_P const str) {
  PGM_P p_str = (PGM_P)str;
  while (char c = pgm_read_byte(p_str++)) write_byte(c);
}

void ST7920_Lite_Status_Screen::write_str(progmem_str str) {
  write_str_P((PGM_P)str);
}

void ST7920_Lite_Status_Screen::write_number(const int16_t value, const uint8_t digits/*=3*/) {
  char str[7];
  PGM_P fmt;
  switch (digits) {
    case 6: fmt = PSTR("%6d"); break;
    case 5: fmt = PSTR("%5d"); break;
    case 4: fmt = PSTR("%4d"); break;
    case 3: fmt = PSTR("%3d"); break;
    case 2: fmt = PSTR("%2d"); break;
    case 1: fmt = PSTR("%1d"); break;
    default: return;
  }
  sprintf_P(str, fmt, value);
  write_str(str);
}

void ST7920_Lite_Status_Screen::display_status(const bool display_on, const bool cursor_on, const bool blink_on) {
  extended_function_set(false);
  cmd(0b00001000 |
    (display_on ? 0b0100 : 0) |
    (cursor_on  ? 0b0010 : 0) |
    (blink_on   ? 0b0001 : 0)
  );
}

// Sets the extended and graphics bits simultaneously, regardless of
// the current state. This is a helper function for extended_function_set()
// and graphics()
void ST7920_Lite_Status_Screen::_extended_function_set(const bool extended, const bool graphics) {
  cmd(  0b00100000 |
    (extended   ? 0b00000100 : 0) |
    (graphics   ? 0b00000010 : 0)
  );
  current_bits.extended = extended;
  current_bits.graphics = graphics;
}

void ST7920_Lite_Status_Screen::extended_function_set(const bool extended) {
  if (extended != current_bits.extended)
    _extended_function_set(extended, current_bits.graphics);
}

void ST7920_Lite_Status_Screen::graphics(const bool graphics) {
  if (graphics != current_bits.graphics)
    _extended_function_set(current_bits.extended, graphics);
}

void ST7920_Lite_Status_Screen::entry_mode_select(const bool ac_increase, const bool shift) {
  extended_function_set(false);
  cmd(0b00000100 |
    (ac_increase ? 0b00000010 : 0) |
    (shift       ? 0b00000001 : 0)
  );
}

// Sets the sa bit regardless of the current state. This is a helper
// function for scroll_or_addr_select()
void ST7920_Lite_Status_Screen::_scroll_or_addr_select(const bool sa) {
  extended_function_set(true);
  cmd(0b00100010 |
    (sa   ? 0b000001 : 0)
  );
  current_bits.sa = sa;
}

void ST7920_Lite_Status_Screen::scroll_or_addr_select(const bool sa) {
  if (sa != current_bits.sa)
    _scroll_or_addr_select(sa);
}

void ST7920_Lite_Status_Screen::set_ddram_address(const uint8_t addr) {
  extended_function_set(false);
  cmd(0b10000000 | (addr & 0b00111111));
}

void ST7920_Lite_Status_Screen::set_cgram_address(const uint8_t addr) {
  extended_function_set(false);
  cmd(0b01000000 | (addr & 0b00111111));
}

void ST7920_Lite_Status_Screen::set_gdram_address(const uint8_t x, const uint8_t y) {
  extended_function_set(true);
  cmd(0b10000000 | (y & 0b01111111));
  cmd(0b10000000 | (x & 0b00001111));
}

void ST7920_Lite_Status_Screen::clear() {
  extended_function_set(false);
  cmd(0x00000001);
  delay(15);                 //delay for CGRAM clear
}

void ST7920_Lite_Status_Screen::home() {
  extended_function_set(false);
  cmd(0x00000010);
}

/* This fills the entire text buffer with spaces */
void ST7920_Lite_Status_Screen::clear_ddram() {
  set_ddram_address(DDRAM_LINE_1);
  begin_data();
  for (uint8_t i = 64; i--;) write_byte(' ');
}

/* This fills the entire graphics buffer with zeros */
void ST7920_Lite_Status_Screen::clear_gdram() {
  for (uint8_t y = 0; y < BUFFER_HEIGHT; y++) {
    set_gdram_address(0, y);
    begin_data();
    for (uint8_t i = (BUFFER_WIDTH) / 16; i--;) write_word(0);
  }
}

void ST7920_Lite_Status_Screen::load_cgram_icon(const uint16_t addr, const void *data) {
  const uint16_t *p_word = (const uint16_t *)data;
  set_cgram_address(addr);
  begin_data();
  for (uint8_t i = 16; i--;)
    write_word(pgm_read_word(p_word++));
}

/**
 * Draw an icon in GDRAM. Position specified in DDRAM
 * coordinates. i.e., X from 1 to 8, Y from 1 to 4.
 */
void ST7920_Lite_Status_Screen::draw_gdram_icon(uint8_t x, uint8_t y, const void *data) {
  const uint16_t *p_word = (const uint16_t *)data;
  // Handle display folding
  if (y > 1) y -= 2, x += 8;
  for (int i = 0; i < 16; i++) {
    set_gdram_address(x, i + y * 16);
    begin_data();
    write_word(pgm_read_word(p_word++));
  }
}

/************************** ICON DEFINITIONS *************************************/

#define CGRAM_ICON_1_ADDR 0x00
#define CGRAM_ICON_2_ADDR 0x10
#define CGRAM_ICON_3_ADDR 0x20
#define CGRAM_ICON_4_ADDR 0x30

#define CGRAM_ICON_1_WORD 0x00
#define CGRAM_ICON_2_WORD 0x02
#define CGRAM_ICON_3_WORD 0x04
#define CGRAM_ICON_4_WORD 0x06

const uint8_t degree_symbol_y_top = 1;
PROGMEM const uint8_t degree_symbol[] = {
  0b00110000,
  0b01001000,
  0b01001000,
  0b00110000,
};

const uint16_t nozzle_icon[] PROGMEM = {
  0b0000000000000000,
  0b0000000000000000,
  0b0000111111110000,
  0b0001111111111000,
  0b0001111111111000,
  0b0001111111111000,
  0b0000111111110000,
  0b0000111111110000,
  0b0001111111111000,
  0b0001111111111000,
  0b0001111111111000,
  0b0000011111100000,
  0b0000001111000000,
  0b0000000110000000,
  0b0000000000000000,
  0b0000000000000000
};

const uint16_t bed_icon[] PROGMEM = {
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0111111111111110,
  0b0111111111111110,
  0b0110000000000110,
  0b0000000000000000,
  0b0000000000000000
};

const uint16_t heat1_icon[] PROGMEM = {
  0b0000000000000000,
  0b0010001000100000,
  0b0001000100010000,
  0b0000100010001000,
  0b0000100010001000,
  0b0001000100010000,
  0b0010001000100000,
  0b0010001000100000,
  0b0001000100010000,
  0b0000100010001000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000
};

const uint16_t heat2_icon[] PROGMEM = {
  0b0000000000000000,
  0b0000100010001000,
  0b0000100010001000,
  0b0001000100010000,
  0b0010001000100000,
  0b0010001000100000,
  0b0001000100010000,
  0b0000100010001000,
  0b0000100010001000,
  0b0001000100010000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000,
  0b0000000000000000
};

const uint16_t fan1_icon[] PROGMEM = {
  0b0000000000000000,
  0b0111111111111110,
  0b0111000000001110,
  0b0110001111000110,
  0b0100001111000010,
  0b0100000110000010,
  0b0101100000011010,
  0b0101110110111010,
  0b0101100000011010,
  0b0100000110000010,
  0b0100001111000010,
  0b0110001111000110,
  0b0111000000001110,
  0b0111111111111110,
  0b0000000000000000,
  0b0000000000000000
};

const uint16_t fan2_icon[] PROGMEM = {
  0b0000000000000000,
  0b0111111111111110,
  0b0111000000001110,
  0b0110010000100110,
  0b0100111001110010,
  0b0101111001111010,
  0b0100110000110010,
  0b0100000110000010,
  0b0100110000110010,
  0b0101111001111010,
  0b0100111001110010,
  0b0110010000100110,
  0b0111000000001110,
  0b0111111111111110,
  0b0000000000000000,
  0b0000000000000000
};

const uint16_t feedrate_icon[] PROGMEM = {
  0b0000000000000000,
  0b0111111000000000,
  0b0110000000000000,
  0b0110000000000000,
  0b0110000000000000,
  0b0111111011111000,
  0b0110000011001100,
  0b0110000011001100,
  0b0110000011001100,
  0b0110000011111000,
  0b0000000011001100,
  0b0000000011001100,
  0b0000000011001100,
  0b0000000011001100,
  0b0000000000000000,
  0b0000000000000000
};

/************************** MAIN SCREEN *************************************/

/**
 * The ST7920 has no degree character, so draw it to GDRAM.
 * This function takes character position xy
 * i.e., x is [0-15], while the y position is [0-3]
 */
void ST7920_Lite_Status_Screen::draw_degree_symbol(uint8_t x, uint8_t y, const bool draw) {
  const uint8_t *p_bytes = degree_symbol;
    // Handle display folding
    if (y > 1) y -= 2, x += 16;
    const bool    oddChar = x & 1;
    const uint8_t x_word  = x >> 1,
                  y_top   = degree_symbol_y_top,
                  y_bot   = y_top + sizeof(degree_symbol)/sizeof(degree_symbol[0]);
    for (uint8_t i = y_top; i < y_bot; i++) {
      uint8_t byte = pgm_read_byte(p_bytes++);
      set_gdram_address(x_word, i + y * 16);
      begin_data();
      if (draw) {
        write_byte(oddChar ? 0x00 : byte);
        write_byte(oddChar ? byte : 0x00);
      }
      else
        write_word(0x0000);
    }
}

void ST7920_Lite_Status_Screen::draw_static_elements() {
  scroll_or_addr_select(0);

  // Load the animated bed and fan icons
  load_cgram_icon(CGRAM_ICON_1_ADDR, heat1_icon);
  load_cgram_icon(CGRAM_ICON_2_ADDR, heat2_icon);
  load_cgram_icon(CGRAM_ICON_3_ADDR, fan1_icon);
  load_cgram_icon(CGRAM_ICON_4_ADDR, fan2_icon);

  // Draw the static icons in GDRAM
  draw_gdram_icon(0, 0, nozzle_icon);
  #if HOTENDS > 1
    draw_gdram_icon(0, 1, nozzle_icon);
    draw_gdram_icon(0, 2, bed_icon);
  #else
    draw_gdram_icon(0, 1, bed_icon);
  #endif
  draw_gdram_icon(5, 1, feedrate_icon);

  // Draw the initial fan icon
  draw_fan_icon(false);
}

/**
 * Although this is undocumented, the ST7920 allows the character
 * data buffer (DDRAM) to be used in conjunction with the graphics
 * bitmap buffer (CGRAM). The contents of the graphics buffer is
 * XORed with the data from the character generator. This allows
 * us to make the progess bar out of graphical data (the bar) and
 * text data (the percentage).
 */
void ST7920_Lite_Status_Screen::draw_progress_bar(const uint8_t value) {
  #if HOTENDS == 1
    // If we have only one extruder, draw a long progress bar on the third line
    constexpr uint8_t top     = 1,         // Top in pixels
                      bottom  = 13,        // Bottom in pixels
                      left    = 12,        // Left edge, in 16-bit words
                      width   = 4;         // Width of progress bar, in 16-bit words
  #else
    constexpr uint8_t top     = 16 + 1,
                      bottom  = 16 + 13,
                      left    = 5,
                      width   = 3;
  #endif
  const uint8_t char_pcnt  = 100 / width; // How many percent does each 16-bit word represent?

  // Draw the progress bar as a bitmap in CGRAM
  for (uint8_t y = top; y <= bottom; y++) {
    set_gdram_address(left, y);
    begin_data();
    for (uint8_t x = 0; x < width; x++) {
      uint16_t gfx_word = 0x0000;
      if ((x + 1) * char_pcnt <= value)
        gfx_word = 0xFFFF;                                              // Draw completely filled bytes
      else if ((x * char_pcnt) < value)
        gfx_word = int(0x8000) >> (value % char_pcnt) * 16 / char_pcnt; // Draw partially filled bytes

      // Draw the frame around the progress bar
      if (y == top || y == bottom)
        gfx_word = 0xFFFF;        // Draw top/bottom border
      else if (x == width - 1)
        gfx_word |= 0x0001;       // Draw right border
      else if (x == 0)
        gfx_word |= 0x8000;       // Draw left border
      write_word(gfx_word);
    }
  }

  // Draw the percentage as text in DDRAM
  #if HOTENDS == 1
    set_ddram_address(DDRAM_LINE_3 + 4);
    begin_data();
    write_byte(' ');
  #else
    set_ddram_address(DDRAM_LINE_2 + left);
    begin_data();
  #endif

  // Draw centered
  if (value > 9) {
    write_number(value, 4);
    write_str(F("% "));
  }
  else {
    write_number(value, 3);
    write_str(F("%  "));
  }
}

void ST7920_Lite_Status_Screen::draw_fan_icon(const bool whichIcon) {
  set_ddram_address(DDRAM_LINE_1 + 5);
  begin_data();
  write_word(whichIcon ? CGRAM_ICON_3_WORD : CGRAM_ICON_4_WORD);
}

void ST7920_Lite_Status_Screen::draw_heat_icon(const bool whichIcon, const bool heating) {
  set_ddram_address(
    #if HOTENDS == 1
      DDRAM_LINE_2
    #else
      DDRAM_LINE_3
    #endif
  );
  begin_data();
  if (heating)
    write_word(whichIcon ? CGRAM_ICON_1_WORD : CGRAM_ICON_2_WORD);
  else {
    write_byte(' ');
    write_byte(' ');
  }
}

#define FAR(a,b) (((a > b) ? (a-b) : (b-a)) > 2)

static struct {
  bool E1_show_target  : 1;
  bool E2_show_target  : 1;
  #if HAS_HEATED_BED
    bool bed_show_target : 1;
  #endif
} display_state = {
  true, true
  #if HAS_HEATED_BED
    , true
  #endif
};

void ST7920_Lite_Status_Screen::draw_temps(uint8_t line, const int16_t temp, const int16_t target, bool showTarget, bool targetStateChange) {
  switch (line) {
    case 0: set_ddram_address(DDRAM_LINE_1 + 1); break;
    case 1: set_ddram_address(DDRAM_LINE_2 + 1); break;
    case 2: set_ddram_address(DDRAM_LINE_3 + 1); break;
    case 3: set_ddram_address(DDRAM_LINE_3 + 1); break;
  }
  begin_data();
  write_number(temp);

  if (showTarget) {
    write_str(F("\x1A"));
    write_number(target);
  };

  if (targetStateChange) {
    if (!showTarget) write_str(F("    "));
    draw_degree_symbol(5, line, !showTarget);
    draw_degree_symbol(9, line,  showTarget);
  }
}

void ST7920_Lite_Status_Screen::draw_extruder_1_temp(const int16_t temp, const int16_t target, bool forceUpdate) {
  const bool show_target = target && FAR(temp, target);
  draw_temps(0, temp, target, show_target, display_state.E1_show_target != show_target || forceUpdate);
  display_state.E1_show_target = show_target;
}

void ST7920_Lite_Status_Screen::draw_extruder_2_temp(const int16_t temp, const int16_t target, bool forceUpdate) {
  const bool show_target = target && FAR(temp, target);
  draw_temps(1, temp, target, show_target, display_state.E2_show_target != show_target || forceUpdate);
  display_state.E2_show_target = show_target;
}

#if HAS_HEATED_BED
  void ST7920_Lite_Status_Screen::draw_bed_temp(const int16_t temp, const int16_t target, bool forceUpdate) {
    const bool show_target = target && FAR(temp, target);
    draw_temps(1
      #if HOTENDS > 1
        + 1
      #endif
      , temp, target, show_target, display_state.bed_show_target != show_target || forceUpdate
    );
    display_state.bed_show_target = show_target;
  }
#endif

void ST7920_Lite_Status_Screen::draw_fan_speed(const uint8_t value) {
  set_ddram_address(DDRAM_LINE_1 + 6);
  begin_data();
  write_number(value, 3);
  write_byte('%');
}

void ST7920_Lite_Status_Screen::draw_print_time(const duration_t &elapsed) {
  #if HOTENDS == 1
    set_ddram_address(DDRAM_LINE_3);
  #else
    set_ddram_address(DDRAM_LINE_3 + 5);
  #endif
  char str[7];
  str[elapsed.toDigital(str)] = ' ';
  begin_data();
  write_str(str, 6);
}

void ST7920_Lite_Status_Screen::draw_feedrate_percentage(const uint16_t percentage) {
  // We only have enough room for the feedrate when
  // we have one extruder
  #if HOTENDS == 1
    set_ddram_address(DDRAM_LINE_2 + 6);
    begin_data();
    write_number(percentage, 3);
    write_byte('%');
  #endif
}

void ST7920_Lite_Status_Screen::draw_status_message() {
  const char *str = ui.status_message;

  set_ddram_address(DDRAM_LINE_4);
  begin_data();
  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    uint8_t slen = utf8_strlen(str);

    if (slen <= LCD_WIDTH) {
      // String fits the LCD, so just print it
      write_str(str);
      for (; slen < LCD_WIDTH; ++slen) write_byte(' ');
    }
    else {
      // String is larger than the available space in screen.

      // Get a pointer to the next valid UTF8 character
      const char *stat = str + ui.status_scroll_offset;

      // Get the string remaining length
      const uint8_t rlen = utf8_strlen(stat);

      // If we have enough characters to display
      if (rlen >= LCD_WIDTH) {
        // The remaining string fills the screen - Print it
        write_str(stat, LCD_WIDTH);
      }
      else {
        // The remaining string does not completely fill the screen
        write_str(stat);                        // The string leaves space
        uint8_t chars = LCD_WIDTH - rlen;         // Amount of space left in characters

        write_byte('.');                        // Always at 1+ spaces left, draw a dot
        if (--chars) {                          // Draw a second dot if there's space
          write_byte('.');
          if (--chars)
            write_str(str, chars);              // Print a second copy of the message
        }
      }

      // Adjust by complete UTF8 characters
      if (ui.status_scroll_offset < slen) {
        ui.status_scroll_offset++;
        while (!START_OF_UTF8_CHAR(str[ui.status_scroll_offset]))
          ui.status_scroll_offset++;
      }
      else
        ui.status_scroll_offset = 0;
    }

  #else

    uint8_t slen = utf8_strlen(str);
    write_str(str, LCD_WIDTH);
    for (; slen < LCD_WIDTH; ++slen) write_byte(' ');

  #endif
}

void ST7920_Lite_Status_Screen::draw_position(const float x, const float y, const float z, bool position_known) {
  char str[7];
  set_ddram_address(DDRAM_LINE_4);
  begin_data();

  // If position is unknown, flash the labels.
  const unsigned char alt_label = position_known ? 0 : (ui.get_blink() ? ' ' : 0);

  dtostrf(x, -4, 0, str);
  write_byte(alt_label ? alt_label : 'X');
  write_str(str, 4);

  dtostrf(y, -4, 0, str);
  write_byte(alt_label ? alt_label : 'Y');
  write_str(str, 4);

  dtostrf(z, -5, 1, str);
  write_byte(alt_label ? alt_label : 'Z');
  write_str(str, 5);
}

bool ST7920_Lite_Status_Screen::indicators_changed() {
  // We only add the target temperatures to the checksum
  // because the actual temps fluctuate so by updating
  // them only during blinks we gain a bit of stability.
  const bool       blink             = ui.get_blink();
  const uint16_t   feedrate_perc     = feedrate_percentage;
  const uint8_t    fs                = (((uint16_t)fan_speed[0] + 1) * 100) / 256;
  const int16_t    extruder_1_target = thermalManager.degTargetHotend(0);
  #if HOTENDS > 1
    const int16_t  extruder_2_target = thermalManager.degTargetHotend(1);
  #endif
  #if HAS_HEATED_BED
    const int16_t  bed_target        = thermalManager.degTargetBed();
  #endif
  static uint16_t last_checksum = 0;
  const uint16_t checksum = blink ^ feedrate_perc ^ fs ^ extruder_1_target
    #if HOTENDS > 1
      ^ extruder_2_target
    #endif
    #if HAS_HEATED_BED
      ^ bed_target
    #endif
  ;
  if (last_checksum == checksum) return false;
  last_checksum = checksum;
  return true;
}

void ST7920_Lite_Status_Screen::update_indicators(const bool forceUpdate) {
  if (forceUpdate || indicators_changed()) {
    const bool       blink             = ui.get_blink();
    const duration_t elapsed           = print_job_timer.duration();
    const uint16_t   feedrate_perc     = feedrate_percentage;
    const uint8_t    fs                = (((uint16_t)fan_speed[0] + 1) * 100) / 256;
    const int16_t    extruder_1_temp   = thermalManager.degHotend(0),
                     extruder_1_target = thermalManager.degTargetHotend(0);
    #if HOTENDS > 1
      const int16_t  extruder_2_temp   = thermalManager.degHotend(1),
                     extruder_2_target = thermalManager.degTargetHotend(1);
    #endif
    #if HAS_HEATED_BED
      const int16_t  bed_temp          = thermalManager.degBed(),
                     bed_target        = thermalManager.degTargetBed();
    #endif

    draw_extruder_1_temp(extruder_1_temp, extruder_1_target, forceUpdate);
    #if HOTENDS > 1
      draw_extruder_2_temp(extruder_2_temp, extruder_2_target, forceUpdate);
    #endif
    #if HAS_HEATED_BED
      draw_bed_temp(bed_temp, bed_target, forceUpdate);
    #endif
    draw_fan_speed(fs);
    draw_print_time(elapsed);
    draw_feedrate_percentage(feedrate_perc);

    // Update the fan and bed animations
    if (fs) draw_fan_icon(blink);
    #if HAS_HEATED_BED
      draw_heat_icon(bed_target > 0 && blink, bed_target > 0);
    #endif
  }
}

bool ST7920_Lite_Status_Screen::position_changed() {
  const float x_pos = current_position[X_AXIS], y_pos = current_position[Y_AXIS], z_pos = current_position[Z_AXIS];
  const uint8_t checksum = uint8_t(x_pos) ^ uint8_t(y_pos) ^ uint8_t(z_pos);
  static uint8_t last_checksum = 0, changed = last_checksum != checksum;
  if (changed) last_checksum = checksum;
  return changed;
}

bool ST7920_Lite_Status_Screen::status_changed() {
  uint8_t checksum = 0;
  for (const char *p = ui.status_message; *p; p++) checksum ^= *p;
  static uint8_t last_checksum = 0, changed = last_checksum != checksum;
  if (changed) last_checksum = checksum;
  return changed;
}

bool ST7920_Lite_Status_Screen::blink_changed() {
  static uint8_t last_blink = 0;
  const bool blink = ui.get_blink(), changed = last_blink != blink;
  if (changed) last_blink = blink;
  return changed;
}

#ifndef STATUS_EXPIRE_SECONDS
  #define STATUS_EXPIRE_SECONDS 20
#endif

void ST7920_Lite_Status_Screen::update_status_or_position(bool forceUpdate) {

  #if STATUS_EXPIRE_SECONDS
    static uint8_t countdown = 0;
  #endif

  /**
   * There's only enough room for either the status message or the position,
   * so draw one or the other. When the status message changes, show it for
   * a few seconds, then return to the position display once the head moves.
   *
   *  countdown > 1  -- Show status
   *  countdown = 1  -- Show status, until movement
   *  countdown = 0  -- Show position
   *
   * If STATUS_EXPIRE_SECONDS is zero, only the status is shown.
   */
  if (forceUpdate || status_changed()) {
    #if ENABLED(STATUS_MESSAGE_SCROLLING)
      ui.status_scroll_offset = 0;
    #endif
    #if STATUS_EXPIRE_SECONDS
      countdown = ui.status_message[0] ? STATUS_EXPIRE_SECONDS : 0;
    #endif
    draw_status_message();
    blink_changed(); // Clear changed flag
  }
  #if !STATUS_EXPIRE_SECONDS
    #if ENABLED(STATUS_MESSAGE_SCROLLING)
      else
        draw_status_message();
    #endif
  #else
    else if (blink_changed()) {
      if (countdown > 1) {
        countdown--;
        #if ENABLED(STATUS_MESSAGE_SCROLLING)
          draw_status_message();
        #endif
      }
      else if (countdown > 0) {
        if (position_changed()) {
          countdown--;
          forceUpdate = true;
        }
        #if ENABLED(STATUS_MESSAGE_SCROLLING)
          draw_status_message();
        #endif
      }
    }

    if (countdown == 0 && (forceUpdate || position_changed() ||
      #if DISABLED(DISABLE_REDUCED_ACCURACY_WARNING)
        blink_changed()
      #endif
    )) {
      draw_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],
        #if ENABLED(DISABLE_REDUCED_ACCURACY_WARNING)
          true
        #else
          all_axes_known()
        #endif
      );
    }
  #endif
}

void ST7920_Lite_Status_Screen::update_progress(const bool forceUpdate) {
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY) || ENABLED(SDSUPPORT)

    // Since the progress bar involves writing
    // quite a few bytes to GDRAM, only do this
    // when an update is actually necessary.

    static uint8_t last_progress = 0;
    const uint8_t progress = ui.get_progress();
    if (forceUpdate || last_progress != progress) {
      last_progress = progress;
      draw_progress_bar(progress);
    }

  #else

    UNUSED(forceUpdate);

  #endif // LCD_SET_PROGRESS_MANUALLY || SDSUPPORT
}

void ST7920_Lite_Status_Screen::update(const bool forceUpdate) {
  cs();
  update_indicators(forceUpdate);
  update_status_or_position(forceUpdate);
  update_progress(forceUpdate);
  ncs();
}

void ST7920_Lite_Status_Screen::reset_state_from_unknown() {
  _extended_function_set(true, true); // Do it twice as only one bit
  _extended_function_set(true, true); // get set at a time.
  _scroll_or_addr_select(false);
}

void ST7920_Lite_Status_Screen::on_entry() {
  cs();
  reset_state_from_unknown();
  clear();
  clear_gdram();
  draw_static_elements();
  update(true);
  ncs();
}

void ST7920_Lite_Status_Screen::on_exit() {
  cs();
  clear();
  _extended_function_set(true, true); // Restore state to what u8g expects.
  ncs();
}

// This is called prior to the KILL screen to
// clear the screen, preventing a garbled display.
void ST7920_Lite_Status_Screen::clear_text_buffer() {
  cs();
  reset_state_from_unknown();
  clear();
  _extended_function_set(true, true); // Restore state to what u8g expects.
  ncs();
}

#if ENABLED(U8GLIB_ST7920) && !defined(U8G_HAL_LINKS) && !defined(__SAM3X8E__)

  #include "ultralcd_st7920_u8glib_rrd_AVR.h"

  void ST7920_Lite_Status_Screen::cs() {
    ST7920_CS();
    current_bits.synced = false;
  }

  void ST7920_Lite_Status_Screen::ncs() {
    ST7920_NCS();
    current_bits.synced = false;
  }

  void ST7920_Lite_Status_Screen::sync_cmd() {
    ST7920_SET_CMD();
  }

  void ST7920_Lite_Status_Screen::sync_dat() {
    ST7920_SET_DAT();
  }

  void ST7920_Lite_Status_Screen::write_byte(const uint8_t data) {
    ST7920_WRITE_BYTE(data);
  }

#endif

void MarlinUI::draw_status_screen() {
  ST7920_Lite_Status_Screen::update(false);
}

// This method is called before each screen update and
// fires on_entry() and on_exit() events upon entering
// or exiting the Status Screen.
void MarlinUI::lcd_in_status(const bool inStatus) {
  static bool lastInStatus = false;
  if (lastInStatus == inStatus) return;
  if ((lastInStatus = inStatus))
    ST7920_Lite_Status_Screen::on_entry();
  else
    ST7920_Lite_Status_Screen::on_exit();
}

#endif // LIGHTWEIGHT_UI
