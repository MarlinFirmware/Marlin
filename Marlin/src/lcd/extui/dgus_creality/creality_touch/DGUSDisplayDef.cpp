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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "../DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"
#include "AxisSettingsHandler.h"
#include "EstepsHandler.h"
#include "FilamentLoadUnloadHandler.h"
#include "PIDHandler.h"
#include "MeshValidationHandler.h"

#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"

#include "../../../../feature/caselight.h"

#if ENABLED(FWRETRACT)
  #include "../../../../feature/fwretract.h"
#endif

#if HAS_COLOR_LEDS
  #include "../../../../feature/leds/leds.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
#include "../../../../feature/runout.h"
#endif

#include "../../ui_api.h"
#include "../../../marlinui.h"

#include "PageHandlers.h"

#if ENABLED(DGUS_UI_MOVE_DIS_OPTION)
  uint16_t distanceToMove = 10;
#endif
using namespace ExtUI;


const char MarlinVersion[] PROGMEM = SHORT_BUILD_VERSION;

// ----- Common var lists
#if HOTENDS >= 1
#define VPList_HeatHotend VP_T_E0_Is, VP_T_E0_Set,
#else
#define VPList_HeatHotend
#endif

#if HAS_HEATED_BED
#define VPList_HeatBed  VP_T_Bed_Is, VP_T_Bed_Set,
#else
#define VPList_HeatBed
#endif

#define VPList_Common VP_BACK_BUTTON_STATE
#define VPList_CommonWithStatus VPList_HeatHotend VPList_HeatBed VP_Z_OFFSET, VP_Feedrate_Percentage, VP_BACK_BUTTON_STATE
#define VPList_CommonWithHeatOnly VPList_HeatHotend VPList_HeatBed VP_BACK_BUTTON_STATE

// ----- Which variables to auto-update on which screens
const uint16_t VPList_None[] PROGMEM = {
  VPList_Common,

  0x0000
};

const uint16_t VPList_DialogStop[] PROGMEM = {
  VPList_Common,

  0x0000
};

const uint16_t VPList_Main[] PROGMEM = {
  VPList_CommonWithStatus,

  0x0000
};

const uint16_t VPList_SDFileList[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_SD_FileName0,
  VP_SD_FileName1,
  VP_SD_FileName2,
  VP_SD_FileName3,
  VP_SD_FileName4,
  VP_SD_FileName5,

  0x0000
};

const uint16_t VPList_Control[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_LED_TOGGLE,
  VP_MUTE_ICON,

  0x0000
};

const uint16_t VPList_Feed[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_FILCHANGE_NOZZLE_TEMP,
  VP_FILCHANGE_LENGTH,

  0x0000
};

const uint16_t VPList_Temp[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_FAN_TOGGLE,

  0x0000
};


const uint16_t VPList_PreheatPLASettings[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_PREHEAT_PLA_HOTEND_TEMP,
  VP_PREHEAT_PLA_BED_TEMP,

  0x0000
};

const uint16_t VPList_PreheatABSSettings[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_PREHEAT_ABS_HOTEND_TEMP,
  VP_PREHEAT_ABS_BED_TEMP,

  0x0000
};


const uint16_t VPList_PrintPausingError[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_X_POSITION,
  VP_Y_POSITION,
  VP_Z_POSITION_PRECISION,
  VP_Z_OFFSET,
  VP_Fan0_Percentage,
  VP_Feedrate_Percentage,

  VP_PrintProgress_Percentage,
  VP_PrintTimeProgressBar,
  VP_PrintTime,
  VP_PrintTimeWithRemainingVisible,
  VP_PrintTimeRemaining,
  VP_LINEAR_ADVANCE_FACTOR,

  0x0000
};

const uint16_t VPList_PrintScreen[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_X_POSITION, VP_Y_POSITION, VP_Z_POSITION, VP_Z_POSITION_PRECISION,
  SP_X_POSITION, SP_Y_POSITION, SP_Z_POSITION,

  VP_Flowrate_E0,
  VP_Fan0_Percentage,

  VP_PrintProgress_Percentage,
  VP_PrintTimeProgressBar,
  VP_PrintTime,
  VP_PrintTimeWithRemainingVisible,
  VP_PrintTimeRemaining,
  VP_LINEAR_ADVANCE_FACTOR,

  VP_FWRETRACT_INDICATOR_ICON,

  0x0000
};

