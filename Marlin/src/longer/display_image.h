#pragma once
#include "../inc/MarlinConfig.h"

#ifdef LONGER3D
//#include "../../lcd/menu/touch/xpt2046.h"
#include "gbk_font16.h"
enum E_PRINT_CMD{
	E_PRINT_CMD_NONE=0,
	E_PRINT_DISPAUSE,  //disable pause
	E_PRINT_PAUSE,
	E_PRINT_RESUME,
};
enum E_BUTTON_KEY {
/*******************Move page****************************/
	eBT_BUTTON_NONE=0,
	eBT_MOVE_X_MINUS,eBT_MOVE_X_PLUS,eBT_MOVE_X_HOME,
	eBT_MOVE_Y_MINUS,eBT_MOVE_Y_PLUS,eBT_MOVE_Y_HOME,
	eBT_MOVE_Z_MINUS,eBT_MOVE_Z_PLUS,eBT_MOVE_Z_HOME,
	eBT_MOVE_ALL_HOME,

/*******************Leveling page****************************/
	eBT_MOVE_L0,eBT_MOVE_L1,eBT_MOVE_L2,eBT_MOVE_L3,eBT_MOVE_L4, eBT_MOVE_RETURN,

/*******************preheating page****************************/
	eBT_PR_PLA,eBT_PR_ABS,eBT_PR_PETG,eBT_PR_COOL,
	eBT_PR_E_PLUS,eBT_PR_E_MINUS,eBT_PR_B_PLUS,EBT_PR_B_MINUS,

/*******************Extrude page****************************/	
	eBT_TEMP_PLUS,eBT_TEMP_MINUS,//eBT_BED_PLUS,eBT_BED_MINUS,
	eBT_JOG_EPLUS,eBT_JOG_EMINUS,eBT_AUTO_EPLUS,eBT_AUTO_EMINUS,eBT_STOP,eBT_BED_E,

/*******************File page****************************/	
	eBT_FILE_NEXT,eBT_FILE_LAST,eBT_FILE_OPEN,eBT_FILE_FOLDER,
	eBT_FILE_LIST1,eBT_FILE_LIST2,eBT_FILE_LIST3,eBT_FILE_LIST4,eBT_FILE_LIST5,

/*******************printing page****************************/	
	eBT_PRINT_PAUSE,eBT_PRINT_ADJUST,eBT_PRINT_END,

/*******************Adjust page****************************/
	eBT_ADJUSTE_PLUS,eBT_ADJUSTE_MINUS,eBT_ADJUSTBED_PLUS,eBT_ADJUSTBED_MINUS,
	eBT_ADJUSTFAN_PLUS,eBT_ADJUSTFAN_MINUS,eBT_ADJUSTSPEED_PLUS,eBT_ADJUSTSPEED_MINUS,

/****************************Dialog page*******************************/
	eBT_DIALOG_PRINT_START,eBT_DIALOG_PRINT_NO,eBT_DIALOG_REFACTORY_YES,eBT_DIALOG_SAVE_YES,
	
/****************************Settings page*******************************/
	eBT_SETTING_ADJUST,eBT_SETTING_REFACTORY,eBT_SETTING_SAVE,eBT_SETTING_LAST,eBT_SETTING_NEXT,eBT_SETTING_ADD,eBT_SETTING_SUB,

	eBT_DISTANCE_CHANGE
};

typedef enum{

	eMENU_HOME = 0,
    eMENU_HOME_MORE,
	eMENU_MOVE,
	eMENU_FILE,
	eMENU_FILE1,   //next_window_id=eMENU_FILE1 when click eBT_DIALOG_PRINT_NO
	eMENU_PRINT,
	eMENU_ADJUST,
	eMENU_ADJUST_MORE,
	eMENU_PREHEAT,
	eMENU_LEVELING,
	eMENU_EXTRUDE,
	eMENU_SETTINGS,
	eMENU_SETTINGS2,
	eMENU_ABOUT,
	eMENU_DIALOG_START,   //print or not print
	eMENU_DIALOG_END,     //stop print or not stop print
	eMENU_DIALOG_NO_FIL,   // no filament page
	eMENU_DIALOG_RECOVERY,
	eMENU_DIALOG_REFACTORY,
	eMENU_DIALOG_SAVE,
	eMENU_DIALOG_SAVE_OK,
	eMENU_DIALOG_ERRORTEMPBED,
	eMENU_DIALOG_ERRORTEMPE,
// 	eMENU_PCB_TEST,
// 	eMENU_SCREEN_CALIBRATION,
// 	eMENU_AGING_TEST,
// #ifdef LCD_COLOR_TEST
// 	eMENU_SCREEN_COLOR_TEST,
// #endif	

 	eWINDOW_MAX ,
	eWINDOW_NONE
}E_WINDOW_ID;

