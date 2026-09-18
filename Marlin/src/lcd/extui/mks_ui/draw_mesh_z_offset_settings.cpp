#include "../../../inc/MarlinConfigPre.h"

#if ALL(HAS_TFT_LVGL_UI, GLOBAL_MESH_Z_OFFSET)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../feature/bedlevel/bedlevel.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_MESH_ZOFFSET_RETURN = 1,
  ID_MESH_ZOFFSET_VALUE
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_MESH_ZOFFSET_RETURN:
      lv_clear_mesh_z_offset_settings();
      draw_return_ui();
      break;
    case ID_MESH_ZOFFSET_VALUE:
      value = meshZOffset;
      lv_clear_mesh_z_offset_settings();
      lv_draw_number_key();
      break;
  }
}

void lv_draw_mesh_z_offset_settings() {
  scr = lv_screen_create(LEVELING_PARA_UI, machine_menu.LevelingParaConfTitle);

  dtostrf(mesh_z_offset, 1, 2, public_buf_l);
  lv_screen_menu_item_1_edit(scr, GET_TEXT(MSG_MESH_Z_OFFSET), PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_MESH_ZOFFSET_VALUE, 0, public_buf_l);

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X, PARA_UI_BACK_POS_Y, event_handler, ID_MESH_ZOFFSET_RETURN, true);
}

void lv_clear_mesh_z_offset_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && GLOBAL_MESH_Z_OFFSET
