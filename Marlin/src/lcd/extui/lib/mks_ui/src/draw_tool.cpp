#include "../../../../../MarlinCore.h"
#if ENABLED(TFT_LITTLE_VGL_UI)
#include "lv_conf.h"
//#include "../../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../../lvgl/src/lv_objx/lv_img.h"
//#include "../../lvgl/src/lv_core/lv_disp.h"
//#include "../../lvgl/src/lv_core/lv_refr.h"
#include "../inc/draw_ui.h"
#include "../../../../../gcode/queue.h"

//static lv_obj_t *buttonMoveZ,*buttonTest,*buttonZ0,*buttonStop,*buttonReturn;
static lv_obj_t * scr;
#if ENABLED(MKS_TEST)
extern uint8_t curent_disp_ui;
#endif

#define ID_T_PRE_HEAT		1
#define ID_T_EXTRUCT		2
#define ID_T_MOV			3
#define ID_T_HOME			4
#define ID_T_LEVELING		5
#define ID_T_FILAMENT		6
#define ID_T_MORE			7
#define ID_T_RETURN			8

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
	switch(obj->mks_obj_id)
	{
	case ID_T_PRE_HEAT:
	    if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        lv_clear_tool();
			lv_draw_preHeat();
	    }
		break;
	case ID_T_EXTRUCT:
		if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        lv_clear_tool();
		 lv_draw_extrusion();
	    }
		break;
	case ID_T_MOV:
		if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        lv_clear_tool();
		 	lv_draw_move_motor();
	    }
		
		break;
	case ID_T_HOME:
		if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        lv_clear_tool();
		 lv_draw_home();
	    }
		break;
	case ID_T_LEVELING:
		if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
			queue.enqueue_one_P(PSTR("G28"));
			queue.enqueue_one_P(PSTR("G29"));
			#else
			uiCfg.leveling_first_time = 1;
			lv_clear_tool();
		 	lv_draw_manualLevel();
			#endif
	    }
		
		break;
	case ID_T_FILAMENT:
		
		break;
	case ID_T_MORE:
		
		break;
	case ID_T_RETURN:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        	#if ENABLED(MKS_TEST)
			curent_disp_ui = 1;
			#endif
			lv_obj_del(scr);
	        	lv_draw_ready_print();
	    }
		break;

	}
}


