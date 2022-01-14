/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if BOTH(HAS_TFT_LVGL_UI, PROBE_OFFSET_WIZARD)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../gcode/queue.h"
#include "../../../module/motion.h"

#include "../../../module/planner.h"
#include "../../../inc/MarlinConfig.h"
#include "../../../module/probe.h"

#if HAS_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
  bool leveling_was_active;
#endif

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV, *labelP;
static lv_obj_t *labelP_z_offset_ref;
static lv_task_t *updatePosTask;
static char cur_label = 'Z';
static float cur_pos = 0;
static float cur_OffsetPos = 0;

// Global storage
float z_offset_backup, calculated_z_offset, z_offset_ref;

enum {
  ID_M_Z_P = 1,
  ID_M_Z_N,
  ID_M_STEP,
  ID_M_SAVE,
  ID_M_RETURN
};

void disp_cur_wizard_pos() {
  char str_1[18];
  sprintf_P(public_buf_l, PSTR("%c:%s mm"), cur_label, dtostrf(cur_pos, 1, 3, str_1));
  if (labelP) lv_label_set_text(labelP, public_buf_l);

  sprintf_P(public_buf_l, PSTR("%c Offset:%s mm"), cur_label, dtostrf(cur_OffsetPos, 1, 3, str_1));
  if (labelP_z_offset_ref) lv_label_set_text(labelP_z_offset_ref, public_buf_l);
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  char str_1[16];
  if (event != LV_EVENT_RELEASED) return;
  //lv_clear_z_offset_wizard();
  if (!queue.ring_buffer.full(3)) {
    bool do_inject = true;
    float dist = uiCfg.move_dist;
    switch (obj->mks_obj_id) {
      case ID_M_Z_N: dist *= -1; case ID_M_Z_P: cur_label = 'Z'; break;
      default: do_inject = false;
    }
    if (do_inject) {
      sprintf_P(public_buf_l, PSTR("G91\nG1 %c%s F%d\nG90"), cur_label, dtostrf(dist, 1, 3, str_1), uiCfg.moveSpeed);
      queue.inject(public_buf_l);
      //calculated_z_offset = probe.offset.z + current_position.z - z_offset_ref;
      disp_cur_wizard_pos();
    }
  }

  switch (obj->mks_obj_id) {
    case ID_M_STEP:
      if (ABS((int)(10 * uiCfg.move_dist)) == 10)
        uiCfg.move_dist = 0.025;
      else if (ABS((int)(1000 * uiCfg.move_dist)) == 25)
        uiCfg.move_dist = 0.1;
      else
        uiCfg.move_dist *= 10.0f;
      disp_move_wizard_dist();
      break;
    case ID_M_SAVE:
      current_position.z = z_offset_ref;  // Set Z to z_offset_ref, as we can expect it is at probe height
      probe.offset.z = calculated_z_offset;
      sync_plan_position();
      // Raise Z as if it was homed
      do_z_clearance(Z_POST_CLEARANCE);
      watchdog_refresh();
      draw_return_ui();
      return;
    case ID_M_RETURN:
      probe.offset.z = z_offset_backup;
      SET_SOFT_ENDSTOP_LOOSE(false);
      TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));
      #if HOMING_Z_WITH_PROBE && defined(PROBE_OFFSET_WIZARD_START_Z)
        set_axis_never_homed(Z_AXIS); // On cancel the Z position needs correction
        queue.inject_P(PSTR("G28Z"));
      #else // Otherwise do a Z clearance move like after Homing
        do_z_clearance(Z_POST_CLEARANCE);
      #endif
      watchdog_refresh();
      draw_return_ui();
      return;
  }
  disp_cur_wizard_pos();
}

void refresh_wizard_pos(lv_task_t *) {
  switch (cur_label) {
    case 'Z':
      cur_pos = current_position.z;
      calculated_z_offset = probe.offset.z + current_position.z - z_offset_ref;
      cur_OffsetPos = calculated_z_offset;
    break;
    default: return;
  }
  disp_cur_wizard_pos();
}

void lv_draw_z_offset_wizard() {

  set_all_unhomed();

  // Store probe.offset.z for Case: Cancel
  z_offset_backup = probe.offset.z;

  #ifdef PROBE_OFFSET_WIZARD_START_Z
    probe.offset.z = PROBE_OFFSET_WIZARD_START_Z;
  #endif

  // Store Bed-Leveling-State and disable
  #if HAS_LEVELING
    leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(leveling_was_active);
  #endif

  queue.inject_P(PSTR("G28"));

  z_offset_ref = 0;             // Set Z Value for Wizard Position to 0
  calculated_z_offset = probe.offset.z + current_position.z - z_offset_ref;
  cur_OffsetPos = calculated_z_offset;

  scr = lv_screen_create(Z_OFFSET_WIZARD_UI, machine_menu.LevelingZoffsetTitle);

  lv_obj_t *buttonXI = lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, INTERVAL_V, titleHeight, event_handler, ID_M_Z_P);
  lv_obj_clear_protect(buttonXI, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Z_N);

  // button with image and label changed dynamically by disp_move_dist
  buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_M_STEP);
  labelV = lv_label_create_empty(buttonV);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonV);
  #endif

  // save and back
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_save, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_SAVE);
  // cancel and back
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_RETURN);

  // We need to patch the title to leave some space on the right for displaying the status
  lv_obj_t * z_offset_ref_title = lv_obj_get_child_back(scr, nullptr);
  if (z_offset_ref_title != nullptr) lv_obj_set_width(z_offset_ref_title, (int)(TFT_WIDTH / 2) - 101);
    labelP_z_offset_ref = lv_label_create(scr, (int)(TFT_WIDTH / 2) - 80, (int)(TFT_HEIGHT/2)-20, "Z Offset:0.0mm");

  // We need to patch the Z Offset to leave some space in the middle for displaying the status
  lv_obj_t * title= lv_obj_get_child_back(scr, nullptr);
  if (title != nullptr) lv_obj_set_width(title, TFT_WIDTH - 101);
  labelP = lv_label_create(scr, TFT_WIDTH - 100, TITLE_YPOS, "Z:0.0mm");

  if (labelP != nullptr)
    updatePosTask = lv_task_create(refresh_wizard_pos, 300, LV_TASK_PRIO_LOWEST, 0);

  disp_move_wizard_dist();
  disp_cur_wizard_pos();
}

void disp_move_wizard_dist() {
  if ((int)(1000 * uiCfg.move_dist) == 25)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move0_1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move10.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(1000 * uiCfg.move_dist) == 25) {
      lv_label_set_text(labelV, move_menu.step_0025mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(10 * uiCfg.move_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(10 * uiCfg.move_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_1mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void lv_clear_z_offset_wizard() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && PROBE_OFFSET_WIZARD
