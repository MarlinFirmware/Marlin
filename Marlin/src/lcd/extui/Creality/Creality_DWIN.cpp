/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/* ****************************************
 * lcd/extui/lib/Creality/Creality_DWIN.cpp
 * ****************************************
 * Extensible_UI implementation for Creality DWIN
 * 10SPro, Max, CRX, and others
 * Based original Creality release, ported to ExtUI for Marlin 2.0
 * Written by Insanity Automation, sponsored by Tiny Machines 3D
 *
 * ***************************************/


#include "Creality_DWIN.h"
#include "FileNavigator.h"
#include <HardwareSerial.h>
#include <WString.h>
#include <stdio.h>
#include "../ui_api.h"

#if ENABLED(EXTENSIBLE_UI)
namespace ExtUI
{
  static uint16_t fileIndex = 0;
  uint8_t recordcount = 0;
  uint8_t waitway_lock = 0;
  const float manual_feedrate_mm_m[] = MANUAL_FEEDRATE;
  uint8_t startprogress = 0;

  char waitway = 0;
  int recnum = 0;
  unsigned char Percentrecord = 0;
  float ChangeMaterialbuf[2] = {0};

  char NozzleTempStatus[3] = {0};

  bool PrintMode = false; //Eco Mode default off

  char PrinterStatusKey[2] = {0}; // PrinterStatusKey[1] value: 0 represents to keep temperature, 1 represents  to heating , 2 stands for cooling , 3 stands for printing
                  // PrinterStatusKey[0] value: 0 reprensents 3D printer ready

  unsigned char AxisPagenum = 0; //0 for 10mm, 1 for 1mm, 2 for 0.1mm
  bool InforShowStatus = true;
  bool TPShowStatus = false; // true for only opening time and percentage, false for closing time and percentage.
  bool FanStatus = true;
  bool AutohomeKey = false;
  unsigned char AutoHomeIconNum;

  creality_dwin_settings_t Settings;
  uint8_t dwin_settings_version = 1;

  bool reEntryPrevent = false;
  uint16_t idleThrottling = 0;

  #if HAS_PID_HEATING
    uint16_t pid_hotendAutoTemp = 150;
    uint16_t pid_bedAutoTemp = 70;
  #endif

void onStartup()
{
	DWIN_SERIAL.begin(115200);
	rtscheck.recdat.head[0] = rtscheck.snddat.head[0] = FHONE;
	rtscheck.recdat.head[1] = rtscheck.snddat.head[1] = FHTWO;
	memset(rtscheck.databuf, 0, sizeof(rtscheck.databuf));

  delay_ms(500); // Delay to allow screen startup
  SetTouchScreenConfiguration();
  rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
  delay_ms(400); // Delay to allow screen to configure
  onStatusChanged(CUSTOM_MACHINE_NAME " Ready");
  //Set Eco Mode
	if (PrintMode)
		rtscheck.RTS_SndData(3, FanKeyIcon + 1); // saving mode
	else
		rtscheck.RTS_SndData(2, FanKeyIcon + 1); // normal

	rtscheck.RTS_SndData(100, FeedrateDisplay);

	/***************turn off motor*****************/
	rtscheck.RTS_SndData(11, FilenameIcon);

	/***************transmit temperature to screen*****************/
	rtscheck.RTS_SndData(0, NozzlePreheat);
	rtscheck.RTS_SndData(0, BedPreheat);
	rtscheck.RTS_SndData(getActualTemp_celsius(H0), NozzleTemp);
	rtscheck.RTS_SndData(getActualTemp_celsius(BED), Bedtemp);
	/***************transmit Fan speed to screen*****************/
	rtscheck.RTS_SndData(2, FanKeyIcon); //turn 0ff fan icon
	FanStatus = true;

	/***************transmit Printer information to screen*****************/
	for (int j = 0; j < 20; j++) //clean filename
		rtscheck.RTS_SndData(0, MacVersion + j);
	char sizebuf[20] = {0};
	sprintf(sizebuf, "%d X %d X %d", Y_BED_SIZE, X_BED_SIZE, Z_MAX_POS);
  #if defined(CUSTOM_MACHINE_NAME)
	  rtscheck.RTS_SndData(CUSTOM_MACHINE_NAME, MacVersion);
  #else
    rtscheck.RTS_SndData(MACHINE_NAME, MacVersion);
  #endif
	rtscheck.RTS_SndData(SHORT_BUILD_VERSION, SoftVersion);
	rtscheck.RTS_SndData(sizebuf, PrinterSize);
	rtscheck.RTS_SndData(WEBSITE_URL, CorpWebsite);

	/**************************some info init*******************************/
	rtscheck.RTS_SndData(0, PrintscheduleIcon);
	rtscheck.RTS_SndData(0, PrintscheduleIcon + 1);

	/************************clean screen*******************************/
	for (int i = 0; i < MaxFileNumber; i++)
	{
		for (int j = 0; j < 10; j++)
			rtscheck.RTS_SndData(0, SDFILE_ADDR + i * 10 + j);
	}

	for (int j = 0; j < 10; j++)
	{
		rtscheck.RTS_SndData(0, Printfilename + j);  //clean screen.
		rtscheck.RTS_SndData(0, Choosefilename + j); //clean filename
	}
	for (int j = 0; j < 8; j++)
		rtscheck.RTS_SndData(0, FilenameCount + j);
	for (int j = 1; j <= MaxFileNumber; j++)
	{
		rtscheck.RTS_SndData(10, FilenameIcon + j);
		rtscheck.RTS_SndData(10, FilenameIcon1 + j);
	}

	SERIAL_ECHOLNPGM_P(PSTR("==Dwin Init Complete=="));
}

void onIdle()
{
  if (reEntryPrevent)
    return;
  if(idleThrottling++ < 750){
    return;
  }

  // Always send temperature data
  rtscheck.RTS_SndData(getActualTemp_celsius(H0), NozzleTemp);
	rtscheck.RTS_SndData(getActualTemp_celsius(BED), Bedtemp);
  rtscheck.RTS_SndData(getTargetTemp_celsius(H0), NozzlePreheat);
	rtscheck.RTS_SndData(getTargetTemp_celsius(BED), BedPreheat);

  if(awaitingUserConfirm())
  {
    switch(ExtUI::pauseModeStatus)
      {
      case PAUSE_MESSAGE_PARKING:  ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_PAUSE_PRINT_PARKING)); break;
      case PAUSE_MESSAGE_CHANGING: ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_INIT)); break;
      case PAUSE_MESSAGE_UNLOAD:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_UNLOAD)); break;
      case PAUSE_MESSAGE_WAITING:  ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING)); break;
      case PAUSE_MESSAGE_INSERT:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_INSERT)); break;
      case PAUSE_MESSAGE_LOAD:     ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_LOAD)); break;
      case PAUSE_MESSAGE_PURGE:
        #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
          ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_CONT_PURGE)); break;
        #else
          ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE)); break;
        #endif
      case PAUSE_MESSAGE_RESUME:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT)); break;
      case PAUSE_MESSAGE_HEATING:  ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_OPTION:   ExtUI::onUserConfirmRequired(GET_TEXT_F(MSG_FILAMENT_CHANGE_OPTION_HEADER)); break;
      case PAUSE_MESSAGE_STATUS: SERIAL_ECHOLNPGM_P(PSTR("PauseStatus")); break;
      default: onUserConfirmRequired(PSTR("Confirm Continue")); break;
    }

  }

  reEntryPrevent = true;
  idleThrottling = 0;
  if(waitway && !commandsInQueue())
    waitway_lock++;
  else
    waitway_lock = 0;

  if(waitway_lock > 100) {
    waitway_lock = 0;
    waitway = 0; //clear waitway if nothing is going on
  }

	switch (waitway)
	{
		case 1:
      if(isPositionKnown()) {
        InforShowStatus = true;
        SERIAL_ECHOLNPGM_P(PSTR("==waitway 1=="));
        rtscheck.RTS_SndData(4 + CEIconGrap, IconPrintstatus); // 4 for Pause
        rtscheck.RTS_SndData(ExchangePageBase + 54, ExchangepageAddr);
        waitway = 0;
      }
			break;

		case 2:
      if (isPositionKnown() && !commandsInQueue())
			  waitway = 0;
			break;

		case 3:
      SERIAL_ECHOLNPGM_P(PSTR("==waitway 3=="));
      //if(isPositionKnown() && (getActualTemp_celsius(BED) >= (getTargetTemp_celsius(BED)-1))) {
			  rtscheck.RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
        waitway = 7;
        //return;
      //}
			break;

		case 4:
			if (AutohomeKey && isPositionKnown() && !commandsInQueue())
			{ //Manual Move Home Done
        SERIAL_ECHOLNPGM_P(PSTR("==waitway 4=="));
				rtscheck.RTS_SndData(ExchangePageBase + 71 + AxisPagenum, ExchangepageAddr);
				AutohomeKey = false;
				waitway = 0;
			}
			break;
		case 5:
        if(isPositionKnown() && !commandsInQueue()) {
        InforShowStatus = true;
        waitway = 0;
        SERIAL_ECHOLNPGM_P(PSTR("==waitway 5=="));
        rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr); //exchange to 78 page
      }
      break;
    case 6:
      if(!commandsInQueue()) {
        setAxisPosition_mm(LEVEL_CORNERS_HEIGHT, (axis_t)Z);
        waitway = 0;
      }
      break;
    case 7:
      if(!commandsInQueue())
        waitway = 0;
      break;
		}

  void yield();

  #if HAS_MESH
    if (getLevelingActive())
      rtscheck.RTS_SndData(3, AutoLevelIcon); /*On*/
    else
      rtscheck.RTS_SndData(2, AutoLevelIcon); /*Off*/
  #endif

  if (startprogress == 0)
  {
    startprogress += 25;
          delay_ms(300); // Delay to show bootscreen
  }
  else if( startprogress < 250)
  {
    if(isMediaInserted()) //Re init media as it happens too early on STM32 boards often
      onMediaInserted();
    else
      injectCommands_P(PSTR("M22\nM21"));
    startprogress = 254;
    SERIAL_ECHOLNPGM_P(PSTR("  startprogress "));
    InforShowStatus = true;
    TPShowStatus = false;
    rtscheck.RTS_SndData(ExchangePageBase + 45, ExchangepageAddr);
    reEntryPrevent = false;
		return;
  }
  if (startprogress <= 100)
    rtscheck.RTS_SndData(startprogress, StartIcon);
  else
    rtscheck.RTS_SndData((startprogress - 100), StartIcon + 1);

  //rtscheck.RTS_SndData((startprogress++) % 5, ExchFlmntIcon);

  if (isPrinting())
	{
    rtscheck.RTS_SndData(0 + CEIconGrap, IconPrintstatus);
		rtscheck.RTS_SndData(getProgress_seconds_elapsed() / 3600, Timehour);
		rtscheck.RTS_SndData((getProgress_seconds_elapsed() % 3600) / 60, Timemin);
		if (getProgress_percent() > 0)
		{
			Percentrecord = getProgress_percent() + 1;
			if (Percentrecord <= 50)
			{
				rtscheck.RTS_SndData((unsigned int)Percentrecord * 2, PrintscheduleIcon);
				rtscheck.RTS_SndData(0, PrintscheduleIcon + 1);
			}
			else
	  	{
				rtscheck.RTS_SndData(100, PrintscheduleIcon);
				rtscheck.RTS_SndData((unsigned int)Percentrecord * 2 - 100, PrintscheduleIcon + 1);
			}
		}
		else
		{
			rtscheck.RTS_SndData(0, PrintscheduleIcon);
			rtscheck.RTS_SndData(0, PrintscheduleIcon + 1);
		}
		rtscheck.RTS_SndData((unsigned int)getProgress_percent(), Percentage);
	}

  rtscheck.RTS_SndData(map(constrain(Settings.display_volume, 0, 255), 0, 255, 0, 100), VolumeDisplay);
  rtscheck.RTS_SndData(Settings.screen_brightness, DisplayBrightness);
  rtscheck.RTS_SndData(Settings.standby_screen_brightness, DisplayStandbyBrightness);
  rtscheck.RTS_SndData(Settings.standby_time_seconds, DisplayStandbySeconds);
  if(Settings.display_standby)
    rtscheck.RTS_SndData(3, DisplayStandbyEnableIndicator);
  else
    rtscheck.RTS_SndData(2, DisplayStandbyEnableIndicator);

  if(getTargetTemp_celsius(BED)==0 && getTargetTemp_celsius(H0)==0)
  {
    rtscheck.RTS_SndData(0 + CEIconGrap, IconPrintstatus);
  }
  else if (getActualTemp_celsius(BED) < (getTargetTemp_celsius(BED) - THERMAL_PROTECTION_BED_HYSTERESIS ) || (getActualTemp_celsius(H0) < (getTargetTemp_celsius(H0) - THERMAL_PROTECTION_HYSTERESIS)))
  {
    rtscheck.RTS_SndData(1 + CEIconGrap, IconPrintstatus); // Heating Status
    PrinterStatusKey[1] = (PrinterStatusKey[1] == 0 ? 1 : PrinterStatusKey[1]);
  }
  else if (getActualTemp_celsius(BED) > (getTargetTemp_celsius(BED) + THERMAL_PROTECTION_BED_HYSTERESIS) || (getActualTemp_celsius(H0) > (getTargetTemp_celsius(H0) + THERMAL_PROTECTION_HYSTERESIS)))
  {
    rtscheck.RTS_SndData(8 + CEIconGrap, IconPrintstatus); // Cooling Status
    PrinterStatusKey[1] = (PrinterStatusKey[1] == 0 ? 2 : PrinterStatusKey[1]);
  }
  else
    rtscheck.RTS_SndData(0 + CEIconGrap, IconPrintstatus);

	rtscheck.RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
	rtscheck.RTS_SndData((unsigned int)(getFlow_percent(E0)), Flowrate);
  rtscheck.RTS_SndData((unsigned int)(getAxisSteps_per_mm(X) * 10), StepMM_X);
  rtscheck.RTS_SndData((unsigned int)(getAxisSteps_per_mm(Y) * 10), StepMM_Y);
  rtscheck.RTS_SndData((unsigned int)(getAxisSteps_per_mm(Z) * 10), StepMM_Z);
  rtscheck.RTS_SndData((unsigned int)(getAxisSteps_per_mm(E0) * 10), StepMM_E);

  #if HAS_BED_PROBE
    rtscheck.RTS_SndData(getProbeOffset_mm(X) * 100, ProbeOffset_X);
    rtscheck.RTS_SndData(getProbeOffset_mm(Y) * 100, ProbeOffset_Y);
  #endif

  #if HAS_PID_HEATING
    rtscheck.RTS_SndData(pid_hotendAutoTemp, HotendPID_AutoTmp);
    rtscheck.RTS_SndData(pid_bedAutoTemp, BedPID_AutoTmp);
    rtscheck.RTS_SndData((unsigned int)(getPIDValues_Kp(E0) * 10), HotendPID_P);
    rtscheck.RTS_SndData((unsigned int)(getPIDValues_Ki(E0) * 10), HotendPID_I);
    rtscheck.RTS_SndData((unsigned int)(getPIDValues_Kd(E0) * 10), HotendPID_D);
    #if ENABLED(PIDTEMPBED)
      rtscheck.RTS_SndData((unsigned int)(getBedPIDValues_Kp() * 10), BedPID_P);
      rtscheck.RTS_SndData((unsigned int)(getBedPIDValues_Ki() * 10), BedPID_I);
      rtscheck.RTS_SndData((unsigned int)(getBedPIDValues_Kd() * 10), BedPID_D);
    #endif
  #endif

	if (NozzleTempStatus[0] || NozzleTempStatus[2]) //statuse of loadfilement and unloadfinement when temperature is less than
	{
		unsigned int IconTemp;

		IconTemp = getActualTemp_celsius(H0) * 100 / getTargetTemp_celsius(H0);
		if (IconTemp >= 100)
			IconTemp = 100;
		rtscheck.RTS_SndData(IconTemp, HeatPercentIcon);
		if (getActualTemp_celsius(H0) > EXTRUDE_MINTEMP && NozzleTempStatus[0]!=0)
		{
			NozzleTempStatus[0] = 0;
			rtscheck.RTS_SndData(10 * ChangeMaterialbuf[0], FilementUnit1);
			rtscheck.RTS_SndData(10 * ChangeMaterialbuf[1], FilementUnit2);
      SERIAL_ECHOLNPGM_P(PSTR("==Heating Done Change Filament=="));
			rtscheck.RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
		}
		else if (getActualTemp_celsius(H0) >= getTargetTemp_celsius(H0) && NozzleTempStatus[2])
		{
			SERIAL_ECHOLNPGM("***NozzleTempStatus[2] =", (int)NozzleTempStatus[2]);
			NozzleTempStatus[2] = 0;
			TPShowStatus = true;
			rtscheck.RTS_SndData(4, ExchFlmntIcon);
			rtscheck.RTS_SndData(ExchangePageBase + 83, ExchangepageAddr);
		}
		else if (NozzleTempStatus[2])
		{
			//rtscheck.RTS_SndData((startprogress++) % 5, ExchFlmntIcon);
		}
	}

	if (AutohomeKey)
	{
		rtscheck.RTS_SndData(AutoHomeIconNum++, AutoZeroIcon);
		if (AutoHomeIconNum > 9)
			AutoHomeIconNum = 0;
	}

  if(isMediaInserted())
  {
    uint16_t currPage, maxPageAdd;
    if(fileIndex == 0)
      currPage = 1;
    else
      currPage = CEIL((float)((float)fileIndex / (float)DISPLAY_FILES)) +1;

    if(filenavigator.folderdepth!=0)
      maxPageAdd = 1;
    else
      maxPageAdd = 0;
    uint16_t maxPages = CEIL((float)(filenavigator.maxFiles()+ maxPageAdd) / (float)DISPLAY_FILES );

    rtscheck.RTS_SndData(currPage, FilesCurentPage);
    rtscheck.RTS_SndData(maxPages, FilesMaxPage);
  }
  else
  {
    rtscheck.RTS_SndData(0, FilesCurentPage);
    rtscheck.RTS_SndData(0, FilesMaxPage);
  }

  void yield();
	if (rtscheck.RTS_RecData() > 0)
		rtscheck.RTS_HandleData();

  if(rtscheck.recdat.addr != DisplayZaxis && rtscheck.recdat.addr != DisplayYaxis && rtscheck.recdat.addr != DisplayZaxis) {
		rtscheck.RTS_SndData(10 * getAxisPosition_mm((axis_t)X), DisplayXaxis);
	  rtscheck.RTS_SndData(10 * getAxisPosition_mm((axis_t)Y), DisplayYaxis);
		rtscheck.RTS_SndData(10 * getAxisPosition_mm((axis_t)Z), DisplayZaxis);
  }
  reEntryPrevent = false;
}

