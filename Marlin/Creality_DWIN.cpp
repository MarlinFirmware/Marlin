#include "Creality_DWIN.h"
#include <HardwareSerial.h>
#include <arduino.h>
#include <wstring.h>
#include <stdio.h>
#include "MarlinConfig.h"

#include "types.h"
#include "Marlin.h"
#include "language.h"
#include "cardreader.h"
#include "temperature.h"
#include "planner.h"
#include "printcounter.h"
#include "stepper.h"
#include "configuration_store.h"

#include "duration_t.h"

#if ENABLED(BLTOUCH)
  #include "endstops.h"
#endif

const float manual_feedrate_mm_m[] = MANUAL_FEEDRATE;
uint8_t progress_bar_percent;
int startprogress = 0;
CRec CardRecbuf;
int temphot=0;
//int tempbed=0;
//float pause_z = 0;
#if DISABLED(POWER_LOSS_RECOVERY)
	int power_off_type_yes = 0;
	int power_off_commands_count = 0;
#endif

float PLA_ABSModeTemp = 195;
millis_t next_rts_update_ms = 0;
int	last_target_temperature_bed;
int	last_target_temperature[4] = {0};
char waitway = 0;
int recnum = 0;
unsigned char original_extruder = 0;
unsigned char Percentrecord = 0;
float	ChangeMaterialbuf[2] = {0};

char NozzleTempStatus[3] = {0};


bool PrintMode = true;

char PrintStatue[2] = {0};	//PrintStatue[0], 0 represent  to 43 page, 1 represent to 44 page
bool CardUpdate = false;	//represents to update file list
char CardCheckStatus[2] = {0};	//CardCheckStatus[0] represents to check card in printing and after making sure to begin and to check card in heating with value as 1, but 0 for off
unsigned char LanguageRecbuf; // !0 represent Chinese, 0 represent English
char PrinterStatusKey[2] = {0};	// PrinterStatusKey[1] value: 0 represents to keep temperature, 1 represents  to heating , 2 stands for cooling , 3 stands for printing
							// PrinterStatusKey[0] value: 0 reprensents 3D printer ready
extern CardReader card;
bool lcd_sd_status;//represents SD-card status, true means SD is available, false means opposite.

char Checkfilenum=0;
char FilenamesCount = 0;
char cmdbuf[20] = {0};
char FilementStatus[2] = {0};

unsigned char	AxisPagenum = 0;	//0 for 10mm, 1 for 1mm, 2 for 0.1mm
bool InforShowStatus = true;
bool TPShowStatus = false;	// true for only opening time and percentage, false for closing time and percentage.
bool FanStatus = true;
bool AutoLevelStatus = true;
bool AutohomeKey = false;
unsigned char AutoHomeIconNum;
RTSSHOW rtscheck;
int Update_Time_Value = 0;
unsigned long VolumeSet = 0x80;
extern char power_off_commands[9][96];
bool PoweroffContinue = false;
extern const char *injected_commands_P;
char commandbuf[30];

inline void RTS_line_to_current(AxisEnum axis) {
    if (!planner.is_full()) 
        planner.buffer_line_kinematic(current_position, MMM_TO_MMS(manual_feedrate_mm_m[(int8_t)axis]), active_extruder);
}

RTSSHOW::RTSSHOW(){
  recdat.head[0] = snddat.head[0] = FHONE;
  recdat.head[1] = snddat.head[1] = FHTWO;
  memset(databuf,0, sizeof(databuf));
}

void RTSSHOW::RTS_SDCardInit(void)
{
	if(!card.cardOK)
		card.initsd();
	delay(2);
	if (card.cardOK)
	{
		SERIAL_ECHOLN("***Initing card is OK***");
		uint16_t fileCnt = card.getnrfilenames();
		card.getWorkDirName();
		if (card.filename[0] == '/') {
			card.initsd();
		}
		else 
		{
			card.updir();
		}

		int addrnum =0;
		int num = 0;
		for (uint16_t i = 0; i < fileCnt && i < MaxFileNumber + addrnum; i++) 
		{
			card.getfilename(fileCnt-1-i);
			char *pointFilename = card.longFilename;
			int filenamelen = strlen(card.longFilename);
			int j = 1;
			while((strncmp(&pointFilename[j],".gcode",6) && strncmp(&pointFilename[j],".GCODE",6)) && (j++) < filenamelen);
			if(j >= filenamelen)
			{
				addrnum++;
				continue;
			}
			
			if(j >= TEXTBYTELEN)	
			{
				strncpy(&card.longFilename[TEXTBYTELEN -3],"~~",2);
				card.longFilename[TEXTBYTELEN-1] = '\0';
				j = TEXTBYTELEN-1;
			}
			
			delay(3);
			strncpy(CardRecbuf.Cardshowfilename[num], card.longFilename,j);

			strcpy(CardRecbuf.Cardfilename[num],card.filename);
			CardRecbuf.addr[num] = SDFILE_ADDR +num*10;
			RTS_SndData(CardRecbuf.Cardshowfilename[num],CardRecbuf.addr[num]);
			CardRecbuf.Filesum = (++num);
			//SERIAL_ECHO("  CardRecbuf.Filesum ==");
			//SERIAL_ECHO(CardRecbuf.Filesum);
			RTS_SndData(1,FilenameIcon+CardRecbuf.Filesum);
		}
		if(LanguageRecbuf != 0)
			RTS_SndData(0,IconPrintstatus);	// 0 for Ready
		else
			RTS_SndData(0+CEIconGrap,IconPrintstatus);

		lcd_sd_status = IS_SD_INSERTED; 
		
	}
	else
	{
		SERIAL_ECHOLN("***Initing card fails***");
		if(LanguageRecbuf != 0)
			RTS_SndData(6,IconPrintstatus);	// 6 for Card Removed
		else
			RTS_SndData(6+CEIconGrap,IconPrintstatus);
	}
}

void RTSSHOW::RTS_SDCardUpate(void)
{	
	//SERIAL_ECHO("\n SDUpdate ");
	const bool sd_status = IS_SD_INSERTED;
    	if (sd_status != lcd_sd_status)
	{
		//SERIAL_PROTOCOLLN("  ***test6*** ");
		if (sd_status)	//sd_status = true
		{
			card.initsd();
    			#if ENABLED(POWER_LOSS_RECOVERY)
					init_power_off_info();
				#endif
			RTS_SDCardInit();
		}
		else
		{
			card.release();
			if( CardCheckStatus[0] == 1)// heating or printing
			{
				RTS_SDcard_Stop();
				CardCheckStatus[0] = 0;	//cancel to check card during printing the gcode file 
			}

			if(LanguageRecbuf != 0)
				RTS_SndData(6,IconPrintstatus);	// 6 for Card Removed
			else
				RTS_SndData(6+CEIconGrap,IconPrintstatus);
			for(int i = 0;i < CardRecbuf.Filesum;i++)
			{
				for(int j = 0;j < 10;j++)
					RTS_SndData(0,CardRecbuf.addr[i]+j);
				//RTS_SndData(4,FilenameIcon+1+i);
				RTS_SndData((unsigned long)0xFFFF,FilenameNature + (i+1)*16);		// white
			}
			
			for(int j = 0;j < 10;j++)	
			{
				RTS_SndData(0,Printfilename+j); //clean screen.
				RTS_SndData(0,Choosefilename+j); //clean filename
			}
			for(int j = 0;j < 8;j++)
				RTS_SndData(0,FilenameCount+j);
			for(int j = 1;j <= 20;j++)	//clean filename Icon
			{
				RTS_SndData(10,FilenameIcon+j);
				RTS_SndData(10,FilenameIcon1+j);
			}
			memset(&CardRecbuf,0,sizeof(CardRecbuf));
		}

		lcd_sd_status = sd_status;

	}
			
	//SERIAL_ECHOPAIR("\n ***CardUpdate = ",CardUpdate);
	//SERIAL_ECHOPAIR("\n ***lcd_sd_status = ",lcd_sd_status);
	//SERIAL_ECHOPAIR("\n ***card.cardOK = ",card.cardOK);
	if (CardUpdate && lcd_sd_status &&  card.cardOK) // represents to update file list
	{
		//SERIAL_PROTOCOLLN("  ***test7*** ");
		for(int j = 0;j < 10;j++)	//clean filename
			RTS_SndData(0,Choosefilename+j);
		for(int j = 0;j < 8;j++)
			RTS_SndData(0,FilenameCount+j);
		for (uint16_t i = 0; i < CardRecbuf.Filesum ; i++) 
		{
			delay(3);
			RTS_SndData(CardRecbuf.Cardshowfilename[i],CardRecbuf.addr[i]);
			RTS_SndData(1,FilenameIcon+1+i);
			RTS_SndData((unsigned long)0xFFFF,FilenameNature + (i+1)*16);		// white
			RTS_SndData(10,FilenameIcon1+1+i);
		}
		CardUpdate = false;
		SERIAL_ECHO("====end====");
	}
}

#if ENABLED(MachineCR10SPro) || ENABLED(AddonFilSensor)

	int RTSSHOW::RTS_CheckFilement(int mode)
	{
		SERIAL_ECHO("   ****RTS_CheckFilement***   ");
		waitway = 4;
		for(Checkfilenum= 0; 0==READ(FIL_RUNOUT_PIN) && Checkfilenum < 50;Checkfilenum++)// no filements check
			delay(15);
		
		if(49 <= Checkfilenum ) //no filements
		{
			SERIAL_ECHO("\n Check Filament Setting Screen ");
			if(mode)
			{
				FilementStatus[0] = mode; // for mode status of no filement . the sentence can be canceled, which isn't neccessary?
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 38, ExchangepageAddr); //exchange to 38 page
				else
					RTS_SndData(ExchangePageBase + 78, ExchangepageAddr); 
			}
			Checkfilenum = 0;
			waitway = 0;
			return 1;
		}
		else
			Checkfilenum = 0;
		
		waitway = 0;
		return 0;
		
	}
