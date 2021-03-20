#include "../inc/MarlinConfig.h"
#include "display_image.h"
#ifdef LONGER3D
#include "gbk_font16.h"
#include "../../longer/W25Qxx.h"
#include "../../lcd/menu/touch/lcd.h"
#include "../../lcd/menu/touch/xpt2046.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/temperature.h"
#include "sdfile.h"
#include "../../sd/cardreader.h"
#include "../../HAL/HAL_STM32F1/HAL_sdio_STM32F1.h"
#include "../../libs/stopwatch.h"
#include "LGT_MACRO.h"
#include "config_storedata.h"
extern bool check_filament_disable,list_order;
extern ARGUMENTS re_settings;
config_storedata ConfigSettings;
extern Stopwatch print_job_timer;
sdfile CardFile;
extern uint8_t cp_time_minutes, cp_time_seconds,Remaining_minutes;	
extern uint16_t cp_time_hours,Remaining_hours;
extern CardReader card;
extern uint16_t color,bgColor;
extern  W25Qxx W25QxxFlash;
extern display_image Display_Screen;
extern Planner planner;
extern uint16_t lcdId;
extern uint16_t color;
extern int16_t xCalibration, yCalibration, xOffset, yOffset;
extern float resume_xyze_position[XYZE];
extern float save_feedtate;
bool recovery_flag;
extern void disable_all_steppers();
extern void LGT_Lcdfsmc_init();

uint16_t POINT_COLOR=0x0000;
uint8_t image_buffer[GET_FLASH_SIZE];	//2k

int16_t cur_fanspeed=0;
uint8_t default_move_distance=5;
int8_t dir_auto_feed=0;
static uint8_t total_out_distance=0;
uint8_t printpercent=0;
uint8_t page_index_num=0;
int8_t choose_printfile=-1;
	
uint16_t X_AD=0,Y_AD=0,cur_x=0,cur_y=0;
uint16_t page_index_max=0,page_index=0,file_count=0,choose_file_page=0;

char s_text[64];
char pdata[64];
uint32_t print_times=0;
const float manual_feedrate_mm_m[] = MANUAL_FEEDRATE;

bool is_aixs_homed[XYZ]={false};
static bool is_bed_select = false;
bool sd_insert=false;
bool is_printing=false;
bool is_print_finish=false;
bool pause_print=false;
bool cur_flag=false;  
char cur_pstatus=10;   //0 is heating ,1 is printing, 2 is pause
char cur_ppage=10;   //  0 is heating page , 1 is printing page, 2 is pause page

bool setting_return_more=false;
float default_parameter[4]=DEFAULT_MAX_FEEDRATE;

E_PRINT_CMD current_print_cmd=E_PRINT_CMD_NONE;
E_BUTTON_KEY current_button_id=eBT_BUTTON_NONE;
/**********  window definition  **********/
E_WINDOW_ID current_window_ID = eMENU_HOME,next_window_ID =eWINDOW_NONE;


const char* c_dialog_text[eDIALOG_MAX][4]={
	{TXT_DIALOG_CAPTION_START,				DIALOG_PROMPT_PRINT_START1,DIALOG_PROMPT_PRINT_START2,DIALOG_PROMPT_PRINT_START3},
	{TXT_DIALOG_CAPTION_EXIT, 	   			DIALOG_PROMPT_PRINT_EXIT1,DIALOG_PROMPT_PRINT_EXIT2,DIALOG_PROMPT_PRINT_EXIT3},
	{TXT_DIALOG_CAPTION_ABORT, 	   			DIALOG_PROMPT_PRINT_ABORT1,DIALOG_PROMPT_PRINT_ABORT2,DIALOG_PROMPT_PRINT_ABORT3},
	{TXT_DIALOG_CAPTION_RECOVERY, 			DIALOG_PROMPT_PRINT_RECOVERY1,DIALOG_PROMPT_PRINT_RECOVERY2,DIALOG_PROMPT_PRINT_RECOVERY3},
	{TXT_DIALOG_CAPTION_ERROR, 	     		DIALOG_PROMPT_ERROR_READ1,DIALOG_PROMPT_ERROR_READ2,DIALOG_PROMPT_ERROR_READ3},
	{TXT_DIALOG_CAPTION_RESTORE,     		DIALOG_PROMPT_SETTS_RESTORE1,DIALOG_PROMPT_SETTS_RESTORE2,DIALOG_PROMPT_SETTS_RESTORE3},
	{TXT_DIALOG_CAPTION_SAVE, 				DIALOG_PROMPT_SETTS_SAVE_OK1,DIALOG_PROMPT_SETTS_SAVE_OK2,DIALOG_PROMPT_SETTS_SAVE_OK3},
	{TXT_DIALOG_CAPTION_SAVE,         		DIALOG_PROMPT_SETTS_SAVE1,DIALOG_PROMPT_SETTS_SAVE2,DIALOG_PROMPT_SETTS_SAVE3},
	{TXT_DIALOG_CAPTION_NO_FIALMENT,  		DIALOG_PROMPT_NO_FILAMENT1,DIALOG_PROMPT_NO_FILAMENT2,DIALOG_PROMPT_NO_FILAMENT3},
	{TXT_DIALOG_CAPTION_ERROR,        		DIALOG_ERROR_FILE_TYPE1,DIALOG_ERROR_FILE_TYPE2,DIALOG_ERROR_FILE_TYPE3},
	{TXT_DIALOG_CAPTION_ERROR,       		DIALOG_ERROR_TEMP_BED1,DIALOG_ERROR_TEMP_BED2,DIALOG_ERROR_TEMP_BED3},
	{TXT_DIALOG_CAPTION_ERROR,       		DIALOG_ERROR_TEMP_HEAD1,DIALOG_ERROR_TEMP_HEAD2,DIALOG_ERROR_TEMP_HEAD3},
	{TXT_DIALOG_CAPTION_OPEN_FOLER,         DIALOG_PROMPT_MAX_FOLDER1,DIALOG_PROMPT_MAX_FOLDER2,DIALOG_PROMPT_MAX_FOLDER3},
	{TXT_DIALOG_CAPTION_NO_FIALMENT,         DIALOG_START_PRINT_NOFILA1,DIALOG_START_PRINT_NOFILA2,DIALOG_START_PRINT_NOFILA3}
};

void LGT_Line_To_Current_Position(AxisEnum axis)
{
	if (!planner.is_full())
	{
		planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[(int8_t)axis]), active_extruder);
	}
}
void actAutoFeed(void)
{
	if(dir_auto_feed == 0) return; 
	if(dir_auto_feed == 1)   //positive direction
	{		       				
		enqueue_and_echo_commands_P(PSTR("G0 E15 F120"));
	}
	else if(dir_auto_feed == -1)  //negative direction
	{      
        if(total_out_distance < 90)
		{
            enqueue_and_echo_commands_P(PSTR("G0 E-10 F600"));
            total_out_distance += 1;
        }
		else  // arrive max length 
		{  
            stopAutoFeed();
           thermalManager.temp_hotend[0].target=0;  //prevent from fire harzard   
        }
	}
}
void stopAutoFeed(void)
{
	if(dir_auto_feed==1||dir_auto_feed==-1)
	{
		clear_command_queue();	
		planner.quick_stop();
		enqueue_and_echo_commands_P(PSTR("G90"));
		dir_auto_feed=0;
		total_out_distance=0;
	}
}
void startAutoFeed(int8_t dir)
{
	if(dir == dir_auto_feed) return; 
	if(thermalManager.temp_hotend[0].target<200)
	{
		thermalManager.temp_hotend[0].target=200;
		thermalManager.start_watching_heater(0);
		if(thermalManager.temp_hotend[0].current>195)
		{
			 if(abs(dir) == 1)    //  validity checking
			{    
        		dir_auto_feed = dir;  
				enqueue_and_echo_commands_P(PSTR("G91"));
        		total_out_distance = 0;
   			 }
		}
		if(is_bed_select)
		{
			is_bed_select=false;
			Display_Screen.displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);
		}
	}
	else if(thermalManager.temp_hotend[0].current>195)
	{
		if(abs(dir) == 1)    //  validity checking
		{    
        	dir_auto_feed = dir;  
			enqueue_and_echo_commands_P(PSTR("G91"));
        	total_out_distance = 0;
   		 }
		if(is_bed_select)
		{
			is_bed_select=false;
			Display_Screen.displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);
		}
	}
}
void clearVarPrintEnd()
{
	//time
	print_job_timer.reset();
	CardFile.is_gettime=false;
	print_times=0;
 	cp_time_minutes=cp_time_seconds=0;	
    cp_time_hours =0;
	Remaining_hours=0;
    Remaining_minutes=0;
	
	checkFilamentReset();
	recovery_flag=false;
	W25QxxFlash.W25QXX_Write((uint8_t*)0xff,SAVE_FILE_ADDR,(uint16_t)sizeof(card.longFilename));
	
	planner.flow_percentage[0]=100;
	feedrate_percentage=100;
	clearfilevar();
//	card.returnroot();
	card.initsd();   //return root directory when printing is completed
}
void clearfilevar()
{
	//clear
	page_index_max=page_index=file_count=choose_file_page=0;
	page_index_num=0;choose_printfile=-1;
}
bool check_nodialog=false;
uint8_t check_temp_E=0,check_temp_Bed=0;
void actExceedTemperScope(uint8_t e, bool is_hot)
{
	switch (current_window_ID)
	{
		case eMENU_PRINT:case eMENU_ADJUST:case eMENU_ADJUST_MORE:
			card.flag.abort_sd_printing=true;
			is_printing=wait_for_user = wait_for_heatup=false;
			current_window_ID=eMENU_HOME;
			Display_Screen.displayWindowHome();
			check_nodialog=false;
		break;
		case eMENU_HOME:
			check_nodialog=true;
		break;
		default:
			current_window_ID=eMENU_HOME;
			Display_Screen.displayWindowHome();
			check_nodialog=false;
		break;
	}
	// if(check_nodialog)
	// 	return;
	if(e == 0 && is_hot) //hot extruder
	{	
		if(check_temp_Bed!=CHECK_TEMP_TIMES)		
		{
			#ifndef Chinese
				Display_Screen.displayImage(10, 45, IMG_ADDR_WARNING_HOT_HEAD);
			#else
					Display_Screen.displayImage(10, 45, IMG_ADDR_WARNING_HOT_HEAD_CN);
			#endif
			thermalManager.temp_bed.target=0;
			if(!check_nodialog)
			{
				Display_Screen.dispalyDialogYes(eDIALOG_ERROR_TEMP_HEAD);
				current_window_ID=eMENU_DIALOG_ERRORTEMPE;
			}
		}
	}
	else if(e == 0 && !is_hot)  //cold extruder
	{	
		if(check_temp_Bed!=CHECK_TEMP_TIMES)		
		{
			#ifndef Chinese
				Display_Screen.displayImage(10, 45, IMG_ADDR_WARNING_COLD_HEAD); 
			#else
				Display_Screen.displayImage(10, 45, IMG_ADDR_WARNING_COLD_HEAD_CN); 
			#endif
			thermalManager.temp_bed.target=0;
			if(!check_nodialog)
			{	
				Display_Screen.dispalyDialogYes(eDIALOG_ERROR_TEMP_HEAD);
				current_window_ID=eMENU_DIALOG_ERRORTEMPE;
			}
		}
	}
	else if(e == 1 && is_hot) 	//hot bed
	{	
		if(check_temp_E!=CHECK_TEMP_TIMES)		
		{
			#ifndef Chinese
				Display_Screen.displayImage(10, 145, IMG_ADDR_WARNING_HOT_BED);
			#else
				Display_Screen.displayImage(10, 145, IMG_ADDR_WARNING_HOT_BED_CN);
			#endif
			thermalManager.temp_hotend[0].target=0;
			if(!check_nodialog)
			{	
				Display_Screen.dispalyDialogYes(eDIALOG_ERROR_TEMP_BED);
				current_window_ID=eMENU_DIALOG_ERRORTEMPBED;
			}
		}
	}
	else //cold bed
	{			
		if(check_temp_E!=CHECK_TEMP_TIMES)		
		{	
			#ifndef Chinese		
				Display_Screen.displayImage(10, 145, IMG_ADDR_WARNING_COLD_BED);	
			#else
				Display_Screen.displayImage(10, 145, IMG_ADDR_WARNING_COLD_BED_CN);	
			#endif
			thermalManager.temp_hotend[0].target=0;
			if(!check_nodialog)
			{
				Display_Screen.dispalyDialogYes(eDIALOG_ERROR_TEMP_BED);
				current_window_ID=eMENU_DIALOG_ERRORTEMPBED;
			}
		}
	}
	if(check_temp_E==CHECK_TEMP_TIMES&&check_temp_Bed==CHECK_TEMP_TIMES)
	{
		#ifndef Chinese
			Display_Screen.displayImage(10, 145, IMG_ADDR_WARNING_COLD_BED);
			Display_Screen.displayImage(10, 45, IMG_ADDR_WARNING_COLD_HEAD); 	
		#else
			Display_Screen.displayImage(10, 145, IMG_ADDR_WARNING_COLD_BED_CN);	
			Display_Screen.displayImage(10, 45, IMG_ADDR_WARNING_COLD_HEAD_CN); 
		#endif
	}
}
void checkTemprature()
{
	if(current_window_ID==eMENU_DIALOG_ERRORTEMPBED||           
	current_window_ID==eMENU_DIALOG_ERRORTEMPE||is_printing)       //if it's displaying error prompt, just return with no check
		return;	
	if(thermalManager.temp_hotend[0].current<=MIN_ADJUST_TEMP_EXTRUDE)
	{
		if(++check_temp_E>=CHECK_TEMP_TIMES)
		{
			check_temp_E=CHECK_TEMP_TIMES;
			 actExceedTemperScope(0, 0);
		}
	}
	else if(thermalManager.temp_hotend[0].current>=MAX_ADJUST_TEMP_EXTRUDE+10)
	{
		if(++check_temp_E>=CHECK_TEMP_TIMES)
		{
			check_temp_E=CHECK_TEMP_TIMES;
		    actExceedTemperScope(0, 1);
		}
	}
	else
	{
		if(check_temp_E==CHECK_TEMP_TIMES)
		{
			check_temp_E=0;
			next_window_ID=eMENU_HOME;
		}
	}
	
	if(thermalManager.temp_bed.current<=MIN_ADJUST_TEMP_BED)
	{
		if(++check_temp_Bed>=CHECK_TEMP_TIMES)
		{
			check_temp_Bed=CHECK_TEMP_TIMES;
			actExceedTemperScope(1, 0);
		}
	}
	else if(thermalManager.temp_bed.current>=MAX_ADJUST_TEMP_BED+10)
	{
		if(++check_temp_Bed>=CHECK_TEMP_TIMES)
		{
			check_temp_Bed=CHECK_TEMP_TIMES;
			actExceedTemperScope(1, 1);
		}
	}
	else
	{
		if(check_temp_Bed==CHECK_TEMP_TIMES)
		{	
			check_temp_Bed=0;
			next_window_ID=eMENU_HOME;
		}
	}	
}
extern  SAVEDATA flashsave;
void checkScreenCalibrate(void)
{
	// Serial1.print(flashsave.checksflag);
	// Serial1.println(SCREEN_CALIBRATE); 
    if(flashsave.checksflag==SCREEN_CALIBRATE)
    {
		Display_Screen.displayWindowHome();
    }
    else
    {
       enqueue_and_echo_commands_P(PSTR("M995"));
	   ConfigSettings.restoreDefaultSettings();
	   FLASH_WRITE_VAR(SAVE_ADDR_SETTINGS, re_settings);
	   ConfigSettings.settingsLoad();
    }
}
display_image::display_image(/* args */)
{
}

display_image::~display_image()
{
}
/********************************************************
 * is_bed:false->extruder0, true->bed
 * sign:  false->plus, true->minus 
 * return:  false->no change
 *********************************************************/