RTSSHOW::RTSSHOW()
{
	recdat.head[0] = snddat.head[0] = FHONE;
	recdat.head[1] = snddat.head[1] = FHTWO;
	memset(databuf, 0, sizeof(databuf));
}

int RTSSHOW::RTS_RecData()
{
	while (DWIN_SERIAL.available() > 0 && (recnum < SizeofDatabuf))
	{
		databuf[recnum] = DWIN_SERIAL.read();
		if (databuf[0] != FHONE) //ignore the invalid data
		{
			if (recnum > 0) // prevent the program from running.
			{
				memset(databuf, 0, sizeof(databuf));
				recnum = 0;
			}
			continue;
		}
		delay_ms(2);
		recnum++;
	}

	if (recnum < 1) //receive nothing
		return -1;
	else if ((recdat.head[0] == databuf[0]) && (recdat.head[1] == databuf[1]) && recnum > 2)
	{
		//  SERIAL_ECHOLN(" *** RTS_RecData1*** ");

		recdat.len = databuf[2];
		recdat.command = databuf[3];
		if (recdat.len == 0x03 && (recdat.command == 0x82 || recdat.command == 0x80) && (databuf[4] == 0x4F) && (databuf[5] == 0x4B)) //response for writing byte
		{
			memset(databuf, 0, sizeof(databuf));
			recnum = 0;
			//SERIAL_ECHOLN(" *** RTS_RecData1*** ");
			return -1;
		}
		else if (recdat.command == 0x83) //response for reading the data from the variate
		{
			recdat.addr = databuf[4];
			recdat.addr = (recdat.addr << 8) | databuf[5];
			recdat.bytelen = databuf[6];
			for (unsigned long i = 0; i < recdat.bytelen; i += 2)
			{
				recdat.data[i / 2] = databuf[7 + i];
				recdat.data[i / 2] = (recdat.data[i / 2] << 8) | databuf[8 + i];
			}
		}
		else if (recdat.command == 0x81) //response for reading the page from the register
		{
			recdat.addr = databuf[4];
			recdat.bytelen = databuf[5];
			for (unsigned long i = 0; i < recdat.bytelen; i++)
			{
				recdat.data[i] = databuf[6 + i];
				//recdat.data[i]= (recdat.data[i] << 8 )| databuf[7+i];
			}
		}
	}
	else
	{
		memset(databuf, 0, sizeof(databuf));
		recnum = 0;
		return -1; //receive the wrong data
	}
	memset(databuf, 0, sizeof(databuf));
	recnum = 0;
	return 2;
}

