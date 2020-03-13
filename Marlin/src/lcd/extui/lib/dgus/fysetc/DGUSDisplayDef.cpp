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

#if ENABLED(DGUS_LCD_UI_FYSETC)

#include "../DGUSDisplayDef.h"
#include "../DGUSScreenHandler.h"

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
    VP_E1_FILAMENT_LOAD_UNLOAD,
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
    VP_T_E0_Is, VP_T_E0_Set, VP_Flowrate_E0,
  #endif
  #if HOTENDS >= 2
    VP_T_E1_Is, VP_T_E1_Set, VP_Flowrate_E1,
  #endif
  #if HAS_HEATED_BED
    VP_T_Bed_Is, VP_T_Bed_Set,
  #endif
  VP_Feedrate_Percentage,
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
  #if ENABLED(PIDTEMPBED)
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
  { DGUS_SCREEN_BOOT, VPList_Boot },
  { DGUS_SCREEN_MAIN, VPList_Main },
  { DGUS_SCREEN_TEMPERATURE, VPList_Temp },
  { DGUS_SCREEN_STATUS, VPList_Status },
  { DGUS_SCREEN_STATUS2, VPList_Status2 },
  { DGUS_SCREEN_PREHEAT, VPList_Preheat },
  { DGUS_SCREEN_MANUALMOVE, VPList_ManualMove },
  { DGUS_SCREEN_MANUALEXTRUDE, VPList_ManualExtrude },
  { DGUS_SCREEN_FILAMENT_HEATING, VPList_Filament_heating },
  { DGUS_SCREEN_FILAMENT_LOADING, VPList_Filament_load_unload },
  { DGUS_SCREEN_FILAMENT_UNLOADING, VPList_Filament_load_unload },
  { DGUS_SCREEN_SDPRINTMANIPULATION, VPList_SD_PrintManipulation },
  { DGUS_SCREEN_SDFILELIST, VPList_SDFileList },
  { DGUS_SCREEN_SDPRINTTUNE, VPList_SDPrintTune },
  { DGUS_SCREEN_WAITING, VPList_PIDTuningWaiting },
  { DGUS_SCREEN_FLC_PREHEAT, VPList_FLCPreheat },
  { DGUS_SCREEN_FLC_PRINTING, VPList_FLCPrinting },
  { DGUS_SCREEN_Z_OFFSET, VPList_Z_Offset },
  { DGUS_SCREEN_STEPPERMM, VPList_StepPerMM },
  { DGUS_SCREEN_PID_E, VPList_PIDE0 },
  { DGUS_SCREEN_PID_BED, VPList_PIDBED },
  { DGUS_SCREEN_INFOS, VPList_Infos },
  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

// Helper to define a DGUS_VP_Variable with a specific size (eg. a string)
#define VPHELPER(VPADR, VPADRVAR, SIZE, RXFPTR, TXFPTR) { .VP=VPADR, .memadr=VPADRVAR, .size=SIZE, \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

// Helper to define a DGUS_VP_Variable for common use cases.
// Most VPs currently expect 2 bytes, size is 2 by default
#define VPHELPER_RXTX(VPADR, VPADRVAR, RXFPTR, TXFPTR) VPHELPER(VPADR, VPADRVAR, 2, RXFPTR, TXFPTR)

#define VPHELPER_RX(VPADR, VPADRVAR, RXFPTR) VPHELPER_RXTX(VPADR, VPADRVAR, RXFPTR, nullptr)
#define VPHELPER_TX(VPADR, VPADRVAR, TXFPTR) VPHELPER_RXTX(VPADR, VPADRVAR, nullptr, TXFPTR)

