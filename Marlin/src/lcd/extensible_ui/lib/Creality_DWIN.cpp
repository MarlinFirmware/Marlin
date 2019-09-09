#include "Creality_DWIN.h"
#include <HardwareSerial.h>
#include <arduino.h>
#include <wstring.h>
#include <stdio.h>
#include "../ui_api.h"

#if ENABLED(EXTENSIBLE_UI)
namespace ExtUI
{
  uint8_t waitway_lock = 0;
  const float manual_feedrate_mm_m[] = MANUAL_FEEDRATE;
  int startprogress = 0;
  CRec CardRecbuf;
  #if DISABLED(POWER_LOSS_RECOVERY)
    int power_off_type_yes = 0;
    int power_off_commands_count = 0;
  #endif

  char waitway = 0;
  int recnum = 0;
  unsigned char Percentrecord = 0;
  float ChangeMaterialbuf[2] = {0};

  char NozzleTempStatus[3] = {0};

  bool PrintMode = false; //Eco Mode default off

  char PrintStatue[2] = {0};		//PrintStatue[0], 0 represent  to 43 page, 1 represent to 44 page

  char PrinterStatusKey[2] = {0}; // PrinterStatusKey[1] value: 0 represents to keep temperature, 1 represents  to heating , 2 stands for cooling , 3 stands for printing
                  // PrinterStatusKey[0] value: 0 reprensents 3D printer ready
  char FilementStatus[2] = {0};

  unsigned char AxisPagenum = 0; //0 for 10mm, 1 for 1mm, 2 for 0.1mm
  bool InforShowStatus = true;
  bool TPShowStatus = false; // true for only opening time and percentage, false for closing time and percentage.
  bool FanStatus = true;
  bool AutohomeKey = false;
  unsigned char AutoHomeIconNum;
  unsigned long VolumeSet = 0x80;
  extern char power_off_commands[9][96];
  bool PoweroffContinue = false;

