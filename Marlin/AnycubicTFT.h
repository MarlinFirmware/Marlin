/*
 AnycubicTFT.h  --- Support for Anycubic i3 Mega TFT
 Created by Christian Hopp on 09.12.17.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef AnycubicTFT_h
#define AnycubicTFT_h

#include <stdio.h>
#include "MarlinConfig.h"

char *itostr2(const uint8_t &x);

#ifndef ULTRA_LCD
char* itostr3(const int);
#endif

#define TFTBUFSIZE 4
#define TFT_MAX_CMD_SIZE 96
#define MSG_MY_VERSION "V116"

#define ANYCUBIC_TFT_STATE_IDLE           0
#define ANYCUBIC_TFT_STATE_SDPRINT        1
#define ANYCUBIC_TFT_STATE_SDPAUSE        2
#define ANYCUBIC_TFT_STATE_SDPAUSE_REQ    3
#define ANYCUBIC_TFT_STATE_SDPAUSE_OOF    4
#define ANYCUBIC_TFT_STATE_SDSTOP_REQ     5
#define ANYCUBIC_TFT_STATE_SDOUTAGE       99


class AnycubicTFTClass {
public:
  AnycubicTFTClass();
  void Setup();
  void CommandScan();
  void BedHeatingStart();
  void BedHeatingDone();
  void HeatingDone();
  void HeatingStart();
  void FilamentRunout();
  void KillTFT();
  
private:
  char TFTcmdbuffer[TFTBUFSIZE][TFT_MAX_CMD_SIZE];
  int TFTbuflen=0;
  int TFTbufindr = 0;
  int TFTbufindw = 0;
  char serial3_char;
  int serial3_count = 0;
  char *TFTstrchr_pointer;
  char TFTstate=ANYCUBIC_TFT_STATE_IDLE;
  char FlagResumFromOutage=0;
  uint16_t MyFileNrCnt=0;
  uint16_t fileoutputcnt=0;
  uint16_t filenumber=0;
  unsigned long starttime=0;
  unsigned long stoptime=0;
  uint8_t tmp_extruder=0;
  char LastSDstatus=0;
  uint16_t HeaterCheckCount=0;
  
  struct OutageDataStruct {
    char OutageDataVersion;
    char OutageFlag;
    float last_position[XYZE];
    float last_bed_temp;
    float last_hotend_temp;
    long lastSDposition;
  } OutageData;
  
  void WriteOutageEEPromData();
  void ReadOutageEEPromData();
  
  float CodeValue();
  bool CodeSeen(char);
  uint16_t GetFileNr();
  void Ls();
  void StartPrint();
  void PausePrint();
  void StopPrint();
  void StateHandler();
  void GetCommandFromTFT();
  void CheckSDCardChange();
  void CheckHeaterError();

#if ENABLED(ANYCUBIC_FILAMENT_RUNOUT_SENSOR)
  char FilamentTestStatus=false;
  char FilamentTestLastStatus=false;
  long FilamentRunoutCounter=0;
#endif
};

extern AnycubicTFTClass AnycubicTFT;

#endif /* AnycubicTFT_h */
