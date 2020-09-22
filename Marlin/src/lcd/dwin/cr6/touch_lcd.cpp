#include "LCD_RTS.h"
#include <arduino.h>
#include <wstring.h>
#include <stdio.h>
#include <libmaple/usart.h>
#include "../../inc/MarlinConfig.h"
#include "../../../Version.h"

#include "../../Marlin.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/configuration_store.h"
#include "../../module/printcounter.h"
#include "../../feature/babystep.h"
#include "../../feature/power_loss_recovery.h"
#include "../../gcode/gcode.h"

#include "../../feature/bedlevel/abl/abl.h"

#include "../../libs/duration_t.h"

#if ENABLED(BLTOUCH)
  #include "endstops.h"
#endif

#define CHECKFILEMENT true

char errorway = 0;
char errornum = 0;

bool StartPrint_flag = 0;

float last_zoffset = 0.0;

int power_off_type_yes = 0;

const float manual_feedrate_mm_m[] = { 50*60, 50*60, 4*60, 60 };

HMI_ValueTypeDef HMI_ValueStruct;

bool heat_flag = 0;

int startprogress = 0;
CRec CardRecbuf;
int temphot = 0;
int tempbed = 0;
int PLAhead = 0;
int PLAbed = 0;
int PLAfan = 0;
int ABShead = 0;
int ABSbed = 0;
int ABSfan = 0;
float pause_z = 0;
float pause_e = 0;
bool sdcard_pause_check = true;
bool pause_action_flag = false;
bool probe_offset_flag = false;

millis_t next_rts_update_ms = 0;
int last_target_temperature[4] = {0};
int last_target_temperature_bed;
char waitway = 0;
int change_page_font = 0;
int recnum = 0;
unsigned char Percentrecord = 0;
float FilamentLOAD = 0;

// 1 represent 10mm, 2 represent 1mm, 3 represent 0.1mm
unsigned char AxisUnitMode;
float axis_unit = 10.0;

// represents to update file list
bool CardUpdate = false;
extern CardReader card;
unsigned int language_change_font = 0;
// represents SD-card status, true means SD is available, false means opposite.
bool lcd_sd_status;

char Checkfilenum = 0;
char checkpause = 0;
char FilenamesCount = 0;

bool LEDStatus = true;
RTSSHOW rtscheck;
int Update_Time_Value = 0;
unsigned long VolumeSet = 0x80;

bool print_finish = false;
bool finish_home = false;

bool home_flag = false;
bool AutohomeZflag = false;
char cmdbuf[20] = {0};

unsigned short int checktime = 0;

inline void RTS_line_to_current(AxisEnum axis) 
{
  if (!planner.is_full())
  {
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[(int8_t)axis]), active_extruder);
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
  if(!IS_SD_INSERTED())
  {
    card.mount(); 
  }
  delay(2);
  if(IS_SD_INSERTED())
  {
    uint16_t fileCnt = card.get_num_Files();
    card.getWorkDirName();
    if (card.filename[0] == '/') 
    {
      card.mount();
    }
    else 
    {
      card.cdup();
    }

    int addrnum = 0;
    int num = 0;
    for(uint16_t i = 0;(i < fileCnt) && (i < MaxFileNumber + addrnum);i ++) 
    {
      card.selectFileByIndex(fileCnt - 1 - i);
      char *pointFilename = card.longFilename;
      int filenamelen = strlen(card.longFilename);
      int j = 1;
      while((strncmp(&pointFilename[j],".gcode",6) && strncmp(&pointFilename[j],".GCODE",6)) && ((j++) < filenamelen));
      if(j >= filenamelen)
      {
        addrnum++;
        continue;
      }

      if(j >= TEXTBYTELEN)
      {
        strncpy(&card.longFilename[TEXTBYTELEN -3], "~~", 2);
        card.longFilename[TEXTBYTELEN-1] = '\0';
        j = TEXTBYTELEN-1;
      }

      delay(3);
      strncpy(CardRecbuf.Cardshowfilename[num], card.longFilename, j);

      strcpy(CardRecbuf.Cardfilename[num], card.filename);
      CardRecbuf.addr[num] = FILE1_TEXT_VP + num * 10;
      RTS_SndData(CardRecbuf.Cardshowfilename[num],CardRecbuf.addr[num]);
      CardRecbuf.Filesum = (++num);
    }
    lcd_sd_status = IS_SD_INSERTED();
    SERIAL_ECHOLN("***Initing card is OK***");
  }
  else
  {
    SERIAL_ECHOLN("***Initing card fail***");
  }
}

void RTSSHOW::RTS_SDCardUpate(void)
{
  const bool sd_status = IS_SD_INSERTED();
  if (sd_status != lcd_sd_status)
  {
    if(sd_status)
    {
      card.mount();
      RTS_SDCardInit();
    }
    else
    {
      card.release();
      for(int i = 0;i < CardRecbuf.Filesum;i ++)
      {
        for(int j = 0;j < 10;j++)
        RTS_SndData(0, CardRecbuf.addr[i] + j);
        // white
        RTS_SndData((unsigned long)0xFFFF, FilenameNature + (i + 1) * 16);
      }

      for(int j = 0;j < 10;j ++)
      {
        // clean screen.
        RTS_SndData(0, CONTINUE_PRINT_FILE_TEXT_VP + j);
      }
      // clean filename Icon
      for(int j = 1;j <= 20;j ++)
      {
        RTS_SndData(10, FILE1_SELECT_ICON_VP - 1 + j);
      }
      memset(&CardRecbuf, 0, sizeof(CardRecbuf));
    }
    lcd_sd_status = sd_status;
  }

  // represents to update file list
  if(CardUpdate && lcd_sd_status && IS_SD_INSERTED())
  {
    for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++) 
    {
      delay(3);
      RTS_SndData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
      RTS_SndData((unsigned long)0xFFFF,FilenameNature + (i+1)*16);
      RTS_SndData(10, FILE1_SELECT_ICON_VP + i);
    }
    CardUpdate = false;
  }
}

