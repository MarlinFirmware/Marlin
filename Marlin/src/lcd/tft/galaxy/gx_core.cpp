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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(NOVIN3DP_GALAXY_UI)

#include "gx_core.h"
#include "gx_home_icons.h"
#include "../ui_common.h"
#include "../../marlinui.h"
#include "../../menu/menu.h"
#include "../../../module/temperature.h"
#include "../../../libs/numtostr.h"
#if HAS_BEEPER
  #include "../../../libs/buzzer.h"
#endif

extern const uint8_t NotoSans_Medium_ASCII_19[];

#define GX_RGB(R,G,B) uint16_t((((R) & 0xF8) << 8) | (((G) & 0xFC) << 3) | ((B) >> 3))

//
// Galaxy background: vertical gradient + two separable soft nebulae, no stars.
// RAM: 640 B (rows) + 1600 B (nebula tables). No flash image.
//
static uint16_t gx_row_color[GX_H];
static uint8_t gx_nx[2][GX_W], gx_ny[2][GX_H];
static const uint16_t gx_neb_color[2] = { GX_RGB(34, 78, 170), GX_RGB(52, 40, 140) };

bool gx_full_redraw = true;

xyze_feedrate_t gx_manual_feedrate_mm_m = MANUAL_FEEDRATE;
void gx_reset_manual_feedrate() { gx_manual_feedrate_mm_m = MANUAL_FEEDRATE; }
static uint16_t gx_refresh_ms = 0;
static millis_t gx_next_refresh = 0;

static uint8_t gauss8(const int16_t v, const int16_t m, const float s, const float a) {
  const float d = float(v - m);
  return uint8_t(a * expf(-(d * d) / (2.0f * s * s)) + 0.5f);
}

void gx_init() {
  constexpr uint8_t top[3] = { 4, 9, 24 }, bot[3] = { 9, 22, 52 };
  for (uint16_t y = 0; y < GX_H; y++) {
    uint8_t c[3];
    for (uint8_t k = 0; k < 3; k++) c[k] = top[k] + ((int16_t(bot[k]) - top[k]) * int16_t(y) + (GX_H - 1) / 2) / (GX_H - 1);
    gx_row_color[y] = GX_RGB(c[0], c[1], c[2]);
    gx_ny[0][y] = gauss8(y, 85, 75, 150);
    gx_ny[1][y] = gauss8(y, 265, 60, 110);
  }
  for (uint16_t x = 0; x < GX_W; x++) {
    gx_nx[0][x] = gauss8(x, 345, 105, 255);
    gx_nx[1][x] = gauss8(x, 80, 90, 255);
  }
}

uint16_t gx_blend(const uint16_t a, const uint16_t b, const uint8_t alpha) {
  const int16_t ar = a >> 11, ag = (a >> 5) & 0x3F, ab = a & 0x1F,
                br = b >> 11, bg = (b >> 5) & 0x3F, bb = b & 0x1F;
  return uint16_t(((ar + (((br - ar) * alpha) >> 8)) << 11) | ((ag + (((bg - ag) * alpha) >> 8)) << 5) | (ab + (((bb - ab) * alpha) >> 8)));
}

void gx_fill_galaxy_row(uint16_t *dst, uint16_t x0, uint16_t y, uint16_t count) {
  if (y >= GX_H) y = GX_H - 1;
  const uint16_t base = gx_row_color[y];
  const uint8_t ny0 = gx_ny[0][y], ny1 = gx_ny[1][y];
  for (uint16_t i = 0; i < count; i++) {
    const uint16_t x = _MIN(uint16_t(x0 + i), uint16_t(GX_W - 1));
    uint16_t c = base;
    if (ny0) { const uint8_t n = (uint16_t(gx_nx[0][x]) * ny0) >> 8; if (n) c = gx_blend(c, gx_neb_color[0], n); }
    if (ny1) { const uint8_t n = (uint16_t(gx_nx[1][x]) * ny1) >> 8; if (n) c = gx_blend(c, gx_neb_color[1], n); }
    dst[i] = ENDIAN_COLOR(c);
  }
}