void RTSSHOW::RTS_SndData(void)
{
	if ((snddat.head[0] == FHONE) && (snddat.head[1] == FHTWO) && snddat.len >= 3)
	{
		databuf[0] = snddat.head[0];
		databuf[1] = snddat.head[1];
		databuf[2] = snddat.len;
		databuf[3] = snddat.command;
		if (snddat.command == 0x80) //to write data to the register
		{
			databuf[4] = snddat.addr;
			for (int i = 0; i < (snddat.len - 2); i++)
				databuf[5 + i] = snddat.data[i];
		}
		else if (snddat.len == 3 && (snddat.command == 0x81)) //to read data from the register
		{
			databuf[4] = snddat.addr;
			databuf[5] = snddat.bytelen;
		}
		else if (snddat.command == 0x82) //to write data to the variate
		{
			databuf[4] = snddat.addr >> 8;
			databuf[5] = snddat.addr & 0xFF;
			for (int i = 0; i < (snddat.len - 3); i += 2)
			{
				databuf[6 + i] = snddat.data[i / 2] >> 8;
				databuf[7 + i] = snddat.data[i / 2] & 0xFF;
			}
		}
		else if (snddat.len == 4 && (snddat.command == 0x83)) //to read data from the variate
		{
			databuf[4] = snddat.addr >> 8;
			databuf[5] = snddat.addr & 0xFF;
			databuf[6] = snddat.bytelen;
		}
		for (int i = 0; i < (snddat.len + 3); i++)
		{
			DWIN_SERIAL.write(databuf[i]);
			delay_us(1);
		}

		memset(&snddat, 0, sizeof(snddat));
		memset(databuf, 0, sizeof(databuf));
		snddat.head[0] = FHONE;
		snddat.head[1] = FHTWO;
	}
}

