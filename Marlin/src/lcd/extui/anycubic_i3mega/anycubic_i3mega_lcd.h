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
  AMPAUSESTATE_FILAMENT_PURGING,
  AMPAUSESTATE_HEATER_TIMEOUT,
  AMPAUSESTATE_REHEATING,
  AMPAUSESTATE_REHEAT_FINISHED
};

class AnycubicTFT {
public:
  AnycubicTFT();
  static void onSetup();
  static void onCommandScan();
  static void onKillTFT();
  static void onSDCardStateChange(bool);
  static void onSDCardError();
  static void onFilamentRunout();
  static void onUserConfirmRequired(const char *);
  static void onPrintTimerStarted();
  static void onPrintTimerPaused();
  static void onPrintTimerStopped();

private:
  static char tftCommands[TFTBUFSIZE][TFT_MAX_CMD_SIZE];
  static int tftBufLen, tftBufIndR, tftBufIndW;
  static char *tftStrchrPtr;
  static uint8_t specialMenu;
  static AnycubicMediaPrintState mediaPrintingState;
  static AnycubicMediaPauseState mediaPauseState;

  static float codeValue();
  static bool codeSeen(char);
  static bool isNozzleHomed();
  static void renderCurrentFileList();
  static void renderSpecialMenu(uint16_t);
  static void renderCurrentFolder(uint16_t);
  static void getCommandFromTFT();
  static void checkSDCardChange();
  static void checkPauseState();
  static void handleSpecialMenu();
  static void doSDCardStateCheck();
  static void doFilamentRunoutCheck();
  static void startPrint();
  static void pausePrint();
  static void resumePrint();
  static void stopPrint();

  static char selectedDirectory[30];
  static char selectedFile[FILENAME_LENGTH];
};

extern AnycubicTFT anycubicTFT;
extern const char G28_STR[];