  bool reEntryPrevent = false;
  uint16_t idleThrottling = 0;

void onStartup()
{
	Serial2.begin(115200);

	rtscheck.recdat.head[0] = rtscheck.snddat.head[0] = FHONE;
	rtscheck.recdat.head[1] = rtscheck.snddat.head[1] = FHTWO;
	memset(rtscheck.databuf, 0, sizeof(rtscheck.databuf));

	//VolumeSet = eeprom_read_byte((unsigned char*)FONT_EEPROM+4);
	//if(VolumeSet < 0 || VolumeSet > 0xFF)
	VolumeSet = 0x20;

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
	rtscheck.RTS_SndData(CUSTOM_MACHINE_NAME, MacVersion);
	rtscheck.RTS_SndData(DETAILED_BUILD_VERSION, SoftVersion);
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

	SERIAL_ECHOLN("==Dwin Init Complete==");
}

void onIdle()
{
  if (reEntryPrevent)
    return;
  if(idleThrottling++ < 1000){
    return;
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
        SERIAL_ECHOLN("==waitway 1==");
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
      SERIAL_ECHOLN("==waitway 3==");
      //if(isPositionKnown() && (getActualTemp_celsius(BED) >= (getTargetTemp_celsius(BED)-1))) {
			  rtscheck.RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
        waitway = 7;
        //return;
      //}
			break;

		case 4:
			if (AutohomeKey && isPositionKnown() && !commandsInQueue())
			{ //Manual Move Home Done
        SERIAL_ECHOLN("==waitway 4==");
				rtscheck.RTS_SndData(ExchangePageBase + 71 + AxisPagenum, ExchangepageAddr);
				AutohomeKey = false;
				waitway = 0;
			}
			break;
		case 5:
        if(isPositionKnown() && !commandsInQueue()) {
        InforShowStatus = true;
        waitway = 0;
        SERIAL_ECHOLN("==waitway 5==");
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


    #if ENABLED(POWER_LOSS_RECOVERY)
      if (PoweroffContinue)
      {
        PoweroffContinue = false;
        injectCommands_P(power_off_commands[3]);
        card.startFileprint();
        print_job_timer.power_off_start();
      }
    #endif

  void yield();

	if (InforShowStatus)
	{
    if(power_off_type_yes ==0)
    {
      if (startprogress == 0)
			{
				rtscheck.RTS_SndData(StartSoundSet, SoundAddr);

				if (VolumeSet == 0)
				{
					rtscheck.RTS_SndData(0, VolumeIcon);
					rtscheck.RTS_SndData(9, SoundIcon);
				}
				else
				{
					rtscheck.RTS_SndData((VolumeSet + 1) / 32 - 1, VolumeIcon);
					rtscheck.RTS_SndData(8, SoundIcon);
				}
				rtscheck.RTS_SndData(VolumeSet, VolumeIcon - 2);
				rtscheck.RTS_SndData(VolumeSet << 8, SoundAddr + 1);
        if (getLevelingActive())
			    rtscheck.RTS_SndData(2, AutoLevelIcon); /*Off*/
		    else
			    rtscheck.RTS_SndData(3, AutoLevelIcon); /*On*/
			}
			if (startprogress <= 100)
				rtscheck.RTS_SndData(startprogress, StartIcon);
			else
				rtscheck.RTS_SndData((startprogress - 100), StartIcon + 1);
      if ((startprogress += 1) > 200)
			{
        #if ENABLED(POWER_LOSS_RECOVERY)
          if(isMediaInserted() && (power_off_commands_count > 0)) {
            for (uint16_t i = 0; i < CardRecbuf.Filesum; i++)
            {
              if (!strcmp(CardRecbuf.Cardfilename[i], &power_off_info.sd_filename[1]))
              {
                InforShowStatus = true;
                int filelen = strlen(CardRecbuf.Cardshowfilename[i]);
                filelen = (TEXTBYTELEN - filelen) / 2;
                if (filelen > 0)
                {
                  char buf[20];
                  memset(buf, 0, sizeof(buf));
                  strncpy(buf, "         ", filelen);
                  strcpy(&buf[filelen], CardRecbuf.Cardshowfilename[i]);
                  RTS_SndData(buf, Printfilename);
                }
                else
                  RTS_SndData(CardRecbuf.Cardshowfilename[i], Printfilename); //filenames
                RTS_SndData(ExchangePageBase + 76, ExchangepageAddr);
                break;
              }
            }
          }
          reEntryPrevent = false;
          return;
        #endif
          SERIAL_ECHOLN("  startprogress ");
          power_off_type_yes = 1;
          InforShowStatus = true;
          TPShowStatus = false;
          rtscheck.RTS_SndData(ExchangePageBase + 45, ExchangepageAddr);
			}
      reEntryPrevent = false;
			return;
    }

			if (isPrinting()) //need to optimize
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


			rtscheck.RTS_SndData(getZOffset_mm() * 100, 0x1026);
			//float temp_buf = getActualTemp_celsius(H0);
			rtscheck.RTS_SndData(getActualTemp_celsius(H0), NozzleTemp);
			rtscheck.RTS_SndData(getActualTemp_celsius(BED), Bedtemp);
      rtscheck.RTS_SndData(getTargetTemp_celsius(H0), NozzlePreheat);
			rtscheck.RTS_SndData(getTargetTemp_celsius(BED), BedPreheat);

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
          SERIAL_ECHOLN("==Heating Done Change Filament==");
					rtscheck.RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
				}
				else if (getActualTemp_celsius(H0) >= getTargetTemp_celsius(H0) && NozzleTempStatus[2])
				{
					SERIAL_ECHOPAIR("\n ***NozzleTempStatus[2] =", (int)NozzleTempStatus[2]);
					startprogress = NozzleTempStatus[2] = 0;
					TPShowStatus = true;
					rtscheck.RTS_SndData(4, ExchFlmntIcon);
					rtscheck.RTS_SndData(ExchangePageBase + 83, ExchangepageAddr);
				}
				else if (NozzleTempStatus[2])
				{
					rtscheck.RTS_SndData((startprogress++) % 5, ExchFlmntIcon);
				}
			}

			if (AutohomeKey)
			{
				rtscheck.RTS_SndData(AutoHomeIconNum++, AutoZeroIcon);
				if (AutoHomeIconNum > 9)
					AutoHomeIconNum = 0;
			}

    if(rtscheck.recdat.addr != DisplayZaxis && rtscheck.recdat.addr != DisplayYaxis && rtscheck.recdat.addr != DisplayZaxis) {
			rtscheck.RTS_SndData(10 * getAxisPosition_mm((axis_t)X), DisplayXaxis);
		  rtscheck.RTS_SndData(10 * getAxisPosition_mm((axis_t)Y), DisplayYaxis);
		  rtscheck.RTS_SndData(10 * getAxisPosition_mm((axis_t)Z), DisplayZaxis);
    }

	}
  void yield();
	if (rtscheck.RTS_RecData() > 0)
		rtscheck.RTS_HandleData();
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
	while (Serial2.available() > 0 && (recnum < SizeofDatabuf))
	{
		databuf[recnum] = Serial2.read();
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
			Serial2.write(databuf[i]);
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

	if (len > 0)
	{
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
			Serial2.write(databuf[i]);
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

void RTSSHOW::RTS_SDCardUpate(bool removed, bool inserted)
{
	SERIAL_ECHOLN("SDUpdate");
  bool shouldCheck;
  if(!removed && !inserted)
    shouldCheck = isMediaInserted();
  else
    shouldCheck = false;

  if (inserted || shouldCheck)
  {
    ExtUI::FileList files;
    files.count();

    int addrnum = 0;
    int num = 0;
    for (uint16_t i = 0; i < files.count() && i < (uint16_t)MaxFileNumber + addrnum; i++)
    {
      files.seek(i);
      files.filename();
      const char *pointFilename = files.longFilename();
      int filenamelen = strlen(files.longFilename());
      int j = 1;
      while ((strncmp(&pointFilename[j], ".gcode", 6) && strncmp(&pointFilename[j], ".GCODE", 6)) && (j++) < filenamelen)
          ;
      if (j >= filenamelen)
      {
        addrnum++;
        continue;
      }

      if (j >= TEXTBYTELEN)
      {
        //strncpy(&files.longFilename[TEXTBYTELEN -3],"~~",2);
        //files.longFilename()[TEXTBYTELEN-1] = '\0';
        j = TEXTBYTELEN - 1;
      }

      strncpy(CardRecbuf.Cardshowfilename[num], files.longFilename(), j);

      strcpy(CardRecbuf.Cardfilename[num], files.shortFilename());
      CardRecbuf.addr[num] = SDFILE_ADDR + num * 10;
      rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[num], CardRecbuf.addr[num]);
      CardRecbuf.Filesum = (++num);
      //SERIAL_ECHO("  CardRecbuf.Filesum ==");
      //SERIAL_ECHO(CardRecbuf.Filesum);
      rtscheck.RTS_SndData(1, FilenameIcon + CardRecbuf.Filesum);
    }
      rtscheck.RTS_SndData(17, IconPrintstatus);
    return;
  }

  if(removed || !shouldCheck)
  {
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
    SERIAL_ECHOLN("***Card Removed***");
    rtscheck.RTS_SndData(18, IconPrintstatus);
    return;
  }
}

void RTSSHOW::RTS_HandleData()
{
	int Checkkey = -1;
	SERIAL_ECHOLN("  *******RTS_HandleData******** ");
	if (waitway > 0) //for waiting
	{
		SERIAL_ECHOLNPAIR("waitway ==", (int)waitway);
		memset(&recdat, 0, sizeof(recdat));
		recdat.head[0] = FHONE;
		recdat.head[1] = FHTWO;
		return;
	}
	SERIAL_ECHOLNPAIR("recdat.data[0] ==", recdat.data[0]);
	SERIAL_ECHOLNPAIR("recdat.addr ==", recdat.addr);
	for (int i = 0; Addrbuf[i] != 0; i++)
	{
		if (recdat.addr == Addrbuf[i])
		{
			if (Addrbuf[i] >= Stopprint && Addrbuf[i] <= Resumeprint)
				Checkkey = PrintChoice;
			else if (Addrbuf[i] == NzBdSet || Addrbuf[i] == NozzlePreheat || Addrbuf[i] == BedPreheat)
				Checkkey = ManualSetTemp;
			else if (Addrbuf[i] >= AutoZero && Addrbuf[i] <= DisplayZaxis)
				Checkkey = XYZEaxis;
			else if (Addrbuf[i] >= FilementUnit1 && Addrbuf[i] <= FilementUnit2)
				Checkkey = Filement;
			else
				Checkkey = i;
			break;
		}
	}

	if (recdat.addr >= SDFILE_ADDR && recdat.addr <= (SDFILE_ADDR + 10 * (FileNum + 1)))
		Checkkey = Filename;

	if (Checkkey < 0)
	{
		memset(&recdat, 0, sizeof(recdat));
		recdat.head[0] = FHONE;
		recdat.head[1] = FHTWO;
		return;
	}
	SERIAL_ECHO("== Checkkey==");
	SERIAL_ECHOLN(Checkkey);

  #if (ENABLED(MachineCRX) && DISABLED(Force10SProDisplay)) || ENABLED(ForceCRXDisplay)
      const unsigned short topLeftData = 1;
      const unsigned short topRightData = 2;
      const unsigned short lowLeftData = 4;
      const unsigned short lowRightData = 5;
      const unsigned short centerData = 3;
      const unsigned short homeZ = 99;
      const unsigned short babystepUp = 98;
      const unsigned short babystepDown = 97;
      const unsigned short autoMeasure = 96;
      const unsigned short assistEntry = 95;
      const unsigned short levelOn = 94;

    #else
      const uint8_t topLeftData = 7;
      const uint8_t topRightData = 8;
      const uint8_t lowLeftData = 10;
      const uint8_t lowRightData = 9;
      const uint8_t centerData = 6;
      const uint8_t homeZ = 1;
      const uint8_t babystepUp = 2;
      const uint8_t babystepDown = 3;
      const uint8_t autoMeasure = 5;
      const uint8_t assistEntry = 4;
      const uint8_t levelOn = 11;
    #endif
SERIAL_ECHOLN(PSTR("BeginSwitch"));

	switch (Checkkey)
	{
    case Printfile:
      if (recdat.data[0] == 1) // card
      {
        InforShowStatus = false;
        CardRecbuf.recordcount = -1;
        RTS_SDCardUpate(false, false);
        SERIAL_ECHO("\n Handle Data PrintFile 1 Setting Screen ");
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

        SERIAL_ECHO("\n Handle Data PrintFile 2 Setting Screen ");
        RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); //exchange to 45 page
      }
      else if (recdat.data[0] == 3) // Temperature control
      {
        InforShowStatus = true;
        TPShowStatus = false;
        SERIAL_ECHO("\n Handle Data PrintFile 3 Setting Screen ");
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
        SERIAL_ECHO("\n Handle Data Adjust 2 Setting Screen ");
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
        RTS_SndData(ExchangePageBase + 45, ExchangepageAddr);
        RTS_SndData(0, Timehour);
        RTS_SndData(0, Timemin);
        stopPrint();
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
        PrintStatue[1] = 0;
        //PrinterStatusKey[1] = 3;
        RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
      }
      if (recdat.addr == Resumeprint && recdat.data[0] == 2) // warming
      {
        resumePrint();
        NozzleTempStatus[2] = 1;
        PrinterStatusKey[1] = 0;
        InforShowStatus = true;
        startprogress = 0;
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

      if (WITHIN((tmp_zprobe_offset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
      {
        babystepAxis_steps((400 * (getZOffset_mm() - tmp_zprobe_offset) * -1), (axis_t)Z);
        setZOffset_mm(tmp_zprobe_offset);
        injectCommands_P((PSTR("M500")));
      }
      else
      {
        RTS_SndData(getZOffset_mm() * 100, 0x1026);
      }
      char zOffs[20], tmp1[11];
      sprintf_P(zOffs, PSTR("Z Offset : %s"), dtostrf(getZOffset_mm(), 1, 3, tmp1));
      onStatusChanged(zOffs);
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
      {
        setTargetTemp_celsius((float)recdat.data[0], H0);
      }
      else if (recdat.addr == BedPreheat)
      {
        setTargetTemp_celsius((float)recdat.data[0], BED);
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
        if (getLevelingActive())
          RTS_SndData(3, AutoLevelIcon);
        else
          RTS_SndData(2, AutoLevelIcon);

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
        //InforShowoStatus = false;
        AxisPagenum = 0;
        RTS_SndData(10 * getAxisPosition_mm((axis_t)X), DisplayXaxis);
        RTS_SndData(10 * getAxisPosition_mm((axis_t)Y), DisplayYaxis);
        RTS_SndData(10 * getAxisPosition_mm((axis_t)Z), DisplayZaxis);
        delay_ms(2);
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
      switch(recdat.data[0])
      {
        case homeZ: // Z-axis to home
        {
          // Disallow Z homing if X or Y are unknown
          if (!isAxisPositionKnown((axis_t)X) || !isAxisPositionKnown((axis_t)Y))
            injectCommands_P(PSTR("G28\nG1F1500Z0.0"));
          else
            injectCommands_P(PSTR("G28Z\nG1F1500Z0.0"));

          RTS_SndData(getZOffset_mm() * 100, 0x1026);
          break;
        }
        case babystepUp: // Z-axis to Up
        {
          if (WITHIN((getZOffset_mm() + 0.1), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          {
            babystepAxis_steps(40, (axis_t)Z);
            setZOffset_mm(getZOffset_mm() + 0.1);
            RTS_SndData(getZOffset_mm() * 100, 0x1026);
            char zOffs[20], tmp1[11];
            sprintf_P(zOffs, PSTR("Z Offset : %s"), dtostrf(getZOffset_mm(), 1, 3, tmp1));
            onStatusChanged(zOffs);
            injectCommands_P(PSTR("M500"));
          }
          break;
        }
        case babystepDown: // Z-axis to Down
        {
          if (WITHIN((getZOffset_mm() - 0.1), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          {
            babystepAxis_steps(-40, (axis_t)Z);
            setZOffset_mm(getZOffset_mm() - 0.1);
            RTS_SndData(getZOffset_mm() * 100, 0x1026);
            char zOffs[20], tmp1[11];
            sprintf_P(zOffs, PSTR("Z Offset : %s"), dtostrf(getZOffset_mm(), 1, 3, tmp1));
            onStatusChanged(zOffs);
            injectCommands_P(PSTR("M500"));
          }
          break;
        }
        case assistEntry: // Assitant Level
        {
          setLevelingActive(false);
          if (!isPositionKnown())
            injectCommands_P((PSTR("G28\nG1 F1000 Z0.0")));
          else
            injectCommands_P((PSTR("G1 F1000 Z0.0")));
          waitway = 2;
          RTS_SndData(ExchangePageBase + 84, ExchangepageAddr);
          break;
        }
        case autoMeasure: // AutoLevel "Measuring" Button
        {
          waitway = 3; //only for prohibiting to receive massage
          RTS_SndData(3, AutolevelIcon);
          bool zig = true;
          for (uint8_t yCount = 0, showcount = 0; yCount < GRID_MAX_POINTS_Y; yCount++)
          {
            int8_t inStart, inStop, inInc;
            if (zig)
            { // away from origin
              inStart = 0;
              inStop = GRID_MAX_POINTS_X;
              inInc = 1;
            }
            else
            { // towards origin
              inStart = GRID_MAX_POINTS_X - 1;
              inStop = -1;
              inInc = -1;
            }

            zig ^= true; // zag
            for (int8_t xCount = inStart; xCount != inStop; xCount += inInc)
            {
              if ((showcount+1) < (GRID_MAX_POINTS_X * GRID_MAX_POINTS_X))
              {
                if (
                #if ENABLED(ABL_UBL)
                  xCount != (GRID_MAX_POINTS_X - 1) && yCount != (GRID_MAX_POINTS_Y - 1)
                #else
                  true
                #endif
                ){
              showcount++;
                  rtscheck.RTS_SndData(ExtUI::getMeshPoint(xCount, yCount) * 1000, AutolevelVal + (showcount - 1) * 2);
                }
              }
            }
          }
          RTS_SndData(ExchangePageBase + 85, ExchangepageAddr);
          injectCommands_P(PSTR(USER_GCODE_1));
          break;
        }

        case centerData: // Assitant Level ,  Centre 1
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm(X_CENTER, (axis_t)X);
          setAxisPosition_mm(Y_CENTER, (axis_t)Y);
          waitway = 6;
          break;
        }
        case topLeftData: // Assitant Level , Front Left 2
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MIN_BED + LEVEL_CORNERS_INSET), (axis_t)X);
          setAxisPosition_mm((Y_MIN_BED + LEVEL_CORNERS_INSET), (axis_t)Y);
          waitway = 6;
          break;
        }
        case topRightData: // Assitant Level , Front Right 3
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MAX_BED - LEVEL_CORNERS_INSET), (axis_t)X);
          setAxisPosition_mm((Y_MIN_BED + LEVEL_CORNERS_INSET), (axis_t)Y);
          waitway = 6;
          break;
        }
        case lowRightData: // Assitant Level , Back Right 4
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MAX_BED - LEVEL_CORNERS_INSET), (axis_t)X);
          setAxisPosition_mm((Y_MAX_BED - LEVEL_CORNERS_INSET), (axis_t)Y);
          waitway = 6;
          break;
        }
        case lowLeftData: // Assitant Level , Back Left 5
        {
          setAxisPosition_mm(LEVEL_CORNERS_Z_HOP, (axis_t)Z);
          setAxisPosition_mm((X_MIN_BED + LEVEL_CORNERS_INSET), (axis_t)X);
          setAxisPosition_mm((Y_MAX_BED - LEVEL_CORNERS_INSET), (axis_t)Y);
          waitway = 6;
          break;
        }
        case levelOn: // Autolevel switch
        {
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
          RTS_SndData(getZOffset_mm() * 100, 0x1026);
          break;
        }
        default:
        {
          SERIAL_ECHOLN(PSTR("Unsupported Option Selected"));
        }
      }

      RTS_SndData(10, FilenameIcon);
      break;

    case XYZEaxis:
      axis_t axis;
      float min, max;
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

      targetPos = ((float)recdat.data[0]) / 10;

      if (targetPos < min)
        targetPos = min;
      else if (targetPos > max)
        targetPos = max;

      setAxisPosition_mm(targetPos, axis);
      delay_ms(1);
      RTS_SndData(10 * getAxisPosition_mm((axis_t)X), DisplayXaxis);
      RTS_SndData(10 * getAxisPosition_mm((axis_t)Y), DisplayYaxis);
      RTS_SndData(10 * getAxisPosition_mm((axis_t)Z), DisplayZaxis);
      delay_ms(1);
      waitway = 0;
      RTS_SndData(10, FilenameIcon);
      break;

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

      SERIAL_ECHOPAIR("\n ***recdat.data[0] =", recdat.data[0]);
      /*if(recdat.data[0]==1) {
          settings.save();
        }
        else {
          injectCommands_P(PSTR("M300"));
        }*/
      // may at some point use language change screens to save eeprom explicitly
      switch(recdat.data[0])
      {
        case 0:
          SERIAL_ECHOLN("Chinese Not Supported");
          break;
        case 1:
          SERIAL_ECHOLN("English Already Set");
          break;
        case 2:
          //PID Hotend
          injectCommands_P(PSTR("M303E0S215C8U"));
          break;
        case 3:
          //Init EEPROM
          injectCommands_P(PSTR("M502\nM500"));
          break;
        case 4:
          //BLTouch Reset
          injectCommands_P(PSTR("M999"));
          break;
        case 5:
          //PID Bed
          injectCommands_P(PSTR("M303E-1S65C4U"));
          break;
        default:
          SERIAL_ECHOLN("Invalid Option");
          break;
      }
      break;
    case No_Filement:
      SERIAL_ECHOLN("\n No Filament");

      if (recdat.data[0] == 1) //Filament is out, resume / resume selected on screen
      {
        if(
        #if DISABLED(FILAMENT_RUNOUT_SENSOR)
          true
        #elif NUM_RUNOUT_SENSORS > 1
          (getActiveTool() == E0 && READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_INVERTING) || (getActiveTool() == E1 && READ(FIL_RUNOUT2_PIN) != FIL_RUNOUT_INVERTING)
        #else
          getActiveTool() == E0 && READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_INVERTING
        #endif
        ) {
          SERIAL_ECHOLN("Resume Yes during print");
          setHostResponse(1); //Send Resume host prompt command
          RTS_SndData(1 + CEIconGrap, IconPrintstatus);
          PrintStatue[1] = 0;
          PrinterStatusKey[1] = 3;
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
          FilementStatus[0] = 0; // recover the status waiting to check filements
        }
      }
      else if (recdat.data[0] == 0) // Filamet is out, Cancel Selected
      {
          SERIAL_ECHOLN(" Filament Response No");
          if (FilementStatus[0] == 1)
          {
            SERIAL_ECHOLN("Filament Stat 0 - 1");
            RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
            PrinterStatusKey[0] = 0;
            setUserConfirmed();
          }
          else if (FilementStatus[0] == 2) // like the pause
          {
            SERIAL_ECHOLN("Filament Stat 0 - 2");
            RTS_SndData(ExchangePageBase + 54, ExchangepageAddr);
            setUserConfirmed();
          }
          else if (FilementStatus[0] == 3)
          {
            SERIAL_ECHOLN("Filament Stat 0 - 3");
            RTS_SndData(ExchangePageBase + 65, ExchangepageAddr);
            setUserConfirmed();
          }
          FilementStatus[0] = 0; // recover the status waiting to check filements
          stopPrint();
      }
      break;

  #if ENABLED(POWER_LOSS_RECOVERY)
    case PwrOffNoF:
      char cmd1[30];
      if (recdat.data[0] == 1) // Yes:continue to print the 3Dmode during power-off.
      {
        if (power_off_commands_count > 0)
        {
          sprintf_P(cmd1, PSTR("M190 S%i"), power_off_info.target_temperature_bed);
          injectCommands_P(cmd1);
          sprintf_P(cmd1, PSTR("M109 S%i"), power_off_info.target_temperature[0]);
          injectCommands_P(cmd1);
          injectCommands_P(PSTR("M106 S255"));
          sprintf_P(cmd1, PSTR("T%i"), power_off_info.saved_extruder);
          injectCommands_P(cmd1);
          power_off_type_yes = 1;

  #if FAN_COUNT > 0
          for (uint8_t i = 0; i < FAN_COUNT; i++)
            fanSpeeds[i] = FanOn;
  #endif
          FanStatus = false;

          PrintStatue[1] = 0;
          PrinterStatusKey[0] = 1;
          PrinterStatusKey[1] = 3;
          PoweroffContinue = true;
          TPShowStatus = InforShowStatus = true;

          RTS_SndData(1 + CEIconGrap, IconPrintstatus);
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);

          //card.startFileprint();
          //print_job_timer.power_off_start();
        }
      }
      else if (recdat.data[0] == 2) // No
      {
        InforShowStatus = true;
        TPShowStatus = false;
        RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); //exchange to 45 page

        stopPrint();

  #if ENABLED(SDSUPPORT) && ENABLED(POWEROFF_SAVE_SD_FILE)
        card.openPowerOffFile(power_off_info.power_off_filename, O_CREAT | O_WRITE | O_TRUNC | O_SYNC);
        power_off_info.valid_head = 0;
        power_off_info.valid_foot = 0;
        if (card.savePowerOffInfo(&power_off_info, sizeof(power_off_info)) == -1)
        {
          SERIAL_ECHOLN("Stop to Write power off file failed.");
        }
        card.closePowerOffFile();
        power_off_commands_count = 0;
  #endif

        wait_for_heatup = false;
        PrinterStatusKey[0] = 0;
        delay_ms(500); //for system
      }
      break;
  #endif
    case Volume:
      if (recdat.data[0] < 0)
        VolumeSet = 0;
      else if (recdat.data[0] > 255)
        VolumeSet = 0xFF;
      else
        VolumeSet = recdat.data[0];

