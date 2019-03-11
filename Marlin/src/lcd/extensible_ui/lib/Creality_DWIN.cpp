#include "Creality_DWIN.h"
#include "RTS_Show.h"
#include <HardwareSerial.h>
#include <arduino.h>
#include <wstring.h>
#include <stdio.h>

#include "../ui_api.h"

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
int Update_Time_Value = 0;
unsigned long VolumeSet = 0x80;
extern char power_off_commands[9][96];
bool PoweroffContinue = false;
extern const char *injected_commands_P;
char commandbuf[30];


  void onStartup() {
    Serial2.begin(115200);

	//LanguageRecbuf =eeprom_read_byte((unsigned char*)FONT_EEPROM);
	//AutoLevelStatus = eeprom_read_byte((unsigned char*)FONT_EEPROM+2);
	LanguageRecbuf = 0;
	AutoLevelStatus = 1;
	int showcount = 0;

	recdat.head[0] = snddat.head[0] = FHONE;
  recdat.head[1] = snddat.head[1] = FHTWO;
  memset(databuf,0, sizeof(databuf));

	
	#if HAS_MESH && (ENABLED(MachineCR10SPro) || ENABLED(Force10SProDisplay))
		if (leveling_is_valid())
		{
			for(int xCount  = 0; xCount < GRID_MAX_POINTS_X; xCount++)
			{
				for(int yCount  = 0; yCount < GRID_MAX_POINTS_X; yCount++)
				{
					if((showcount++) < 16)
					{
						RTS_SndData(z_values[xCount][yCount] *10000, AutolevelVal + (showcount-1)*2);
						RTS_SndData(showcount,AutolevelIcon);
					}
				}
			}
			RTS_SndData(2, AutoLevelIcon);/*On*/
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
	last_target_temperature_bed = getTargetTemp_celsius(BED); 
	last_target_temperature[0] =  getTargetTemp_celsius(H0);
	feedrate_percentage = 100;
	RTS_SndData(feedrate_percentage,FeedrateDisplay);
	
	/***************turn off motor*****************/
	RTS_SndData(11, FilenameIcon); 
	
	/***************transmit temperature to screen*****************/
	RTS_SndData(0, NozzlePreheat);
	RTS_SndData(0, BedPreheat);
	RTS_SndData(getActualTemp_celsius(H0), NozzleTemp);
	RTS_SndData(getActualTemp_celsius(BED), Bedtemp);
	/***************transmit Fan speed to screen*****************/
	RTS_SndData(2, FanKeyIcon);	//turn 0ff fan icon
	FanStatus = true;
	
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
	
	rts_probe_zoffset = getZOffset_mm();
		//SERIAL_ECHOPAIR("\n init rts_probe_zoffset = ",rts_probe_zoffset);
		SERIAL_ECHOLNPAIR("\n init zprobe_zoffset = ",getZOffset_mm());
	RTS_SndData(getZOffset_mm()*100, 0x1026);  
	/************************EEPROM*******************************/
	//settings.load();
	
	SERIAL_ECHOLN("===Initing RTS has finished===");
  }

  void onIdle() {
    millis_t ms = millis();
	if(ms > next_rts_update_ms && InforShowStatus)
	{
		if ((power_off_type_yes == 0)  && lcd_sd_status && (power_off_commands_count > 0)) // print the file before the power is off.
		{
			SERIAL_PROTOCOLLN("  ***test1*** ");
			if(startprogress == 0)
			{
				RTS_SndData(StartSoundSet, SoundAddr);
				
				RTS_SndData(5, VolumeIcon);
				RTS_SndData(8, SoundIcon);
				RTS_SndData(0xC0, VolumeIcon-2);
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
				RTS_SndData(VolumeSet, VolumeIcon-2);
				RTS_SndData(VolumeSet<<8, SoundAddr+1);
			}
			if(startprogress <= 100)
				RTS_SndData(startprogress,StartIcon);
			else
				RTS_SndData((startprogress-100),StartIcon+1);
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
								RTS_SndData(buf, Printfilename);
							}
							else
								RTS_SndData(CardRecbuf.Cardshowfilename[i],Printfilename); //filenames
							if(LanguageRecbuf != 0)
								RTS_SndData(ExchangePageBase + 36, ExchangepageAddr); //exchange to 36 page
							else
								RTS_SndData(ExchangePageBase + 76, ExchangepageAddr); 
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
				RTS_SndData(StartSoundSet, SoundAddr);
				
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
				RTS_SndData(VolumeSet, VolumeIcon-2);
				RTS_SndData(VolumeSet<<8, SoundAddr+1);
			}
			if(startprogress <= 100)
				RTS_SndData(startprogress,StartIcon);
			else
				RTS_SndData((startprogress-100),StartIcon+1);
			delay(30);
			if((startprogress +=1) > 200)
			{
				SERIAL_PROTOCOLLN("  startprogress ");
			   	power_off_type_yes = 1;
				InforShowStatus = true;
				TPShowStatus = false;
				Update_Time_Value = RTS_UPDATE_VALUE;
				if(LanguageRecbuf != 0)
					RTS_SndData(ExchangePageBase + 1, ExchangepageAddr); //exchange to 1 page
				else
					RTS_SndData(ExchangePageBase + 45, ExchangepageAddr); 
			}
			return;
		}
		else
		{
			if (TPShowStatus && previous_move_ms != 0)		//need to optimize
			{
				duration_t elapsed = print_job_timer.duration();
				static unsigned int last_cardpercentValue = 101; 
				RTS_SndData(getProgress_seconds_elapsed() / 3600,Timehour);		
				RTS_SndData((getProgress_seconds_elapsed() /3600)/60,Timemin);	
				
				if(last_cardpercentValue != getProgress_percent())
				{
					if( progress_bar_percent > 0 )
					{	
						Percentrecord = progress_bar_percent+1;
						if(Percentrecord<= 50)
						{
							RTS_SndData((unsigned int)Percentrecord*2 ,PrintscheduleIcon);
							RTS_SndData(0,PrintscheduleIcon+1);
						}
						else
						{
							RTS_SndData(100 ,PrintscheduleIcon);
							RTS_SndData((unsigned int)Percentrecord*2 -100,PrintscheduleIcon+1);
						}
					}	
					else
					{
						RTS_SndData(0,PrintscheduleIcon);
						RTS_SndData(0,PrintscheduleIcon+1);
					}
					.RTS_SndData((unsigned int)getProgress_percent(),Percentage);
					last_cardpercentValue = getProgress_percent();
				}
			}
			
			RTS_SndData(getZOffset_mm()*100, 0x1026); 
			//float temp_buf = thermalManager.current_temperature[0];
			RTS_SndData(getActualTemp_celsius(H0),NozzleTemp);
			RTS_SndData(getActualTemp_celsius(BED),Bedtemp);
			if(last_target_temperature_bed != getTargetTemp_celsius(BED) || (last_target_temperature[0] != getTargetTemp_celsius(H0)))
			{
				RTS_SndData(getTargetTemp_celsius(H0),NozzlePreheat);
				RTS_SndData(getTargetTemp_celsius(BED),BedPreheat);

				if(card.sdprinting)
				{
					//keep the icon
				}
				else if(last_target_temperature_bed < thermalManager.target_temperature_bed || (last_target_temperature[0] < thermalManager.target_temperature[0]))
				{
					if(LanguageRecbuf != 0)
						RTS_SndData(1,IconPrintstatus);	// 1 for Heating
					else
						RTS_SndData(1+CEIconGrap,IconPrintstatus);
					Update_Time_Value = 0;
					PrinterStatusKey[1] =( PrinterStatusKey[1] == 0? 1 : PrinterStatusKey[1]);
				}
				else if(last_target_temperature_bed > thermalManager.target_temperature_bed || (last_target_temperature[0] > thermalManager.target_temperature[0]))
				{
					if(LanguageRecbuf != 0)
						RTS_SndData(8,IconPrintstatus);	// 8 for Cooling
					else
						RTS_SndData(8+CEIconGrap,IconPrintstatus);
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
				RTS_SndData(IconTemp, HeatPercentIcon);
				if(getActualTemp_celsius(H0) >= getTargetTemp_celsius(H0) && NozzleTempStatus[0])
				{
					NozzleTempStatus[1] = 0;
					NozzleTempStatus[0] = 0;
					RTS_SndData(10*ChangeMaterialbuf[0], FilementUnit1);	
					RTS_SndData(10*ChangeMaterialbuf[1], FilementUnit2);
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 23, ExchangepageAddr); //exchange to 23 page
					else
						RTS_SndData(ExchangePageBase + 65, ExchangepageAddr); 
					RTS_line_to_current(E_AXIS);
					active_extruder = original_extruder;
					//delay(current_position[E_AXIS] * 1000);
				}
				else if(thermalManager.current_temperature[0] >= thermalManager.target_temperature[0] && NozzleTempStatus[2])
				{
					SERIAL_ECHOPAIR("\n ***NozzleTempStatus[2] =",(int)NozzleTempStatus[2]);
					startprogress = NozzleTempStatus[2] = 0;
					TPShowStatus = true;
					RTS_SndData(4, ExchFlmntIcon);
					if(LanguageRecbuf != 0)
						RTS_SndData(ExchangePageBase + 8, ExchangepageAddr); //exchange to 23 page
					else
						RTS_SndData(ExchangePageBase + 83, ExchangepageAddr); 
				}
				else if( NozzleTempStatus[2] )
				{
					RTS_SndData((startprogress++)%5, ExchFlmntIcon);
				}
			}
			if(AutohomeKey )
			{
				RTS_SndData(AutoHomeIconNum++,AutoZeroIcon);
				if(AutoHomeIconNum > 9)	AutoHomeIconNum = 0;
			}
		}

		if(planner.leveling_active) 
			RTS_SndData(2, AutoLevelIcon);/*Off*/
		else
			RTS_SndData(3, AutoLevelIcon);/*On*/
		next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + Update_Time_Value;
	}
  //SERIAL_ECHOPAIR("\n RTSUpdate Waitway",waitway);
	/*wait to receive massage and response*/
	if(!waitway && RTS_RecData() > 0)
		//SERIAL_PROTOCOLLN("  Handle Data ");
	    RTS_HandleData();

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

//SERIAL_ECHOPAIR("\n RTSUpdate Waitway",waitway);
	/*wait to receive massage and response*/
	if(!waitway && rtscheck.RTS_RecData() > 0)
		//SERIAL_PROTOCOLLN("  Handle Data ");
	    rtscheck.RTS_HandleData();
}