#define DEFAULT_LANGUAGE 0	// 0: EN, 1: CN
bool first_load_language = 0;
void lcd_select_language(void)
{
  BL24CXX_Read(FONT_EEPROM+2, (uint8_t*)&first_load_language, sizeof(first_load_language));
  delay(10);
  if(first_load_language == 0)
  {
    BL24CXX_Read(FONT_EEPROM, (uint8_t*)&language_change_font, 1);
  }
  else
  {
    first_load_language = 0;
    BL24CXX_Write(FONT_EEPROM+2, (uint8_t*)&first_load_language, sizeof(first_load_language));
    #if ENABLED(DEFAULT_LANGUAGE)
      language_change_font = 1;
    #else
      language_change_font = 0;
    #endif
    BL24CXX_Write(FONT_EEPROM, (uint8_t*)&language_change_font, 1);
  }
}

void RTSSHOW::RTS_Init()
{
  lcd_select_language();
  AxisUnitMode = 1;
  last_zoffset = probe_offset.z;
  RTS_SndData(probe_offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
  last_target_temperature[0] = thermalManager.temp_hotend[0].target;
  last_target_temperature_bed = thermalManager.temp_bed.target;
  feedrate_percentage = 100;
  RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);

  /***************turn off motor*****************/
  RTS_SndData(2, MOTOR_FREE_ICON_VP); 

  /***************transmit temperature to screen*****************/
  RTS_SndData(0, HEAD_SET_TEMP_VP);
  RTS_SndData(0, BED_SET_TEMP_VP);
  RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD_CURRENT_TEMP_VP);
  RTS_SndData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);
  /***************transmit Fan speed to screen*****************/
  #if FAN_COUNT > 0
    // turn off fans
    for (uint8_t i = 0; i < FAN_COUNT; i++) thermalManager.fan_speed[i] = 255;
  #endif
  // turn on fans
  RTS_SndData(1, PRINTER_FANOPEN_TITLE_VP);
  RTS_SndData(2, PRINTER_LEDOPEN_TITLE_VP);
  LEDStatus = true;

  /*********transmit SD card filename to screen***************/
  RTS_SDCardInit();
  /***************transmit Printer information to screen*****************/
  // clean filename
  for(int j = 0;j < 20;j ++)
  {
    RTS_SndData(0, PRINTER_MACHINE_TEXT_VP + j);
  }
  char PRINTSIZE[20] = {0};
  sprintf(PRINTSIZE,"%d X %d X %d",MAC_LENGTH, MAC_WIDTH, MAC_HEIGHT);
  RTS_SndData(SOFTVERSION, PRINTER_VERSION_TEXT_VP);
  RTS_SndData(PRINTSIZE, PRINTER_PRINTSIZE_TEXT_VP);
  if(language_change_font != 0)
  {
    RTS_SndData(CORP_WEBSITE_C, PRINTER_WEBSITE_TEXT_VP);
  }
  else
  {
    RTS_SndData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);
  }
  RTS_SndData(language_change_font + 7, SYSTEM_LANGUAGE_TEXT_VP);

  /**************************some info init*******************************/
  RTS_SndData(0, PRINT_PROCESS_TITLE_VP);

  /************************clean screen*******************************/
  for(int i = 0;i < MaxFileNumber;i ++)
  {
    for(int j = 0;j < 10;j ++)
    {
      RTS_SndData(0, FILE1_TEXT_VP + i * 10 + j);
    }
  }

  for(int j = 0;j < 10;j ++)
  {
    // clean screen.
    RTS_SndData(0, CONTINUE_PRINT_FILE_TEXT_VP + j);
  }
  for(int j = 1;j <= MaxFileNumber;j ++)
  {
    RTS_SndData(10, FILE1_SELECT_ICON_VP - 1 + j);
  }

  rtscheck.RTS_SndData(ExchangePageBase, ExchangepageAddr);
  change_page_font = 0;
  for(startprogress = 0; startprogress <= 100; startprogress++)
  {
    rtscheck.RTS_SndData(startprogress, START_PROCESS_ICON_VP);
    delay(30);
  }

  rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
  Update_Time_Value = RTS_UPDATE_VALUE;
  if(language_change_font != 0)
  {
    rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
    change_page_font = 1;
  }
  else
  {
    rtscheck.RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
    change_page_font = 28;
  }
  
  SERIAL_ECHOLN("===Initing RTS has finished===");
}

int RTSSHOW::RTS_RecData()
{
  while(MYSERIAL1.available() > 0 && (recnum < SizeofDatabuf))
  {
    delay(1);
    databuf[recnum] = MYSERIAL1.read();
    if(databuf[0] == FHONE)
    {
      recnum++;
    }
    else if(databuf[0] == FHTWO)
    {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      recnum += 2;
    }
    else if(databuf[0] == FHLENG)
    {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      databuf[2] = FHLENG;
      recnum += 3;
    }
    else if(databuf[0] == VarAddr_R)
    {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      databuf[2] = FHLENG;
      databuf[3] = VarAddr_R;
      recnum += 4;
    }
    else
    {
      recnum = 0;
    }
  }

  // receive nothing  	
  if(recnum < 1)
  {
    return -1;
  }
  else  if((recdat.head[0] == databuf[0]) && (recdat.head[1] == databuf[1]) && recnum > 2)
  {
    recdat.len = databuf[2];
    recdat.command = databuf[3];
    if(recdat.len == 0x03 && (recdat.command == 0x82 || recdat.command == 0x80) && (databuf[4] == 0x4F) && (databuf[5] == 0x4B))  //response for writing byte
    {   
      memset(databuf,0, sizeof(databuf));
      recnum = 0;
      return -1;
    }
    else if(recdat.command == 0x83)
    {
      // response for reading the data from the variate
      recdat.addr = databuf[4];
      recdat.addr = (recdat.addr << 8 ) | databuf[5];
      recdat.bytelen = databuf[6];
      for(unsigned int i = 0;i < recdat.bytelen;i+=2)
      {
        recdat.data[i/2]= databuf[7+i];
        recdat.data[i/2]= (recdat.data[i/2] << 8 )| databuf[8+i];
      }
    }
    else if(recdat.command == 0x81)
    {
      // response for reading the page from the register
      recdat.addr = databuf[4];
      recdat.bytelen = databuf[5];
      for(unsigned int i = 0;i < recdat.bytelen;i++)
      {
        recdat.data[i]= databuf[6+i];
        // recdat.data[i]= (recdat.data[i] << 8 )| databuf[7+i];
      }
    }
  }
  else
  {
    memset(databuf,0, sizeof(databuf));
    recnum = 0;
    // receive the wrong data
    return -1;
  }
  memset(databuf,0, sizeof(databuf));
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
      for(int i =0;i <(snddat.len - 2);i++)
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
    // for(int i = 0;i < (snddat.len + 3);i ++)
    // {
    //   MYSERIAL1.write(databuf[i]);
    //   delayMicroseconds(1);
    // }
    usart_tx(MYSERIAL1.c_dev(), databuf, snddat.len + 3);
    MYSERIAL1.flush();

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
    databuf[2] = 3+len;
    databuf[3] = cmd;
    databuf[4] = addr >> 8;
    databuf[5] = addr & 0x00FF;
    for(int i = 0;i < len;i ++)
    {
      databuf[6 + i] = str[i];
    }

    for(int i = 0;i < (len + 6);i ++)
    {
      MYSERIAL1.write(databuf[i]);
      delayMicroseconds(1);
    }
    memset(databuf, 0, sizeof(databuf));
  }
}

