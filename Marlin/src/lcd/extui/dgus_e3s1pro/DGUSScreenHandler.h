/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../inc/MarlinConfigPre.h"

#include "config/DGUS_Addr.h"
#include "config/DGUS_Data.h"
#include "config/DGUS_Screen.h"
#include "config/DGUS_Constants.h"

#include "DGUSSDCardHandler.h"

#include "../ui_api.h"

class DGUSScreenHandler {
public:
  typedef struct {
    bool initialized;
    bool levelingEnabled;
    uint8_t volume;
    uint8_t brightness;
    DGUS_Data::Language language;
    DGUS_Data::AxisControlCommand jogLength;
    uint16_t plaExtruderTemp;
    uint16_t plaBedTemp;
    uint16_t absExtruderTemp;
    uint16_t absBedTemp;
  } eeprom_data_t;

public:
  static eeprom_data_t config;
  static uint16_t currentMeshPointIndex;
  static bool isLeveling;
  
  static char homeStatusMessage[128];

private:
  static bool settings_ready;
  static bool booted;

  static DGUS_Screen current_screen;
  static DGUS_Screen new_screen;
  static DGUS_Screen wait_return_screen;
  static DGUS_Screen confirm_return_screen;
  static bool full_update;
  static uint8_t angry_beeps;

  static const char* sdPrintFilename;

  #ifdef DGUS_SOFTWARE_AUTOSCROLL
    static ssize_t currentScrollIndex;
    static size_t pageMaxStringLen;
    static size_t pageMaxControlLen;
  #endif

  static millis_t status_expire;
  static millis_t eeprom_save;

public:
  DGUSScreenHandler() = default;

  static void Init();
  static void Ready();
  static void Loop();

  static void printerKilled(FSTR_P const error, FSTR_P const component);
  static bool IsOnUserConfirmationScreen() { return confirm_return_screen != DGUS_Screen::BOOT; }
  static void UserConfirmRequired(const char * const msg);
  static void UserConfirmation();
  static void SettingsReset();
  static void StoreSettings(char *buff);
  static void LoadSettings(const char *buff);
  static void ConfigurationStoreWritten(bool success);
  static void ConfigurationStoreRead(bool success);

  static void PlayTone(const uint16_t frequency, const uint16_t duration);
  static void AngryBeeps(const uint8_t beepCount);

  static void LevelingStart();
  static void LevelingEnd();
  static void MeshUpdate(const int8_t xpos, const int8_t ypos);

  static void PrintTimerStarted();
  static void PrintTimerPaused();
  static void PrintTimerStopped();
  static void filamentRunout(const ExtUI::extruder_t extruder);

  #ifdef DGUS_SOFTWARE_AUTOSCROLL
    static ssize_t GetScrollIndex();
    static void AddCurrentPageStringLength(size_t stringLength, size_t textControlLength);
  #endif

  #if HAS_MEDIA
    /// Marlin informed us that a new SD has been inserted.
    static void SDCardInserted();
    /// Marlin informed us that the SD Card has been removed().
    static void SDCardRemoved();
    /// Marlin informed us about a bad SD Card.
    static void SDCardError();

    static const char* GetSDCardPrintFilename() { return sdPrintFilename; }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    static void PowerLossResume();
  #endif

  #if HAS_PID_HEATING
    static void PidTuning(const ExtUI::result_t rst);
  #endif

  static void SteppersStatusChanged(bool steppersEnabled);
  static void HomingDone();

  static void StartPrintFromSD(const char* const filename);
  static void SetStatusMessage(FSTR_P msg, const millis_t duration=DGUS_STATUS_EXPIRATION_MS);
  static void SetStatusMessage(const char* msg, const millis_t duration=DGUS_STATUS_EXPIRATION_MS);
  
  static DGUS_Screen GetCurrentScreen();
  static void HomeThenChangeScreen(DGUS_Screen screen);
  static void TriggerScreenChange(DGUS_Screen screen);
  static void TriggerTempScreenChange(DGUS_Screen screen, DGUS_Screen returnScreen);
  static void TriggerReturnScreen();
  static bool IsOnTempScreen(DGUS_Screen screen = DGUS_Screen::BOOT);
  static void TriggerFullUpdate();

  static void TriggerEEPROMSave();

  static bool IsPrinterIdle();

private:
  static const DGUS_Addr* FindScreenAddrList(DGUS_Screen screen);
  static bool CallScreenSetup(DGUS_Screen screen);

  static void MoveToScreen(DGUS_Screen screen, bool abort_wait=false);
  static bool SendScreenVPData(DGUS_Screen screen, bool complete_update);
  static bool RefreshVP(DGUS_Addr vpAddr);
};

extern DGUSScreenHandler dgus_screen_handler;