uint16_t gx_galaxy_avg(const uint16_t y) { return gx_row_color[_MIN(y, uint16_t(GX_H - 1))]; }

const uint8_t* gx_icon_data(const GxIcon icon, const uint8_t size) {
  if (icon >= GXI_COUNT) return nullptr;
  switch (size) {
    case 20: return gx_icons_20[icon];
    case 24: return gx_icons_24[icon];
    case 32: return gx_icons_32[icon];
    case 48: return gx_icons_48[icon];
    case 64: return gx_icons_64[icon];
    case 80: return gx_icons_80[icon];
    default: return nullptr;
  }
}

//
// Fonts
//
static const uint8_t* gx_font_ptr(const GxFont f) { return f == GXF_L ? NotoSans_Medium_ASCII_19 : TFT_String::base_font; }
static void gx_use(const GxFont f) { TFT_String::use_font(gx_font_ptr(f)); }
static void gx_use_base() { TFT_String::use_font(TFT_String::base_font); }

//
// Canvas helpers
//
void gx_canvas(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h) {
  tft.canvas(x, y, w, h);
  tft.set_galaxy_background();
}

void gx_panel(const int16_t x, const int16_t y, const uint16_t w, const uint16_t h, const uint16_t border, const uint8_t flags, const uint16_t fill, const uint8_t r) {
  tft.add_panel(x, y, w, h, fill, border, r, flags);
}

// Icons are run-length coded 4-bit alpha masks, tinted with a light-to-color vertical gradient
void gx_icon_render(uint16_t *buffer, const uint16_t cw, const uint16_t startLine, const uint16_t endLine,
                    const int16_t x, const int16_t y, const uint8_t *data, const uint8_t size, const uint16_t color) {
  // Run-length decoder: 0x00-0x7F = n+1 transparent pixels, 0x80|a<<3|(n-1) = n pixels of alpha a
  const uint8_t *src = data;
  uint8_t run = 0, alpha = 0;
  auto next = [&]() -> uint8_t {
    if (!run) { const uint8_t b = *src++; if (b & 0x80) { alpha = (b >> 3) & 0x0F; run = (b & 7) + 1; } else { alpha = 0; run = b + 1; } }
    run--; return alpha;
  };
  for (int16_t r = 0; r < size; r++) {
    const int16_t line = y + r;
    if (line >= int16_t(endLine)) break;
    if (line < int16_t(startLine)) { for (uint8_t c = 0; c < size; c++) next(); continue; }
    const uint16_t col = gx_blend(color, 0xFFFF, uint8_t(uint16_t(size - 1 - r) * 90 / size));   // Lighter at the top
    uint16_t *row = buffer + (line - startLine) * cw;
    for (int16_t c = 0; c < size; c++) {
      const uint8_t a = next();
      if (!a || !WITHIN(x + c, 0, cw - 1)) continue;
      row[x + c] = a == 15 ? ENDIAN_COLOR(col) : ENDIAN_COLOR(gx_blend(ENDIAN_COLOR(row[x + c]), col, a * 17));
    }
  }
}

struct gx_icon_payload_t { const uint8_t *data; uint8_t size; uint16_t color; };

static void icon_render(uint16_t *buffer, uint16_t cw, uint16_t startLine, uint16_t endLine, int16_t x, int16_t y, const uint8_t *payload) {
  gx_icon_payload_t p; memcpy(&p, payload, sizeof(p));
  gx_icon_render(buffer, cw, startLine, endLine, x, y, p.data, p.size, p.color);
}

void gx_icon(const int16_t x, const int16_t y, const GxIcon icon, const uint8_t size, const uint16_t color) {
  const uint8_t *data = gx_icon_data(icon, size);
  if (!data) return;
  const gx_icon_payload_t p = { data, size, color };
  tft.add_custom(x, y, (void *)icon_render, &p, sizeof(p));
}

// The large font is ASCII-only: a degree sign ("\xC2\xB0") is drawn as a small ring
#define GX_DEG_W 11
static const char* gx_deg_split(const char *str, const GxFont font) {
  return font == GXF_L ? strstr(str, "\xC2\xB0") : nullptr;
}