void RTSSHOW::RTS_SndData(const String &s, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{
	if (s.length() < 1)
		return;
	RTS_SndData(s.c_str(), addr, cmd);
}

void RTSSHOW::RTS_SndData(const char *str, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{

	int len = strlen(str);
  constexpr int maxlen = SizeofDatabuf - 6;
	if (len > 0)
	{
    if(len>maxlen) len = maxlen;
		databuf[0] = FHONE;
		databuf[1] = FHTWO;
		databuf[2] = 3 + len;
		databuf[3] = cmd;
		databuf[4] = addr >> 8;
		databuf[5] = addr & 0x00FF;
		for (int i = 0; i < len; i++)
			databuf[6 + i] = str[i];

		for (int i = 0; i < (len + 6); i++)
		{
			DWIN_SERIAL.write(databuf[i]);
			delay_us(1);
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
    if ((uint8_t)n > 0xFFFF)
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

		  //snddat.data[0] = n >> 24;
			//snddat.data[1] = n >> 16;
			//snddat.data[2] = n >> 8;
			//snddat.data[3] = n;
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

void RTSSHOW::RTS_HandleData()
{
	int Checkkey = -1;
	SERIAL_ECHOLNPGM_P(PSTR("  *******RTS_HandleData******** "));
	if (waitway > 0) //for waiting
	{
		SERIAL_ECHOLNPGM("waitway ==", (int)waitway);
		memset(&recdat, 0, sizeof(recdat));
		recdat.head[0] = FHONE;
		recdat.head[1] = FHTWO;
		return;
	}
	SERIAL_ECHOLNPGM("recdat.data[0] ==", recdat.data[0]);
	SERIAL_ECHOLNPGM("recdat.addr ==", recdat.addr);
	for (int i = 0; Addrbuf[i] != 0; i++)
	{
		if (recdat.addr == Addrbuf[i])
		{
      if (Addrbuf[i] == NzBdSet || Addrbuf[i] == NozzlePreheat || Addrbuf[i] == BedPreheat || Addrbuf[i] == Flowrate)
				Checkkey = ManualSetTemp;
			else if (Addrbuf[i] >= Stopprint && Addrbuf[i] <= Resumeprint)
				Checkkey = PrintChoice;
			else if (Addrbuf[i] >= AutoZero && Addrbuf[i] <= DisplayZaxis)
				Checkkey = XYZEaxis;
			else if (Addrbuf[i] >= FilementUnit1 && Addrbuf[i] <= FilementUnit2)
				Checkkey = Filement;
			else
				Checkkey = i;
			break;
		}
	}

  switch(recdat.addr) {
    case ProbeOffset_Z :
      {
        Checkkey = Zoffset_Value;
        break;
      }
    case Flowrate :
    case StepMM_X :
    case StepMM_Y :
    case StepMM_Z :
    case StepMM_E :
    case ProbeOffset_X :
    case ProbeOffset_Y :
    case HotendPID_AutoTmp :
    case BedPID_AutoTmp :
    case HotendPID_P :
    case HotendPID_I :
    case HotendPID_D :
    case BedPID_P :
    case BedPID_I :
    case BedPID_D :
      Checkkey = ManualSetTemp;
    break;
  }

  if(recdat.addr == VolumeDisplay)
    Checkkey = VolumeDisplay;
  if(recdat.addr == DisplayBrightness)
    Checkkey = DisplayBrightness;
  if(recdat.addr == DisplayStandbyBrightness)
    Checkkey = DisplayStandbyBrightness;
  if(recdat.addr == DisplayStandbySeconds)
    Checkkey = DisplayStandbySeconds;
  if(recdat.addr >= AutolevelVal && recdat.addr <=  4400)  // ((int)AutolevelVal+(GRID_MAX_POINTS_X*GRID_MAX_POINTS_Y*2)) = 4400 with 5x5 mesh
    Checkkey = AutolevelVal;

	if (recdat.addr >= SDFILE_ADDR && recdat.addr <= (SDFILE_ADDR + 10 * (FileNum + 1)))
		Checkkey = Filename;

	if (Checkkey < 0)
	{
		memset(&recdat, 0, sizeof(recdat));
		recdat.head[0] = FHONE;
		recdat.head[1] = FHTWO;
		return;
	}
	SERIAL_ECHOLNPGM_P(PSTR("== Checkkey=="));
	SERIAL_ECHOLN(Checkkey);

  constexpr float lfrb[4] = LEVEL_CORNERS_INSET_LFRB;
  SERIAL_ECHOLNPGM_P(PSTR("BeginSwitch"));

	switch (Checkkey)
	{
    case Printfile:
      if (recdat.data[0] == 1) // card
      {
        InforShowStatus = false;
        SERIAL_ECHOLNPGM_P(PSTR("Handle Data PrintFile Pre"));
        filenavigator.getFiles(0);
        fileIndex = 0;
        recordcount = 0;
        SERIAL_ECHOLNPGM_P(PSTR("Handle Data PrintFile Post"));
        RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
      }
      else if (recdat.data[0] == 2) // return after printing result.
      {
        InforShowStatus = true;
        TPShowStatus = false;
        stopPrint();
        injectCommands_P(PSTR("M84"));
        RTS_SndData(11, FilenameIcon);
        RTS_SndData(0, PrintscheduleIcon);
        RTS_SndData(0, PrintscheduleIcon + 1);
        RTS_SndData(0, Percentage);
        delay_ms(2);
        RTS_SndData(0, Timehour);
        RTS_SndData(0, Timemin);

        SERIAL_ECHOLNPGM_P(PSTR("Handle Data PrintFile 2 Setting Screen "));
        RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); //exchange to 45 page
      }
      else if (recdat.data[0] == 3) // Temperature control
      {
        InforShowStatus = true;
        TPShowStatus = false;
        SERIAL_ECHOLNPGM_P(PSTR("Handle Data PrintFile 3 Setting Screen "));
        if (FanStatus)
          RTS_SndData(ExchangePageBase + 58, ExchangepageAddr); //exchange to 58 page, the fans off
        else
          RTS_SndData(ExchangePageBase + 57, ExchangepageAddr); //exchange to 57 page, the fans on
      }
      else if (recdat.data[0] == 4) //Settings
        InforShowStatus = false;
      break;

    case Ajust:
      if (recdat.data[0] == 1)
      {
        InforShowStatus = false;
        FanStatus ? RTS_SndData(2, FanKeyIcon) : RTS_SndData(3, FanKeyIcon);
      }
      else if (recdat.data[0] == 2)
      {
        SERIAL_ECHOLNPGM_P(PSTR("Handle Data Adjust 2 Setting Screen "));
        InforShowStatus = true;
        if (PrinterStatusKey[1] == 3) // during heating
        {
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
        }
        else if (PrinterStatusKey[1] == 4)
        {
          RTS_SndData(ExchangePageBase + 54, ExchangepageAddr);
        }
        else
        {
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
        }
      }
      else if (recdat.data[0] == 3)
      {
        if (FanStatus) //turn on the fan
        {
          RTS_SndData(3, FanKeyIcon);
          setTargetFan_percent(100, FAN0);
          FanStatus = false;
        }
        else //turn off the fan
        {
          RTS_SndData(2, FanKeyIcon);
          setTargetFan_percent(0, FAN0);
          FanStatus = true;
        }
      }
      else if (recdat.data[0] == 4)
      {
        if (PrintMode) // normal printing mode
        {
          RTS_SndData(2, FanKeyIcon + 1);
          PrintMode = false;
        }
        else // power saving mode
        {
          RTS_SndData(3, FanKeyIcon + 1);
          PrintMode = true;
        }
      }
      break;

    case Feedrate:
      setFeedrate_percent(recdat.data[0]);
      break;

    case PrintChoice:
      if (recdat.addr == Stopprint)
      {
        SERIAL_ECHOLNPGM_P(PSTR("StopPrint"));
        if (recdat.data[0] == 240) // no
        {
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
          SERIAL_ECHOLNPGM("Stop No", recdat.data[0] );
        }
        else
        {
          RTS_SndData(ExchangePageBase + 45, ExchangepageAddr);
          RTS_SndData(0, Timehour);
          RTS_SndData(0, Timemin);
          SERIAL_ECHOLNPGM("Stop Triggered", recdat.data[0] );
          stopPrint();
        }
      }
      else if (recdat.addr == Pauseprint)
      {
        if (recdat.data[0] != 0xF1)
          break;

        RTS_SndData(ExchangePageBase + 54, ExchangepageAddr);
        pausePrint();
      }
      else if (recdat.addr == Resumeprint && recdat.data[0] == 1)
      {
        resumePrint();

        PrinterStatusKey[1] = 0;
        InforShowStatus = true;

        RTS_SndData(0 + CEIconGrap, IconPrintstatus);
        //PrinterStatusKey[1] = 3;
        RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
      }
      if (recdat.addr == Resumeprint && recdat.data[0] == 2) // warming
      {
        resumePrint();
        NozzleTempStatus[2] = 1;
        PrinterStatusKey[1] = 0;
        InforShowStatus = true;
        RTS_SndData(ExchangePageBase + 82, ExchangepageAddr);
      }
      break;

    case Zoffset:
      float tmp_zprobe_offset;
      if (recdat.data[0] >= 32768)
      {
        tmp_zprobe_offset = ((float)recdat.data[0] - 65536) / 100;
      }
      else
      {
        tmp_zprobe_offset = ((float)recdat.data[0]) / 100;
      }
      SERIAL_ECHOLNPGM("Requested Offset ", tmp_zprobe_offset);
      if (WITHIN((tmp_zprobe_offset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
      {
        int16_t tmpSteps = mmToWholeSteps(getZOffset_mm() - tmp_zprobe_offset, (axis_t)Z);
        if(tmpSteps==0)
        {
          SERIAL_ECHOLNPGM_P(PSTR("Rounding to step"));
          if(getZOffset_mm() < tmp_zprobe_offset)
            tmpSteps = 1;
          else
            tmpSteps = -1;
        }
        smartAdjustAxis_steps(tmpSteps*-1, (axis_t)Z, false);
        char zOffs[20], tmp1[11];
        sprintf_P(zOffs, PSTR("Z Offset : %s"), dtostrf(getZOffset_mm(), 1, 3, tmp1));
        onStatusChanged(zOffs);
      }
      else
      {
        onStatusChanged("Requested Offset Beyond Limits");
        RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
      }

      rtscheck.RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
      break;

    case TempControl:
      if (recdat.data[0] == 0)
      {
        InforShowStatus = true;
        TPShowStatus = false;
      }
      else if (recdat.data[0] == 1)
      {
        if(FanStatus)
          RTS_SndData(ExchangePageBase + 60, ExchangepageAddr); //exchange to 60 page, the fans off
        else
          RTS_SndData(ExchangePageBase + 59, ExchangepageAddr); //exchange to 59 page, the fans on
      }
      else if (recdat.data[0] == 2)
      {
        InforShowStatus = true;
      }
      else if (recdat.data[0] == 3)
      {
        if (FanStatus) //turn on the fan
        {
          setTargetFan_percent(100, FAN0);
          FanStatus = false;
          RTS_SndData(ExchangePageBase + 57, ExchangepageAddr); //exchange to 57 page, the fans on
        }
        else //turn off the fan
        {
          setTargetFan_percent(0, FAN0);
          FanStatus = true;
          RTS_SndData(ExchangePageBase + 58, ExchangepageAddr); //exchange to 58 page, the fans on
        }
      }
      else if (recdat.data[0] == 5) //PLA mode
      {
        setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, H0);
        setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);

        RTS_SndData(PREHEAT_1_TEMP_HOTEND, NozzlePreheat);
        RTS_SndData(PREHEAT_1_TEMP_BED, BedPreheat);
      }
      else if (recdat.data[0] == 6) //ABS mode
      {
        setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, H0);
        setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);

        RTS_SndData(PREHEAT_2_TEMP_HOTEND, NozzlePreheat);
        RTS_SndData(PREHEAT_2_TEMP_BED, BedPreheat);
      }
      else if (recdat.data[0] == 0xF1)
      {
        //InforShowStatus = true;
        #if FAN_COUNT > 0
              for (uint8_t i = 0; i < FAN_COUNT; i++)
                setTargetFan_percent(0, (fan_t)i);
        #endif
        FanStatus = false;
        setTargetTemp_celsius(0.0, H0);
        setTargetTemp_celsius(0.0, BED);

        RTS_SndData(0, NozzlePreheat);
        delay_ms(1);
        RTS_SndData(0, BedPreheat);
        delay_ms(1);

        RTS_SndData(8 + CEIconGrap, IconPrintstatus);
        RTS_SndData(ExchangePageBase + 57, ExchangepageAddr);
        PrinterStatusKey[1] = 2;
      }
      break;

    case ManualSetTemp:
    SERIAL_ECHOLNPGM_P(PSTR("ManualSetTemp"));
      if (recdat.addr == NzBdSet)
      {
        if (recdat.data[0] == 0)
        {
          if (FanStatus)
            RTS_SndData(ExchangePageBase + 58, ExchangepageAddr); //exchange to 58 page, the fans off
          else
            RTS_SndData(ExchangePageBase + 57, ExchangepageAddr); //exchange to 57 page, the fans on
        }
        else if (recdat.data[0] == 1)
        {
          setTargetTemp_celsius(0.0, H0);
          RTS_SndData(0, NozzlePreheat);
        }
        else if (recdat.data[0] == 2)
        {
          setTargetTemp_celsius(0.0, BED);
          RTS_SndData(0, BedPreheat);
        }
      }
      else if (recdat.addr == NozzlePreheat)
        setTargetTemp_celsius((float)recdat.data[0], H0);
      else if (recdat.addr == BedPreheat)
        setTargetTemp_celsius((float)recdat.data[0], BED);
      else if (recdat.addr == Flowrate)
        setFlow_percent((int16_t)recdat.data[0], getActiveTool());
      #if HAS_PID_HEATING
        else if (recdat.addr == HotendPID_AutoTmp)
          pid_hotendAutoTemp = (uint16_t)recdat.data[0];
        else if (recdat.addr == BedPID_AutoTmp)
          pid_bedAutoTemp = (uint16_t)recdat.data[0];
      #endif
      else {
        float tmp_float_handling;
        if (recdat.data[0] >= 32768)
        {
          tmp_float_handling = ((float)recdat.data[0] - 65536) / 100;
        }
        else
        {
          tmp_float_handling = ((float)recdat.data[0]) / 100;
        }
        if (recdat.addr == StepMM_X) {
          setAxisSteps_per_mm(tmp_float_handling*10, X);
        }
        else if (recdat.addr == StepMM_Y) {
          setAxisSteps_per_mm(tmp_float_handling*10, Y);
        }
        else if (recdat.addr == StepMM_Z) {
          setAxisSteps_per_mm(tmp_float_handling*10, Z);
        }
        else if (recdat.addr == StepMM_E) {
          setAxisSteps_per_mm(tmp_float_handling*10, E0);
          setAxisSteps_per_mm(tmp_float_handling*10, E1);
        }
        #if HAS_BED_PROBE
          else if (recdat.addr == ProbeOffset_X) {
            setProbeOffset_mm(tmp_float_handling, X);
          }
          else if (recdat.addr == ProbeOffset_Y) {
            setProbeOffset_mm(tmp_float_handling, Y);
          }
          else if (recdat.addr == ProbeOffset_Z) {
            setProbeOffset_mm(tmp_float_handling, Z);
          }
        #endif
        #if HAS_PID_HEATING
          else if (recdat.addr == HotendPID_P) {
            setPIDValues(tmp_float_handling*10, getPIDValues_Ki(getActiveTool()), getPIDValues_Kd(getActiveTool()), getActiveTool());
          }
          else if (recdat.addr == HotendPID_I) {
            setPIDValues(getPIDValues_Kp(getActiveTool()), tmp_float_handling*10, getPIDValues_Kd(getActiveTool()), getActiveTool());
          }
          else if (recdat.addr == HotendPID_D) {
            setPIDValues(getPIDValues_Kp(getActiveTool()), getPIDValues_Ki(getActiveTool()), tmp_float_handling*10, getActiveTool());
          }
          #if ENABLED(PIDTEMPBED)
            else if (recdat.addr == BedPID_P) {
              setBedPIDValues(tmp_float_handling*10, getBedPIDValues_Ki(), getBedPIDValues_Kd());
            }
            else if (recdat.addr == BedPID_I) {
              setBedPIDValues(getBedPIDValues_Kp(), tmp_float_handling*10, getBedPIDValues_Kd());
            }
            else if (recdat.addr == BedPID_D) {
              setBedPIDValues(getBedPIDValues_Kp(), getBedPIDValues_Ki(), tmp_float_handling*10);
            }
          #endif
        #endif
      }
      break;

    case Setting:
      if (recdat.data[0] == 0) // return to main page
      {
        InforShowStatus = true;
        TPShowStatus = false;
      }
      else if (recdat.data[0] == 1) //Bed Autoleveling
      {
        #if HAS_MESH
          if (getLevelingActive())
            RTS_SndData(3, AutoLevelIcon);
          else
            RTS_SndData(2, AutoLevelIcon);

          if (ExtUI::getMeshValid())
          {
            uint8_t abl_probe_index = 0;
            for(uint8_t outer = 0; outer < GRID_MAX_POINTS_Y; outer++)
            {
              for (uint8_t inner = 0; inner < GRID_MAX_POINTS_X; inner++)
              {
                uint8_t x_Point = inner;
                bool zig = (outer & 1);
                if (zig) x_Point = (GRID_MAX_POINTS_X - 1) - inner;
                xy_uint8_t point = {x_Point, outer};
                rtscheck.RTS_SndData(ExtUI::getMeshPoint(point) * 1000, AutolevelVal + (abl_probe_index * 2));
                ++abl_probe_index;
              }
            }
          }
        #endif
        RTS_SndData(10, FilenameIcon); //Motor Icon
        if (!isPositionKnown())
          injectCommands_P(PSTR("G28\nG1F1000Z0.0"));
        else
          injectCommands_P(PSTR("G1F1000Z0.0"));
        waitway = 2;

        RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
      }
      else if (recdat.data[0] == 2) // Exchange filement
      {
        InforShowStatus = true;
        TPShowStatus = false;
        memset(ChangeMaterialbuf, 0, sizeof(ChangeMaterialbuf));
        ChangeMaterialbuf[1] = ChangeMaterialbuf[0] = 10;
        RTS_SndData(10 * ChangeMaterialbuf[0], FilementUnit1); //It's ChangeMaterialbuf for show,instead of current_position[E_AXIS] in them.
        RTS_SndData(10 * ChangeMaterialbuf[1], FilementUnit2);
        RTS_SndData(getActualTemp_celsius(H0), NozzleTemp);
        RTS_SndData(getTargetTemp_celsius(H0), NozzlePreheat);
        delay_ms(2);
        RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
      }
      else if (recdat.data[0] == 3) //Move
      {
        AxisPagenum = 0;
        RTS_SndData(ExchangePageBase + 71, ExchangepageAddr);
      }
      else if (recdat.data[0] == 4) //Language
      {
        //Just loads language screen, now used for tools
      }
      else if (recdat.data[0] == 5) //Printer Information
      {
        RTS_SndData(WEBSITE_URL, CorpWebsite);
      }
      else if (recdat.data[0] == 6) // Diabalestepper
      {
        injectCommands_P(PSTR("M84"));
        RTS_SndData(11, FilenameIcon);
      }
      break;

    case ReturnBack:
      SERIAL_ECHOPGM("Return : ", recdat.data[0]);
      if (recdat.data[0] == 1) // return to the tool page
      {
        InforShowStatus = false;
        RTS_SndData(ExchangePageBase + 63, ExchangepageAddr);
      }
      if (recdat.data[0] == 2) // return to the Level mode page
      {
        RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
      }
      break;

    case Bedlevel:
      SERIAL_ECHOLNPGM("Bed Level Option ",  recdat.data[0]);
      switch(recdat.data[0])
      {
        case 1: // Z-axis to home
        {
          // Disallow Z homing if X or Y are unknown
          if (!isAxisPositionKnown((axis_t)X) || !isAxisPositionKnown((axis_t)Y))
            injectCommands_P(PSTR("G28\nG1F1500Z0.0"));
          else
            injectCommands_P(PSTR("G28Z\nG1F1500Z0.0"));

          RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
          break;
        }
        case 2: // Z-axis to Up
        {
          if (WITHIN((getZOffset_mm() + 0.1), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          {
            smartAdjustAxis_steps((getAxisSteps_per_mm(Z) / 10), (axis_t)Z, false);
            //SERIAL_ECHOLNPGM("Babystep Pos Steps : ", (int)(getAxisSteps_per_mm(Z) / 10));
            //setZOffset_mm(getZOffset_mm() + 0.1);
            RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
            char zOffs[20], tmp1[11];
            sprintf_P(zOffs, PSTR("Z Offset : %s"), dtostrf(getZOffset_mm(), 1, 3, tmp1));
            injectCommands_P(PSTR("M500"));
            onStatusChanged(zOffs);
          }
          break;
        }
        case 3: // Z-axis to Down
        {
          if (WITHIN((getZOffset_mm() - 0.1), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          {
            smartAdjustAxis_steps(((getAxisSteps_per_mm(Z) / 10) * -1), (axis_t)Z, false);
            //SERIAL_ECHOLNPGM("Babystep Neg Steps : ", (int)((getAxisSteps_per_mm(Z) / 10) * -1));
            //babystepAxis_steps((((int)getAxisSteps_per_mm(Z) / 10) * -1), (axis_t)Z);
            //setZOffset_mm(getZOffset_mm() - 0.1);
            RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
            char zOffs[20], tmp1[11];
            sprintf_P(zOffs, PSTR("Z Offset : %s"), dtostrf(getZOffset_mm(), 1, 3, tmp1));
            injectCommands_P(PSTR("M500"));
            onStatusChanged(zOffs);
          }
          break;
        }
        case 4: // Assitant Level
        {
          #if HAS_MESH
            setLevelingActive(false);
          #endif
          if (!isPositionKnown())
            injectCommands_P((PSTR("G28\nG1 F1000 Z0.0")));
          else
            injectCommands_P((PSTR("G1 F1000 Z0.0")));
          waitway = 2;
          RTS_SndData(ExchangePageBase + 84, ExchangepageAddr);
          break;
        }
        case 5: // AutoLevel "Measuring" Button
        {
          #if ENABLED(MESH_BED_LEVELING)
            RTS_SndData(ExchangePageBase + 93, ExchangepageAddr);
          #else
            waitway = 3; //only for prohibiting to receive massage
            RTS_SndData(3, AutolevelIcon);
            uint8_t abl_probe_index = 0;
            while (abl_probe_index < 25) {
              rtscheck.RTS_SndData(0, AutolevelVal + abl_probe_index * 2);
              ++abl_probe_index;
            }
            RTS_SndData(ExchangePageBase + 85, ExchangepageAddr);
            injectCommands_P(PSTR(MAIN_MENU_ITEM_1_GCODE));
          #endif
          break;
        }

        case 6: // Assitant Level ,  Centre 1
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm(X_CENTER, (axis_t)X);
          setAxisPosition_mm(Y_CENTER, (axis_t)Y);
          waitway = 6;
          break;
        }
        case 7: // Assitant Level , Front Left 2
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MIN_BED + lfrb[0]), (axis_t)X);
          setAxisPosition_mm((Y_MIN_BED + lfrb[1]), (axis_t)Y);
          waitway = 6;
          break;
        }
        case 8: // Assitant Level , Front Right 3
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MAX_BED - lfrb[2]), (axis_t)X);
          setAxisPosition_mm((Y_MIN_BED + lfrb[1]), (axis_t)Y);
          waitway = 6;
          break;
        }
        case 9: // Assitant Level , Back Right 4
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MAX_BED - lfrb[2]), (axis_t)X);
          setAxisPosition_mm((Y_MAX_BED - lfrb[3]), (axis_t)Y);
          waitway = 6;
          break;
        }
        case 10: // Assitant Level , Back Left 5
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MIN_BED + lfrb[0]), (axis_t)X);
          setAxisPosition_mm((Y_MAX_BED - lfrb[3]), (axis_t)Y);
          waitway = 6;
          break;
        }
        case 11: // Autolevel switch
        {
          #if HAS_MESH
            if (!getLevelingActive()) //turn on the Autolevel
            {
              RTS_SndData(3, AutoLevelIcon);
              setLevelingActive(true);
            }
            else //turn off the Autolevel
            {
              RTS_SndData(2, AutoLevelIcon);
              setLevelingActive(false);
            }
          #endif
          RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
          break;
        }
        case 12:
        {
          injectCommands_P(PSTR("G26R255"));
          onStatusChanged("Beginning G26.. Heating");
          break;
        }
        case 13:
        {
          injectCommands_P(PSTR("G29S1"));
          onStatusChanged("Begin Manual Mesh");
          break;
        }
        case 14:
        {
          injectCommands_P(PSTR("G29S2"));
          onStatusChanged("Moving to Next Mesh Point");
          break;
        }
        case 15:
        {
          injectCommands_P(PSTR("M211S0\nG91\nG1Z-0.025\nG90\nM211S1"));
          onStatusChanged("Moved down 0.025");
          break;
        }
        case 16:
        {
          injectCommands_P(PSTR("M211S0\nG91\nG1Z0.025\nG90\nM211S1"));
          onStatusChanged("Moved up 0.025");
          break;
        }
        case 17:
        {
          Settings.display_volume = 0;
          Settings.display_sound = false;
          SetTouchScreenConfiguration();
          break;
        }
        case 18:
        {
          Settings.display_volume = 255;
          Settings.display_sound = true;
          SetTouchScreenConfiguration();
          break;
        }
        case 19:
        {
          Settings.screen_brightness = 10;
          SetTouchScreenConfiguration();
          break;
        }
        case 20:
        {
          Settings.screen_brightness = 100;
          SetTouchScreenConfiguration();
          break;
        }
        case 21:
        {
          if(Settings.display_standby) {
            Settings.display_standby = false;
          }
          else {
            Settings.display_standby = true;
          }
          SetTouchScreenConfiguration();
          break;
        }
        case 22:
        {
          if(Settings.screen_rotation==10) {
            Settings.screen_rotation = 0;
          }
          else {
            Settings.screen_rotation = 10;
          }
          SetTouchScreenConfiguration();
          break;
        }
        default:
        {
          SERIAL_ECHOLNPGM("Unsupported Option Selected", recdat.data[0]);
          break;
        }
      }

      RTS_SndData(10, FilenameIcon);
      break;

    case XYZEaxis:
    {
      axis_t axis = X;
      float min, max = 0;
      waitway = 4;
      if (recdat.addr == DisplayXaxis)
      {
        axis = X;
        min = X_MIN_POS;
        max = X_MAX_POS;
      }
      else if (recdat.addr == DisplayYaxis)
      {
        axis = Y;
        min = Y_MIN_POS;
        max = Y_MAX_POS;
      }
      else if (recdat.addr == DisplayZaxis)
      {
        axis = Z;
        min = Z_MIN_POS;
        max = Z_MAX_POS;
      }
      else if (recdat.addr == AutoZero)
      {
        if (recdat.data[0] == 3) //autohome
        {
          waitway = 4;
          injectCommands_P((PSTR("G28\nG1 F1000 Z10")));
          InforShowStatus = AutohomeKey = true;
          AutoHomeIconNum = 0;
          RTS_SndData(ExchangePageBase + 74, ExchangepageAddr);
          RTS_SndData(10, FilenameIcon);
        }
        else
        {
          AxisPagenum = recdat.data[0];
          waitway = 0;
        }
        break;
      }

      float targetPos = ((float)recdat.data[0]) / 10;

      if (targetPos < min)
        targetPos = min;
      else if (targetPos > max)
        targetPos = max;
      setAxisPosition_mm(targetPos, axis);
      waitway = 0;
      RTS_SndData(10, FilenameIcon);
      break;
    }

    case Filement:

      unsigned int IconTemp;
      if (recdat.addr == Exchfilement)
      {
        if (getActualTemp_celsius(H0) < EXTRUDE_MINTEMP && recdat.data[0] < 5)
        {
          RTS_SndData((int)EXTRUDE_MINTEMP, 0x1020);
          delay_ms(5);
          RTS_SndData(ExchangePageBase + 66, ExchangepageAddr);
          break;
        }

        switch(recdat.data[0])
        {
          case 1 : // Unload filement1
          {
            setAxisPosition_mm((getAxisPosition_mm(E0) - ChangeMaterialbuf[0]), E0);
            break;
          }
          case 2: // Load filement1
          {
            setAxisPosition_mm((getAxisPosition_mm(E0) + ChangeMaterialbuf[0]), E0);
            break;
          }
          case 3: // Unload filement2
          {
            setAxisPosition_mm((getAxisPosition_mm(E1) - ChangeMaterialbuf[1]), E1);
            break;
          }
          case 4: // Load filement2
          {
            setAxisPosition_mm((getAxisPosition_mm(E1) + ChangeMaterialbuf[1]), E1);
            break;
          }
          case 5: // sure to heat
          {
            NozzleTempStatus[0] = 1;
            //InforShowoStatus = true;

            setTargetTemp_celsius((PREHEAT_1_TEMP_HOTEND+10), H0);
            IconTemp = getActualTemp_celsius(H0) * 100 / getTargetTemp_celsius(H0);
            if (IconTemp >= 100)
              IconTemp = 100;
            RTS_SndData(IconTemp, HeatPercentIcon);

            RTS_SndData(getActualTemp_celsius(H0), NozzleTemp);
            RTS_SndData(getTargetTemp_celsius(H0), NozzlePreheat);
            delay_ms(5);
            RTS_SndData(ExchangePageBase + 68, ExchangepageAddr);
            break;
          }
          case 6: //cancel to heat
          {
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
            break;
          }
          case 0xF1: //Sure to cancel heating
          {
            //InforShowoStatus = true;
            NozzleTempStatus[0] = 0;
            delay_ms(1);
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
            break;
          }
          case 0xF0: // not to cancel heating
            break;
        }
        RTS_SndData(10 * ChangeMaterialbuf[0], FilementUnit1); //It's ChangeMaterialbuf for show,instead of current_position[E_AXIS] in them.
        RTS_SndData(10 * ChangeMaterialbuf[1], FilementUnit2);
      }
      else if (recdat.addr == FilementUnit1)
      {
        ChangeMaterialbuf[0] = ((float)recdat.data[0]) / 10;
      }
      else if (recdat.addr == FilementUnit2)
      {
        ChangeMaterialbuf[1] = ((float)recdat.data[0]) / 10;
      }
      break;

    case LanguageChoice:

      SERIAL_ECHOLNPGM("\n ***recdat.data[0] =", recdat.data[0]);
      /*if(recdat.data[0]==1) {
          settings.save();
        }
        else {
          injectCommands_P(PSTR("M300"));
        }*/
      // may at some point use language change screens to save eeprom explicitly
      SERIAL_ECHOLNPGM_P(PSTR("InLangChoice"));
      switch(recdat.data[0])
      {
        case 0: {
          SERIAL_ECHOLNPGM_P(PSTR("Store Settings"));
          injectCommands_P(PSTR("M500"));
          break;
        }
        case 1: {
          RTS_SndData(ExchangePageBase + 94, ExchangepageAddr);
          break;
        }
        #if ENABLED(PIDTEMP)
          case 2: {
            onStatusChanged("Hotend PID Started");
            startPIDTune(static_cast<celsius_t>(pid_hotendAutoTemp), getActiveTool());
            break;
          }
        #endif
        case 3: {
          SERIAL_ECHOLNPGM_P(PSTR("Init EEPROM"));
          injectCommands_P(PSTR("M502\nM500"));
          break;
        }
        case 4: {
          SERIAL_ECHOLNPGM_P(PSTR("BLTouch Reset"));
          injectCommands_P(PSTR("M999\nM280P0S160"));
          break;
        }

        case 5: {
          #if ENABLED(PIDTEMPBED)
            onStatusChanged("Bed PID Started");
            startBedPIDTune(static_cast<celsius_t>(pid_bedAutoTemp));
          #else
            SERIAL_ECHOLNPGM_P(PSTR("Bed PID Disabled"));
          #endif
          break;
        }
        case 6: {
          SERIAL_ECHOLNPGM_P(PSTR("Store Settings"));
          injectCommands_P(PSTR("M500"));
          break;
        }
        default: {
          SERIAL_ECHOLNPGM_P(PSTR("Invalid Option"));
          break;
        }
      }
      break;
    case No_Filement:
      SERIAL_ECHOLNPGM_P(PSTR("\n No Filament"));

      if (recdat.data[0] == 1) //Filament is out, resume / resume selected on screen
      {

        if(
        #if DISABLED(FILAMENT_RUNOUT_SENSOR) || ENABLED(FILAMENT_MOTION_SENSOR)
          true
        #elif NUM_RUNOUT_SENSORS > 1
          (getActiveTool() == E0 && READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE) || (getActiveTool() == E1 && READ(FIL_RUNOUT2_PIN) != FIL_RUNOUT2_STATE)
        #else
          getActiveTool() == E0 && READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE
        #endif
        ) {
          SERIAL_ECHOLNPGM_P(PSTR("Resume Yes during print"));
          //setHostResponse(1); //Send Resume host prompt command
          setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
          setUserConfirmed();
          RTS_SndData(1 + CEIconGrap, IconPrintstatus);
          PrinterStatusKey[1] = 3;
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
          //reEntryPrevent = false;
        }
      }
      else if (recdat.data[0] == 0) // Filamet is out, Cancel Selected
      {
        SERIAL_ECHOLNPGM_P(PSTR(" Filament Response No"));
        if(ExtUI::pauseModeStatus == PAUSE_MESSAGE_PURGE || ExtUI::pauseModeStatus == PAUSE_MESSAGE_OPTION) {
          setPauseMenuResponse(PAUSE_RESPONSE_EXTRUDE_MORE);
          setUserConfirmed();
        }
      }
      break;

  #if ENABLED(POWER_LOSS_RECOVERY)
    case PwrOffNoF:
      if (recdat.data[0] == 1) // Yes:continue to print the 3Dmode during power-off.
      {
        injectCommands_P(PSTR("M1000"));
      }
      else if (recdat.data[0] == 2) // No
      {
        injectCommands_P(PSTR("M1000C"));
      }
      break;
  #endif
    case Volume:
      if (recdat.data[0] < 0)
        Settings.display_volume = 0;
      else if (recdat.data[0] > 255)
        Settings.display_volume = 0xFF;
      else
        Settings.display_volume = recdat.data[0];

      if (Settings.display_volume == 0)
      {
        RTS_SndData(0, VolumeIcon);
        RTS_SndData(9, SoundIcon);
      }
      else
      {
        RTS_SndData((Settings.display_volume + 1) / 32 - 1, VolumeIcon);
        RTS_SndData(8, SoundIcon);
      }
      RTS_SndData(Settings.display_volume << 8, SoundAddr + 1);
      break;

    case Filename:
      SERIAL_ECHOLNPGM_P(PSTR("Filename Selected"));
      if (isMediaInserted() && recdat.addr == FilenameChs)
      {
        SERIAL_ECHOLNPGM_P(PSTR("Has Media"));

        recordcount = recdat.data[0] - 1;
        if(filenavigator.currentindex == 0 && filenavigator.folderdepth > 0 && (fileIndex + recordcount) == 0) {
          filenavigator.upDIR();
          SERIAL_ECHOLNPGM_P(PSTR("GoUpDir"));
          filenavigator.getFiles(0);
          fileIndex = 0;
          return;
        }

        if(filenavigator.currentindex == 0 && filenavigator.folderdepth > 0)
          recordcount = recordcount-1; // account for return dir link in file index

        for (int j = 1; j <= 4; j++) // Clear filename BG Color and Frame
        {
          RTS_SndData((unsigned long)0xFFFF, FilenameNature + j * 16); // white
          RTS_SndData(10, FilenameIcon1 + j);							 //clean
        }
        for (int j = 0; j < 10; j++) // clear current filename
            RTS_SndData(0, Choosefilename + j);

        if(filenavigator.getIndexisDir(fileIndex + recordcount)) {
          SERIAL_ECHOLNPGM("Is Dir ", (fileIndex + recordcount));
          filenavigator.changeDIR((char *)filenavigator.getIndexName(fileIndex + recordcount));
          filenavigator.getFiles(0);
          fileIndex = 0;
          return;
        }
        else{
          SERIAL_ECHOLNPGM("Is File ", (fileIndex + recordcount));
          RTS_SndData(filenavigator.getIndexName(fileIndex + recordcount), Choosefilename);
          RTS_SndData((unsigned long)0x87F0, FilenameNature + recdat.data[0] * 16); // Change BG of selected line to Light Green
          RTS_SndData(6, FilenameIcon1 + recdat.data[0]);							  // show frame
        }
      }
      else if (recdat.addr == FilenamePlay)
      {
        if (recdat.data[0] == 1 && isMediaInserted()) //for sure
        {
          printFile(filenavigator.getIndexName(fileIndex + recordcount));

          for (int j = 0; j < 10; j++) //clean screen.
            RTS_SndData(0, Printfilename + j);

          RTS_SndData(filenavigator.getIndexName(fileIndex + recordcount), Printfilename);
          delay_ms(2);

          RTS_SndData(1 + CEIconGrap, IconPrintstatus); // 1 for Heating
          delay_ms(2);
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);

          TPShowStatus = InforShowStatus = true;
          PrinterStatusKey[0] = 1;
          PrinterStatusKey[1] = 3;
          fileIndex = 0;
          recordcount = 0;
        }
        else if(recdat.data[0] == 2) //Page Down
        {
          SERIAL_ECHOLNPGM_P(PSTR("PgDown"));
          if((fileIndex+DISPLAY_FILES) < (filenavigator.maxFiles() + (filenavigator.folderdepth!=0))) {
            fileIndex = fileIndex + DISPLAY_FILES;
            //if(filenavigator.folderdepth!=0 && fileIndex!=0) //Shift to acknowledge Return DIR button on first page
            //  filenavigator.getFiles(fileIndex-1);
           // else
              filenavigator.getFiles(fileIndex);
           //filenavigator.getFiles(filenavigator.currentindex+1);
          }
        }
        else if(recdat.data[0] == 3) //Page Up
        {
          SERIAL_ECHOLNPGM_P(PSTR("PgUp"));
          if(fileIndex>=DISPLAY_FILES) {
            fileIndex = fileIndex - DISPLAY_FILES;
            //if(filenavigator.folderdepth!=0 && fileIndex!=0) //Shift to acknowledge Return DIR button on first page
              //filenavigator.getFiles(filenavigator.currentindex-DISPLAY_FILES);
            //else
            filenavigator.getFiles(fileIndex);
          }
        }
        else if(recdat.data[0] == 4) //Page Up
        {
          SERIAL_ECHOLNPGM_P(PSTR("Refresh"));
          injectCommands_P(PSTR("M22\nM21"));
        }
        else if (recdat.data[0] == 0) //	return to main page
        {
          InforShowStatus = true;
          TPShowStatus = false;
        }
      }
      break;

    case VolumeDisplay:
    {
      SERIAL_ECHOLN("VolumeDisplay");
      if(recdat.data[0]==0) {
        Settings.display_volume = 0;
        Settings.display_sound = false;
      } else if (recdat.data[0] > 100) {
        Settings.display_volume = 255;
        Settings.display_sound = true;
      } else {

        Settings.display_volume = (uint8_t)map(constrain(recdat.data[0], 0, 100), 0, 100, 0, 255);
        Settings.display_sound = true;
      }
      SetTouchScreenConfiguration();
      break;
    }

    case DisplayBrightness:
    {
      SERIAL_ECHOLN("DisplayBrightness");
      SERIAL_ECHOLNPGM("DisplayBrightness LCD: ", recdat.data[0]);
      if(recdat.data[0]<10) {
        Settings.screen_brightness = 10;
      } else if (recdat.data[0] > 100) {
        Settings.screen_brightness = 100;
      } else {
        Settings.screen_brightness = (uint8_t)recdat.data[0];
      }
      SERIAL_ECHOLNPGM("DisplayBrightness Set: ", Settings.screen_brightness);
      SetTouchScreenConfiguration();
      break;
    }

    case DisplayStandbyBrightness:
    {
      SERIAL_ECHOLN("DisplayStandbyBrightness");
      if(recdat.data[0]<10) {
        Settings.standby_screen_brightness = 10;
      } else if (recdat.data[0] > 100) {
        Settings.standby_screen_brightness = 100;
      } else {
        Settings.standby_screen_brightness = (uint8_t)recdat.data[0];
      }
      SetTouchScreenConfiguration();
      break;
    }

    case DisplayStandbySeconds:
    {
      SERIAL_ECHOLN("DisplayStandbySeconds");
      if(recdat.data[0]<5) {
        Settings.standby_time_seconds = 5;
      } else if (recdat.data[0] > 100) {
        Settings.standby_time_seconds = 100;
      } else {
        Settings.standby_time_seconds = (uint8_t)recdat.data[0];
      }
      SetTouchScreenConfiguration();
      break;
    }

    case AutolevelVal:
    {
      uint8_t meshPoint = (recdat.addr - AutolevelVal) / 2;
      uint8_t yPnt = floor(meshPoint / GRID_MAX_POINTS_X);
      uint8_t xPnt;
      if ( yPnt % 2 == 0)
        xPnt = meshPoint - (yPnt*GRID_MAX_POINTS_X);
      else
        xPnt = (GRID_MAX_POINTS_X - 1)- (meshPoint - (yPnt*GRID_MAX_POINTS_X)); //zag row
      float meshVal;
      SERIAL_ECHOLNPGM("meshPoint ", meshPoint);
      SERIAL_ECHOLNPGM("xPnt ", xPnt);
      SERIAL_ECHOLNPGM("yPnt ", yPnt);

      if (recdat.data[0] >= 32768)
        meshVal = ((float)recdat.data[0] - 65536) / 1000;
      else
        meshVal = ((float)recdat.data[0]) / 1000;

      SERIAL_ECHOLNPGM("meshVal ", meshVal);
      meshVal = constrain(meshVal, Z_PROBE_LOW_POINT, Z_CLEARANCE_BETWEEN_PROBES);
      SERIAL_ECHOLNPGM("Constrain meshVal ", meshVal);
      xy_uint8_t point = {xPnt, yPnt};
      setMeshPoint(point, meshVal);
      rtscheck.RTS_SndData((meshVal*1000), recdat.addr);
      break;
    }

    default:
      SERIAL_ECHOLNPGM_P(PSTR("No Match :"));
      break;
    }

	memset(&recdat, 0, sizeof(recdat));
	recdat.head[0] = FHONE;
	recdat.head[1] = FHTWO;
}

 void WriteVariable(uint16_t adr, const void* values, uint8_t valueslen, bool isstr=false, char fillChar = ' ') {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = !myvalues;
  DWIN_SERIAL.write(FHONE);
  DWIN_SERIAL.write(FHTWO);
  DWIN_SERIAL.write(valueslen + 3);
  DWIN_SERIAL.write(0x82);
  DWIN_SERIAL.write(adr >> 8);
  DWIN_SERIAL.write(adr & 0xFF);
  while (valueslen--) {
    char x;
    if (!strend) x = *myvalues++;
    if ((isstr && !x) || strend) {
      strend = true;
      x = fillChar;
    }
    DWIN_SERIAL.write(x);
  }
}

