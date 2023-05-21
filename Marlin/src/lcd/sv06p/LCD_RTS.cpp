#include "../../inc/MarlinConfig.h"

#if ENABLED(RTS_AVAILABLE)

#include "LCD_RTS.h"
#include <WString.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "../../../Version.h"
#include "../../MarlinCore.h"
#include "../../core/serial.h"
#include "../../core/macros.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/settings.h"
#include "../../module/motion.h"
#include "../../module/printcounter.h"
#include "../../libs/duration_t.h"
#include "../../feature/babystep.h"
#include "../../feature/powerloss.h"
#include "../../feature/bedlevel/bedlevel.h"
//#include "../../feature/bedlevel/abl/bbl.h"
#include "../../feature/tmc_util.h"
#include "../../gcode/parser.h"
#include "../../gcode/queue.h"
#include "../../gcode/gcode.h"
#include "../marlinui.h"
#include "../utf8.h"
#include "../../libs/BL24CXX.h"

#if ENABLED(FIX_MOUNTED_PROBE)
  #include "../../module/endstops.h"
#endif
#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif
#define CHECKFILEMENT false

float zprobe_zoffset;
float last_zoffset = 0.0;

const float custom_feedrate_mm_m[] = { 50*60, 50*60, 4*60, 60 };

int startprogress = 0;
CRec CardRecbuf;
float pause_z = 0;
float pause_e = 0;
bool sdcard_pause_check = true;
bool print_preheat_check = false;

float ChangeFilament0Temp = 200;

int StartFlag = 0;
int PrintFlag = 0;
int StartPrintFlag = 0;

int heatway = 0;
millis_t next_rts_update_ms = 0;
int last_target_temperature[4] = {0};
int last_target_temperature_bed;
char waitway = 0;
int recnum = 0;
unsigned char Percentrecord = 0;

bool pause_action_flag = false;
bool pause_flag = false;
int power_off_type_yes = 0;
// represents to update file list
bool CardUpdate = false;

extern CardReader card;
// represents SD-card status, true means SD is available, false means opposite.
bool lcd_sd_status;

char Checkfilenum = 0;
int FilenamesCount = 0;
char cmdbuf[20] = {0};
float Filament0LOAD = 10;
float XoffsetValue ;

// 0 for 10mm, 1 for 1mm, 2 for 0.1mm
unsigned char AxisUnitMode;
float axis_unit = 10;
RTSSHOW rtscheck;
int Update_Time_Value = 0;

bool PoweroffContinue = false;
char commandbuf[30];

static int change_page_number = 0;

uint16_t remain_time = 0;

static bool last_card_insert_st;
bool card_insert_st;
bool sd_printing;

bool Mode_flag = false;
int fan_speed;
char cmd[MAX_CMD_SIZE+16];
inline void RTS_line_to_current(AxisEnum axis)
{
  if (!planner.is_full())
  {
    planner.buffer_line(current_position, MMM_TO_MMS(custom_feedrate_mm_m[(int8_t)axis]), active_extruder);
  }
}

RTSSHOW::RTSSHOW()
{
  recdat.head[0] = snddat.head[0] = FHONE;
  recdat.head[1] = snddat.head[1] = FHTWO;
  memset(databuf, 0, sizeof(databuf));
}

void RTSSHOW::RTS_SDCardInit(void)
{
  if(RTS_SD_Detected())
  {
    card.mount();
  }
  if(CardReader::flag.mounted)
  {
    int16_t fileCnt = card.get_num_items();
    card.getWorkDirName();
    if(card.filename[0] != '/')
    {
      card.cdup();
    }

    int addrnum = 0;
    int num = 0;
    for(int16_t i = 0;(i < fileCnt) && (i < (MaxFileNumber + addrnum));i ++)
    {
      card.selectFileByIndex(fileCnt - 1 - i);
      char *pointFilename = card.longFilename;
      int filenamelen = strlen(card.longFilename);
      int j = 1;
      while((strncmp(&pointFilename[j], ".gcode", 6) && strncmp(&pointFilename[j], ".GCODE", 6)) && ((j ++) < filenamelen));
      if(j >= filenamelen)
      {
        addrnum++;
        continue;
      }

      if (j >= TEXTBYTELEN)
      {
        strncpy(&card.longFilename[TEXTBYTELEN - 3], "..", 2);
        card.longFilename[TEXTBYTELEN - 1] = '\0';
        j = TEXTBYTELEN - 1;
      }

      strncpy(CardRecbuf.Cardshowfilename[num], card.longFilename, j);

      strcpy(CardRecbuf.Cardfilename[num], card.filename);
      CardRecbuf.addr[num] = FILE1_TEXT_VP + (num * 20);
      RTS_SndData(CardRecbuf.Cardshowfilename[num], CardRecbuf.addr[num]);
      CardRecbuf.Filesum = (++num);
    }
    for(int j = CardRecbuf.Filesum;j < MaxFileNumber;j ++)
    {
      CardRecbuf.addr[j] = FILE1_TEXT_VP + (j * 20);
      RTS_SndData(0, CardRecbuf.addr[j]);
    }
    for(int j = 0;j < 20;j ++)
    {
      // clean print file 清除打印界面中显示的文件名
      RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
    }
    lcd_sd_status = IS_SD_INSERTED();
  }
  else
  {

    // clean filename Icon
    for(int j = 0;j < MaxFileNumber;j ++)
    {
      // clean filename Icon
      for(int i = 0;i < TEXTBYTELEN;i ++)
      {
        RTS_SndData(0, CardRecbuf.addr[j] + i);
      }
    }
    memset(&CardRecbuf, 0, sizeof(CardRecbuf));
  }
}

bool RTSSHOW::RTS_SD_Detected(void)
{
  static bool last;
  static bool state;
  static bool flag_stable;
  static uint32_t stable_point_time;

  bool tmp = IS_SD_INSERTED();

  if(tmp != last)
  {
    flag_stable = false;
  }
  else
  {
    if(!flag_stable)
    {
      flag_stable = true;
      stable_point_time = millis();
    }
  }

  if(flag_stable)
  {
    if(millis() - stable_point_time > 30)
    {
      state = tmp;
    }
  }

  last = tmp;

  return state;
}

void RTSSHOW::RTS_SDCardUpate(void)
{
  const bool sd_status = RTS_SD_Detected();
  if (sd_status != lcd_sd_status)
  {
    if (sd_status)
    {
      // SD card power on
      card.mount();
      RTS_SDCardInit();
    }
    else
    {
      card.release();

      for(int i = 0;i < CardRecbuf.Filesum;i ++)
      {
        for(int j = 0;j < 20;j ++)
        {
          RTS_SndData(0, CardRecbuf.addr[i] + j);
        }
        RTS_SndData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
      }

      for(int j = 0;j < 20;j ++)
      {
        // clean screen.
        RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
        RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
      }
      memset(&CardRecbuf, 0, sizeof(CardRecbuf));
    }
    lcd_sd_status = sd_status;
  }

  // represents to update file list
  if(CardUpdate && lcd_sd_status && RTS_SD_Detected())
  {
    for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++)
    {
      delay(1);
      RTS_SndData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
      RTS_SndData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
    }
    CardUpdate = false;
  }
}

