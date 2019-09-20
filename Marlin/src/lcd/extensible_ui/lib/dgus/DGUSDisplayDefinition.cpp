/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD)

#include "DGUSDisplayDefinition.h"
#include "DGUSDisplay.h"

#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"

#include "../../../ultralcd.h"

const uint16_t VPList_Boot[] PROGMEM = {
  VP_MARLIN_VERSION,
  0x0000
};

const uint16_t VPList_Main[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  0x0000
};

const uint16_t VPList_Temp[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E2_I, VP_T_E2_S,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  0x0000
};

const uint16_t VPList_Status[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded */
  #if HOTENDS >= 1
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E2_I, VP_T_E2_S,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  #if FAN_COUNT > 0
    VP_Fan_Percentage,
  #endif
  VP_XPos, VP_YPos, VP_ZPos,
  VP_Fan_Percentage,
  VP_Feedrate_Percentage,
  VP_PrintProgress_Percentage,
  0x0000
};

const uint16_t VPList_Status2[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded */
  #if HOTENDS >= 1
    VP_Flowrate_E1,
  #endif
  #if HOTENDS >= 2
    VP_Flowrate_E2,
  #endif
  VP_PrintProgress_Percentage,
  VP_PrintTime,
  0x0000
};

const uint16_t VPList_ManualMove[] PROGMEM = {
  VP_XPos, VP_YPos, VP_ZPos,
  0x0000
};

const uint16_t VPList_ManualExtrude[] PROGMEM = {
  VP_EPos,
  0x0000
};

const uint16_t VPList_FanAndFeedrate[] PROGMEM = {
  VP_Feedrate_Percentage, VP_Fan_Percentage,
  0x0000
};

const uint16_t VPList_SD_FlowRates[] PROGMEM = {
  VP_Flowrate_E1, VP_Flowrate_E2,
  0x0000
};

const uint16_t VPList_SDFileList[] PROGMEM = {
  VP_SD_FileName0, VP_SD_FileName1, VP_SD_FileName2, VP_SD_FileName3, VP_SD_FileName4,
  0x0000
};

const uint16_t VPList_SD_PrintManipulation[] PROGMEM = {
  VP_PrintProgress_Percentage, VP_PrintTime,
  0x0000
};