void RTSSHOW::RTS_SndData(char c, unsigned long addr, unsigned char cmd/*= VarAddr_W*/)
{
  snddat.command = cmd;
  snddat.addr = addr;
  snddat.data[0] = (unsigned long)c;
  snddat.data[0] = snddat.data[0] << 8;
  snddat.len = 5;
  RTS_SndData();
}

void RTSSHOW::RTS_SndData(unsigned char* str, unsigned long addr, unsigned char cmd){RTS_SndData((char *)str, addr, cmd);}

void RTSSHOW::RTS_SndData(int n, unsigned long addr, unsigned char cmd/*= VarAddr_W*/)
{
  if(cmd == VarAddr_W )
  {
    if(n > 0xFFFF)
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
  else if(cmd == RegAddr_W)
  {
    snddat.data[0] = n;
    snddat.len = 3;
  }
  else if(cmd == VarAddr_R)
  {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;
  RTS_SndData();
}

void RTSSHOW::RTS_SndData(unsigned int n, unsigned long addr, unsigned char cmd){ RTS_SndData((int)n, addr, cmd); }

void RTSSHOW::RTS_SndData(float n, unsigned long addr, unsigned char cmd){ RTS_SndData((int)n, addr, cmd); }

void RTSSHOW::RTS_SndData(long n, unsigned long addr, unsigned char cmd){ RTS_SndData((unsigned long)n, addr, cmd); }

void RTSSHOW::RTS_SndData(unsigned long n, unsigned long addr, unsigned char cmd/*= VarAddr_W*/)
{
  if(cmd == VarAddr_W )
  {
    if(n > 0xFFFF)
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
  else if(cmd == VarAddr_R)
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
  if(heat_flag)
  {
    if(home_flag) planner.synchronize();
    card.stopSDPrint();
    queue.clear();
    quickstop_stepper();
    print_job_timer.stop();
    thermalManager.disable_all_heaters();
    thermalManager.zero_fan_speeds();
    wait_for_heatup = false;
    #if ENABLED(SDSUPPORT) && ENABLED(POWER_LOSS_RECOVERY)
      card.removeJobRecoveryFile();
    #endif
    #ifdef EVENT_GCODE_SD_STOP
      queue.inject_P(PSTR(EVENT_GCODE_SD_STOP));
    #endif
  }
  else
  {
    if(home_flag) planner.synchronize();
    card.flag.abort_sd_printing = true;
  }

  wait_for_heatup = false;
  sdcard_pause_check = true;

  waitway = 7;
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
      Checkkey = i;
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
    case MainEnterKey:
      if(recdat.data[0] == 1)
      {
        CardUpdate = true;
        RTS_SDCardUpate();
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          change_page_font = 2;
	      }
        else
        {
          RTS_SndData(ExchangePageBase + 29, ExchangepageAddr);
          change_page_font = 29;
        }
      }
      else if(recdat.data[0] == 2)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
          change_page_font = 15;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 42, ExchangepageAddr);
          change_page_font = 42;
        }
      }
      else if(recdat.data[0] == 3)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
          change_page_font = 20;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 47, ExchangepageAddr);
          change_page_font = 47;
        }
        RTS_SndData(language_change_font + 7, SYSTEM_LANGUAGE_TEXT_VP);
      }
      else if(recdat.data[0] == 4)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
          change_page_font = 25;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 52, ExchangepageAddr);
          change_page_font = 52;
        }
      }
      else if(recdat.data[0] == 5)
      {
        RTS_SndData(1, MOTOR_FREE_ICON_VP); 
        RTS_SndData(0, PRINT_PROCESS_TITLE_VP);
        RTS_SndData(0, PRINT_PROCESS_VP);
        delay(2);
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        print_job_timer.reset();
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
          change_page_font = 28;
        }
      }
      else if(recdat.data[0] == 6)
      {
        waitway = 3;
        RTS_SndData(1, AUTO_BED_LEVEL_TITLE_VP);
        RTS_SndData(AUTO_BED_LEVEL_PREHEAT, AUTO_BED_PREHEAT_HEAD_DATA_VP);
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 26, ExchangepageAddr);
          change_page_font = 26;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
          change_page_font = 53;
        }
        thermalManager.setTargetHotend(AUTO_BED_LEVEL_PREHEAT, 0);
        RTS_SndData(AUTO_BED_LEVEL_PREHEAT, HEAD_SET_TEMP_VP);
        if(thermalManager.temp_hotend[0].celsius < (AUTO_BED_LEVEL_PREHEAT - 5))
        {
          queue.enqueue_now_P(PSTR("G4 S40"));
        }
        queue.enqueue_now_P(PSTR("G28""\n""G29"));
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
      }
      break;
    case AdjustEnterKey:
      if(recdat.data[0] == 1)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 14, ExchangepageAddr);
          change_page_font = 14;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 41, ExchangepageAddr);
          change_page_font = 41;
        }
      }
      else if(recdat.data[0] == 2)
      {
        Update_Time_Value = RTS_UPDATE_VALUE;

        if(last_zoffset != probe_offset.z)
        {
          last_zoffset = probe_offset.z;
          RTS_SndData(probe_offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
          settings.save();
        }
        if(card.isPrinting())
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
            change_page_font = 10;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
            change_page_font = 37;
          }
        }
        else
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
            change_page_font = 12;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            change_page_font = 39;
          }
        }
      }
      else if(recdat.data[0] == 3)
      {
        // turn on the fan
        if(thermalManager.fan_speed[0] == 0)
        {
          RTS_SndData(1, PRINTER_FANOPEN_TITLE_VP); 
          thermalManager.fan_speed[0] = 255;
        }
        else
        {
          // turn off the fan
          RTS_SndData(2, PRINTER_FANOPEN_TITLE_VP); 
          thermalManager.fan_speed[0] = 0;
        }
      }
      else if(recdat.data[0] == 4)
      {
        // turn on the LED
        if(LEDStatus)
        {
          RTS_SndData(1, PRINTER_LEDOPEN_TITLE_VP); 
          digitalWrite(LED_CONTROL_PIN, HIGH);
          LEDStatus = false;
        }
        else
        {
          // turn off the LED
          RTS_SndData(2, PRINTER_LEDOPEN_TITLE_VP); 
          digitalWrite(LED_CONTROL_PIN, LOW);
          LEDStatus = true;
        }
      }
      break;
    case PrintSpeedEnterKey:
      feedrate_percentage = recdat.data[0];
      break;
    case StopPrintKey:
      if(recdat.data[0] == 1)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 13, ExchangepageAddr);
          change_page_font = 13;
	      }
        else
        {
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          change_page_font = 40;
        }
      }
      else if(recdat.data[0] == 2)
      {
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        Update_Time_Value = 0;
        tempbed = 0;
        temphot = 0;
        RTS_SDcard_Stop();
      }
      else if(recdat.data[0] == 3)
      {
        if(card.isPrinting())
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
            change_page_font = 10;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
            change_page_font = 37;
          }
        }
        else
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
            change_page_font = 12;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            change_page_font = 39;
          }
        }
      }
      break;
    case PausePrintKey:
      if(recdat.data[0] == 1)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
          change_page_font = 11;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 38, ExchangepageAddr);
          change_page_font = 38;
        }
      }
      else if(recdat.data[0] == 2)
      {
        pause_z = current_position[Z_AXIS];
        pause_e = current_position[E_AXIS] - 5;

        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          change_page_font = 12;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
          change_page_font = 39;
        }

        if(!temphot)
          temphot = thermalManager.degTargetHotend(0);
        if(!tempbed)
          tempbed = thermalManager.degTargetBed();

        queue.inject_P(PSTR("M25"));

        Update_Time_Value = 0;
        pause_action_flag = true;
      }
      else if(recdat.data[0] == 3)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
          change_page_font = 37;
        }
      }
      break;
    case ResumePrintKey:
      if(recdat.data[0] == 1)
      {
        char commandbuf[20];
        char pause_str_Z[16];
        char pause_str_E[16];
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
	      }
        else
        {
          RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
          change_page_font = 37;
        }
        memset(pause_str_Z, 0, sizeof(pause_str_Z));
        dtostrf(pause_z, 3, 2, pause_str_Z);
        memset(pause_str_E, 0, sizeof(pause_str_E));
        dtostrf(pause_e, 3, 2, pause_str_E);

        memset(commandbuf, 0, sizeof(commandbuf));
        sprintf_P(commandbuf, PSTR("G0 Z%s"), pause_str_Z);
        gcode.process_subcommands_now_P(commandbuf);
        sprintf_P(commandbuf, PSTR("G92 E%s"), pause_str_E);
        gcode.process_subcommands_now_P(commandbuf);

        gcode.process_subcommands_now_P(PSTR("M24"));
        sdcard_pause_check = true;
      }
      else if(recdat.data[0] == 2)
      {
        thermalManager.setTargetHotend(temphot, 0);
      }
      break;
    case ZoffsetEnterKey:
      last_zoffset = probe_offset.z;
      if(recdat.data[0] >= 32768)
      {
        probe_offset.z = ((float)recdat.data[0] - 65536)/100;
      }
      else
      {
        probe_offset.z = ((float)recdat.data[0])/100;
      }
      if(WITHIN((probe_offset.z), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
      {
        babystep.add_mm(Z_AXIS, probe_offset.z - last_zoffset);
      }
      break;
    case TempControlKey:
      if(recdat.data[0] == 2)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
          change_page_font = 21;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 48, ExchangepageAddr);
          change_page_font = 48;
        }
      }
      else if(recdat.data[0] == 3)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 22, ExchangepageAddr);
          change_page_font = 22;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 49, ExchangepageAddr);
          change_page_font = 49;
        }
      }
      else if(recdat.data[0] == 4)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
          change_page_font = 23;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 50, ExchangepageAddr);
          change_page_font = 50;
        }
      }
      else if(recdat.data[0] == 5)
      {
        thermalManager.setTargetHotend(HMI_ValueStruct.preheat_hotend_temp[0], 0);
        thermalManager.setTargetBed(HMI_ValueStruct.preheat_bed_temp[0]);

        RTS_SndData(HMI_ValueStruct.preheat_hotend_temp[0], HEAD_SET_TEMP_VP);
        RTS_SndData(HMI_ValueStruct.preheat_bed_temp[0], BED_SET_TEMP_VP);
      }
      else if(recdat.data[0] == 6)
      {
        thermalManager.setTargetHotend(HMI_ValueStruct.preheat_hotend_temp[1], 0);
        thermalManager.setTargetBed(HMI_ValueStruct.preheat_bed_temp[1]);

        RTS_SndData(HMI_ValueStruct.preheat_hotend_temp[1], HEAD_SET_TEMP_VP);
        RTS_SndData(HMI_ValueStruct.preheat_bed_temp[1], BED_SET_TEMP_VP);
      }
      else if(recdat.data[0] == 7)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
          change_page_font = 20;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 47, ExchangepageAddr);
          change_page_font = 47;
        }
      }
      break;
    case CoolDownKey:
      if(recdat.data[0] == 1)
      {
        thermalManager.setTargetHotend(0, 0);
        thermalManager.setTargetBed(0);

        RTS_SndData(0, HEAD_SET_TEMP_VP);
        RTS_SndData(0, BED_SET_TEMP_VP);
      }
      else if(recdat.data[0] == 2)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
          change_page_font = 21;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 48, ExchangepageAddr);
          change_page_font = 48;
        }
      }
      break;
    case HeaterTempEnterKey:
      thermalManager.temp_hotend[0].target = recdat.data[0];
      thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
      RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
      break;
    case HotBedTempEnterKey:
      thermalManager.temp_bed.target = recdat.data[0];
      thermalManager.setTargetBed(thermalManager.temp_bed.target);
      RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      break;
    case PrepareEnterKey:
      if(recdat.data[0] == 3)
      {
        rtscheck.RTS_SndData(10*current_position[X_AXIS], AXIS_X_COORD_VP);
        rtscheck.RTS_SndData(10*current_position[Y_AXIS], AXIS_Y_COORD_VP);
        rtscheck.RTS_SndData(10*current_position[Z_AXIS], AXIS_Z_COORD_VP);
        delay(2);
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 16, ExchangepageAddr);
          change_page_font = 16;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 43, ExchangepageAddr);
          change_page_font = 43;
        }
      }
      else if(recdat.data[0] == 5)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(CORP_WEBSITE_C, PRINTER_WEBSITE_TEXT_VP);
          RTS_SndData(ExchangePageBase + 24, ExchangepageAddr);
          change_page_font = 24;
        }
        else
        {
          RTS_SndData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);
          RTS_SndData(ExchangePageBase + 51, ExchangepageAddr);
          change_page_font = 51;
        }
      }
      else if(recdat.data[0] == 6)
      {
        queue.enqueue_now_P(PSTR("M84"));
        RTS_SndData(2, MOTOR_FREE_ICON_VP);
      }
      else if(recdat.data[0] == 7)
      {
        settings.reset();
        settings.save();
      }
      else if(recdat.data[0] == 8)
      {
        settings.save();
      }
      else if(recdat.data[0] == 9)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
          change_page_font = 28;
        }
      }
      break;
    case BedLevelKey:
      if(recdat.data[0] == 1)
      {
        waitway = 6;
        if (!TEST(axis_known_position, X_AXIS) || !TEST(axis_known_position, Y_AXIS))
        {
          queue.enqueue_now_P(PSTR("G28"));
        }
        else
        {
          queue.enqueue_now_P(PSTR("G28 Z"));
        }
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
        Update_Time_Value = 0;
      }
      else if(recdat.data[0] == 2)
      {
        if (WITHIN((probe_offset.z + 0.05), -0.52, 0.52))
        {
          babystep.add_mm(Z_AXIS, 0.05);
          probe_offset.z = (probe_offset.z + 0.05);
        }
        RTS_SndData(probe_offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      }
      else if(recdat.data[0] == 3)
      {
        if (WITHIN((probe_offset.z - 0.05), -0.52, 0.52))
        {
          babystep.add_mm(Z_AXIS, -0.05);
          probe_offset.z = (probe_offset.z - 0.05);
        }
        RTS_SndData(probe_offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      }
      else if(recdat.data[0] == 4)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
          change_page_font = 25;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 52, ExchangepageAddr);
          change_page_font = 52;
        }
      }
      break;
    case AutoHomeKey:
      if(recdat.data[0] == 1)
      {
        AxisUnitMode = 1;
        axis_unit = 10.0;
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 16, ExchangepageAddr);
          change_page_font = 16;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 43, ExchangepageAddr);
          change_page_font = 43;
        }
      }
      else if(recdat.data[0] == 2)
      {
        AxisUnitMode = 2;
        axis_unit = 1.0;
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 17, ExchangepageAddr);
          change_page_font = 17;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 44, ExchangepageAddr);
          change_page_font = 44;
        }
      }
      else if(recdat.data[0] == 3)
      {
        AxisUnitMode = 3;
        axis_unit = 0.1;
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 18, ExchangepageAddr);
          change_page_font = 18;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 45, ExchangepageAddr);
          change_page_font = 45;
        }
      }
      else if(recdat.data[0] == 4)
      {
        waitway = 4;
        queue.enqueue_now_P(PSTR("G28"));
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
        Update_Time_Value = 0;
      }
      break;
    case XaxismoveKey:
      float x_min, x_max;
      waitway = 4;
      x_min = 0;
      x_max = X_MAX_POS;
      current_position[X_AXIS] = ((float)recdat.data[0])/10;
      if(current_position[X_AXIS] < x_min)
      {
        current_position[X_AXIS] = x_min;
      }
      else if(current_position[X_AXIS] > x_max)
      {
        current_position[X_AXIS] = x_max;
      }
      RTS_line_to_current(X_AXIS);
      RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
      delay(1);
      RTS_SndData(1, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;
    case YaxismoveKey:
      float y_min, y_max;
      waitway = 4;
      y_min = 0;
      y_max = Y_MAX_POS;
      current_position[Y_AXIS] = ((float)recdat.data[0])/10;
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
      delay(1);
      RTS_SndData(1, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;
    case ZaxismoveKey:
      float z_min, z_max;
      waitway = 4;
      z_min = Z_MIN_POS;
      z_max = Z_MAX_POS;
      current_position[Z_AXIS] = ((float)recdat.data[0])/10;
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
      delay(1);
      RTS_SndData(1, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;
    case HeaterLoadEnterKey:
      FilamentLOAD = ((float)recdat.data[0])/10;
      break;
    case HeaterLoadStartKey:
      if(recdat.data[0] == 1)
      {
        current_position[E_AXIS] += FilamentLOAD;

        if(thermalManager.temp_hotend[0].celsius < PREHEAT_1_TEMP_HOTEND)
        {
          thermalManager.temp_hotend[0].target = PREHEAT_1_TEMP_HOTEND;
          thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
          RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        }
      }
      else if(recdat.data[0] == 2)
      {
        current_position[E_AXIS] -= FilamentLOAD;

        if(thermalManager.temp_hotend[0].celsius < PREHEAT_1_TEMP_HOTEND)
        {
          thermalManager.temp_hotend[0].target = PREHEAT_1_TEMP_HOTEND;
          thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
          RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        }
      }
      else if(recdat.data[0] == 3)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
          change_page_font = 15;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 42, ExchangepageAddr);
          change_page_font = 42;
        }
      }
      else if(recdat.data[0] == 4)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 19, ExchangepageAddr);
          change_page_font = 19;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
          change_page_font = 46;
        }
      }
      RTS_line_to_current(E_AXIS);
      RTS_SndData(10 * FilamentLOAD, HEAD_FILAMENT_LOAD_DATA_VP);
      break;
    case SelectLanguageKey:
      if(recdat.data[0] != 0)
      {
        language_change_font = recdat.data[0] - 1;
      }
      if(recdat.data[0] == 2)
      {
        RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
        change_page_font = 20;
      }
      else if(recdat.data[0] == 1)
      {
        RTS_SndData(ExchangePageBase + 47, ExchangepageAddr);
        change_page_font = 47;
      }
      RTS_SndData(language_change_font + 7, SYSTEM_LANGUAGE_TEXT_VP);
      BL24CXX_Write(FONT_EEPROM, (uint8_t*)&language_change_font, 1);
      break;
    case PowerContinuePrintKey:
      if(recdat.data[0] == 1)
      {
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
          change_page_font = 37;
        }
        if(recovery.info.recovery_flag) 
        {
          power_off_type_yes = 1;

          RTS_SndData(1, PRINTER_FANOPEN_TITLE_VP);

          Update_Time_Value = 0;

          heat_flag = 1;

          recovery.resume();
        }
      }
      else if(recdat.data[0] == 2)
      {
        waitway = 3;
        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
          change_page_font = 28;
        }
        Update_Time_Value = RTS_UPDATE_VALUE;
        card.stopSDPrint();
        queue.clear();
        quickstop_stepper();
        print_job_timer.stop();
        thermalManager.disable_all_heaters();
        print_job_timer.reset();

        #if ENABLED(SDSUPPORT) && ENABLED(POWER_LOSS_RECOVERY)
          card.removeJobRecoveryFile();
        #endif
        wait_for_heatup = false;
        sdcard_pause_check = true;
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
        delay(500);
        waitway = 0;
      }
      break;
    case FanSpeedEnterKey:
      #if FAN_COUNT > 0
        for (uint8_t i = 0; i < FAN_COUNT; i++)
        {
          thermalManager.fan_speed[i] = recdat.data[0];
          RTS_SndData(thermalManager.fan_speed[i], FAN_SPEED_CONTROL_DATA_VP);
        }
      #endif
      break;
    case PLAHeadSetEnterKey:
      HMI_ValueStruct.preheat_hotend_temp[0] = recdat.data[0];
      RTS_SndData(HMI_ValueStruct.preheat_hotend_temp[0], PLA_HEAD_SET_DATA_VP);
      break;
    case PLABedSetEnterKey:
      HMI_ValueStruct.preheat_bed_temp[0] = recdat.data[0];
      RTS_SndData(HMI_ValueStruct.preheat_bed_temp[0], PLA_BED_SET_DATA_VP);
      break;
    case PLAFanSetEnterKey:
      HMI_ValueStruct.preheat_fan_speed[0] = recdat.data[0];
      RTS_SndData(HMI_ValueStruct.preheat_fan_speed[0], PLA_FAN_SET_DATA_VP);
      break;
    case ABSHeadSetEnterKey:
      HMI_ValueStruct.preheat_hotend_temp[1] = recdat.data[0];
      RTS_SndData(HMI_ValueStruct.preheat_hotend_temp[1], ABS_HEAD_SET_DATA_VP);
      break;
    case ABSBedSetEnterKey:
      HMI_ValueStruct.preheat_bed_temp[1] = recdat.data[0];
      RTS_SndData(HMI_ValueStruct.preheat_bed_temp[1], ABS_BED_SET_DATA_VP);
      break;
    case ABSFanSetEnterKey:
      HMI_ValueStruct.preheat_fan_speed[1] = recdat.data[0];
      RTS_SndData(HMI_ValueStruct.preheat_fan_speed[1], ABS_FAN_SET_DATA_VP);
      break;
    case SelectFileKey:
      if(card.flag.mounted)
      {
        if(recdat.data[0] > CardRecbuf.Filesum) break;

        CardRecbuf.recordcount = recdat.data[0] - 1;
        delay(2);
        for(int j = 1;j <= CardRecbuf.Filesum;j ++)
        {
          RTS_SndData((unsigned long)0xFFFF, FilenameNature + j * 16);
          RTS_SndData(10, FILE1_SELECT_ICON_VP - 1 + j);
        }
        RTS_SndData((unsigned long)0x87F0, FilenameNature + recdat.data[0] * 16);
        RTS_SndData(6, FILE1_SELECT_ICON_VP - 1 + recdat.data[0]);
      }
      break;
    case StartFileKey:
      if((recdat.data[0] == 1) && card.flag.mounted)
      {
        if(CardRecbuf.recordcount < 0)
        {
          break;
        }

        char cmd[30];
        char* c;
        sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[CardRecbuf.recordcount]);
        for (c = &cmd[4]; *c; c++) *c = tolower(*c);

        FilenamesCount = CardRecbuf.recordcount;
        memset(cmdbuf, 0, sizeof(cmdbuf));
        strcpy(cmdbuf, cmd);

        queue.enqueue_one_now(cmd);
        queue.enqueue_now_P(PSTR("M24"));

        card.removeJobRecoveryFile();
        StartPrint_flag = 1;

        heat_flag = 1;

        // clean screen.
        for(int j = 0;j < 10;j ++)
        {
          RTS_SndData(0, CONTINUE_PRINT_FILE_TEXT_VP + j);
        }

        int filelen = strlen(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
        filelen = (TEXTBYTELEN - filelen)/2;
        if(filelen > 0)
        {
          char buf[20];
          memset(buf, 0, sizeof(buf));
          strncpy(buf,"         ",filelen);
          strcpy(&buf[filelen], CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
          RTS_SndData(buf, CONTINUE_PRINT_FILE_TEXT_VP);
        }
        else
        {
          RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], CONTINUE_PRINT_FILE_TEXT_VP);
        }
        delay(2);
        #if FAN_COUNT > 0
          for (uint8_t i = 0; i < FAN_COUNT; i++) thermalManager.fan_speed[i] = 255;
        #endif
        RTS_SndData(1, PRINTER_FANOPEN_TITLE_VP);

        if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 37, ExchangepageAddr); 
          change_page_font = 37;
        }
        Update_Time_Value = 0;
      }
      else if(recdat.data[0] == 4)
      {
	    if(language_change_font != 0)
        {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;
        }
        else
        {
          RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
          change_page_font = 28;
        }
      }
      break;
    case ChangePageKey:
    {
      // clean screen.
      for(int j = 0;j < 10;j ++)
      {
        RTS_SndData(0, CONTINUE_PRINT_FILE_TEXT_VP + j);
      }

      int filelen = strlen(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
      filelen = (TEXTBYTELEN - filelen)/2;
      if(filelen > 0)
      {
        char buf[20];
        memset(buf, 0, sizeof(buf));
        strncpy(buf, "         ", filelen);
        strcpy(&buf[filelen], CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
        RTS_SndData(buf, CONTINUE_PRINT_FILE_TEXT_VP);
      }
      else
      {
        RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], CONTINUE_PRINT_FILE_TEXT_VP);
      }

      // represents to update file list
      if(IS_SD_INSERTED())
      {
        for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++) 
        {
          delay(3);
          RTS_SndData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
          RTS_SndData((unsigned long)0xFFFF,FilenameNature + (i+1)*16);
          RTS_SndData(10, FILE1_SELECT_ICON_VP + i);
        }
      }

      char PRINTSIZE[20] = {0};
      sprintf(PRINTSIZE,"%d X %d X %d",MAC_LENGTH, MAC_WIDTH, MAC_HEIGHT);
      RTS_SndData(SOFTVERSION, PRINTER_VERSION_TEXT_VP);
      RTS_SndData(PRINTSIZE, PRINTER_PRINTSIZE_TEXT_VP);
      if(language_change_font != 0)
      {
        RTS_SndData(CORP_WEBSITE_C, PRINTER_WEBSITE_TEXT_VP);
      }
      else
      {
        RTS_SndData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);
      }
      RTS_SndData(language_change_font + 7, SYSTEM_LANGUAGE_TEXT_VP);

      if(thermalManager.fan_speed[0])
      {
        RTS_SndData(1, PRINTER_FANOPEN_TITLE_VP); 
      }
      else
      {
        RTS_SndData(2, PRINTER_FANOPEN_TITLE_VP); 
      }

      if(LEDStatus)
      {
        RTS_SndData(1, PRINTER_LEDOPEN_TITLE_VP); 
      }
      else
      {
        RTS_SndData(2, PRINTER_LEDOPEN_TITLE_VP); 
      }

      Percentrecord = card.percentDone() + 1;
      if(Percentrecord <= 100)
      {
        rtscheck.RTS_SndData((unsigned int)Percentrecord, PRINT_PROCESS_TITLE_VP);
      }

      RTS_SndData(probe_offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

      RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
      RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);

      RTS_SndData(ExchangePageBase + change_page_font, ExchangepageAddr);
      break;
    }
    case ErrorKey:
    {
      if(recdat.data[0] == 1)
      {
        if(printingIsActive()) // printing
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
            change_page_font = 10;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
            change_page_font = 37;
          }
        }
        else if(printingIsPaused()) // pause
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
            change_page_font = 12;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
            change_page_font = 39;
          }
        }
        else  // other
        {
          if(language_change_font != 0)
          {
            RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
            change_page_font = 1;
          }
          else
          {
            RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
            change_page_font = 28;
          }
        }
      }
      break;
    }
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
    if((power_off_type_yes == 0) && lcd_sd_status && recovery.info.recovery_flag)
    {
      power_off_type_yes = 1;
      for(uint16_t i = 0;i < CardRecbuf.Filesum;i ++) 
      {
        if(!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1]))
        {
          int filelen = strlen(CardRecbuf.Cardshowfilename[i]);
          filelen = (TEXTBYTELEN - filelen)/2;
          if(filelen > 0)
          {
            char buf[20];
            memset(buf, 0, sizeof(buf));
            strncpy(buf, "         ", filelen);
            strcpy(&buf[filelen],CardRecbuf.Cardshowfilename[i]);
            rtscheck.RTS_SndData(buf, CONTINUE_PRINT_FILE_TEXT_VP);
          }
          else
          {
            rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], CONTINUE_PRINT_FILE_TEXT_VP);
          }
          if(language_change_font != 0)
          {
            rtscheck.RTS_SndData(ExchangePageBase + 27, ExchangepageAddr);
            change_page_font = 27;
          }
          else
          {
            rtscheck.RTS_SndData(ExchangePageBase + 54, ExchangepageAddr);
            change_page_font = 54;
          }
          break;
        }
      }
      return;
    }
    else if((power_off_type_yes == 0) && !recovery.info.recovery_flag)
    {
      power_off_type_yes = 1;
      Update_Time_Value = RTS_UPDATE_VALUE;
      if(language_change_font != 0)
      {
        rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        change_page_font = 1;
      }
      else
      {
        rtscheck.RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
        change_page_font = 28;
      }
      return;
    }
    else
    {
      // need to optimize
      if(gcode.previous_move_ms != 0)
      {
        duration_t elapsed = print_job_timer.duration();
        static unsigned int last_cardpercentValue = 101;
        rtscheck.RTS_SndData(elapsed.value/3600, PRINT_TIME_HOUR_VP);
        rtscheck.RTS_SndData((elapsed.value%3600)/60, PRINT_TIME_MIN_VP);

        if(card.isPrinting() && last_cardpercentValue != card.percentDone())
        {
          if((unsigned int) card.percentDone() > 0)
          {
            Percentrecord = card.percentDone() + 1;
            if(Percentrecord <= 100)
            {
              rtscheck.RTS_SndData((unsigned int)Percentrecord, PRINT_PROCESS_TITLE_VP);
            }
          }
          else
          {
            rtscheck.RTS_SndData(0, PRINT_PROCESS_TITLE_VP);
          }
          rtscheck.RTS_SndData((unsigned int) card.percentDone(), PRINT_PROCESS_VP);
          last_cardpercentValue = card.percentDone();
        }
      }

      // save z offset
      if(probe_offset.z != last_zoffset)
      {
        settings.save();
        last_zoffset = probe_offset.z;
      }

      if(print_finish && !planner.has_blocks_queued())
      {
        print_finish = false;
        finish_home = true;
        queue.inject_P(PSTR(EVENT_GCODE_SD_STOP));
      }

      // float temp_buf = thermalManager.temp_hotend[0].celsius;
      rtscheck.RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD_CURRENT_TEMP_VP);
      rtscheck.RTS_SndData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);
      if((false == card.isPrinting()) && (false == sdcard_pause_check))
      {
        if(0 == READ(CHECK_MATWEIAL))
        {
          if(language_change_font != 0)
          {
            rtscheck.RTS_SndData(ExchangePageBase + 8, ExchangepageAddr);
            change_page_font = 8;
          }
          else
          {
            rtscheck.RTS_SndData(ExchangePageBase + 35, ExchangepageAddr);
            change_page_font = 35;
          }
          sdcard_pause_check = true;
        }
      }

      if(1 == READ(CHECK_MATWEIAL))
      {
        if(language_change_font != 0)
        {
          rtscheck.RTS_SndData(11, FILAMENT_LOAD_ICON_VP);
        }
        else
        {
          rtscheck.RTS_SndData(9, FILAMENT_LOAD_ICON_VP);
        }
      }
      else if(0 == READ(CHECK_MATWEIAL))
      {
        if(language_change_font != 0)
        {
          rtscheck.RTS_SndData(12, FILAMENT_LOAD_ICON_VP);
        }
        else
        {
          rtscheck.RTS_SndData(10, FILAMENT_LOAD_ICON_VP);
        }
      }

      if(pause_action_flag && printingIsPaused() && !planner.has_blocks_queued()) 
      {
        pause_action_flag = false;
        queue.enqueue_now_P(PSTR("G1 X0 Y0 F3000 "));
      }

      if(last_target_temperature_bed != thermalManager.temp_bed.target || (last_target_temperature[0] != thermalManager.temp_hotend[0].target))
      {
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        rtscheck.RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        rtscheck.RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);

        if(card.isPrinting())
        {
          // keep the icon
        }
        else if(last_target_temperature_bed < thermalManager.temp_bed.target || (last_target_temperature[0] < thermalManager.temp_hotend[0].target))
        {
          Update_Time_Value = 0;
        }
        else if((last_target_temperature_bed > thermalManager.temp_bed.target) || (last_target_temperature[0] > thermalManager.temp_hotend[0].target))
        {
          Update_Time_Value = 0;
        }
        last_target_temperature_bed = thermalManager.temp_bed.target;
        last_target_temperature[0] = thermalManager.temp_hotend[0].target;
      }
    }
    ErrorHanding();
    next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + Update_Time_Value;
  }
}

