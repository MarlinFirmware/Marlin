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

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr,*outL,*outV = 0;
static int currentWritePos = 0;
extern uint8_t public_buf[513];
extern "C" { extern char public_buf_m[100]; }

enum {
  ID_GCODE_RETURN = 1,
  ID_GCODE_COMMAND,
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_gcode();
  switch (obj->mks_obj_id) {
    case ID_GCODE_RETURN:
      lv_draw_more();
      return;
    case ID_GCODE_COMMAND:
      keyboard_value = GCodeCommand;
      lv_draw_keyboard();
      break;
  }
}

void lv_show_gcode_output(void * that, const char * txt) {
  // Ignore echo of command
  if (!memcmp(txt, "echo:", 5)) {
    public_buf[0] = 0; // Clear output buffer
    return;
   }

  // Avoid overflow if the answer is too large
  size_t len = strlen((const char*)public_buf), tlen = strlen(txt);
  if (len + tlen + 1 < sizeof(public_buf)) {
    memcpy(public_buf + len, txt, tlen);
    public_buf[len + tlen] = '\n';
  }
}

void lv_serial_capt_hook(void * userPointer, uint8_t c)
{
  if (c == '\n' || currentWritePos == sizeof(public_buf_m) - 1) { // End of line, probably end of command anyway
    public_buf_m[currentWritePos] = 0;
    lv_show_gcode_output(userPointer, public_buf_m);
    currentWritePos = 0;
  }
  else public_buf_m[currentWritePos++] = c;
}
void lv_eom_hook(void *)
{
  // Message is done, let's remove the hook now
  MYSERIAL1.setHook();
  // We are back from the keyboard, so let's redraw ourselves
  draw_return_ui();
}

void lv_draw_gcode(bool clear) {
  if (clear) {
    currentWritePos = 0;
    public_buf[0] = 0;
  }
  scr = lv_screen_create(GCODE_UI, more_menu.gcode);
  lv_screen_menu_item(scr, more_menu.entergcode, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_GCODE_COMMAND, 1);
  outL = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 2, "Result:");
  outV = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3, (const char*)public_buf);

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X + 10, PARA_UI_BACL_POS_Y, event_handler, ID_GCODE_RETURN, true);
}

void lv_clear_gcode() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
  outV = 0;
}

#endif // HAS_TFT_LVGL_UI
