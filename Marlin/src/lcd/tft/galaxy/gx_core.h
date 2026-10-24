/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * NOVIN3DP Galaxy UI - core drawing, widgets and touch actions
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */
#pragma once

#include "../../../inc/MarlinConfig.h"

#if ENABLED(NOVIN3DP_GALAXY_UI)

#include <stdint.h>
#include "gx_icons.h"

//
// Palette (RGB565) - see novin_ui_design/novin3dp_galaxy_ui.html
//
#define GX_C_BG       0x0043  // #040918
#define GX_C_ACC      0x36BF  // #35D6FF
#define GX_C_TEXT     0xE7BF  // #E2F6FF
#define GX_C_DIM      0x7D58  // #7FA8C6
#define GX_C_LINE     0x1A6F  // #1C4F7A
#define GX_C_PANEL    0x094A  // #0E2850 (drawn at 50%)
#define GX_C_POP      0x00A6  // #071430
#define GX_C_HOT      0xFCE8  // #FF9F43
#define GX_C_BAD      0xFAAE  // #FF5470
#define GX_C_OK       0x4711  // #47E08F

// Heater icons turn orange at or above this temperature (hot to the touch)
#ifndef GX_HOT_TEMP
  #define GX_HOT_TEMP   50
#endif
inline uint16_t gx_heat_color(const int16_t cur) { return cur >= GX_HOT_TEMP ? GX_C_HOT : GX_C_ACC; }

// Panel flags
#define GXP_BLEND     0x01    // Fill with 50% panel color over the galaxy
#define GXP_SOLID     0x02    // Fill with a solid color
#define GXP_BORDER    0x04    // 1px rounded border

// Touch action codes: high byte = command, low byte = argument
#define GXA(CMD, ARG) uint16_t((uint16_t(CMD) << 8) | uint8_t(ARG))
#define GX_REPEAT     0x8000  // Auto-repeat while held

#define GX_NO_ICON    GXI_COUNT

enum GxCmd : uint8_t {
  GA_NONE = 0,
  GA_BACK,    // 1 - previous screen
  GA_TEMPS,   // 2 - temperature screen
  GA_GO,      // 3 - open screen (arg = GxScreen)
  GA_EDIT,    // 4 - edit value (arg = GxValue)
  GA_CMD,     // 5 - run command (arg = GxCommand)
  GA_KEY,     // 6 - keypad key
  GA_FILE,    // 7 - file card slot
  GA_CELL,    // 8 - mesh cell
  GA_PAGE,    // 9 - file page (0 = up, 1 = down)
  GA_ADJ,     // 10 - heater/fan adjust: arg = (target << 4) | op
  GA_TXT,     // 11 - G-code keyboard key
  GA_STEP     // 12 - TMC step mode: arg = (driver << 1) | stealthChop
};

enum GxFont : uint8_t { GXF_S, GXF_L };            // NotoSans 14 (full) / NotoSans 19 (ASCII)
enum GxBtnStyle : uint8_t { GXB_NORMAL, GXB_PRIMARY, GXB_DANGER, GXB_SELECTED };
enum GxRowTail : uint8_t { GXT_NONE, GXT_EDIT, GXT_CHEVRON, GXT_ON, GXT_OFF };

// Layout of the 480x320 screen
#define GX_W          480
#define GX_H          320
#define GX_HEADER_H    52
#define GX_FOOTER_Y   276
#define GX_CONTENT_Y   58
#define GX_CONTENT_B  268   // Bottom of the content area

//
// Low level
//
void gx_init();
void gx_fill_galaxy_row(uint16_t *dst, uint16_t x0, uint16_t y, uint16_t count);
uint16_t gx_galaxy_avg(const uint16_t y);
uint16_t gx_blend(const uint16_t a, const uint16_t b, const uint8_t alpha);   // alpha = weight of b
inline uint16_t gx_half(const uint16_t a, const uint16_t b) { return ((a & 0xF7DE) >> 1) + ((b & 0xF7DE) >> 1); }
const uint8_t* gx_icon_data(const GxIcon icon, const uint8_t size);   // 4-bit alpha, run-length coded
void gx_icon_render(uint16_t *buffer, const uint16_t cw, const uint16_t startLine, const uint16_t endLine,
                    const int16_t x, const int16_t y, const uint8_t *data, const uint8_t size, const uint16_t color);