#define VPHELPER_RX_NODATA(VPADR, VPADRVAR, RXFPTR) VPHELPER(VPADR, VPADRVAR, 0, RXFPTR, nullptr)

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // Helper to detect touch events
  VPHELPER_RX(VP_SCREENCHANGE, nullptr, &DGUSScreenHandler::ScreenChangeHook),
  VPHELPER_RX(VP_SCREENCHANGE_ASK, nullptr, &DGUSScreenHandler::ScreenChangeHookIfIdle),
  #if ENABLED(SDSUPPORT)
    VPHELPER_RX(VP_SCREENCHANGE_WHENSD, nullptr, &DGUSScreenHandler::ScreenChangeHookIfSD),
  #endif
  VPHELPER_RX_NODATA(VP_CONFIRMED, nullptr, &DGUSScreenHandler::ScreenConfirmedOK),

  VPHELPER_RX_NODATA(VP_TEMP_ALL_OFF, nullptr, &DGUSScreenHandler::HandleAllHeatersOff),

  #if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
    VPHELPER_RX(VP_MOVE_OPTION, &distanceToMove, &DGUSScreenHandler::HandleManualMoveOption),

    VPHELPER_RX(VP_MOVE_X, &distanceToMove, &DGUSScreenHandler::HandleManualMove),
    VPHELPER_RX(VP_MOVE_Y, &distanceToMove, &DGUSScreenHandler::HandleManualMove),
    VPHELPER_RX(VP_MOVE_Z, &distanceToMove, &DGUSScreenHandler::HandleManualMove),
    VPHELPER_RX(VP_HOME_ALL, &distanceToMove, &DGUSScreenHandler::HandleManualMove),
  #else
    VPHELPER_RX(VP_MOVE_X, nullptr, &DGUSScreenHandler::HandleManualMove),
    VPHELPER_RX(VP_MOVE_Y, nullptr, &DGUSScreenHandler::HandleManualMove),
    VPHELPER_RX(VP_MOVE_Z, nullptr, &DGUSScreenHandler::HandleManualMove),
    VPHELPER_RX(VP_HOME_ALL, nullptr, &DGUSScreenHandler::HandleManualMove),
  #endif

  VPHELPER_RX(VP_MOTOR_LOCK_UNLOCK, nullptr, &DGUSScreenHandler::HandleMotorLockUnlock),

  #if ENABLED(POWER_LOSS_RECOVERY)
    VPHELPER_RX(VP_POWER_LOSS_RECOVERY, nullptr, &DGUSScreenHandler::HandlePowerLossRecovery),
  #endif
  VPHELPER_RX(VP_SETTINGS, nullptr, &DGUSScreenHandler::HandleSettings),

  #if ENABLED(SINGLE_Z_CALIBRATION)
    // Not implemented yet
    //VPHELPER_RX(VP_Z_CALIBRATE, nullptr, &DGUSScreenHandler::HandleZCalibration),
  #endif
  #if ENABLED(FIRST_LAYER_CAL)
    // Not implemented yet
    //VPHELPER_RX(VP_Z_FIRST_LAYER_CAL, nullptr, &DGUSScreenHandler::HandleFirstLayerCal),
  #endif

  VPHELPER(VP_MARLIN_VERSION, (void*)MarlinVersion, VP_MARLIN_VERSION_LEN, nullptr, &DGUSScreenHandler::SendStringToDisplayPGM),
  // M117 LCD String (We don't need the string in memory but "just" push it to the display on demand, hence the nullptr
  VPHELPER(VP_M117, nullptr, VP_M117_LEN, nullptr, &DGUSScreenHandler::SendStringToDisplay),

  // Temperature Data
  #if HOTENDS >= 1
    VPHELPER_TX(VP_T_E0_Is, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].celsius, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<0>),
    VPHELPER_RXTX(VP_T_E0_Set, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].target, &DGUSScreenHandler::HandleTemperatureChanged, &DGUSScreenHandler::SendWordValueToDisplay),
    VPHELPER_RXTX(VP_Flowrate_E0, &planner.flow_percentage[ExtUI::extruder_t::E0], &DGUSScreenHandler::HandleFlowRateChanged, &DGUSScreenHandler::SendWordValueToDisplay),
    VPHELPER_TX(VP_EPos, &destination.e, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<2>),
    VPHELPER_RX(VP_MOVE_E0, nullptr, &DGUSScreenHandler::HandleManualExtrude),
    VPHELPER_RX_NODATA(VP_E0_CONTROL, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].target, &DGUSScreenHandler::HandleHeaterControl),
    VPHELPER_TX(VP_E0_STATUS, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].target, &DGUSScreenHandler::SendHeaterStatusToDisplay),
    #if ENABLED(DGUS_PREHEAT_UI)
      VPHELPER_RX(VP_E0_BED_PREHEAT, nullptr, &DGUSScreenHandler::HandlePreheat),
    #endif
    #if ENABLED(PIDTEMP)
      VPHELPER_RXTX(VP_E0_PID_P, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].pid.Kp, &DGUSScreenHandler::HandleTemperaturePIDChanged, &DGUSScreenHandler::SendTemperaturePID),
      VPHELPER_RXTX(VP_E0_PID_I, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].pid.Ki, &DGUSScreenHandler::HandleTemperaturePIDChanged, &DGUSScreenHandler::SendTemperaturePID),
      VPHELPER_RXTX(VP_E0_PID_D, &thermalManager.temp_hotend[ExtUI::extruder_t::E0].pid.Kd, &DGUSScreenHandler::HandleTemperaturePIDChanged, &DGUSScreenHandler::SendTemperaturePID),
      VPHELPER_RX_NODATA(VP_PID_AUTOTUNE_E0, nullptr, &DGUSScreenHandler::HandlePIDAutotune),
    #endif
    #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
      VPHELPER_RXTX(VP_E0_FILAMENT_LOAD_UNLOAD, nullptr, &DGUSScreenHandler::HandleFilamentOption, &DGUSScreenHandler::HandleFilamentLoadUnload),
    #endif
  #endif
  #if HOTENDS >= 2
    VPHELPER_TX(VP_T_E1_Is, &thermalManager.temp_hotend[ExtUI::extruder_t::E1].celsius, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<0>),
    VPHELPER_RXTX(VP_T_E1_Set, &thermalManager.temp_hotend[ExtUI::extruder_t::E1].target, &DGUSScreenHandler::HandleTemperatureChanged, &DGUSScreenHandler::SendWordValueToDisplay),
    VPHELPER_RXTX(VP_Flowrate_E1, &planner.flow_percentage[ExtUI::extruder_t::E1], &DGUSScreenHandler::HandleFlowRateChanged, &DGUSScreenHandler::SendWordValueToDisplay),
    VPHELPER_RX(VP_MOVE_E1, nullptr, &DGUSScreenHandler::HandleManualExtrude),
    VPHELPER_RX_NODATA(VP_E1_CONTROL, &thermalManager.temp_hotend[ExtUI::extruder_t::E1].target, &DGUSScreenHandler::HandleHeaterControl),
    VPHELPER_TX(VP_E1_STATUS, &thermalManager.temp_hotend[ExtUI::extruder_t::E1].target, &DGUSScreenHandler::SendHeaterStatusToDisplay),
    #if ENABLED(PIDTEMP)
      VPHELPER_RX_NODATA(VP_PID_AUTOTUNE_E1, nullptr, &DGUSScreenHandler::HandlePIDAutotune),
    #endif
    VPHELPER_RXTX(VP_E1_FILAMENT_LOAD_UNLOAD, nullptr, &DGUSScreenHandler::HandleFilamentOption, &DGUSScreenHandler::HandleFilamentLoadUnload),
  #endif
  #if HAS_HEATED_BED
    VPHELPER_TX(VP_T_Bed_Is, &thermalManager.temp_bed.celsius, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<0>),
    VPHELPER_RXTX(VP_T_Bed_Set, &thermalManager.temp_bed.target, &DGUSScreenHandler::HandleTemperatureChanged, &DGUSScreenHandler::SendWordValueToDisplay),
    VPHELPER_RX_NODATA(VP_BED_CONTROL, &thermalManager.temp_bed.target, &DGUSScreenHandler::HandleHeaterControl),
    VPHELPER_TX(VP_BED_STATUS, &thermalManager.temp_bed.target, &DGUSScreenHandler::SendHeaterStatusToDisplay),
    #if ENABLED(PIDTEMPBED)
      VPHELPER_RXTX(VP_BED_PID_P, &thermalManager.temp_bed.pid.Kp, &DGUSScreenHandler::HandleTemperaturePIDChanged, &DGUSScreenHandler::SendTemperaturePID),
      VPHELPER_RXTX(VP_BED_PID_I, &thermalManager.temp_bed.pid.Ki, &DGUSScreenHandler::HandleTemperaturePIDChanged, &DGUSScreenHandler::SendTemperaturePID),
      VPHELPER_RXTX(VP_BED_PID_D, &thermalManager.temp_bed.pid.Kd, &DGUSScreenHandler::HandleTemperaturePIDChanged, &DGUSScreenHandler::SendTemperaturePID),
      VPHELPER_RX_NODATA(VP_PID_AUTOTUNE_BED, nullptr, &DGUSScreenHandler::HandlePIDAutotune),
    #endif
  #endif

  // Fan Data
  #if FAN_COUNT > 0
    #define FAN_VPHELPER(N) \
      VPHELPER_RXTX(VP_Fan##N##_Percentage, &thermalManager.fan_speed[N], &DGUSScreenHandler::PercentageToUint8, &DGUSScreenHandler::SendPercentageToDisplay), \
      VPHELPER_RX_NODATA(VP_FAN##N##_CONTROL, &thermalManager.fan_speed[N], &DGUSScreenHandler::HandleFanControl), \
      VPHELPER_TX(VP_FAN##N##_STATUS, &thermalManager.fan_speed[N], &DGUSScreenHandler::SendFanStatusToDisplay),
    REPEAT(FAN_COUNT, FAN_VPHELPER)
  #endif

  // Feedrate
  VPHELPER_RXTX(VP_Feedrate_Percentage, &feedrate_percentage, &DGUSScreenHandler::SetValueDirectly<int16_t>, &DGUSScreenHandler::SendWordValueToDisplay ),

  // Position Data
  VPHELPER_TX(VP_XPos, &current_position.x, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<2>),
  VPHELPER_TX(VP_YPos, &current_position.y, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<2>),
  VPHELPER_TX(VP_ZPos, &current_position.z, &DGUSScreenHandler::SendFloatAsLongValueToDisplay<2>),

  // Print Progress
  VPHELPER_TX(VP_PrintProgress_Percentage, nullptr, &DGUSScreenHandler::SendPrintProgressToDisplay ),

  // Print Time
  VPHELPER(VP_PrintTime, nullptr, VP_PrintTime_LEN, nullptr, &DGUSScreenHandler::SendPrintTimeToDisplay),
  #if ENABLED(PRINTCOUNTER)
    VPHELPER(VP_PrintAccTime, nullptr, VP_PrintAccTime_LEN, nullptr, &DGUSScreenHandler::SendPrintAccTimeToDisplay),
    VPHELPER(VP_PrintsTotal, nullptr, VP_PrintsTotal_LEN, nullptr, &DGUSScreenHandler::SendPrintsTotalToDisplay),
  #endif

  VPHELPER_RXTX(VP_X_STEP_PER_MM, &planner.settings.axis_steps_per_mm[X_AXIS], &DGUSScreenHandler::HandleStepPerMMChanged, &DGUSScreenHandler::SendFloatAsIntValueToDisplay<1>),
  VPHELPER_RXTX(VP_Y_STEP_PER_MM, &planner.settings.axis_steps_per_mm[Y_AXIS], &DGUSScreenHandler::HandleStepPerMMChanged, &DGUSScreenHandler::SendFloatAsIntValueToDisplay<1>),
  VPHELPER_RXTX(VP_Z_STEP_PER_MM, &planner.settings.axis_steps_per_mm[Z_AXIS], &DGUSScreenHandler::HandleStepPerMMChanged, &DGUSScreenHandler::SendFloatAsIntValueToDisplay<1>),
  #if HOTENDS >= 1
    VPHELPER_RXTX(VP_E0_STEP_PER_MM, &planner.settings.axis_steps_per_mm[E_AXIS_N(0)], &DGUSScreenHandler::HandleStepPerMMExtruderChanged, &DGUSScreenHandler::SendFloatAsIntValueToDisplay<1>),
  #endif
  #if HOTENDS >= 2
    VPHELPER_RXTX(VP_E1_STEP_PER_MM, &planner.settings.axis_steps_per_mm[E_AXIS_N(1)], &DGUSScreenHandler::HandleStepPerMMExtruderChanged, &DGUSScreenHandler::SendFloatAsIntValueToDisplay<1>),
  #endif

  // SDCard File listing.
  #if ENABLED(SDSUPPORT)
    VPHELPER_RX(VP_SD_ScrollEvent, nullptr, &DGUSScreenHandler::SD_ScrollFilelist),
    VPHELPER_RX(VP_SD_FileSelected, nullptr, &DGUSScreenHandler::SD_FileSelected),
    VPHELPER_RX_NODATA(VP_SD_FileSelectConfirm, nullptr, &DGUSScreenHandler::SD_StartPrint),
    VPHELPER(VP_SD_FileName0, nullptr, VP_SD_FileName_LEN, nullptr, &DGUSScreenHandler::SD_SendFilename),
    VPHELPER(VP_SD_FileName1, nullptr, VP_SD_FileName_LEN, nullptr, &DGUSScreenHandler::SD_SendFilename),
    VPHELPER(VP_SD_FileName2, nullptr, VP_SD_FileName_LEN, nullptr, &DGUSScreenHandler::SD_SendFilename),
    VPHELPER(VP_SD_FileName3, nullptr, VP_SD_FileName_LEN, nullptr, &DGUSScreenHandler::SD_SendFilename),
    VPHELPER(VP_SD_FileName4, nullptr, VP_SD_FileName_LEN, nullptr, &DGUSScreenHandler::SD_SendFilename),
    VPHELPER_RX(VP_SD_ResumePauseAbort, nullptr, &DGUSScreenHandler::SD_ResumePauseAbort),
    VPHELPER_RX_NODATA(VP_SD_AbortPrintConfirmed, nullptr, &DGUSScreenHandler::SD_ReallyAbort),
    VPHELPER_RX_NODATA(VP_SD_Print_Setting, nullptr, &DGUSScreenHandler::SD_PrintTune),
    #if HAS_BED_PROBE
      VPHELPER_RXTX(VP_SD_Print_ProbeOffsetZ, &probe.offset.z, &DGUSScreenHandler::HandleProbeOffsetZChanged, &DGUSScreenHandler::SendFloatAsIntValueToDisplay<2>),
      #if ENABLED(BABYSTEPPING)
        VPHELPER_RX(VP_SD_Print_LiveAdjustZ, nullptr, &DGUSScreenHandler::HandleLiveAdjustZ),
      #endif
    #endif
  #endif

  #if ENABLED(DGUS_UI_WAITING)
    VPHELPER_TX(VP_WAITING_STATUS, nullptr, &DGUSScreenHandler::SendWaitingStatusToDisplay),
  #endif

  // Messages for the User, shared by the popup and the kill screen. They cant be autouploaded as we do not buffer content.
  VPHELPER(VP_MSGSTR1, nullptr, VP_MSGSTR1_LEN, nullptr, &DGUSScreenHandler::SendStringToDisplayPGM),
  VPHELPER(VP_MSGSTR2, nullptr, VP_MSGSTR2_LEN, nullptr, &DGUSScreenHandler::SendStringToDisplayPGM),
  VPHELPER(VP_MSGSTR3, nullptr, VP_MSGSTR3_LEN, nullptr, &DGUSScreenHandler::SendStringToDisplayPGM),
  VPHELPER(VP_MSGSTR4, nullptr, VP_MSGSTR4_LEN, nullptr, &DGUSScreenHandler::SendStringToDisplayPGM),

  VPHELPER(0, nullptr, 0, nullptr, nullptr)  // must be last entry.
};

#endif // DGUS_LCD_UI_FYSETC