const uint16_t VPList_Leveling[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_MESH_LEVEL_TEMP,

  0x0000
};

const uint16_t VPList_ZOffsetLevel[] PROGMEM = {
  VPList_CommonWithStatus,

  0x0000
};

const uint16_t VPList_TuneScreen[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_PrintTime,

  VP_Flowrate_E0,

  VP_LED_TOGGLE,
  VP_FAN_TOGGLE,
  VP_Fan0_Percentage,

  0x0000
};


const uint16_t VPList_TuneExtraScreen[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_LINEAR_ADVANCE_FACTOR,
  VP_RGB_NAV_BUTTON_ICON,

  0x0000
};


const uint16_t VPList_Prepare[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_PrintTime,

  0x0000
};

const uint16_t VPList_Info[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_PrintTime,

  VP_PRINTER_BEDSIZE,
  VP_MARLIN_WEBSITE,
  VP_MARLIN_VERSION,

  0x0000
};

const uint16_t VPList_EstepsCalibration[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_ESTEPS_CURRENT,
  VP_ESTEPS_CALIBRATION_TEMP,
  VP_ESTEPS_CALIBRATION_LENGTH,
  VP_ESTEPS_CALIBRATION_LEFTOVER_LENGTH,
  VP_ESTEPS_CALIBRATION_MARK_LENGTH,
  VP_ESTEPS_CALCULATED_ESTEPS,

  0x0000
};

const uint16_t VPList_PidTune[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_PIDTUNE_TARGET_TEMP,
  VP_PIDTUNE_FAN_TOGGLE_ICON,
  VP_PIDTUNE_CYCLES,

  0x0000
};

const uint16_t VPList_FWRetractTune[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_FWRETRACT_RETRACT_LENGTH,
  VP_FWRETRACT_RETRACT_FEEDRATE,
  VP_FWRETRACT_RETRACT_ZHOP,
  VP_FWRETRACT_RESTART_LENGTH,
  VP_FWRETRACT_RESTART_FEEDRATE,

  VP_FWRETRACT_TOGGLE_BUTTON_ICON,

  0x0000
};

const uint16_t VPList_LevelingSettings[] PROGMEM = {
  VPList_CommonWithStatus,

  VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_ICON,
  VP_TOGGLE_PROBE_PREHEAT_HOTEND_TEMP,
  VP_TOGGLE_PROBE_PREHEAT_BED_TEMP,
  VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_ICON,
  VP_LEVELING_FADE_HEIGHT,

  0x0000
};

const uint16_t VPList_AxisSettingsNav[] PROGMEM = {
  VPList_CommonWithStatus,

  0x0000
};

const uint16_t VPList_AxisSettingsAxis[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_AXIS_SETTINGS_TITLE_ICON,

  VP_AXIS_SETTINGS_AXIS_STEPSMM,
  VP_AXIS_SETTINGS_AXIS_MAX_ACCEL,
  VP_AXIS_SETTINGS_AXIS_JERK,
  VP_AXIS_SETTINGS_AXIS_FEEDRATE,

  VP_AXIS_TMC_NAV_ICON,

  0x0000
};

const uint16_t VPList_AxisSettingsTMC[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_AXIS_SETTINGS_AXIS_TMCCURRENT,
  VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_ICON,
  VP_AXIS_SETTINGS_AXIS_TMCHYBRIDTHRESHOLD,

  0x0000
};

const uint16_t VPList_AdvMovementSettings[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_MOV_MINIMUM_SEGMENT_TIME,
  VP_MOV_MINIMUM_FEEDRATE,
  VP_MOV_NORMAL_ACCELERATION,
  VP_MOV_RETRACT_ACCELERATION,

  VP_MOV_MINIMUM_TRAVEL_FEEDRATE,
  VP_MOV_MINIMUM_TRAVEL_ACCELERATION, 

  0x0000
};

