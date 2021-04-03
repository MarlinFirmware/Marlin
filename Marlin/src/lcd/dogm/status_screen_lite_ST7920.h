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
 */
#pragma once

#include "../../HAL/shared/HAL_ST7920.h"

#include "../../core/types.h"
#include "../../core/macros.h"
#include "../../libs/duration_t.h"

class ST7920_Lite_Status_Screen {
  private:
    static struct st7920_state_t {
      uint8_t synced   : 1; // Whether a sync has been sent
      uint8_t cmd      : 1; // Whether the sync was cmd or data
      uint8_t extended : 1;
      uint8_t graphics : 1;
      uint8_t sa       : 1;
    } current_bits;

    static void cs()                        { ST7920_cs(); current_bits.synced = false; }
    static void ncs()                       { ST7920_cs(); current_bits.synced = false; }
    static void sync_cmd()                  { ST7920_set_cmd(); }
    static void sync_dat()                  { ST7920_set_dat(); }
    static void write_byte(const uint8_t w) { ST7920_write_byte(w); }

    FORCE_INLINE static void write_word(const uint16_t w) {
      write_byte((w >> 8) & 0xFF);
      write_byte((w >> 0) & 0xFF);
    }

    static void cmd(const uint8_t cmd);
    static void begin_data();

    static void write_str(const char *str);
    static void write_str(const char *str, const uint8_t len);
    static void write_str_P(PGM_P const str);
    static void write_number(const int16_t value, const uint8_t digits=3);

    static void _extended_function_set(const bool extended, const bool graphics);
    static void _scroll_or_addr_select(const bool sa);
    static void reset_state_from_unknown();

    static void home();
    static void display_status(const bool display_on, const bool cursor_on, const bool blink_on);
    static void extended_function_set(const bool extended);
    static void graphics(const bool graphics);
    static void entry_mode_select(const bool ac_increase, const bool shift);
    static void scroll_or_addr_select(const bool sa);
    static void set_ddram_address(const uint8_t addr);
    static void set_cgram_address(const uint8_t addr);
    static void set_gdram_address(const uint8_t x, const uint8_t y);

    static void clear();
    static void clear_ddram();
    static void clear_gdram();

    static void load_cgram_icon(const uint16_t addr, const void *data);
    static void draw_gdram_icon(uint8_t x, uint8_t y, const void *data);

    static uint8_t string_checksum(const char *str);

  protected:
    static void draw_degree_symbol(uint8_t x, uint8_t y, const bool draw);
    static void draw_static_elements();
    static void draw_progress_bar(const uint8_t value);
    static void draw_fan_icon(const bool whichIcon);
    static void draw_heat_icon(const bool whichIcon, const bool heating);
    static void draw_temps(uint8_t line, const int16_t temp, const int16_t target, bool showTarget, bool targetStateChange);
    static void draw_extruder_1_temp(const int16_t temp, const int16_t target, bool forceUpdate=false);
    static void draw_extruder_2_temp(const int16_t temp, const int16_t target, bool forceUpdate=false);
    static void draw_bed_temp(const int16_t temp, const int16_t target, bool forceUpdate=false);
    static void draw_fan_speed(const uint8_t value);
    static void draw_print_time(const duration_t &elapsed, char suffix=' ');
    static void draw_feedrate_percentage(const uint16_t percentage);
    static void draw_status_message();
    static void draw_position(const xyze_pos_t &pos, bool position_known=true);

    static bool indicators_changed();
    static bool position_changed();
    static bool blink_changed();
    static bool status_changed();

    static void update_indicators(const bool forceUpdate);
    static void update_position(const bool forceUpdate, bool resetChecksum);
    static void update_status_or_position(bool forceUpdate);
    static void update_progress(const bool forceUpdate);

  public:
    static void update(const bool forceUpdate);
    static void on_entry();
    static void on_exit();
    static void clear_text_buffer();
};
