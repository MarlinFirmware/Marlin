/**
 * anycubic_i3mega_lcd.h  --- Support for Anycubic i3 Mega TFT
 * Created by Christian Hopp on 09.12.17.
 * Improved by David Ramiro
 * Converted to ext_iu by John BouAntoun 21 June 2020
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#pragma once

#include "../../../../inc/MarlinConfigPre.h"
#include "../../../../sd/SdFatConfig.h"   // for the FILENAME_LENGTH macro

#define TFTBUFSIZE 4
#define TFT_MAX_CMD_SIZE 96

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
  uint8_t SpecialMenu = false;
  AnycubicMediaPrintState mediaPrintingState = AMPRINTSTATE_NOT_PRINTING;
  AnycubicMediaPauseState mediaPauseState = AMPAUSESTATE_NOT_PAUSED;

  float CodeValue();
  bool CodeSeen(char);
  bool IsNozzleHomed();
  void RenderCurrentFileList();
  void RenderSpecialMenu(uint16_t);
  void RenderCurrentFolder(uint16_t);
  void GetCommandFromTFT();
  void CheckSDCardChange();
  void CheckPauseState();
  void CheckPrintCompletion();
  void HandleSpecialMenu();
  void DoSDCardStateCheck();
  void DoFilamentRunoutCheck();
  void StartPrint();
  void PausePrint();
  void ResumePrint();
  void StopPrint();

  char SelectedDirectory[30];
  char SelectedFile[FILENAME_LENGTH];
};

extern AnycubicTFTClass AnycubicTFT;
