/*
 AnycubicTFT.h  --- Support for Anycubic i3 Mega TFT
 Created by Christian Hopp on 09.12.17.
 Improved by David Ramiro
 Converted to ext_iu by John BouAntoun 21 June 2020
 
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

#include "../../../../inc/MarlinConfigPre.h"
#include "../../../../sd/SdFatConfig.h"   // for the FILENAME_LENGTH macro
  
#ifndef AnycubicTFT_h
#define AnycubicTFT_h

// command sending macro's with debugging capability
#define ANYCUBIC_SENDCOMMANDPGM(x)                  ANYCUBIC_SERIAL_PROTOCOLLNPGM(x)
#define ANYCUBIC_SENDCOMMANDPGM_VAL(x,y)            (ANYCUBIC_SERIAL_PROTOCOLPGM(x), ANYCUBIC_SERIAL_PROTOCOLLN(itostr3(y)))
#define ANYCUBIC_SENDCOMMAND(x)                     ANYCUBIC_SERIAL_PROTOCOLLN(x)
#if ENABLED(ANYCUBIC_TFT_DEBUG)
  #define ANYCUBIC_SENDCOMMAND_DBG_PGM(x,y)         (ANYCUBIC_SERIAL_PROTOCOLLNPGM(x), SERIAL_ECHOLNPGM(y))
  #define ANYCUBIC_SENDCOMMAND_DBG_PGM_VAL(x,y,z)   (ANYCUBIC_SERIAL_PROTOCOLLNPGM(x), SERIAL_ECHOPGM(y), SERIAL_ECHOLN(z))
#elif
  #define ANYCUBIC_SENDCOMMAND_DBG_PGM(x,y)         (ANYCUBIC_SERIAL_PROTOCOLLNPGM(x))
  #define ANYCUBIC_SENDCOMMAND_DBG_PGM_VAL(x,y,z)   (ANYCUBIC_SERIAL_PROTOCOLLNPGM(x))
#endif

char *itostr2(const uint8_t &x);
#ifndef ULTRA_LCD
  char *itostr3(const int);
  char *ftostr32(const float &);
#endif

#define TFTBUFSIZE 4
#define TFT_MAX_CMD_SIZE 96

// TODO: JBA don't think these are needed
/*
#define ANYCUBIC_TFT_STATE_IDLE           0
#define ANYCUBIC_TFT_STATE_SDPRINT        1
#define ANYCUBIC_TFT_STATE_SDPAUSE        2
#define ANYCUBIC_TFT_STATE_SDPAUSE_REQ    3
#define ANYCUBIC_TFT_STATE_SDPAUSE_OOF    4
#define ANYCUBIC_TFT_STATE_SDSTOP_REQ     5
#define ANYCUBIC_TFT_STATE_SDOUTAGE       99
*/
enum AnycubicMediaPrintState {
  AMPRINTSTATE_NOT_PRINTING,
  AMPRINTSTATE_PRINTING,
  AMPRINTSTATE_PAUSE_REQUESTED,
  AMPRINTSTATE_PAUSED,
  AMPRINTSTATE_STOP_REQUESTED
};

enum AnycubicMediaPauseState {
  AMPAUSESTATE_NOT_PAUSED,
  AMPAUSESTATE_PARKING,
  AMPAUSESTATE_PARKED,
  AMPAUSESTATE_FILAMENT_OUT,
  AMPAUSESTATE_FIAMENT_PRUGING,
  AMPAUSESTATE_HEATER_TIMEOUT,
  AMPAUSESTATE_REHEATING,
  AMPAUSESTATE_REHEAT_FINISHED
};

class AnycubicTFTClass {
public:
  AnycubicTFTClass();
  void OnSetup();
  void OnCommandScan();
  void OnKillTFT();
  void OnSDCardStateChange(bool);
  void OnSDCardError();
  void OnFilamentRunout();
  void OnUserConfirmRequired(const char *);
  void OnPrintTimerStarted();
  void OnPrintTimerPaused();
  void OnPrintTimerStopped();

  
private:
  char TFTcmdbuffer[TFTBUFSIZE][TFT_MAX_CMD_SIZE];
  int TFTbuflen=0;
  int TFTbufindr = 0;
  int TFTbufindw = 0;
  char serial3_char;
  int serial3_count = 0;
  char *TFTstrchr_pointer;
  uint16_t HeaterCheckCount = 0;
  uint8_t SpecialMenu = false;
  AnycubicMediaPrintState mediaPrintingState = AMPRINTSTATE_NOT_PRINTING;
  AnycubicMediaPauseState mediaPauseState = AMPAUSESTATE_NOT_PAUSED;
  millis_t clockTicks = millis();   // used to slow the stopped print check down to reasonable times
  
  float CodeValue();
  bool CodeSeen(char);
  bool IsNozzleHomed();
  void RenderCurrentFileList();
  void RenderSpecialMenu(uint16_t); 
  void RenderCurrentFolder(uint16_t);
  void GetCommandFromTFT();
  void CheckSDCardChange();
  // void CheckHeaterError();
  void CheckPauseState();
  void CheckPrintCompletion();
  void HandleSpecialMenu();
  void DoSDCardStateCheck();
  void DoFilamentRunoutCheck();
  void StartPrint();
  void PausePrint();
  void ResumePrint();
  void StopPrint();

  char     SelectedDirectory[30];
  char     SelectedFile[FILENAME_LENGTH];
  
};

extern AnycubicTFTClass AnycubicTFT;

#endif /* AnycubicTFT_h */