bool display_image::setTemperatureInWindow(bool is_bed, bool sign)
{
	if((is_bed&&thermalManager.temp_bed.current<0)||
	(thermalManager.temp_hotend[0].current<0))
		return false; 
	int16_t temp_limit;
    int16_t *p_temp;
    if(!sign)
	{      /* add */
        if(!is_bed){     /* extruder */
            temp_limit = MAX_ADJUST_TEMP_EXTRUDE;
            p_temp = &thermalManager.temp_hotend[0].target;
        }
        else{           /* bed */
            temp_limit = MAX_ADJUST_TEMP_BED;
            p_temp = &thermalManager.temp_bed.target;   
        }
        if(*p_temp < temp_limit){ /* within the limit */
            if(default_move_distance == 0xff)
                if(!is_bed){
                    if(*p_temp < NORMAL_ADJUST_TEMP_EXTRUDE)
                        *p_temp = NORMAL_ADJUST_TEMP_EXTRUDE;
                    else                     
                        *p_temp = MAX_ADJUST_TEMP_EXTRUDE;
                }
                else{
                    if(*p_temp < NORMAL_ADJUST_TEMP_BED)
                        *p_temp = NORMAL_ADJUST_TEMP_BED;
                    else                     
                        *p_temp = MAX_ADJUST_TEMP_BED;
                }
            else{   /* if distance is 1, 5, 10 */
                *p_temp += default_move_distance;
                if(*p_temp > temp_limit)
                    *p_temp= temp_limit; 
            }
            return true;
        }   
    }
	 else
	 {       /* minus */
        if(!is_bed){    /* extruder */
            temp_limit = MIN_ADJUST_TEMP_EXTRUDE; 
            p_temp = &thermalManager.temp_hotend[0].target;
        }
        else    {       /* bed */
            temp_limit = MIN_ADJUST_TEMP_BED;
            p_temp = &thermalManager.temp_bed.target;   
        }
        if(*p_temp > temp_limit){ /* within the limit */
            if(default_move_distance == 0xff)
                if(!is_bed){
                    if(*p_temp <= NORMAL_ADJUST_TEMP_EXTRUDE)
                        *p_temp = MIN_ADJUST_TEMP_EXTRUDE;
                    else                     
                        *p_temp = NORMAL_ADJUST_TEMP_EXTRUDE;
                }
                else{
                    if(*p_temp <= NORMAL_ADJUST_TEMP_BED)
                        *p_temp = MIN_ADJUST_TEMP_BED;
                    else                     
                        *p_temp = NORMAL_ADJUST_TEMP_BED;
                }       
            else{
                *p_temp -= default_move_distance;
                if(*p_temp < temp_limit)
                    *p_temp = temp_limit;  
            }
            return true;
	    }
    }
return false;
}
void display_image::changeMoveDistance(uint16_t pos_x, uint16_t pos_y)
{
		switch(default_move_distance)
		{
			default: break;
			case 1:
				default_move_distance = 5;
				displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_5);
			break;
			case 5:
				default_move_distance = 10;
				displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_10);
			break;
			case 10:
				if(current_window_ID == eMENU_EXTRUDE 
					|| current_window_ID == eMENU_ADJUST
					||current_window_ID == eMENU_ADJUST_MORE
					|| current_window_ID == eMENU_PREHEAT)
				{
					default_move_distance = 15;
					displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_MAX);
            	}
				else    /* if not in extrude or adjust menu */
				{  
					default_move_distance = 1;
					displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_1);
				}
			break;
			case 15:
				default_move_distance = 1;
				displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_1);
			break;
		}
}
/********************************************************************************
 * Function: draw lines
 * (x1,y1)：start coordinates
 * (x2,y2):end coordinates
 ********************************************************************************/
void display_image::LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1;  //Calculating coordinate increments
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //Setting one-step direction
	else if(delta_x==0)incx=0; //Vertical line
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//horizontal line
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //Selection of Basic Incremental Coordinate Axis
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )// draw Lines output
	{  
		LCD_DrawPoint(uRow,uCol);//draw points
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 
}
/********************************************************************************
 * Function: draw points
 * (x,y)：coordinate
 ********************************************************************************/
void display_image::LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);	
	LCD_WriteRAM_Prepare();	
	LCD_WriteValue_LCD_RAM(POINT_COLOR);   
}
/********************************************************************************
 * Function: draw rectangle
 * (x1,y1)：start coordinates
 * (x2,y2):end coordinates
 *********************************************************************************/
void display_image::LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
void display_image::initialMoveDistance(uint16_t pos_x, uint16_t pos_y)
{
	switch(default_move_distance)
	{
		default:   break;	
		case 1:    displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_1); break;
		case 5:	   displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_5); break;
		case 10:   displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_10); break;
        case 15:case 0xff: displayImage(pos_x, pos_y, IMG_ADDR_BUTTON_DISTANCE_MAX); break;
	}
}
void display_image::image_show(uint16_t xsta,uint16_t ysta,uint16_t width,uint16_t high,uint8_t *p)
{ 
  uint16_t i=0,k=0,x=0,y=0,real_size=0;
	uint8_t get_image_times=0;
	uint32_t image_size;
	x=xsta;
	y=ysta;
	image_size=width*high*2;
	get_image_times=image_size/GET_FLASH_SIZE;
	if((image_size-get_image_times*GET_FLASH_SIZE)>0)
	{
		get_image_times=get_image_times+1;
	}
	for(k=0;k<get_image_times;k++)
	{
		real_size=(get_image_times-k)>1?GET_FLASH_SIZE:(image_size-k*GET_FLASH_SIZE);
		W25QxxFlash.W25QXX_Read(image_buffer,(uint32_t)p+k*GET_FLASH_SIZE,real_size); 
		 LCD_SetCursor(x,y);
		 LCD_WriteRAM_Prepare();		 
		for(i=0;i<real_size;i=i+2)
		{
			POINT_COLOR=image_buffer[i]|image_buffer[i+1]<<8;
			LCD_WriteValue_LCD_RAM(POINT_COLOR);
			x++;			 
			if(x>=(xsta+width))
			{ 
			x=xsta;
			y++; 
			LCD_SetCursor(x,y);	
		    LCD_WriteRAM_Prepare();	
			}				
		}
	}  
}

void display_image::displayImage(uint16_t x_start, uint16_t y_start, uint32_t addr)
{
     HEADCOLOR *imginfo;
	 unsigned char Image_headinfo[8];
	 W25QxxFlash.W25QXX_Read(Image_headinfo, addr, 8);
    imginfo = (HEADCOLOR*) Image_headinfo;
	image_show(x_start, y_start, imginfo->w, imginfo->h, (uint8_t *)addr+8);
}
/****************************************************************************** 
 * Fill in a specified area with a single color
 * (sx,sy),(ex,ey): Filled rectangular diagonal coordinates    Area size：(ex-sx+1)*(ey-sy+1) 
 * color: Filled color
****************************************************************************** */
void display_image::LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	uint16_t i,j;
	uint16_t xlen=0;
	uint16_t temp;
	xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      	 //Setting cursor position		
			LCD_WriteRAM_Prepare();      //start writing gram	
			for(j=0;j<xlen;j++)
				LCD_WriteValue_LCD_RAM(color);  //show color
		}
}

void display_image::displayMoveCoordinate(void)
{
	for(int i=0;i<3;i++)
	{
		CLEAN_SINGLE_TXT(POS_MOVE_COL_TXT+20 + POS_MOVE_TXT_INTERVAL * i, POS_MOVE_ROW_0, 60); 
		CLEAN_STRING(s_text);
		sprintf(s_text,"%.1f",current_position[i]);
		LCD_ShowString(60 + 90 * i, 32,s_text);
	}
}

// void display_image::scanWindowAdjust( uint16_t rv_x, uint16_t rv_y )
// {

// 	if(rv_x>50&&rv_x<105&&rv_y>45&&rv_y<95)
// 	{
// 		displayWindowMove();
// 	}
// 	else if(rv_x>215&&rv_x<270&&rv_y>145&&rv_y<200)
// 	{
// 		displayWindowAdjustMore();
// 	}
// }

void display_image::scanWindowHome(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>50&&rv_x<105&&rv_y>45&&rv_y<95)
	{
		next_window_ID=eMENU_MOVE;
	}
	else if(rv_x>133&&rv_x<188&&rv_y>45&&rv_y<95)
	{
		next_window_ID=eMENU_FILE;
	}
	else if(rv_x>215&&rv_x<270&&rv_y>45&&rv_y<95)
	{
		next_window_ID=eMENU_EXTRUDE;
	}
	else if(rv_x>50&&rv_x<105&&rv_y>145&&rv_y<195)
	{
		next_window_ID=eMENU_PREHEAT;
	}
	else if(rv_x>133&&rv_x<188&&rv_y>145&&rv_y<195) //recovery
	{
		if(recovery_flag)
		{
			next_window_ID=eMENU_DIALOG_RECOVERY;
		}
	}
	else if(rv_x>215&&rv_x<270&&rv_y>145&&rv_y<195)
	{
		next_window_ID=eMENU_HOME_MORE;
	}
	
}
void display_image::scanWindowMove( uint16_t rv_x, uint16_t rv_y )
{
    if(rv_x>260&&rv_x<315&&rv_y>180&&rv_y<235)  //return home
	{
		next_window_ID=eMENU_HOME;
	}
	else if(rv_x>0&&rv_x<60&&rv_y>115&&rv_y<165) //-X move		
	{						
		current_button_id=eBT_MOVE_X_MINUS;			
	}
	else if(rv_x>115&&rv_x<175&&rv_y>115&&rv_y<165)  	//+X move
	{ 						
			 current_button_id=eBT_MOVE_X_PLUS;	
	}
	else if(rv_x>65&&rv_x<115&&rv_y>170&&rv_y<230)  	//-Y move
	{					
		 current_button_id=eBT_MOVE_Y_MINUS;	
	}
	else if(rv_x>65&&rv_x<115&&rv_y>55&&rv_y<115)  	//+Y move
	{						
		current_button_id=eBT_MOVE_Y_PLUS;
	}
	else if(rv_x>190&&rv_x<240&&rv_y>170&&rv_y<230)  //-Z move
	{						
		current_button_id=eBT_MOVE_Z_MINUS;
	}
	else if(rv_x>190&&rv_x<240&&rv_y>55&&rv_y<115)   //+Z move
	{						
		current_button_id=eBT_MOVE_Z_PLUS;
	}
	else if(rv_x>5&&rv_x<55&&rv_y>55&&rv_y<105)   //x homing
	{							
		current_button_id=eBT_MOVE_X_HOME;
	}
	else if(rv_x>125&&rv_x<175&&rv_y>55&&rv_y<105)   //y homing
	{						
		current_button_id=eBT_MOVE_Y_HOME;
	}
	else if(rv_x>125&&rv_x<175&&rv_y>180&&rv_y<230)   //z homing
	{						
		current_button_id=eBT_MOVE_Z_HOME;
	}
	else if(rv_x>5&&rv_x<55&&rv_y>180&&rv_y<230)  	//all homing
	{						
		current_button_id=eBT_MOVE_ALL_HOME;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>110&&rv_y<165)   //unlock all motors
	{						
		disable_all_steppers();
		set_all_unhomed();
	}
	else if(rv_x>260&&rv_x<315&&rv_y>55&&rv_y<95)  //select distance
	{	  					
		current_button_id=eBT_DISTANCE_CHANGE;
	}
}

void display_image::scanWindowFile( uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>260&&rv_x<315&&rv_y>176&&rv_y<226)  //return home
	{
		next_window_ID=eMENU_HOME;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>25&&rv_y<55)	//1st file
	{		
		current_button_id=eBT_FILE_LIST1;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>55&&rv_y<85)	//2nd file
	{		
		current_button_id=eBT_FILE_LIST2;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>85&&rv_y<115)	//3rd file
	{		
		current_button_id=eBT_FILE_LIST3;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>115&&rv_y<145)	//4th file
	{		
	current_button_id=eBT_FILE_LIST4;
	} 
	else if(rv_x>0&&rv_x<240&&rv_y>145&&rv_y<175)  //5th file
	{		
		current_button_id=eBT_FILE_LIST5;
	}
	else if(rv_x>35&&rv_x<90&&rv_y>180&&rv_y<235)  //last page
	{		
		current_button_id=eBT_FILE_LAST;
	}
	else if(rv_x>150&&rv_x<205&&rv_y>180&&rv_y<235)  	//next page
	{
		current_button_id=eBT_FILE_NEXT;
	}
	else if(rv_x>255&&rv_x<310&&rv_y>30&&rv_y<85)	//open file or folder	
	{							
		current_button_id=eBT_FILE_OPEN;
	}
	else if(rv_x>255&&rv_x<310&&rv_y>105&&rv_y<160)	//return parent dir
	{	
		current_button_id=eBT_FILE_FOLDER;
	}
}
void display_image::displayFilePageNumber(void)
{
	LCD_Fill(100, 195, 145, 215, White);	//celan file page number display zone
	if(file_count!=0)
	{
		CLEAN_STRING(s_text);
		POINT_COLOR=BLACK;
		sprintf((char *)s_text, "%d/%d", page_index + 1, page_index_max);
		LCD_ShowString(105, 200,s_text);
	}
}
void showfilename(uint8_t j,uint8_t i)
{
	switch (j) 
	{
		case 1:
			LCD_ShowString(35,32,CardFile.getsdfilename(i));
			if(CardFile.filenameIsFolder)
				Display_Screen.displayImage(0,25,IMG_ADDR_INDICATOR_FOLDER);
			else
				Display_Screen.displayImage(0,25,IMG_ADDR_INDICATOR_FILE);
			break;
		case 2:
			LCD_ShowString(35,62,CardFile.getsdfilename(i));
			if(CardFile.filenameIsFolder)
				Display_Screen.displayImage(0,55,IMG_ADDR_INDICATOR_FOLDER);
			else
				Display_Screen.displayImage(0,55,IMG_ADDR_INDICATOR_FILE);
		break;
		case 3:
			LCD_ShowString(35,92,CardFile.getsdfilename(i));
			if(CardFile.filenameIsFolder)
				Display_Screen.displayImage(0,85,IMG_ADDR_INDICATOR_FOLDER);
			else
				Display_Screen.displayImage(0,85,IMG_ADDR_INDICATOR_FILE);
		break;
		case 4:
			LCD_ShowString(35,122,CardFile.getsdfilename(i));
			if(CardFile.filenameIsFolder)
				Display_Screen.displayImage(0,115,IMG_ADDR_INDICATOR_FOLDER);
			else
				Display_Screen.displayImage(0,115,IMG_ADDR_INDICATOR_FILE);
		break;
		case 5:
			LCD_ShowString(35,152,CardFile.getsdfilename(i));
			if(CardFile.filenameIsFolder)
				Display_Screen.displayImage(0,145,IMG_ADDR_INDICATOR_FOLDER);
			else
				Display_Screen.displayImage(0,145,IMG_ADDR_INDICATOR_FILE);
		break;
		case 6:
		default:
		break;
	}
	//j++;
}
void display_image::displayFileList()
{
	uint8_t j=1;
	if(list_order)    //forward
	{
		for(int i=(page_index)*5;(i<page_index*5+5)&&i<file_count;i++)
		{
			showfilename(j,i);
			j++;
		}
	}
	else         // inverse
	{
		uint8_t n=0;
		for(int i=file_count-(page_index)*5-n-1;n<5&&i>=0;n++,i--)
		{
			showfilename(j,i);	
			j++;
		}	
	}

}
//display chosed file when return file window from dialog
void display_image::displayChosenFile()
{
	if(choose_printfile==-1)
		return;
	page_index=choose_file_page;
	displayWindowFiles();
	CardFile.ChoseFile(choose_printfile);
}
void display_image::displayPromptSDCardError(void)
{
	LCD_Fill(100, 195, 145, 215, White);    //clean file page number display zone
	LCD_Fill(0, 25, 239, 174,White);	//clean file list display zone 
	displayImage(45, 85, IMG_ADDR_PROMPT_ERROR);
	color=RED;	
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s", TXT_MENU_FILE_SD_ERROR);
	LCD_ShowString(80, 92,s_text);
	color=Black;
	clearfilevar();
}
void display_image::displayPromptEmptyFolder(void)
{
    LCD_Fill(100, 195, 145, 215, White);    //clean file page number display zone
	LCD_Fill(0, 25, 239, 174,White);	//clean file list display zone 
	color = Gray;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s", TXT_MENU_FILE_EMPTY);
	LCD_ShowString(35,87,s_text); 
	color = BLACK;
}

