#include "sdfile.h"
#ifdef LONGER3D
#include "../../sd/cardreader.h"
//#include "../../lcd/menu/touch/xpt2046.h"
#include "../../lcd/menu/touch/lcd.h"
#include "display_image.h"
#include "../../gcode/queue.h"
#include "../../libs/stopwatch.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "LGT_MACRO.h"
#include "W25Qxx.h"
#include "../../module/temperature.h"
#include "config_storedata.h"

extern bool list_order;
extern Temperature thermalManager;
extern  W25Qxx W25QxxFlash;
extern Planner planner;
extern Stopwatch print_job_timer;
extern CardReader card;
extern uint16_t page_index,file_count;
extern display_image Display_Screen;
extern uint8_t page_index_num;
extern int8_t choose_printfile;
extern uint16_t choose_file_page;
extern uint16_t color,bgColor;
extern char pdata[64];
extern uint32_t print_times;
uint8_t cp_time_minutes=0, cp_time_seconds=0;	
uint16_t cp_time_hours =0;
uint16_t Remaining_hours=0;
uint8_t Remaining_minutes=0;
bool is_filament_check_disable=false;
uint8_t check_filament=0;
float resume_xyze_position[XYZE]={0.0};
float save_feedtate = 0.0;
sdfile::sdfile(/* args */)
{
}
sdfile::~sdfile()
{
}
 uint16_t sdfile::getsdfilecount()
 {
     return card.getnrfilenames();
 }
uint16_t sdfile::getsdfilepage()
{ 
    uint16_t PageCnt=0;
    const uint16_t FileCnt=card.getnrfilenames();
    PageCnt=FileCnt/5;
    if(FileCnt%5!=0)
        PageCnt+=1;
    return PageCnt;
}
char* sdfile::getsdfilename(uint16_t nr)
{
     card.getfilename(nr);
      filenameIsFolder=card.flag.filenameIsDir;
    // filenameIsFolde=DIR_IS_SUBDIR(card.filename);
    if(filenameIsFolder)
    {
        return card.filename;
    }
    else
    {
        uint8_t filenamelen=strlen(card.longFilename);
        if(filenamelen>25)
        {
            card.longFilename[25]='\0';
            card.longFilename[26]='\0';
        }
        return card.longFilename;
    }
}

char* sdfile::getprintfilename(void)
{
    return card.longFilename;
}
void sdfile::ChoseFile(uint8_t index)
{
    if((page_index*5 + index) >= file_count) 
        return;
    choose_printfile=index;
    choose_file_page=page_index;  //page_index start from 0
    Display_Screen.LCD_Fill(35, 25+page_index_num*30, 239, 55 - 1 + page_index_num*30,WHITE);
    if(list_order)
        LCD_ShowString(35,32+page_index_num*30,getsdfilename(page_index*5+page_index_num));   //page_index start from 0   file_count-(page_index)*5-n-1
    else 
        LCD_ShowString(35,32+page_index_num*30,getsdfilename(file_count-(page_index)*5-page_index_num-1));
    page_index_num=index;
    Display_Screen.LCD_Fill(35, 25+page_index_num*30, 239, 55 - 1 + page_index_num*30,DARKBLUE);
    //page_index_num=index;
    color=WHITE,bgColor=DARKBLUE;
    if(list_order)
        LCD_ShowString(35,32+page_index_num*30,getsdfilename(page_index*5+index));   //page_index start from 0
    else
         LCD_ShowString(35,32+page_index_num*30,getsdfilename(file_count-(page_index)*5-page_index_num-1));   //page_index start from 0
    color=BLACK,bgColor=WHITE;
}

bool sdfile::sdcardcheckstatus()
{
    card.initsd();
    return card.flag.detected;
}

void sdfile::sdopenAndPrintFile()
{
  char cmd[4 + strlen(card.filename) + 1]; // Room for "M23 ", filename, and null
  sprintf_P(cmd, PSTR("M23 %s"), card.filename);
  for (char *c = &cmd[4]; *c; c++) *c = tolower(*c);
  enqueue_and_echo_command_now(cmd);
  enqueue_and_echo_commands_P(PSTR("M24"));
Serial1.print(card.filename);
Serial1.println();
Serial1.print(card.longFilename);
}
void sdfile::getfiletotaltime()
{
    int16_t n=0;
    memset(pdata,0,sizeof(pdata));
    pdata[0]=';';
    for(int i=1;i<37;i++)
    {
        n=card.get();
       if(n!=-1)
            pdata[i]=n;
    }
    if(strstr(&pdata[1],"TIME:") != NULL)
    {
		actPrintTimeCalculate();
        is_gettime=true;
	}
    else if(strstr(&pdata[1],"Print time") != NULL)
    { 
	    actPrintTimeCalculateLegacy();
        is_gettime=true;
    }
}

 void sdfile::actPrintTimeCalculateLegacy(void)
 {
    uint16_t print_hours=0;
	uint8_t print_minutes=0;	
	if(strstr(pdata,"hour") != NULL)
    {
		if(ReadTF_code_seen(pdata,':'))
			print_hours = ReadTF_code_value(pdata);
		if(print_hours==1)
        {
			if(ReadTF_code_seen(pdata,'r')) print_minutes = ReadTF_code_value1(pdata) ; //1 hour ??
		}
        else
        {
			if(ReadTF_code_seen(pdata,'s')) print_minutes = ReadTF_code_value(pdata) ;  //n hours  ??
		}		
	}
    else //if(strstr(pdata,"minutes") != NULL)
    {
		if(ReadTF_code_seen(pdata,':'))
        {
			print_minutes = ReadTF_code_value(pdata);
		}
		print_hours=0;	
	}
	print_times = print_hours * 60 + print_minutes;
    Serial1.println(print_times);
 }
  void sdfile::actPrintTimeCalculate(void)
  {
    char *p;
	uint32_t temp_second = 0;
    p = sdcode_seen(':', &pdata[1]);
	if(p != NULL){
		p++;
		temp_second = (uint32_t)sdcode_value(p);
		print_times = temp_second / 60;
	}
  }
  void sdfile::calcCountUpTime(void)
{
	cp_time_hours =print_job_timer.duration() /3600;
	cp_time_minutes = print_job_timer.duration() % 3600 / 60;
	cp_time_seconds = print_job_timer.duration() % 60;
}
void sdfile::calcCountDownTime(void)
{
    uint32_t Elapsed_time;
    if(print_times==0)  return;
    Elapsed_time= print_times*(1.0 - card.percentfloatDone());
    //Serial1.println(Elapsed_time);
    Remaining_hours=Elapsed_time/60;
    Remaining_minutes=Elapsed_time %60;
    
}