void lv_draw_tool(void)
{
	lv_obj_t *buttonPreHeat,*buttonExtrusion,*buttonMove,*buttonHome,*buttonLevel;
	lv_obj_t *buttonBack;

	if(disp_state_stack._disp_state[disp_state_stack._disp_index] != TOOL_UI)
	{
		disp_state_stack._disp_index++;
		disp_state_stack._disp_state[disp_state_stack._disp_index] = TOOL_UI;
	}
	disp_state = TOOL_UI;

	scr = lv_obj_create(NULL, NULL);

	//static lv_style_t tool_style;
	
	lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
	lv_obj_set_style(title, &tft_style_lable_rel);
	lv_obj_set_pos(title,TITLE_XPOS,TITLE_YPOS);
	lv_label_set_text(title, creat_title_text());
  
  lv_refr_now(lv_refr_get_disp_refreshing());
	
	LV_IMG_DECLARE(bmp_pic);
	
    /*Create an Image button*/
       buttonPreHeat = lv_imgbtn_create(scr, NULL);
	buttonExtrusion = lv_imgbtn_create(scr, NULL);
	buttonMove = lv_imgbtn_create(scr, NULL);
	buttonHome = lv_imgbtn_create(scr, NULL);
	buttonLevel = lv_imgbtn_create(scr, NULL);
	//buttonFilament = lv_imgbtn_create(scr, NULL);
	//buttonMore = lv_imgbtn_create(scr, NULL);
	buttonBack = lv_imgbtn_create(scr, NULL);

	
	lv_obj_set_event_cb_mks(buttonPreHeat, event_handler,ID_T_PRE_HEAT,"bmp_PreHeat.bin",0);
	lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_REL, &bmp_pic);
	lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_PR, &bmp_pic);
	lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_REL, &tft_style_lable_rel);
	lv_obj_clear_protect(buttonPreHeat, LV_PROTECT_FOLLOW);
	#if 1
	lv_obj_set_event_cb_mks(buttonExtrusion, event_handler,ID_T_EXTRUCT,"bmp_Extruct.bin",0);
	lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_REL, &bmp_pic);
	lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_PR, &bmp_pic);
	lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_REL, &tft_style_lable_rel);

	lv_obj_set_event_cb_mks(buttonMove, event_handler,ID_T_MOV,"bmp_Mov.bin",0);	
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_REL, &tft_style_lable_rel);

	lv_obj_set_event_cb_mks(buttonHome, event_handler,ID_T_HOME,"bmp_Zero.bin",0);	
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_REL, &tft_style_lable_rel);

	lv_obj_set_event_cb_mks(buttonLevel, event_handler,ID_T_LEVELING,"bmp_Leveling.bin",0);	
    lv_imgbtn_set_src(buttonLevel, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonLevel, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonLevel, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonLevel, LV_BTN_STATE_REL, &tft_style_lable_rel);

	//lv_obj_set_event_cb_mks(buttonFilament, event_handler,ID_T_FILAMENT,"bmp_Filamentchange.bin",0);	
    //lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_REL, &bmp_pic);
    //lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_PR, &bmp_pic);	
	//lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_PR, &tft_style_lable_pre);
	//lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_REL, &tft_style_lable_rel);

	//lv_obj_set_event_cb_mks(buttonMore, event_handler,ID_T_MORE,"bmp_More.bin",0);	
    //lv_imgbtn_set_src(buttonMore, LV_BTN_STATE_REL, &bmp_pic);
    //lv_imgbtn_set_src(buttonMore, LV_BTN_STATE_PR, &bmp_pic);	
	//lv_imgbtn_set_style(buttonMore, LV_BTN_STATE_PR, &tft_style_lable_pre);
	//lv_imgbtn_set_style(buttonMore, LV_BTN_STATE_REL, &tft_style_lable_rel);

	lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_T_RETURN,"bmp_Return.bin",0);	
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_lable_rel);

	#endif
	
	lv_obj_set_pos(buttonPreHeat,INTERVAL_V,titleHeight);
	lv_obj_set_pos(buttonExtrusion,BTN_X_PIXEL+INTERVAL_V*2,titleHeight);
	lv_obj_set_pos(buttonMove,BTN_X_PIXEL*2+INTERVAL_V*3,titleHeight);
	lv_obj_set_pos(buttonHome,BTN_X_PIXEL*3+INTERVAL_V*4,titleHeight);
	lv_obj_set_pos(buttonLevel,INTERVAL_V,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);
	//lv_obj_set_pos(buttonFilament,BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H+titleHeight);
	//lv_obj_set_pos(buttonMore,BTN_X_PIXEL*2+INTERVAL_V*3,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);
	lv_obj_set_pos(buttonBack,BTN_X_PIXEL*3+INTERVAL_V*4,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);

    /*Create a label on the Image button*/
	lv_btn_set_layout(buttonPreHeat, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonExtrusion, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonMove, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonHome, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonLevel, LV_LAYOUT_OFF);
	//lv_btn_set_layout(buttonFilament, LV_LAYOUT_OFF);
	//lv_btn_set_layout(buttonMore, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
	
    lv_obj_t * labelPreHeat = lv_label_create(buttonPreHeat, NULL);
	lv_obj_t * labelExtrusion = lv_label_create(buttonExtrusion, NULL);
	lv_obj_t * label_Move = lv_label_create(buttonMove, NULL);
	lv_obj_t * label_Home = lv_label_create(buttonHome, NULL);
	lv_obj_t * label_Level = lv_label_create(buttonLevel, NULL);
	//lv_obj_t * label_Filament = lv_label_create(buttonFilament, NULL);
	//lv_obj_t * label_More = lv_label_create(buttonMore, NULL);
	lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);
	
	
	if(gCfgItems.multiple_language !=0)
	{
	    lv_label_set_text(labelPreHeat, tool_menu.preheat);
		lv_obj_align(labelPreHeat, buttonPreHeat, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		lv_label_set_text(labelExtrusion, tool_menu.extrude);
		lv_obj_align(labelExtrusion, buttonExtrusion, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		lv_label_set_text(label_Move, tool_menu.move);
		lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

	    lv_label_set_text(label_Home, tool_menu.home);
		lv_obj_align(label_Home, buttonHome, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		/*if(gCfgItems.leveling_mode != 2)
		{
			if(gCfgItems.leveling_mode == 1)
				lv_label_set_text(label_Level, tool_menu.autoleveling);
			else
				lv_label_set_text(label_Level, tool_menu.leveling);
			
			lv_obj_align(label_Level, buttonLevel, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
		}*/
		#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
		lv_label_set_text(label_Level, tool_menu.autoleveling);
		#else
		lv_label_set_text(label_Level, tool_menu.leveling);
		#endif
		lv_obj_align(label_Level, buttonLevel, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
		
		//lv_label_set_text(label_Filament, tool_menu.filament);
		//lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		//lv_label_set_text(label_More, tool_menu.more);
		//lv_obj_align(label_More, buttonMore, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		lv_label_set_text(label_Back, common_menu.text_back);
		lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
	}
}

void lv_clear_tool()
{
	lv_obj_del(scr);
}
#endif