      if (VolumeSet == 0)
      {
        RTS_SndData(0, VolumeIcon);
        RTS_SndData(9, SoundIcon);
      }
      else
      {
        RTS_SndData((VolumeSet + 1) / 32 - 1, VolumeIcon);
        RTS_SndData(8, SoundIcon);
      }
      //eeprom_write_byte((unsigned char*)FONT_EEPROM+4, VolumeSet);
      RTS_SndData(VolumeSet << 8, SoundAddr + 1);
      break;

    case Filename:
      SERIAL_ECHOLN("Filename");
      if (isMediaInserted() && recdat.addr == FilenameChs)
      {
        SERIAL_ECHOLN("Filename-Media");
        if (recdat.data[0] > (uint8_t)CardRecbuf.Filesum)
          break;

        SERIAL_ECHOLN("Recdata");
        CardRecbuf.recordcount = recdat.data[0] - 1;
        for (int j = 0; j < 10; j++)
          RTS_SndData(0, Choosefilename + j);
        int filelen = strlen(CardRecbuf.Cardshowfilename[(int)CardRecbuf.recordcount]);
        filelen = (TEXTBYTELEN - filelen) / 2;
        if (filelen > 0)
        {
          char buf[20];
          memset(buf, 0, sizeof(buf));
          strncpy(buf, "         ", filelen);
          strcpy(&buf[filelen], CardRecbuf.Cardshowfilename[(int)CardRecbuf.recordcount]);
          RTS_SndData(buf, Choosefilename);
        }
        else
          RTS_SndData(CardRecbuf.Cardshowfilename[(int)CardRecbuf.recordcount], Choosefilename);

        for (int j = 0; j < 8; j++)
          RTS_SndData(0, FilenameCount + j);
        char buf[20];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d/%d", (int)recdat.data[0], CardRecbuf.Filesum);
        RTS_SndData(buf, FilenameCount);
        delay_ms(2);
        for (int j = 1; j <= CardRecbuf.Filesum; j++)
        {
          RTS_SndData((unsigned long)0xFFFF, FilenameNature + j * 16); // white
          RTS_SndData(10, FilenameIcon1 + j);							 //clean
        }

        RTS_SndData((unsigned long)0x87F0, FilenameNature + recdat.data[0] * 16); // Change BG of selected line to Light Green
        RTS_SndData(6, FilenameIcon1 + recdat.data[0]);							  // show frame
      }
      else if (recdat.addr == FilenamePlay)
      {
        if (recdat.data[0] == 1 && isMediaInserted()) //for sure
        {
          if (CardRecbuf.recordcount < 0)
            break;
          printFile(CardRecbuf.Cardfilename[(int)CardRecbuf.recordcount]);

          for (int j = 0; j < 10; j++) //clean screen.
            RTS_SndData(0, Printfilename + j);

          int filelen = strlen(CardRecbuf.Cardshowfilename[(int)CardRecbuf.recordcount]);
          filelen = (TEXTBYTELEN - filelen) / 2;
          if (filelen > 0)
          {
            char buf[20];
            memset(buf, 0, sizeof(buf));
            strncpy(buf, "         ", filelen);
            strcpy(&buf[filelen], CardRecbuf.Cardshowfilename[(int)CardRecbuf.recordcount]);
            RTS_SndData(buf, Printfilename);
          }
          else
            RTS_SndData(CardRecbuf.Cardshowfilename[(int)CardRecbuf.recordcount], Printfilename);
          delay_ms(2);

          #if FAN_COUNT > 0
            for (uint8_t i = 0; i < FAN_COUNT; i++)
              setTargetFan_percent(FanOn, (fan_t)i);
            #endif
          FanStatus = false;

          RTS_SndData(1 + CEIconGrap, IconPrintstatus); // 1 for Heating
          delay_ms(2);
          RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);

          TPShowStatus = InforShowStatus = true;
          PrintStatue[1] = 0;
          PrinterStatusKey[0] = 1;
          PrinterStatusKey[1] = 3;
        }
        else if (recdat.data[0] == 0) //	return to main page
        {
          InforShowStatus = true;
          TPShowStatus = false;
        }
      }
      break;

    default:
      SERIAL_ECHOLN(PSTR("No Match :"));
      break;
    }

	memset(&recdat, 0, sizeof(recdat));
	recdat.head[0] = FHONE;
	recdat.head[1] = FHTWO;
}

void onPrinterKilled(PGM_P msg) {
  SERIAL_ECHOLN("***kill***");
  //First we send screen available on old versions of software
	rtscheck.RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
  //Then we send the new one Creality added in 1.70.1
	rtscheck.RTS_SndData(ExchangePageBase + 88, ExchangepageAddr);
  delay_ms(3);
  int j = 0;
  char outmsg[40];
  while (j<4)
	{
    outmsg[j] = '*';
    j++;
  }
  while (const char c = pgm_read_byte(msg++)) {
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
	SERIAL_ECHOLN("***Initing card is OK***");
  rtscheck.RTS_SDCardUpate(false, true);
}

void onMediaError()
{
  rtscheck.RTS_SDCardUpate(true, false);
	SERIAL_ECHOLN("***Initing card fails***");
}

void onMediaRemoved()
{
  rtscheck.RTS_SDCardUpate(true, false);
	SERIAL_ECHOLN("***Card Removed***");
}

void onPlayTone(const uint16_t frequency, const uint16_t duration) {

	SERIAL_ECHOLN("***CPlay Tone***");
  rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
}

void onPrintTimerStarted()
{
	SERIAL_ECHOLN("==onPrintTimerStarted==");
  if ( waitway == 7 )
    return;
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (PoweroffContinue)
    {
      injectCommands_P(power_off_commands[0]);
      injectCommands_P(power_off_commands[1]);
      injectCommands_P((PSTR("G28 X0 Y0")));
    }
  #endif
	PrinterStatusKey[1] = 3;
	InforShowStatus = true;
	rtscheck.RTS_SndData(4 + CEIconGrap, IconPrintstatus);
	delay_ms(1);
	rtscheck.RTS_SndData(ExchangePageBase + 53, ExchangepageAddr);
}

void onPrintTimerPaused()
{
	SERIAL_ECHOLN("==onPrintTimerPaused==");
	rtscheck.RTS_SndData(ExchangePageBase + 87, ExchangepageAddr); //Display Pause Screen
}
void onPrintTimerStopped()
{
	SERIAL_ECHOLN("==onPrintTimerStopped==");
  if(waitway == 3)
    return;

#if ENABLED(SDSUPPORT) && ENABLED(POWEROFF_SAVE_SD_FILE)
	card.openPowerOffFile(power_off_info.power_off_filename, O_CREAT | O_WRITE | O_TRUNC | O_SYNC);
	power_off_info.valid_head = 0;
	power_off_info.valid_foot = 0;
	if (card.savePowerOffInfo(&power_off_info, sizeof(power_off_info)) == -1)
	{
		SERIAL_PROTOCOLLN("Stop to Write power off file failed.");
	}
	card.closePowerOffFile();
	power_off_commands_count = 0;
#endif

#if FAN_COUNT > 0
	for (uint8_t i = 0; i < FAN_COUNT; i++)
		setTargetFan_percent(FanOff, (fan_t)i);
#endif
	FanStatus = true;

	PrinterStatusKey[0] = 0;
	power_off_type_yes = 1;
	InforShowStatus = true;
	TPShowStatus = false;
	rtscheck.RTS_SndData(ExchangePageBase + 45, ExchangepageAddr);
}

void onFilamentRunout()
{
	SERIAL_ECHOLN("==onFilamentRunout==");
	PrintStatue[1] = 1; // for returning the corresponding page
	PrinterStatusKey[1] = 4;
	TPShowStatus = false;
  FilementStatus[0] = 2;
  rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
}
void onFilamentRunout(extruder_t extruder)
{
	SERIAL_ECHOLN("==onFilamentRunout==");
  PrintStatue[1] = 1; // for returning the corresponding page
  PrinterStatusKey[1] = 4;
  TPShowStatus = false;
  FilementStatus[0] = 2;
  rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
}
void onUserConfirmRequired(const char *const msg)
{
  PrintStatue[1] = 1; // for returning the corresponding page
  PrinterStatusKey[1] = 4;
  TPShowStatus = false;
  FilementStatus[0] = 2;
  rtscheck.RTS_SndData(ExchangePageBase + 78, ExchangepageAddr);
	SERIAL_ECHOLN("==onUserConfirmRequired==");
}
void onStatusChanged(const char *const msg)
{
  for (int j = 0; j < 40; j++) // Clear old message
    rtscheck.RTS_SndData(' ', 0x20E8+j);
  rtscheck.RTS_SndData(msg, 0x20E8);
}
void onFactoryReset()
{
	SERIAL_ECHOLN("==onFactoryReset==");
}
void onMeshUpdate(const uint8_t xpos, const uint8_t ypos, const float zval)
{
  if(waitway==3)
    if(isPositionKnown() && (getActualTemp_celsius(BED) >= (getTargetTemp_celsius(BED)-1)))
			  rtscheck.RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);

	bool zig = true;
	for (uint8_t yCount = 0, showcount = 0; yCount < GRID_MAX_POINTS_Y; yCount++)
	{
		int8_t inStart, inStop, inInc;
		if (zig)
		{ // away from origin
			inStart = 0;
			inStop = GRID_MAX_POINTS_X;
			inInc = 1;
		}
		else
		{ // towards origin
			inStart = GRID_MAX_POINTS_X - 1;
			inStop = -1;
			inInc = -1;
		}

		zig ^= true; // zag
		for (int8_t xCount = inStart; xCount != inStop; xCount += inInc)
		{
			if ((showcount+1) < (GRID_MAX_POINTS_X * GRID_MAX_POINTS_X))
			{
        if (
        #if ENABLED(ABL_UBL)
          xCount != (GRID_MAX_POINTS_X - 1) && yCount != (GRID_MAX_POINTS_Y - 1)
        #else
          true
        #endif
        ){
          showcount++;
				  rtscheck.RTS_SndData(ExtUI::getMeshPoint(xCount, yCount) * 1000, AutolevelVal + (showcount - 1) * 2);
        }
			}
		}
	}
};