void display_image::scanWindowExtrude( uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>260&&rv_x<315&&rv_y>176&&rv_y<226)  //return home
	{
		if(extrude2file)
		{
			extrude2file=false;
			next_window_ID=eMENU_FILE1;
			return;
		}
		if(is_printing)
			next_window_ID=eMENU_ADJUST_MORE;
		else
			next_window_ID=eMENU_HOME;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>34&&rv_y<89) //add extruder/bed temperature
	{				
		current_button_id=eBT_TEMP_PLUS;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>176&&rv_y<231)   //subtract extruder/bed temperature
	{				
		current_button_id=eBT_TEMP_MINUS;
	}
	else if(rv_x>15&&rv_x<65&&rv_y>95&&rv_y<136)   //select bed/extruder
	{				
			current_button_id=eBT_BED_E;		
	}
	else if(rv_x>85&&rv_x<140&&rv_y>35&&rv_y<100)  //+e move
	{	
		current_button_id=eBT_JOG_EPLUS;
	}
	else if(rv_x>85&&rv_x<140&&rv_y>165&&rv_y<230)  //-e move
	{		
		current_button_id=eBT_JOG_EMINUS;
	}
	else if(rv_x>167&&rv_x<237&&rv_y>45&&rv_y<100)  //autofeed in positive direction 
	{			
		current_button_id=eBT_AUTO_EPLUS;
	}
	else if(rv_x>167&&rv_x<237&&rv_y>165&&rv_y<220) //autofeed in negative direction 
	{	
		current_button_id=eBT_AUTO_EMINUS;		
	}
	else if(rv_x>260&&rv_x<315&&rv_y>40&&rv_y<80)  //change distance
	{	
		current_button_id=eBT_DISTANCE_CHANGE;			
	}
	else if(rv_x>260&&rv_x<315&&rv_y>100&&rv_y<155) //stop autofeed
	{	
		current_button_id=eBT_STOP;		
	}
}
void display_image::dispalyExtrudeTemp(void)
{
	LCD_Fill(5,143,65,163,White);		//clean extruder/bed temperature display zone
	POINT_COLOR=BLACK;
	CLEAN_STRING(s_text);
	if(!is_bed_select)
		sprintf((char *)s_text,"%d/%d",(int16_t)thermalManager.temp_hotend[0].current, thermalManager.temp_hotend[0].target);

	else{
		sprintf((char *)s_text,"%d/%d", (int16_t)thermalManager.temp_bed.current,thermalManager.temp_bed.target);
	}
	LCD_ShowString(8,143,s_text);

}
void display_image::displayRunningAutoFeed(void)
{
	if(dir_auto_feed==0) return;
	static bool is_display_run_auto_feed = false;
	if(!is_display_run_auto_feed)
	{		
		if(dir_auto_feed == 1)
		{			
			LCD_Fill(167,96,234,99,White);	 //clean partial feed in display zone		
			displayImage(167, 41, IMG_ADDR_BUTTON_FEED_IN_1);
		}
		else
		{		
			LCD_Fill(167,166,234,169,White); //clean partial feed out display zone			
			displayImage(167, 169, IMG_ADDR_BUTTON_FEED_OUT_1);	
		}
	}
	else
	{
		if(dir_auto_feed == 1)
		{		
			LCD_Fill(167,41,234,44,White);	 //clean partial feed in display zone					
			displayImage(167, 44, IMG_ADDR_BUTTON_FEED_IN_0);
		}
		else
		{
			LCD_Fill(167,221,234,224,White); //clean partial feed out display zone	
			displayImage(167, 166, IMG_ADDR_BUTTON_FEED_OUT_0);

		}
	}
	is_display_run_auto_feed = !is_display_run_auto_feed;
}

void display_image::scanWindowPreheating( uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>260&&rv_x<315&&rv_y>160&&rv_y<215)  //return home
	{
		next_window_ID=eMENU_HOME;
	}
	 else if(rv_x>10&&rv_x<65&&rv_y>30&&rv_y<85)  /* add extruder0 temperature  */
	{			

		current_button_id=eBT_PR_E_PLUS;
	}
   else if(rv_x>180&&rv_x<235&&rv_y>30&&rv_y<85)   /* subtract extruder0 temperature */
   { 		   
	    current_button_id= eBT_PR_E_MINUS;
   }
	else if(rv_x>10&&rv_x<65&&rv_y>95&&rv_y<150)  /* add bed temperature  */
	{			
		current_button_id=eBT_PR_B_PLUS;
	}
    else if(rv_x>180&&rv_x<235&&rv_y>95&&rv_y<150)  /* subtract bed temperature */
	{ 		
		current_button_id=EBT_PR_B_MINUS;
    }
    else if(rv_x>260&&rv_x<315&&rv_y>37&&rv_y<77)      /* change distance */	
	{	      				
		current_button_id=eBT_DISTANCE_CHANGE;
	}    
    else if(rv_x>260&&rv_x<315&&rv_y>95&&rv_y<150)   /* cooling down */
	{		  
		current_button_id=eBT_PR_COOL;
	}
    else if(rv_x>10&&rv_x<65&&rv_y>160&&rv_y<215)   /* filament 0 PLA */
	{	      					
		current_button_id=eBT_PR_PLA;
	}  
    else if(rv_x>95&&rv_x<150&&rv_y>160&&rv_y<215)  /* filament 1 ABS */
	{	       					
		current_button_id=eBT_PR_ABS;
	} 				
    else if(rv_x>180&&rv_x<235&&rv_y>160&&rv_y<215)   /* filament 2 PETG */
	{	   					
		current_button_id=eBT_PR_PETG;
	} 
}
void display_image::updatePreheatingTemp(void)
{
	LCD_Fill(110,49,170,69,White);		//clean extruder temperature display zone
	LCD_Fill(110,114,170,134,White);		//clean bed temperature display zone
	POINT_COLOR=BLACK;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%d/%d",(int16_t)thermalManager.temp_hotend[0].current,thermalManager.temp_hotend[0].target);
	LCD_ShowString(110,49,s_text);
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%d/%d",(int16_t)thermalManager.temp_bed.current,thermalManager.temp_bed.target);
	LCD_ShowString(110,114,s_text);
}

void display_image::scanWindowMoreHome(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>50&&rv_x<105&&rv_y>45&&rv_y<95)
	{
		next_window_ID=eMENU_LEVELING;
	}
	else if(rv_x>133&&rv_x<188&&rv_y>45&&rv_y<95)
	{
		next_window_ID=eMENU_SETTINGS;
	}
	else if(rv_x>215&&rv_x<270&&rv_y>45&&rv_y<95)
	{
		next_window_ID=eMENU_ABOUT;
	}
	else if(rv_x>215&&rv_x<270&&rv_y>145&&rv_y<195)  //return home
	{
		next_window_ID=eMENU_HOME;
	}
}
void display_image::scanWindowLeveling( uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>245&&rv_x<300&&rv_y>165&&rv_y<220)   /* return */
	{			
		next_window_ID=eMENU_HOME_MORE;
		current_button_id=eBT_MOVE_RETURN;   //Z up 10mm
	}
    else if(rv_x>20&&rv_x<75&&rv_y>45&&rv_y<100)  /* 0 top left */
	{		
		current_button_id=eBT_MOVE_L0;
	}
    else if(rv_x>160&&rv_x<215&&rv_y>45&&rv_y<100)   /* 1 top right */
	{	  						
		current_button_id=eBT_MOVE_L1;
	}
    else if(rv_x>160&&rv_x<215&&rv_y>165&&rv_y<220) /* 2 bottom right */	
	{					
		current_button_id=eBT_MOVE_L2;
	}    
    else if(rv_x>20&&rv_x<75&&rv_y>165&&rv_y<220) /* 3 bottom left */	
	{							
		current_button_id=eBT_MOVE_L3;
	}
    else if(rv_x>90&&rv_x<145&&rv_y>105&&rv_y<160) /* 4 center */		
	{				
		current_button_id=eBT_MOVE_L4;
	}  
    else if(rv_x>245&&rv_x<300&&rv_y>45&&rv_y<100) /* unlock x and y  */
	{		
		enqueue_and_echo_commands_P(PSTR("M84 X0 Y0"));
		set_all_unhomed();
	}	
}
void display_image::scanWindowAbout(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>260&&rv_x<315&&rv_y>153&&rv_y<208)  	//select return 
	{	
		next_window_ID=eMENU_HOME_MORE;
	}
}
void display_image::displayWindowLeveling(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_LEVELING); 	//caption background
	#ifndef Chinese
   		 displayImage(115, 5, IMG_ADDR_CAPTION_LEVELING);     //caption words
	#else
		 displayImage(115, 5, IMG_ADDR_CAPTION_LEVELING_CN);     //caption words
	#endif
	/* rectangular frame*/
   LCD_DrawRectangle(48, 73, 48+140, 73+121);
   /* icons showing */
    displayImage(20, 45, IMG_ADDR_BUTTON_LEVELING0);     //top left
    displayImage(160, 45, IMG_ADDR_BUTTON_LEVELING1);    //top right 
    displayImage(160, 165, IMG_ADDR_BUTTON_LEVELING2);   //bottom right
    displayImage(20, 165, IMG_ADDR_BUTTON_LEVELING3);    //bottom left
    displayImage(90, 105, IMG_ADDR_BUTTON_LEVELING4);    //center
    displayImage(245, 45, IMG_ADDR_BUTTON_UNLOCK);  
    displayImage(245, 165, IMG_ADDR_BUTTON_RETURN); 
  	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_LEVELING_UNLOCK);
    LCD_ShowString(235,109,s_text);
}
void display_image::displayWindowAbout(void)
{	
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_ABOUT); 		//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_ABOUT);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_ABOUT_CN);		//caption words
	#endif
	displayImage(259, 153, IMG_ADDR_BUTTON_RETURN);
	POINT_COLOR = PT_COLOR_DISABLE;
	LCD_DrawLine(0, 83, 240, 83);	
	LCD_DrawLine(0, 133, 240, 133); 
	POINT_COLOR = BLACK;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MORE_RETURN);
	LCD_ShowString(263,212,s_text);	
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_ABOUT_MAX_SIZE_LABEL);
	LCD_ShowString(10,40,s_text);
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",MAC_SIZE);
	LCD_ShowString(10,60,s_text);
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_ABOUT_FW_VER_LABLE);
	LCD_ShowString(10,90,s_text);		
	CLEAN_STRING(s_text);	
//	sprintf((char *)s_text, "%s %s %s", FW_VERSION, __TIME__, __DATE__ );
	sprintf((char *)s_text, "%s", FW_VERSION);
	LCD_ShowString(10, 110,s_text);
}