uint16_t gx_text(const int16_t x, const int16_t y, const char *str, const uint16_t color, const GxFont font, const uint16_t maxw) {
  if (!str || !*str) return 0;
  if (const char * const deg = gx_deg_split(str, font)) {
    char head[24]; strlcpy(head, str, _MIN(sizeof(head), size_t(deg - str + 1)));
    uint16_t w = gx_text(x, y, head, color, font, maxw);
    gx_panel(x + w + 2, y + 5, 7, 7, color, GXP_BORDER, color, 3);
    gx_panel(x + w + 3, y + 6, 5, 5, color, GXP_BORDER, color, 2);
    w += GX_DEG_W;
    if (deg[2]) w += gx_text(x + w, y, deg + 2, color, font);
    return w;
  }
  gx_use(font);
  tft_string.set(str);
  const uint16_t w = tft_string.width();
  tft.add_text(x, y, color, tft_string, maxw);
  gx_use_base();
  return w;
}

uint16_t gx_text_width(const char *str, const GxFont font) {
  if (!str || !*str) return 0;
  if (const char * const deg = gx_deg_split(str, font)) {
    char head[24]; strlcpy(head, str, _MIN(sizeof(head), size_t(deg - str + 1)));
    return gx_text_width(head, font) + GX_DEG_W + gx_text_width(deg + 2, font);
  }
  gx_use(font);
  tft_string.set(str);
  const uint16_t w = tft_string.width();
  gx_use_base();
  return w;
}

uint16_t gx_font_height(const GxFont font) {
  gx_use(font);
  const uint16_t h = tft_string.font_height();
  gx_use_base();
  return h;
}

int16_t gx_vcenter(const uint16_t h, const GxFont font) {
  gx_use(font);
  const int16_t v = tft_string.vcenter(h);
  gx_use_base();
  return v;
}

void gx_text_center(const int16_t x, const int16_t y, const uint16_t w, const char *str, const uint16_t color, const GxFont font) {
  const uint16_t tw = gx_text_width(str, font);
  gx_text(x + (tw < w ? (w - tw) / 2 : 0), y, str, color, font, w);
}

void gx_text_right(const int16_t xr, const int16_t y, const char *str, const uint16_t color, const GxFont font) {
  const uint16_t tw = gx_text_width(str, font);
  gx_text(xr - tw, y, str, color, font);
}

// 8-bit palette image (full color, per-entry alpha blended with what is underneath)
static void pal_render(uint16_t *buffer, uint16_t cw, uint16_t startLine, uint16_t endLine, int16_t x, int16_t y, const uint8_t *payload) {
  const gx_pal_image_t *img; memcpy(&img, payload, sizeof(img));
  for (int16_t r = 0; r < img->height; r++) {
    const int16_t line = y + r;
    if (line < int16_t(startLine) || line >= int16_t(endLine)) continue;
    uint16_t *row = buffer + (line - startLine) * cw;
    const uint8_t *src = img->index + r * img->width;
    for (int16_t c = 0; c < img->width; c++) {
      const uint8_t i = src[c], a = img->alpha[i];
      if (!a || !WITHIN(x + c, 0, cw - 1)) continue;
      row[x + c] = a >= 250 ? ENDIAN_COLOR(img->colors[i]) : ENDIAN_COLOR(gx_blend(ENDIAN_COLOR(row[x + c]), img->colors[i], a));
    }
  }
}

void gx_pal_image(const int16_t x, const int16_t y, const gx_pal_image_t &img) {
  const gx_pal_image_t *p = &img;
  tft.add_custom(x, y, (void *)pal_render, &p, sizeof(p));
}

void gx_bar(const int16_t x, const int16_t y, const uint16_t w, const uint16_t h, const uint16_t color) {
  if (w && h) tft.add_bar(x, y, w, h, color);
}

//
// Screen state
//
void gx_invalidate() {
  gx_full_redraw = true;
  ui.refresh(LCDVIEW_REDRAW_NOW);
}