void RTSSHOW::RTS_Init()
{
  Mode_flag = BL24CXX::readOneByte(FONT_EEPROM);
  AxisUnitMode = 1;
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    bool zig = false;
    int8_t inStart, inStop, inInc, showcount;
    showcount = 0;
    for (int y = 0; y < GRID_MAX_POINTS_Y; y++)
    {
      // away from origin
      if (zig)
      {
        inStart = 0;
        inStop = GRID_MAX_POINTS_X;
        inInc = 1;
      }
      else
      {
        // towards origin
        inStart = GRID_MAX_POINTS_X - 1;
        inStop = -1;
        inInc = -1;
      }
      zig ^= true;
      for (int x = inStart; x != inStop; x += inInc)
      {
        RTS_SndData(bedlevel.z_values[x][y] * 100, AUTO_BED_LEVEL_1POINT_VP + showcount * 2);
        showcount++;
      }
    }
    queue.enqueue_now_P(PSTR("M420 S1"));
  #endif
  last_zoffset = zprobe_zoffset = probe.offset.z;
  RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

  last_target_temperature_bed = thermalManager.temp_bed.target;
  last_target_temperature[0] = thermalManager.temp_hotend[0].target;

  feedrate_percentage = 100;
  RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);

  /***************turn off motor*****************/
  RTS_SndData(1, MOTOR_FREE_ICON_VP);

  /***************transmit temperature to screen*****************/
  RTS_SndData(0, HEAD0_SET_TEMP_VP);
  RTS_SndData(0, BED_SET_TEMP_VP);
  RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
  RTS_SndData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);

  /***************transmit Fan speed to screen*****************/
  // turn off fans
  thermalManager.set_fan_speed(0, 0);
  // queue.enqueue_now_P(PSTR("M107"));
  // RTS_SndData(1, HEAD0_FAN_ICON_VP);
  RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);

  delay(5);

  /*********transmit SD card filename to screen***************/
  RTS_SDCardInit();

  /**************************some info init*******************************/
  RTS_SndData(0, PRINT_PROCESS_ICON_VP);
  if(CardReader::flag.mounted)
  {
    if(Mode_flag) change_page_number = 1;
    else change_page_number = 56;
  }
  else
  {
    change_page_number = 0;
  }
}

int RTSSHOW::RTS_RecData()
{
	int frame_index = 0;
	int timeout = 0;
	int framelen = 0;
	bool frame_flag = false;
	if(LCD_SERIAL.available() <= 0){
		return -1;
	}
	do{
		if(LCD_SERIAL.available() > 0){
			databuf[frame_index] = LCD_SERIAL.read();
			timeout = 0;
			/* 0x5A */
			if((frame_index == 0) && (databuf[frame_index] == FHONE)){
				frame_index++;
				continue;
			}
			/* 0xA5 */
			else if(frame_index == 1){
				if(databuf[frame_index] == FHTWO){
					frame_index++;
				}
				else{
					frame_index = 0;
				}
				continue;
			}
			/* 长度 */
			else if(frame_index == 2){
				framelen = databuf[frame_index];
				frame_index++;
				continue;
			}
			else if(frame_index != 0){
				frame_index++;
				/* 一帧数据提取完毕，剩余的串口数据下次进入这个函数会在处理 */
				if(frame_index == (framelen + 3)){
					frame_flag = true;
					break;
				}
			}
		}
		else{
			timeout++;
			delay(1);
		}
	}while(timeout < 50); /* 超时函数 */

	if(frame_flag == true){
		recdat.head[0] = databuf[0];
		recdat.head[1] = databuf[1];
		recdat.len = databuf[2];
		recdat.command = databuf[3];
		for(int idx = 0; idx < frame_index; idx++){
		}
	}
	else{
		return -1;
	}
    // response for writing byte
    if ((recdat.len == 0x03) &&
		((recdat.command == 0x82) || (recdat.command == 0x80)) &&
		(databuf[4] == 0x4F) &&
		(databuf[5] == 0x4B)){
		memset(databuf, 0, sizeof(databuf));
		recnum = 0;
		return -1;
    }
    else if (recdat.command == 0x83){
		// response for reading the data from the variate
		recdat.addr = databuf[4];
		recdat.addr = (recdat.addr << 8) | databuf[5];
		recdat.bytelen = databuf[6];
		for (unsigned int i = 0; i < recdat.bytelen; i += 2){
			recdat.data[i / 2] = databuf[7 + i];
			recdat.data[i / 2] = (recdat.data[i / 2] << 8) | databuf[8 + i];
		}
    }
    else if (recdat.command == 0x81){
		// response for reading the page from the register
		recdat.addr = databuf[4];
		recdat.bytelen = databuf[5];
		for (unsigned int i = 0; i < recdat.bytelen; i ++){
			recdat.data[i] = databuf[6 + i];
			// recdat.data[i] = (recdat.data[i] << 8 )| databuf[7 + i];
		}
    }
	else{
		memset(databuf, 0, sizeof(databuf));
		recnum = 0;
		// receive the wrong data
		return -1;
	}
	memset(databuf, 0, sizeof(databuf));
	recnum = 0;

	return 2;
}

void RTSSHOW::RTS_SndData(void)
{
  if((snddat.head[0] == FHONE) && (snddat.head[1] == FHTWO) && (snddat.len >= 3))
  {
    databuf[0] = snddat.head[0];
    databuf[1] = snddat.head[1];
    databuf[2] = snddat.len;
    databuf[3] = snddat.command;
    // to write data to the register
    if(snddat.command == 0x80)
    {
      databuf[4] = snddat.addr;
      for(int i = 0;i <(snddat.len - 2);i ++)
      {
        databuf[5 + i] = snddat.data[i];
      }
    }
    else if((snddat.len == 3) && (snddat.command == 0x81))
    {
      // to read data from the register
      databuf[4] = snddat.addr;
      databuf[5] = snddat.bytelen;
    }
    else if(snddat.command == 0x82)
    {
      // to write data to the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      for(int i =0;i <(snddat.len - 3);i += 2)
      {
        databuf[6 + i] = snddat.data[i/2] >> 8;
        databuf[7 + i] = snddat.data[i/2] & 0xFF;
      }
    }
    else if((snddat.len == 4) && (snddat.command == 0x83))
    {
      // to read data from the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      databuf[6] = snddat.bytelen;
    }
     for(int i = 0;i < (snddat.len + 3); i ++)
     {
       LCD_SERIAL.write(databuf[i]);
     }

    memset(&snddat, 0, sizeof(snddat));
    memset(databuf, 0, sizeof(databuf));
    snddat.head[0] = FHONE;
    snddat.head[1] = FHTWO;
  }
}