static char s_argus[10];     /* used to temporarily store value */
const char *c_machine_argument[ARGUMENST_MAX_NUM]={
	
	TXT_MENU_SETTS_ACCL, //"Accel(mm/s^2):",
	TXT_MENU_SETTS_JERK_XY,//"Vxy-jerk(mm/s):",
	TXT_MENU_SETTS_JERK_Z,//"Vz-jerk(mm/s):",
	TXT_MENU_SETTS_JERK_E,//"Ve-jerk(mm/s):",
	TXT_MENU_SETTS_VMAX_X,//"Vmax x(mm/s):",
	TXT_MENU_SETTS_VMAX_Y,//"Vmax y(mm/s):",
	TXT_MENU_SETTS_VMAX_Z,//"Vmax z(mm/s):",
	TXT_MENU_SETTS_VMAX_E,//"Vmax e(mm/s):",
	TXT_MENU_SETTS_VMIN,//"Vmin(mm/s):",
	TXT_MENU_SETTS_VTRAVEL,//"Vtrav min(mm/s):",
	TXT_MENU_SETTS_AMAX_X,//"Amax x(mm/s^2):",
	TXT_MENU_SETTS_AMAX_Y,//"Amax y(mm/s^2):",	
	TXT_MENU_SETTS_AMAX_Z,//"Amax z(mm/s^2):",
	TXT_MENU_SETTS_AMAX_E,//"Amax e(mm/s^2):",
	TXT_MENU_SETTS_ARETRACT,//"A-retract(mm/s^2):",
//	"A-travel:",
	TXT_MENU_SETTS_STEP_X,//"X(steps/mm):",	
	TXT_MENU_SETTS_STEP_Y,//"Y(steps/mm):",	
	TXT_MENU_SETTS_STEP_Z,//"Z(steps/mm):",	
	TXT_MENU_SETTS_STEP_E,//"E(steps/mm):",
	TXT_MENU_SETTS_CHECK_FILA,//"Filament check:",
	TXT_MENU_SETTS_LIST_ORDER,//"File list order:"
};
void convertArgu2Str(uint8_t i, char* p)
{
	switch (i)
	{
		default: break;
		case 0:
			sprintf(p,"%8.2f", re_settings.acceleration);
			break;		
		case 1:
			sprintf(p,"%8.2f", re_settings.max_xy_jerk);
			break;				
		case 2:
			sprintf(p,"%8.2f", re_settings.max_z_jerk);
			break;				
		case 3:
			sprintf(p,"%8.2f", re_settings.max_e_jerk);
			break;				
		case 4:
			sprintf(p,"%8.2f", re_settings.max_feedrate[0]);
			break;				
		case 5: 
			sprintf(p,"%8.2f", re_settings.max_feedrate[1]);
			break;	
		case 6:
			sprintf(p,"%8.2f", re_settings.max_feedrate[2]);
			break;				
		case 7:
			sprintf(p,"%8.2f", re_settings.max_feedrate[3]);
			break;				
		case 8: 
			sprintf(p,"%8.2f", re_settings.minimumfeedrate);
			break;				
		case 9:
			sprintf(p,"%8.2f", re_settings.mintravelfeedrate);
			break;
		#ifndef Chinese 				
		case 10:
			sprintf(p,"%8lu", re_settings.max_acceleration_units_per_sq_second[0]);
			break;				
		case 11: 
			sprintf(p,"%8lu", re_settings.max_acceleration_units_per_sq_second[1]);
			break;				
		case 12:
			sprintf(p,"%8lu", re_settings.max_acceleration_units_per_sq_second[2]);
			break;				
		case 13:
			sprintf(p,"%8lu", re_settings.max_acceleration_units_per_sq_second[3]);
			break;
		#else
			case 10:
			sprintf(p,"%6lu", re_settings.max_acceleration_units_per_sq_second[0]);
			break;				
		case 11: 
			sprintf(p,"%6lu", re_settings.max_acceleration_units_per_sq_second[1]);
			break;				
		case 12:
			sprintf(p,"%6lu", re_settings.max_acceleration_units_per_sq_second[2]);
			break;				
		case 13:
			sprintf(p,"%6lu", re_settings.max_acceleration_units_per_sq_second[3]);
			break;
		#endif				
		case 14: 
			sprintf(p,"%8.2f", re_settings.retract_acceleration);
			break;	
		case 15:
			sprintf(p,"%8.2f", re_settings.axis_steps_per_unit[0]);
			break;				
		case 16:
			sprintf(p,"%8.2f", re_settings.axis_steps_per_unit[1]);
			break;				
		case 17: 
			sprintf(p,"%8.2f", re_settings.axis_steps_per_unit[2]);
			break;				
		case 18:
			sprintf(p,"%8.2f", re_settings.axis_steps_per_unit[3]);
			break;	
		#ifndef Chinese
			case 19:
			if(re_settings.is_filament_check_disable)
				sprintf(p,"%8s", TXT_MENU_SETTS_VALUE_OFF);
            else
				sprintf(p,"%8s", TXT_MENU_SETTS_VALUE_ON);

			break;
			case 20:
        	if(re_settings.file_order)
				sprintf(p,"%8s", TXT_MENU_SETTS_VALUE_FORWARD);
            else
				sprintf(p,"%8s", TXT_MENU_SETTS_VALUE_INVERSE);

        	break;  
		#else
			case 19:
			if(re_settings.is_filament_check_disable)
				sprintf(p,"%5s", TXT_MENU_SETTS_VALUE_OFF);
            else
				sprintf(p,"%5s", TXT_MENU_SETTS_VALUE_ON);

			break;
			case 20:
        	if(re_settings.file_order)
				sprintf(p,"%5s", TXT_MENU_SETTS_VALUE_FORWARD);
            else
				sprintf(p,"%5s", TXT_MENU_SETTS_VALUE_INVERSE);

        	break;  
		#endif			
	}
	return;
}
uint8_t choose_setting=ARGUMENST_MAX_NUM;
void ChoseArgument(uint8_t index)
{
	uint8_t i=index+page_index*5;
	if(i >= ARGUMENST_MAX_NUM) 
		return;
	choose_setting=i;
	Display_Screen.LCD_Fill(0, 25+page_index_num*30, 239, 55 - 1 + page_index_num*30,White);		//clean file last file display zone 
	page_index_num = index;
	Display_Screen.displayArgumentList();						
	Display_Screen.LCD_Fill(0, 25+page_index_num*30, 239, 55 - 1 + page_index_num*30,DARKBLUE);
    //page_index_num=index;
	memset(s_argus, 0, sizeof(s_argus));
	convertArgu2Str(i, s_argus);
	sprintf((char*)s_text, "%-20s%s", c_machine_argument[i], s_argus);
    color=WHITE,bgColor=DARKBLUE;
    LCD_ShowString(10,32+page_index_num*30,s_text);   //page_index start from 0
    color=BLACK,bgColor=WHITE;
}
void *getArgumentAddr(uint8_t i)
{
	switch(i)
	{
		default: return 0;
		case 0:
			return &re_settings.acceleration;	
		case 1:
			return &re_settings.max_xy_jerk;		
		case 2:
			return &re_settings.max_z_jerk;			
		case 3:
			return &re_settings.max_e_jerk;		
		case 4:
			return &re_settings.max_feedrate[0];			
		case 5: 
			return &re_settings.max_feedrate[1];
		case 6:
			return &re_settings.max_feedrate[2];			
		case 7:
			return &re_settings.max_feedrate[3];
		case 8: 
			return &re_settings.minimumfeedrate;
		case 9:
			return &re_settings.mintravelfeedrate;
		case 10:
			return &re_settings.max_acceleration_units_per_sq_second[0];
		case 11: 
			return &re_settings.max_acceleration_units_per_sq_second[1];
		case 12:
			return &re_settings.max_acceleration_units_per_sq_second[2];
		case 13:
			return &re_settings.max_acceleration_units_per_sq_second[3];
		case 14: 
			return &re_settings.retract_acceleration;
		case 15:
			return &re_settings.axis_steps_per_unit[0];
		case 16:
			return &re_settings.axis_steps_per_unit[1];
		case 17: 
			return &re_settings.axis_steps_per_unit[2];
		case 18:
			return &re_settings.axis_steps_per_unit[3];
		case 19:
			return &re_settings.is_filament_check_disable;
        case 20:
            return &re_settings.file_order;
	}
}
float getDistanceMultiplier(uint8_t i)
{
	switch(i){
		default:
			return 0.0; 
		case 2: case 15: case 16: case 17: case 18:	
			return 0.1;
		case 1:	case 3:case 4: case 5: case 6: case 7:
		case 8: case 9: case 12:
			return 1.0;
		case 0: case 10: case 11: case 13: case 14:
			return 100.0;
	}
}
bool is_setting_change=false;
void changeValueArgument(uint8_t i,int8_t inc_sign)
{
	if(i >= ARGUMENST_MAX_NUM){			/* error index */
		return;
	}
	else if(i >= 19)  	/* bool type */
	{				
		*(bool *)getArgumentAddr(i) = !(*(bool *)getArgumentAddr(i));
		return;
	}
	else if(i >= 10 && i<= 13)  /* unsigned long type */
	{		
		*(unsigned long *)getArgumentAddr(i) = *(unsigned long *)getArgumentAddr(i) +
			inc_sign * default_move_distance * (unsigned long)getDistanceMultiplier(i);
		if((long)*(unsigned long *)getArgumentAddr(i) < 0) *(unsigned long *)getArgumentAddr(i) = 0;	//minimum value
	}
	else if(i >= 15 && i<= 18) /* float type */
	{		
		*(float *)getArgumentAddr(i) = *(float *)getArgumentAddr(i) + inc_sign * default_move_distance * getDistanceMultiplier(i);
		if(*(float *)getArgumentAddr(i) < 0.0) *(float *)getArgumentAddr(i) = 0.0;		//minimum value
	}
	else /* float type */
	{								
		*(float *)getArgumentAddr(i) = *(float *)getArgumentAddr(i) + inc_sign * default_move_distance * getDistanceMultiplier(i);
		if(*(float *)getArgumentAddr(i) < 0.0) *(float *)getArgumentAddr(i) = 0.0;	//minimum value
		return;
	}
	ConfigSettings.calcAccelRates();
}
void display_image::displayArugumentPageNumber(void)
{
	LCD_Fill(69, 195, 94, 215, WHITE);	//celan file page display zone
	CLEAN_STRING(s_text);
	color=BLACK;
	sprintf((char *)s_text, "%d/%d", page_index + 1, ARGUMENST_MAX_PAGE + 1);
	LCD_ShowString(69, 200,s_text);
}
void display_image::displayArgumentList(void)
{
	uint8_t i,j=0;
	color = BLACK;
	for(i = page_index*5; i < page_index*5 +5; i++, j++)
	{
		if(i >=  ARGUMENST_MAX_NUM)	return;		//avoid display unnecessary strings 
		memset(s_text, 0, sizeof(s_text));	
        memset(s_argus, 0, sizeof(s_argus));
		convertArgu2Str(i, s_argus);
        sprintf((char*)s_text, "%-20s%s", c_machine_argument[i], s_argus);
        LCD_ShowString(10, 32 + 30*j,s_text);
	}
}
void display_image::displayWindowSettings(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_SETTINGS); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_SETTINGS);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_SETTINGS_CN);		//caption words
	#endif
	displayImage(255, 30, IMG_ADDR_BUTTON_MODIFY);
	displayImage(255, 105, IMG_ADDR_BUTTON_RESTORE);
	displayImage(178, 180, IMG_ADDR_BUTTON_SAVE);
	displayImage(5, 180, IMG_ADDR_BUTTON_PAGE_LAST);
	displayImage(101, 180, IMG_ADDR_BUTTON_PAGE_NEXT);
	displayImage(255, 180, IMG_ADDR_BUTTON_RETURN);	
	//draw frame
	POINT_COLOR=DARKBLUE;	
	LCD_DrawLine(0, 175, 240, 175);	
	LCD_DrawLine(0, 176, 240, 176);
	LCD_DrawLine(240, 175, 240, 25);
	LCD_DrawLine(241, 176, 241, 25);
	displayArgumentList();
	displayArugumentPageNumber();
}
void display_image::scanWindowSettings(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>255&&rv_x<315&&rv_y>180&&rv_y<240) //return
	{			
		next_window_ID=eMENU_HOME_MORE;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>25&&rv_y<55) //1st 
	{		
		//ChoseArgument(0);
		current_button_id=eBT_FILE_LIST1;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>55&&rv_y<85) //2nd 
	{		
		//ChoseArgument(1);
		current_button_id=eBT_FILE_LIST2;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>85&&rv_y<115) //3rd 
	{		
		//ChoseArgument(2);	
		current_button_id=eBT_FILE_LIST3;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>115&&rv_y<145) //4th 
	{		
		//ChoseArgument(3);	
		current_button_id=eBT_FILE_LIST4;
	}
	else if(rv_x>0&&rv_x<240&&rv_y>145&&rv_y<175)  //5th 
	{		
		//ChoseArgument(4);	
		current_button_id=eBT_FILE_LIST5;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>180&&rv_y<235)  	//last page
	{	
		current_button_id=eBT_SETTING_LAST;
	}
	else if(rv_x>100&&rv_x<155&&rv_y>180&&rv_y<235) //next page
	{	
		current_button_id=eBT_SETTING_NEXT;
	}
	else if(rv_x>255&&rv_x<315&&rv_y>30&&rv_y<85)   //modify
	{	
		current_button_id=eBT_SETTING_ADJUST;
	}
	else if(rv_x>255&&rv_x<315&&rv_y>105&&rv_y<160)  //restore
	{	
		current_button_id=eBT_SETTING_REFACTORY;
	}	
	else if(rv_x>178&&rv_x<233&&rv_y>180&&rv_y<235)  //save	
	{
		if(is_setting_change)
			current_button_id=eBT_SETTING_SAVE;
	}
}
void display_image::displayWindowSettings2(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_SETTINGS); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_SETTINGS);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_SETTINGS_CN);		//caption words
	#endif
	initialMoveDistance(255, 43);	
	displayImage(150, 180, IMG_ADDR_BUTTON_SUB);
	displayImage(35, 180, IMG_ADDR_BUTTON_ADD);
	displayImage(255, 180, IMG_ADDR_BUTTON_RETURN);	
	displayModifyArgument();
}
void display_image::displayModifyArgument(void)
{
	LCD_Fill(170, 100, 240, 120, WHITE);	//celan value  display zone
	POINT_COLOR = BLACK;
    memset(s_text, 0, sizeof(s_text));  
    memset(s_argus, 0, sizeof(s_argus));
    convertArgu2Str(choose_setting, s_argus);    
    sprintf((char*)s_text, "%-20s%s", c_machine_argument[choose_setting], s_argus);
    LCD_ShowString(10, 100,s_text);

}
void display_image::scanWindowSettings2(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>255&&rv_x<315&&rv_y>180&&rv_y<240) //return
	{			
		next_window_ID=eMENU_SETTINGS;
		page_index=(choose_setting-page_index_num)/5;
		choose_setting=ARGUMENST_MAX_NUM;
	}
	else if(rv_x>35&&rv_x<90&&rv_y>180&&rv_y<235)  //add
	{		
		current_button_id=eBT_SETTING_ADD;
	}
	else if(rv_x>150&&rv_x<205&&rv_y>180&&rv_y<235)  //subs
	{		
		current_button_id=eBT_SETTING_SUB;
	}else if(rv_x>255&&rv_x<315&&rv_y>43&&rv_y<83)  //distance
	{		
	    current_button_id=eBT_DISTANCE_CHANGE;
	}
}
void display_image::displayStartUpLogo(void)
{
  lcdClear(White);
  #if defined(U30) || defined(U20) || defined(U20_PLUS) 
  	displayImage(60, 95, IMG_ADDR_STARTUP_LOGO_0);
  #elif defined(LK1_PLUS) ||  defined(LK1) || defined(LK2) || defined(LK4)  
	displayImage(60, 95, IMG_ADDR_STARTUP_LOGO_2);
  #endif
}
void display_image::displayWindowHome(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_HOME); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_HOME);		//caption words  
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_HOME_CN);
	#endif
	displayImage(50, 45, IMG_ADDR_BUTTON_MOVE);
	displayImage(133, 45, IMG_ADDR_BUTTON_FILE);
	displayImage(215, 45, IMG_ADDR_BUTTON_EXTRUDE);
	displayImage(50, 145, IMG_ADDR_BUTTON_PREHEAT);
	if(false==recovery_flag)
	{
		displayImage(133, 145, IMG_ADDR_BUTTON_RECOVERY_DISABLE);
		color=PT_COLOR_DISABLE;
		CLEAN_STRING(s_text);
		sprintf((char*)s_text,"%s",TXT_MENU_HOME_RECOVERY);
		LCD_ShowString(129,209,s_text);
	}
	else
	{
		displayImage(133, 145, IMG_ADDR_BUTTON_RECOVERY);
		color=BLACK;
		CLEAN_STRING(s_text);
		sprintf((char*)s_text,"%s",TXT_MENU_HOME_RECOVERY);
		LCD_ShowString(129,209,s_text);	
	}
	displayImage(215, 145, IMG_ADDR_BUTTON_MORE);
	color=BLACK;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MORE);
	LCD_ShowString(227,209,s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_FILE);
	LCD_ShowString(142,109,s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_EXTRUDE);
	LCD_ShowString(216,109,s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MOVE);
	#ifndef Chinese
		LCD_ShowString(41,109,s_text);
	#else
		LCD_ShowString(62,109,s_text);
	#endif
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_PREHEAT);
	#ifndef Chinese
		LCD_ShowString(39,209,s_text);
	#else
		LCD_ShowString(62,209,s_text);
	#endif
}

void display_image::displayWindowMove(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 25, BG_COLOR_CAPTION_MOVE); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_MOVE);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_MOVE_CN);		//caption words
	#endif
	displayImage(5, 180, IMG_ADDR_BUTTON_HOME_ALL);
	displayImage(5, 55, IMG_ADDR_BUTTON_HOME_X);
	displayImage(125, 55, IMG_ADDR_BUTTON_HOME_Y);
	displayImage(125, 180, IMG_ADDR_BUTTON_HOME_Z);
	displayImage(0, 118, IMG_ADDR_BUTTON_MINUS_X);
	displayImage(65, 170, IMG_ADDR_BUTTON_MINUS_Y);
	displayImage(193, 170, IMG_ADDR_BUTTON_MINUS_Z);
	displayImage(115, 118, IMG_ADDR_BUTTON_PLUS_X);
	displayImage(65, 55, IMG_ADDR_BUTTON_PLUS_Y);
	displayImage(193, 55, IMG_ADDR_BUTTON_PLUS_Z);
    default_move_distance = 5;		//default distance
	initialMoveDistance(260, 55);	
	displayImage(260, 110, IMG_ADDR_BUTTON_UNLOCK);
	displayImage(260, 180, IMG_ADDR_BUTTON_RETURN);
	POINT_COLOR=BLUE;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s","X:");
	LCD_ShowString(40,32,s_text);
	sprintf((char*)s_text,"%s","Y:");
	LCD_ShowString(130,32,s_text);
	sprintf((char*)s_text,"%s","Z:");
	LCD_ShowString(220,32,s_text);	
	displayMoveCoordinate();
}
void display_image::displayWindowFiles(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_FILE); 	//caption background
	#ifndef Chinese
    	displayImage(115, 5, IMG_ADDR_CAPTION_FILE);		//caption words  
		displayImage(255, 30, IMG_ADDR_BUTTON_OPEN);
	#else
		displayImage(115, 5,  IMG_ADDR_CAPTION_FILE_CN);		//caption words
		displayImage(255, 30, IMG_ADDR_BUTTON_OPEN_CN);
	#endif
	displayImage(255, 105, IMG_ADDR_BUTTON_RETURN_FOLDER);
	displayImage(150, 180, IMG_ADDR_BUTTON_PAGE_NEXT);
	displayImage(35, 180, IMG_ADDR_BUTTON_PAGE_LAST);
	displayImage(255, 180, IMG_ADDR_BUTTON_RETURN);	
	// draw frame
	POINT_COLOR=DARKBLUE;	
	LCD_DrawLine(0, 175, 240, 175);
	LCD_DrawLine(0, 176, 240, 176);
	LCD_DrawLine(240, 175, 240, 25);
	LCD_DrawLine(241, 176, 241, 25);

	if((SDIO_GetCardState()==SDIO_CARD_ERROR))
		displayPromptSDCardError();
	else
	{
		file_count=card.getnrfilenames();
		if(file_count==0)
		{
			displayPromptEmptyFolder();
			return;
		}
		page_index_max=CardFile.getsdfilepage();
		displayFileList();
		displayFilePageNumber();
	}
}
void display_image::displayWindowExtrude(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_EXTRUDE); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_EXTRUDE);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_EXTRUDE_CN);		//caption words
	#endif
	displayImage(5, 34, IMG_ADDR_BUTTON_ADD);
	displayImage(5, 176, IMG_ADDR_BUTTON_SUB);
	displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);

	displayImage(86, 34, IMG_ADDR_BUTTON_PLUS_E);
	displayImage(86, 166, IMG_ADDR_BUTTON_MINUS_E);	
	displayImage(167, 44, IMG_ADDR_BUTTON_FEED_IN_0);
	displayImage(167, 166, IMG_ADDR_BUTTON_FEED_OUT_0);
	default_move_distance = 10;
	initialMoveDistance(260, 41);
	#ifndef Chinese
		displayImage(260, 101, IMG_ADDR_BUTTON_FEED_STOP);
	#else
		displayImage(260, 101, IMG_ADDR_BUTTON_FEED_STOP_CN);
	#endif
	displayImage(260, 176, IMG_ADDR_BUTTON_RETURN);
	POINT_COLOR = 0xC229;
	LCD_DrawRectangle(96, 121, 134, 140);	//jog frame //97  126
	POINT_COLOR = BLUE;	
	LCD_DrawRectangle(180, 121, 219, 140);	//auto frame
	POINT_COLOR=BLACK;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_EXTRUDE_MANUAL);
	LCD_ShowString(100,123,s_text);	
	sprintf((char*)s_text,"%s",TXT_MENU_EXTRUDE_AUTOMATIC);
	LCD_ShowString(184,123,s_text);
	dispalyExtrudeTemp();
}
void display_image::displayWindowPreheat(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_PREHEAT); 	//caption background
	#ifndef Chinese
    	displayImage(115, 5, IMG_ADDR_CAPTION_PREHEAT);     //caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_PREHEAT_CN);     //caption words
	#endif
	displayImage(10, 30, IMG_ADDR_BUTTON_ADD);
	displayImage(10, 95, IMG_ADDR_BUTTON_ADD);    
	displayImage(180, 30, IMG_ADDR_BUTTON_SUB);
	displayImage(180, 95, IMG_ADDR_BUTTON_SUB);
    displayImage(75, 42, IMG_ADDR_INDICATOR_HEAD);
    displayImage(75, 107, IMG_ADDR_INDICATOR_BED);
	default_move_distance = 10;
	initialMoveDistance(260, 37);
    displayImage(260, 95, IMG_ADDR_BUTTON_COOLING);
    displayImage(10, 160, IMG_ADDR_BUTTON_FILAMENT_2);
    displayImage(95, 160, IMG_ADDR_BUTTON_FILAMENT_0);   
    displayImage(180, 160, IMG_ADDR_BUTTON_FILAMENT_1);      
    displayImage(260, 160, IMG_ADDR_BUTTON_RETURN);    
	POINT_COLOR=BLACK;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s","PLA");
    LCD_ShowString(25,217,s_text);
	sprintf((char*)s_text,"%s","ABS");
    LCD_ShowString(109,217,s_text);
	sprintf((char*)s_text,"%s","PETG");
    LCD_ShowString(191,217,s_text);
    updatePreheatingTemp();
}
void display_image::displayWindowHomeMore(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 24, BG_COLOR_CAPTION_HOME); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_HOME);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_HOME_CN);
	#endif
	displayImage(50, 45, IMG_ADDR_BUTTON_LEVELING);
	displayImage(133, 45, IMG_ADDR_BUTTON_SETTINGS);
	displayImage(215, 45, IMG_ADDR_BUTTON_ABOUT);
	displayImage(215, 145, IMG_ADDR_BUTTON_RETURN);
	POINT_COLOR = BLACK;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MORE_ABOUT);
	LCD_ShowString(224,109,s_text);	
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MORE_RETURN);
	LCD_ShowString(220,209,s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MORE_LEVELING);
   	LCD_ShowString(46,109,s_text); 
	sprintf((char*)s_text,"%s",TXT_MENU_HOME_MORE_SETTINGS);
	LCD_ShowString(130,109,s_text);
}