void gx_clear_screen() {
  #if ENABLED(TOUCH_SCREEN)
    touch.reset();
    draw_menu_navigation = false;
  #endif
  // Never abort a running SPI DMA (the touch chip shares the bus): wait for it,
  // then let the last bytes leave the shift register.
  while (tft.is_busy()) { /* wait */ }
  delayMicroseconds(20);
  tft.queue.reset();
  // Paint the galaxy in separate bands; each band re-sends the display window
  for (uint16_t y = 0; y < GX_H; y += 40) gx_canvas(0, y, GX_W, 40);
  tft.queue.sync();         // Draw the background now, before anything else can use the bus
  gx_full_redraw = true;
  ui.lcd_clicked = false;   // Drop any stale click so Marlin's status screen never opens its own main menu
  gx_refresh_ms = 0;
}

void gx_set_refresh(const uint16_t ms) {
  if (gx_refresh_ms != ms) gx_next_refresh = millis() + ms;
  gx_refresh_ms = ms;
}

void gx_idle() {
  if (gx_refresh_ms && ELAPSED(millis(), gx_next_refresh)) {
    gx_next_refresh = millis() + gx_refresh_ms;
    if (!ui.should_draw()) ui.refresh(LCDVIEW_REDRAW_NOW);
  }
}

void gx_touch_add(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint16_t action) {
  #if ENABLED(TOUCH_SCREEN)
    if (gx_full_redraw && action) touch.add_control(GX_ACTION, x, y, w, h, action);
  #endif
}

void gx_touch_beep() {
  #if HAS_CHIRP
    if (ui.sound_on && gx_beep_enabled()) {
      ui.chirp();   // Display drawing pauses while it sounds (see MarlinUI::tft_idle)
    }
  #endif
}

//
// Formatting
//
static char gx_fmt_buf[4][16];
static uint8_t gx_fmt_idx = 0;
static char* gx_fmt_next() { gx_fmt_idx = (gx_fmt_idx + 1) & 3; return gx_fmt_buf[gx_fmt_idx]; }

const char* gx_ftoa(const float v, const uint8_t decimals) {
  char *b = gx_fmt_next();
  int32_t scale = 1;
  for (uint8_t i = 0; i < decimals; i++) scale *= 10;
  const bool neg = v < 0;
  const int32_t n = int32_t((neg ? -v : v) * scale + 0.5f);
  const int32_t ip = n / scale, fp = n % scale;
  char tmp[16]; uint8_t t = 0;
  if (decimals) {
    int32_t f = fp;
    for (uint8_t i = 0; i < decimals; i++) { tmp[t++] = '0' + f % 10; f /= 10; }
    tmp[t++] = '.';
  }
  int32_t q = ip;
  do { tmp[t++] = '0' + q % 10; q /= 10; } while (q && t < 14);
  uint8_t o = 0;
  if (neg && n) b[o++] = '-';
  while (t) b[o++] = tmp[--t];
  b[o] = '\0';
  return b;
}

const char* gx_itoa(const int32_t v) { return gx_ftoa(float(v), 0); }

const char* gx_time(const uint32_t s) {
  char *b = gx_fmt_next();
  const uint32_t h = s / 3600, m = (s / 60) % 60, sec = s % 60;
  if (h > 99) { sprintf_P(b, PSTR("%lud %luh"), (unsigned long)(h / 24), (unsigned long)(h % 24)); return b; }
  sprintf_P(b, PSTR("%02u:%02u:%02u"), unsigned(h), unsigned(m), unsigned(sec));
  return b;
}

//
// Widgets
//
void gx_header(const char *title, const bool back) {
  gx_canvas(0, 0, GX_W, GX_HEADER_H);
  const int16_t tx = back ? 66 : 20;
  if (back) {
    gx_panel(10, 8, 44, 36);
    gx_icon(20, 14, GXI_BACK, 24);
  }
  gx_text(tx, 8 + gx_vcenter(36, GXF_L), title, GX_C_ACC, GXF_L, 236 - tx);
  gx_bar(tx, 50, GX_W - 10 - tx, 1, GX_C_ACC);
  if (back) gx_touch_add(0, 0, 70, GX_HEADER_H, GXA(GA_BACK, 0));
}

