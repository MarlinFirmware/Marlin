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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* DGUS VPs changed by George Fu in 2019 for Marlin */

#include "../../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_HIPRECY)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"

#include "../../../../../module/temperature.h"
#include "../../../../../module/motion.h"
#include "../../../../../module/planner.h"

#include "../../../ui_api.h"
#include "../../../../ultralcd.h"

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  uint16_t distanceToMove = 0.1;
#endif

const uint16_t VPList_Boot[] PROGMEM = {
  VP_MARLIN_VERSION,
  0x0000
};

const uint16_t VPList_Main[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded. */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set, VP_E0_STATUS,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set, VP_BED_STATUS,
  #endif
  #if FAN_COUNT > 0
    VP_Fan0_Percentage, VP_FAN0_STATUS,
  #endif
  VP_XPos, VP_YPos, VP_ZPos,
  VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    VP_PrintProgress_Percentage,
  #endif
  0x0000
};

const uint16_t VPList_Temp[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  0x0000
};

const uint16_t VPList_Status[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded */
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  #if FAN_COUNT > 0
    VP_Fan0_Percentage,
  #endif
  VP_XPos, VP_YPos, VP_ZPos,
  VP_Fan0_Percentage,
  VP_Feedrate_Percentage,
  VP_PrintProgress_Percentage,
  0x0000
};

const uint16_t VPList_Status2[] PROGMEM = {
  /* VP_M117, for completeness, but it cannot be auto-uploaded */
  #if HOTENDS >= 1
    VP_Flowrate_E0,
  #endif
  #if HOTENDS >= 2
    VP_Flowrate_E1,
  #endif
  VP_PrintProgress_Percentage,
  VP_PrintTime,
  0x0000
};

const uint16_t VPList_Preheat[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  0x0000
};

const uint16_t VPList_ManualMove[] PROGMEM = {
  VP_XPos, VP_YPos, VP_ZPos,
  0x0000
};

const uint16_t VPList_ManualExtrude[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  VP_EPos,
  0x0000
};

const uint16_t VPList_FanAndFeedrate[] PROGMEM = {
  VP_Feedrate_Percentage, VP_Fan0_Percentage,
  0x0000
};

const uint16_t VPList_SD_FlowRates[] PROGMEM = {
  VP_Flowrate_E0, VP_Flowrate_E1,
  0x0000
};

const uint16_t VPList_Filament_heating[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
    VP_E0_FILAMENT_LOAD_UNLOAD,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  0x0000
};

const uint16_t VPList_Filament_load_unload[] PROGMEM = {
  #if HOTENDS >= 1
    VP_E0_FILAMENT_LOAD_UNLOAD,
  #endif
  #if HOTENDS >= 2
    VP_E1_FILAMENT_LOAD_UNLOAD,
  #endif
  0x0000
};

const uint16_t VPList_SDFileList[] PROGMEM = {
  VP_SD_FileName0, VP_SD_FileName1, VP_SD_FileName2, VP_SD_FileName3, VP_SD_FileName4,
  0x0000
};

const uint16_t VPList_SD_PrintManipulation[] PROGMEM = {
  VP_PrintProgress_Percentage, VP_PrintTime,
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  #if FAN_COUNT > 0
    VP_Fan0_Percentage,
    #if FAN_COUNT > 1
      VP_Fan1_Percentage,
    #endif
  #endif
  VP_Flowrate_E0,
  0x0000
};

const uint16_t VPList_SDPrintTune[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  VP_Feedrate_Percentage,
  #if FAN_COUNT > 0
    VP_Fan0_Percentage,
  #endif
  VP_Flowrate_E0,
  VP_SD_Print_ProbeOffsetZ,
  0x0000
};

const uint16_t VPList_StepPerMM[] PROGMEM = {
  VP_X_STEP_PER_MM,
  VP_Y_STEP_PER_MM,
  VP_Z_STEP_PER_MM,
  #if HOTENDS >= 1
    VP_E0_STEP_PER_MM,
  #endif
  #if HOTENDS >= 2
    VP_E1_STEP_PER_MM,
  #endif
  0x0000
};