void display_image::scanWindowPrint( uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>260&&rv_x<315&&rv_y>30&&rv_y<85) //pause or resume
	{		
		current_button_id=eBT_PRINT_PAUSE;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>105&&rv_y<160)  //adjust
	{	
		current_button_id=eBT_PRINT_ADJUST;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>180&&rv_y<235) //end
	{	
		current_button_id=eBT_PRINT_END;
	}
}
void display_image::displayWindowPrint(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 25, BG_COLOR_CAPTION_PRINT); 	//caption background	
	//display file name
	bgColor = BG_COLOR_CAPTION_PRINT;
	color=WHITE;
//	LCD_ShowString(10,5,CardFile.getprintfilename());	
	if(recovery_flag==false)
		LCD_ShowString(10,5,card.longFilename);
	else
		;
	bgColor = WHITE;
	color=BLACK;
	displayImage(10, 30, IMG_ADDR_INDICATOR_HEAD);	
	displayImage(140, 30, IMG_ADDR_INDICATOR_FAN_0);	
	displayImage(10, 70, IMG_ADDR_INDICATOR_BED);	
	displayImage(144, 70, IMG_ADDR_INDICATOR_HEIGHT);	
	displayImage(10, 150, IMG_ADDR_INDICATOR_TIMER_CD);	
	displayImage(140, 150, IMG_ADDR_INDICATOR_TIMER_CU);	
	LCD_Fill(10,110,200,140,LIGHTBLUE); 	//progress bar background
//	if(!cur_flag)
	{
		switch(cur_ppage)
		{
			case 0:
				displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE_DISABLE); 
				current_print_cmd=E_PRINT_DISPAUSE;	
				cur_pstatus=0;		
			break;
			case 1:   //printing
				displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE);
				cur_pstatus=1;
				//current_print_cmd=E_PRINT_PAUSE;
			break;
			case 2:
				displayImage(260, 30, IMG_ADDR_BUTTON_RESUME);
				cur_pstatus=2;
			break;
			case 3:
				displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE_DISABLE); 
				current_print_cmd=E_PRINT_DISPAUSE;	
				cur_pstatus=3;
			break;
			case 10:
			default:
				//disable button pause
				displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE_DISABLE); 
				current_print_cmd=E_PRINT_DISPAUSE;			
			break;
		}
		cur_flag=true;
	}
	displayImage(260, 105, IMG_ADDR_BUTTON_ADJUST);	
	displayImage(260, 180, IMG_ADDR_BUTTON_END);        
	displayPrintInformation();
	
}
void display_image::displayPrintInformation(void)
{
	if(thermalManager.fan_speed[0]>0)
	{
		displayRunningFan(140, 30);	
	}
	displayPrintTemperature();
	displayHeightValue();
	displayFanSpeed();
//	if(cur_flag)
	{
		dispalyCurrentStatus();
	}
	if(is_print_finish)
	{
		printpercent=100;
	}
	else
	{
		printpercent=card.percentDone();
	//	CardFile.calcCountUpTime();
	//	displayCountUpTime();
	}
	displayPrintProgress();
	CardFile.calcCountUpTime();
	CardFile.calcCountDownTime();
	displayCountUpTime();
	displayCountDownTime();
}
void display_image::displayRunningFan(uint16_t pos_x, uint16_t pos_y)
{
	static bool is_fan0_display = false;
	if(!is_fan0_display)
	{
		displayImage(pos_x, pos_y, IMG_ADDR_INDICATOR_FAN_0);				
	}
	else
	{
		displayImage(pos_x, pos_y, IMG_ADDR_INDICATOR_FAN_1);				 					
	}
	is_fan0_display = !is_fan0_display;
}
void display_image::displayFanSpeed(void)
{
	LCD_Fill(170,30,250,60,White);		//clean fan speed display zone
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"F: %d", thermalManager.fan_speed[0]);
	LCD_ShowString(175,37,s_text);	
}
void display_image::displayHeating(void)
{
	LCD_Fill(0,190,200,240,White); 
	displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE_DISABLE);
//	current_print_cmd=E_PRINT_DISPAUSE;
	displayImage(10, 195, IMG_ADDR_PROMPT_HEATING); 		
	color=RED;		
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_PRINT_STATUS_HEATING);
	LCD_ShowString(45,202,s_text);	
	color=BLACK;
}
void display_image::displayPrinting(void)
{
	LCD_Fill(0,190,200,240,White); 
	displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE);
//	current_print_cmd=E_PRINT_PAUSE;
	//prompt printing
	displayImage(10, 195, IMG_ADDR_PROMPT_PRINTING); 		
	color=BLACK;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_PRINT_STATUS_RUNNING);
	LCD_ShowString(45,202,s_text);
}
void display_image::displayPause(void)
{
	LCD_Fill(0,190,200,240,White); 
	displayImage(260, 30, IMG_ADDR_BUTTON_RESUME);		
	//prompt pause	
	displayImage(10, 195, IMG_ADDR_PROMPT_PAUSE); 		
	color=BLACK;	
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_PRINT_STATUS_PAUSING);	
	LCD_ShowString(45,202,s_text);	
}
void display_image::displayPrintTemperature(void)
{
	LCD_Fill(45,30,130,60,White);		//clean extruder display zone	
	LCD_Fill(45,70,130,100,White);		//clean bed display zone
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"E: %d/%d",(int16_t)thermalManager.temp_hotend[0].current,thermalManager.temp_hotend[0].target);
	LCD_ShowString(45,37,s_text);
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"B: %d/%d",(int16_t)thermalManager.temp_bed.current,thermalManager.temp_bed.target);
	LCD_ShowString(45,77,s_text);
}
void display_image::displayPrintProgress(void)
{
	LCD_Fill(210,110,250,140,White);							//clean percentage display zone
	LCD_Fill(10,110,(uint16_t)(10+printpercent*1.9),140,DARKBLUE); 	//display current progress
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%d %%",(printpercent));
	LCD_ShowString(210,117,s_text);
}
void display_image::displayHeightValue(void)
{
	LCD_Fill(170,70,250,100,White);		//clean height display zone
	color=Black;	
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"Z: %.1f",current_position[Z_AXIS]);
	LCD_ShowString(175,77,s_text);	
}
void display_image::dispalyCurrentStatus(void)
{
	switch(cur_pstatus)
	{
		case 0:   //heating
			displayHeating();
			current_print_cmd=E_PRINT_DISPAUSE;
			cur_ppage=0;	
			cur_pstatus=10;	
		break;
		case 1:   //printing
			displayPrinting();
			current_print_cmd=E_PRINT_PAUSE;
			cur_pstatus=10;
			cur_ppage=1;
		break;
		case 2:  //pause
			if(READ(FIL_RUNOUT_PIN))
				displayNofilament();
			else
				displayPause();	
			cur_pstatus=10;
			cur_ppage=2;
		break;
		case 3:
			LCD_Fill(260,30,320,90,White);		//clean pause/resume icon display zone
			LCD_Fill(0,190,200,240,White); 		//clean prompt display zone
			//disable pause button
			displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE_DISABLE);
			displayImage(10, 195, IMG_ADDR_PROMPT_COMPLETE);
			color =Green;
			current_print_cmd=E_PRINT_DISPAUSE;
			CLEAN_STRING(s_text);
			sprintf((char*)s_text,"%s",TXT_MENU_PRINT_STATUS_FINISH);
			LCD_ShowString(45,202,s_text);
			is_printing=false;
			is_print_finish=true;
			cur_pstatus=10;
			cur_ppage=3;
		 //   clearVarPrintEnd();
		 	print_times=0;  //Make sure that the remaining time is 0 after printing 
		break;
		case 10:
		default:
		break;
	}
}
void display_image::displayCountUpTime(void)
{
	LCD_Fill(175,150,250,180,White);	//clean cout-up timer display zone
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%02d:%02d:%02d", cp_time_hours, cp_time_minutes, cp_time_seconds);
	LCD_ShowString(175,157,s_text);
}
void display_image::displayCountDownTime(void)
{
	if( print_times == 0&&is_print_finish==false){ 		/* if don't get total time */
		CLEAN_STRING(s_text);
		sprintf((char *)s_text,"%s",TXT_MENU_PRINT_CD_TIMER_NULL);
		LCD_ShowString(45,157,s_text);			
	}else{   /* if get total time */
		LCD_Fill(45,150,130,180,White); 	/* clean count-down timer display zone */
		color=Black;
		CLEAN_STRING(s_text);
		sprintf((char *)s_text,TXT_MENU_PRINT_CD_TIMER,Remaining_hours,Remaining_minutes);
		LCD_ShowString(45,157,s_text);
	}
}

/***************************Adjust page*******************************************/
void display_image::displayWindowAdjust(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 25, BG_COLOR_CAPTION_ADJUST); 	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_ADJUST);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_ADJUST_CN);		//caption words
	#endif
	displayImage(5, 35, IMG_ADDR_BUTTON_ADD);
	displayImage(69, 35, IMG_ADDR_BUTTON_ADD);
	displayImage(133, 35, IMG_ADDR_BUTTON_ADD);
	displayImage(196, 35, IMG_ADDR_BUTTON_ADD);
	displayImage(5, 175, IMG_ADDR_BUTTON_SUB);
	displayImage(69, 175, IMG_ADDR_BUTTON_SUB);
	displayImage(133, 175, IMG_ADDR_BUTTON_SUB);
	displayImage(196, 175, IMG_ADDR_BUTTON_SUB);	
	//default_move_distance=5;
	initialMoveDistance(260, 40);	
	displayImage(260, 101, IMG_ADDR_BUTTON_MORE);	
	displayImage(260, 175, IMG_ADDR_BUTTON_RETURN);
	displayImage(20, 105, IMG_ADDR_INDICATOR_HEAD);
	displayImage(85, 105, IMG_ADDR_INDICATOR_BED);
	displayImage(144, 105, IMG_ADDR_INDICATOR_FAN_0);
	#ifndef Chinese
		displayImage(209, 105, IMG_ADDR_INDICATOR_SPEED);
	#else
		displayImage(209, 105, IMG_ADDR_INDICATOR_SPEED_CN);
	#endif
	dispalyAdjustTemp(); 
	dispalyAdjustFanSpeed(); 	
	dispalyAdjustMoveSpeed();

}
void display_image::dispalyAdjustTemp(void)
{
	LCD_Fill(5,143,65,163,White);		//clean extruder temperature display zone
	LCD_Fill(74,143,134,163,White);		//clean bed temperature display zone
	color=BLACK;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%d/%d",(int16_t)(thermalManager.temp_hotend[0].current),thermalManager.temp_hotend[0].target);
	LCD_ShowString(5,143,s_text);
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%d/%d",(int16_t)(thermalManager.temp_bed.current),thermalManager.temp_bed.target);
	LCD_ShowString(74,143,s_text);
}
void display_image::dispalyAdjustFanSpeed(void)
{
	LCD_Fill(146,143,196,163,White); 	//clean fan speed display zone
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%03d",thermalManager.fan_speed[0]);
	LCD_ShowString(146,143,s_text); 
}
void display_image::dispalyAdjustMoveSpeed(void)
{
	LCD_Fill(208,143,258,163,White); 	//clean feed rate display zone
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%03d%%",feedrate_percentage);
	LCD_ShowString(208,143,s_text); 
}
void display_image::scanWindowAdjust(uint16_t rv_x,uint16_t rv_y)
{
	if(rv_x>260&&rv_x<315&&rv_y>175&&rv_y<230)	//return
	{
		next_window_ID=eMENU_PRINT;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>35&&rv_y<90) //add extruder temperature or flow multiplier
	{			
		current_button_id=eBT_ADJUSTE_PLUS;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>175&&rv_y<230) //subtract extruder temperature or flow multiplier
	{
		current_button_id=eBT_ADJUSTE_MINUS;
	}
	else if(rv_x>69&&rv_x<124&&rv_y>35&&rv_y<90)  //add bed temperature 
	{			
		current_button_id=eBT_ADJUSTBED_PLUS;
	}
	else if(rv_x>69&&rv_x<124&&rv_y>175&&rv_y<230) //subtract bed temperature
	{ 		
		current_button_id=eBT_ADJUSTBED_MINUS;
	}
	else if(rv_x>133&&rv_x<188&&rv_y>35&&rv_y<90)  //add fan speed
	{			
		current_button_id=eBT_ADJUSTFAN_PLUS;
	}
	else if(rv_x>133&&rv_x<188&&rv_y>175&&rv_y<230)  //subtract fan speed
	{		
		current_button_id=eBT_ADJUSTFAN_MINUS;
	}
	else if(rv_x>196&&rv_x<251&&rv_y>35&&rv_y<90)  //add feed rate	
	{			
		current_button_id=eBT_ADJUSTSPEED_PLUS;
	}	
	else if(rv_x>196&&rv_x<251&&rv_y>175&&rv_y<230)  //subtract feed rate
	{		
		current_button_id=eBT_ADJUSTSPEED_MINUS;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>40&&rv_y<80)  //change distance
	{			
		current_button_id=eBT_DISTANCE_CHANGE;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>100&&rv_y<155)  //select more
	{		
		next_window_ID=eMENU_ADJUST_MORE;
	}
}
void display_image::dispalyAdjustFlow(void)
{
	if(cur_pstatus==2)   //show extrude when no filament in adjustmore page
	{
		displayImage(196, 35, IMG_ADDR_BUTTON_EXTRUDE); 
		cur_ppage=2;  
		cur_pstatus=10;
	}
	LCD_Fill(5,143,65,163,White);		//clean flow display zone
	color=Black;
	CLEAN_STRING(s_text);
	sprintf((char *)s_text,"%03d%%",planner.flow_percentage[0]);
	LCD_ShowString(19,143,s_text);
}
void display_image::displayWindowAdjustMore(void)
{
	lcdClear(White);
	LCD_Fill(0, 0, 320, 25, BG_COLOR_CAPTION_ADJUST);	//caption background
	#ifndef Chinese
		displayImage(115, 5, IMG_ADDR_CAPTION_ADJUST);		//caption words
	#else
		displayImage(115, 5, IMG_ADDR_CAPTION_ADJUST_CN);		//caption words
	#endif
	displayImage(5, 35, IMG_ADDR_BUTTON_ADD);
	displayImage(5, 175, IMG_ADDR_BUTTON_SUB);
	#ifndef Chinese
		displayImage(20, 105, IMG_ADDR_INDICATOR_FLOW);
	#else
		displayImage(20, 105, IMG_ADDR_INDICATOR_FLOW_CN);
	#endif
	initialMoveDistance(260, 40);	
	displayImage(260, 101, IMG_ADDR_BUTTON_MORE);	
	displayImage(260, 175, IMG_ADDR_BUTTON_RETURN);
	if(cur_ppage==2){	//when printing pause
		displayImage(196, 35, IMG_ADDR_BUTTON_EXTRUDE);
	}
	dispalyAdjustFlow();
}
void display_image::scanWindowAdjustMore(uint16_t rv_x,uint16_t rv_y)
{
	if(rv_x>260&&rv_x<315&&rv_y>175&&rv_y<230)  //return
	{				
		next_window_ID=eMENU_PRINT;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>35&&rv_y<90)  //add flow
	{
		current_button_id=eBT_ADJUSTE_PLUS;
	}
	else if(rv_x>5&&rv_x<60&&rv_y>175&&rv_y<230) //subtract flow
	{
		current_button_id=eBT_ADJUSTE_MINUS;
	}
	else if(rv_x>196&&rv_x<251&&rv_y>35&&rv_y<90)   //go to extrude
	{
		if(cur_ppage==2)
			next_window_ID=eMENU_EXTRUDE;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>40&&rv_y<80) //change distance
	{			
		current_button_id=eBT_DISTANCE_CHANGE;
	}
	else if(rv_x>260&&rv_x<315&&rv_y>100&&rv_y<155) //select more
	{		
		next_window_ID=eMENU_ADJUST;
	}
}