#endif
void RTSSHOW::RTS_Init()
{
	Serial2.begin(115200);

	//LanguageRecbuf =eeprom_read_byte((unsigned char*)FONT_EEPROM);
	//AutoLevelStatus = eeprom_read_byte((unsigned char*)FONT_EEPROM+2);
	LanguageRecbuf = 0;
	AutoLevelStatus = 1;
	int showcount = 0;
	
	#if HAS_MESH && (ENABLED(MachineCR10SPro) || ENABLED(Force10SProDisplay))
		if (leveling_is_valid())
		{
			for(int xCount  = 0; xCount < GRID_MAX_POINTS_X; xCount++)
			{
				for(int yCount  = 0; yCount < GRID_MAX_POINTS_X; yCount++)
				{
					if((showcount++) < 16)
					{
						rtscheck.RTS_SndData(z_values[xCount][yCount] *10000, AutolevelVal + (showcount-1)*2);
						rtscheck.RTS_SndData(showcount,AutolevelIcon);
					}
				}
			}
			RTS_SndData(2, AutoLevelIcon);//2=On, 3=Off
			enqueue_and_echo_commands_P((PSTR("M420 S1")));
			AutoLevelStatus = planner.leveling_active;
		}
		else 
		{
			RTS_SndData(3, AutoLevelIcon);/*Off*/
			//settings.load();
		}
	#endif
	
	//VolumeSet = eeprom_read_byte((unsigned char*)FONT_EEPROM+4);
	//if(VolumeSet < 0 || VolumeSet > 0xFF)
		VolumeSet = 0x20;
		
	if(PrintMode)RTS_SndData(3, FanKeyIcon+1);	// saving mode
	else RTS_SndData(2, FanKeyIcon+1);	// normal
	last_target_temperature_bed = thermalManager.target_temperature_bed;
	last_target_temperature[0] =  thermalManager.target_temperature[0];
	feedrate_percentage = 100;
	RTS_SndData(feedrate_percentage,FeedrateDisplay);
	
	/***************turn off motor*****************/
	RTS_SndData(11, FilenameIcon); 
	
	/***************transmit temperature to screen*****************/
	RTS_SndData(0, NozzlePreheat);
	RTS_SndData(0, BedPreheat);
	RTS_SndData(thermalManager.current_temperature[0], NozzleTemp);
	RTS_SndData(thermalManager.current_temperature_bed, Bedtemp);
	/***************transmit Fan speed to screen*****************/
	#if FAN_COUNT > 0
		for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = FanOff;//turn 0ff fans
	#endif
	RTS_SndData(2, FanKeyIcon);	//turn 0ff fans
	FanStatus = true;
	
	/*********transmit SD card filename to screen***************/
	RTS_SDCardInit();
	/***************transmit Printer information to screen*****************/
	for(int j = 0;j < 20;j++)	//clean filename
		RTS_SndData(0,MacVersion+j);
	char sizebuf[20]={0};
	sprintf(sizebuf,"%d X %d X %d",Y_BED_SIZE, X_BED_SIZE, Z_MAX_POS);
	RTS_SndData(CUSTOM_MACHINE_NAME, MacVersion);
	RTS_SndData(DETAILED_BUILD_VERSION, SoftVersion);
	RTS_SndData(sizebuf, PrinterSize);
	if(LanguageRecbuf != 0)
		RTS_SndData(WEBSITE_URL, CorpWebsite);
	else
		RTS_SndData(WEBSITE_URL, CorpWebsite);

	/**************************some info init*******************************/
	RTS_SndData(0,PrintscheduleIcon);
	RTS_SndData(0,PrintscheduleIcon+1);

	/************************clean screen*******************************/
	for(int i = 0;i < MaxFileNumber;i++)
	{
		for(int j = 0;j < 10;j++)
			RTS_SndData(0,SDFILE_ADDR +i*10+j);
	}
	
	for(int j = 0;j < 10;j++)	
	{
		RTS_SndData(0,Printfilename+j); //clean screen.
		RTS_SndData(0,Choosefilename+j); //clean filename
	}
	for(int j = 0;j < 8;j++)
		RTS_SndData(0,FilenameCount+j);
	for(int j = 1;j <= MaxFileNumber;j++)
	{
		RTS_SndData(10,FilenameIcon+j);
		RTS_SndData(10,FilenameIcon1+j);
	}
	
	rts_probe_zoffset = zprobe_zoffset;
		//SERIAL_ECHOPAIR("\n init rts_probe_zoffset = ",rts_probe_zoffset);
		SERIAL_ECHOPAIR("\n init zprobe_zoffset = ",zprobe_zoffset);
	RTS_SndData(zprobe_zoffset*100, 0x1026);  
	/************************EEPROM*******************************/
	//settings.load();
	
	SERIAL_ECHOLN("===Initing RTS has finished===");

}

int RTSSHOW::RTS_RecData()
{
  while(Serial2.available() > 0 && (recnum < SizeofDatabuf))
  {
    databuf[recnum] = Serial2.read();
    if(databuf[0] != FHONE)    //ignore the invalid data
    {
    	 if(recnum > 0) // prevent the program from running.
    	 {
	    	 memset(databuf,0,sizeof(databuf));
		 recnum = 0;
    	 }
        continue;
    }
    delay(10);
    recnum++;
  }

  if(recnum < 1)    //receive nothing  	
    return -1;
  else  if((recdat.head[0] == databuf[0]) && (recdat.head[1] == databuf[1]) && recnum > 2)
  {
  	//  SERIAL_ECHOLN(" *** RTS_RecData1*** ");

    	recdat.len = databuf[2];
    	recdat.command = databuf[3];
	if(recdat.len == 0x03 && (recdat.command == 0x82 || recdat.command == 0x80) && (databuf[4] == 0x4F) && (databuf[5] == 0x4B))  //response for writing byte
	{   
            memset(databuf,0, sizeof(databuf));
            recnum = 0;
	     //SERIAL_ECHOLN(" *** RTS_RecData1*** ");
            return -1;
	}
	else if(recdat.command == 0x83) //response for reading the data from the variate
	{
            recdat.addr = databuf[4];
            recdat.addr = (recdat.addr << 8 ) | databuf[5];
            recdat.bytelen = databuf[6];
	     for(int i = 0;i < recdat.bytelen;i+=2)
	     {
			recdat.data[i/2]= databuf[7+i];
			recdat.data[i/2]= (recdat.data[i/2] << 8 )| databuf[8+i];
	     }
	}
	else if(recdat.command == 0x81)  //response for reading the page from the register
	{
            recdat.addr = databuf[4];
            recdat.bytelen = databuf[5];
	     for(int i = 0;i < recdat.bytelen;i++)
	     {
			recdat.data[i]= databuf[6+i];
	            //recdat.data[i]= (recdat.data[i] << 8 )| databuf[7+i];
	     }
       }
  
  }
  else
  {
	memset(databuf,0, sizeof(databuf));
	recnum = 0;
       return -1;  //receive the wrong data
  }
    memset(databuf,0, sizeof(databuf));
    recnum = 0;
    return 2;
}

void RTSSHOW::RTS_SndData(void)
{
    if((snddat.head[0] == FHONE) && (snddat.head[1] == FHTWO) && snddat.len >= 3){
          databuf[0] = snddat.head[0];
          databuf[1] = snddat.head[1];
          databuf[2] = snddat.len;
          databuf[3] = snddat.command;
          if(snddat.command ==0x80)    //to write data to the register
          {
		databuf[4] = snddat.addr;
		for(int i =0;i <(snddat.len - 2);i++)
			databuf[5+i] = snddat.data[i];
          }
          else if(snddat.len == 3 && (snddat.command ==0x81))   //to read data from the register
          {
                databuf[4] = snddat.addr;
                databuf[5] = snddat.bytelen;
          }
          else if(snddat.command ==0x82)   //to write data to the variate
          {
                databuf[4] = snddat.addr >> 8;
                databuf[5] = snddat.addr & 0xFF;
		   for(int i =0;i <(snddat.len - 3);i += 2)
		   {
			databuf[6 + i] = snddat.data[i/2] >> 8;
			databuf[7 + i] = snddat.data[i/2] & 0xFF;
		   }
          }
          else if(snddat.len == 4 && (snddat.command ==0x83))   //to read data from the variate
          {
                databuf[4] = snddat.addr >> 8;
                databuf[5] = snddat.addr & 0xFF;
                databuf[6] = snddat.bytelen;
          }
            for(int i = 0;i < (snddat.len + 3);i++)
            {
                Serial2.write(databuf[i]);
                delayMicroseconds(1);
            }
			
          memset(&snddat,0,sizeof(snddat));
          memset(databuf,0, sizeof(databuf));
          snddat.head[0] = FHONE;
          snddat.head[1] = FHTWO;
    }
}


void RTSSHOW::RTS_SndData(const String &s, unsigned long addr, unsigned char cmd /*= VarAddr_W*/)
{
	if(s.length() < 1)
		return;
	RTS_SndData(s.c_str(), addr, cmd);
}