const uint16_t VPList_PIDE0[] PROGMEM = {
  #if ENABLED(PIDTEMP)
    VP_E0_PID_P,
    VP_E0_PID_I,
    VP_E0_PID_D,
  #endif
  0x0000
};

const uint16_t VPList_PIDBED[] PROGMEM = {
  #if ENABLED(PIDTEMP)
    VP_BED_PID_P,
    VP_BED_PID_I,
    VP_BED_PID_D,
  #endif
  0x0000
};

const uint16_t VPList_Infos[] PROGMEM = {
  VP_MARLIN_VERSION,
  VP_PrintTime,
  #if ENABLED(PRINTCOUNTER)
    VP_PrintAccTime,
    VP_PrintsTotal,
  #endif
  0x0000
};

const uint16_t VPList_PIDTuningWaiting[] PROGMEM = {
  VP_WAITING_STATUS,
  0x0000
};

const uint16_t VPList_FLCPreheat[] PROGMEM = {
  #if HOTENDS >= 1
    VP_T_E0_Is, VP_T_E0_Set,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  0x0000
};

const uint16_t VPList_FLCPrinting[] PROGMEM = {
  #if HOTENDS >= 1
    VP_SD_Print_ProbeOffsetZ,
  #endif
  0x0000
};

const uint16_t VPList_Z_Offset[] PROGMEM = {
  #if HOTENDS >= 1
    VP_SD_Print_ProbeOffsetZ,
  #endif
  0x0000
};