const uint16_t VPList_MiscSettings[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_FILAMENTRUNOUT_SENSOR_TOGGLE_ICON,

  VP_PLR_TOGGLE_ICON,

  VP_STANDBY_BACKLIGHT_ICON,
  VP_SCREEN_BACKLIGHT_STANDBY,
  VP_SCREEN_BACKLIGHT,
  VP_SCREEN_STANDBY_TIME,
  VP_RGB_NAV_BUTTON_ICON,

  0x0000
};

const uint16_t VPList_MeshValidation[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_MESHPATTERN_NOZZLE_TEMP,
  VP_MESHPATTERN_BED_TEMP,

  VP_MESHPATTERN_BUTTON_ICON,

  0x0000
};

const uint16_t VPList_Calibrate[] PROGMEM = {
  VPList_CommonWithStatus,

  0x0000
};

const uint16_t VPList_RGB[] PROGMEM = {
  VPList_CommonWithHeatOnly,

  VP_RGB_CONTROL_R,
  VP_RGB_CONTROL_G,
  VP_RGB_CONTROL_B,
  VP_RGB_CONTROL_W,
  VP_RGB_CONTROL_I,

  0x0000
};


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
  { DGUSLCD_SCREEN_TUNING_EXTRA, VPList_TuneExtraScreen },
  { DGUSLCD_SCREEN_PREPARE, VPList_Prepare },

  { DGUSLCD_SCREEN_INFO, VPList_Info },

  { DGUSLCD_SCREEN_MOVE1MM, VPList_PrintScreen },
  { DGUSLCD_SCREEN_MOVE10MM, VPList_PrintScreen },
  { DGUSLCD_SCREEN_MOVE01MM, VPList_PrintScreen },

  { DGUSLCD_SCREEN_FEED, VPList_Feed },
  { DGUSLCD_SCREEN_SETUP, VPList_Control },

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

  { DGUSLCD_SCREEN_ESTEPS_CALIBRATION, VPList_EstepsCalibration },
  { DGUSLCD_SCREEN_PIDTUNE_CALIBRATION, VPList_PidTune },

  { DGUSLCD_SCREEN_TUNEFWRETRACT, VPList_FWRetractTune },

  { DGUSLCD_SCREEN_ESTEPS_CALIBRATION_RESULTS, VPList_EstepsCalibration },
  { DGUSLCD_SCREEN_LEVELING_SETTINGS, VPList_LevelingSettings },

  { DGUSLCD_SCREEN_AXIS_SETTINGS_NAV, VPList_AxisSettingsNav },
  { DGUSLCD_SCREEN_AXIS_SETTINGS_AXIS , VPList_AxisSettingsAxis },
  { DGUSLCD_SCREEN_AXIS_SETTINGS_TMC, VPList_AxisSettingsTMC },
  { DGUSLCD_SCREEN_ADV_MOV_SETTINGS, VPList_AdvMovementSettings },

  { DGUSLCD_SCREEN_MISC_SETTINGS, VPList_MiscSettings },
  { DGUSLCD_SCREEN_MESH_VALIDATION, VPList_MeshValidation },

  { DGUSLCD_SCREEN_CALIBRATE, VPList_Calibrate },
  { DGUSLCD_SCREEN_RGB, VPList_RGB},

  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

// Helper to define a DGUS_VP_Variable for common use cases.
#define VPHELPER(VPADR, VPADRVAR, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=sizeof(VPADRVAR), \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

