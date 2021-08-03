/**
 * anycubic_i3mega_lcd.h  --- Support for Anycubic i3 Mega TFT
 * Created by Christian Hopp on 09.12.17.
 * Improved by David Ramiro
 * Converted to ExtUI by John BouAntoun 21 June 2020
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

#include "../../../inc/MarlinConfigPre.h"
#include "../../../sd/SdFatConfig.h"   // for the FILENAME_LENGTH macro

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
  static void OnSetup();
  static void OnCommandScan();
  static void OnKillTFT();
  static void OnSDCardStateChange(bool);
  static void OnSDCardError();
  static void OnFilamentRunout();
  static void OnUserConfirmRequired(const char *);
  static void OnPrintTimerStarted();
  static void OnPrintTimerPaused();
  static void OnPrintTimerStopped();

private:
  static char TFTcmdbuffer[TFTBUFSIZE][TFT_MAX_CMD_SIZE];
  static int TFTbuflen, TFTbufindr, TFTbufindw;
  static char serial3_char;
  static int serial3_count;
  static char *TFTstrchr_pointer;
  static uint8_t SpecialMenu;
  static AnycubicMediaPrintState mediaPrintingState;
  static AnycubicMediaPauseState mediaPauseState;

  static float CodeValue();
  static bool CodeSeen(char);
  static bool IsNozzleHomed();
  static void RenderCurrentFileList();
  static void RenderSpecialMenu(uint16_t);
  static void RenderCurrentFolder(uint16_t);
  static void GetCommandFromTFT();
  static void CheckSDCardChange();
  static void CheckPauseState();
  static void CheckPrintCompletion();
  static void HandleSpecialMenu();
  static void DoSDCardStateCheck();
  static void DoFilamentRunoutCheck();
  static void StartPrint();
  static void PausePrint();
  static void ResumePrint();
  static void StopPrint();

  static char SelectedDirectory[30];
  static char SelectedFile[FILENAME_LENGTH];
};

extern AnycubicTFTClass AnycubicTFT;
extern const char G28_STR[];
