/*
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

#ifndef ULTRALCD_ST7920_LITE_STATUS_H
#define ULTRALCD_ST7920_LITE_STATUS_H

class ST7920_Lite_Status_Screen {
  private:
    static struct st7920_state_t {
      uint8_t synced   : 1; // Whether a sync has been sent
      uint8_t cmd      : 1; // Whether the sync was cmd or data
      uint8_t extended : 1;
      uint8_t graphics : 1;
      uint8_t sa       : 1;
    } current_bits;

    static void cs();
    static void ncs();
    static void sync_cmd();
    static void sync_dat();
    static void write_byte(uint8_t w);

    static void cmd(uint8_t cmd);
    static void begin_data();
    static void write_word(uint16_t w);
    static void write_str(const char *str);
    static void write_str(const char *str, uint8_t len);
    static void write_str_P(const char *str);
    static void write_str(progmem_str str);
    static void write_number(uint8_t value, uint8_t digits=3);

    static void _extended_function_set(bool extended, bool graphics);
    static void _scroll_or_addr_select(bool sa);
    static void reset_state_from_unknown();

    static void home();
    static void display_status(bool display_on, bool cursor_on, bool blink_on);
    static void extended_function_set(bool extended);
    static void graphics(bool graphics);
    static void entry_mode_select(bool ac_increase, bool shift);
    static void scroll_or_addr_select(bool sa);
    static void set_ddram_address(uint8_t addr);
    static void set_cgram_address(uint8_t addr);
    static void set_gdram_address(uint8_t x, uint8_t y);

    static void clear();
    static void clear_ddram();
    static void clear_gdram();

    static void load_cgram_icon(uint16_t addr, const void *data);
    static void draw_gdram_icon(uint8_t x, uint8_t y, const void *data);

    static uint8_t string_checksum(const char *str);

  protected:
    static void draw_static_elements();
    static void draw_progress_bar(uint8_t value);
    static void draw_fan_icon(bool whichIcon);
    static void draw_heat_icon(bool whichIcon, bool heating);
    static void draw_extruder_1_temp(uint8_t temp, uint8_t target);
    static void draw_extruder_2_temp(uint8_t temp, uint8_t target);
    static void draw_bed_temp(uint8_t temp, uint8_t target);
    static void draw_fan_speed(uint8_t value);
    static void draw_print_time(uint32_t elapsed);
    static void draw_feedrate_percentage(uint8_t percentage);
    static void draw_status_message(const char *str);
    static void draw_position(const float x, const float y, const float z, bool position_known = true);

    static bool indicators_changed();
    static bool position_changed();
    static bool blink_changed();
    static bool status_changed();

    static void update_indicators(bool forceUpdate);
    static void update_position(bool forceUpdate, bool resetChecksum);
    static void update_status_or_position(bool forceUpdate);
    static void update_progress(bool forceUpdate);

  public:
    static void update(bool forceUpdate);
    static void on_entry();
    static void on_exit();
    static void clear_text_buffer();
};

/************************** ICON DEFINITIONS *************************************/

#define CGRAM_ICON_1_ADDR 0x00
#define CGRAM_ICON_2_ADDR 0x10
#define CGRAM_ICON_3_ADDR 0x20
#define CGRAM_ICON_4_ADDR 0x30

#define CGRAM_ICON_1_WORD 0x00
#define CGRAM_ICON_2_WORD 0x02
#define CGRAM_ICON_3_WORD 0x04
#define CGRAM_ICON_4_WORD 0x06

PROGMEM const uint16_t nozzle_icon[] = {
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

PROGMEM const uint16_t bed_icon[] = {
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
  0b0000000000000000,
  0b0111111111111110,
  0b0111111111111110,
  0b0000000000000000,
  0b0000000000000000
};

PROGMEM const uint16_t heat1_icon[] = {
  0b0000000000000000,
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
  0b0000000000000000
};

PROGMEM const uint16_t heat2_icon[] = {
  0b0000000000000000,
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
  0b0000000000000000
};

PROGMEM const uint16_t fan1_icon[] = {
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

PROGMEM const uint16_t fan2_icon[] = {
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

PROGMEM const uint16_t feedrate_icon[] = {
  0b0000000000000000,
  0b0111111000000000,
  0b0100000000000000,
  0b0100000000000000,
  0b0100000000000000,
  0b0111111011111000,
  0b0100000010000100,
  0b0100000010000100,
  0b0100000010000100,
  0b0100000011111000,
  0b0000000010001000,
  0b0000000010000100,
  0b0000000010000100,
  0b0000000010000010,
  0b0000000000000000,
  0b0000000000000000
};

static void lcd_implementation_status_screen();
static void lcd_in_status(bool inStatus);

#endif // ULTRALCD_ST7920_LITE_STATUS_H