// Helper to define a DGUS_VP_Variable when the sizeo of the var cannot be determined automaticalyl (eg. a string)
#define VPHELPER_STR(VPADR, VPADRVAR, STRLEN, RXFPTR, TXFPTR ) { .VP=VPADR, .memadr=VPADRVAR, .size=STRLEN, \
  .set_by_display_handler = RXFPTR, .send_to_display_handler = TXFPTR }

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // Back button state
  VPHELPER(VP_BACK_BUTTON_STATE, nullptr, nullptr, ScreenHandler.SendBusyState),

  // Screen version
  VPHELPER(VP_UI_VERSION_MAJOR, nullptr, ScreenHandler.HandleScreenVersion, nullptr),

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

  // Feedrate
  VPHELPER(VP_Feedrate_Percentage, &feedrate_percentage, ScreenHandler.DGUSLCD_SetValueDirectly<int16_t>, &ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  VPHELPER(VP_PrintProgress_Percentage, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintProgressToDisplay),
  VPHELPER(VP_PrintTimeProgressBar, nullptr, nullptr, ScreenHandler.DGUSLCD_SendPrintProgressToDisplay),

  // Calibration
  // ... e-steps
  VPHELPER(VP_ESTEPS_CURRENT, &EstepsHandler::set_esteps, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_ESTEPS_CALIBRATION_TEMP, &EstepsHandler::calibration_temperature, ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_ESTEPS_CALIBRATION_LENGTH, &EstepsHandler::filament_to_extrude, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_ESTEPS_CALIBRATION_MARK_LENGTH, &EstepsHandler::mark_filament_mm, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_ESTEPS_CALIBRATION_LEFTOVER_LENGTH, &EstepsHandler::remaining_filament, EstepsHandler::HandleRemainingFilament, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_ESTEPS_CALCULATED_ESTEPS, &EstepsHandler::calculated_esteps, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  VPHELPER(VP_ESTEPS_CALIBRATESTART_BUTTON, nullptr, EstepsHandler::HandleStartButton, nullptr),
  VPHELPER(VP_ESTEPS_APPLY_BUTTON, nullptr, EstepsHandler::HandleApplyButton, nullptr),
  VPHELPER(VP_ESTEPS_BACK_BUTTON, nullptr, EstepsHandler::HandleBackButton, nullptr),

  VPHELPER(VP_ESTEP_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_ESTEPS_CALIBRATION, EstepsHandler>), nullptr),

  // ... PID
  VPHELPER(VP_PIDTUNE_TARGET_TEMP, &PIDHandler::calibration_temperature, ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_PIDTUNE_CYCLES, &PIDHandler::cycles, ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_PIDTUNE_FAN_TOGGLE, &PIDHandler::fan_on, ScreenHandler.DGUSLCD_ToggleBoolean, nullptr),
  VPHELPER(VP_PIDTUNE_FAN_TOGGLE_ICON, &PIDHandler::fan_on, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),

  VPHELPER(VP_PIDTUNE_START_BUTTON, nullptr, PIDHandler::HandleStartButton, nullptr),

  VPHELPER(VP_PIDTUNE_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_PIDTUNE_CALIBRATION, PIDHandler>), nullptr),

  VPHELPER(VP_GENERIC_BACK_BUTTON, nullptr, ScreenHandler.OnBackButton, nullptr),

#if HAS_MESH
  // ... Mesh validation
  VPHELPER(VP_MESHPATTERN_NOZZLE_TEMP, &MeshValidationHandler::nozzle_temperature, MeshValidationHandler::HandleTemperature, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_MESHPATTERN_BED_TEMP, &MeshValidationHandler::bed_temperature, MeshValidationHandler::HandleTemperature, ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  VPHELPER(VP_MESHPATTERN_BUTTON_ICON, &MeshValidationHandler::is_running,  nullptr, (ScreenHandler.DGUSLCD_SendIconValue<MESHPATTERN_BUTTON_CANCEL, MESHPATTERN_BUTTON_START>)),

  VPHELPER(VP_MESHPATTERN_START_BUTTON, nullptr, MeshValidationHandler::HandleStartOrCancelButton, nullptr),
  VPHELPER(VP_MESHPATTERN_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_MESH_VALIDATION, MeshValidationHandler>), nullptr),
