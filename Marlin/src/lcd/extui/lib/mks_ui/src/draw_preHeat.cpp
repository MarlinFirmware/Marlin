#include "../../../../../MarlinCore.h"
#if ENABLED(TFT_LITTLE_VGL_UI)
#include "lv_conf.h"
//#include "../../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../../lvgl/src/lv_objx/lv_img.h"
//#include "../../lvgl/src/lv_core/lv_disp.h"
//#include "../../lvgl/src/lv_core/lv_refr.h"
//#include "../../MarlinCore.h"
#include "../../../../../../Configuration.h"
#include "../inc/draw_ui.h"
#include "../../../../../module/temperature.h"

static lv_obj_t * scr;
static lv_obj_t *buttoType,*buttonStep;
static lv_obj_t * labelType;
static lv_obj_t * labelStep;
static lv_obj_t * tempText1;

#define ID_P_ADD		1
#define ID_P_DEC		2
#define ID_P_TYPE		3
#define ID_P_STEP		4
#define ID_P_OFF		5
#define ID_P_RETURN		6

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
	switch(obj->mks_obj_id)
	{
	case ID_P_ADD:
	    if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
	        if(uiCfg.curTempType == 0)
		{
			thermalManager.temp_hotend[uiCfg.curSprayerChoose].target += uiCfg.stepHeat; 
			if(uiCfg.curSprayerChoose == 0)
			{
				if((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > (HEATER_0_MAXTEMP- (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1)))
				{
					thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = (float)HEATER_0_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
			
					thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
				}
			}
			#if !defined(SINGLENOZZLE) && EXTRUDERS >= 2
			else
			{
				if((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > (HEATER_1_MAXTEMP- (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1)))
				{
					thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = (float)HEATER_1_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
			
					thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
				}						
			}
			#endif
		}
		#if HAS_HEATED_BED
		else
		{
			
			thermalManager.temp_bed.target += uiCfg.stepHeat;
			
			if((int)thermalManager.temp_bed.target > BED_MAXTEMP- (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1))
			{
				thermalManager.temp_bed.target = (float)BED_MAXTEMP - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1);
				thermalManager.start_watching_bed();
			}
			
		}
		#endif
		disp_desire_temp();
	    }
		break;
	case ID_P_DEC:
		if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(uiCfg.curTempType == 0)
			{
				if((int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target > uiCfg.stepHeat)
				{
					thermalManager.temp_hotend[uiCfg.curSprayerChoose].target -= uiCfg.stepHeat;
					thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
				}
				else
				{
					thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = (float)0;
					thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
				}
			}	
			#if HAS_HEATED_BED
			else
			{
				if((int)thermalManager.temp_bed.target > uiCfg.stepHeat)
				{
					thermalManager.temp_bed.target -= uiCfg.stepHeat;	
					thermalManager.start_watching_bed();
				}
				else
				{
					thermalManager.temp_bed.target = (float)0;
					thermalManager.start_watching_bed();
				}
			}
			#endif
			disp_desire_temp();
	    }
		
		break;
	case ID_P_TYPE:
		if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(uiCfg.curTempType == 0)
			{
				if(EXTRUDERS == 2)
				{
					if(uiCfg.curSprayerChoose == 0)
					{
						uiCfg.curSprayerChoose = 1;
					}
					else if(uiCfg.curSprayerChoose == 1)
					{
						if(TEMP_SENSOR_BED != 0)
						{
							uiCfg.curTempType = 1;
						}
						else
						{
							uiCfg.curTempType = 0;
							uiCfg.curSprayerChoose = 0;
						}
					}

				}
				else
				{	
					if(uiCfg.curSprayerChoose == 0)
					{
						if(TEMP_SENSOR_BED != 0)
						{
							uiCfg.curTempType = 1;
						}
						else
						{
							uiCfg.curTempType = 0;
						}
					}
				}
			
			}
			else if(uiCfg.curTempType == 1)
			{
				uiCfg.curSprayerChoose = 0;
				uiCfg.curTempType = 0;
			}
	        	disp_temp_type();
	    }
		break;
	case ID_P_STEP:
		if(event == LV_EVENT_CLICKED) {
			
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			switch(uiCfg.stepHeat)
			{
				case 1:
					uiCfg.stepHeat = 5;
					break;

				case 5:
					uiCfg.stepHeat = 10;
					break;
					
				case 10:
					uiCfg.stepHeat = 1;
					break;

				default:
					break;
			}
			disp_step_heat();
	    }
		break;
	case ID_P_OFF:
		if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(uiCfg.curTempType == 0)
			{
				thermalManager.temp_hotend[uiCfg.curSprayerChoose].target = (float)0;
				thermalManager.start_watching_hotend(uiCfg.curSprayerChoose);
			}
			#if HAS_HEATED_BED
			else
			{
				thermalManager.temp_bed.target = (float)0;
				thermalManager.start_watching_bed();						
			}
			#endif
			disp_desire_temp();
	    }
		break;
	case ID_P_RETURN:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			clear_cur_ui();
	        	draw_return_ui();
	    }
		break;

	}
}


