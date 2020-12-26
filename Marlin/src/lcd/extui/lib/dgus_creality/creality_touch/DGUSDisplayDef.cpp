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

/* DGUS implementation written by Sebastiaan Dammann in 2020 for Marlin */

#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/motion.h"
#include "../../../../../module/planner.h"

#include "../../../../../feature/caselight.h"

#include "../../../ui_api.h"
#include "../../lcd/marlinui.h"

#include "PageHandlers.h"

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  uint16_t distanceToMove = 10;
#endif
using namespace ExtUI;


const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

// ----- Which variables to auto-update on which screens
const uint16_t VPList_None[] PROGMEM = {
  0x0000
};

const uint16_t VPList_DialogStop[] PROGMEM = {
  0x0000
};

const uint16_t VPList_Main[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif
  0x0000
};

const uint16_t VPList_SDFileList[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif

  VP_SD_FileName0,
  VP_SD_FileName1,
  VP_SD_FileName2,
  VP_SD_FileName3,
  VP_SD_FileName4,
  VP_SD_FileName5,

  0x0000
};

const uint16_t VPList_Control[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif

  VP_LED_TOGGLE,
  VP_MUTE_ICON,
  VP_STANDBY_BACKLIGHT_ICON,
  VP_SCREEN_BACKLIGHT_STANDBY,

  0x0000
};

const uint16_t VPList_Feed[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_FEED_AMOUNT,

  0x0000
};

const uint16_t VPList_Temp[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif

  VP_FAN_TOGGLE,

  0x0000
};


const uint16_t VPList_PreheatPLASettings[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_PREHEAT_PLA_HOTEND_TEMP,
  VP_PREHEAT_PLA_BED_TEMP,

  0x0000
};

const uint16_t VPList_PreheatABSSettings[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_PREHEAT_ABS_HOTEND_TEMP,
  VP_PREHEAT_ABS_BED_TEMP,

  0x0000
};


const uint16_t VPList_PrintPausingError[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_PrintProgress_Percentage,
  VP_PrintTimeProgressBar,
  VP_PrintTime,

  0x0000
};

const uint16_t VPList_PrintScreen[] PROGMEM = {
  VP_PrintTime,

  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif

  VP_X_POSITION, VP_Y_POSITION, VP_Z_POSITION,
  VP_X_POSITION_SP, VP_Y_POSITION_SP, VP_Z_POSITION_SP,

  VP_Z_OFFSET,
  VP_Flowrate_E0,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_PrintProgress_Percentage,
  VP_PrintTimeProgressBar,
  VP_PrintTime,

  0x0000
};

const uint16_t VPList_Leveling[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_MESH_LEVEL_TEMP,

  0x0000
};

const uint16_t VPList_ZOffsetLevel[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  0x0000
};

const uint16_t VPList_TuneScreen[] PROGMEM = {
  VP_PrintTime,

  VP_Flowrate_E0,

  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_LED_TOGGLE,
  VP_FAN_TOGGLE,


  0x0000
};

const uint16_t VPList_Prepare[] PROGMEM = {
  VP_PrintTime,

  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_STEPPERS,

  0x0000
};

const uint16_t VPList_Info[] PROGMEM = {
  VP_PrintTime,

  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,// VP_E0_STATUS,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,// VP_BED_STATUS,
  #endif
  VP_Z_OFFSET,
  //VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_PRINTER_BEDSIZE,
  VP_MARLIN_WEBSITE,
  VP_MARLIN_VERSION,

  0x0000
};

// Toggle button handler
void DGUSCrealityDisplay_HandleToggleButton(DGUS_VP_Variable &var, void *val_ptr) {
  switch (*(uint16_t*)var.memadr) {
    case ICON_TOGGLE_ON:
      *((bool*)var.memadr) = true;
      break;

    case ICON_TOGGLE_OFF:
      *((bool*)var.memadr) = true;
      break;
  }
}

void DGUSCrealityDisplay_SendToggleButton(DGUS_VP_Variable &var) {
  dgusdisplay.WriteVariable(var.VP, *(bool*)var.memadr ? ICON_TOGGLE_ON : ICON_TOGGLE_OFF);
}