void RTSSHOW::RTS_SndData(const String &s, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{
  if(s.length() < 1)
  {
    return;
  }
  RTS_SndData(s.c_str(), addr, cmd);
}

void RTSSHOW::RTS_SndData(const char *str, unsigned long addr, unsigned char cmd/*= VarAddr_W*/)
{
  int len = strlen(str);
  if(len > 0)
  {
    databuf[0] = FHONE;
    databuf[1] = FHTWO;
    databuf[2] = 3 + len;
    databuf[3] = cmd;
    databuf[4] = addr >> 8;
    databuf[5] = addr & 0x00FF;
    for(int i = 0;i < len;i ++)
    {
      databuf[6 + i] = str[i];
    }

    for(int i = 0;i < (len + 6);i ++)
    {
      LCD_SERIAL.write(databuf[i]);
    }
    memset(databuf, 0, sizeof(databuf));
  }
}

void RTSSHOW::RTS_SndData(char c, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{
  snddat.command = cmd;
  snddat.addr = addr;
  snddat.data[0] = (unsigned long)c;
  snddat.data[0] = snddat.data[0] << 8;
  snddat.len = 5;
  RTS_SndData();
}

void RTSSHOW::RTS_SndData(unsigned char *str, unsigned long addr, unsigned char cmd) { RTS_SndData((char *)str, addr, cmd); }

void RTSSHOW::RTS_SndData(int n, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{
  if (cmd == VarAddr_W)
  {
    if (n > 0xFFFF)
    {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len = 7;
    }
    else
    {
      snddat.data[0] = n;
      snddat.len = 5;
    }
  }
  else if (cmd == RegAddr_W)
  {
    snddat.data[0] = n;
    snddat.len = 3;
  }
  else if (cmd == VarAddr_R)
  {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;
  RTS_SndData();
}

void RTSSHOW::RTS_SndData(unsigned int n, unsigned long addr, unsigned char cmd) { RTS_SndData((int)n, addr, cmd); }

void RTSSHOW::RTS_SndData(float n, unsigned long addr, unsigned char cmd) { RTS_SndData((int)n, addr, cmd); }

void RTSSHOW::RTS_SndData(long n, unsigned long addr, unsigned char cmd) { RTS_SndData((unsigned long)n, addr, cmd); }

void RTSSHOW::RTS_SndData(unsigned long n, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{
  if (cmd == VarAddr_W)
  {
    if (n > 0xFFFF)
    {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len = 7;
    }
    else
    {
      snddat.data[0] = n;
      snddat.len = 5;
    }
  }
  else if (cmd == VarAddr_R)
  {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;
  RTS_SndData();
}
void RTSSHOW::RTS_SDcard_Stop()
{
  waitway = 7;
  change_page_number = 1;
  card.flag.abort_sd_printing = true;

  #if DISABLED(SD_ABORT_NO_COOLDOWN)
    thermalManager.disable_all_heaters();
  #endif
   print_job_timer.reset();

  thermalManager.setTargetHotend(0, 0);
  RTS_SndData(0, HEAD0_SET_TEMP_VP);
  thermalManager.setTargetBed(0);
  RTS_SndData(0, BED_SET_TEMP_VP);
  thermalManager.zero_fan_speeds();
  wait_for_heatup = wait_for_user = false;
  PoweroffContinue = false;
  if(CardReader::flag.mounted)
  {
    #if ENABLED(SDSUPPORT) && ENABLED(POWER_LOSS_RECOVERY)
      card.removeJobRecoveryFile();
    #endif
  }
  #ifdef EVENT_GCODE_SD_STOP
    queue.inject_P(PSTR(EVENT_GCODE_SD_STOP));
  #endif

  // shut down the stepper motor.
  // queue.enqueue_now_P(PSTR("M84"));
  RTS_SndData(0, MOTOR_FREE_ICON_VP);

  RTS_SndData(0, PRINT_PROCESS_ICON_VP);
  RTS_SndData(0, PRINT_PROCESS_VP);
  delay(2);
  for(int j = 0;j < 20;j ++)
  {
    // clean screen.
    RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
    // clean filename
    RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
  }
}
void RTSSHOW::RTS_HandleData()
{
  int Checkkey = -1;
  // for waiting
  if(waitway > 0)
  {
    memset(&recdat, 0, sizeof(recdat));
    recdat.head[0] = FHONE;
    recdat.head[1] = FHTWO;
    return;
  }
  for(int i = 0;Addrbuf[i] != 0;i ++)
  {
    if(recdat.addr == Addrbuf[i])
    {
      if(Addrbuf[i] >= ChangePageKey)
      {
        Checkkey = i;
      }
      break;
    }
  }

  if(Checkkey < 0)
  {
    memset(&recdat, 0, sizeof(recdat));
    recdat.head[0] = FHONE;
    recdat.head[1] = FHTWO;
    return;
  }

  switch(Checkkey)
  {
    case MainPageKey: //首页
      if(recdat.data[0] == 1)//选择打印文件
      {
        CardUpdate = true;
        CardRecbuf.recordcount = -1;
        if(CardReader::flag.mounted)
        {
          for (int j = 0; j < 20; j ++)
          {
            RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
          }
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 57, ExchangepageAddr);
          }
        }
        else
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 47, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 102, ExchangepageAddr);
          }
        }
      }
      else if(recdat.data[0] == 2)//完成打印
      {
        waitway = 7;
        card.flag.abort_sd_printing = true;
        quickstop_stepper();
        print_job_timer.reset();
        queue.clear();
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
        RTS_SndData(0, PRINT_PROCESS_ICON_VP);
        RTS_SndData(0, PRINT_PROCESS_VP);
        delay(2);
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        RTS_SndData(0, PRINT_SURPLUS_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_SURPLUS_TIME_MIN_VP);

        change_page_number = 1;
      }
      else if(recdat.data[0] == 3)//进入调平界面
      {
        waitway = 6;
        queue.enqueue_now_P(PSTR("G28\nG1 F200 Z0.0"));
        RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);
        if(Mode_flag)
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 32, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 4)//进入设置界面
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 76, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 5)//进入温度界面
      {
        RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 70, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 6)//换白色模式
      {
        Mode_flag = false;
        BL24CXX::writeOneByte(FONT_EEPROM, Mode_flag);
        RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        change_page_number = 56;
        settings.save();
      }
      else if(recdat.data[0] == 7)//换黑色模式
      {
        Mode_flag = true;
        BL24CXX::writeOneByte(FONT_EEPROM, Mode_flag);
        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        change_page_number = 1;
        settings.save();
      }
    break;

    case AdjustmentKey:  //调整界面
      if(recdat.data[0] == 1)//进入调整界面
      {
        RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);
        RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 83, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 2)//返回打印界面
      {
        if(StartPrintFlag == 1)
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
          }
        }
        else if(sdcard_pause_check == false)
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 67, ExchangepageAddr);
          }
        }
        else
        {
          if(Mode_flag)
          {
            RTS_SndData(1, Time_VP);
            RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(1, Time1_VP);
            RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
          }
          StartPrintFlag = 0;
        }
      }
      else if(recdat.data[0] == 4)
      {
        RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        gcode.process_subcommands_now(F("M600"));
      }
    break;

    case Fan_speed://设置风扇速度
      fan_speed = recdat.data[0];
      thermalManager.set_fan_speed(0, fan_speed);
      RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);
    break;

    case PrintSpeedKey://设置打印速度
      feedrate_percentage = recdat.data[0];
      RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
    break;

    case StopPrintKey://停止打印
      if((recdat.data[0] == 1) || (recdat.data[0] == 0xF1))
      {
        if(Mode_flag)
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 95, ExchangepageAddr);
        }
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        RTS_SndData(0, PRINT_SURPLUS_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_SURPLUS_TIME_MIN_VP);
        RTS_SDcard_Stop();
        PrintFlag = 0;
        Update_Time_Value = 0;
      }
      else if(recdat.data[0] == 0xF0)
      {
        if(StartPrintFlag == 1)
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
          }
        }
        else if(sdcard_pause_check == false)
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 67, ExchangepageAddr);
          }
        }
        else
        {
          if(Mode_flag)
          {
            RTS_SndData(1, Time_VP);
            RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(1, Time1_VP);
            RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
          }
          StartPrintFlag = 0;
        }
      }
    break;

    case PausePrintKey://暂停打印
      if(recdat.data[0] == 0xF0)
      {
        break;
      }
      else if(recdat.data[0] == 0xF1)
      {
        if(Mode_flag)
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 95, ExchangepageAddr);
        }
        // reject to receive cmd
        change_page_number = 12;
        waitway = 1;
        pause_z = current_position[Z_AXIS];
        card.pauseSDPrint();
        pause_action_flag = true;
        pause_flag = true;
        Update_Time_Value = 0;
        sdcard_pause_check = false;
        PrintFlag = 1;
      }
    break;

    case ResumePrintKey://恢复打印
      if(recdat.data[0] == 1)//暂停恢复打印
      {
        #if ENABLED(CHECKFILEMENT)
          if(0 == READ(FIL_RUNOUT_PIN))
          {
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
            }
          }
        #endif
        // if(Mode_flag)
        // {
        //   RTS_SndData(1, Wait_VP);
        //   RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        // }
        // else
        // {
        //   RTS_SndData(1, Wait_VP);
        //   RTS_SndData(ExchangePageBase + 95, ExchangepageAddr);
        // }
        card.startOrResumeFilePrinting();
        Update_Time_Value = 0;
        sdcard_pause_check = true;
        pause_action_flag = false;
        pause_flag = false;
        if(Mode_flag)
        {
          RTS_SndData(1, Time_VP);
          RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Time1_VP);
          RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
        }
        PrintFlag = 2;
      }
      else if(recdat.data[0] == 2)
      {
        #if ENABLED(CHECKFILEMENT)
          if(0 == READ(FIL_RUNOUT_PIN))
          {
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
            }
          }
          else
          {
            if(Mode_flag)
            {
              RTS_SndData(1, Wait_VP);
              RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(1, Wait_VP);
              RTS_SndData(ExchangePageBase + 95, ExchangepageAddr);
            }
            card.startOrResumeFilePrinting();
            print_job_timer.start();

            Update_Time_Value = 0;
            pause_action_flag = false;
            sdcard_pause_check = true;
            if(Mode_flag)
            {
              RTS_SndData(1, Time_VP);
              RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(1, Time1_VP);
              RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
            }
            PrintFlag = 2;
          }
        #endif
      }
      else if(recdat.data[0] == 3)//断电续打恢复打印
      {
        if(PoweroffContinue == true)
        {
          #if ENABLED(CHECKFILEMENT)
            if( 0 == READ(FIL_RUNOUT_PIN))
            {
              RTS_SndData(0, CHANGE_FILAMENT_ICON_VP);
            }
            else
            {
              RTS_SndData(1, CHANGE_FILAMENT_ICON_VP);
            }
            if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 8, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 63, ExchangepageAddr);
          }
          #endif
        }
        else if(PoweroffContinue == false)
        {
          #if ENABLED(CHECKFILEMENT)
            if(0 == READ(FIL_RUNOUT_PIN))
            {
              if(Mode_flag)
              {
                RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
              }
              else
              {
                RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
              }
              break;
            }
          #endif

          char cmd[30];
          char *c;
          sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[FilenamesCount]);
          for (c = &cmd[4]; *c; c++)
            *c = tolower(*c);

          queue.enqueue_one_now(cmd);
          delay(20);
          queue.enqueue_now_P(PSTR("M24"));
          // clean screen.
          for (int j = 0; j < 20; j ++)
          {
            RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
          }
          RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);
          delay(2);
          #if ENABLED(BABYSTEPPING)
            RTS_SndData(0, AUTO_BED_LEVEL_ZOFFSET_VP);
          #endif
          feedrate_percentage = 100;
          RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          zprobe_zoffset = last_zoffset;
          RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
          PoweroffContinue = true;
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
          }
          sdcard_pause_check = true;
        }
      }
      else if(recdat.data[0] == 4)//拔卡恢复打印
      {
        if(!CardReader::flag.mounted)
        {
          CardUpdate = true;
          RTS_SDCardUpate();
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 101, ExchangepageAddr);
          }
        }
        else
        {
          #if ENABLED(CHECKFILEMENT)
          if(0 == READ(FIL_RUNOUT_PIN))
          {
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
            }
          }
          #endif
          if(Mode_flag)
          {
            RTS_SndData(1, Wait_VP);
            RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(1, Wait_VP);
            RTS_SndData(ExchangePageBase + 95, ExchangepageAddr);
          }
          card.startOrResumeFilePrinting();
          Update_Time_Value = 0;
          sdcard_pause_check = true;
          pause_action_flag = false;
          PrintFlag = 2;
          for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++)
          {
            if(!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1]))
            {
              rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
            }
          }

          if(Mode_flag)
          {
            RTS_SndData(1, Time_VP);
            RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(1, Time1_VP);
            RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
          }
        }
      }
    break;
    case CoolScreenKey://温度界面
      if (recdat.data[0] == 1)//PLA 预热
      {
        thermalManager.temp_hotend[0].target = PREHEAT_1_TEMP_HOTEND;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        thermalManager.temp_bed.target = PREHEAT_1_TEMP_BED;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      }
      else if (recdat.data[0] == 2)//ABS预热
      {
        thermalManager.temp_hotend[0].target = PREHEAT_2_TEMP_HOTEND;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        thermalManager.temp_bed.target = PREHEAT_2_TEMP_BED;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      }
      else if (recdat.data[0] == 3)//返回主界面
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 0xF1)//冷却
      {
        #if FAN_COUNT > 0
          for (uint8_t i = 0; i < FAN_COUNT; i++)
          {
            thermalManager.fan_speed[i] = 255;
          }
        #endif
        RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);
        thermalManager.setTargetHotend(0, 0);
        RTS_SndData(0, HEAD0_SET_TEMP_VP);
        delay(1);
        thermalManager.setTargetBed(0);
        RTS_SndData(0, BED_SET_TEMP_VP);
        delay(1);

        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 70, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 0xF0)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 70, ExchangepageAddr);
        }
      }
    break;

    case Heater0TempEnterKey:
      thermalManager.temp_hotend[0].target = recdat.data[0];
      thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
      RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
    break;

    case HotBedTempEnterKey:
      thermalManager.temp_bed.target = recdat.data[0];
      thermalManager.setTargetBed(thermalManager.temp_bed.target);
      RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
    break;

    case Heater0LoadEnterKey:
      Filament0LOAD = ((float)recdat.data[0]) / 10;
    break;

    case AxisPageSelectKey://移动轴界面
      if(recdat.data[0] == 1)
      {
        AxisUnitMode = 1;
        axis_unit = 10.0;
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 29, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 84, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 2)
      {
        AxisUnitMode = 2;
        axis_unit = 1.0;
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 30, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 85, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 3)
      {
        AxisUnitMode = 3;
        axis_unit = 0.1;
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 31, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 86, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 4)
      {
        waitway = 4;
        queue.enqueue_now_P(PSTR("G28"));
        Update_Time_Value = 0;
        if(Mode_flag)
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 32, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Wait_VP);
          RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
        }
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
      }
      else if(recdat.data[0] == 5)//解锁电机
      {
        queue.enqueue_now_P(PSTR("M84"));
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
      }
    break;

    case SettingScreenKey://设置界面
      if(recdat.data[0] == 2)//进入换料界面
      {
        Filament0LOAD = 10;
        RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
        RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);

        delay(2);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 3)//进入移动界面
      {
        AxisUnitMode = 1;
        RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
        RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
        RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);

        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 29, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 84, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 4)//进入高级设置界面
      {
        RTS_SndData(planner.extruder_advance_K[0] * 100, Advance_K_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 49, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 104, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 5)//信息
      {
        char sizebuf[20] = {0};
        sprintf(sizebuf, "%d X %d X %d", X_MAX_POS, Y_MAX_POS, Z_MAX_POS);
        RTS_SndData(MACVERSION, PRINTER_MACHINE_TEXT_VP);
        RTS_SndData(sizebuf, PRINTER_PRINTSIZE_TEXT_VP);
        RTS_SndData(SOFTVERSION, PRINTER_VERSION_TEXT_VP);
        RTS_SndData(MARLINVERSION, MARLIN_VERSION_TEXT_VP);
        RTS_SndData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 88, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 7)//返回
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        }
      }
    break;

    case SettingBackKey://调节界面返回按键
      if (recdat.data[0] == 1)
      {
        Update_Time_Value = RTS_UPDATE_VALUE;
        //settings.save();
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 2)
      {
        if(!planner.has_blocks_queued())
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 111, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 117, ExchangepageAddr);
          }
        }
      }
    break;

    case BedLevelFunKey://调平界面
      if (recdat.data[0] == 1)//保存按钮
      {
        settings.save();
        waitway = 6;
        queue.enqueue_now_P(PSTR("G28 Z"));
        queue.enqueue_now_P(PSTR("G1 F200 Z0.0"));
      }
      else if (recdat.data[0] == 4)
      {
        queue.enqueue_now_P(PSTR("G34"));
        queue.enqueue_now_P(PSTR("G1 F200 Z0.0"));
      }
      else if (recdat.data[0] == 5)
      {
        #if ENABLED(FIX_MOUNTED_PROBE)
          waitway = 3;
          RTS_SndData(1, AUTO_BED_LEVEL_ICON_VP);
          queue.enqueue_now_P(PSTR("G29"));
        #endif
      }
      if (recdat.data[0] == 11)//调整界面保存
      {
        settings.save();
      }
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
    break;
    case XaxismoveKey:
      float x_min, x_max;
      waitway = 4;
      x_min = X_MIN_POS;
      x_max = X_MAX_POS;
      if(recdat.data[0] >= 32768)
      {
        current_position[X_AXIS] = ((float)recdat.data[0] - 65536) / 10;
      }
      else
      {
        current_position[X_AXIS] = ((float)recdat.data[0]) / 10;
      }
      if (current_position[X_AXIS] < x_min)
      {
        current_position[X_AXIS] = x_min;
      }
      else if (current_position[X_AXIS] > x_max)
      {
        current_position[X_AXIS] = x_max;
      }
      RTS_line_to_current(X_AXIS);
      RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
    break;

    case YaxismoveKey:
      float y_min, y_max;
      waitway = 4;
      y_min = Y_MIN_POS;
      y_max = Y_MAX_POS;
      current_position[Y_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[Y_AXIS] < y_min)
      {
        current_position[Y_AXIS] = y_min;
      }
      else if (current_position[Y_AXIS] > y_max)
      {
        current_position[Y_AXIS] = y_max;
      }
      RTS_line_to_current(Y_AXIS);
      RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
    break;

    case ZaxismoveKey:
      float z_min, z_max;
      waitway = 4;
      z_min = Z_MIN_POS;
      z_max = Z_MAX_POS;
      current_position[Z_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[Z_AXIS] < z_min)
      {
        current_position[Z_AXIS] = z_min;
      }
      else if (current_position[Z_AXIS] > z_max)
      {
        current_position[Z_AXIS] = z_max;
      }
      RTS_line_to_current(Z_AXIS);
      RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
    break;
    case FilamentLoadKey://换料
      if(recdat.data[0] == 1)
      {
        if(!planner.has_blocks_queued())
        {
          #if ENABLED(CHECKFILEMENT)
            if(0 == READ(FIL_RUNOUT_PIN))
            {
              if(Mode_flag)
              {
                RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
              }
              else
              {
                RTS_SndData(ExchangePageBase + 75, ExchangepageAddr);
              }
            }
          #endif
          current_position[E_AXIS] -= Filament0LOAD;

          if(thermalManager.temp_hotend[0].celsius < (ChangeFilament0Temp - 5))
          {
            RTS_SndData((int)ChangeFilament0Temp, CHANGE_FILAMENT0_TEMP_VP);
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 24, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 79, ExchangepageAddr);
            }
          }
          else
          {
            RTS_line_to_current(E_AXIS);
            RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          }
        }
      }
      else if(recdat.data[0] == 2)
      {
        if(!planner.has_blocks_queued())
        {
          #if ENABLED(CHECKFILEMENT)
            if(0 == READ(FIL_RUNOUT_PIN))
            {
              if(Mode_flag)
              {
                RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
              }
              else
              {
                RTS_SndData(ExchangePageBase + 75, ExchangepageAddr);
              }
            }
          #endif
          current_position[E_AXIS] += Filament0LOAD;

          if(thermalManager.temp_hotend[0].celsius < (ChangeFilament0Temp - 5))
          {
            RTS_SndData((int)ChangeFilament0Temp, CHANGE_FILAMENT0_TEMP_VP);
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 24, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 79, ExchangepageAddr);
            }
          }
          else
          {
            RTS_line_to_current(E_AXIS);
            RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          }
        }
      }
      else if(recdat.data[0] == 3)
      {
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 27, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 82, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 5)
      {
        if(!planner.has_blocks_queued())
        {
          RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
          thermalManager.setTargetHotend(ChangeFilament0Temp, 0);
          RTS_SndData(ChangeFilament0Temp, HEAD0_SET_TEMP_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 26, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
          }
          heatway = 1;
        }
      }
      else if(recdat.data[0] == 6)
      {
        if(!planner.has_blocks_queued())
        {
          Filament0LOAD = 10;
          RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
          }
        }
      }
      else if(recdat.data[0] == 4)
      {
        if(!planner.has_blocks_queued())
        {
          thermalManager.temp_hotend[0].target = 0;
          RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
          }
          Filament0LOAD = 10;
          RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          break;
        }
      }
      else if(recdat.data[0] == 8)
      {
        if(!planner.has_blocks_queued())
        {
          RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
          thermalManager.setTargetHotend(ChangeFilament0Temp, 0);
          RTS_SndData(ChangeFilament0Temp, HEAD0_SET_TEMP_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 26, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
          }
          heatway = 1;
        }
        break;
      }
    break;

    case FilamentCheckKey:
      if (recdat.data[0] == 1)
      {
        #if ENABLED(CHECKFILEMENT)
          if(0 == READ(FIL_RUNOUT_PIN))
          {
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 75, ExchangepageAddr);
            }
          }
          else
          {
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
            }
          }
        #endif
      }
      else if (recdat.data[0] == 2)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 76, ExchangepageAddr);
        }
        Filament0LOAD = 10;
      }
    break;

    case PowerContinuePrintKey://断电续打
      if (recdat.data[0] == 1)
      {
        if (PoweroffContinue)
        {
          power_off_type_yes = 1;
          Update_Time_Value = 0;
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
          }
          queue.enqueue_now_P(PSTR("M1000"));

          PoweroffContinue = true;
          sdcard_pause_check = true;
          zprobe_zoffset = probe.offset.z;
          RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
          RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          PrintFlag = 2;
        }
      }
      else if (recdat.data[0] == 2)
      {
        Update_Time_Value = RTS_UPDATE_VALUE;
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        }
        PoweroffContinue = false;
        sdcard_pause_check = true;
        queue.clear();
        quickstop_stepper();
        print_job_timer.abort();
        thermalManager.disable_all_heaters();
        print_job_timer.reset();

        if(CardReader::flag.mounted)
        {
          #if ENABLED(SDSUPPORT) && ENABLED(POWER_LOSS_RECOVERY)
            card.removeJobRecoveryFile();
            recovery.info.valid_head = 0;
            recovery.info.valid_foot = 0;
            recovery.close();
          #endif
        }

        wait_for_heatup = wait_for_user = false;
        PrintFlag = 0;
      }
    break;

    case SelectFileKey:
      if (RTS_SD_Detected())
      {
        if (recdat.data[0] > CardRecbuf.Filesum)
        {
          break;
        }

        CardRecbuf.recordcount = recdat.data[0] - 1;
        for (int j = 0; j < 10; j ++)
        {
          RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
          RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
        }
        RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], SELECT_FILE_TEXT_VP);
        RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);
        delay(2);
        for(int j = 1;j <= CardRecbuf.Filesum;j ++)
        {
          RTS_SndData((unsigned long)0x738E, FilenameNature + j * 16);
        }
        RTS_SndData((unsigned long)0x041F, FilenameNature + recdat.data[0] * 16);
        RTS_SndData(1, FILE1_SELECT_ICON_VP + (recdat.data[0] - 1));
      }
    break;

    case PrintFileKey://打印文件列表
    if (recdat.data[0] == 1 && RTS_SD_Detected())
      {
        if (CardRecbuf.recordcount < 0)
        {
          break;
        }
        char cmd[30];
        char *c;
        sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[CardRecbuf.recordcount]);
        for (c = &cmd[4]; *c; c++)
          *c = tolower(*c);

        memset(cmdbuf, 0, sizeof(cmdbuf));
        strcpy(cmdbuf, cmd);
        FilenamesCount = CardRecbuf.recordcount;
        #if ENABLED(CHECKFILEMENT)
          if(0 == READ(FIL_RUNOUT_PIN))
          {
            if(Mode_flag)
            {
              RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            }
            else
            {
              RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
            }
            sdcard_pause_check = false;
            break;
          }
        #endif
        queue.enqueue_one_now(cmd);
        delay(20);
        queue.enqueue_now_P(PSTR("M24"));
        // clean screen.
        for (int j = 0; j < 20; j ++)
        {
          RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
        }

        RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);

        delay(2);

        #if ENABLED(BABYSTEPPING)
          RTS_SndData(0, AUTO_BED_LEVEL_ZOFFSET_VP);
        #endif
        feedrate_percentage = 100;
        RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
        zprobe_zoffset = last_zoffset;
        RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        PoweroffContinue = true;
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
        }
        change_page_number = 11;
        Update_Time_Value = 0;
        StartPrintFlag = 1;
        PrintFlag = 2;
      }
      else if(recdat.data[0] == 2)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 3, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 58, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 3)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 57, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 4)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 4, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 59, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 5)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 3, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 58, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 6)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 5, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 60, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 7)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 4, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 59, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 8)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        }
      }
    break;

    case StoreMemoryKey://初始化
      if(recdat.data[0] == 0xF1)
      {
        settings.init_eeprom();
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          bool zig = true;
          int8_t inStart, inStop, inInc, showcount;
          showcount = 0;
          for (int y = 0; y < GRID_MAX_POINTS_Y; y++)
          {
            // away from origin
            if (zig)
            {
              inStart = 0;
              inStop = GRID_MAX_POINTS_X;
              inInc = 1;
            }
            else
            {
              // towards origin
              inStart = GRID_MAX_POINTS_X - 1;
              inStop = -1;
              inInc = -1;
            }
            zig ^= true;
            for (int x = inStart; x != inStop; x += inInc)
            {
              RTS_SndData(bedlevel.z_values[x][y] * 100, AUTO_BED_LEVEL_1POINT_VP + showcount * 2);
              showcount++;
            }
          }
        #endif
        rtscheck.RTS_SndData(Beep, SoundAddr);
        zprobe_zoffset = 0;
        last_zoffset = 0;
        RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        if(Mode_flag)
        {
          rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        }
        else
        {
         rtscheck.RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 0xF0)
      {
        if(Mode_flag)
        {
          rtscheck.RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        }
        else
        {
         rtscheck.RTS_SndData(ExchangePageBase + 76, ExchangepageAddr);
        }
      }
      break;
     case AdvancedKey://高级设置界面
      if (recdat.data[0] == 1)//PID
        {
          float hot_p,hot_i,hot_d,bed_p,bed_i,bed_d;
          hot_p = thermalManager.temp_hotend[0].pid.p()*100;
          hot_i = (thermalManager.temp_hotend[0].pid.i()/8*10000)+0.00001;
          hot_d = thermalManager.temp_hotend[0].pid.d()*8;

          bed_p = thermalManager.temp_bed.pid.Kp*100;
          bed_i = (thermalManager.temp_bed.pid.Ki/8*10000)+0.0001;
          bed_d = thermalManager.temp_bed.pid.Kd*0.8;

          RTS_SndData(hot_p, Nozzle_P_VP);
          RTS_SndData(hot_i, Nozzle_I_VP);
          RTS_SndData(hot_d, Nozzle_D_VP);
          RTS_SndData(bed_p, Hot_Bed_P_VP);
          RTS_SndData(bed_i, Hot_Bed_I_VP);
          RTS_SndData(bed_d, Hot_Bed_D_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 41, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 96, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 2)//速度
        {
          RTS_SndData(planner.settings.max_feedrate_mm_s[X_AXIS], Vmax_X_VP);
          RTS_SndData(planner.settings.max_feedrate_mm_s[Y_AXIS], Vmax_Y_VP);
          RTS_SndData(planner.settings.max_feedrate_mm_s[Z_AXIS], Vmax_Z_VP);
          RTS_SndData(planner.settings.max_feedrate_mm_s[E_AXIS], Vmax_E_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 80, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 3)//加速度
        {
          RTS_SndData(planner.settings.acceleration, Accel_VP);
          RTS_SndData(planner.settings.retract_acceleration, A_Retract_VP);
          RTS_SndData(planner.settings.travel_acceleration, A_Travel_VP);
          RTS_SndData(planner.settings.max_acceleration_mm_per_s2[X_AXIS], Amax_X_VP);
          RTS_SndData(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], Amax_Y_VP);
          RTS_SndData(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], Amax_Z_VP);
          RTS_SndData(planner.settings.max_acceleration_mm_per_s2[E_AXIS], Amax_E_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 34, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 4)//Jrek
        {
          RTS_SndData(planner.max_jerk[X_AXIS]*10, Jerk_X_VP);
          RTS_SndData(planner.max_jerk[Y_AXIS]*10, Jerk_Y_VP);
          RTS_SndData(planner.max_jerk[Z_AXIS]*10, Jerk_Z_VP);
          RTS_SndData(planner.max_jerk[E_AXIS]*10, Jerk_E_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 35, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 90, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 5)//Steps
        {
          RTS_SndData(planner.settings.axis_steps_per_mm[X_AXIS]*10, Steps_X_VP);
          RTS_SndData(planner.settings.axis_steps_per_mm[Y_AXIS]*10, Steps_Y_VP);
          RTS_SndData(planner.settings.axis_steps_per_mm[Z_AXIS]*10, Steps_Z_VP);
          RTS_SndData(planner.settings.axis_steps_per_mm[E_AXIS]*10, Steps_E_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 92, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 6)//返回
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 76, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 7)//确认
        {
          RTS_SndData(planner.extruder_advance_K[0] * 100, Advance_K_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 18, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 73, ExchangepageAddr);
          }
        }
        else if(recdat.data[0] == 8)//TMC
        {
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 113, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 119, ExchangepageAddr);
          }
        }
        else if (recdat.data[0] == 9)//保存
        {
           settings.save();
        }
      break;
    case Nozzle_P:
      SET_HOTEND_PID(Kp, 0, (float)recdat.data[0]/100);
      thermalManager.updatePID();
    break;
    case Nozzle_I:
      SET_HOTEND_PID(Ki, 0, (float)recdat.data[0]*8/10000);
      thermalManager.updatePID();
    break;
    case Nozzle_D:
      SET_HOTEND_PID(Kd, 0, (float)recdat.data[0]/8);
      thermalManager.updatePID();
    break;
    case Hot_Bed_P:
      thermalManager.temp_bed.pid.Kp = (float)recdat.data[0]/100;
      thermalManager.updatePID();
    break;
    case Hot_Bed_I:
      thermalManager.temp_bed.pid.Ki = (float)recdat.data[0]*8/10000;
      thermalManager.updatePID();
    break;
    case Hot_Bed_D:
      thermalManager.temp_bed.pid.Kd = (float)recdat.data[0]/0.8;
      thermalManager.updatePID();
    break;
    case Vmax_X:
      planner.settings.max_feedrate_mm_s[X_AXIS] = recdat.data[0];
    break;
    case Vmax_Y:
      planner.settings.max_feedrate_mm_s[Y_AXIS] = recdat.data[0];
    break;
    case Vmax_Z:
      planner.settings.max_feedrate_mm_s[Z_AXIS] = recdat.data[0];
    break;
    case Vmax_E:
      planner.settings.max_feedrate_mm_s[E_AXIS] = recdat.data[0];
    break;
    case Accel:
      planner.settings.acceleration = recdat.data[0];
    break;
    case A_Retract:
      planner.settings.retract_acceleration = recdat.data[0];
    break;
    case A_Travel:
      planner.settings.travel_acceleration = recdat.data[0];
    break;
    case Amax_X:
      planner.settings.max_acceleration_mm_per_s2[X_AXIS] = recdat.data[0];
    break;
    case Amax_Y:
      planner.settings.max_acceleration_mm_per_s2[Y_AXIS] = recdat.data[0];
    break;
    case Amax_Z:
      planner.settings.max_acceleration_mm_per_s2[Z_AXIS] = recdat.data[0];
    break;
    case Amax_E:
      planner.settings.max_acceleration_mm_per_s2[E_AXIS] = recdat.data[0];
    break;
    case Jerk_X:
      planner.max_jerk[X_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Jerk_Y:
      planner.max_jerk[Y_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Jerk_Z:
      planner.max_jerk[Z_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Jerk_E:
      planner.max_jerk[E_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Steps_X:
      planner.settings.axis_steps_per_mm[X_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Steps_Y:
      planner.settings.axis_steps_per_mm[Y_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Steps_Z:
      planner.settings.axis_steps_per_mm[Z_AXIS] = ((float)recdat.data[0])/10;
    break;
    case Steps_E:
      planner.settings.axis_steps_per_mm[E_AXIS] = ((float)recdat.data[0])/10;
    break;
    case AdvancedBackKey:
      if(Mode_flag)
      {
        RTS_SndData(ExchangePageBase + 18, ExchangepageAddr);
      }
      else
      {
        RTS_SndData(ExchangePageBase + 73, ExchangepageAddr);
      }
    break;
    case Advance_K:
      planner.extruder_advance_K[0] = ((float)recdat.data[0])/100;
    break;
    case FilamentChange://自动换料
      if (recdat.data[0] == 1 && READ(FIL_RUNOUT_PIN) == 1)
      {
        RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        wait_for_heatup = wait_for_user = false;
      }
      else if(recdat.data[0] == 2)
      {
        RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        wait_for_heatup = wait_for_user = false;
      }
      else if(recdat.data[0] == 3)
      {
        pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
      }
      else if(recdat.data[0] == 4)
      {
        pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
      }
    break;
    case ZoffsetUnitKey://Zoffset unit
      if(recdat.data[0] == 1)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 83, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 2)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 14, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 69, ExchangepageAddr);
        }
      }
      if(recdat.data[0] == 3)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 111, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 117, ExchangepageAddr);
        }
      }
      else if(recdat.data[0] == 4)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 22, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 77, ExchangepageAddr);
        }
      }

    break;
    case ZOffsetKey:
        last_zoffset = zprobe_zoffset;
        if((float)recdat.data[0] >= 32767)
        {
          zprobe_zoffset = ((float)recdat.data[0] - 65537) / 100 +0.0001;
        }
        else
        {
          zprobe_zoffset = ((float)recdat.data[0]) / 100 +0.0001;
        }
        if(WITHIN((zprobe_zoffset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
        {
          babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
        }
        probe.offset.z = zprobe_zoffset;
        RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
    break;
    case TMCDriver:
      if (recdat.data[0] == 1)//current
        {
          RTS_SndData(stepperX.getMilliamps(), Current_X_VP);
          RTS_SndData(stepperY.getMilliamps(), Current_Y_VP);
          RTS_SndData(stepperZ.getMilliamps(), Current_Z_VP);
          RTS_SndData(stepperE0.getMilliamps(), Current_E_VP);
          if(Mode_flag)
          {
            RTS_SndData(ExchangePageBase + 114, ExchangepageAddr);
          }
          else
          {
            RTS_SndData(ExchangePageBase + 120, ExchangepageAddr);
          }
        }
      else if (recdat.data[0] == 2)//threshold
      {
        RTS_SndData(stepperX.get_pwm_thrs(), Threshold_X_VP);
        RTS_SndData(stepperY.get_pwm_thrs(), Threshold_Y_VP);
        RTS_SndData(stepperZ.get_pwm_thrs(), Threshold_Z_VP);
        RTS_SndData(stepperE0.get_pwm_thrs(), Threshold_E_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 115, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 121, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 3)//Sensorless
      {
        RTS_SndData(stepperX.homing_threshold(), Sensorless_X_VP);
        RTS_SndData(stepperY.homing_threshold(), Sensorless_Y_VP);
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 116, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 122, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 4)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 18, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 73, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 5)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 113, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 119, ExchangepageAddr);
        }
      }
    break;
    case Current_X:
      sprintf_P(cmd, PSTR("M906 X%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Current_Y:
      sprintf_P(cmd, PSTR("M906 Y%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Current_Z:
      sprintf_P(cmd, PSTR("M906 Z%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Current_E:
      sprintf_P(cmd, PSTR("M906 E0%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Threshold_X:
      sprintf_P(cmd, PSTR("M913 X%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Threshold_Y:
      sprintf_P(cmd, PSTR("M913 Y%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Threshold_Z:
      sprintf_P(cmd, PSTR("M913 Z%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Threshold_E:
      sprintf_P(cmd, PSTR("M913 E0%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Sensorless_X:
      sprintf_P(cmd, PSTR("M914 X%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case Sensorless_Y:
      sprintf_P(cmd, PSTR("M914 Y%i"), recdat.data[0]);
      queue.enqueue_now_P(cmd);
    break;
    case ChangePageKey:
      if ((change_page_number == 36) || (change_page_number == 76))
      {
        break;
      }
      else if(change_page_number == 11)
      {
        if(Mode_flag)
        {
          RTS_SndData(1, Time_VP);
          RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Time1_VP);
          RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
        }
        StartPrintFlag = 0;
      }
      else if(change_page_number == 12)
      {
        if(Mode_flag)
        {
          RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 67, ExchangepageAddr);
        }
      }
      else if((change_page_number != 12) && (change_page_number != 11) && card.isPrinting())
      {
         for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++)
          {
            if(!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1]))
            {
              rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
            }
          }
        if(Mode_flag)
        {
          RTS_SndData(1, Time1_VP);
          RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(1, Time1_VP);
          RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
        }
      }
      else
      {
        RTS_SndData(change_page_number + ExchangePageBase, ExchangepageAddr);
      }
      for (int i = 0; i < MaxFileNumber; i ++)
      {
        for (int j = 0; j < 20; j ++)
        {
          RTS_SndData(0, FILE1_TEXT_VP + i * 20 + j);
        }
      }

      for (int i = 0; i < CardRecbuf.Filesum; i++)
      {
        for (int j = 0; j < 20; j++)
        {
          RTS_SndData(0, CardRecbuf.addr[i] + j);
        }
        RTS_SndData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
      }

      for (int j = 0; j < 20; j ++)
      {
        // clean screen.
        RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
        // clean filename
        RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
      }
      // clean filename Icon
      for (int j = 0; j < 20; j ++)
      {
        RTS_SndData(10, FILE1_SELECT_ICON_VP + j);
      }

      RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);

      // represents to update file list
      if (CardUpdate && lcd_sd_status && IS_SD_INSERTED())
      {
        for (uint16_t i = 0; i < CardRecbuf.Filesum; i++)
        {
          delay(3);
          RTS_SndData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
          RTS_SndData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
          RTS_SndData(0, FILE1_SELECT_ICON_VP + i);
        }
      }

      char sizeBuf[20];
      sprintf(sizeBuf, "%d X %d X %d", X_BED_SIZE-10, Y_BED_SIZE, Z_MAX_POS);
      RTS_SndData(MACVERSION, PRINTER_MACHINE_TEXT_VP);
      RTS_SndData(SOFTVERSION, PRINTER_VERSION_TEXT_VP);
      RTS_SndData(sizeBuf, PRINTER_PRINTSIZE_TEXT_VP);

      RTS_SndData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);

      RTS_SndData(thermalManager.fan_speed[0], Fan_speed_VP);
      Percentrecord = card.percentDone() + 1;
      if (Percentrecord <= 100)
      {
        rtscheck.RTS_SndData((unsigned char)Percentrecord, PRINT_PROCESS_ICON_VP);
      }
      rtscheck.RTS_SndData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);

      RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

      RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
      RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);

    break;

    default:
      break;
  }
  memset(&recdat, 0, sizeof(recdat));
  recdat.head[0] = FHONE;
  recdat.head[1] = FHTWO;
}
void EachMomentUpdate()
{
  millis_t ms = millis();
  if(ms > next_rts_update_ms)
  {
    // print the file before the power is off.
    if((power_off_type_yes == 0) && lcd_sd_status && (PoweroffContinue == true))
    {
      rtscheck.RTS_SndData(ExchangePageBase, ExchangepageAddr);
      if(startprogress < 0)startprogress = 0;
      if (startprogress < 56)
      {
        rtscheck.RTS_SndData(startprogress, START1_PROCESS_ICON_VP);
      }
      delay(80);
      if((startprogress += 1) > 56)
      {
        rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
        power_off_type_yes = 1;
        for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++)
        {
          if(!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1]))
          {
            rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
            if(Mode_flag)
            {
              rtscheck.RTS_SndData(ExchangePageBase + 36, ExchangepageAddr);
            }
            else
            {
              rtscheck.RTS_SndData(ExchangePageBase + 91, ExchangepageAddr);
            }
            break;
          }
        }
        StartFlag = 1;
      }
      return;
    }
    else if((power_off_type_yes == 0) && (PoweroffContinue == false))
    {
      rtscheck.RTS_SndData(ExchangePageBase, ExchangepageAddr);
      if(startprogress < 0)startprogress = 0;
      else if(startprogress < 56)
      {
        rtscheck.RTS_SndData(startprogress, START1_PROCESS_ICON_VP);
      }
      delay(80);
      if((startprogress += 1) > 56)
      {
        rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
        power_off_type_yes = 1;
        Update_Time_Value = RTS_UPDATE_VALUE;
        if(Mode_flag)
        {
          rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_number = 1;
        }
        else
        {
          rtscheck.RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
          change_page_number = 56;
        }
      }
      return;
    }
    else
    {
      // need to optimize
      if(print_job_timer.duration() != 0)
      {
        duration_t elapsed = print_job_timer.duration();
        static unsigned char last_cardpercentValue = 100;
        rtscheck.RTS_SndData(elapsed.value / 3600, PRINT_TIME_HOUR_VP);
        rtscheck.RTS_SndData((elapsed.value % 3600) / 60, PRINT_TIME_MIN_VP);

        if(card.isPrinting() && (last_cardpercentValue != card.percentDone()))
        {
          if((unsigned char) card.percentDone() > 0)
          {
            Percentrecord = card.percentDone();
            if(Percentrecord <= 100)
            {
              rtscheck.RTS_SndData((unsigned char)Percentrecord, PRINT_PROCESS_ICON_VP);
            }
            // Estimate remaining time every 20 seconds
            static millis_t next_remain_time_update = 0;
            if(ELAPSED(ms, next_remain_time_update))
            {
              if( (thermalManager.temp_hotend[0].celsius >= (thermalManager.temp_hotend[0].target - 5)))
              {
                remain_time = elapsed.value / (Percentrecord * 0.01f) - elapsed.value;
                next_remain_time_update += 20 * 1000UL;
                rtscheck.RTS_SndData(remain_time / 3600, PRINT_SURPLUS_TIME_HOUR_VP);
                rtscheck.RTS_SndData((remain_time % 3600) / 60, PRINT_SURPLUS_TIME_MIN_VP);
              }
            }
          }
          else
          {
            rtscheck.RTS_SndData(0, PRINT_PROCESS_ICON_VP);
            rtscheck.RTS_SndData(0, PRINT_SURPLUS_TIME_HOUR_VP);
            rtscheck.RTS_SndData(0, PRINT_SURPLUS_TIME_MIN_VP);
          }
          rtscheck.RTS_SndData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);
          last_cardpercentValue = card.percentDone();
        }
      }

      if(pause_action_flag && (false == sdcard_pause_check) && printingIsPaused() && !planner.has_blocks_queued())
      {
        pause_action_flag = false;
        queue.enqueue_now_P(PSTR("G0 F3000 X0 Y0"));
        queue.enqueue_now_P(PSTR("M18 S0"));
      }

      rtscheck.RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
      rtscheck.RTS_SndData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);
      rtscheck.RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);

      if((last_target_temperature[0] != thermalManager.temp_hotend[0].target) || (last_target_temperature_bed != thermalManager.temp_bed.target))
      {
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        rtscheck.RTS_SndData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
        rtscheck.RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
        last_target_temperature[0] = thermalManager.temp_hotend[0].target;
        last_target_temperature_bed = thermalManager.temp_bed.target;
      }

      if((thermalManager.temp_hotend[0].celsius >= thermalManager.temp_hotend[0].target) && (heatway == 1))
      {
        if(Mode_flag)
        {
          rtscheck.RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
        }
        else
        {
          rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
        }
        heatway = 0;
        rtscheck.RTS_SndData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
      }


      #if ENABLED(CHECKFILEMENT)
        if((0 == READ(FIL_RUNOUT_PIN)))
        {
          rtscheck.RTS_SndData(0, CHANGE_FILAMENT_ICON_VP);
        }
        else
        {
          rtscheck.RTS_SndData(1, CHANGE_FILAMENT_ICON_VP);
        }

      #endif
    }
    next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + Update_Time_Value;
  }
}
// looping at the loop function
void RTSUpdate()
{
  // Check the status of card
  rtscheck.RTS_SDCardUpate();

	sd_printing = IS_SD_PRINTING();
	card_insert_st = IS_SD_INSERTED() ;

	if((card_insert_st == false) && (sd_printing == true))
  {
		if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 101, ExchangepageAddr);
    }
		rtscheck.RTS_SndData(0, CHANGE_SDCARD_ICON_VP);
		/* 暂停打印，使得喷头可以回到零点 */
    card.pauseSDPrint();
    print_job_timer.pause();
    pause_action_flag = true;
    sdcard_pause_check = false;
	}
	/* 更新拔卡和插卡提示图标 */
	if(last_card_insert_st != card_insert_st){
		/* 当前页面显示为拔卡提示页面，但卡已经插入了，更新插卡图标 */
		rtscheck.RTS_SndData((int)card_insert_st, CHANGE_SDCARD_ICON_VP);
		last_card_insert_st = card_insert_st;
	}
  #if ENABLED(CHECKFILEMENT)
    // checking filement status during printing
    if((true == card.isPrinting()) && (true == PoweroffContinue))
    {
      if(0 == READ(FIL_RUNOUT_PIN))
      {
        Checkfilenum ++;
        delay(5);
      }
      else
      {
        delay(5);
        if(0 == READ(FIL_RUNOUT_PIN))
        {
          Checkfilenum ++;
        }
        else
        {
          Checkfilenum = 0;
        }
      }
      if(Checkfilenum > 10)
      {
        rtscheck.RTS_SndData(Beep, SoundAddr);
        if((thermalManager.temp_hotend[0].celsius <= (thermalManager.temp_hotend[0].target - 5)))
        {
          if(Mode_flag)
          {
            rtscheck.RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
          }
          else
          {
            rtscheck.RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
          }
          card.pauseSDPrint();
          print_job_timer.pause();
          pause_action_flag = true;
          Update_Time_Value = 0;
          sdcard_pause_check = false;
          PrintFlag = 1;
        }
        else if(thermalManager.temp_bed.celsius <= thermalManager.temp_bed.target)
        {
          if(Mode_flag)
          {
            rtscheck.RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
          }
          else
          {
            rtscheck.RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
          }
          card.pauseSDPrint();
          print_job_timer.pause();
          pause_action_flag = true;
          Checkfilenum = 0;
          Update_Time_Value = 0;
          sdcard_pause_check = false;
          print_preheat_check = true;
          PrintFlag = 1;
        }
        else
        {
          if(Mode_flag)
          {
            rtscheck.RTS_SndData(1, Wait_VP);
            rtscheck.RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          }
          else
          {
            rtscheck.RTS_SndData(1, Wait_VP);
            rtscheck.RTS_SndData(ExchangePageBase + 95, ExchangepageAddr);
          }
          waitway = 5;

          #if ENABLED(POWER_LOSS_RECOVERY)
            if (recovery.enabled)
            {
              recovery.save(true, false);
            }
          #endif
          card.pauseSDPrint();
          print_job_timer.pause();
          pause_action_flag = true;
          Checkfilenum = 0;
          Update_Time_Value = 0;
          sdcard_pause_check = false;
          PrintFlag = 1;
        }
      }
    }
  #endif

  EachMomentUpdate();

  // wait to receive massage and response
  while(rtscheck.RTS_RecData() > 0)
  {
    rtscheck.RTS_HandleData();
  }
}