void gx_header_info(const char *info, const uint16_t color, const GxFont font) {
  gx_canvas(240, 8, 230, 36);
  if (info) gx_text_right(230, gx_vcenter(36, font), info, color, font);
}

static void gx_footer_frame() {
  gx_canvas(0, GX_FOOTER_Y, GX_W, GX_H - GX_FOOTER_Y);
  gx_bar(0, 0, GX_W, 1, GX_C_LINE);
}

// Footer columns: nozzle, bed, (chamber), fan
#define GX_FOOTER_COLS (3 + ENABLED(HAS_TEMP_CHAMBER))

static void gx_footer_item(const uint8_t col, const GxIcon icon, const uint16_t icolor, const char *txt) {
  const uint16_t cw = GX_W / (GX_FOOTER_COLS), x0 = col * cw;
  const uint16_t tw = gx_text_width(txt), total = 24 + 8 + tw, sx = x0 + (cw > total ? (cw - total) / 2 : 0);
  gx_icon(sx, 10, icon, 24, icolor);
  gx_text(sx + 32, 2 + gx_vcenter(42, GXF_S), txt, GX_C_TEXT);
  if (col) gx_bar(x0, 9, 1, 26, GX_C_LINE);
}

static const char* gx_temp_pair(const celsius_t cur, const celsius_t tgt) {
  static char b[24];
  sprintf_P(b, PSTR("%i\xC2\xB0 / %i\xC2\xB0"), int(cur), int(tgt));
  return b;
}

static const char *gx_alert_msg = nullptr;
static millis_t gx_alert_until = 0;
void gx_alert(const char *msg) { gx_alert_msg = msg; gx_alert_until = millis() + 4000UL; ui.set_status(msg); gx_invalidate(); }

void gx_footer_temps() {
  if (gx_alert_msg && PENDING(millis(), gx_alert_until)) { gx_footer_msg(gx_alert_msg, GXI_WARNING, GX_C_HOT); return; }
  gx_alert_msg = nullptr;
  gx_footer_frame();
  #if HAS_HOTEND
    const celsius_t nc = thermalManager.wholeDegHotend(0), nt = thermalManager.degTargetHotend(0);
    gx_footer_item(0, GXI_NOZZLE, gx_heat_color(nc), gx_temp_pair(nc, nt));
  #endif
  #if HAS_HEATED_BED
    const celsius_t bc = thermalManager.wholeDegBed(), bt = thermalManager.degTargetBed();
    gx_footer_item(1, GXI_BED, gx_heat_color(bc), gx_temp_pair(bc, bt));
  #endif
  #if HAS_TEMP_CHAMBER
    const celsius_t cc = thermalManager.wholeDegChamber();
    #if HAS_HEATED_CHAMBER
      const char * const ct = gx_temp_pair(cc, thermalManager.degTargetChamber());
    #else
      char ct[12]; sprintf_P(ct, PSTR("%i\xC2\xB0"), int(cc));
    #endif
    gx_footer_item(2, GXI_THERMO, gx_heat_color(cc), ct);
  #endif
  #if HAS_FAN
    char f[8]; sprintf_P(f, PSTR("%u%%"), unsigned(fans[0].speed_pct()));
    gx_footer_item(GX_FOOTER_COLS - 1, GXI_FAN, GX_C_ACC, f);
  #endif
  // Each item opens its own screen: nozzle, bed, (chamber: temperature screen), fan
  constexpr uint16_t cw = GX_W / (GX_FOOTER_COLS);
  gx_touch_add(0,  GX_FOOTER_Y, cw, GX_H - GX_FOOTER_Y, GXA(GA_GO, 27));
  gx_touch_add(cw, GX_FOOTER_Y, cw, GX_H - GX_FOOTER_Y, GXA(GA_GO, 28));
  #if HAS_TEMP_CHAMBER
    gx_touch_add(2 * cw, GX_FOOTER_Y, cw, GX_H - GX_FOOTER_Y, GXA(GA_TEMPS, 0));
  #endif
  gx_touch_add((GX_FOOTER_COLS - 1) * cw, GX_FOOTER_Y, cw, GX_H - GX_FOOTER_Y, GXA(GA_GO, 29));
}