// -- Mapping from screen to variable list
const struct VPMapping VPMap[] PROGMEM = {
  { DGUSLCD_SCREEN_BOOT, VPList_None },
  { DGUSLCD_SCREEN_MAIN, VPList_Main },

  { DGUSLCD_SCREEN_SDFILELIST, VPList_SDFileList },

  { DGUSLCD_SCREEN_FILAMENTRUNOUT1, VPList_PrintPausingError },
  { DGUSLCD_SCREEN_FILAMENTRUNOUT2, VPList_PrintPausingError },

  { DGUSLCD_SCREEN_PRINT_FINISH, VPList_PrintScreen },
  { DGUSLCD_SCREEN_PRINT_RUNNING, VPList_PrintScreen },
  { DGUSLCD_SCREEN_PRINT_PAUSED, VPList_PrintScreen },

  { DGUSLCD_SCREEN_TUNING, VPList_TuneScreen },
  { DGUSLCD_SCREEN_PREPARE, VPList_Prepare },

  { DGUSLCD_SCREEN_INFO, VPList_Info },

  { DGUSLCD_SCREEN_MOVE1MM, VPList_PrintScreen },
  { DGUSLCD_SCREEN_MOVE10MM, VPList_PrintScreen },
  { DGUSLCD_SCREEN_MOVE01MM, VPList_PrintScreen },

  { DGUSLCD_SCREEN_FEED, VPList_Feed },
  { DGUSLCD_SCREEN_CONTROL, VPList_Control },

  { DGUSLCD_SCREEN_TEMP, VPList_Temp },
  { DGUSLCD_SCREEN_TEMP_PLA, VPList_PreheatPLASettings },
  { DGUSLCD_SCREEN_TEMP_ABS, VPList_PreheatABSSettings },

  { DGUSLCD_SCREEN_INFO, VPList_PrintScreen },
  { DGUSLCD_SCREEN_ZOFFSET_LEVEL, VPList_ZOffsetLevel },
  { DGUSLCD_SCREEN_LEVELING, VPList_Leveling },

  { DGUSLCD_SCREEN_POWER_LOSS, VPList_None },
  { DGUSLCD_SCREEN_THERMAL_RUNAWAY, VPList_None },
  { DGUSLCD_SCREEN_HEATING_FAILED, VPList_None },
  { DGUSLCD_SCREEN_THERMISTOR_ERROR, VPList_None },

  { DGUSLCD_SCREEN_AUTOHOME, VPList_PrintScreen },

  { DGUSLCD_SCREEN_DIALOG_PAUSE, VPList_None },
  { DGUSLCD_SCREEN_DIALOG_STOP, VPList_DialogStop },

  { DGUSLCD_SCREEN_CONFIRM, VPList_None },
  { DGUSLCD_SCREEN_POPUP, VPList_None },


  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

// Helper to define a DGUS_VP_Variable for common use cases.
#define VPHELPER(VPADR, VPADRVAR, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=sizeof(VPADRVAR), \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

// Helper to define a DGUS_VP_Variable when the sizeo of the var cannot be determined automaticalyl (eg. a string)
#define VPHELPER_STR(VPADR, VPADRVAR, STRLEN, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=STRLEN, \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // TODO:

  #if HOTENDS >= 1
    VPHELPER(VP_Flowrate_E0, &planner.flow_percentage[ExtUI::extruder_t::E0], ScreenHandler.HandleFlowRateChanged, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_T_E0_Is, &thermalManager.temp_hotend[0].celsius, nullptr, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<0>),
    VPHELPER(VP_T_E0_Set, &thermalManager.temp_hotend[0].target, ScreenHandler.HandleTemperatureChanged, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  #endif

  #if HAS_HEATED_BED
    VPHELPER(VP_T_Bed_Is, &thermalManager.temp_bed.celsius, nullptr, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<0>),
    VPHELPER(VP_T_Bed_Set, &thermalManager.temp_bed.target, ScreenHandler.HandleTemperatureChanged, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  #endif

  VPHELPER(VP_MESH_LEVEL_TEMP, &thermalManager.temp_hotend[0].target, nullptr, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_MESH_LEVEL_STATUS, nullptr, nullptr, nullptr),

  // Feedrate
  VPHELPER(VP_Feedrate_Percentage, &feedrate_percentage, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  VPHELPER(VP_PrintProgress_Percentage, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintProgressToDisplay),
  VPHELPER(VP_PrintTimeProgressBar, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintProgressToDisplay),

  // Preheat settings
  #ifdef PREHEAT_1_LABEL
  VPHELPER(VP_PREHEAT_PLA_HOTEND_TEMP, &ui.material_preset[0].hotend_temp, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_PREHEAT_PLA_BED_TEMP, &ui.material_preset[0].bed_temp, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  #endif

  #ifdef PREHEAT_2_LABEL
  VPHELPER(VP_PREHEAT_ABS_HOTEND_TEMP, &ui.material_preset[1].hotend_temp, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_PREHEAT_ABS_BED_TEMP, &ui.material_preset[1].bed_temp, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  #endif

  // About info
  VPHELPER(VP_MARLIN_WEBSITE, nullptr, nullptr, ScreenHandler.DGUSLCD_SendAboutFirmwareWebsite),
  VPHELPER(VP_MARLIN_VERSION, nullptr, nullptr, ScreenHandler.DGUSLCD_SendAboutFirmwareVersion),
  VPHELPER(VP_PRINTER_BEDSIZE, nullptr, nullptr, ScreenHandler.DGUSLCD_SendAboutPrintSize),

  // Position Data
  VPHELPER(VP_X_POSITION, &current_position.x, ScreenHandler.HandlePositionChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Y_POSITION, &current_position.y, ScreenHandler.HandlePositionChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Z_POSITION, &current_position.z, ScreenHandler.HandlePositionChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  VPHELPER(VP_X_POSITION_SP, nullptr, nullptr, ScreenHandler.SendAxisTrustValue<X_AXIS>),
  VPHELPER(VP_Y_POSITION_SP, nullptr, nullptr, ScreenHandler.SendAxisTrustValue<Y_AXIS>),
  VPHELPER(VP_Z_POSITION_SP, nullptr, nullptr, ScreenHandler.SendAxisTrustValue<Z_AXIS>),

  VPHELPER(VP_Z_OFFSET, &probe.offset.z, ScreenHandler.HandleZoffsetChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<2>),

  VPHELPER(VP_FAN_TOGGLE, &thermalManager.fan_speed[0], nullptr, ScreenHandler.DGUSLCD_SendFanStatusToDisplay),

  #if ENABLED(POWER_LOSS_RECOVERY)
    VPHELPER(VP_POWER_LOSS_RECOVERY, nullptr, &ScreenHandler.HandlePowerLossRecovery, nullptr),
  #endif

  VPHELPER_STR(VP_PrintTime, nullptr, VP_PrintTime_LEN, nullptr, ScreenHandler.DGUSLCD_SendPrintTimeToDisplay),
  VPHELPER(VP_SCREENCHANGE, nullptr, ScreenHandler.ScreenChangeHook, nullptr),
  VPHELPER(VP_CONFIRMED, nullptr, ScreenHandler.ScreenConfirmedOK, nullptr),

  // Feed
  VPHELPER(VP_FEED_AMOUNT, &ScreenHandler.feed_amount, ScreenHandler.HandleFeedAmountChanged, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  // Creality has the same button ID mapped all over the place, so let the generic handler figure it out
  VPHELPER(VP_BUTTON_MAINENTERKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_ADJUSTENTERKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_PREPAREENTERKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_RESUMEPRINTKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_SELECTFILEKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_STARTPRINTKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_STOPPRINTKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_PAUSEPRINTKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_COOLDOWN, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_TEMPCONTROL, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_BEDLEVELKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_HEATLOADSTARTKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),
  VPHELPER(VP_BUTTON_MOVEKEY, nullptr, DGUSCrealityDisplay_HandleReturnKeyEvent, nullptr),

  // File listing
  VPHELPER(VP_SD_ScrollEvent, nullptr, ScreenHandler.DGUSLCD_SD_ScrollFilelist, nullptr),
  VPHELPER(VP_SD_FileSelected, nullptr, ScreenHandler.DGUSLCD_SD_FileSelected, nullptr),
  VPHELPER(VP_SD_FileSelectConfirm, nullptr, ScreenHandler.DGUSLCD_SD_StartPrint, nullptr),
  VPHELPER_STR(VP_SD_FileName0,  nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName1,  nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName2,  nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName3,  nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName4,  nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),
  VPHELPER_STR(VP_SD_FileName5,  nullptr, VP_SD_FileName_LEN, nullptr, ScreenHandler.DGUSLCD_SD_SendFilename),

  // Additional buttons
  VPHELPER(VP_MUTE_TOGGLE, nullptr, ScreenHandler.HandleToggleTouchScreenMute, nullptr),
  VPHELPER(VP_STANDBY_BACKLIGHT_TOGGLE, nullptr, ScreenHandler.HandleToggleTouchScreenStandbySetting, nullptr),

  // Additional settings
  VPHELPER(VP_SCREEN_BACKLIGHT_STANDBY, &ScreenHandler.Settings.standby_screen_brightness, ScreenHandler.HandleTouchScreenStandbyBrightnessSetting, ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  // Icons
  VPHELPER(VP_STEPPERS, &ScreenHandler.are_steppers_enabled, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_TOGGLE_OFF, ICON_TOGGLE_ON>)),
  VPHELPER(VP_LED_TOGGLE, &caselight.on, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_LED_TOGGLE_ON, ICON_LED_TOGGLE_OFF>)),
  VPHELPER(VP_STANDBY_BACKLIGHT_ICON, &ScreenHandler.Settings.display_standby, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_STANDBY_TOGGLE_ON, ICON_STANDBY_TOGGLE_OFF>)),
  VPHELPER(VP_MUTE_ICON, &ScreenHandler.Settings.display_sound, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_SOUND_TOGGLE_OFF, ICON_SOUND_TOGGLE_ON>)),

  // M117 LCD String (We don't need the string in memory but "just" push it to the display on demand, hence the nullptr
  { .VP = VP_M117, .memadr = nullptr, .size = VP_M117_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&ScreenHandler.DGUSLCD_SendStringToDisplay },

 // Messages for the User, shared by the popup and the kill screen. They cant be autouploaded as we do not buffer content.
  { .VP = VP_MSGSTR1, .memadr = nullptr, .size = VP_MSGSTR1_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR2, .memadr = nullptr, .size = VP_MSGSTR2_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR3, .memadr = nullptr, .size = VP_MSGSTR3_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },
  //{ .VP = VP_MSGSTR4, .memadr = nullptr, .size = VP_MSGSTR4_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_ORIGIN