typedef enum{
	
	eDIALOG_PRINT_START = 0,
	eDIALOG_PRINT_EXIT,
	eDIALOG_PRINT_ABORT,
	eDIALOG_PRINT_RECOVERY,
	eDIALOG_ERROR_READ,
	eDIALOG_SETTS_RESTORE,
	eDIALOG_SETTS_SAVE_OK,
	eDIALOG_SETTS_SAVE,
	eDIALOG_NO_FILAMENT,
	eDIALOG_ERROR_FILE_TYPE,
	eDIALOG_ERROR_TEMP_BED,
	eDIALOG_ERROR_TEMP_HEAD,
	eDIALOG_FILE_MAX_FOLDER,
	eDIALOG_START_JOB_NOFILA,
	eDIALOG_MAX
	
}EDIALOG;

typedef struct _HEADCOLOR
{
   unsigned char scan;
   unsigned char gray;
   unsigned short w;
   unsigned short h;
   unsigned char is565;
   unsigned char rgb;
}HEADCOLOR; 

class display_image
{
public:
bool  extrude2file=false;
display_image(/* args */);
 ~display_image();
bool setTemperatureInWindow(bool is_bed, bool sign);
void changeMoveDistance(uint16_t pos_x, uint16_t pos_y);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void initialMoveDistance(uint16_t pos_x, uint16_t pos_y);
void displayImage(uint16_t x_start, uint16_t y_start, uint32_t addr);
void image_show(uint16_t xsta,uint16_t ysta,uint16_t width,uint16_t high,uint8_t *p);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);

void displayMoveCoordinate(void);

//void scanWindowAdjust( uint16_t rv_x, uint16_t rv_y );

/***************************Move page*******************************************/
void scanWindowMove( uint16_t rv_x, uint16_t rv_y );

/***************************File page*******************************************/
void scanWindowFile( uint16_t rv_x, uint16_t rv_y );
void displayFilePageNumber(void);
void displayFileList();
void displayChosenFile(void);
void displayPromptSDCardError(void);
void displayPromptEmptyFolder(void);

/***************************Extrude page*******************************************/
void scanWindowExtrude( uint16_t rv_x, uint16_t rv_y );
void dispalyExtrudeTemp(void);
void displayRunningAutoFeed(void);

/***************************preheating page*******************************************/
void scanWindowPreheating( uint16_t rv_x, uint16_t rv_y );
void updatePreheatingTemp(void);

/***************************More page*******************************************/
void scanWindowMoreHome(uint16_t rv_x, uint16_t rv_y);
void scanWindowLeveling( uint16_t rv_x, uint16_t rv_y );
void scanWindowAbout(uint16_t rv_x, uint16_t rv_y);
void displayWindowLeveling(void);
void displayWindowAbout(void);

void displayArugumentPageNumber(void);
void displayArgumentList(void);
void displayWindowSettings(void);
void scanWindowSettings(uint16_t rv_x, uint16_t rv_y);
void displayWindowSettings2(void);
void displayModifyArgument(void);
void scanWindowSettings2(uint16_t rv_x, uint16_t rv_y);


/***************************Start page*******************************************/
void displayStartUpLogo(void);
void displayWindowHome(void);

/***************************Home page*******************************************/
void scanWindowHome(uint16_t rv_x, uint16_t rv_y);
void displayWindowMove(void);
void displayWindowFiles(void);
void displayWindowExtrude(void);
void displayWindowPreheat(void);
void displayWindowHomeMore(void);