void RTSSHOW::RTS_SndData(const char *str, unsigned long addr, unsigned char cmd/*= VarAddr_W*/)
{

	int len = strlen(str);
	
	if( len > 0)
	{
		databuf[0] = FHONE;
		databuf[1] = FHTWO;
		databuf[2] = 3+len;
		databuf[3] = cmd;
		databuf[4] = addr >> 8;
		databuf[5] = addr & 0x00FF;
		for(int i =0;i <len ;i++)
			databuf[6 + i] = str[i];

		for(int i = 0;i < (len + 6);i++)
		{
		    Serial2.write(databuf[i]);
		    delayMicroseconds(1);
		}
	      memset(databuf,0, sizeof(databuf));
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
	waitway = 4;
	if(card.sdprinting) {
		card.stopSDPrint();
		clear_command_queue();
		quickstop_stepper();
	}
	SERIAL_ECHO("\n   stopping ==\n");
	SERIAL_ECHOLNPGM("//action:cancel");
	thermalManager.disable_all_heaters();
	print_job_timer.reset();
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
		for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = FanOff;
	#endif
	FanStatus = true;
	
	wait_for_heatup = false;
	PrinterStatusKey[0] = 0;
	enqueue_and_echo_commands_P(PSTR("M84"));	//shut down the stepper motor.
	SERIAL_ECHOLNPGM("//action:cancel");
}


void RTSSHOW::RTS_HandleData()
{
	int Checkkey = -1;
	SERIAL_PROTOCOLLN("  *******RTS_HandleData********\n ");
	if(waitway > 0)	//for waiting
	{
		SERIAL_ECHO("\n   waitway ==");
		SERIAL_ECHO((int)waitway);
		memset(&recdat,0 , sizeof(recdat));
		recdat.head[0] = FHONE;
		recdat.head[1] = FHTWO;
		return;
	}
	SERIAL_ECHO("\n   recdat.data[0] ==");
	SERIAL_ECHO(recdat.data[0]);
	SERIAL_ECHO("\n   recdat.addr ==");
	SERIAL_ECHO(recdat.addr);
    for(int i = 0;Addrbuf[i] != 0;i++)
    {
	  if(recdat.addr == Addrbuf[i])
	  {
	  	if(Addrbuf[i] >= Stopprint && Addrbuf[i] <= Resumeprint)
			Checkkey = PrintChoice;
		else if(Addrbuf[i] == NzBdSet || Addrbuf[i] == NozzlePreheat || Addrbuf[i] == BedPreheat )
			Checkkey = ManualSetTemp;
		else if(Addrbuf[i] >= AutoZero && Addrbuf[i] <= DisplayZaxis)
			Checkkey = XYZEaxis;
		else if(Addrbuf[i] >= FilementUnit1 && Addrbuf[i] <= FilementUnit2)
			Checkkey = Filement;
		else
			Checkkey = i;
		break;
	  }
    }
	
    if(recdat.addr >= SDFILE_ADDR && recdat.addr <= (SDFILE_ADDR + 10 *(FileNum+1)))
	   Checkkey = Filename;
  
    if(Checkkey < 0)
    {
    	memset(&recdat,0 , sizeof(recdat));
	recdat.head[0] = FHONE;
	recdat.head[1] = FHTWO;
	return;
    }
SERIAL_ECHO("== Checkkey==");
SERIAL_ECHO(Checkkey);
    switch(Checkkey)
    {
	case Printfile :
		if(recdat.data[0] == 1)		// card
		{
			InforShowStatus = false;
			CardUpdate = true;
			CardRecbuf.recordcount = -1;
			RTS_SDCardUpate();
			SERIAL_ECHO("\n Handle Data PrintFile 1 Setting Screen ");
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 2, ExchangepageAddr); //exchange to 2 page
			else
				RTS_SndData(ExchangePageBase + 46, ExchangepageAddr); 

		}
		else if(recdat.data[0] == 2)	// return after printing result.
		{
			InforShowStatus = true;
		      TPShowStatus = false;
			quickstop_stepper();
			RTS_SndData(11, FilenameIcon); 
			RTS_SndData(0,PrintscheduleIcon);
			RTS_SndData(0,PrintscheduleIcon+1);
			RTS_SndData(0,Percentage);
			delay(2);
			RTS_SndData(0,Timehour);
			RTS_SndData(0,Timemin);
			print_job_timer.reset();
			
			SERIAL_ECHO("\n Handle Data PrintFile 2 Setting Screen ");
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 1, ExchangepageAddr); //exchange to 1 page
			else
				RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); //exchange to 45 page
		} 
		else if(recdat.data[0] == 3 )	// Temperature control
		{
			InforShowStatus = true;
		      TPShowStatus = false;
			  SERIAL_ECHO("\n Handle Data PrintFile 3 Setting Screen ");
			if(LanguageRecbuf != 0)
			{
				if(FanStatus)
					RTS_SndData(ExchangePageBase + 16, ExchangepageAddr); //exchange to 16 page, the fans off
				else
					RTS_SndData(ExchangePageBase + 15, ExchangepageAddr); //exchange to 15 page, the fans on
			}
			else
			{
				if(FanStatus)
					RTS_SndData(ExchangePageBase + 58, ExchangepageAddr); //exchange to 58 page, the fans off
				else
					RTS_SndData(ExchangePageBase + 57, ExchangepageAddr); //exchange to 57 page, the fans on
			}
		}
		else if(recdat.data[0] == 4 )	//Settings
			InforShowStatus = false;
		break;
		
	case Ajust :
		if(recdat.data[0] == 1)
		{
			InforShowStatus = false;
			FanStatus?RTS_SndData(2, FanKeyIcon):RTS_SndData(3, FanKeyIcon);
		}
		else if(recdat.data[0] == 2)
		{
			SERIAL_ECHO("\n Handle Data Adjust 2 Setting Screen ");
			InforShowStatus = true;
			if(PrinterStatusKey[1] == 3)// during heating
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);	// in heating
				else
					RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 
			}
			else if(PrinterStatusKey[1] == 4)
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);	// in the pause
				else
					RTS_SndData(ExchangePageBase + 54, ExchangepageAddr); 
			}
			else
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);	// in printing
				else
					RTS_SndData(ExchangePageBase + 53, ExchangepageAddr); 

			}
		}
		else if(recdat.data[0] == 3)
		{
			if(FanStatus)	//turn on the fan
			{
				RTS_SndData(3, FanKeyIcon); 
				fanSpeeds[0] = FanOn;
				FanStatus = false;
			}
			else//turn off the fan
			{
				RTS_SndData(2, FanKeyIcon); 
				fanSpeeds[0] = FanOff;
				FanStatus = true;
			}
		}
		else if(recdat.data[0] == 4)
		{
			if(PrintMode)	// normal printing mode
			{
				RTS_SndData(2, FanKeyIcon+1); 
				PrintMode = false;
			}
			else // power saving mode
			{
				RTS_SndData(3, FanKeyIcon+1); 
  				PrintMode = true;
			}

		}
		break;
		
	case Feedrate :
		feedrate_percentage = recdat.data[0];
		break;

	case PrintChoice:
		if(recdat.addr == Stopprint)
		{ 
			if(PrintStatue[1] == 1 && recdat.data[0] == 0xF0)	// in the pause
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 12, ExchangepageAddr); //exchange to 12 page
				else
					RTS_SndData(ExchangePageBase + 54, ExchangepageAddr); 
				break;
			}
			else if(PrintStatue[1] == 0 && recdat.data[0] == 0xF0)	
			{
				if(PrinterStatusKey[1] == 3)// during heating
				{
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);	// in heating
					else
						RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 
				}
				else
				{
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);	// in printing
					else
						RTS_SndData(ExchangePageBase + 53, ExchangepageAddr); 
				}
				break;
			}
			//FilementStatus[0]  =  0; // recover the status waiting to check filements
			

			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 86, ExchangepageAddr);	
			else
				RTS_SndData(ExchangePageBase + 87, ExchangepageAddr); 
			RTS_SndData(0,Timehour);		
			RTS_SndData(0,Timemin);	
			Update_Time_Value = 0;
			CardCheckStatus[0] = 0;// close the key of  checking card in  printing
			RTS_SDcard_Stop();
		}
		else if(recdat.addr == Pauseprint)
		{				
			if(recdat.data[0] != 0xF1)
				break;

			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 86, ExchangepageAddr);	
			else
				RTS_SndData(ExchangePageBase + 87, ExchangepageAddr); 
			
			//char pause_str_Z[16];
			waitway = 1;		//reject to receive cmd
			//current_position[Z_AXIS] += 5;
			//pause_z = current_position[Z_AXIS];
 			card.pauseSDPrint();
			print_job_timer.pause();
			#ifdef ACTION_ON_PAUSE
			  SERIAL_ECHOLNPGM("From Pauseprint\n");
    		  SERIAL_ECHOLNPGM("//action:" ACTION_ON_PAUSE);
    		#endif
 			temphot=thermalManager.degTargetHotend(0); //thermalManager.target_temperature[0];
 			//tempbed=thermalManager.degTargetBed();//thermalManager.target_temperature_bed;
			//thermalManager.setTargetHotend(0, 0);
			//thermalManager.setTargetBed(0);

 			PrintStatue[1] = 1;	//for return the corresponding page
 			PrinterStatusKey[1] = 4;

			

			Update_Time_Value = 0;
			//memset(commandbuf,0,sizeof(commandbuf));
			//dtostrf(pause_z, 3, 2, pause_str_Z);
			//sprintf(commandbuf,"G0 X10 Y10  Z%s",pause_str_Z);
   			//strncpy_P(commandbuf, PSTR("G28 X0 Y0"), sizeof(commandbuf) - 1);
			//injected_commands_P =commandbuf;// PSTR("G28 X0 Y0");//commandbuf;
			//enqueue_and_echo_commands_P(PSTR("G28 X0 Y0"));

		      // Wait for planner moves to finish!
		       planner.synchronize();
			enqueue_and_echo_commands_P(PSTR("M25"));
			//enqueue_and_echo_commands_P(PSTR("G1 Z +5"));
			//enqueue_and_echo_commands_P(PSTR("G0 X10 Y10 F3000"));

		}
		else if(recdat.addr == Resumeprint && recdat.data[0] == 1)
		{				
			#if ENABLED(MachineCR10SPro) || ENABLED(AddonFilSensor)
			/**************checking filement status during printing************/
			if(RTS_CheckFilement(0)) 
			{
				for(startprogress=0;startprogress < 5;startprogress++)
				{
					RTS_SndData(startprogress, ExchFlmntIcon);
					delay(400);
				}
				break;
			}
			#endif
			
			//char pause_str_Z[16];
			//memset(pause_str_Z, 0, sizeof(pause_str_Z));
			//dtostrf(pause_z-5, 3, 2, pause_str_Z);
			//memset(commandbuf,0,sizeof(commandbuf));
			enqueue_and_echo_commands_P(PSTR("M24"));
			//sprintf_P(commandbuf, PSTR("M190 S%i"), tempbed);
			//enqueue_and_echo_command(commandbuf);
			memset(commandbuf,0,sizeof(commandbuf));
			sprintf_P(commandbuf, PSTR("M109 S%i"), temphot);
			enqueue_and_echo_command(commandbuf);
			//enqueue_and_echo_commands_P(PSTR("G28 X0 Y0"));
			planner.synchronize();
			//memset(commandbuf,0,sizeof(commandbuf));
			//sprintf_P(commandbuf, PSTR("G0 Z%s"), pause_str_Z);
		   	//enqueue_and_echo_command(commandbuf);
			
			//card.startFileprint();
			//print_job_timer.start();
			#ifdef ACTION_ON_RESUME
      			SERIAL_ECHOLNPGM("//action:" ACTION_ON_RESUME);
    		#endif

			#if ENABLED(FILAMENT_RUNOUT_SENSOR)
      			runout.reset();
    		#endif
			FilementStatus[1] = 2;
			
			PrinterStatusKey[1] = 0;
			InforShowStatus = true;
			Update_Time_Value = RTS_UPDATE_VALUE;

			if(LanguageRecbuf != 0)
				RTS_SndData(0,IconPrintstatus);	// 1 for Heating
			else
				RTS_SndData(0+CEIconGrap,IconPrintstatus);
			PrintStatue[1] = 0;
			Update_Time_Value = 0;
			//PrinterStatusKey[1] = 3;
			CardCheckStatus[0] = 1;	// open the key of  checking card in  printing
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 10, ExchangepageAddr); //exchange to 10 page
			else
				RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 
		}
		if(recdat.addr == Resumeprint && recdat.data[0] == 2)	// warming
		{ 
			NozzleTempStatus[2] = 1;
			PrinterStatusKey[1] = 0;
			InforShowStatus = true;
			Update_Time_Value = RTS_UPDATE_VALUE;
			thermalManager.target_temperature[0] = temphot;
			startprogress  = 0;
			FilementStatus[1] = 2;
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 7, ExchangepageAddr); //exchange to 12 page
			else
				RTS_SndData(ExchangePageBase + 82, ExchangepageAddr); 
		}
		break;
		
	case Zoffset:
		//SERIAL_ECHOPAIR("\n rts_probe_zoffset = ",rts_probe_zoffset);
		//SERIAL_ECHOPAIR("\n rcv data = ",recdat.data[0]);
		if(recdat.data[0]>= 32768) {
			rts_probe_zoffset = ((float)recdat.data[0]-65536)/100;
		}
		else {
			rts_probe_zoffset = ((float)recdat.data[0])/100;
		}
		
		//SERIAL_ECHOPAIR("\n rts_probe_zoffset = ",rts_probe_zoffset);
		//SERIAL_ECHOPAIR("\n target = ",(zprobe_zoffset - rts_probe_zoffset));
		//SERIAL_ECHOPAIR("\n target axis = ",Z_AXIS);
		//SERIAL_ECHOPAIR("\n steps mm = ",planner.steps_to_mm[Z_AXIS]);
        if (WITHIN((rts_probe_zoffset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        	thermalManager.babystep_axis(Z_AXIS, (400 * (zprobe_zoffset - rts_probe_zoffset) * -1));
        	zprobe_zoffset = rts_probe_zoffset;
			//SERIAL_ECHOPAIR("\n StepsMoved = ",(400 * (zprobe_zoffset - rts_probe_zoffset) * -1));
			//SERIAL_ECHOPAIR("\n probe_zoffset = ",zprobe_zoffset);
			RTS_SndData(zprobe_zoffset*100, 0x1026);  
		}
		//SERIAL_ECHOPAIR("\n rts_probe_zoffset = ",rts_probe_zoffset);
		settings.save();
		//SERIAL_ECHOPAIR("\n probe_zoffset = ",zprobe_zoffset);
		break;
		
	case TempControl:
		if(recdat.data[0] == 0)
		{
			InforShowStatus = true;
			TPShowStatus = false;
		}
		else if(recdat.data[0] == 1)
		{
			/*if(LanguageRecbuf != 0)
			{
				if(FanStatus)
					RTS_SndData(ExchangePageBase + 18, ExchangepageAddr); //exchange to 18 page, the fans off
				else
					RTS_SndData(ExchangePageBase + 17, ExchangepageAddr); //exchange to 17 page, the fans on
			}
			else
			{
				if(FanStatus)
					RTS_SndData(ExchangePageBase + 60, ExchangepageAddr); //exchange to 60 page, the fans off
				else
					RTS_SndData(ExchangePageBase + 59, ExchangepageAddr); //exchange to 59 page, the fans on
			} */
		}
		else if(recdat.data[0] == 2)
		{
			//InforShowStatus = true;
		}
		else if(recdat.data[0] == 3)
		{
			if(FanStatus)	//turn on the fan
			{
				fanSpeeds[0] = FanOn;
				FanStatus = false;
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 15, ExchangepageAddr); //exchange to 15 page, the fans on
				else
					RTS_SndData(ExchangePageBase + 57, ExchangepageAddr); //exchange to 57 page, the fans on
			}
			else//turn off the fan
			{
				fanSpeeds[0] = FanOff;
				FanStatus = true;
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 16, ExchangepageAddr); //exchange to 16 page, the fans off
				else
					RTS_SndData(ExchangePageBase + 58, ExchangepageAddr); //exchange to 58 page, the fans on
			}
		}
		else if(recdat.data[0] == 5)	//PLA mode
		{
			thermalManager.setTargetHotend((PLA_ABSModeTemp = PREHEAT_1_TEMP_HOTEND), 0);
			thermalManager.setTargetBed(PREHEAT_1_TEMP_BED);

			RTS_SndData(PREHEAT_1_TEMP_HOTEND,NozzlePreheat);
			RTS_SndData(PREHEAT_1_TEMP_BED,BedPreheat);

		}
		else if(recdat.data[0] == 6)	//ABS mode
		{
			thermalManager.setTargetHotend((PLA_ABSModeTemp = PREHEAT_2_TEMP_HOTEND), 0);
			thermalManager.setTargetBed(PREHEAT_2_TEMP_BED);
			
			RTS_SndData(PREHEAT_2_TEMP_HOTEND,NozzlePreheat);
			RTS_SndData(PREHEAT_2_TEMP_BED,BedPreheat);
		}
		else if(recdat.data[0] == 0xF1)
		{
			//InforShowStatus = true;
			#if FAN_COUNT > 0
			for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = FanOn;
			#endif
			FanStatus = false;
			thermalManager.disable_all_heaters();

			RTS_SndData(0,NozzlePreheat);
			delay(1);
			RTS_SndData(0,BedPreheat);
			delay(1);
			if(LanguageRecbuf != 0)
			{
				RTS_SndData(8,IconPrintstatus);	// 8 for Cooling
				RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
			}
			else
			{
				RTS_SndData(8+CEIconGrap,IconPrintstatus);
				RTS_SndData(ExchangePageBase + 57, ExchangepageAddr);
			}

			PrinterStatusKey[1] = 2;
		}
		break;

	case ManualSetTemp:
		if(recdat.addr == NzBdSet)
		{
			if(recdat.data[0] == 0)
			{
				if(LanguageRecbuf != 0)
				{
					if(FanStatus)
						RTS_SndData(ExchangePageBase + 16, ExchangepageAddr); //exchange to 16 page, the fans off
					else
						RTS_SndData(ExchangePageBase + 15, ExchangepageAddr); //exchange to 15 page, the fans on
				}
				else
				{
					if(FanStatus)
						RTS_SndData(ExchangePageBase + 58, ExchangepageAddr); //exchange to 58 page, the fans off
					else
						RTS_SndData(ExchangePageBase + 57, ExchangepageAddr); //exchange to 57 page, the fans on
				}
			}
			else if(recdat.data[0] == 1)
			{
				thermalManager.target_temperature[0] = 0;
				RTS_SndData(0,NozzlePreheat);
			}
			else if(recdat.data[0] == 2)
			{
				thermalManager.target_temperature_bed = 0;
				RTS_SndData(0,BedPreheat);
			}
		}
		else if(recdat.addr == NozzlePreheat)
		{
			thermalManager.target_temperature[0] = recdat.data[0];
		}
		else if(recdat.addr == BedPreheat)
		{
			thermalManager.target_temperature_bed = recdat.data[0];
		}
		break;

	case Setting:
		if(recdat.data[0] == 0)	// return to main page
		{
			InforShowStatus = true;
			TPShowStatus = false;
		}
		else if(recdat.data[0] == 1)	//Bed Autoleveling
		{
			AutoLevelStatus?RTS_SndData(2, AutoLevelIcon):RTS_SndData(3, AutoLevelIcon);
			RTS_SndData(10, FilenameIcon);	//Motor Icon
			waitway = 2;		//only for prohibiting to receive massage

			enqueue_and_echo_commands_P((PSTR("G28")));
			waitway = 2;
			 set_bed_leveling_enabled(false);
			enqueue_and_echo_commands_P((PSTR("G1 F150 Z0.0")));
			 set_bed_leveling_enabled(true);
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 22, ExchangepageAddr); // Autoleve
			else
				RTS_SndData(ExchangePageBase + 64, ExchangepageAddr); 
			
		}
		else if(recdat.data[0] == 2)	// Exchange filement
		{
			InforShowStatus = true;
			TPShowStatus = false;
			memset(ChangeMaterialbuf,0,sizeof(ChangeMaterialbuf));
			ChangeMaterialbuf[1]=ChangeMaterialbuf[0] = 10;
			RTS_SndData(10*ChangeMaterialbuf[0], FilementUnit1);	//It's ChangeMaterialbuf for show,instead of current_position[E_AXIS] in them.
			RTS_SndData(10*ChangeMaterialbuf[1], FilementUnit2);
			RTS_SndData(thermalManager.current_temperature[0],NozzleTemp);
			RTS_SndData(thermalManager.target_temperature[0],NozzlePreheat);
			delay(2);
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //Change filement
			else
				RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
			
		}
		else if(recdat.data[0] == 3)	//Move
		{
			//InforShowoStatus = false;
			AxisPagenum = 0;
			rtscheck.RTS_SndData(10*current_position[X_AXIS], DisplayXaxis);
			rtscheck.RTS_SndData(10*current_position[Y_AXIS], DisplayYaxis);
			rtscheck.RTS_SndData(10*current_position[Z_AXIS], DisplayZaxis);
			delay(2);
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 29, ExchangepageAddr); //Move axis, 10mm
			else
				RTS_SndData(ExchangePageBase + 71, ExchangepageAddr); 
		}
		else if(recdat.data[0] == 4)	//Language
		{

		}
		else if(recdat.data[0] == 5)	//Printer Information
		{
			if(LanguageRecbuf != 0)
				RTS_SndData(WEBSITE_URL, CorpWebsite);
			else
				RTS_SndData(WEBSITE_URL, CorpWebsite);
		}
		else if(recdat.data[0] == 6)// Diabalestepper
		{
			 enqueue_and_echo_commands_P(PSTR("M84"));
			 RTS_SndData(11, FilenameIcon); 
		}
		break;

	case ReturnBack:
		 if(recdat.data[0] == 1)	 // return to the tool page
		 {
		 	Update_Time_Value = RTS_UPDATE_VALUE;
		 	InforShowStatus = false;
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 21, ExchangepageAddr); 
			else
				RTS_SndData(ExchangePageBase + 63, ExchangepageAddr); 
		 }
		 if(recdat.data[0] == 2)	 // return to the Level mode page
		 {
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 22, ExchangepageAddr); 
			else
				RTS_SndData(ExchangePageBase + 64, ExchangepageAddr); 
		 }
		break;
		
	case Bedlevel:
		#if (ENABLED(MachineCRX) && DISABLED(Force10SProDisplay)) || ENABLED(ForceCRXDisplay)
			if(recdat.data[0] == 1) // Top Left
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z3;"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X30 Y30 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z-3")));
				
			}
			else if(recdat.data[0] == 2) // Top Right
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z3"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X270 Y30 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z-3")));
			}
			else if(recdat.data[0] == 3) //  Centre
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z3"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X150 Y150 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z-3")));
			}
			else if(recdat.data[0] == 4) // Bottom Left
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z3")));
				enqueue_and_echo_commands_P((PSTR("G1 X30 Y270 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z-3")));
			}
			else if(recdat.data[0] == 5) //  Bottom Right
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z3")));
				enqueue_and_echo_commands_P((PSTR("G1 X270 Y270 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z-3")));
			}
			break;
		#else
			if(recdat.data[0] == 1)// Z-axis to home
			{
				// Disallow Z homing if X or Y are unknown
				if (!all_axes_known())
					enqueue_and_echo_commands_P(PSTR("G28")); 
				else
					enqueue_and_echo_commands_P(PSTR("G28 Z0")); 
				set_bed_leveling_enabled(false);
				enqueue_and_echo_commands_P(PSTR("G1 F150 Z0.0")); 
				//set_bed_leveling_enabled(true);
				RTS_SndData(3, AutoLevelIcon);//2=On, 3=Off
				rts_probe_zoffset = zprobe_zoffset;
				RTS_SndData(zprobe_zoffset*100, 0x1026); 
			}
			else if(recdat.data[0] == 2)// Z-axis to Up
			{
				//current_position[Z_AXIS] += 0.1; 
				//RTS_line_to_current(Z_AXIS);
				if (WITHIN((zprobe_zoffset +  0.1), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        			thermalManager.babystep_axis(Z_AXIS, 40);
        			zprobe_zoffset = (zprobe_zoffset + 0.1);
					rts_probe_zoffset = zprobe_zoffset;
					RTS_SndData(zprobe_zoffset*100, 0x1026);
					settings.save();
				}
			}
			else if(recdat.data[0] == 3)// Z-axis to Down
			{
				if (WITHIN((zprobe_zoffset -  0.1), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        			thermalManager.babystep_axis(Z_AXIS, -40);
        			zprobe_zoffset = (zprobe_zoffset - 0.1);
					rts_probe_zoffset = zprobe_zoffset;
					RTS_SndData(zprobe_zoffset*100, 0x1026);
					settings.save();
				}
			}
			else if(recdat.data[0] == 4) 	// Assitant Level
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G28 X0 Y0 Z0")));
				enqueue_and_echo_commands_P((PSTR("G90")));
				waitway = 2;
				set_bed_leveling_enabled(false);
				enqueue_and_echo_commands_P((PSTR("G1 F200 Z0.0")));
				//set_bed_leveling_enabled(true);
				RTS_SndData(3, AutoLevelIcon);//2=On, 3=Off
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 28, ExchangepageAddr); 
				else
					RTS_SndData(ExchangePageBase + 84, ExchangepageAddr); 
			}
			else if(recdat.data[0] == 5) 	// AutoLevel
			{
				waitway = 3;		//only for prohibiting to receive massage
				RTS_SndData(1, AutolevelIcon); 
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 43, ExchangepageAddr); // Autoleve ... ,please wait
				else
					RTS_SndData(ExchangePageBase + 85, ExchangepageAddr); 
				enqueue_and_echo_commands_P(PSTR("G29")); 
				//stepper.synchronize();
				set_bed_leveling_enabled(false);
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z10.2;"))); 
				enqueue_and_echo_commands_P(PSTR("G1 X150 Y150 F5000")); 
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z0.0")));
				//set_bed_leveling_enabled(true);
				RTS_SndData(3, AutoLevelIcon);//2=On, 3=Off
			}
			else if(recdat.data[0] == 6) 	// Assitant Level ,  Centre 1
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z3;"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X150 Y150 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z0")));
			}
			else if(recdat.data[0] == 7) 	// Assitant Level , Front Left 2
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z3;"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X30 Y30 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z0")));
			}
			else if(recdat.data[0] == 8) 	// Assitant Level , Front Right 3
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z3;"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X270 Y30 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z0")));
			}
			else if(recdat.data[0] == 9) 	// Assitant Level , Back Right 4
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z3;"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X270 Y270 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z0")));
			}
			else if(recdat.data[0] == 10) 	// Assitant Level , Back Left 5
			{
				waitway = 4;		//only for prohibiting to receive massage
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z3;"))); 
				enqueue_and_echo_commands_P((PSTR("G1 X30 Y270 F5000")));
				waitway = 2;
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z0")));
			}
			else if(recdat.data[0] == 11) 	// Autolevel switch
			{
				if(AutoLevelStatus)	//turn on the Autolevel
				{
					RTS_SndData(3, AutoLevelIcon);	
					AutoLevelStatus = false;
					enqueue_and_echo_commands_P((PSTR("M420 S1")));
				}
				else//turn off the Autolevel
				{
					RTS_SndData(2, AutoLevelIcon);
					AutoLevelStatus = true;
					enqueue_and_echo_commands_P((PSTR("M420 S0")));
				}
				last_zoffset = rts_probe_zoffset;
				RTS_SndData(zprobe_zoffset*100, 0x1026); 
				//eeprom_write_byte((unsigned char*)FONT_EEPROM+2, AutoLevelStatus);
			}
			
			RTS_SndData(10, FilenameIcon); 
			#endif
		break;

	case XYZEaxis:
		AxisEnum axis;
		float min,max;
		waitway = 4;
		if(recdat.addr == DisplayXaxis)
		{
			axis = X_AXIS;
			min = X_MIN_POS;
			max = X_MAX_POS;
		}
		else if(recdat.addr == DisplayYaxis)
		{
			axis = Y_AXIS;
			min = Y_MIN_POS;
			max = Y_MAX_POS;
		}
		else if(recdat.addr == DisplayZaxis)
		{
			axis = Z_AXIS;
			min = Z_MIN_POS;
			max = Z_MAX_POS;
		}
		else if(recdat.addr == AutoZero)
		{
			if(recdat.data[0] == 3)	//autohome
			{
				waitway = 4;
				enqueue_and_echo_commands_P((PSTR("G28")));
				enqueue_and_echo_commands_P((PSTR("G1 F100 Z10.2")));
				InforShowStatus = AutohomeKey = true;
				AutoHomeIconNum = 0;
				Update_Time_Value = 0;
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 32, ExchangepageAddr); 
				else
					RTS_SndData(ExchangePageBase + 74, ExchangepageAddr); 
				RTS_SndData(10,FilenameIcon);
			}
			else
			{
				AxisPagenum = recdat.data[0];
				waitway = 0;
			}
			break;
		}
		
		current_position[axis] = ((float)recdat.data[0])/10;
		if (current_position[axis] < min) current_position[axis] = min;
		else if (current_position[axis] > max) current_position[axis] = max;
		
		if(axis==Z_AXIS) //roll polling
		{
			RTS_line_to_current(axis);
			RTS_SndData(10*current_position[X_AXIS], DisplayXaxis);
			RTS_SndData(10*current_position[Y_AXIS], DisplayYaxis);
			RTS_SndData(10*current_position[Z_AXIS], DisplayZaxis);
		}
		else
			RTS_line_to_current(axis);
		delay(1);
		RTS_SndData(10, FilenameIcon); 
		waitway = 0;
		break;

	case Filement:
		
		#if ENABLED(MachineCR10SPro) || ENABLED(AddonFilSensor)
		/**************checking filement status during changing filement************/
		if(RTS_CheckFilement(3)) break;
		#endif
		
		unsigned int IconTemp;
		if(recdat.addr == Exchfilement)
		{
			if(recdat.data[0] == 1)	// Unload filement1
			{
				original_extruder = active_extruder;
				active_extruder = 0;
				
				current_position[E_AXIS] -= ChangeMaterialbuf[0];

				if( NozzleTempStatus[1]== 0 && thermalManager.current_temperature[0] < (PLA_ABSModeTemp-5))
				{
					NozzleTempStatus[1] = 1; 
					RTS_SndData((int)PLA_ABSModeTemp, 0x1020);
					delay(5);
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 24, ExchangepageAddr); //exchange to 24 page
					else
						RTS_SndData(ExchangePageBase + 66, ExchangepageAddr); 
					break;
				}
			}
			else if(recdat.data[0] == 2) // Load filement1
			{
				original_extruder = active_extruder;
				active_extruder = 0;
				current_position[E_AXIS] += ChangeMaterialbuf[0];
				
				if( NozzleTempStatus[1]== 0 && thermalManager.current_temperature[0] < (PLA_ABSModeTemp-5))
				{
					NozzleTempStatus[1] = 1; 
					RTS_SndData((int)PLA_ABSModeTemp, 0x1020);
					delay(5);
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 24, ExchangepageAddr); //exchange to 24 page
					else
						RTS_SndData(ExchangePageBase + 66, ExchangepageAddr); 
					break;
				}
			}
			else if(recdat.data[0] == 3) // Unload filement2
			{
				original_extruder = active_extruder;
				active_extruder = 1;
				
				current_position[E_AXIS] -= ChangeMaterialbuf[1];
				
				if( NozzleTempStatus[1]== 0 && thermalManager.current_temperature[0] < (PLA_ABSModeTemp-5))
				{
					NozzleTempStatus[1] = 1; 
					RTS_SndData((int)PLA_ABSModeTemp, 0x1020);
					delay(5);
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 24, ExchangepageAddr); //exchange to 24 page
					else
						RTS_SndData(ExchangePageBase + 66, ExchangepageAddr); 
					break;
				}
			}
			else if(recdat.data[0] == 4) // Load filement2
			{
				original_extruder = active_extruder;
				active_extruder = 1;
				
				current_position[E_AXIS] += ChangeMaterialbuf[1];
				
				if( NozzleTempStatus[1]== 0 && thermalManager.current_temperature[0] < (PLA_ABSModeTemp-5))
				{
					NozzleTempStatus[1] = 1; 
					RTS_SndData((int)PLA_ABSModeTemp, 0x1020);
					delay(5);
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 24, ExchangepageAddr); //exchange to 24 page
					else
						RTS_SndData(ExchangePageBase + 66, ExchangepageAddr); 
					break;
				}
			}
			else if(recdat.data[0] == 5) // sure to heat
			{
				NozzleTempStatus[0] = 1;
				//InforShowoStatus = true;
				
				thermalManager.target_temperature[0] = (thermalManager.target_temperature[0] >= PLA_ABSModeTemp? thermalManager.target_temperature[0]:  PLA_ABSModeTemp);
				IconTemp = thermalManager.current_temperature[0] * 100/thermalManager.target_temperature[0];
				if(IconTemp >= 100)
					IconTemp = 100;
				RTS_SndData(IconTemp, HeatPercentIcon);

				RTS_SndData(thermalManager.current_temperature[0], NozzleTemp);
				RTS_SndData(thermalManager.target_temperature[0], NozzlePreheat); 
				delay(5);
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 26, ExchangepageAddr); //exchange to 26 page
				else
					RTS_SndData(ExchangePageBase + 68, ExchangepageAddr); 
				break;
			}
			else if(recdat.data[0] == 6) //cancel to heat
			{
				NozzleTempStatus[1] = 0;
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //exchange to 23 page
				else
					RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
				break;
			}
			else if(recdat.data[0] == 0xF1)	//Sure to cancel heating
			{
				//InforShowoStatus = true;
				NozzleTempStatus[0] = NozzleTempStatus[1] = 0;
				delay(1);
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //exchange to 22 page
				else
					RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
				break;
			}
			else if(recdat.data[0] == 0xF0)	// not to cancel heating
				break;

			RTS_line_to_current(E_AXIS);
			RTS_SndData(10*ChangeMaterialbuf[0], FilementUnit1);	//It's ChangeMaterialbuf for show,instead of current_position[E_AXIS] in them.
			RTS_SndData(10*ChangeMaterialbuf[1], FilementUnit2);
			active_extruder = original_extruder;
		}
		else if(recdat.addr == FilementUnit1)
		{
			ChangeMaterialbuf[0] = ((float)recdat.data[0])/10;
		}
		else if(recdat.addr == FilementUnit2)
		{
			ChangeMaterialbuf[1] = ((float)recdat.data[0])/10;
		}
		break;

	case LanguageChoice:
		#if DISABLED(TM3DTouchscreenUpdates)
			LanguageRecbuf = recdat.data[0];
			//eeprom_write_byte((unsigned char*)FONT_EEPROM, LanguageRecbuf);
			/*next step:record the data to EEPROM*/
			if(card.cardOK)
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(0,IconPrintstatus);	// 0 for Ready
				else
					RTS_SndData(0+CEIconGrap,IconPrintstatus);
				}
			else
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(6,IconPrintstatus);	// 6 for Card Removed
				else
					RTS_SndData(6+CEIconGrap,IconPrintstatus);
			}
		#else
			SERIAL_ECHOPAIR("\n ***recdat.data[0] =",recdat.data[0]);
			if(recdat.data[0]==1) {
				settings.save();
			}
			else {
				enqueue_and_echo_commands_P(PSTR("M300"));
			}

		#endif
		break;
		
	case No_Filement:
		char comdbuf[30];
		SERIAL_ECHO("\n No Filament");
		if(recdat.data[0] == 1)
		{
			/**************checking filement status during changing filement************/
			#if ENABLED(MachineCR10SPro) || ENABLED(AddonFilSensor)
				if(RTS_CheckFilement(0)) break;
			#endif
			
			if(FilementStatus[0] == 1)	// check filement before starting to print
			{
				enqueue_and_echo_command(cmdbuf);
				enqueue_and_echo_commands_P(PSTR("M24"));
				for(int j = 0;j < 10;j++)	//clean screen.
					RTS_SndData(0,Printfilename+j);
				
				int filelen = strlen(CardRecbuf.Cardshowfilename[FilenamesCount]);
				filelen = (TEXTBYTELEN - filelen)/2;
				if(filelen > 0)
				{
					char buf[20];
					memset(buf,0,sizeof(buf));
					strncpy(buf,"         ",filelen);
					strcpy(&buf[filelen],CardRecbuf.Cardshowfilename[FilenamesCount]);
					RTS_SndData(buf, Printfilename);
				}
				else
					RTS_SndData(CardRecbuf.Cardshowfilename[FilenamesCount], Printfilename);
				delay(2);
				if(LanguageRecbuf != 0)
				{
					RTS_SndData(1,IconPrintstatus);	// 1 for Heating 
					delay(2);
					RTS_SndData(ExchangePageBase + 10, ExchangepageAddr); //exchange to 10 page
				}
				else
				{
					RTS_SndData(1+CEIconGrap,IconPrintstatus);	// 1 for Heating 
					delay(2);
					RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 
				}
				
				TPShowStatus = InforShowStatus = true;
				PrinterStatusKey[0] = 1;
				PrinterStatusKey[1] = 3;
				CardCheckStatus[0] = 1;	// open the key of  checking card in  printing
				FilenamesCount = PrintStatue[1] = 0;

				FilementStatus[0]  =  0; // recover the status waiting to check filements
			}
			else if(FilementStatus[0] == 2)   // check filements status during printing
			{
				//char pause_str_Z[16];
				//memset(pause_str_Z, 0, sizeof(pause_str_Z));
				//dtostrf(pause_z, 3, 2, pause_str_Z);
				//sprintf_P(comdbuf, PSTR("M190 S%i"), tempbed);
				//enqueue_and_echo_command(comdbuf);
				memset(comdbuf,0,sizeof(comdbuf));
				sprintf_P(comdbuf, PSTR("M109 S%i"), temphot);
				enqueue_and_echo_command(comdbuf);
				//memset(comdbuf,0,sizeof(comdbuf));
				//sprintf_P(comdbuf, PSTR("G0 Z%s"), pause_str_Z);
			   	//enqueue_and_echo_command(comdbuf);
				
				card.startFileprint();
				print_job_timer.start();
				
				if(LanguageRecbuf != 0)
					RTS_SndData(1,IconPrintstatus);	// 1 for Heating
				else
					RTS_SndData(1+CEIconGrap,IconPrintstatus);
				PrintStatue[1] = 0;
				PrinterStatusKey[1] = 3;
				CardCheckStatus[0] = 1;	// open the key of  checking card in  printing
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 10, ExchangepageAddr); //exchange to 10 page
				else
					RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 

				FilementStatus[0]  =  0; // recover the status waiting to check filements
			}
			else if(FilementStatus[0] == 3)
			{
				//RTS_SndData(current_position[E_AXIS], FilementUnit1);
				// RTS_SndData(current_position[E_AXIS], FilementUnit2);
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //exchange to 23 page
				else
					RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
			}
		}
		else if(recdat.data[0] == 0)
		{
			if(FilementStatus[0] == 1)
			{
				if(LanguageRecbuf != 0)
				{
					RTS_SndData(ExchangePageBase + 2, ExchangepageAddr); //exchange to 2 page
				}
				else
				{
					RTS_SndData(ExchangePageBase + 46, ExchangepageAddr); 
				}
				PrinterStatusKey[0] = 0;
			}
			else if(FilementStatus[0] == 2) // like the pause
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 12, ExchangepageAddr); //exchange to 12 page
				else
					RTS_SndData(ExchangePageBase + 54, ExchangepageAddr); 
			}
			else if(FilementStatus[0] == 3)
			{
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //exchange to 23 page
				else
					RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
			}
			FilementStatus[0]  =  0; // recover the status waiting to check filements
		}
		break;
	#if ENABLED(POWER_LOSS_RECOVERY)
	case PwrOffNoF:
		//SERIAL_ECHO("\n   recdat.data[0] ==");
		//SERIAL_ECHO(recdat.data[0]);
		//SERIAL_ECHO("\n   recdat.addr ==");
		//SERIAL_ECHO(recdat.addr);
		char cmd1[30];
		if(recdat.data[0] == 1)// Yes:continue to print the 3Dmode during power-off.
		{
			if (power_off_commands_count > 0) {
				sprintf_P(cmd1, PSTR("M190 S%i"), power_off_info.target_temperature_bed);
				enqueue_and_echo_command(cmd1);
				sprintf_P(cmd1, PSTR("M109 S%i"), power_off_info.target_temperature[0]);
				enqueue_and_echo_command(cmd1);
				enqueue_and_echo_commands_P(PSTR("M106 S255"));
				sprintf_P(cmd1, PSTR("T%i"), power_off_info.saved_extruder);
				enqueue_and_echo_command(cmd1);
				power_off_type_yes = 1;

				#if FAN_COUNT > 0
				for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = FanOn;
				#endif
				FanStatus = false;
			
				PrintStatue[1] = 0;
				PrinterStatusKey[0] = 1; 
				PrinterStatusKey[1] = 3;
				PoweroffContinue = true;
				TPShowStatus = InforShowStatus = true;
				CardCheckStatus[0] = 1;	// open the key of  checking card in  printing
				Update_Time_Value = 0;
				if(LanguageRecbuf != 0)
				{
					RTS_SndData(1,IconPrintstatus);	// 1 for Heating
					RTS_SndData(ExchangePageBase + 10, ExchangepageAddr); //exchange to 10 page
				}
				else
				{
					RTS_SndData(1+CEIconGrap,IconPrintstatus);
					RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 
				}
				//card.startFileprint();
				//print_job_timer.power_off_start();
			}
		}
		else if(recdat.data[0] == 2)// No
		{
			InforShowStatus = true;
			TPShowStatus = false;
			Update_Time_Value = RTS_UPDATE_VALUE;
			if(LanguageRecbuf != 0)
				RTS_SndData(ExchangePageBase + 1, ExchangepageAddr); //exchange to 1 page
			else
				RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); //exchange to 45 page

			card.stopSDPrint();
			clear_command_queue();
			quickstop_stepper();
			thermalManager.disable_all_heaters();
			
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

			wait_for_heatup = false;
			PrinterStatusKey[0] = 0;
			delay(500);	//for system

		}
		break;
	#endif
	case Volume:
		if(recdat.data[0] < 0) VolumeSet = 0;
		else if(recdat.data[0] > 255 ) VolumeSet = 0xFF;
		else VolumeSet = recdat.data[0];


		if(VolumeSet == 0)
		{
			RTS_SndData(0, VolumeIcon);
			RTS_SndData(9, SoundIcon);
		}
		else
		{
			RTS_SndData((VolumeSet+1)/32 - 1, VolumeIcon);
			RTS_SndData(8, SoundIcon);
		}
		//eeprom_write_byte((unsigned char*)FONT_EEPROM+4, VolumeSet);
		RTS_SndData(VolumeSet<<8, SoundAddr+1);
		break;
		
	case Filename :
	       //if(card.cardOK && recdat.data[0] > 0 && recdat.data[0] <= CardRecbuf.Filesum && recdat.addr != 0x20D2)
		/*SERIAL_ECHO("\n   recdat.data[0] ==");
		SERIAL_ECHO(recdat.data[0]);
		SERIAL_ECHO("\n   recdat.addr ==");
		SERIAL_ECHO(recdat.addr); */
		if(card.cardOK && recdat.addr == FilenameChs)
	       {
	       	if(recdat.data[0] > CardRecbuf.Filesum) break;
			
			CardRecbuf.recordcount = recdat.data[0] - 1;
			for(int j = 0;j < 10;j++)
				RTS_SndData(0,Choosefilename+j);
			int filelen = strlen(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
			filelen = (TEXTBYTELEN - filelen)/2;
			if(filelen > 0)
			{
				char buf[20];
				memset(buf,0,sizeof(buf));
				strncpy(buf,"         ",filelen);
				strcpy(&buf[filelen],CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
				RTS_SndData(buf, Choosefilename);
			}
			else
				RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], Choosefilename);
			
			for(int j = 0;j < 8;j++)
				RTS_SndData(0,FilenameCount+j);
			char buf[20];
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d/%d",(int)recdat.data[0], CardRecbuf.Filesum);
			RTS_SndData(buf, FilenameCount);
			delay(2);
			for(int j = 1;j <= CardRecbuf.Filesum;j++)
			{
				RTS_SndData((unsigned long)0xFFFF,FilenameNature + j*16);		// white
				RTS_SndData(10,FilenameIcon1+j);	//clean
			}
				
			RTS_SndData((unsigned long)0x87F0,FilenameNature + recdat.data[0]*16);	// Light green
			RTS_SndData(6,FilenameIcon1 + recdat.data[0]);	// show frame
			
	       }
		else if(recdat.addr == FilenamePlay)
		{
			if(recdat.data[0] == 1 && card.cardOK)	//for sure
			{
				if(CardRecbuf.recordcount < 0)
					break;

				//SERIAL_ECHO("*************suceed1**********");
				char cmd[30];
				char* c;
				sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[CardRecbuf.recordcount]);
				for (c = &cmd[4]; *c; c++) *c = tolower(*c);

				FilenamesCount = CardRecbuf.recordcount;
				memset(cmdbuf,0,sizeof(cmdbuf));
				strcpy(cmdbuf,cmd);

				#if ENABLED(MachineCR10SPro) || ENABLED(AddonFilSensor)
				/**************checking filement status during printing beginning ************/
					if(RTS_CheckFilement(1)) break;
				#endif
					
				//InforShowoStatus = true;
				enqueue_and_echo_command(cmd);
				enqueue_and_echo_commands_P(PSTR("M24"));
				for(int j = 0;j < 10;j++)	//clean screen.
					RTS_SndData(0,Printfilename+j);
				
				int filelen = strlen(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
				filelen = (TEXTBYTELEN - filelen)/2;
				if(filelen > 0)
				{
					char buf[20];
					memset(buf,0,sizeof(buf));
					strncpy(buf,"         ",filelen);
					strcpy(&buf[filelen],CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
					RTS_SndData(buf, Printfilename);
				}
				else
					RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], Printfilename);
				delay(2);
				
				#if FAN_COUNT > 0
				for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = FanOn;
				#endif
				FanStatus = false;
			
				if(LanguageRecbuf != 0)
				{
					RTS_SndData(1,IconPrintstatus);	// 1 for Heating 
					delay(2);
					RTS_SndData(ExchangePageBase + 10, ExchangepageAddr); //exchange to 10 page
				}
				else
				{
					RTS_SndData(1+CEIconGrap,IconPrintstatus);	// 1 for Heating 
					delay(2);
					RTS_SndData(ExchangePageBase + 52, ExchangepageAddr); 
				}
				TPShowStatus = InforShowStatus = true;
				PrintStatue[1] = 0;
				PrinterStatusKey[0] = 1;
				PrinterStatusKey[1] = 3;
				CardCheckStatus[0] = 1;	// open the key of  checking card in  printing
				Update_Time_Value = 0;
			}
			else if(recdat.data[0] == 0) //	return to main page
			{
				InforShowStatus = true;
				TPShowStatus = false;
			}
		}
		break;
		
	default:
	       break;
    	}
	
	memset(&recdat,0 , sizeof(recdat));
	recdat.head[0] = FHONE;
	recdat.head[1] = FHTWO;
}