const struct VPMapping VPMap[] PROGMEM = {
  { DGUSLCD_SCREEN_BOOT, VPList_Boot },
  { DGUSLCD_SCREEN_MAIN, VPList_Main },
  { DGUSLCD_SCREEN_TEMPERATURE, VPList_Temp },
  { DGUSLCD_SCREEN_STATUS, VPList_Status },
  { DGUSLCD_SCREEN_STATUS2, VPList_Status2 },
  { DGUSLCD_SCREEN_MANUALMOVE, VPList_ManualMove },
  { DGUSLCD_SCREEN_MANUALEXTRUDE, VPList_ManualExtrude },
  { DGUSLCD_SCREEN_FANANDFEEDRATE, VPList_FanAndFeedrate },
  { DGUSLCD_SCREEN_FLOWRATES, VPList_SD_FlowRates },
  { DGUSLCD_SCREEN_SDPRINTMANIPULATION, VPList_SD_PrintManipulation },
  { DGUSLCD_SCREEN_SDFILELIST, VPList_SDFileList },
  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

// Helper to define a DGUS_VP_Variable for common use cases.
#define VPHELPER(VPADR, VPADRVAR, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=sizeof(VPADRVAR), \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

// Helper to define a DGUS_VP_Variable when the sizeo of the var cannot be determined automaticalyl (eg. a string)
#define VPHELPER_STR(VPADR, VPADRVAR, STRLEN, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=STRLEN, \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // Helper to detect touch events
  VPHELPER(VP_SCREENCHANGE, nullptr, DGUSScreenVariableHandler::ScreenChangeHook, nullptr),
  VPHELPER(VP_SCREENCHANGE_ASK, nullptr, DGUSScreenVariableHandler::ScreenChangeHookIfIdle, nullptr),
  VPHELPER(VP_SCREENCHANGE_WHENSD, nullptr, DGUSScreenVariableHandler::ScreenChangeHookIfSD, nullptr),
  VPHELPER(VP_CONFIRMED, nullptr, DGUSScreenVariableHandler::ScreenConfirmedOK, nullptr),

  VPHELPER(VP_TEMP_ALL_OFF, nullptr, &DGUSScreenVariableHandler::HandleAllHeatersOff, nullptr),

  VPHELPER(VP_MOVE_X, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
  VPHELPER(VP_MOVE_Y, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
  VPHELPER(VP_MOVE_Z, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
  VPHELPER(VP_HOME_ALL, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),

  { .VP = VP_MARLIN_VERSION, .memadr = (void*)MarlinVersion, .size = VP_MARLIN_VERSION_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  // M117 LCD String (We don't need the string in memory but "just" push it to the display on demand, hence the nullptr
  { .VP = VP_M117, .memadr = nullptr, .size = VP_M117_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay },

  // Temperature Data
  #if HOTENDS >= 1
    VPHELPER(VP_T_E1_Is, &thermalManager.temp_hotend[0].celsius, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_E1_Set, &thermalManager.temp_hotend[0].target, DGUSScreenVariableHandler::HandleTemperatureChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_Flowrate_E1, nullptr, DGUSScreenVariableHandler::HandleFlowRateChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_EPos, &destination[3], nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),
    VPHELPER(VP_MOVE_E1, nullptr, &DGUSScreenVariableHandler::HandleManualExtrude, nullptr),
  #endif
  #if HOTENDS >= 2
    VPHELPER(VP_T_E2_I, &thermalManager.temp_hotend[1].celsius, nullptr, DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_E2_S, &thermalManager.temp_hotend[1].target, DGUSScreenVariableHandler::HandleTemperatureChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_Flowrate_E2, nullptr, DGUSScreenVariableHandler::HandleFlowRateChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_MOVE_E2, nullptr, &DGUSScreenVariableHandler::HandleManualExtrude, nullptr),
  #endif
  #if HOTENDS >= 3
    #error More than 2 Hotends currently not implemented on the Display UI design.
  #endif
  #if HAS_HEATED_BED
    VPHELPER(VP_T_Bed_Is, &thermalManager.temp_bed.celsius, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_Bed_Set, &thermalManager.temp_bed.target, DGUSScreenVariableHandler::HandleTemperatureChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
  #endif

  // Fan Data.
  #if FAN_COUNT > 0
    VPHELPER(VP_Fan_Percentage, &thermalManager.fan_speed[0], DGUSScreenVariableHandler::DGUSLCD_PercentageToUint8, &DGUSScreenVariableHandler::DGUSLCD_SendPercentageToDisplay),
  #endif

  // Feedrate.
  VPHELPER(VP_Feedrate_Percentage, &feedrate_percentage, DGUSScreenVariableHandler::DGUSLCD_SetValueDirectly<int16_t>, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay ),

  // Position Data.
  VPHELPER(VP_XPos, &current_position[0], nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),
  VPHELPER(VP_YPos, &current_position[1], nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),
  VPHELPER(VP_ZPos, &current_position[2], nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),

  // Print Progress.
  VPHELPER(VP_PrintProgress_Percentage, &ui.progress_bar_percent, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay ),

  // Print Time
  VPHELPER_STR(VP_PrintTime, nullptr, VP_PrintTime_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendPrintTimeToDisplay ),

  // SDCard File listing.
  #if ENABLED(SDSUPPORT)
    VPHELPER(VP_SD_ScrollEvent, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_ScrollFilelist, nullptr),
    VPHELPER(VP_SD_FileSelected, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_FileSelected, nullptr),
    VPHELPER(VP_SD_FileSelectConfirm, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_StartPrint, nullptr),
    VPHELPER_STR(VP_SD_FileName0,  nullptr, VP_SD_FileName_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_SendFilename ),
    VPHELPER_STR(VP_SD_FileName1,  nullptr, VP_SD_FileName_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_SendFilename ),
    VPHELPER_STR(VP_SD_FileName2,  nullptr, VP_SD_FileName_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_SendFilename ),
    VPHELPER_STR(VP_SD_FileName3,  nullptr, VP_SD_FileName_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_SendFilename ),
    VPHELPER_STR(VP_SD_FileName4,  nullptr, VP_SD_FileName_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_SendFilename ),
    VPHELPER(VP_SD_ResumePauseAbort, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_ResumePauseAbort, nullptr),
    VPHELPER(VP_SD_AbortPrintConfirmed, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_ReallyAbort, nullptr),
  #endif

  // Messages for the User, shared by the popup and the kill screen. They cant be autouploaded as we do not buffer content.
  { .VP = VP_MSGSTR1, .memadr = nullptr, .size = VP_MSGSTR1_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR2, .memadr = nullptr, .size = VP_MSGSTR2_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR3, .memadr = nullptr, .size = VP_MSGSTR3_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR4, .memadr = nullptr, .size = VP_MSGSTR4_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD
