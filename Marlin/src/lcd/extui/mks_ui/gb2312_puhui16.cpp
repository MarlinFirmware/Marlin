/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "pic_manager.h"
#include <lvgl.h>

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_FLASH_FONT

typedef struct {
  uint16_t min;
  uint16_t max;
  uint8_t bpp;
  uint8_t reserved[3];
} x_header_t;

typedef struct {
  uint32_t pos;
} x_table_t;

typedef struct {
  uint8_t adv_w;
  uint8_t box_w;
} glyph_dsc_t;

static x_header_t __g_xbf_hd = { .min = 0, .max = 0, .bpp = 0 };
static uint8_t __g_font_buf[63];

static uint8_t *__user_font_getdata(int offset, int size) {
  get_spi_flash_data((char *)__g_font_buf, offset, size);
  return __g_font_buf;
}

static const uint8_t * __user_font_get_bitmap(const lv_font_t * font, uint32_t unicode_letter) {
  if (__g_xbf_hd.max == 0) {
    uint8_t *p = __user_font_getdata(0, sizeof(x_header_t));
    memcpy(&__g_xbf_hd, p, sizeof(x_header_t));
  }
  if (unicode_letter > __g_xbf_hd.max || unicode_letter < __g_xbf_hd.min)
    return nullptr;
  uint32_t unicode_offset = sizeof(x_header_t) + (unicode_letter - __g_xbf_hd.min) * 4;
  uint32_t *p_pos = (uint32_t *)__user_font_getdata(unicode_offset, 4);
  if (p_pos[0] != 0) {
    uint32_t pos = p_pos[0];
    __user_font_getdata(pos, 2);
    return __user_font_getdata(pos + 2, sizeof(__g_font_buf));
  }
  return nullptr;
}

static bool __user_font_get_glyph_dsc(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next) {
  if (__g_xbf_hd.max == 0) {
    uint8_t *p = __user_font_getdata(0, sizeof(x_header_t));
    memcpy(&__g_xbf_hd, p, sizeof(x_header_t));
  }
  if (unicode_letter > __g_xbf_hd.max || unicode_letter < __g_xbf_hd.min)
    return false;
  uint32_t unicode_offset = sizeof(x_header_t) + (unicode_letter - __g_xbf_hd.min) * 4;
  uint32_t *p_pos = (uint32_t *)__user_font_getdata(unicode_offset, 4);
  if (p_pos[0] != 0) {
    glyph_dsc_t * gdsc = (glyph_dsc_t*)__user_font_getdata(p_pos[0], 2);
    dsc_out->adv_w = gdsc->adv_w;
    dsc_out->box_h = font->line_height;
    dsc_out->box_w = gdsc->box_w;
    dsc_out->ofs_x = 0;
    dsc_out->ofs_y = 0;
    dsc_out->bpp = __g_xbf_hd.bpp;
    return true;
  }
  return false;
}

lv_font_t gb2312_puhui32;
void init_gb2312_font() {
  gb2312_puhui32.get_glyph_bitmap = __user_font_get_bitmap;
  gb2312_puhui32.get_glyph_dsc = __user_font_get_glyph_dsc;
  gb2312_puhui32.line_height = 21;
  gb2312_puhui32.base_line = 0;
}

#endif // HAS_SPI_FLASH_FONT
#endif // HAS_TFT_LVGL_UI