#endif

  // Axis settings
  VPHELPER(VP_AXIS_SETTINGS_NAV_BUTTON, nullptr, AxisSettingsHandler::HandleNavigation, nullptr),
  VPHELPER(VP_AXIS_SETTINGS_TITLE_ICON, &AxisSettingsHandler::axis_settings_title_icon, nullptr, ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  VPHELPER(VP_AXIS_SETTINGS_AXIS_STEPSMM, &AxisSettingsHandler::axis_steps_mm, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_MAX_ACCEL, &AxisSettingsHandler::max_acceleration_mm_per_s2, ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  
  VPHELPER(VP_AXIS_SETTINGS_AXIS_JERK, &AxisSettingsHandler::jerk, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_FEEDRATE, &AxisSettingsHandler::max_feedrate, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCCURRENT, &AxisSettingsHandler::tmc_current, ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_BUTTON, &AxisSettingsHandler::stealthchop, ScreenHandler.DGUSLCD_ToggleBoolean, nullptr),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_ICON, &AxisSettingsHandler::stealthchop, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCHYBRIDTHRESHOLD, &AxisSettingsHandler::hybrid_threshold, ScreenHandler.DGUSLCD_ReceiveULongFromDisplay, ScreenHandler.DGUSLCD_SendULongToDisplay),
  
  VPHELPER(VP_AXIS_TMC_NAV_ICON, &AxisSettingsHandler::has_tmc_settings,  nullptr, (ScreenHandler.DGUSLCD_SendIconValue<AXIS_TMC_NAV_ICON_SHOWING, AXIS_TMC_NAV_ICON_HIDING>)),

  VPHELPER(VP_AXIS_SETTINGS_NAV_BACKBUTTON, nullptr, AxisSettingsHandler::HandleBackNavigation, nullptr),

  VPHELPER(VP_AXIS_TUNING_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_AXIS_SETTINGS_NAV>), nullptr),
  VPHELPER(VP_AXIS_TMC_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_AXIS_SETTINGS_TMC>), nullptr),

  // Advanced movement settings
  VPHELPER(VP_MOV_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_ADV_MOV_SETTINGS>), nullptr),

  VPHELPER(VP_MOV_MINIMUM_SEGMENT_TIME, &planner.settings.min_segment_time_us, ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>, ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  VPHELPER(VP_MOV_MINIMUM_FEEDRATE, &planner.settings.min_feedrate_mm_s, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_MOV_NORMAL_ACCELERATION, &planner.settings.acceleration, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_MOV_RETRACT_ACCELERATION, &planner.settings.retract_acceleration, ScreenHandler.DGUSLCD_SetFloatAsLongFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<1>),

  VPHELPER(VP_MOV_MINIMUM_TRAVEL_FEEDRATE, &planner.settings.min_travel_feedrate_mm_s, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_MOV_MINIMUM_TRAVEL_ACCELERATION, &planner.settings.travel_acceleration, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  // Misc settings
  VPHELPER(VP_MISCSETTINGS_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_MISC_SETTINGS>), nullptr),

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  VPHELPER(VP_FILAMENTRUNOUT_SENSOR_TOGGLE_BUTTON, &runout.enabled, ScreenHandler.DGUSLCD_ToggleBoolean, nullptr),
  VPHELPER(VP_FILAMENTRUNOUT_SENSOR_TOGGLE_ICON, &runout.enabled,  nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  VPHELPER(VP_PLR_TOGGLE_BUTTON, nullptr, ScreenHandler.TogglePowerLossRecovery, nullptr),
  VPHELPER(VP_PLR_TOGGLE_ICON, &PrintJobRecovery::enabled,  nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
#endif

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
  VPHELPER_STR(VP_MARLIN_WEBSITE, nullptr, VP_MARLIN_WEBSITE_LEN, nullptr, ScreenHandler.DGUSLCD_SendAboutFirmwareWebsite),
  VPHELPER_STR(VP_MARLIN_VERSION, nullptr, VP_MARLIN_VERSION_LEN, nullptr, ScreenHandler.DGUSLCD_SendAboutFirmwareVersion),
  VPHELPER_STR(VP_PRINTER_BEDSIZE, nullptr, VP_PRINTER_BEDSIZE_LEN, nullptr, ScreenHandler.DGUSLCD_SendAboutPrintSize),

  // Position Data
  VPHELPER(VP_X_POSITION, &current_position.x, ScreenHandler.HandlePositionChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Y_POSITION, &current_position.y, ScreenHandler.HandlePositionChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Z_POSITION, &current_position.z, ScreenHandler.HandlePositionChange, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Z_POSITION_PRECISION, &current_position.z, nullptr, ScreenHandler.DGUSLCD_SendFloatAsLongValueToDisplay<2>),

  VPHELPER(SP_X_POSITION, nullptr, nullptr, ScreenHandler.SendAxisTrustValue<X_AXIS>),
  VPHELPER(SP_Y_POSITION, nullptr, nullptr, ScreenHandler.SendAxisTrustValue<Y_AXIS>),
  VPHELPER(SP_Z_POSITION, nullptr, nullptr, ScreenHandler.SendAxisTrustValue<Z_AXIS>),

#if ENABLED(HAS_BED_PROBE)
  VPHELPER(VP_Z_OFFSET, &probe.offset.z, ScreenHandler.HandleZoffsetChange<2>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<2>),
#elif ENABLED(BABYSTEPPING)
  VPHELPER(VP_Z_OFFSET, nullptr, ScreenHandler.HandleZoffsetChange<2>, ScreenHandler.DGUSLCD_SendZOffsetToDisplay<2>),
#endif

  VPHELPER(VP_FAN_TOGGLE, &thermalManager.fan_speed[0], nullptr, ScreenHandler.DGUSLCD_SendFanStatusToDisplay),
  VPHELPER(VP_Fan0_Percentage, &thermalManager.fan_speed[0], ScreenHandler.HandleFanSpeedChanged, ScreenHandler.DGUSLCD_SendFanSpeedToDisplay),

  #if ENABLED(POWER_LOSS_RECOVERY)
    VPHELPER(VP_POWER_LOSS_RECOVERY, nullptr, &ScreenHandler.HandlePowerLossRecovery, nullptr),
  #endif

  VPHELPER_STR(VP_PrintTime, nullptr, VP_PrintTime_LEN, nullptr, ScreenHandler.DGUSLCD_SendPrintTimeToDisplay),
  VPHELPER_STR(VP_PrintTimeWithRemainingVisible, nullptr, VP_PrintTime_LEN, nullptr, ScreenHandler.DGUSLCD_SendPrintTimeWithRemainingToDisplay),
  VPHELPER_STR(VP_PrintTimeRemaining, nullptr, VP_PrintTimeRemaining_LEN, nullptr, ScreenHandler.DGUSLCD_SendPrintTimeRemainingToDisplay),
  VPHELPER(VP_SCREENCHANGE, nullptr, ScreenHandler.ScreenChangeHook, nullptr),
  VPHELPER(VP_CONFIRMED, nullptr, ScreenHandler.ScreenConfirmedOK, nullptr),

#if ALL(HAS_BED_PROBE_SETTINGS, HAS_BED_PROBE)
  VPHELPER(VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_BUTTON, nullptr, ScreenHandler.HandleToggleProbeHeaters, nullptr),
  VPHELPER(VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_ICON, &probe.settings.turn_heaters_off, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_ACCURACY_TOGGLE_ON, ICON_ACCURACY_TOGGLE_OFF>)),

  VPHELPER(VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_BUTTON, nullptr, ScreenHandler.HandleToggleProbeTemperatureStabilization, nullptr),
  VPHELPER(VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_ICON, &probe.settings.stabilize_temperatures_after_probing, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_POST_PROBE_TEMP_STABILIZATION_TOGGLE_ON, ICON_POST_PROBE_TEMP_STABILIZATION_TOGGLE_OFF>)),

  VPHELPER(VP_TOGGLE_PROBE_PREHEAT_HOTEND_TEMP, &probe.settings.preheat_hotend_temp, ScreenHandler.HandleToggleProbePreheatTemp, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_TOGGLE_PROBE_PREHEAT_BED_TEMP, &probe.settings.preheat_bed_temp, ScreenHandler.HandleToggleProbePreheatTemp, ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  VPHELPER(VP_TOGGLE_PROBE_SETTINGS_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_LEVELING_SETTINGS>), nullptr),
#endif
#ifdef HAS_MESH
  VPHELPER(VP_LEVELING_FADE_HEIGHT, &planner.z_fade_height, ScreenHandler.HandleFadeHeight, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  VPHELPER(VP_LEVELING_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_ZOFFSET_LEVEL>), nullptr),
  VPHELPER(VP_LEVELING_EDIT_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_LEVELING>), nullptr),