void EachMomentUpdate()
{		
	millis_t ms = millis();
	if(ms > next_rts_update_ms && InforShowStatus)
	{
		if ((power_off_type_yes == 0)  && lcd_sd_status && (power_off_commands_count > 0)) // print the file before the power is off.
		{
			SERIAL_PROTOCOLLN("  ***test1*** ");
			if(startprogress == 0)
			{
				rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
				
				rtscheck.RTS_SndData(5, VolumeIcon);
				rtscheck.RTS_SndData(8, SoundIcon);
				rtscheck.RTS_SndData(0xC0, VolumeIcon-2);
				if(VolumeSet == 0)
				{
					rtscheck.RTS_SndData(0, VolumeIcon);
					rtscheck.RTS_SndData(9, SoundIcon);
				}
				else
				{
					rtscheck.RTS_SndData((VolumeSet+1)/32 - 1, VolumeIcon);
					rtscheck.RTS_SndData(8, SoundIcon);
				}
				rtscheck.RTS_SndData(VolumeSet, VolumeIcon-2);
				rtscheck.RTS_SndData(VolumeSet<<8, SoundAddr+1);
			}
			if(startprogress <= 100)
				rtscheck.RTS_SndData(startprogress,StartIcon);
			else
				rtscheck.RTS_SndData((startprogress-100),StartIcon+1);
			delay(30);
			if((startprogress +=1) > 200)
			{
						#if ENABLED(POWER_LOSS_RECOVERY)
				      		power_off_type_yes = 1;
						
					for (uint16_t i = 0; i < CardRecbuf.Filesum ; i++) 
					{
						if(!strcmp(CardRecbuf.Cardfilename[i], &power_off_info.sd_filename[1]))
						{
							InforShowStatus = true;
							int filelen = strlen(CardRecbuf.Cardshowfilename[i]);
							filelen = (TEXTBYTELEN - filelen)/2;
							if(filelen > 0)
							{
								char buf[20];
								memset(buf,0,sizeof(buf));
								strncpy(buf,"         ",filelen);
								strcpy(&buf[filelen],CardRecbuf.Cardshowfilename[i]);
								rtscheck.RTS_SndData(buf, Printfilename);
							}
							else
								rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i],Printfilename); //filenames
							if(LanguageRecbuf != 0)
								rtscheck.RTS_SndData(ExchangePageBase + 36, ExchangepageAddr); //exchange to 36 page
							else
								rtscheck.RTS_SndData(ExchangePageBase + 76, ExchangepageAddr); 
							break;
						}
					}
					#endif
			}
			
			return;

		}	
		else if((power_off_type_yes == 0) && !power_off_commands_count )
		{
			
			if(startprogress == 0)
			{
				rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
				
				if(VolumeSet == 0)
				{
					rtscheck.RTS_SndData(0, VolumeIcon);
					rtscheck.RTS_SndData(9, SoundIcon);
				}
				else
				{
					rtscheck.RTS_SndData((VolumeSet+1)/32 - 1, VolumeIcon);
					rtscheck.RTS_SndData(8, SoundIcon);
				}
				rtscheck.RTS_SndData(VolumeSet, VolumeIcon-2);
				rtscheck.RTS_SndData(VolumeSet<<8, SoundAddr+1);
			}
			if(startprogress <= 100)
				rtscheck.RTS_SndData(startprogress,StartIcon);
			else
				rtscheck.RTS_SndData((startprogress-100),StartIcon+1);
			delay(30);
			if((startprogress +=1) > 200)
			{
				SERIAL_PROTOCOLLN("  startprogress ");
			   	power_off_type_yes = 1;
				InforShowStatus = true;
				TPShowStatus = false;
				Update_Time_Value = RTS_UPDATE_VALUE;
				if(LanguageRecbuf != 0)
					rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr); //exchange to 1 page
				else
					rtscheck.RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); 
			}
			return;
		}
		else
		{
			if (TPShowStatus && previous_move_ms != 0)		//need to optimize
			{
				duration_t elapsed = print_job_timer.duration();
				static unsigned int last_cardpercentValue = 101; 
				rtscheck.RTS_SndData(elapsed.value/3600,Timehour);		
				rtscheck.RTS_SndData((elapsed.value%3600)/60,Timemin);	
				if(card.sdprinting)
					progress_bar_percent = card.percentDone();
				if((card.sdprinting || print_job_timer.isRunning) && last_cardpercentValue != progress_bar_percent)
				{
					if( progress_bar_percent > 0 )
					{	
						Percentrecord = progress_bar_percent+1;
						if(Percentrecord<= 50)
						{
							rtscheck.RTS_SndData((unsigned int)Percentrecord*2 ,PrintscheduleIcon);
							rtscheck.RTS_SndData(0,PrintscheduleIcon+1);
						}
						else
						{
							rtscheck.RTS_SndData(100 ,PrintscheduleIcon);
							rtscheck.RTS_SndData((unsigned int)Percentrecord*2 -100,PrintscheduleIcon+1);
						}
					}	
					else
					{
						rtscheck.RTS_SndData(0,PrintscheduleIcon);
						rtscheck.RTS_SndData(0,PrintscheduleIcon+1);
					}
					rtscheck.RTS_SndData((unsigned int)progress_bar_percent,Percentage);
					last_cardpercentValue = progress_bar_percent;
				}
			}
			
			rtscheck.RTS_SndData(zprobe_zoffset*100, 0x1026); 
			//float temp_buf = thermalManager.current_temperature[0];
			rtscheck.RTS_SndData(thermalManager.current_temperature[0],NozzleTemp);
			rtscheck.RTS_SndData(thermalManager.current_temperature_bed,Bedtemp);
			if(last_target_temperature_bed != thermalManager.target_temperature_bed || (last_target_temperature[0] != thermalManager.target_temperature[0]))
			{
				rtscheck.RTS_SndData(thermalManager.target_temperature[0],NozzlePreheat);
				rtscheck.RTS_SndData(thermalManager.target_temperature_bed,BedPreheat);

				if(card.sdprinting)
				{
					//keep the icon
				}
				else if(last_target_temperature_bed < thermalManager.target_temperature_bed || (last_target_temperature[0] < thermalManager.target_temperature[0]))
				{
					if(LanguageRecbuf != 0)
						rtscheck.RTS_SndData(1,IconPrintstatus);	// 1 for Heating
					else
						rtscheck.RTS_SndData(1+CEIconGrap,IconPrintstatus);
					Update_Time_Value = 0;
					PrinterStatusKey[1] =( PrinterStatusKey[1] == 0? 1 : PrinterStatusKey[1]);
				}
				else if(last_target_temperature_bed > thermalManager.target_temperature_bed || (last_target_temperature[0] > thermalManager.target_temperature[0]))
				{
					if(LanguageRecbuf != 0)
						rtscheck.RTS_SndData(8,IconPrintstatus);	// 8 for Cooling
					else
						rtscheck.RTS_SndData(8+CEIconGrap,IconPrintstatus);
					Update_Time_Value = 0;
					PrinterStatusKey[1] =( PrinterStatusKey[1] == 0? 2 : PrinterStatusKey[1] );
				}
					
				last_target_temperature_bed = thermalManager.target_temperature_bed;
				last_target_temperature[0] = thermalManager.target_temperature[0];

			}

			if(NozzleTempStatus[0] || NozzleTempStatus[2])	//statuse of loadfilement and unloadfinement when temperature is less than
			{
				unsigned int IconTemp;
				
				IconTemp = thermalManager.current_temperature[0] * 100/thermalManager.target_temperature[0];
				if(IconTemp >= 100)
					IconTemp = 100;
				rtscheck.RTS_SndData(IconTemp, HeatPercentIcon);
				if(thermalManager.current_temperature[0] >= thermalManager.target_temperature[0] && NozzleTempStatus[0])
				{
					NozzleTempStatus[1] = 0;
					NozzleTempStatus[0] = 0;
					rtscheck.RTS_SndData(10*ChangeMaterialbuf[0], FilementUnit1);	
					rtscheck.RTS_SndData(10*ChangeMaterialbuf[1], FilementUnit2);
					if(LanguageRecbuf != 0)
						rtscheck.RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //exchange to 23 page
					else
						rtscheck.RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
					RTS_line_to_current(E_AXIS);
					active_extruder = original_extruder;
					//delay(current_position[E_AXIS] * 1000);
				}
				else if(thermalManager.current_temperature[0] >= thermalManager.target_temperature[0] && NozzleTempStatus[2])
				{
					SERIAL_ECHOPAIR("\n ***NozzleTempStatus[2] =",(int)NozzleTempStatus[2]);
					startprogress = NozzleTempStatus[2] = 0;
					TPShowStatus = true;
					rtscheck.RTS_SndData(4, ExchFlmntIcon);
					if(LanguageRecbuf != 0)
						rtscheck.RTS_SndData(ExchangePageBase + 8, ExchangepageAddr); //exchange to 23 page
					else
						rtscheck.RTS_SndData(ExchangePageBase + 83, ExchangepageAddr); 
				}
				else if( NozzleTempStatus[2] )
				{
					rtscheck.RTS_SndData((startprogress++)%5, ExchFlmntIcon);
				}
			}
			if(AutohomeKey )
			{
				rtscheck.RTS_SndData(AutoHomeIconNum++,AutoZeroIcon);
				if(AutoHomeIconNum > 9)	AutoHomeIconNum = 0;
			}
		}

		if(planner.leveling_active) 
			rtscheck.RTS_SndData(2, AutoLevelIcon);/*Off*/
		else
			rtscheck.RTS_SndData(3, AutoLevelIcon);/*On*/
		next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + Update_Time_Value;
	}

}