const struct VPMapping VPMap[] PROGMEM = {
  { DGUSLCD_SCREEN_BOOT, VPList_Boot },
  { DGUSLCD_SCREEN_MAIN, VPList_Main },
  { DGUSLCD_SCREEN_TEMPERATURE, VPList_Temp },
  { DGUSLCD_SCREEN_STATUS, VPList_Status },
  { DGUSLCD_SCREEN_STATUS2, VPList_Status2 },
  { DGUSLCD_SCREEN_PREHEAT, VPList_Preheat },
  { DGUSLCD_SCREEN_MANUALMOVE, VPList_ManualMove },
  { DGUSLCD_SCREEN_Z_OFFSET, VPList_Z_Offset },
  { DGUSLCD_SCREEN_MANUALEXTRUDE, VPList_ManualExtrude },
  { DGUSLCD_SCREEN_FILAMENT_HEATING, VPList_Filament_heating },
  { DGUSLCD_SCREEN_FILAMENT_LOADING, VPList_Filament_load_unload },
  { DGUSLCD_SCREEN_FILAMENT_UNLOADING, VPList_Filament_load_unload },
  { DGUSLCD_SCREEN_SDPRINTMANIPULATION, VPList_SD_PrintManipulation },
  { DGUSLCD_SCREEN_SDFILELIST, VPList_SDFileList },
  { DGUSLCD_SCREEN_SDPRINTTUNE, VPList_SDPrintTune },
  { DGUSLCD_SCREEN_WAITING, VPList_PIDTuningWaiting },
  { DGUSLCD_SCREEN_FLC_PREHEAT, VPList_FLCPreheat },
  { DGUSLCD_SCREEN_FLC_PRINTING, VPList_FLCPrinting },
  { DGUSLCD_SCREEN_STEPPERMM, VPList_StepPerMM },
  { DGUSLCD_SCREEN_PID_E, VPList_PIDE0 },
  { DGUSLCD_SCREEN_PID_BED, VPList_PIDBED },
  { DGUSLCD_SCREEN_INFOS, VPList_Infos },
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
  #if ENABLED(SDSUPPORT)
    VPHELPER(VP_SCREENCHANGE_WHENSD, nullptr, DGUSScreenVariableHandler::ScreenChangeHookIfSD, nullptr),
  #endif
  VPHELPER(VP_CONFIRMED, nullptr, DGUSScreenVariableHandler::ScreenConfirmedOK, nullptr),

  VPHELPER(VP_TEMP_ALL_OFF, nullptr, &DGUSScreenVariableHandler::HandleAllHeatersOff, nullptr),

  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    VPHELPER(VP_MOVE_OPTION, &distanceToMove, &DGUSScreenVariableHandler::HandleManualMoveOption, nullptr),
  #endif
  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    VPHELPER(VP_MOVE_X, &distanceToMove, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
    VPHELPER(VP_MOVE_Y, &distanceToMove, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
    VPHELPER(VP_MOVE_Z, &distanceToMove, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
    VPHELPER(VP_HOME_ALL, &distanceToMove, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
  #else
    VPHELPER(VP_MOVE_X, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
    VPHELPER(VP_MOVE_Y, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
    VPHELPER(VP_MOVE_Z, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
    VPHELPER(VP_HOME_ALL, nullptr, &DGUSScreenVariableHandler::HandleManualMove, nullptr),
  #endif
  VPHELPER(VP_MOTOR_LOCK_UNLOK, nullptr, &DGUSScreenVariableHandler::HandleMotorLockUnlock, nullptr),
  #if ENABLED(POWER_LOSS_RECOVERY)
    VPHELPER(VP_POWER_LOSS_RECOVERY, nullptr, &DGUSScreenVariableHandler::HandlePowerLossRecovery, nullptr),
  #endif
  VPHELPER(VP_SETTINGS, nullptr, &DGUSScreenVariableHandler::HandleSettings, nullptr),
  #if ENABLED(SINGLE_Z_CALIBRATION)
    VPHELPER(VP_Z_CALIBRATE, nullptr, &DGUSScreenVariableHandler::HandleZCalibration, nullptr),
  #endif
  #if ENABLED(FIRST_LAYER_CAL)
    VPHELPER(VP_Z_FIRST_LAYER_CAL, nullptr, &DGUSScreenVariableHandler::HandleFirstLayerCal, nullptr),
  #endif

  { .VP = VP_MARLIN_VERSION, .memadr = (void*)MarlinVersion, .size = VP_MARLIN_VERSION_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  // M117 LCD String (We don't need the string in memory but "just" push it to the display on demand, hence the nullptr
  { .VP = VP_M117, .memadr = nullptr, .size = VP_M117_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplay },

  // Temperature Data
  #if HOTENDS >= 1
    VPHELPER(VP_T_E0_Is, &thermalManager.temp_hotend[0].celsius, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_E0_Set, &thermalManager.temp_hotend[0].target, DGUSScreenVariableHandler::HandleTemperatureChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_Flowrate_E0, &planner.flow_percentage[ExtUI::extruder_t::E0], DGUSScreenVariableHandler::HandleFlowRateChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_EPos, &destination.e, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),
    VPHELPER(VP_MOVE_E0, nullptr, &DGUSScreenVariableHandler::HandleManualExtrude, nullptr),
    VPHELPER(VP_E0_CONTROL, &thermalManager.temp_hotend[0].target, &DGUSScreenVariableHandler::HandleHeaterControl, nullptr),
    VPHELPER(VP_E0_STATUS, &thermalManager.temp_hotend[0].target, nullptr, &DGUSScreenVariableHandler::DGUSLCD_SendHeaterStatusToDisplay),
    #if ENABLED(DGUS_PREHEAT_UI)
      VPHELPER(VP_E0_BED_PREHEAT, nullptr, &DGUSScreenVariableHandler::HandlePreheat, nullptr),
    #endif
    #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
      VPHELPER(VP_E0_FILAMENT_LOAD_UNLOAD, nullptr, &DGUSScreenVariableHandler::HandleFilamentOption, &DGUSScreenVariableHandler::HandleFilamentLoadUnload),
    #endif
    #if ENABLED(PIDTEMP)
      VPHELPER(VP_E0_PID_P, &thermalManager.temp_hotend[0].pid.Kp, DGUSScreenVariableHandler::HandleTemperaturePIDChanged, DGUSScreenVariableHandler::DGUSLCD_SendTemperaturePID),
      VPHELPER(VP_E0_PID_I, &thermalManager.temp_hotend[0].pid.Ki, DGUSScreenVariableHandler::HandleTemperaturePIDChanged, DGUSScreenVariableHandler::DGUSLCD_SendTemperaturePID),
      VPHELPER(VP_E0_PID_D, &thermalManager.temp_hotend[0].pid.Kd, DGUSScreenVariableHandler::HandleTemperaturePIDChanged, DGUSScreenVariableHandler::DGUSLCD_SendTemperaturePID),
      VPHELPER(VP_PID_AUTOTUNE_E0, nullptr, &DGUSScreenVariableHandler::HandlePIDAutotune, nullptr),
    #endif
  #endif
  #if HOTENDS >= 2
    VPHELPER(VP_T_E1_Is, &thermalManager.temp_hotend[1].celsius, nullptr, DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_E1_Set, &thermalManager.temp_hotend[1].target, DGUSScreenVariableHandler::HandleTemperatureChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_Flowrate_E1, nullptr, DGUSScreenVariableHandler::HandleFlowRateChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_MOVE_E1, nullptr, &DGUSScreenVariableHandler::HandleManualExtrude, nullptr),
    VPHELPER(VP_E1_CONTROL, &thermalManager.temp_hotend[1].target, &DGUSScreenVariableHandler::HandleHeaterControl, nullptr),
    VPHELPER(VP_E1_STATUS, &thermalManager.temp_hotend[1].target, nullptr, &DGUSScreenVariableHandler::DGUSLCD_SendHeaterStatusToDisplay),
  #endif
  #if HAS_HEATED_BED
    VPHELPER(VP_T_Bed_Is, &thermalManager.temp_bed.celsius, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<0>),
    VPHELPER(VP_T_Bed_Set, &thermalManager.temp_bed.target, DGUSScreenVariableHandler::HandleTemperatureChanged, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay),
    VPHELPER(VP_BED_CONTROL, &thermalManager.temp_bed.target, &DGUSScreenVariableHandler::HandleHeaterControl, nullptr),
    VPHELPER(VP_BED_STATUS, &thermalManager.temp_bed.target, nullptr, &DGUSScreenVariableHandler::DGUSLCD_SendHeaterStatusToDisplay),
    #if ENABLED(PIDTEMP)
      VPHELPER(VP_BED_PID_P, &thermalManager.temp_bed.pid.Kp, DGUSScreenVariableHandler::HandleTemperaturePIDChanged, DGUSScreenVariableHandler::DGUSLCD_SendTemperaturePID),
      VPHELPER(VP_BED_PID_I, &thermalManager.temp_bed.pid.Ki, DGUSScreenVariableHandler::HandleTemperaturePIDChanged, DGUSScreenVariableHandler::DGUSLCD_SendTemperaturePID),
      VPHELPER(VP_BED_PID_D, &thermalManager.temp_bed.pid.Kd, DGUSScreenVariableHandler::HandleTemperaturePIDChanged, DGUSScreenVariableHandler::DGUSLCD_SendTemperaturePID),
      VPHELPER(VP_PID_AUTOTUNE_BED, nullptr, &DGUSScreenVariableHandler::HandlePIDAutotune, nullptr),
    #endif
  #endif

  // Fan Data
  #if FAN_COUNT
    #define FAN_VPHELPER(N) \
      VPHELPER(VP_Fan##N##_Percentage, &thermalManager.fan_speed[N], DGUSScreenVariableHandler::DGUSLCD_PercentageToUint8, &DGUSScreenVariableHandler::DGUSLCD_SendPercentageToDisplay), \
      VPHELPER(VP_FAN##N##_CONTROL, &thermalManager.fan_speed[N], &DGUSScreenVariableHandler::HandleFanControl, nullptr), \
      VPHELPER(VP_FAN##N##_STATUS, &thermalManager.fan_speed[N], nullptr, &DGUSScreenVariableHandler::DGUSLCD_SendFanStatusToDisplay),
    REPEAT(FAN_COUNT, FAN_VPHELPER)
  #endif

  // Feedrate
  VPHELPER(VP_Feedrate_Percentage, &feedrate_percentage, DGUSScreenVariableHandler::DGUSLCD_SetValueDirectly<int16_t>, &DGUSScreenVariableHandler::DGUSLCD_SendWordValueToDisplay ),

  // Position Data
  VPHELPER(VP_XPos, &current_position.x, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),
  VPHELPER(VP_YPos, &current_position.y, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),
  VPHELPER(VP_ZPos, &current_position.z, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsLongValueToDisplay<2>),

  // Print Progress
  VPHELPER(VP_PrintProgress_Percentage, nullptr, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendPrintProgressToDisplay ),

  // Print Time
  VPHELPER_STR(VP_PrintTime, nullptr, VP_PrintTime_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendPrintTimeToDisplay ),
  #if ENABLED(PRINTCOUNTER)
    VPHELPER_STR(VP_PrintAccTime, nullptr, VP_PrintAccTime_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendPrintAccTimeToDisplay ),
    VPHELPER_STR(VP_PrintsTotal, nullptr, VP_PrintsTotal_LEN, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendPrintsTotalToDisplay ),
  #endif

  VPHELPER(VP_X_STEP_PER_MM, &planner.settings.axis_steps_per_mm[X_AXIS], DGUSScreenVariableHandler::HandleStepPerMMChanged, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Y_STEP_PER_MM, &planner.settings.axis_steps_per_mm[Y_AXIS], DGUSScreenVariableHandler::HandleStepPerMMChanged, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Z_STEP_PER_MM, &planner.settings.axis_steps_per_mm[Z_AXIS], DGUSScreenVariableHandler::HandleStepPerMMChanged, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  #if HOTENDS >= 1
    VPHELPER(VP_E0_STEP_PER_MM, &planner.settings.axis_steps_per_mm[E_AXIS_N(0)], DGUSScreenVariableHandler::HandleStepPerMMExtruderChanged, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  #endif
  #if HOTENDS >= 2
    VPHELPER(VP_E1_STEP_PER_MM, &planner.settings.axis_steps_per_mm[E_AXIS_N(1)], DGUSScreenVariableHandler::HandleStepPerMMExtruderChanged, DGUSScreenVariableHandler::DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  #endif

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
    VPHELPER(VP_SD_Print_Setting, nullptr, DGUSScreenVariableHandler::DGUSLCD_SD_PrintTune, nullptr),
    #if HAS_BED_PROBE
      VPHELPER(VP_SD_Print_ProbeOffsetZ, &probe.offset.z, DGUSScreenVariableHandler::HandleProbeOffsetZChanged, &DGUSScreenVariableHandler::DGUSLCD_SendFloatAsIntValueToDisplay<2>),
      #if ENABLED(BABYSTEPPING)
        VPHELPER(VP_SD_Print_LiveAdjustZ, nullptr, DGUSScreenVariableHandler::HandleLiveAdjustZ, nullptr),
      #endif
    #endif
  #endif

  #if ENABLED(DGUS_UI_WAITING)
    VPHELPER(VP_WAITING_STATUS, nullptr, nullptr, DGUSScreenVariableHandler::DGUSLCD_SendWaitingStatusToDisplay),
  #endif

  // Messages for the User, shared by the popup and the kill screen. They cant be autouploaded as we do not buffer content.
  { .VP = VP_MSGSTR1, .memadr = nullptr, .size = VP_MSGSTR1_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR2, .memadr = nullptr, .size = VP_MSGSTR2_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR3, .memadr = nullptr, .size = VP_MSGSTR3_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR4, .memadr = nullptr, .size = VP_MSGSTR4_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &DGUSScreenVariableHandler::DGUSLCD_SendStringToDisplayPGM },

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_HIPRECY