/***************************Printing page*******************************************/
void scanWindowPrint( uint16_t rv_x, uint16_t rv_y );
void displayWindowPrint(void);
void displayPrintInformation(void);
void displayRunningFan(uint16_t pos_x, uint16_t pos_y);
void displayFanSpeed(void);
void displayHeating(void);
void displayPrinting(void);
void displayPause(void);
void displayPrintTemperature(void);
void displayPrintProgress(void);
void displayHeightValue(void);
void dispalyCurrentStatus(void);
//void calcCountUpTime(void);
void displayCountUpTime(void);
void displayCountDownTime(void);

/***************************Adjust page*******************************************/
void displayWindowAdjust(void);
void dispalyAdjustTemp(void);
void dispalyAdjustFanSpeed(void);
void dispalyAdjustMoveSpeed(void);
void scanWindowAdjust(uint16_t rv_x,uint16_t rv_y);
void dispalyAdjustFlow(void);
void displayWindowAdjustMore(void);
void scanWindowAdjustMore(uint16_t rv_x,uint16_t rv_y);

/***************************dialog page*******************************************/
void dispalyDialogYesNo(uint8_t dialog_index);
void dispalyDialogYes(uint8_t dialog_index);
void displayDialogText(uint8_t dialog_index);
void scanDialogStart(uint16_t rv_x, uint16_t rv_y );
void scanDialogEnd( uint16_t rv_x, uint16_t rv_y ); 
void displayNofilament(void);
void scanDialogRecovery( uint16_t rv_x, uint16_t rv_y);
void scanDialogRefactory(uint16_t rv_x, uint16_t rv_y);
void scanDialogSave(uint16_t rv_x, uint16_t rv_y);
void scanDialogYes(uint16_t rv_x, uint16_t rv_y);

void LGT_Printerabnormal_Kill(const char* info);
void LGT_Tempabnormal_Warning(const char* info);    //is_printing=false
void LGT_lcd_init();
bool LGT_MainScanWindow(void);
bool LGT_Ui_Update(void);
void LGT_Printer_Data_Update(void);
void LGT_Ui_Buttoncmd(void);
void LGT_Main_Function(void);
};
void LGT_Line_To_Current_Position(AxisEnum axis);
void actAutoFeed(void);
void stopAutoFeed(void);
void startAutoFeed(int8_t dir);
void clearVarPrintEnd();
void clearfilevar();
void actExceedTemperScope(uint8_t e, bool is_hot);
void checkTemprature(void);
void checkScreenCalibrate(void);
void convertArgu2Str(uint8_t i, char* p);
void ChoseArgument(uint8_t index);
void changeValueArgument(uint8_t i,int8_t inc_sign);
void *getArgumentAddr(uint8_t i);
float getDistanceMultiplier(uint8_t i);

#define LCD_SetCursor(x,y)                ILI9341_SetCursor(x,y)
#define LCD_WriteRAM_Prepare()            ILI9341_WriteRam()
#define LCD_WriteValue_LCD_RAM(RegValue)  TOUCH_LCD_IO_WriteData(RegValue)  
#ifndef Chinese
	#define LCD_ShowString(x,y,test)          lcdPrint(x,y,test) 
#else
	#define LCD_ShowString(x,y,test)          lcd_print_gbk(x,y,test)
#endif

#define CLEAN_STRING(str)				   		memset((uint8_t*)str,0,sizeof(str))
#define SHOW_SINGLE_TXT(str, x, y, pt_color)    showText((char *)str, pt_color, WHITE, false, (uint16_t)(x), (uint16_t)(y))
#define SHOW_BLACK_SINGLE_TXT(str, x, y)        SHOW_SINGLE_TXT(str, x, y, BLACK)
#define FILL_ZONE(x, y, w, h, bg_color)         LCD_Fill((uint16_t)(x), (uint16_t)(y), (uint16_t)((x)+(w)-1), (uint16_t)((y)+(h)-1), (uint16_t)bg_color)
#define CLEAN_ZONE(x, y, w, h)                  FILL_ZONE(x, y, w, h, WHITE)
#define CLEAN_SINGLE_TXT(x, y, w)               CLEAN_ZONE(x, y, w, 16)     /* clean single line text */

#endif