#include "../../../../../MarlinCore.h"
#if ENABLED(TFT_LITTLE_VGL_UI)
#include "lv_conf.h"
//#include "../../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../../lvgl/src/lv_objx/lv_img.h"
//#include "../../lvgl/src/lv_core/lv_disp.h"
//#include "../../lvgl/src/lv_core/lv_refr.h"
//#include "../../MarlinCore.h"

#include "../inc/draw_ui.h"
#if ENABLED(ADVANCED_PAUSE_FEATURE)
#include "../../../../../feature/pause.h"


void lv_draw_pause_message(const PauseMessage msg)
{
    switch (msg) {
    case PAUSE_MESSAGE_PAUSING:  clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_PAUSING);break;
    case PAUSE_MESSAGE_CHANGING: clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_CHANGING);break;
    case PAUSE_MESSAGE_UNLOAD:   clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_UNLOAD);break;
    case PAUSE_MESSAGE_WAITING:  clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_WAITING);break;
    case PAUSE_MESSAGE_INSERT:   clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_INSERT);break;
    case PAUSE_MESSAGE_LOAD:     clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_LOAD);break;
    case PAUSE_MESSAGE_PURGE:    clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_PURGE);break;
    case PAUSE_MESSAGE_RESUME:   clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_RESUME);break;
    case PAUSE_MESSAGE_HEAT:     clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_HEAT);break;
    case PAUSE_MESSAGE_HEATING:  clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_HEATING);break;
    case PAUSE_MESSAGE_OPTION:   pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
                                 clear_cur_ui();lv_draw_dialog(DIALOG_PAUSE_MESSAGE_OPTION);break;
    case PAUSE_MESSAGE_STATUS:
    default: break;
  }
}
#endif

#endif