void display_image::dispalyDialogYesNo(uint8_t dialog_index)
{
	displayImage(60, 45, IMG_ADDR_DIALOG_BODY);
	displayImage(85, 130, IMG_ADDR_BUTTON_YES);	
	displayImage(180, 130, IMG_ADDR_BUTTON_NO);
	displayImage(70, 80, IMG_ADDR_PROMPT_QUESTION);
	displayDialogText(dialog_index);
}
void display_image::dispalyDialogYes(uint8_t dialog_index)
{
	displayImage(60, 45, IMG_ADDR_DIALOG_BODY);
	displayImage(132, 130, IMG_ADDR_BUTTON_YES);
	if(dialog_index==eDIALOG_SETTS_SAVE_OK)
		displayImage(70, 80, IMG_ADDR_PROMPT_COMPLETE);
	else
		displayImage(70, 80, IMG_ADDR_PROMPT_ERROR);
	displayDialogText(dialog_index);
}
void display_image::displayDialogText(uint8_t dialog_index)
{
	/* caption */
	bgColor=BG_COLOR_CAPTION_DIALOG;
	color = WHITE;
	LCD_ShowString(70,50,(char*)c_dialog_text[dialog_index][0]);	
	/* prompt */
	bgColor = WHITE;
	color = BLACK;	
	//LCD_ShowString(110,76,(char*)c_dialog_text[dialog_index][1]);		
	LCD_ShowString(110,76,(char*)c_dialog_text[dialog_index][1]);	
	LCD_ShowString(110,92,(char*)c_dialog_text[dialog_index][2]);	
	LCD_ShowString(110,108,(char*)c_dialog_text[dialog_index][3]);
}
void display_image::scanDialogStart(uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>85&&rv_x<140&&rv_y>130&&rv_y<185) //select yes
	{	
		current_button_id=eBT_DIALOG_PRINT_START;
	}
	else if(rv_x>180&&rv_x<235&&rv_y>130&&rv_y<185)  //select no
	{	
		current_button_id=eBT_DIALOG_PRINT_NO;
	}
}
void display_image::scanDialogEnd( uint16_t rv_x, uint16_t rv_y )
{
	if(rv_x>85&&rv_x<140&&rv_y>130&&rv_y<185)  //select yes
	{	
		next_window_ID=eMENU_HOME;
		//cur_ppage=0;
	}
	else if(rv_x>180&&rv_x<235&&rv_y>130&&rv_y<185) //select no
	{	
		//cur_flag=false;
		next_window_ID=eMENU_PRINT;
	}
}
void display_image::displayNofilament(void)
{
	LCD_Fill(260,30,320,90,White);		//clean pause/resume icon display zone
	LCD_Fill(0,190,200,240,White);		//clean prompt display zone
	//disable pause button
	displayImage(260, 30, IMG_ADDR_BUTTON_RESUME);
	//diplay no flament prompt
	displayImage(10, 195, IMG_ADDR_PROMPT_WARNING);
	color=0xFDE0;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_MENU_PRINT_STATUS_NO_FILAMENT);
	LCD_ShowString(45,202,s_text);
	color=Black;
}
void display_image::scanDialogRecovery( uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>85&&rv_x<140&&rv_y>130&&rv_y<185)  //select yes
	{
		current_button_id=eBT_DIALOG_PRINT_START;
	}
	else if(rv_x>180&&rv_x<235&&rv_y>130&&rv_y<185) //select no
	{
		next_window_ID=eMENU_HOME;
	}
}
void display_image::scanDialogRefactory(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>85&&rv_x<140&&rv_y>130&&rv_y<185)  //select yes
	{
		current_button_id=eBT_DIALOG_REFACTORY_YES;
	}
	else if(rv_x>180&&rv_x<235&&rv_y>130&&rv_y<185) //select no
	{
		next_window_ID=eMENU_SETTINGS;
	}
}
void display_image::scanDialogSave(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>85&&rv_x<140&&rv_y>130&&rv_y<185)  //select yes
	{
		current_button_id=eBT_DIALOG_SAVE_YES;
	}
	else if(rv_x>180&&rv_x<235&&rv_y>130&&rv_y<185) //select no
	{
		if(setting_return_more)
		{
			next_window_ID=eMENU_HOME_MORE;
			setting_return_more=false;
			is_setting_change=false;
		}
		else
			next_window_ID=eMENU_SETTINGS;
	}
}
void display_image::scanDialogYes(uint16_t rv_x, uint16_t rv_y)
{
	if(rv_x>132&&rv_x<187&&rv_y>130&&rv_y<185)  //select ok
	{
		if(current_window_ID==eMENU_DIALOG_ERRORTEMPBED||current_window_ID==eMENU_DIALOG_ERRORTEMPE)
		{
			next_window_ID=eMENU_HOME;
		}
		else
		{
			if(setting_return_more)
			{
				next_window_ID=eMENU_HOME_MORE;
				setting_return_more=false;
			}
			else
				next_window_ID=eMENU_SETTINGS;
		}
	}
}
void display_image::LGT_Printerabnormal_Kill(const char* info)
{
	displayImage(60, 45, IMG_ADDR_DIALOG_BODY);
	bgColor=BG_COLOR_CAPTION_DIALOG;
	color = WHITE;
	LCD_ShowString(70,50, (char*)info);
	color=Red;
	bgColor=White;
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_PRINTER_KILLED_INFO1);
	LCD_ShowString(75,90,s_text); 
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_PRINTER_KILLED_INFO2);
	LCD_ShowString(75,116,s_text);
	CLEAN_STRING(s_text);
	sprintf((char*)s_text,"%s",TXT_PRINTER_KILLED_INFO3);
	LCD_ShowString(75,142,s_text);
	color=Black;
}
void display_image::LGT_Tempabnormal_Warning(const char* info)
{
	thermalManager.disable_all_heaters();
	displayImage(60, 45, IMG_ADDR_DIALOG_BODY);
	bgColor=BG_COLOR_CAPTION_DIALOG;
	color = WHITE;
	LCD_ShowString(70,50, (char*)info);
	color=Black;
	bgColor=White;
	displayImage(70, 80, IMG_ADDR_PROMPT_ERROR);
	displayImage(132, 130, IMG_ADDR_BUTTON_YES); 
	//sprintf((char*)s_text,"%s","Click on the sc-");
	sprintf((char*)s_text,"%s",TXT_PRINTER_TEMPERROR_INFO1);
	LCD_ShowString(110,76,s_text); 
	CLEAN_STRING(s_text);
	//sprintf((char*)s_text,"%s","reen back to the");  
	sprintf((char*)s_text,"%s",TXT_PRINTER_TEMPERROR_INFO2); 
	LCD_ShowString(110,92,s_text);
	CLEAN_STRING(s_text);
	//	sprintf((char*)s_text,"%s","main page!");
	sprintf((char*)s_text,"%s",TXT_PRINTER_TEMPERROR_INFO3);
	LCD_ShowString(110,108,s_text);
    current_window_ID=eWINDOW_NONE;
    loop:while (!getTouchPoint(&X_AD,&Y_AD))      //getTouchPoint(&X_AD,&Y_AD)
    {};
	cur_x=(uint16_t)((((int32_t)X_AD * (int32_t)xCalibration) >> 16) + xOffset);
	cur_y=(uint16_t)((((int32_t)Y_AD* (int32_t)yCalibration) >> 16) + yOffset);
	if(cur_x>320||cur_y>240)
	{
		goto loop;
	}
	if(cur_x>132&&cur_x<187&&cur_y>130&&cur_y<185)
	{
    	next_window_ID=eMENU_HOME;
		cur_x=cur_y=X_AD=Y_AD=0;
	}
	else
	{
		cur_x=cur_y=X_AD=Y_AD=0;
		goto loop;
	}
	
}
uint32_t getdata;
void display_image::LGT_lcd_init()
{
    OUT_WRITE(LCD_BACKLIGHT_PIN, LOW);
    OUT_WRITE(LCD_RESET_PIN, LOW); // perform a clean hardware reset
    _delay_ms(5);
    OUT_WRITE(LCD_RESET_PIN, HIGH);
    _delay_ms(5); 
    OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
	LGT_Lcdfsmc_init();
	lcdId= TOUCH_LCD_IO_ReadReg(0x0000);  //read  id
	if (lcdId == 0) 
	{
		// read ID1 register to get LCD controller ID, MOST of the time located in register 0x04
		getdata = TOUCH_LCD_IO_ReadData(0x04, 3);
		lcdId = (uint16_t)(getdata & 0xFFFF);
  	} 
  //If ID1 is 0, it means we need to check alternate registers, like 0xD3 in the case of ILI9341
  if (lcdId == 0) 
  {
    lcdId = TOUCH_LCD_IO_ReadReg(0x00);
    if (lcdId == 0)
	{
      // reading ID4 register (0xD3)  to get ILI9341 identifier instead of register ID (0x04)
      getdata = TOUCH_LCD_IO_ReadData(0xD3, 3);
      lcdId = (uint16_t)(getdata & 0xFFFF);
    }
  }
//	Serial1.println(lcdId);
	 switch(lcdId) 
	{
		case 0x1505: break; // R61505U
		case 0x8552: st7789v_Init(); break; // ST7789V
		case 0x8989: break; // SSD1289
		case 0x9325: break; // ILI9325
		case 0x9328: ili9328_Init(); break; // ILI9328
		case 0x9341: ILI9341_Init(); break; // ILI9341
		case 0x0404: break; // No LCD Controller detected
		default: break; // Unknown LCD Controller
	}
}
bool display_image::LGT_MainScanWindow(void)
{
		bool window_change=true;
		switch (current_window_ID)
		{
			case eMENU_HOME:
				scanWindowHome(cur_x,cur_y);
				cur_x=cur_y=0;
				break;
			case eMENU_HOME_MORE:
				scanWindowMoreHome(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_MOVE:
				scanWindowMove(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_FILE:
				scanWindowFile(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_PRINT:
				scanWindowPrint(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_ADJUST:
			    scanWindowAdjust(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_ADJUST_MORE:
				scanWindowAdjustMore(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_PREHEAT:
				scanWindowPreheating(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_LEVELING:
				scanWindowLeveling(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_EXTRUDE:
				scanWindowExtrude(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_SETTINGS:
				scanWindowSettings(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_SETTINGS2:
				scanWindowSettings2(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_ABOUT:
				scanWindowAbout(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_DIALOG_START:case eMENU_DIALOG_NO_FIL:
				scanDialogStart(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_DIALOG_END:
				scanDialogEnd(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_DIALOG_RECOVERY:
				scanDialogRecovery(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_DIALOG_REFACTORY:
				scanDialogRefactory(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_DIALOG_SAVE:
				scanDialogSave(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			case eMENU_DIALOG_SAVE_OK:case eMENU_DIALOG_ERRORTEMPE:case eMENU_DIALOG_ERRORTEMPBED:
				scanDialogYes(cur_x,cur_y);
				cur_x=cur_y=0;
			break;
			default:
				window_change=false;
				break;
		}
		return window_change;
}
bool display_image::LGT_Ui_Update(void)
{
	bool button_type=false;   
	switch (next_window_ID)
		{
			case eMENU_HOME:
				if(dir_auto_feed!=0)
					stopAutoFeed();
				else if(current_window_ID==eMENU_DIALOG_END)
				{
					card.flag.abort_sd_printing=true;
					is_printing=wait_for_user = wait_for_heatup=false;
					//clearVarPrintEnd();
				}
				else if(current_window_ID==eMENU_FILE)
				{
					card.returnroot();
					clearfilevar();
				}
				current_window_ID=eMENU_HOME;
				next_window_ID=eWINDOW_NONE;
				displayWindowHome();
				break;
			case eMENU_HOME_MORE:
				if((current_window_ID==eMENU_LEVELING)&&all_axes_homed())
					enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
				if(current_window_ID==eMENU_SETTINGS&&is_setting_change)
				{
					page_index=0;
					choose_setting=ARGUMENST_MAX_NUM;
					setting_return_more=true;
					dispalyDialogYesNo(eDIALOG_SETTS_SAVE);
					current_window_ID=eMENU_DIALOG_SAVE;
				}
				else
				{
					if(current_window_ID==eMENU_DIALOG_SAVE)
						ConfigSettings.settingsLoad();
					else if(current_window_ID==eMENU_SETTINGS)
					{
						page_index=0;
						choose_setting=ARGUMENST_MAX_NUM;
					}
					current_window_ID=eMENU_HOME_MORE;
					displayWindowHomeMore();
				}
				next_window_ID=eWINDOW_NONE;
			break;
			 case eMENU_MOVE:
				current_window_ID=eMENU_MOVE;
				next_window_ID=eWINDOW_NONE;
				displayWindowMove();
			break;
			case eMENU_FILE:
				current_window_ID=eMENU_FILE;
				next_window_ID=eWINDOW_NONE;
				clearfilevar();
				displayWindowFiles();
			break;
			case eMENU_FILE1:
				current_window_ID=eMENU_FILE;
				next_window_ID=eWINDOW_NONE;
				displayChosenFile();
			break;
			case eMENU_PRINT:
				current_window_ID=eMENU_PRINT;
				next_window_ID=eWINDOW_NONE;
				displayWindowPrint();
			break;
			case eMENU_ADJUST:
				current_window_ID=eMENU_ADJUST;
				next_window_ID=eWINDOW_NONE;
				displayWindowAdjust();
			break;
			case eMENU_ADJUST_MORE:
				if(current_window_ID==eMENU_EXTRUDE)
				{
					planner.set_e_position_mm((destination[E_AXIS] = current_position[E_AXIS] = (resume_xyze_position[E_AXIS] - 2)));
					feedrate_mm_s=save_feedtate;
				}
				current_window_ID=eMENU_ADJUST_MORE;
				next_window_ID=eWINDOW_NONE;
				displayWindowAdjustMore();
			break;
			case eMENU_PREHEAT:
				current_window_ID=eMENU_PREHEAT;
				next_window_ID=eWINDOW_NONE;
					displayWindowPreheat();
			break;
			case eMENU_LEVELING:
				current_window_ID=eMENU_LEVELING;
				next_window_ID=eWINDOW_NONE;
				set_all_unhomed();
				displayWindowLeveling();
			break;
			case eMENU_EXTRUDE:
				current_window_ID=eMENU_EXTRUDE;
				next_window_ID=eWINDOW_NONE;
				is_bed_select=false;
				displayWindowExtrude();
			break;
			case eMENU_SETTINGS:
				current_window_ID=eMENU_SETTINGS;
				next_window_ID=eWINDOW_NONE;
				displayWindowSettings();
			break;
			// eMENU_SETTINGS2,
			case eMENU_ABOUT:
				current_window_ID=eMENU_ABOUT;
				next_window_ID=eWINDOW_NONE;
				displayWindowAbout();
			break;
			case eMENU_DIALOG_RECOVERY:
				current_window_ID=eMENU_DIALOG_RECOVERY;
				next_window_ID=eWINDOW_NONE;
				dispalyDialogYesNo(eDIALOG_PRINT_RECOVERY);
			break;
			default:
				button_type=true;
				break;
		}
		if(button_type==false)
			current_button_id=eBT_BUTTON_NONE;
	return button_type;
}
millis_t next_update_Time = 0;
void display_image::LGT_Printer_Data_Update(void)
{
	if(millis()>=next_update_Time)
	{
		next_update_Time=1000+millis();
		checkTemprature();
		switch (current_window_ID)
		{
			// eMENU_HOME = 0,
			// eMENU_HOME_MORE,
			case eMENU_HOME:
				SDIO_Init();
				switch(SDIO_GetCardState())	
				{
					case SDIO_CARD_ERROR:
						if(sd_insert)
						{
							sd_insert=false;
						}
					break;
					default:
						if(!sd_insert)
						{
							card.initsd();
							sd_insert=true;
						}
					break;
				}
			break;
			case eMENU_MOVE:
				displayMoveCoordinate();
			break;
			case eMENU_PRINT:
				displayPrintInformation();
			break;
			case eMENU_ADJUST:
				dispalyAdjustFanSpeed(); 
				dispalyAdjustTemp(); 	
				dispalyAdjustMoveSpeed();
				if(thermalManager.fan_speed[0]>0)
				{
					displayRunningFan(144, 105);	
				}
				// if(cur_pstatus==3)
				// 	cur_ppage=10;
				switch(cur_pstatus)   //save current status page when in adjust page 
				{
					case 0:
						cur_ppage=0;
					break;
					case 1:
						cur_ppage=1;
					break;
					case 2:
						cur_ppage=2;
					break;
					case 3:
						cur_ppage=10;
					break;
					default:
					break;
				}
			break;
			case eMENU_ADJUST_MORE:
				dispalyAdjustFlow();
				switch(cur_pstatus)   //save current status page when in adjust page 
				{
					case 0:
						cur_ppage=0;
					break;
					case 1:
						cur_ppage=1;
					break;
					case 2:
						cur_ppage=2;
					break;
					case 3:
						cur_ppage=10;
					break;
					default:
					break;
				}
			break;
			case eMENU_PREHEAT:
				updatePreheatingTemp();
			break;
			// eMENU_LEVELING,
			case eMENU_EXTRUDE:
				dispalyExtrudeTemp();
				actAutoFeed();
				displayRunningAutoFeed();
			break;
			case eMENU_FILE:
				SDIO_Init();
				switch(SDIO_GetCardState())	
				{
					case SDIO_CARD_ERROR:
						if(sd_insert)
						{
							sd_insert=false;
							displayPromptSDCardError();
						}
					break;
					default:
						if(!sd_insert)
						{
							card.initsd();
							file_count=CardFile.getsdfilecount();
							page_index_max=CardFile.getsdfilepage();
							LCD_Fill(0, 25, 239, 174,White);	//clean  
							displayFileList();
							displayFilePageNumber();
							sd_insert=true;
						}
					break;
				}
			break;
		case eMENU_DIALOG_ERRORTEMPBED:
			if((thermalManager.temp_bed.current>MIN_ADJUST_TEMP_BED)&&(thermalManager.temp_bed.current<MAX_ADJUST_TEMP_BED))
			{
				displayWindowHome();
				current_window_ID=eMENU_HOME;
				check_temp_Bed=0;     //checkTemprature();
			}
		break;
		case eMENU_DIALOG_ERRORTEMPE:
			if((thermalManager.temp_hotend[0].current>MIN_ADJUST_TEMP_EXTRUDE)&&(thermalManager.temp_hotend[0].current<MAX_ADJUST_TEMP_EXTRUDE))
			{
				displayWindowHome();
				current_window_ID=eMENU_HOME;
				check_temp_E=0;    
			}
		break;
			// eMENU_SETTINGS,
			// eMENU_SETTINGS2,
			// eMENU_ABOUT,
		default:
			break;
		}
	}
}
void display_image::LGT_Ui_Buttoncmd(void)
{
		switch (current_button_id)
		{
			case eBT_MOVE_X_MINUS:
				current_position[X_AXIS]-=default_move_distance;
				if(is_aixs_homed[X_AXIS]||all_axes_homed())
				{
					if(current_position[X_AXIS]<0)
					current_position[X_AXIS]=0;
				}
				LGT_Line_To_Current_Position(X_AXIS);
				displayMoveCoordinate();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_X_PLUS:
				current_position[X_AXIS]+=default_move_distance;
				if(current_position[X_AXIS]>X_BED_SIZE)
					current_position[X_AXIS]=X_BED_SIZE;
				LGT_Line_To_Current_Position(X_AXIS);
				displayMoveCoordinate();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_X_HOME:
				enqueue_and_echo_commands_P(PSTR("G28 X0"));
				current_button_id=eBT_BUTTON_NONE;
				is_aixs_homed[X_AXIS]=true;
			break;
			case eBT_MOVE_Y_MINUS:
				current_position[Y_AXIS]-=default_move_distance;
				if(is_aixs_homed[Y_AXIS]||all_axes_homed())
				{
					if(current_position[Y_AXIS]<0)
						current_position[Y_AXIS]=0;
				}
				LGT_Line_To_Current_Position(Y_AXIS);
				displayMoveCoordinate();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_Y_PLUS:
				current_position[Y_AXIS]+=default_move_distance;
				if(current_position[Y_AXIS]>Y_BED_SIZE)
					current_position[Y_AXIS]=Y_BED_SIZE;
				LGT_Line_To_Current_Position(Y_AXIS);
				displayMoveCoordinate();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_Y_HOME:
				enqueue_and_echo_commands_P(PSTR("G28 Y0"));
				current_button_id=eBT_BUTTON_NONE;
				is_aixs_homed[Y_AXIS]=true;
			break;
			case eBT_MOVE_Z_MINUS:
				current_position[Z_AXIS]-=default_move_distance;
				if(is_aixs_homed[Z_AXIS]||all_axes_homed())
				{
					if(current_position[Z_AXIS]<0)
						current_position[Z_AXIS]=0;
				}
				LGT_Line_To_Current_Position(Z_AXIS);
				displayMoveCoordinate();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_Z_PLUS:
				current_position[Z_AXIS]+=default_move_distance;
				if(current_position[Z_AXIS]>Z_MACHINE_MAX)
					current_position[Z_AXIS]=Z_MACHINE_MAX;
				LGT_Line_To_Current_Position(Z_AXIS);
				displayMoveCoordinate();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_Z_HOME:
				enqueue_and_echo_commands_P(PSTR("G28 Z0"));
				current_button_id=eBT_BUTTON_NONE;
				is_aixs_homed[Z_AXIS]=true;
			break;
			case eBT_MOVE_ALL_HOME:
				enqueue_and_echo_commands_P(PSTR("G28"));
				current_button_id=eBT_BUTTON_NONE;
			break;

	
			case eBT_MOVE_L0:
				if(!all_axes_homed())
				{
					enqueue_and_echo_commands_P(PSTR("G28"));
					thermalManager.setTargetHotend(0, 0);
					thermalManager.setTargetBed(0);
				}
				enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
				#if defined(LK1) || defined(U20)
					enqueue_and_echo_commands_P(PSTR("G0 X50 Y250 F5000"));
				#elif defined(LK2) || defined(LK4) || defined(U30)
					enqueue_and_echo_commands_P(PSTR("G0 X50 Y170 F5000"));
				#elif  defined(LK1_PLUS) ||  defined(U20_PLUS) 
					enqueue_and_echo_commands_P(PSTR("G0 X50 Y350 F5000"));
				#endif
			//	enqueue_and_echo_commands_P(PSTR("G0 X50 Y170 F5000"));
				enqueue_and_echo_commands_P(PSTR("G0 Z0 F300"));
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_L1:
				if(!all_axes_homed())
				{
					enqueue_and_echo_commands_P(PSTR("G28"));
					thermalManager.setTargetHotend(0, 0);
					thermalManager.setTargetBed(0);
				}
				enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
				#if defined(LK1) || defined(U20)
					enqueue_and_echo_commands_P(PSTR("G0 X250 Y250 F5000"));
				#elif defined(LK2) || defined(LK4) || defined(U30)
					enqueue_and_echo_commands_P(PSTR("G0 X170 Y170 F5000"));
				#elif  defined(LK1_PLUS) ||  defined(U20_PLUS) 
					enqueue_and_echo_commands_P(PSTR("G0 X350 Y350 F5000"));
				#endif
			//	enqueue_and_echo_commands_P(PSTR("G0 X170 Y170 F5000"));
				enqueue_and_echo_commands_P(PSTR("G0 Z0 F300"));
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_L2:
				if(!all_axes_homed())
				{
					enqueue_and_echo_commands_P(PSTR("G28"));
					thermalManager.setTargetHotend(0, 0);
					thermalManager.setTargetBed(0);
				}
				enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
				#if defined(LK1) || defined(U20)
					enqueue_and_echo_commands_P(PSTR("G0 X250 Y50 F5000"));
			#elif defined(LK2) || defined(LK4) || defined(U30)
					enqueue_and_echo_commands_P(PSTR("G0 X170 Y50 F5000"));
				#elif  defined(LK1_PLUS) ||  defined(U20_PLUS) 
					enqueue_and_echo_commands_P(PSTR("G0 X350 Y50 F5000"));
				#endif
		//		enqueue_and_echo_commands_P(PSTR("G0 X170 Y50 F5000"));
				enqueue_and_echo_commands_P(PSTR("G0 Z0 F300"));
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_L3:
				if(!all_axes_homed())
				{
					enqueue_and_echo_commands_P(PSTR("G28"));
					thermalManager.setTargetHotend(0, 0);
					thermalManager.setTargetBed(0);
				}
				enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
				#if defined(LK1) || defined(U20)
					enqueue_and_echo_commands_P(PSTR("G0 X50 Y50 F5000"));
				#elif defined(LK2) || defined(LK4) || defined(U30)
					enqueue_and_echo_commands_P(PSTR("G0 X50 Y50 F5000"));
				#elif  defined(LK1_PLUS) ||  defined(U20_PLUS) 
					enqueue_and_echo_commands_P(PSTR("G0 X50 Y50 F5000"));
				#endif
		//		enqueue_and_echo_commands_P(PSTR("G0 X50 Y50 F5000"));
				enqueue_and_echo_commands_P(PSTR("G0 Z0 F300"));
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_MOVE_L4:
				if(!all_axes_homed())
				{
					enqueue_and_echo_commands_P(PSTR("G28"));
					thermalManager.setTargetHotend(0, 0);
					thermalManager.setTargetBed(0);
				}
				enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
				#if defined(LK1) || defined(U20)
					enqueue_and_echo_commands_P(PSTR("G0 X150 Y150 F5000"));
				#elif defined(LK2) || defined(LK4) || defined(U30)
					enqueue_and_echo_commands_P(PSTR("G0 X110 Y110 F5000"));
				#elif  defined(LK1_PLUS) ||  defined(U20_PLUS) 
					enqueue_and_echo_commands_P(PSTR("G0 X200 Y200 F5000"));
				#endif
		//		enqueue_and_echo_commands_P(PSTR("G0 X110 Y110 F5000"));
				enqueue_and_echo_commands_P(PSTR("G0 Z0 F300"));
				current_button_id=eBT_BUTTON_NONE;
			break;
			// case eBT_MOVE_RETURN:
			// 	enqueue_and_echo_commands_P(PSTR("G0 Z10 F500"));
			// 	current_button_id=eBT_BUTTON_NONE;
			// break;
			case eBT_PR_PLA:
				current_button_id=eBT_BUTTON_NONE;
				if(thermalManager.temp_hotend[0].current<0||thermalManager.temp_bed.current<0)
					break;
				thermalManager.temp_hotend[0].target=PREHEAT_PLA_TEMP_EXTRUDE;
				thermalManager.start_watching_heater(0);
				thermalManager.temp_bed.target=PREHEAT_PLA_TEMP_BED;
				thermalManager.start_watching_bed();
				updatePreheatingTemp();
			//	current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PR_ABS:
				current_button_id=eBT_BUTTON_NONE;
				if(thermalManager.temp_hotend[0].current<0||thermalManager.temp_bed.current<0)
					break;
				thermalManager.temp_hotend[0].target=PREHEAT_ABS_TEMP_EXTRUDE;
				thermalManager.start_watching_heater(0);
				thermalManager.temp_bed.target=PREHEAT_ABS_TEMP_BED;
				thermalManager.start_watching_bed();
				updatePreheatingTemp();
			//	current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PR_PETG:
				current_button_id=eBT_BUTTON_NONE;
				if(thermalManager.temp_hotend[0].current<0||thermalManager.temp_bed.current<0)
					break;
				thermalManager.temp_hotend[0].target=PREHEAT_PETG_TEMP_EXTRUDE;
				thermalManager.start_watching_heater(0);
				thermalManager.temp_bed.target=PREHEAT_PETG_TEMP_BED;
				thermalManager.start_watching_bed();
				updatePreheatingTemp();
			//	current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PR_COOL:
				current_button_id=eBT_BUTTON_NONE;
				if(thermalManager.temp_hotend[0].current>0)
					thermalManager.temp_bed.target=MIN_ADJUST_TEMP_BED;
				if(thermalManager.temp_hotend[0].current>0)
					thermalManager.temp_hotend[0].target=MIN_ADJUST_TEMP_EXTRUDE;
				updatePreheatingTemp();
			//	current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PR_E_PLUS:
				if(setTemperatureInWindow(false, false))
				{
					thermalManager.start_watching_heater(0);
					updatePreheatingTemp();
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PR_E_MINUS:
				if(setTemperatureInWindow(false, true))
				{
					thermalManager.start_watching_heater(0);
					updatePreheatingTemp();
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PR_B_PLUS:
				if(setTemperatureInWindow(true, false))
				{
					thermalManager.start_watching_bed();
					updatePreheatingTemp();
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case EBT_PR_B_MINUS:
				if(setTemperatureInWindow(true, true))
				{
					thermalManager.start_watching_bed();
					updatePreheatingTemp();
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_TEMP_PLUS:
				if(is_bed_select)   //add bed temperature
				{
					if(setTemperatureInWindow(true,false))
						thermalManager.start_watching_bed();
				}
				else            //add extrude  temprature
				{
					if(setTemperatureInWindow(false,false))
						thermalManager.start_watching_heater(0);
				}
				dispalyExtrudeTemp();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_TEMP_MINUS:
				if(is_bed_select)   //subtract bed temperature
				{
					if(setTemperatureInWindow(true,true))
						thermalManager.start_watching_bed();
				}
				else                //subtract extrude temprature
				{
					if(setTemperatureInWindow(false,true))
						thermalManager.start_watching_heater(0);
				}
				dispalyExtrudeTemp();
				current_button_id=eBT_BUTTON_NONE;
			break;
			// case eBT_BED_PLUS:
			// break;
			// case eBT_BED_MINUS:
			// break;
			case eBT_JOG_EPLUS:
				stopAutoFeed();
				if(thermalManager.temp_hotend[0].target<200)
				{
					thermalManager.temp_hotend[0].target=200;
					thermalManager.start_watching_heater(0);
					if(thermalManager.temp_hotend[0].current>195)
					{
						CLEAN_STRING(s_text);
						sprintf((char*)s_text,PSTR("G0 E%d F60"),default_move_distance);
						enqueue_and_echo_commands_P(PSTR("G91"));
						enqueue_and_echo_commands_P(s_text);
						enqueue_and_echo_commands_P(PSTR("G90"));
					}
					if(is_bed_select)
					{
						is_bed_select=false;
						displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);
					}
				}
				else if(thermalManager.temp_hotend[0].current>195)
				{
					CLEAN_STRING(s_text);
					sprintf((char*)s_text,PSTR("G0 E%d F60"),default_move_distance);
					enqueue_and_echo_commands_P(PSTR("G91"));
					enqueue_and_echo_commands_P(s_text);
					enqueue_and_echo_commands_P(PSTR("G90"));
					if(is_bed_select)
					{
						is_bed_select=false;
						displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);
					}
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_JOG_EMINUS:
				stopAutoFeed();
				if(thermalManager.temp_hotend[0].target<200)
				{
					thermalManager.temp_hotend[0].target=200;
					thermalManager.start_watching_heater(0);
					if(thermalManager.temp_hotend[0].current>195)
					{
						CLEAN_STRING(s_text);
						sprintf((char*)s_text,PSTR("G0 E-%d F60"),default_move_distance);
						enqueue_and_echo_commands_P(PSTR("G91"));
						enqueue_and_echo_commands_P(s_text);
						enqueue_and_echo_commands_P(PSTR("G90"));
					}
					if(is_bed_select)
					{
						is_bed_select=false;
						displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);
					}
				}
				else if(thermalManager.temp_hotend[0].current>195)
				{
					CLEAN_STRING(s_text);
					sprintf((char*)s_text,PSTR("G0 E-%d F120"),default_move_distance);
					enqueue_and_echo_commands_P(PSTR("G91"));
					enqueue_and_echo_commands_P(s_text);
					enqueue_and_echo_commands_P(PSTR("G90"));
					if(is_bed_select)
					{
						is_bed_select=false;
						displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);
					}
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_AUTO_EPLUS:
			   if(dir_auto_feed==-1)
			   		stopAutoFeed();
				startAutoFeed(1);
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_AUTO_EMINUS:
				if(dir_auto_feed==1)
					stopAutoFeed();
				startAutoFeed(-1);
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_STOP:
				stopAutoFeed();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_BED_E:
				is_bed_select=!is_bed_select;
				if(is_bed_select)  //bed mode
					displayImage(15, 95, IMG_ADDR_BUTTON_BED_ON);			
				else  //extruder mode
					displayImage(15, 95, IMG_ADDR_BUTTON_BED_OFF);	
				current_button_id=eBT_BUTTON_NONE;
				dispalyExtrudeTemp();			
			break;
			case eBT_DISTANCE_CHANGE:
				switch(current_window_ID)
				{
					case eMENU_MOVE:
						changeMoveDistance(260, 55);
					break;
					case eMENU_PREHEAT:
						changeMoveDistance(260,37);
					break;
					case eMENU_EXTRUDE:case eMENU_ADJUST:case eMENU_ADJUST_MORE:
						changeMoveDistance(260,40);
					break;
					case eMENU_SETTINGS2:
					 	changeMoveDistance(255,43);	
					break;
					default:
					break;
				}
				current_button_id=eBT_BUTTON_NONE;
			break;

			case eBT_FILE_NEXT:
				if(page_index<page_index_max-1)
				{
					page_index++;
					LCD_Fill(0, 25, 239, 174,White);	//clean file list display zone 
					displayFileList();
					displayFilePageNumber();
					if(choose_file_page==page_index&&choose_printfile!=-1)
						CardFile.ChoseFile(choose_printfile);
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_LAST:
				if(page_index>0)
				{
					page_index--;
					LCD_Fill(0, 25, 239, 174,White);
					displayFileList();
					displayFilePageNumber();
					if(choose_file_page==page_index&&choose_printfile!=-1)
						CardFile.ChoseFile(choose_printfile);
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_LIST1:
				if(current_window_ID==eMENU_FILE)
					CardFile.ChoseFile(0);
				else
					ChoseArgument(0);
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_LIST2:
				if(current_window_ID==eMENU_FILE)
					CardFile.ChoseFile(1);
				else
					ChoseArgument(1);	
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_LIST3:
				if(current_window_ID==eMENU_FILE)
					CardFile.ChoseFile(2);
				else
					ChoseArgument(2);
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_LIST4:
				if(current_window_ID==eMENU_FILE)
					CardFile.ChoseFile(3);
				else
					ChoseArgument(3);
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_LIST5:
				if(current_window_ID==eMENU_FILE)
					CardFile.ChoseFile(4);
				else
					ChoseArgument(4);
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_OPEN:
				current_button_id=eBT_BUTTON_NONE;
				if(choose_printfile==-1||(choose_printfile!=-1&&(choose_file_page!=(page_index)))
					)
				{
					//current_button_id=eBT_BUTTON_NONE;
					break;
				}
				if(list_order)
					card.getfilename(choose_file_page*5+choose_printfile);
				else
					card.getfilename(file_count-(choose_file_page)*5-choose_printfile-1);
				// Serial1.print(CardFile.getsdfilename(choose_file_page*5+choose_printfile));
				 Serial1.println(card.longFilename);
				if(CardFile.filenameIsFolder)
				{
					card.chdir(card.filename);
					if(card.curWorkDirDepth!=MAX_DIR_DEPTH)
					{
						clearfilevar();
						LCD_Fill(0, 25, 239, 174,White);
						if((SDIO_GetCardState()==SDIO_CARD_ERROR))
							displayPromptSDCardError();
						else
						{
							file_count=card.getnrfilenames();
							if(file_count==0)
							{
								displayPromptEmptyFolder();
								return;
							}
							page_index_max=CardFile.getsdfilepage();
							displayFileList();
							displayFilePageNumber();
						}
					}
				}
				else
				{
					uint8_t check_cn=0;
					for(int i=CHECK_FILAMENT_TIMES;i>=0;i--)
					{
						if(READ(FIL_RUNOUT_PIN))
							check_cn++;
					}
					if(!check_filament_disable&&(check_cn>(CHECK_FILAMENT_TIMES-1)))
					{
						dispalyDialogYesNo(eDIALOG_START_JOB_NOFILA);
						current_window_ID=eMENU_DIALOG_NO_FIL;
						extrude2file=true;
					}
					else
					{
						dispalyDialogYesNo(eDIALOG_PRINT_START);
						current_window_ID=eMENU_DIALOG_START;
					}
				}
				//current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_FILE_FOLDER:
				current_button_id=eBT_BUTTON_NONE;
			card.updir();
			if(card.curWorkDirDepth!=MAX_DIR_DEPTH)
			{
				clearfilevar();
				LCD_Fill(0, 25, 239, 174,White);
				if((SDIO_GetCardState()==SDIO_CARD_ERROR))
					displayPromptSDCardError();
				else
				{
					file_count=card.getnrfilenames();
					if(file_count==0)
					{
						displayPromptEmptyFolder();
						return;
					}
					page_index_max=CardFile.getsdfilepage();
					displayFileList();
					displayFilePageNumber();
				}
			}
			break;

			case eBT_PRINT_PAUSE:
				switch(current_print_cmd)
				{
					case E_PRINT_DISPAUSE:
					break;
					case E_PRINT_PAUSE:
						enqueue_and_echo_commands_P((PSTR("M25")));
						LCD_Fill(260,30,320,90,White);		//clean pause/resume icon display zone
						displayImage(260, 30, IMG_ADDR_BUTTON_RESUME);	
						cur_pstatus=2;	
						current_print_cmd=E_PRINT_CMD_NONE;
						displayPause();
					break;
					case E_PRINT_RESUME:
						enqueue_and_echo_commands_P((PSTR("M24")));
						LCD_Fill(260,30,320,90,White);		//clean pause/resume icon display zone
						displayImage(260, 30, IMG_ADDR_BUTTON_PAUSE);	
						cur_pstatus=1;	
						current_print_cmd=E_PRINT_CMD_NONE;
						displayPrinting();
					break;
					default:
					break;
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PRINT_ADJUST:
				next_window_ID=eMENU_ADJUST;
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_PRINT_END:
				if(is_print_finish)
				{
					clearVarPrintEnd();
					displayWindowHome();
					current_window_ID=eMENU_HOME;
				}
				else
				{
					dispalyDialogYesNo(eDIALOG_PRINT_ABORT);
					current_window_ID=eMENU_DIALOG_END;
				}

				current_button_id=eBT_BUTTON_NONE;
			break;

			case eBT_ADJUSTE_PLUS:
				if(current_window_ID==eMENU_ADJUST)   //add e temp
				{
					if(setTemperatureInWindow(false,false))
						dispalyAdjustTemp();
				}
				else     //subtract flow
				{
					planner.flow_percentage[0]+=default_move_distance;
					if(planner.flow_percentage[0]>999)
						planner.flow_percentage[0]=999;
					dispalyAdjustFlow();
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTE_MINUS:
				if(current_window_ID==eMENU_ADJUST)   //subtract e temp
				{
					if(setTemperatureInWindow(false,true))
						dispalyAdjustTemp();
				}
				else     //subtract flow
				{
					planner.flow_percentage[0]-=default_move_distance;
					if(planner.flow_percentage[0]<10)
						planner.flow_percentage[0]=0;
					dispalyAdjustFlow();
				}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTBED_PLUS:
				if(setTemperatureInWindow(true,false))
					dispalyAdjustTemp();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTBED_MINUS:
				if(setTemperatureInWindow(true,true))
					dispalyAdjustTemp();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTFAN_PLUS:
				cur_fanspeed=thermalManager.fan_speed[0];
				cur_fanspeed+=default_move_distance;
				if(cur_fanspeed>255)
					cur_fanspeed=255;
				thermalManager.fan_speed[0]=cur_fanspeed;
				dispalyAdjustFanSpeed();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTFAN_MINUS:
				cur_fanspeed=thermalManager.fan_speed[0];
				cur_fanspeed-=default_move_distance;
				if(cur_fanspeed<0)
					cur_fanspeed=0;
				thermalManager.fan_speed[0]=cur_fanspeed;
				dispalyAdjustFanSpeed();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTSPEED_PLUS:
		//	Serial1.println(default_parameter[0]);
				feedrate_percentage+=default_move_distance;
				if(feedrate_percentage>999)
					feedrate_percentage=999;
				dispalyAdjustMoveSpeed();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_ADJUSTSPEED_MINUS:
				feedrate_percentage-=default_move_distance;
				if(feedrate_percentage<10)
					feedrate_percentage=10;
				dispalyAdjustMoveSpeed();
				current_button_id=eBT_BUTTON_NONE;
			break;


			case eBT_DIALOG_PRINT_START:
			 Serial1.println(card.filename);
			if(current_window_ID==eMENU_DIALOG_NO_FIL)
			{
				displayWindowExtrude();
				current_window_ID=eMENU_EXTRUDE;
			}
			else
			{
				is_printing=true;
				is_print_finish=cur_flag=false;
				cur_ppage=0;cur_pstatus=0;
				if(current_window_ID==eMENU_DIALOG_START)
					recovery_flag=false;
				if(recovery_flag==false)
				{
					#if ENABLED(POWER_LOSS_RECOVERY)
          				card.removeJobRecoveryFile();
        			#endif
				//	card.openAndPrintFile(card.filename);	
					char cmd[4+ strlen(card.filename) + 1];
					sprintf_P(cmd, PSTR("M23 %s"),card.filename);
					enqueue_and_echo_command_now(cmd);
  					enqueue_and_echo_commands_P(PSTR("M24"));
					W25QxxFlash.W25QXX_Write((uint8_t*)card.longFilename,SAVE_FILE_ADDR,(uint16_t)sizeof(card.longFilename));
				}
				else   //recovery
				{
					enqueue_and_echo_commands_P(PSTR("M1000"));
					recovery_flag=false;
					W25QxxFlash.W25QXX_Read((uint8_t*)card.longFilename,SAVE_FILE_ADDR,(uint16_t)sizeof(card.longFilename));
				}
				displayWindowPrint();
				current_window_ID=eMENU_PRINT;
			}
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_DIALOG_PRINT_NO:
				next_window_ID=eMENU_FILE1;
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_DIALOG_REFACTORY_YES:
				current_button_id=eBT_BUTTON_NONE;
				current_window_ID=eMENU_SETTINGS;
				ConfigSettings.restoreDefaultSettings();
				page_index=0;
				is_setting_change=true;
				displayWindowSettings();
			break;
			case eBT_DIALOG_SAVE_YES:
				current_button_id=eBT_BUTTON_NONE;
				FLASH_WRITE_VAR(SAVE_ADDR_SETTINGS, re_settings);
				dispalyDialogYes(eDIALOG_SETTS_SAVE_OK);
				current_window_ID=eMENU_DIALOG_SAVE_OK;
				//ConfigSettings.settingsReset();
				ConfigSettings.settingsLoad();
				is_setting_change=false;
			break;
			case eBT_SETTING_ADJUST:
				current_button_id=eBT_BUTTON_NONE;
				if(choose_setting>=ARGUMENST_MAX_NUM) break;
				current_window_ID=eMENU_SETTINGS2;
				displayWindowSettings2();
				// change_window = true ;
				// highlightSelectedIcon(255,30,55,55);	
				// displayImage(255, 30, pic_address_button_modify);
			break;
			case eBT_SETTING_REFACTORY:
				dispalyDialogYesNo(eDIALOG_SETTS_RESTORE);
				current_window_ID=eMENU_DIALOG_REFACTORY;
				//ConfigSettings.restoreDefaultSettings();
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_SETTING_SAVE:
				current_button_id=eBT_BUTTON_NONE;
				dispalyDialogYesNo(eDIALOG_SETTS_SAVE);
				current_window_ID=eMENU_DIALOG_SAVE;
			break;
			case eBT_SETTING_LAST:
				if(page_index > 0)
				{
					page_index = page_index - 1;
				//	choose_printfile=0;
					LCD_Fill(0, 25, 239, 174,White);	//clean file list display zone 
					displayArgumentList();
					displayArugumentPageNumber();	
					displayImage(5, 180, IMG_ADDR_BUTTON_PAGE_LAST);
				}
				 choose_setting=ARGUMENST_MAX_NUM;
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_SETTING_NEXT:
				if(page_index < ARGUMENST_MAX_PAGE)
				{
					page_index = page_index + 1;
				//	choose_printfile=0; 	 
					LCD_Fill(0, 25, 239, 174,White);	//clean file list display zone 
					displayArgumentList();
					displayArugumentPageNumber();
					displayImage(101, 180, IMG_ADDR_BUTTON_PAGE_NEXT);
				}
				 choose_setting=ARGUMENST_MAX_NUM;
				current_button_id=eBT_BUTTON_NONE;
			break;
			case eBT_SETTING_ADD:
				current_button_id=eBT_BUTTON_NONE;
				changeValueArgument(choose_setting, 1);
				displayModifyArgument();
				is_setting_change=true;
			break;
			case eBT_SETTING_SUB:
				current_button_id=eBT_BUTTON_NONE;
				changeValueArgument(choose_setting, -1);
				displayModifyArgument();
				is_setting_change=true;
			break;
			case eBT_BUTTON_NONE:
			default:
				current_button_id=eBT_BUTTON_NONE;
			break;
		}
}
millis_t getxydelay=0;
void display_image::LGT_Main_Function(void)
{
	if(isTouched())
	{
			//getxydelay=millis();
		if(millis()>=getxydelay)
		{
			getxydelay=millis()+100;
			if(next_window_ID==eWINDOW_NONE&&current_button_id==eBT_BUTTON_NONE)
			{
				//getTouchPoint(&X_AD,&Y_AD);
				if(getTouchPoint(&X_AD,&Y_AD))
				{
					cur_x=(uint16_t)((((int32_t)X_AD * (int32_t)xCalibration) >> 16) + xOffset);
					cur_y=(uint16_t)((((int32_t)Y_AD* (int32_t)yCalibration) >> 16) + yOffset);
					if(cur_x>320||cur_y>240)
						return;
					Serial1.print("x=");Serial1.println(cur_x);
					Serial1.print("y=");Serial1.println(cur_y);
					LGT_MainScanWindow();
				}
			}
		}
	}
	else
	{
		if(LGT_Ui_Update())
			LGT_Ui_Buttoncmd();
		LGT_Printer_Data_Update();
		if(is_printing&&!check_filament_disable)   //check_filament_disable: false --open filament check
			checkFilament();
	}
}
#endif