void onStoreSettings(char *buff)
{
	SERIAL_ECHOLN("==onStoreSettings==");
	// This is called when saving to EEPROM (i.e. M500). If the ExtUI needs
	// permanent data to be stored, it can write up to eeprom_data_size bytes
	// into buff.

	// Example:
	//  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
	//  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
}

void onLoadSettings(const char *buff)
{
	SERIAL_ECHOLN("==onLoadSettings==");
	// This is called while loading settings from EEPROM. If the ExtUI
	// needs to retrieve data, it should copy up to eeprom_data_size bytes
	// from buff

	// Example:
	//  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
	//  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
}

void onConfigurationStoreWritten(bool success)
{
	SERIAL_ECHOLN("==onConfigurationStoreWritten==");
	// This is called after the entire EEPROM has been written,
	// whether successful or not.
}

void onConfigurationStoreRead(bool success)
{
	SERIAL_ECHOLN("==onConfigurationStoreRead==");
  #if HAS_MESH && (ANY(MachineCR10SPro, MachineEnder5Plus, MachineCR10Max) || ENABLED(Force10SProDisplay))
    if (ExtUI::getMeshValid())
    {
      bool zig = true;
      for (uint8_t yCount = 0, showcount = 0; yCount < GRID_MAX_POINTS_Y; yCount++)
      {
        int8_t inStart, inStop, inInc;

        if (zig)
        { // away from origin
          inStart = 0;
          inStop = GRID_MAX_POINTS_X;
          inInc = 1;
        }
        else
        { // towards origin
          inStart = GRID_MAX_POINTS_X - 1;
          inStop = -1;
          inInc = -1;
        }

        zig ^= true; // zag
        for (int8_t xCount = inStart; xCount != inStop; xCount += inInc)
        {
          if ((showcount+1) < (GRID_MAX_POINTS_X * GRID_MAX_POINTS_X))
          {
            if (
            #if ENABLED(ABL_UBL)
              xCount != (GRID_MAX_POINTS_X - 1) && yCount != (GRID_MAX_POINTS_Y - 1)
            #else
              true
            #endif
            ){
              showcount++;
              rtscheck.RTS_SndData(ExtUI::getMeshPoint(xCount, yCount) * 1000, AutolevelVal + (showcount - 1) * 2);
            }
          }
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

	SERIAL_ECHOLNPAIR("\n init zprobe_zoffset = ", getZOffset_mm());
	rtscheck.RTS_SndData(getZOffset_mm() * 100, 0x1026);
}

} // namespace ExtUI

#endif