void SetTouchScreenConfiguration() {
  // Main configuration (System_Config)
  LIMIT(Settings.screen_brightness, 10, 100); // Prevent a possible all-dark screen
  LIMIT(Settings.standby_time_seconds, 10, 655); // Prevent a possible all-dark screen for standby, yet also don't go higher than the DWIN limitation

  unsigned char cfg_bits = 0x0;
  cfg_bits |= 1UL << 7; // 7: Enable Control
  cfg_bits |= 1UL << 5; // 5: load 22 touch file
  cfg_bits |= 1UL << 4; // 4: auto-upload should always be enabled
  if (Settings.display_sound) cfg_bits |= 1UL << 3; // 3: audio
  if (Settings.display_standby) cfg_bits |= 1UL << 2; // 2: backlight on standby
  if(Settings.screen_rotation==10) cfg_bits |= 1UL << 1; // 1 & 0: Inversion
  //cfg_bits |= 1UL << 0; // Portrait Mode


  const unsigned char config_set[] = { 0x5A, 0x00, 0xFF, cfg_bits };
  WriteVariable(0x80 /*System_Config*/, config_set, sizeof(config_set));

  // Standby brightness (LED_Config)
  uint16_t dwinStandbyTimeSeconds = 100 * Settings.standby_time_seconds;  /* milliseconds, but divided by 10 (not 5 like the docs say) */
  const unsigned char brightness_set[] = {
    Settings.screen_brightness /*% active*/,
    Settings.standby_screen_brightness /*% standby*/,
    static_cast<uint8_t>(dwinStandbyTimeSeconds >> 8),
    static_cast<uint8_t>(dwinStandbyTimeSeconds)
  };
  WriteVariable(0x82 /*LED_Config*/, brightness_set, sizeof(brightness_set));

  if (!Settings.display_sound)
  {
    rtscheck.RTS_SndData(0, VolumeIcon);
    rtscheck.RTS_SndData(9, SoundIcon);
  }
  else
  {
    rtscheck.RTS_SndData((Settings.display_volume + 1) / 32 - 1, VolumeIcon);
    rtscheck.RTS_SndData(8, SoundIcon);
  }
  rtscheck.RTS_SndData(Settings.display_volume, VolumeIcon - 2);
  rtscheck.RTS_SndData(Settings.display_volume << 8, SoundAddr + 1);
  rtscheck.RTS_SndData(map(constrain(Settings.display_volume, 0, 255), 0, 255, 0, 100), VolumeDisplay);
  rtscheck.RTS_SndData(Settings.screen_brightness, DisplayBrightness);
  rtscheck.RTS_SndData(Settings.standby_screen_brightness, DisplayStandbyBrightness);
  rtscheck.RTS_SndData(Settings.standby_time_seconds, DisplayStandbySeconds);
  if(Settings.display_standby)
    rtscheck.RTS_SndData(3, DisplayStandbyEnableIndicator);
  else
    rtscheck.RTS_SndData(2, DisplayStandbyEnableIndicator);
}