#endif
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

  // Firmware retract
  VPHELPER(VP_FWRETRACT_NAV_BUTTON, nullptr, ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_TUNEFWRETRACT>, nullptr),

  VPHELPER(VP_FWRETRACT_RETRACT_LENGTH, &fwretract.settings.retract_length, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_FWRETRACT_RETRACT_FEEDRATE, &fwretract.settings.retract_feedrate_mm_s, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_FWRETRACT_RETRACT_ZHOP, &fwretract.settings.retract_zraise, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  VPHELPER(VP_FWRETRACT_RESTART_LENGTH, &fwretract.settings.retract_recover_extra, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_FWRETRACT_RESTART_FEEDRATE, &fwretract.settings.retract_recover_feedrate_mm_s, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),

  VPHELPER(VP_FWRETRACT_INDICATOR_ICON, &fwretract.autoretract_enabled, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_FWRETRACT_AUTO_ENGAGED, ICON_FWRETRACT_AUTO_DISENGAGED>)),
  VPHELPER(VP_FWRETRACT_TOGGLE_BUTTON_ICON, &fwretract.autoretract_enabled, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_FWRETRACT_AUTO_TOGGLE_ON, ICON_FWRETRACT_AUTO_TOGGLE_OFF>)),
  VPHELPER(VP_FWRETRACT_TOGGLE_BUTTON, &fwretract.autoretract_enabled, ScreenHandler.DGUSLCD_ToggleBoolean, nullptr),

  // Other tuning
