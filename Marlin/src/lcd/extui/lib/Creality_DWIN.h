#pragma once

#include "string.h"
#include <Arduino.h>
#include "../ui_api.h"
#if ENABLED(EXTENSIBLE_UI)
namespace ExtUI {
/*********************************/
#define FHONE   (0x5A)
#define FHTWO   (0xA5)
#define TEXTBYTELEN		18
#define MaxFileNumber	20// 16

#define	CEIconGrap	12
#define	FileNum	MaxFileNumber
#define	FileNameLen	TEXTBYTELEN

#define SizeofDatabuf		26

/*************Register and Variable addr*****************/
#define	RegAddr_W	0x80
#define	RegAddr_R	0x81
#define	VarAddr_W	0x82
#define	VarAddr_R	0x83
#define	ExchangePageBase	(unsigned long)0x5A010000	//the first page ID. other page = first page ID + relevant num;
#define	StartSoundSet	((unsigned long)0x060480A0)		// 06,start-music; 04, 4 musics; 80, the volume value; 04, return value about music number.
#define	FONT_EEPROM	90
#define	AutoLeve_EEPROM	100
#define	FanOn	255
#define 	FanOff	0

/*variable addr*/
#define	ExchangepageAddr	0x0084
#define	SoundAddr			0x00A0
#define	StartIcon			0x1000
#define	FeedrateDisplay		0x1006 // Speed
#define	Stopprint			0x1008
#define	Pauseprint			0x100A
#define	Resumeprint			0x100C
#define	PrintscheduleIcon		0x100E
#define	Timehour			0x1010
#define	Timemin				0x1012
#define	IconPrintstatus		0x1014
#define	Percentage			0x1016
#define	FanKeyIcon			0x101E
#define Flowrate        0x1300

#define StepMM_X      0x1242
#define StepMM_Y      0x1246
#define StepMM_Z      0x124A
#define StepMM_E      0x124E

#define ProbeOffset_X  0x1236
#define ProbeOffset_Y  0x123A
#define ProbeOffset_Z  0x123E

#define HotendPID_AutoTmp 0x1252
#define BedPID_AutoTmp    0x1254

#define HotendPID_P 0x1256
#define HotendPID_I 0x125A
#define HotendPID_D 0x125E

#define BedPID_P 0x1262
#define BedPID_I 0x1266
#define BedPID_D 0x126A

#define	HeatPercentIcon		0x1024

#define	NzBdSet				0x1032
#define	NozzlePreheat		0x1034
#define	NozzleTemp			0x1036
#define	BedPreheat			0x103A
#define	Bedtemp				0x103C

#define	AutoZeroIcon		0x1042
#define	AutoLevelIcon		0x1045
#define	AutoZero			0x1046
#define	DisplayXaxis			0x1048
#define	DisplayYaxis			0x104A
#define	DisplayZaxis			0x104C

#define	FilementUnit1		0x1054
#define	Exchfilement			0x1056
#define	FilementUnit2		0x1058

#define	MacVersion			0x1060
#define	SoftVersion			0x106A
#define	PrinterSize			0x1074
#define	CorpWebsite			0x107E
#define	VolumeIcon			0x108A
#define	SoundIcon			0x108C
#define	AutolevelIcon		0x108D
#define	ExchFlmntIcon		0x108E
#define	AutolevelVal			0x1100

#define	FilenameIcon			0x1200
#define	FilenameIcon1		0x1220
#define	Printfilename			0x2000
#define	SDFILE_ADDR		0x200A
#define	FilenamePlay			0x20D2
#define	FilenameChs			0x20D3
#define	Choosefilename		0x20D4
#define	FilenameCount		0x20DE
#define	FilenameNature		0x6003

#if ANY(MachineCR10SPro, MachineEnder5Plus, MachineCR10Max) || ENABLED(Force10SProDisplay)
  #define StatusMessageString 0x3000
#else
  #define StatusMessageString 0x20E8
#endif

#if defined(MCU_LPC1769) || defined(MCU_LPC1768)
  #define DWIN_SERIAL DGUS_SERIAL
#elif defined(TARGET_STM32F4)
  #define DWIN_SERIAL Serial1
#else
  #define DWIN_SERIAL Serial2
#endif

/************struct**************/

typedef struct DataBuf
{
    unsigned char len;
    unsigned char head[2];
    unsigned char command;
    unsigned long addr;
    unsigned long bytelen;
    unsigned short data[32];
    unsigned char reserv[4];
} DB;

typedef struct CardRecord
{
    char recordcount;
    int Filesum;
    unsigned long addr[FileNum];
    char Cardshowfilename[FileNum][FileNameLen];
    char Cardfilename[FileNum][FileNameLen];
}CRec;

class RTSSHOW {
  public:
    RTSSHOW();
    int RTS_RecData();
    void RTS_SDCardInit(void);
    void RTS_SDCardUpate(bool, bool);
    int RTS_CheckFilement(int);
    void RTS_SndData(void);
    void RTS_SndData(const String &, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(const char[], unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(char, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(unsigned char*, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(int, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(float, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(unsigned int,unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(long,unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(unsigned long,unsigned long, unsigned char = VarAddr_W);
    void RTS_SDcard_Stop();
    void RTS_HandleData();
    void RTS_Init();

    DB recdat;
    DB snddat;
    unsigned char databuf[SizeofDatabuf];
  };

static RTSSHOW rtscheck;

  #define	Addvalue	3
  #define	PrintChoice_Value	(0+Addvalue)
  #define	Zoffset_Value		(3+Addvalue)
  #define	Setting_Value		(8+Addvalue)
  #define	XYZEaxis_Value		(12+Addvalue)
  #define	Filement_Value		(15+Addvalue)
  #define	Language_Value		(18+Addvalue)
  #define	Filename_Value		(22+Addvalue)

  enum PROC_COM {Printfile=0,Ajust,Feedrate,PrintChoice=PrintChoice_Value,Zoffset=Zoffset_Value,TempControl,ManualSetTemp,Setting=Setting_Value,
  ReturnBack,Bedlevel,Autohome,XYZEaxis=XYZEaxis_Value,Filement=Filement_Value,LanguageChoice=Language_Value,No_Filement,PwrOffNoF,Volume,Filename=Filename_Value};

  const unsigned long Addrbuf[] = {0x1002, 0x1004, 0x1006, 0x1008, 0x100A, 0x100C,  0x1026, 0x1030, 0x1032, 0x1034, 0x103A,
                0x103E, 0x1040, 0x1044, 0x1046, 0x1048, 0x104A, 0x104C, 0x1054, 0x1056, 0x1058,
                0x105C, 0x105E, 0x105F, 0x1088, 0};

void RTSUpdate();
void RTSInit();

}
#ifndef USER_GCODE_1
  #define USER_GCODE_1 "G28"
#endif
#endif