void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
  SERIAL_ECHOLNPGM_P(PSTR("***kill***"));
  //First we send screen available on old versions of software
	rtscheck.RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
  //Then we send the new one Creality added in 1.70.1
	rtscheck.RTS_SndData(ExchangePageBase + 88, ExchangepageAddr);
  int j = 0;
  char outmsg[40];
  char killMsg[strlen_P(FTOP(error)) + strlen_P(FTOP(component)) + 3];
  sprintf_P(killMsg, PSTR(S_FMT ": " S_FMT), FTOP(error), FTOP(component));
  while (j<4)
	{
    outmsg[j] = '*';
    j++;
  }
  while (const char c = pgm_read_byte(killMsg[j-4])) {
    outmsg[j] = c;
    j++;
  }
  while(j<40)
	{
    outmsg[j] = '*';
    j++;
  }
  rtscheck.RTS_SndData(outmsg, MacVersion);
  delay_ms(10);
}

void onMediaInserted()
{
	SERIAL_ECHOLNPGM_P(PSTR("***Initing card is OK***"));
  filenavigator.reset();
  filenavigator.getFiles(0);
  fileIndex = 0;
  recordcount = 0;
}

void onMediaError()
{
  filenavigator.reset();
  for (int i = 0; i < MaxFileNumber; i++)
  {
    for (int j = 0; j < 10; j++)
      rtscheck.RTS_SndData(0, SDFILE_ADDR + i * 10 + j);
  }

  for (int j = 0; j < 10; j++)
  {
    rtscheck.RTS_SndData(0, Printfilename + j);  //clean screen.
    rtscheck.RTS_SndData(0, Choosefilename + j); //clean filename
  }
  for (int j = 0; j < 8; j++)
    rtscheck.RTS_SndData(0, FilenameCount + j);
  for (int j = 1; j <= MaxFileNumber; j++)
  {
    rtscheck.RTS_SndData(10, FilenameIcon + j);
    rtscheck.RTS_SndData(10, FilenameIcon1 + j);
  }
  rtscheck.RTS_SndData(18, IconPrintstatus);
  return;
	SERIAL_ECHOLNPGM_P(PSTR("***Initing card fails***"));
}