void RTSUpdate()	//looping at the loop function
{
	/*Check the status of card*/
	rtscheck.RTS_SDCardUpate();
	
#if ENABLED(MachineCR10SPro) || ENABLED(AddonFilSensor)
	/*checking filement status during printing */
	
	//SERIAL_ECHOPAIR("\n ***FilementStatus[1] =",FilementStatus[1]);
	//SERIAL_ECHOPAIR("\n ***card.sdprinting =",card.sdprinting);

	if(FilementStatus[1] == 2 && (card.sdprinting || planner.movesplanned()))
	{	
		//SERIAL_ECHOPAIR("\n FIL_RUNOUT_PIN =",card.sdprinting);
		//char cmd[2][30];
		if(READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING)
		{
			//SERIAL_PROTOCOLLN("  Filament runout debounce");
			Checkfilenum++;
			delay(5);
			if(Checkfilenum>50)
			{
				//SERIAL_PROTOCOLLN("  Filament runout > 50");
				//char pause_str_Z[16];
				//current_position[Z_AXIS] += 5;
				//pause_z = current_position[Z_AXIS];
				waitway = 5;		//reject to receive cmd and jump to the corresponding page
	 			card.pauseSDPrint();
				print_job_timer.pause();
	 			temphot=thermalManager.degTargetHotend(0); //thermalManager.target_temperature[0];
	 			//tempbed=thermalManager.degTargetBed();//thermalManager.target_temperature_bed;
				//thermalManager.setTargetHotend(0, 0);
				//thermalManager.setTargetBed(0);
			//	enqueue_and_echo_commands_P(PSTR("G1 X10 Y10"));

	 			PrintStatue[1] = 1;	// for returning the corresponding page
	 	//		FilementStatus[0] = 2;  // no filements during printing
				Checkfilenum=0;
				FilementStatus[1] = 0;
				PrinterStatusKey[1] = 4;
				
				TPShowStatus = false;
				Update_Time_Value = 0;
				//memset(commandbuf,0,sizeof(commandbuf));
				//dtostrf(pause_z, 3, 2, pause_str_Z);
				//sprintf(commandbuf,"G0 X10 Y10  Z%s",pause_str_Z);
	   			//strncpy_P(commandbuf, PSTR("G28 X0 Y0"), sizeof(commandbuf) - 1);
				//injected_commands_P =commandbuf;// PSTR("G28 X0 Y0");//commandbuf;

				#ifdef ACTION_ON_PAUSE
					SERIAL_ECHOLNPGM("\nFrom Runout\n");
					SERIAL_ECHOLNPGM("//action:" ACTION_ON_PAUSE);
				#endif
				planner.synchronize();
				enqueue_and_echo_commands_P(PSTR("M25"));

			}
		}
	}
#endif

	EachMomentUpdate();

if(waitway!=0)
	SERIAL_ECHOPAIR("\n RTSUpdate Waitway",waitway);
	/*wait to receive massage and response*/
	if(!waitway && rtscheck.RTS_RecData() > 0) {
		//SERIAL_PROTOCOLLN("  Handle Data ");
	    rtscheck.RTS_HandleData();
	}
}