// looping at the loop function
void RTSUpdate()
{
  /* Check the status of card */
  rtscheck.RTS_SDCardUpate();

  #if CHECKFILEMENT
    if((true == card.isPrinting()) && (true == sdcard_pause_check))
    {
      if(1 == READ(CHECK_MATWEIAL))
      {
        checktime++;
        delay(10);
      }
      else if(0 == READ(CHECK_MATWEIAL))
      {
        checktime = 0;
      }

      if(checktime > 200)
      {
        checktime = 0;
        pause_z = current_position[Z_AXIS];
        pause_e = current_position[E_AXIS] - 5;
        #if ENABLED(POWER_LOSS_RECOVERY)
          if (recovery.enabled) recovery.save(true, false);
        #endif
        if(!temphot)
          temphot = thermalManager.degTargetHotend(0);
        if(!tempbed)
          tempbed = thermalManager.degTargetBed();

        queue.inject_P(PSTR("M25"));

        Update_Time_Value = 0;
        if(language_change_font != 0)
        {
          rtscheck.RTS_SndData(ExchangePageBase + 7, ExchangepageAddr);
          change_page_font = 7;
        }
        else
        {
          rtscheck.RTS_SndData(ExchangePageBase + 34, ExchangepageAddr);
          change_page_font = 34;
        }
        sdcard_pause_check = false;
        pause_action_flag = true;
      }
    }
  #endif

  EachMomentUpdate();

  // wait to receive massage and response
  if(rtscheck.RTS_RecData() > 0)
  {
    rtscheck.RTS_HandleData();
  }
}

void ErrorHanding()
{
  if(errorway)
  {
    if(errornum < Retry_num) // try again 3 times
    {
      errornum++;
      if(errorway == 1)
      {
        errorway = errornum = 0;  // don't try again

      }
      else if(errorway == 2)
      {
        // No processing is done in printing
        if(!printingIsActive() && !printingIsPaused())
        {
          errorway = 0;
          waitway = 4;
          queue.enqueue_now_P(PSTR("G28"));
          rtscheck.RTS_SndData(1, MOTOR_FREE_ICON_VP);
          Update_Time_Value = 0;
        }
      }
      else if(errorway == 3)
      {
        reset_bed_level();
        errorway = errornum = 0;  // don't try again
      }
    }
    else 
    {
      errorway = errornum = 0;
    }
  }
}