void RTS_PauseMoveAxisPage()
{
  if(waitway == 1)
  {
    if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 67, ExchangepageAddr);
    }
    waitway = 0;
  }
  else if(waitway == 5)
  {
    if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
    }
    waitway = 0;
  }
}

void RTS_AutoBedLevelPage()
{
  if(waitway == 3)
  {
    if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 111, ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 117, ExchangepageAddr);
    }
    waitway = 0;
  }
}
void RTS_MoveAxisHoming()
{
  if(waitway == 4)
  {
    if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 29 + (AxisUnitMode - 1), ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 84 + (AxisUnitMode - 1), ExchangepageAddr);
    }
    waitway = 0;
  }
  else if(waitway == 6)
  {
    if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 111, ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 117, ExchangepageAddr);
    }
    waitway = 0;
  }
  else if(waitway == 7)
  {
    // Click Print finish
    if(Mode_flag)
    {
      rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
    }
    else
    {
      rtscheck.RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
    }
    waitway = 0;
  }

  rtscheck.RTS_SndData(10*current_position[X_AXIS], AXIS_X_COORD_VP);
  rtscheck.RTS_SndData(10*current_position[Y_AXIS], AXIS_Y_COORD_VP);
  rtscheck.RTS_SndData(10*current_position[Z_AXIS], AXIS_Z_COORD_VP);
}
#endif //RTS_AVAILABLE