#if ENABLED(LIN_ADVANCE)
  VPHELPER(VP_LINEAR_ADVANCE_FACTOR, &planner.extruder_advance_K[0], ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<2>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<2>),
#endif

  VPHELPER(VP_OTHER_TUNE_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_TUNING_EXTRA>), nullptr),


  // Additional buttons
  VPHELPER(VP_MUTE_TOGGLE, nullptr, ScreenHandler.HandleToggleTouchScreenMute, nullptr),
  VPHELPER(VP_STANDBY_BACKLIGHT_TOGGLE, nullptr, ScreenHandler.HandleToggleTouchScreenStandbySetting, nullptr),

  // Additional settings
  VPHELPER(VP_SCREEN_BACKLIGHT_STANDBY, &ScreenHandler.Settings.standby_screen_brightness, ScreenHandler.HandleTouchScreenStandbyBrightnessSetting, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_SCREEN_BACKLIGHT, &ScreenHandler.Settings.screen_brightness, ScreenHandler.HandleTouchScreenBrightnessSetting, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_SCREEN_STANDBY_TIME, &ScreenHandler.Settings.standby_time_seconds, ScreenHandler.HandleTouchScreenStandbyTimeSetting, ScreenHandler.DGUSLCD_SendWordValueToDisplay),

  // RGB
  VPHELPER(VP_RGB_NAV_BUTTON_ICON, &ScreenHandler.HasRGBSettings, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_RGB_SETTINGS_AVAILABLE, ICON_RGB_SETTINGS_UNAVAILABLE>)),

  #if HAS_COLOR_LEDS
    VPHELPER(VP_RGB_NAV_BUTTON, nullptr, ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_RGB>, nullptr),

    VPHELPER(VP_RGB_CONTROL_R, &leds.color.r, ScreenHandler.HandleLED, ScreenHandler.SendLEDToDisplay),
    VPHELPER(VP_RGB_CONTROL_G, &leds.color.g, ScreenHandler.HandleLED, ScreenHandler.SendLEDToDisplay),
    VPHELPER(VP_RGB_CONTROL_B, &leds.color.b, ScreenHandler.HandleLED, ScreenHandler.SendLEDToDisplay),

    #if EITHER(RGBW_LED, NEOPIXEL_LED)
      VPHELPER(VP_RGB_CONTROL_W, &leds.color.w, ScreenHandler.HandleLED, ScreenHandler.SendLEDToDisplay),

      #if ENABLED(NEOPIXEL_LED)
        VPHELPER(VP_RGB_CONTROL_I, &leds.color.i, ScreenHandler.HandleLED, ScreenHandler.SendLEDToDisplay),
      #endif
    #endif
  #endif

  // Filament load/unload
  VPHELPER(VP_FILCHANGE_NAV_BUTTON, nullptr, (ScreenHandler.DGUSLCD_NavigateToPage<DGUSLCD_SCREEN_FEED, FilamentLoadUnloadHandler>), nullptr),

  VPHELPER(VP_FILCHANGE_LENGTH, &FilamentLoadUnloadHandler::length, ScreenHandler.DGUSLCD_SetFloatAsIntFromDisplay<1>, ScreenHandler.DGUSLCD_SendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_FILCHANGE_NOZZLE_TEMP, &FilamentLoadUnloadHandler::nozzle_temperature, FilamentLoadUnloadHandler::HandleTemperature, ScreenHandler.DGUSLCD_SendWordValueToDisplay),
  VPHELPER(VP_FILCHANGE_ACTION_BUTTON, nullptr, FilamentLoadUnloadHandler::HandleLoadUnloadButton, nullptr),

  // Icons
  VPHELPER(VP_LED_TOGGLE, &caselight.on, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_LED_TOGGLE_ON, ICON_LED_TOGGLE_OFF>)),
  VPHELPER(VP_STANDBY_BACKLIGHT_ICON, &ScreenHandler.Settings.display_standby, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
  VPHELPER(VP_MUTE_ICON, &ScreenHandler.Settings.display_sound, nullptr, (ScreenHandler.DGUSLCD_SendIconValue<ICON_SOUND_TOGGLE_ON, ICON_SOUND_TOGGLE_OFF>)),

  // Development test button
  VPHELPER(VP_DEVELOPMENT_HELPER_BUTTON, nullptr, ScreenHandler.HandleDevelopmentTestButton, nullptr),

  // Mesh override input
#if HAS_MESH
#if MESH_INPUT_SUPPORTED_SIZE == GRID_MAX_POINTS
  //#define _VPHELPER_GP(N) VPHELPER((VP_MESH_INPUT_X0_Y0 + ( ##N## * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  //REPEAT(MESH_INPUT_SUPPORTED_SIZE, _VPHELPER_GP)

  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 0 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 1 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 2 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 3 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 4 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 5 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 6 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 7 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 8 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 9 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 10 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 11 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 12 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 13 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 14 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
  VPHELPER((VP_MESH_INPUT_X0_Y0 + ( 15 * MESH_INPUT_DATA_SIZE)), nullptr, ScreenHandler.HandleMeshPoint, nullptr),
#endif
#endif
  // M117 LCD String (We don't need the string in memory but "just" push it to the display on demand, hence the nullptr
  { .VP = VP_M117, .memadr = nullptr, .size = VP_M117_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&ScreenHandler.DGUSLCD_SendStringToDisplay },
  { .VP = VP_M117_STATIC, .memadr = nullptr, .size = VP_M117_STATIC_LEN, .set_by_display_handler = nullptr, .send_to_display_handler =&ScreenHandler.DGUSLCD_SendStringToDisplay },

  // Messages for the User, shared by the popup and the kill screen. They cant be autouploaded as we do not buffer content.
  { .VP = VP_MSGSTR1, .memadr = nullptr, .size = VP_MSGSTR1_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR2, .memadr = nullptr, .size = VP_MSGSTR2_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },
  { .VP = VP_MSGSTR3, .memadr = nullptr, .size = VP_MSGSTR3_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },
  //{ .VP = VP_MSGSTR4, .memadr = nullptr, .size = VP_MSGSTR4_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = &ScreenHandler.DGUSLCD_SendStringToDisplayPGM },

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_ORIGIN