void gx_footer_msg(const char *msg, const GxIcon icon, const uint16_t color) {
  gx_footer_frame();
  const uint16_t tw = gx_text_width(msg), total = 24 + 10 + tw, sx = total < GX_W ? (GX_W - total) / 2 : 0;
  gx_icon(sx, 10, icon, 24, color);
  gx_text(sx + 34, 2 + gx_vcenter(42, GXF_S), msg, color, GXF_S, GX_W - sx - 34);
}

void gx_footer_status() {
  if (ui.has_status()) gx_footer_msg(ui.status_message, GXI_INFO, GX_C_DIM);
  else gx_footer_temps();
}

void gx_tile(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const GxIcon icon, const uint8_t isz, const char *label, const uint16_t action, const uint16_t color, const char *sub, const bool on) {
  gx_canvas(x, y, w, h);
  gx_panel(0, 0, w, h, on ? GX_C_ACC : GX_C_LINE);
  const uint16_t fh = gx_font_height(GXF_S) - 2;

  // Too wide for one line? Break at the space that balances the two lines.
  char l1[32], l2[32] = "";
  strlcpy(l1, label ? label : "", sizeof(l1));
  if (gx_text_width(l1) > w - 8) {
    int8_t best = -1; uint16_t bestw = 0xFFFF;
    for (uint8_t i = 1; l1[i]; i++) if (l1[i] == ' ') {
      char a[32]; strlcpy(a, l1, i + 1);
      const uint16_t m = _MAX(gx_text_width(a), gx_text_width(l1 + i + 1));
      if (m < bestw) { bestw = m; best = i; }
    }
    if (best > 0) { strlcpy(l2, l1 + best + 1, sizeof(l2)); l1[best] = '\0'; }
  }
  const uint8_t lines = (l2[0] ? 2 : 1) + (sub ? 1 : 0);
  const uint8_t gap = h - isz > fh * lines + 10 ? 6 : 2;
  const uint16_t content = isz + gap + fh * lines;
  const int16_t top = h > content ? (h - content) / 2 : 0;
  gx_icon((w - isz) / 2, top, icon, isz, color);
  int16_t ty = top + isz + gap;
  gx_text_center(4, ty, w - 8, l1, GX_C_TEXT); ty += fh;
  if (l2[0]) { gx_text_center(4, ty, w - 8, l2, GX_C_TEXT); ty += fh; }
  if (sub) gx_text_center(4, ty, w - 8, sub, GX_C_ACC);
  gx_touch_add(x, y, w, h, action);
}

void gx_button(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const GxIcon icon, const char *label, const uint16_t action, const GxBtnStyle style, const GxFont font) {
  gx_canvas(x, y, w, h);
  uint16_t border = GX_C_LINE, fg = GX_C_TEXT, ic = GX_C_ACC;
  switch (style) {
    case GXB_PRIMARY:  border = GX_C_ACC; fg = GX_C_ACC; break;
    case GXB_DANGER:   border = GX_C_BAD; fg = GX_C_BAD; ic = GX_C_BAD; break;
    case GXB_SELECTED: border = GX_C_ACC; fg = GX_C_BG; ic = GX_C_BG; break;
    default: break;
  }
  if (style == GXB_SELECTED) gx_panel(0, 0, w, h, border, GXP_SOLID | GXP_BORDER, GX_C_ACC);
  else gx_panel(0, 0, w, h, border);
  const uint8_t isz = h >= 40 ? 24 : 20;
  const bool has_icon = icon < GXI_COUNT;
  const uint16_t tw = gx_text_width(label, font), total = (has_icon ? isz : 0) + (has_icon && label ? 8 : 0) + tw;
  int16_t sx = total < w ? (w - total) / 2 : 2;
  if (has_icon) { gx_icon(sx, (h - isz) / 2, icon, isz, ic); sx += isz + 8; }
  if (label) gx_text(sx, gx_vcenter(h, font), label, fg, font, w - sx - 2);
  gx_touch_add(x, y, w, h, action);
}