//
// Canvas helpers (a canvas is a screen rectangle; coordinates inside are relative)
//
void gx_canvas(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h);
void gx_panel(const int16_t x, const int16_t y, const uint16_t w, const uint16_t h, const uint16_t border=GX_C_LINE, const uint8_t flags=GXP_BLEND|GXP_BORDER, const uint16_t fill=GX_C_PANEL, const uint8_t r=6);
void gx_icon(const int16_t x, const int16_t y, const GxIcon icon, const uint8_t size, const uint16_t color=GX_C_ACC);
uint16_t gx_text(const int16_t x, const int16_t y, const char *str, const uint16_t color=GX_C_TEXT, const GxFont font=GXF_S, const uint16_t maxw=0);
uint16_t gx_text_width(const char *str, const GxFont font=GXF_S);
uint16_t gx_font_height(const GxFont font);
int16_t gx_vcenter(const uint16_t h, const GxFont font);
void gx_text_center(const int16_t x, const int16_t y, const uint16_t w, const char *str, const uint16_t color=GX_C_TEXT, const GxFont font=GXF_S);
void gx_text_right(const int16_t xr, const int16_t y, const char *str, const uint16_t color=GX_C_TEXT, const GxFont font=GXF_S);
void gx_bar(const int16_t x, const int16_t y, const uint16_t w, const uint16_t h, const uint16_t color);

//
// Screen state
//
extern bool gx_full_redraw;          // Static parts + touch controls must be (re)drawn
inline bool gx_full() { return gx_full_redraw; }
void gx_invalidate();                // Redraw the whole current screen (no clear)
void gx_clear_screen();              // Galaxy everywhere, reset queue + touch
void gx_set_refresh(const uint16_t ms);
void gx_idle();                      // Periodic refresh, called from tft_idle
void gx_touch_add(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint16_t action);
void gx_touch(const uint16_t action);  // Dispatched from Touch::touch()
void gx_slider(const uint8_t id, const uint16_t permille);  // Slider dragged to 0..1000
void gx_touch_beep();
bool gx_beep_enabled();

//
// Widgets (each one is its own canvas)
//
void gx_header(const char *title, const bool back=true);
void gx_header_info(const char *info, const uint16_t color=GX_C_DIM, const GxFont font=GXF_S);
void gx_footer_temps();
void gx_alert(const char *msg);         // Warning in the footer for a few seconds (static string)
void gx_footer_msg(const char *msg, const GxIcon icon=GXI_INFO, const uint16_t color=GX_C_DIM);
void gx_footer_status();
void gx_tile(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const GxIcon icon, const uint8_t isz, const char *label, const uint16_t action, const uint16_t color=GX_C_ACC, const char *sub=nullptr, const bool on=false);
void gx_button(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const GxIcon icon, const char *label, const uint16_t action, const GxBtnStyle style=GXB_NORMAL, const GxFont font=GXF_S);
void gx_row(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const GxIcon icon, const char *label, const char *value, const char *unit, const uint16_t action, const GxRowTail tail=GXT_NONE, const char letter=0, const uint16_t icolor=GX_C_ACC);
void gx_progress(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint8_t percent, const uint16_t color=GX_C_ACC);
void gx_dialog(const char *title, const char *line1, const char *line2, const GxIcon icon, const uint16_t color, const char *yes, const uint16_t yesAction, const char *no, const uint16_t noAction, const bool danger=false);

//
// Value editor (numeric keypad)
//
typedef float (*gx_getter_t)();
typedef void (*gx_setter_t)(const float v);
void gx_edit_value(const char *label, const char *unit, const float minv, const float maxv, const uint8_t decimals, gx_getter_t getter, gx_setter_t setter);

//
// Screens (gx_screens.cpp)
//
void gx_status_screen();             // Home / printing / finished
void gx_go(void (*screen)());        // Push current screen and open another
void gx_back();

// Manual move feedrates (mm/min), editable in More > Move speed and saved with M500
extern xyze_feedrate_t gx_manual_feedrate_mm_m;
void gx_reset_manual_feedrate();

// Formatting helpers
const char* gx_ftoa(const float v, const uint8_t decimals);
const char* gx_itoa(const int32_t v);
const char* gx_time(const uint32_t seconds);

#endif // NOVIN3DP_GALAXY_UI
