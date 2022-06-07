/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../DGUSScreenHandlerBase.h"

enum DGUSLCD_Screens : uint8_t;

class DGUSScreenHandlerMKS : public DGUSScreenHandler {
public:
  DGUSScreenHandlerMKS() = default;

  #if 0
  static void sendinfoscreen_ch(const uint16_t *line1, const uint16_t *line2, const uint16_t *line3, const uint16_t *line4);
  static void sendinfoscreen_en(PGM_P const line1, PGM_P const line2, PGM_P const line3, PGM_P const line4);
  static void sendinfoscreen(const void *line1, const void *line2, const void *line3, const void *line4, uint16_t language);
  #endif

  static void ScreenBackChange(DGUS_VP_Variable &var, void *val_ptr);

  static void EEPROM_CTRL(DGUS_VP_Variable &var, void *val_ptr);
  static void LanguageChange(DGUS_VP_Variable &var, void *val_ptr);
  static void GetOffsetValue(DGUS_VP_Variable &var, void *val_ptr);
  static void Level_Ctrl(DGUS_VP_Variable &var, void *val_ptr);
  static void MeshLevel(DGUS_VP_Variable &var, void *val_ptr);
  static void MeshLevelDistanceConfig(DGUS_VP_Variable &var, void *val_ptr);
  static void ManualAssistLeveling(DGUS_VP_Variable &var, void *val_ptr);
  static void ZoffsetConfirm(DGUS_VP_Variable &var, void *val_ptr);
  static void Z_offset_select(DGUS_VP_Variable &var, void *val_ptr);
  static void GetManualMovestep(DGUS_VP_Variable &var, void *val_ptr);
  static void GetZoffsetDistance(DGUS_VP_Variable &var, void *val_ptr);
  static void GetMinExtrudeTemp(DGUS_VP_Variable &var, void *val_ptr);
  static void GetParkPos(DGUS_VP_Variable &var, void *val_ptr);
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    static void HandleGetExMinTemp(DGUS_VP_Variable &var, void *val_ptr);
  #endif
  static void DGUS_LanguageDisplay(uint8_t var);
  static void TMC_ChangeConfig(DGUS_VP_Variable &var, void *val_ptr);
  static void GetTurnOffCtrl(DGUS_VP_Variable &var, void *val_ptr);
  static void LanguagePInit();
  static void DGUS_Runout_Idle();
  static void DGUS_RunoutInit();
  static void DGUS_ExtrudeLoadInit();
  static void LCD_BLK_Adjust(DGUS_VP_Variable &var, void *val_ptr);
  static void SD_FileBack(DGUS_VP_Variable &var, void *val_ptr);

  static void HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleMaxSpeedChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleExtruderMaxSpeedChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleAccChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleMaxAccChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleExtruderAccChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleChangeLevelPoint(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleTravelAccChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleFeedRateMinChange(DGUS_VP_Variable &var, void *val_ptr);
  static void HandleMin_T_F(DGUS_VP_Variable &var, void *val_ptr);

  #if HAS_PID_HEATING
    static void FilamentLoadUnload(DGUS_VP_Variable &var, void *val_ptr, const int filamentDir);
    static void FilamentLoad(DGUS_VP_Variable &var, void *val_ptr);
    static void FilamentUnLoad(DGUS_VP_Variable &var, void *val_ptr);
    static void GetManualFilament(DGUS_VP_Variable &var, void *val_ptr);
    static void GetManualFilamentSpeed(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  #if ENABLED(SDSUPPORT)
    // Marlin informed us about SD print completion.
    static void SDPrintingFinished();
  #else
    static void PrintReturn(DGUS_VP_Variable &var, void *val_ptr);
  #endif

  static void DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendBabyStepToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendFanToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendGbkToDisplay(DGUS_VP_Variable &var);
  static void DGUSLCD_SendStringToDisplay_Language(DGUS_VP_Variable &var);
  static void DGUSLCD_SendTMCStepValue(DGUS_VP_Variable &var);

  static void DGUSLCD_SetUint8(DGUS_VP_Variable &var, void *val_ptr);

  static bool loop();
};

enum MKS_Choose : uint8_t { MKS_Language_Choose, MKS_Language_NoChoose };
enum MKS_Language : uint8_t { MKS_SimpleChinese, MKS_English };

extern MKS_Language mks_language_index;
extern bool DGUSAutoTurnOff;

#if ENABLED(POWER_LOSS_RECOVERY)
  #define PLR_SCREEN_RECOVER MKSLCD_SCREEN_PRINT
  #define PLR_SCREEN_CANCEL MKSLCD_SCREEN_HOME
#endif

typedef DGUSScreenHandlerMKS DGUSScreenHandlerClass;