void gx_row(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const GxIcon icon, const char *label, const char *value, const char *unit, const uint16_t action, const GxRowTail tail, const char letter, const uint16_t icolor) {
  gx_canvas(x, y, w, h);
  gx_panel(0, 0, w, h);
  int16_t lx = 10;
  if (letter) {
    const char s[2] = { letter, 0 };
    gx_text(10, gx_vcenter(h, GXF_L), s, GX_C_ACC, GXF_L);
    lx = 40;
  }
  else if (icon < GXI_COUNT) {
    gx_icon(8, (h - 24) / 2, icon, 24, icolor);
    lx = 40;
  }
  // Tail
  int16_t right = w - 8;
  switch (tail) {
    case GXT_EDIT:
      gx_panel(w - 40, (h - 28) / 2, 34, 28);
      gx_icon(w - 33, (h - 20) / 2, GXI_EDIT, 20);
      right = w - 48;
      break;
    case GXT_CHEVRON:
      gx_icon(w - 26, (h - 20) / 2, GXI_CHEV_R, 20, GX_C_DIM);
      right = w - 32;
      break;
    case GXT_ON: case GXT_OFF: {
      const bool on = tail == GXT_ON;
      gx_panel(w - 54, (h - 24) / 2, 46, 24, on ? GX_C_ACC : GX_C_LINE, GXP_BORDER, 0, 11);
      gx_panel(on ? w - 32 : w - 50, (h - 16) / 2, 16, 16, on ? GX_C_ACC : GX_C_DIM, GXP_SOLID, on ? GX_C_ACC : GX_C_DIM, 7);
      right = w - 62;
    } break;
    default: break;
  }
  const uint16_t vy = gx_vcenter(h, GXF_S);
  uint16_t vw = 0;
  if (unit) { const uint16_t uw = gx_text_width(unit); gx_text(right - uw, vy, unit, GX_C_DIM); vw = uw + 3; }
  if (value) { const uint16_t tw = gx_text_width(value); gx_text(right - vw - tw, vy, value, GX_C_TEXT); vw += tw; }
  gx_text(lx, vy, label, GX_C_TEXT, GXF_S, _MAX(int16_t(10), int16_t(right - vw - lx - 8)));
  gx_touch_add(x, y, w, h, action);
}

void gx_progress(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const uint8_t percent, const uint16_t color) {
  gx_canvas(x, y, w, h);
  gx_panel(0, 0, w, h, GX_C_LINE, GXP_BLEND | GXP_BORDER, GX_C_PANEL, _MIN(uint8_t(6), uint8_t(h / 2)));
  const uint16_t fw = uint32_t(w - 4) * _MIN(percent, uint8_t(100)) / 100;
  if (fw >= 2) gx_panel(2, 2, fw, h - 4, color, GXP_SOLID, color, _MIN(uint8_t(4), uint8_t((h - 4) / 2)));
}

void gx_dialog(const char *title, const char *line1, const char *line2, const GxIcon icon, const uint16_t color, const char *yes, const uint16_t yesAction, const char *no, const uint16_t noAction, const bool danger) {
  constexpr uint16_t DX = 50, DY = 40, DW = 380, DH = 240;
  gx_canvas(DX, DY, DW, DH);
  gx_panel(0, 0, DW, DH, danger ? GX_C_BAD : GX_C_ACC, GXP_SOLID | GXP_BORDER, GX_C_POP, 8);
  gx_icon((DW - 48) / 2, 16, icon, 48, color);
  gx_text_center(10, 72, DW - 20, title, danger ? GX_C_BAD : GX_C_ACC, GXF_L);
  if (line1) gx_text_center(10, 112, DW - 20, line1, GX_C_DIM);
  if (line2) gx_text_center(10, 136, DW - 20, line2, GX_C_DIM);
  const uint16_t bw = no ? (DW - 50) / 2 : DW - 40, by = DY + DH - 62;
  if (no) gx_button(DX + 20, by, bw, 46, GXI_CLOSE, no, noAction);
  gx_button(no ? DX + 30 + bw : DX + 20, by, bw, 46, danger ? GXI_STOP : GXI_CHECK, yes, yesAction, danger ? GXB_DANGER : GXB_PRIMARY);
}

#endif // NOVIN3DP_GALAXY_UI