void lv_draw_preHeat(void)
{
	lv_obj_t *buttonAdd,*buttonDec;
	lv_obj_t *buttonOff,*buttonBack;

	if(disp_state_stack._disp_state[disp_state_stack._disp_index] != PRE_HEAT_UI)
	{
		disp_state_stack._disp_index++;
		disp_state_stack._disp_state[disp_state_stack._disp_index] = PRE_HEAT_UI;
	}
	disp_state = PRE_HEAT_UI;

	scr = lv_obj_create(NULL, NULL);
	
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
       buttonAdd = lv_imgbtn_create(scr, NULL);
	buttonDec = lv_imgbtn_create(scr, NULL);
	buttoType = lv_imgbtn_create(scr, NULL);
	buttonStep = lv_imgbtn_create(scr, NULL);
	buttonOff = lv_imgbtn_create(scr, NULL);
	buttonBack = lv_imgbtn_create(scr, NULL);

	lv_obj_set_event_cb_mks(buttonAdd, event_handler,ID_P_ADD,"bmp_Add.bin",0);
	lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_REL, &bmp_pic);
	lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_PR, &bmp_pic);
	lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_REL, &tft_style_lable_rel);
	lv_obj_clear_protect(buttonAdd, LV_PROTECT_FOLLOW);
	#if 1
	lv_obj_set_event_cb_mks(buttonDec, event_handler,ID_P_DEC,"bmp_Dec.bin",0);
	lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_REL, &bmp_pic);
	lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_PR, &bmp_pic);
	lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_REL, &tft_style_lable_rel);

		
    lv_imgbtn_set_src(buttoType, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttoType, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttoType, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttoType, LV_BTN_STATE_REL, &tft_style_lable_rel);

	
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonStep, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonStep, LV_BTN_STATE_REL, &tft_style_lable_rel);

	lv_obj_set_event_cb_mks(buttonOff, event_handler,ID_P_OFF,"bmp_Speed0.bin",0);	
    lv_imgbtn_set_src(buttonOff, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonOff, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonOff, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonOff, LV_BTN_STATE_REL, &tft_style_lable_rel);

	lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_P_RETURN,"bmp_Return.bin",0);	
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_pic);	
	lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_lable_pre);
	lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_lable_rel);

	#endif
	
	lv_obj_set_pos(buttonAdd,INTERVAL_V,titleHeight);
	lv_obj_set_pos(buttonDec,BTN_X_PIXEL*3+INTERVAL_V*4,titleHeight);
	lv_obj_set_pos(buttoType,INTERVAL_V,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);
	lv_obj_set_pos(buttonStep,BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H+titleHeight);
	lv_obj_set_pos(buttonOff,BTN_X_PIXEL*2+INTERVAL_V*3,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);
	lv_obj_set_pos(buttonBack,BTN_X_PIXEL*3+INTERVAL_V*4,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);

    /*Create a label on the Image button*/
	lv_btn_set_layout(buttonAdd, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonDec, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttoType, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonStep, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonOff, LV_LAYOUT_OFF);
	lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
	
    lv_obj_t * labelAdd = lv_label_create(buttonAdd, NULL);
	lv_obj_t * labelDec = lv_label_create(buttonDec, NULL);
	labelType = lv_label_create(buttoType, NULL);
	labelStep = lv_label_create(buttonStep, NULL);
	lv_obj_t * labelOff = lv_label_create(buttonOff, NULL);
	lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);
	
	
	if(gCfgItems.multiple_language !=0)
	{
	    lv_label_set_text(labelAdd, preheat_menu.add);
		lv_obj_align(labelAdd, buttonAdd, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		lv_label_set_text(labelDec, preheat_menu.dec);
		lv_obj_align(labelDec, buttonDec, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		lv_label_set_text(labelOff, preheat_menu.off);
		lv_obj_align(labelOff, buttonOff, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);

		lv_label_set_text(label_Back, common_menu.text_back);
		lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
	}

	disp_temp_type();
	disp_step_heat();

	tempText1 = lv_label_create(scr, NULL);
	lv_obj_set_style(tempText1, &tft_style_lable_rel);
	disp_desire_temp();
}

void disp_temp_type()
{
	
	if(uiCfg.curTempType == 0)
	{	
		if(uiCfg.curSprayerChoose == 1)
		{			
			lv_obj_set_event_cb_mks(buttoType, event_handler,ID_P_TYPE,"bmp_Extru2.bin",0);
			if(gCfgItems.multiple_language != 0)
			{
				lv_label_set_text(labelType, preheat_menu.ext2);
				lv_obj_align(labelType, buttoType, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
			}
		}
		else
		{
			lv_obj_set_event_cb_mks(buttoType, event_handler,ID_P_TYPE,"bmp_Extru1.bin",0);
			if(gCfgItems.multiple_language != 0)
			{
				lv_label_set_text(labelType, preheat_menu.ext1);
				lv_obj_align(labelType, buttoType, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
			}
		}	
	
	}
	else
	{	
		lv_obj_set_event_cb_mks(buttoType, event_handler,ID_P_TYPE,"bmp_Bed.bin",0);
		if(gCfgItems.multiple_language != 0)
		{
			lv_label_set_text(labelType, preheat_menu.hotbed);
			lv_obj_align(labelType, buttoType, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
		}
	}

}

void disp_desire_temp()
{
	char buf[20] = {0};

	public_buf_l[0] = '\0';
	
	if(uiCfg.curTempType == 0)
	{
		if(uiCfg.curSprayerChoose<1)
		{
			strcat(public_buf_l,preheat_menu.ext1);
		}
		else
		{
			strcat(public_buf_l,preheat_menu.ext2);
		}
		sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius,  (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target);
		
	}
	#if HAS_HEATED_BED
	else
	{
		strcat(public_buf_l,preheat_menu.hotbed);

		sprintf(buf, preheat_menu.value_state, (int)thermalManager.temp_bed.celsius,  (int)thermalManager.temp_bed.target);
	}
	#endif
	strcat(public_buf_l,": ");
	strcat(public_buf_l,buf);
	lv_label_set_text(tempText1, public_buf_l);
	lv_obj_align(tempText1, NULL, LV_ALIGN_CENTER,0, -50);
}

void disp_step_heat()
{
	if(uiCfg.stepHeat == 1)
		lv_obj_set_event_cb_mks(buttonStep, event_handler,ID_P_STEP,"bmp_Step1_degree.bin",0);	
	else if(uiCfg.stepHeat == 5)
		lv_obj_set_event_cb_mks(buttonStep, event_handler,ID_P_STEP,"bmp_Step5_degree.bin",0);
	else if(uiCfg.stepHeat == 10)
		lv_obj_set_event_cb_mks(buttonStep, event_handler,ID_P_STEP,"bmp_Step10_degree.bin",0);

	if(gCfgItems.multiple_language != 0)
	{
		if(uiCfg.stepHeat == 1)
		{
			lv_label_set_text(labelStep, preheat_menu.step_1c);
			lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
		}	
		else if(uiCfg.stepHeat == 5)
		{
			lv_label_set_text(labelStep, preheat_menu.step_5c);
			lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
		}
		else if(uiCfg.stepHeat == 10)
		{
			lv_label_set_text(labelStep, preheat_menu.step_10c);
			lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
		}	
	}	
}

void lv_clear_preHeat()
{
	lv_obj_del(scr);
}
#endif