void onMediaRemoved()
{
  filenavigator.reset();
  for (int i = 0; i < MaxFileNumber; i++)
  {
    for (int j = 0; j < 10; j++)
      rtscheck.RTS_SndData(0, SDFILE_ADDR + i * 10 + j);
  }

  for (int j = 0; j < 10; j++)
  {
    rtscheck.RTS_SndData(0, Printfilename + j);  //clean screen.
    rtscheck.RTS_SndData(0, Choosefilename + j); //clean filename
  }
  for (int j = 0; j < 8; j++)
    rtscheck.RTS_SndData(0, FilenameCount + j);
  for (int j = 1; j <= MaxFileNumber; j++)
  {
    rtscheck.RTS_SndData(10, FilenameIcon + j);
    rtscheck.RTS_SndData(10, FilenameIcon1 + j);
  }
  rtscheck.RTS_SndData(18, IconPrintstatus);
  return;
	SERIAL_ECHOLN("***Card Removed***");
}

void onPlayTone(const uint16_t frequency, const uint16_t duration) {
	SERIAL_ECHOLNPGM_P(PSTR("***CPlay Tone***"));
  rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
}

void onPrintTimerStarted()
{
	SERIAL_ECHOLNPGM_P(PSTR("==onPrintTimerStarted=="));
  if ( waitway == 7 )
    return;
	PrinterStatusKey[1] = 3;
	InforShowStatus = true;
	rtscheck.RTS_SndData(4 + CEIconGrap, IconPrintstatus);
	delay_ms(1);
	rtscheck.RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
}

void onPrintTimerPaused()
{
	SERIAL_ECHOLNPGM_P(PSTR("==onPrintTimerPaused=="));
	rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr); //Display Pause Screen
}
void onPrintTimerStopped()
{
	SERIAL_ECHOLNPGM_P(PSTR("==onPrintTimerStopped=="));
  if(waitway == 3)
    return;

#if FAN_COUNT > 0
	for (uint8_t i = 0; i < FAN_COUNT; i++)
		setTargetFan_percent(FanOff, (fan_t)i);
#endif
	FanStatus = true;

	PrinterStatusKey[0] = 0;
	InforShowStatus = true;
	TPShowStatus = false;
	rtscheck.RTS_SndData(ExchangePageBase + 51, ExchangepageAddr);
}