void checkFilament(void)
{
    if(is_filament_check_disable)  
        return;
    // Serial1.println("la");   
    if(READ(FIL_RUNOUT_PIN)&&card.flag.sdprinting)   //no filament
    {
        if(check_filament==0)
        {
            enqueue_and_echo_commands_P(PSTR("M25"));
            is_filament_check_disable=true;
            return;
        }
        check_filament--;
        Serial1.println("check");
    }
}
void checkFilamentReset(void)
{
    check_filament=CHECK_FILAMENT_TIMES;
    is_filament_check_disable=false;
}
void savePausePositonMove(void)
{
    save_feedtate=feedrate_mm_s;
    resume_xyze_position[X_AXIS]=current_position[X_AXIS];
    resume_xyze_position[Y_AXIS]=current_position[Y_AXIS];
    resume_xyze_position[E_AXIS]=current_position[E_AXIS];
    current_position[E_AXIS]-=4;
    LGT_Line_To_Current_Position(E_AXIS);
    do_blocking_move_to_xy(FILAMENT_RUNOUT_MOVE_X,FILAMENT_RUNOUT_MOVE_Y,25);  //
    planner.synchronize();
}

// bool s_is_save = true; 
// uint32_t s_saveblock_addr = SAVE_FILE_ADDR;
// save_recovery_info savedata;
// extern E_WINDOW_ID next_window_ID;
// uint32_t getFileSaveBlockAddr(void)
// {
//     uint32_t at_addr = SAVE_FILE_ADDR;
//     if(s_is_save)
//     {
//         W25QxxFlash.W25QXX_Read( (uint8_t*)&at_addr, SAVE_BASE_ADDR, sizeof(uint32_t));
//         if( at_addr >= SAVE_MAX )  //地址用完
//         {		
//             s_saveblock_addr = SAVE_FILE_ADDR;  //从首地址开始
//             W25QxxFlash.W25QXX_Write((uint8_t *)&s_saveblock_addr, SAVE_BASE_ADDR, sizeof(uint32_t));
//             return SAVE_FILE_ADDR;
//         }
//         else if( at_addr >= SAVE_FILE_ADDR ) //地址正常
//         {		
//             s_saveblock_addr = at_addr;
//         }
//         else  //地址错误
//         {
//             return 0x00;
//         }
//     }
//     return at_addr;
// }
// void getSaveData( char* data )
// {
//     W25QxxFlash.W25QXX_Read((uint8_t*)data,getFileSaveBlockAddr(),sizeof(savedata));
// }
// void setFlashSave( bool set)
// {
//     s_is_save=set;
// }
// void saveRecoveryData(void)
// {
//     if(s_is_save)
//     {
//         uint8_t i;
//         getSaveData((char*)&savedata);
//         if(savedata.wtimes==0xFFFFFFFF)
//         {
//             savedata.wtimes = 0;
//         }
//         if(savedata.wtimes>FLASH_WRITETIMES_MAX)  
//         {
//             s_saveblock_addr += FLASH_PAGE_SIZE;
//             W25QxxFlash.W25QXX_Write((uint8_t *)&s_saveblock_addr, SAVE_BASE_ADDR, sizeof(uint32_t));
//             savedata.wtimes = 0;
//         }
//         else
//         {
//             savedata.wtimes+=1;
//         }
//         savedata.zheight=current_position[Z_AXIS];
//         card.getAbsFilename(savedata.sd_filename);
//         savedata.filepos=card.getIndex();
//         savedata.feedrate=uint16_t(feedrate_mm_s * 60.0f);
//         for(i=0;i<HOTENDS;i++)
//             savedata.tgetemp[i]=thermalManager.temp_hotend[i].target;
//         savedata.tgbedtemp=thermalManager.temp_bed.target;
//         savedata.fanspeed=thermalManager.fan_speed[0];
//         savedata.print_job_elapsed=print_job_timer.duration();
//         savedata.totaltime=print_times;
//         W25QxxFlash.W25QXX_Write( (uint8_t*)&savedata,s_saveblock_addr, sizeof(savedata));
//     }
// }
// void checkRecoveryStatus(void)
// {
//     getSaveData((char*)&savedata);
//     if(savedata.filepos!=0xffffffff||savedata.filepos!=0)
//     {
//         next_window_ID=eMENU_DIALOG_RECOVERY;;
//     }

// }
// void recoveryResume(void)
// {
//     char cmd[16];
//     memset(cmd,0,sizeof(cmd));
//     if(savedata.tgbedtemp>0)
//     {
//          sprintf_P(cmd, PSTR("M190 S%i"), savedata.tgbedtemp);
//          enqueue_and_echo_commands_now_P(PSTR(cmd));
//     }
// }
#endif