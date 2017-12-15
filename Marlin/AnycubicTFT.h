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

char *itostr2(const uint8_t &x);

#define TFTBUFSIZE 4
#define TFT_MAX_CMD_SIZE 96
#define MSG_MY_VERSION "V116"

class AnycubicTFTClass {
public:
  AnycubicTFTClass();
  void Setup();
  void CommandScan();
  void HotbedHeatingDone();
  void HeatingDone();
  void FilamentRunout();
  
private:
  char TFTcmdbuffer[TFTBUFSIZE][TFT_MAX_CMD_SIZE];
  int TFTbuflen=0;
  int TFTbufindr = 0;
  int TFTbufindw = 0;
  char serial3_char;
  int serial3_count = 0;
  char *TFTstrchr_pointer;
  char TFTpausingFlag=0;
  char TFTStatusFlag=0;
  char TFTresumingflag=0;
  char sdcardstartprintingflag=0;
  char FlagResumFromOutage=0;
  uint16_t MyFileNrCnt=0;
  uint16_t fileoutputcnt=0;
  uint16_t filenumber=0;
  unsigned long previous_millis_cmd = 0;
  unsigned long starttime=0;
  unsigned long stoptime=0;
  uint8_t tmp_extruder=0;
  long TFTcode_N=0;
  long TFTcode_LastN=0;

  void ClearToSend();
  void FlushSerialRequestResend();
  float CodeValue();
  bool CodeSeen(char);
  uint16_t GetFileNr();
  void Ls();
  void GetCommandFromTFT();

#if ENABLED(ANYCUBIC_FILAMENT_RUNOUT_SENSOR)
  char FilamentTestStatus=false;
  char FilamentTestLastStatus=false;
  long FilamentRunoutCounter=0;
#endif
};

extern AnycubicTFTClass AnycubicTFT;

#endif /* AnycubicTFT_h */