void onFilamentRunout()
{
	SERIAL_ECHOLNPGM_P(PSTR("==onFilamentRunout=="));
	PrinterStatusKey[1] = 4;
	TPShowStatus = false;
  rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
}
void onFilamentRunout(extruder_t extruder)
{
	SERIAL_ECHOLNPGM_P(PSTR("==onFilamentRunout=="));
  PrinterStatusKey[1] = 4;
  TPShowStatus = false;
  rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
}
void onUserConfirmRequired(const char *const msg)
{
  PrinterStatusKey[1] = 4;
  TPShowStatus = false;
  switch(ExtUI::pauseModeStatus)
  {
    case PAUSE_MESSAGE_WAITING:
    {
      rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
      onStatusChanged("Press Yes to Continue");
      break;
    }
    case PAUSE_MESSAGE_INSERT:
    {
      rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
      onStatusChanged("Load Filament to Continue");
      break;
    }
    case PAUSE_MESSAGE_HEAT:
    {
      rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
      onStatusChanged("Press Yes to Reheat");
      break;
    }
    #if DISABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
      case PAUSE_MESSAGE_PURGE:
      {
        rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
        char newMsg[40] = "Yes to Continue           No to ";
        if(TERN0(FILAMENT_RUNOUT_SENSOR, ExtUI::getFilamentRunoutState()))
          strcat(newMsg, "Disable");
        else
          strcat(newMsg, "Purge");
        onStatusChanged(newMsg);
        break;
      }
    #endif


    case PAUSE_MESSAGE_OPTION:
    {
      rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
      char newMsg[40] = "Yes to Continue           No to ";
      if(TERN0(FILAMENT_RUNOUT_SENSOR, ExtUI::getFilamentRunoutState()))
        strcat(newMsg, "Disable");
      else
        strcat(newMsg, "Purge");
      onStatusChanged(newMsg);
      break;
    }

    case PAUSE_MESSAGE_PARKING:
    {
      rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
      onStatusChanged("Parking...");
      break;
    }
    case PAUSE_MESSAGE_CHANGING:{
      rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
      onStatusChanged("Beginning Filament Change");
      break;
    }
    case PAUSE_MESSAGE_UNLOAD:{
      rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
      onStatusChanged("Unloading...");
      break;
    }
    case PAUSE_MESSAGE_LOAD:{
      rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
      onStatusChanged("Reloading...");
      break;
    }
    case PAUSE_MESSAGE_RESUME:
    #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
      case PAUSE_MESSAGE_PURGE:{
        rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
        onStatusChanged("Press Yes to Stop Purge");
        break;
      }
    #endif

    case PAUSE_MESSAGE_HEATING:
    {
      rtscheck.RTS_SndData(ExchangePageBase + 68, ExchangepageAddr);
      onStatusChanged("Reheating");
      break;
    }

    case PAUSE_MESSAGE_STATUS:
      default:
      {
        setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
        setUserConfirmed();
        SERIAL_ECHOLNPGM_P(PSTR("Pause Mode Status"));
        break;
      }
  }
	SERIAL_ECHOLNPGM_P(PSTR("==onUserConfirmRequired=="), pauseModeStatus);
}

void onStatusChanged(const char *const statMsg)
{
  for (int j = 0; j < 20; j++) // Clear old message
    rtscheck.RTS_SndData(' ', StatusMessageString+j);
  rtscheck.RTS_SndData(statMsg, StatusMessageString);
}
void onFactoryReset()
{
  Settings.settings_size = sizeof(creality_dwin_settings_t);
  Settings.settings_version = dwin_settings_version;
  Settings.display_standby = true;
  Settings.display_sound = true;
  Settings.display_volume = 32;
  Settings.standby_screen_brightness = 15;
  Settings.screen_brightness = 100;
  Settings.standby_time_seconds = 60;
  Settings.screen_rotation = 0;
  onStartup();
  startprogress = 0;
  InforShowStatus = true;
	SERIAL_ECHOLNPGM_P(PSTR("==onFactoryReset=="));

}

void onMeshUpdate(const int8_t xpos, const int8_t ypos, probe_state_t state) {

}

void onMeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval)
{
  if(waitway==3)
    if(isPositionKnown() && (getActualTemp_celsius(BED) >= (getTargetTemp_celsius(BED)-1)))
			  rtscheck.RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
  #if HAS_MESH
    uint8_t abl_probe_index = 0;
        for(uint8_t outer = 0; outer < GRID_MAX_POINTS_Y; outer++)
        {
          for (uint8_t inner = 0; inner < GRID_MAX_POINTS_X; inner++)
          {
            uint8_t x_Point = inner;
            bool zig = (outer & 1); // != ((PR_OUTER_END) & 1);
            if (zig) x_Point = (GRID_MAX_POINTS_X - 1) - inner;
            xy_uint8_t point = {x_Point, outer};
            if(x_Point==xpos && outer ==ypos)
              rtscheck.RTS_SndData(ExtUI::getMeshPoint(point) * 1000, AutolevelVal + (abl_probe_index * 2));
            ++abl_probe_index;
          }
        }
  #endif
};

void onStoreSettings(char *buff)
{
	static_assert(
    ExtUI::eeprom_data_size >= sizeof(creality_dwin_settings_t),
    "Insufficient space in EEPROM for UI parameters"
  );

  // Write to buffer
  SERIAL_ECHOLNPGM("Saving DWIN LCD setting from EEPROM");
  memcpy(buff, &Settings, sizeof(creality_dwin_settings_t));
}

void onLoadSettings(const char *buff)
{
	static_assert(
    ExtUI::eeprom_data_size >= sizeof(creality_dwin_settings_t),
    "Insufficient space in EEPROM for UI parameters"
  );

  creality_dwin_settings_t eepromSettings;
  memcpy(&eepromSettings, buff, sizeof(creality_dwin_settings_t));

  // If size is not the same, discard settings
  if (eepromSettings.settings_size != sizeof(creality_dwin_settings_t)) {
    SERIAL_ECHOLNPGM("Discarding DWIN LCD setting from EEPROM - size incorrect");

    onFactoryReset();
    return;
  }

  if (eepromSettings.settings_version != dwin_settings_version) {
    SERIAL_ECHOLNPGM("Discarding DWIN LCD setting from EEPROM - settings version incorrect");

    onFactoryReset();
    return;
  }

  // Copy into final location
  SERIAL_ECHOLNPGM("Loading DWIN LCD setting from EEPROM");
  memcpy(&Settings, &eepromSettings, sizeof(creality_dwin_settings_t));

  SERIAL_ECHOLNPGM("Setting Brightness : ", Settings.screen_brightness);
  SERIAL_ECHOLNPGM("Setting Standby : ", Settings.standby_screen_brightness);
  SERIAL_ECHOLNPGM("Setting Standby Time : ", Settings.standby_time_seconds);
  SERIAL_ECHOLNPGM("Setting Rotation : ", Settings.screen_rotation);
  SERIAL_ECHOLNPGM("Setting Volume : ", Settings.display_volume);

  SERIAL_ECHOLNPGM("Setting Standby On : ", Settings.display_standby);
  SERIAL_ECHOLNPGM("Setting Volume On : ", Settings.display_sound);

  SetTouchScreenConfiguration();
}

void onConfigurationStoreWritten(bool success)
{
	SERIAL_ECHOLNPGM_P(PSTR("==onConfigurationStoreWritten=="));
	// This is called after the entire EEPROM has been written,
	// whether successful or not.
}

void onConfigurationStoreRead(bool success)
{
	SERIAL_ECHOLNPGM_P(PSTR("==onConfigurationStoreRead=="));
  #if HAS_MESH && (ANY(MachineCR10SPro, MachineEnder5Plus, MachineCR10Max) || ENABLED(FORCE10SPRODISPLAY))
    if (ExtUI::getMeshValid())
    {
      uint8_t abl_probe_index = 0;
        for(uint8_t outer = 0; outer < GRID_MAX_POINTS_Y; outer++)
        {
          for (uint8_t inner = 0; inner < GRID_MAX_POINTS_X; inner++)
          {
            uint8_t x_Point = inner;
            bool zig = (outer & 1);
            if (zig) x_Point = (GRID_MAX_POINTS_X - 1) - inner;
            xy_uint8_t point = {x_Point, outer};
            rtscheck.RTS_SndData(ExtUI::getMeshPoint(point) * 1000, AutolevelVal + (abl_probe_index * 2));
            ++abl_probe_index;
          }
        }

      rtscheck.RTS_SndData(3, AutoLevelIcon); //2=On, 3=Off
      setLevelingActive(true);
    }
    else
    {
      rtscheck.RTS_SndData(2, AutoLevelIcon); /*Off*/
      setLevelingActive(false);
    }
  #endif

	SERIAL_ECHOLNPGM("\n init zprobe_zoffset = ", getZOffset_mm());
	rtscheck.RTS_SndData(getZOffset_mm() * 100, ProbeOffset_Z);
  SetTouchScreenConfiguration();
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void onPowerLossResume() {
    SERIAL_ECHOLNPGM_P(PSTR("==OnPowerLossResume=="));
    startprogress = 254;
    InforShowStatus = true;
    TPShowStatus = false;
    reEntryPrevent = false;
    rtscheck.RTS_SndData(ExchangePageBase + 76, ExchangepageAddr);
  }
#endif

#if HAS_PID_HEATING
  void onPidTuning(const result_t rst) {
    // Called for temperature PID tuning result
    rtscheck.RTS_SndData(pid_hotendAutoTemp, HotendPID_AutoTmp);
    rtscheck.RTS_SndData(pid_bedAutoTemp, BedPID_AutoTmp);
    rtscheck.RTS_SndData((unsigned int)(getPIDValues_Kp(E0) * 10), HotendPID_P);
    rtscheck.RTS_SndData((unsigned int)(getPIDValues_Ki(E0) * 10), HotendPID_I);
    rtscheck.RTS_SndData((unsigned int)(getPIDValues_Kd(E0) * 10), HotendPID_D);
    #if ENABLED(PIDTEMPBED)
      rtscheck.RTS_SndData((unsigned int)(getBedPIDValues_Kp() * 10), BedPID_P);
      rtscheck.RTS_SndData((unsigned int)(getBedPIDValues_Ki() * 10), BedPID_I);
      rtscheck.RTS_SndData((unsigned int)(getBedPIDValues_Kd() * 10), BedPID_D);
    #endif
    onStatusChanged("PID Tune Finished");
  }
#endif
void onMeshLevelingStart() {

}

void onSteppersEnabled()
{

}

void onPrintFinished()
{

}

void onHomingStart()
{

}

void onHomingComplete()
{

}

void onSteppersDisabled()
{

}

void onPostprocessSettings()
{

}


} // namespace ExtUI

#endif