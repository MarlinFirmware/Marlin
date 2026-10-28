/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * lcd/extui/dgus/creality/DGUSDisplayDef.cpp
 *
 * VP table and per-screen VP lists for the CR6Community touch screen firmware.
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"
#include "PageHandlers.h"
#include "PIDHandler.h"
#include "EstepsHandler.h"
#include "AxisSettingsHandler.h"
#include "FilamentLoadUnloadHandler.h"
#include "MeshValidationHandler.h"

#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"
#include "../../../../feature/caselight.h"

#if HAS_FAN
  #include "../../../../module/fans.h"
#endif
#if ENABLED(FWRETRACT)
  #include "../../../../feature/fwretract.h"
#endif
#if HAS_COLOR_LEDS
  #include "../../../../feature/leds/leds.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif
#if HAS_FILAMENT_SENSOR
  #include "../../../../feature/runout.h"
#endif

#include "../../ui_api.h"
#include "../../../marlinui.h" // For material presets

// ----- Common var lists
#if HAS_HOTEND
  #define VPList_HeatHotend VP_T_E0_Is, VP_T_E0_Set,
#else
  #define VPList_HeatHotend
#endif

#if HAS_HEATED_BED
  #define VPList_HeatBed VP_T_Bed_Is, VP_T_Bed_Set,
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

  VP_Z_OFFSET,  // This screen has a Z offset field, too
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
  { DGUS_SCREEN_BOOT, VPList_None },
  { DGUS_SCREEN_MAIN, VPList_Main },

  { DGUS_SCREEN_SDFILELIST, VPList_SDFileList },

  { DGUS_SCREEN_FILAMENTRUNOUT1, VPList_PrintPausingError },
  { DGUS_SCREEN_FILAMENTRUNOUT2, VPList_PrintPausingError },

  { DGUS_SCREEN_PRINT_FINISH, VPList_PrintScreen },
  { DGUS_SCREEN_PRINT_RUNNING, VPList_PrintScreen },
  { DGUS_SCREEN_PRINT_PAUSED, VPList_PrintScreen },

  { DGUS_SCREEN_TUNING, VPList_TuneScreen },
  { DGUS_SCREEN_TUNING_EXTRA, VPList_TuneExtraScreen },
  { DGUS_SCREEN_PREPARE, VPList_Prepare },

  { DGUS_SCREEN_INFO, VPList_Info },

  { DGUS_SCREEN_MOVE1MM, VPList_PrintScreen },
  { DGUS_SCREEN_MOVE10MM, VPList_PrintScreen },
  { DGUS_SCREEN_MOVE01MM, VPList_PrintScreen },

  { DGUS_SCREEN_FEED, VPList_Feed },
  { DGUS_SCREEN_SETUP, VPList_Control },

  { DGUS_SCREEN_TEMP, VPList_Temp },
  { DGUS_SCREEN_TEMP_PLA, VPList_PreheatPLASettings },
  { DGUS_SCREEN_TEMP_ABS, VPList_PreheatABSSettings },

  { DGUS_SCREEN_ZOFFSET_LEVEL, VPList_ZOffsetLevel },
  { DGUS_SCREEN_LEVELING, VPList_Leveling },

  { DGUS_SCREEN_POWER_LOSS, VPList_None },
  { DGUS_SCREEN_THERMAL_RUNAWAY, VPList_None },
  { DGUS_SCREEN_HEATING_FAILED, VPList_None },
  { DGUS_SCREEN_THERMISTOR_ERROR, VPList_None },

  { DGUS_SCREEN_AUTOHOME, VPList_PrintScreen },

  { DGUS_SCREEN_DIALOG_PAUSE, VPList_None },
  { DGUS_SCREEN_DIALOG_STOP, VPList_DialogStop },

  { DGUS_SCREEN_CONFIRM, VPList_None },
  { DGUS_SCREEN_POPUP, VPList_None },

  { DGUS_SCREEN_ESTEPS_CALIBRATION, VPList_EstepsCalibration },
  { DGUS_SCREEN_PIDTUNE_CALIBRATION, VPList_PidTune },

  { DGUS_SCREEN_TUNEFWRETRACT, VPList_FWRetractTune },

  { DGUS_SCREEN_ESTEPS_CALIBRATION_RESULTS, VPList_EstepsCalibration },
  { DGUS_SCREEN_LEVELING_SETTINGS, VPList_LevelingSettings },

  { DGUS_SCREEN_AXIS_SETTINGS_NAV, VPList_AxisSettingsNav },
  { DGUS_SCREEN_AXIS_SETTINGS_AXIS, VPList_AxisSettingsAxis },
  { DGUS_SCREEN_AXIS_SETTINGS_TMC, VPList_AxisSettingsTMC },
  { DGUS_SCREEN_ADV_MOV_SETTINGS, VPList_AdvMovementSettings },

  { DGUS_SCREEN_MISC_SETTINGS, VPList_MiscSettings },
  { DGUS_SCREEN_MESH_VALIDATION, VPList_MeshValidation },

  { DGUS_SCREEN_CALIBRATE, VPList_Calibrate },
  { DGUS_SCREEN_RGB, VPList_RGB },

  { 0 , nullptr } // List is terminated with an nullptr as table entry.
};

#if HAS_MESH && MESH_INPUT_SUPPORTED_SIZE == GRID_MAX_POINTS
  #define _MESH_INPUT_VP(N) VPHELPER(VP_MESH_INPUT_X0_Y0 + (N) * (MESH_INPUT_DATA_SIZE), nullptr, screen.handleMeshPoint, nullptr),
#endif

// State of the compile-time probing options, for the icons on the Leveling Settings screen
static bool probing_heaters_off = ENABLED(PROBING_HEATERS_OFF),
            probing_stabilize = ANY(WAIT_FOR_BED_HEATER, WAIT_FOR_HOTEND);

const struct DGUS_VP_Variable ListOfVP[] PROGMEM = {
  // Back button state
  VPHELPER(VP_BACK_BUTTON_STATE, nullptr, nullptr, screen.sendBusyState),

  // Screen version
  VPHELPER(VP_UI_VERSION_MAJOR, nullptr, screen.handleScreenVersion, nullptr),

  #if HAS_HOTEND
    VPHELPER(VP_Flowrate_E0, &planner.flow_percentage[0], screen.handleFlowRateChanged, screen.sendWordValueToDisplay),
    VPHELPER(VP_T_E0_Is, &thermalManager.temp_hotend[0].celsius, nullptr, screen.sendFloatAsIntValueToDisplay<0>),
    VPHELPER(VP_T_E0_Set, &thermalManager.temp_hotend[0].target, screen.handleTemperatureChanged, screen.sendWordValueToDisplay),
    VPHELPER(VP_MESH_LEVEL_TEMP, &thermalManager.temp_hotend[0].target, nullptr, screen.sendWordValueToDisplay),
  #endif

  #if HAS_HEATED_BED
    VPHELPER(VP_T_Bed_Is, &thermalManager.temp_bed.celsius, nullptr, screen.sendFloatAsIntValueToDisplay<0>),
    VPHELPER(VP_T_Bed_Set, &thermalManager.temp_bed.target, screen.handleTemperatureChanged, screen.sendWordValueToDisplay),
  #endif

  // Feedrate
  VPHELPER(VP_Feedrate_Percentage, &motion.feedrate_percentage, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),

  VPHELPER(VP_PrintProgress_Percentage, nullptr, nullptr, screen.sendPrintProgressToDisplay),
  VPHELPER(VP_PrintTimeProgressBar, nullptr, nullptr, screen.sendPrintProgressToDisplay),

  #if ALL(HAS_EXTRUDERS, EDITABLE_STEPS_PER_UNIT)
    // E-steps calibration
    VPHELPER(VP_ESTEPS_CURRENT, &EstepsHandler::set_esteps, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_ESTEPS_CALIBRATION_TEMP, &EstepsHandler::calibration_temperature, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),
    VPHELPER(VP_ESTEPS_CALIBRATION_LENGTH, &EstepsHandler::filament_to_extrude, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_ESTEPS_CALIBRATION_MARK_LENGTH, &EstepsHandler::mark_filament_mm, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_ESTEPS_CALIBRATION_LEFTOVER_LENGTH, &EstepsHandler::remaining_filament, EstepsHandler::handleRemainingFilament, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_ESTEPS_CALCULATED_ESTEPS, &EstepsHandler::calculated_esteps, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),

    VPHELPER(VP_ESTEPS_CALIBRATESTART_BUTTON, nullptr, EstepsHandler::handleStartButton, nullptr),
    VPHELPER(VP_ESTEPS_APPLY_BUTTON, nullptr, EstepsHandler::handleApplyButton, nullptr),
    VPHELPER(VP_ESTEPS_BACK_BUTTON, nullptr, EstepsHandler::handleBackButton, nullptr),

    VPHELPER(VP_ESTEP_NAV_BUTTON, nullptr, (screen.navigateToPage<DGUS_SCREEN_ESTEPS_CALIBRATION, EstepsHandler>), nullptr),
  #endif

  #if ENABLED(PIDTEMP)
    // Hotend PID autotune
    VPHELPER(VP_PIDTUNE_TARGET_TEMP, &PIDHandler::calibration_temperature, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),
    VPHELPER(VP_PIDTUNE_CYCLES, &PIDHandler::cycles, screen.setValueDirectly<uint16_t>, screen.sendWordValueToDisplay),
    VPHELPER(VP_PIDTUNE_FAN_TOGGLE, &PIDHandler::fan_on, screen.toggleBoolean, nullptr),
    VPHELPER(VP_PIDTUNE_FAN_TOGGLE_ICON, &PIDHandler::fan_on, nullptr, (screen.sendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
    VPHELPER(VP_PIDTUNE_START_BUTTON, nullptr, PIDHandler::handleStartButton, nullptr),
    VPHELPER(VP_PIDTUNE_NAV_BUTTON, nullptr, (screen.navigateToPage<DGUS_SCREEN_PIDTUNE_CALIBRATION, PIDHandler>), nullptr),
  #endif

  VPHELPER(VP_GENERIC_BACK_BUTTON, nullptr, screen.onBackButton, nullptr),

  #if ENABLED(G26_MESH_VALIDATION)
    // Mesh validation
    VPHELPER(VP_MESHPATTERN_NOZZLE_TEMP, &MeshValidationHandler::nozzle_temperature, MeshValidationHandler::handleTemperature, screen.sendWordValueToDisplay),
    VPHELPER(VP_MESHPATTERN_BED_TEMP, &MeshValidationHandler::bed_temperature, MeshValidationHandler::handleTemperature, screen.sendWordValueToDisplay),

    VPHELPER(VP_MESHPATTERN_BUTTON_ICON, &MeshValidationHandler::is_running, nullptr, (screen.sendIconValue<MESHPATTERN_BUTTON_CANCEL, MESHPATTERN_BUTTON_START>)),

    VPHELPER(VP_MESHPATTERN_START_BUTTON, nullptr, MeshValidationHandler::handleStartOrCancelButton, nullptr),
    VPHELPER(VP_MESHPATTERN_NAV_BUTTON, nullptr, (screen.navigateToPage<DGUS_SCREEN_MESH_VALIDATION, MeshValidationHandler>), nullptr),
  #endif
  // Axis settings
  VPHELPER(VP_AXIS_SETTINGS_NAV_BUTTON, nullptr, AxisSettingsHandler::handleNavigation, nullptr),
  VPHELPER(VP_AXIS_SETTINGS_TITLE_ICON, &AxisSettingsHandler::axis_settings_title_icon, nullptr, screen.sendWordValueToDisplay),

  VPHELPER(VP_AXIS_SETTINGS_AXIS_STEPSMM, &AxisSettingsHandler::axis_steps_mm, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_MAX_ACCEL, &AxisSettingsHandler::max_acceleration_mm_per_s2, screen.setValueDirectly<uint16_t>, screen.sendWordValueToDisplay),

  VPHELPER(VP_AXIS_SETTINGS_AXIS_JERK, &AxisSettingsHandler::jerk, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_AXIS_SETTINGS_AXIS_FEEDRATE, &AxisSettingsHandler::max_feedrate, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),

  #if HAS_TRINAMIC_CONFIG
    VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCCURRENT, &AxisSettingsHandler::tmc_current, screen.setValueDirectly<uint16_t>, screen.sendWordValueToDisplay),
    VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_BUTTON, &AxisSettingsHandler::stealthchop, screen.toggleBoolean, nullptr),
    VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_ICON, &AxisSettingsHandler::stealthchop, nullptr, (screen.sendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
    VPHELPER(VP_AXIS_SETTINGS_AXIS_TMCHYBRIDTHRESHOLD, &AxisSettingsHandler::hybrid_threshold, screen.receiveULongFromDisplay, screen.sendULongToDisplay),
  #endif

  VPHELPER(VP_AXIS_TMC_NAV_ICON, &AxisSettingsHandler::has_tmc_settings, nullptr, (screen.sendIconValue<AXIS_TMC_NAV_ICON_SHOWING, AXIS_TMC_NAV_ICON_HIDING>)),

  VPHELPER(VP_AXIS_SETTINGS_NAV_BACKBUTTON, nullptr, AxisSettingsHandler::handleBackNavigation, nullptr),

  VPHELPER(VP_AXIS_TUNING_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_AXIS_SETTINGS_NAV>, nullptr),
  #if HAS_TRINAMIC_CONFIG
    VPHELPER(VP_AXIS_TMC_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_AXIS_SETTINGS_TMC>, nullptr),
  #endif

  // Advanced movement settings
  VPHELPER(VP_MOV_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_ADV_MOV_SETTINGS>, nullptr),

  VPHELPER(VP_MOV_MINIMUM_SEGMENT_TIME, nullptr, screen.handleMinSegmentTime, screen.sendMinSegmentTime),

  VPHELPER(VP_MOV_MINIMUM_FEEDRATE, &planner.settings.min_feedrate_mm_s, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_MOV_NORMAL_ACCELERATION, &planner.settings.acceleration, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_MOV_RETRACT_ACCELERATION, &planner.settings.retract_acceleration, screen.setFloatAsLongFromDisplay<1>, screen.sendFloatAsLongValueToDisplay<1>),

  VPHELPER(VP_MOV_MINIMUM_TRAVEL_FEEDRATE, &planner.settings.min_travel_feedrate_mm_s, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_MOV_MINIMUM_TRAVEL_ACCELERATION, &planner.settings.travel_acceleration, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),

  // Misc settings
  VPHELPER(VP_MISCSETTINGS_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_MISC_SETTINGS>, nullptr),

  #if HAS_FILAMENT_SENSOR
    VPHELPER(VP_FILAMENTRUNOUT_SENSOR_TOGGLE_BUTTON, &runout.enabled, screen.toggleBoolean, nullptr),
    VPHELPER(VP_FILAMENTRUNOUT_SENSOR_TOGGLE_ICON, &runout.enabled, nullptr, (screen.sendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    VPHELPER(VP_PLR_TOGGLE_BUTTON, nullptr, screen.togglePowerLossRecovery, nullptr),
    VPHELPER(VP_PLR_TOGGLE_ICON, &recovery.enabled, nullptr, (screen.sendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
  #endif

  // Preheat settings
  #if PREHEAT_COUNT
    #ifdef PREHEAT_1_LABEL
      VPHELPER(VP_PREHEAT_PLA_HOTEND_TEMP, &ui.material_preset[0].hotend_temp, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),
      VPHELPER(VP_PREHEAT_PLA_BED_TEMP, &ui.material_preset[0].bed_temp, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),
    #endif
    #ifdef PREHEAT_2_LABEL
      VPHELPER(VP_PREHEAT_ABS_HOTEND_TEMP, &ui.material_preset[1].hotend_temp, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),
      VPHELPER(VP_PREHEAT_ABS_BED_TEMP, &ui.material_preset[1].bed_temp, screen.setValueDirectly<int16_t>, screen.sendWordValueToDisplay),
    #endif
  #endif

  // About info
  VPHELPER_STR(VP_MARLIN_WEBSITE, nullptr, VP_MARLIN_WEBSITE_LEN, nullptr, screen.sendAboutFirmwareWebsite),
  VPHELPER_STR(VP_MARLIN_VERSION, nullptr, VP_MARLIN_VERSION_LEN, nullptr, screen.sendAboutFirmwareVersion),
  VPHELPER_STR(VP_PRINTER_BEDSIZE, nullptr, VP_PRINTER_BEDSIZE_LEN, nullptr, screen.sendAboutPrintSize),

  // Position Data
  VPHELPER(VP_X_POSITION, &motion.position.x, screen.handlePositionChange, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Y_POSITION, &motion.position.y, screen.handlePositionChange, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Z_POSITION, &motion.position.z, screen.handlePositionChange, screen.sendFloatAsIntValueToDisplay<1>),
  VPHELPER(VP_Z_POSITION_PRECISION, &motion.position.z, nullptr, screen.sendFloatAsLongValueToDisplay<2>),

  VPHELPER(SP_X_POSITION, nullptr, nullptr, screen.sendAxisTrustValue<X_AXIS>),
  VPHELPER(SP_Y_POSITION, nullptr, nullptr, screen.sendAxisTrustValue<Y_AXIS>),
  VPHELPER(SP_Z_POSITION, nullptr, nullptr, screen.sendAxisTrustValue<Z_AXIS>),

  #if HAS_BED_PROBE
    VPHELPER(VP_Z_OFFSET, &probe.offset.z, screen.handleZoffsetChange<2>, screen.sendFloatAsIntValueToDisplay<2>),
  #elif ENABLED(BABYSTEPPING)
    VPHELPER(VP_Z_OFFSET, nullptr, screen.handleZoffsetChange<2>, screen.sendZOffsetToDisplay<2>),
  #endif

  #if HAS_FAN
    VPHELPER(VP_FAN_TOGGLE, &fans[0].speed, nullptr, screen.sendFanStatusToDisplay),
    VPHELPER(VP_Fan0_Percentage, &fans[0].speed, screen.handleFanSpeedChanged, screen.sendFanSpeedToDisplay),
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    VPHELPER(VP_POWER_LOSS_RECOVERY, nullptr, screen.handlePowerLossRecovery, nullptr),
  #endif

  VPHELPER_STR(VP_PrintTime, nullptr, VP_PrintTime_LEN, nullptr, screen.sendPrintTimeToDisplay),
  VPHELPER_STR(VP_PrintTimeWithRemainingVisible, nullptr, VP_PrintTime_LEN, nullptr, screen.sendPrintTimeWithRemainingToDisplay),
  VPHELPER_STR(VP_PrintTimeRemaining, nullptr, VP_PrintTimeRemaining_LEN, nullptr, screen.sendPrintTimeRemainingToDisplay),
  VPHELPER(VP_SCREENCHANGE, nullptr, screen.screenChangeHook, nullptr),
  VPHELPER(VP_CONFIRMED, nullptr, screen.screenConfirmedOK, nullptr),

  #if HAS_MESH
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      VPHELPER(VP_LEVELING_FADE_HEIGHT, &planner.z_fade_height, screen.handleFadeHeight, screen.sendFloatAsIntValueToDisplay<1>),
    #endif
    VPHELPER(VP_LEVELING_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_ZOFFSET_LEVEL>, nullptr),
    VPHELPER(VP_LEVELING_EDIT_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_LEVELING>, nullptr),
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

  // Leveling Settings. The probing options are fixed in Configuration.h, so only show them.
  VPHELPER(VP_TOGGLE_PROBE_SETTINGS_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_LEVELING_SETTINGS>, nullptr),
  VPHELPER(VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_BUTTON, nullptr, screen.handleCompileTimeSetting, nullptr),
  VPHELPER(VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_ICON, &probing_heaters_off, nullptr, (screen.sendIconValue<ICON_ACCURACY_TOGGLE_ON, ICON_ACCURACY_TOGGLE_OFF>)),
  VPHELPER(VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_BUTTON, nullptr, screen.handleCompileTimeSetting, nullptr),
  VPHELPER(VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_ICON, &probing_stabilize, nullptr, (screen.sendIconValue<ICON_POST_PROBE_TEMP_STABILIZATION_TOGGLE_ON, ICON_POST_PROBE_TEMP_STABILIZATION_TOGGLE_OFF>)),
  #if HAS_LEVELING_TEMP_EDIT
    #if HAS_HOTEND
      VPHELPER(VP_TOGGLE_PROBE_PREHEAT_HOTEND_TEMP, &thermalManager.leveling_temp.hotend, screen.handleLevelingTemp, screen.sendWordValueToDisplay),
    #endif
    #if HAS_HEATED_BED
      VPHELPER(VP_TOGGLE_PROBE_PREHEAT_BED_TEMP, &thermalManager.leveling_temp.bed, screen.handleLevelingTemp, screen.sendWordValueToDisplay),
    #endif
  #endif

  // File listing
  #if HAS_MEDIA
    VPHELPER(VP_SD_ScrollEvent, nullptr, screen.sdScrollFilelist, nullptr),
    VPHELPER(VP_SD_FileSelected, nullptr, screen.sdFileSelected, nullptr),
    VPHELPER(VP_SD_FileSelectConfirm, nullptr, screen.sdStartPrint, nullptr),
    VPHELPER_STR(VP_SD_FileName0, nullptr, VP_SD_FileName_LEN, nullptr, screen.sdSendFilename),
    VPHELPER_STR(VP_SD_FileName1, nullptr, VP_SD_FileName_LEN, nullptr, screen.sdSendFilename),
    VPHELPER_STR(VP_SD_FileName2, nullptr, VP_SD_FileName_LEN, nullptr, screen.sdSendFilename),
    VPHELPER_STR(VP_SD_FileName3, nullptr, VP_SD_FileName_LEN, nullptr, screen.sdSendFilename),
    VPHELPER_STR(VP_SD_FileName4, nullptr, VP_SD_FileName_LEN, nullptr, screen.sdSendFilename),
  #endif

  // Firmware retract
  #if ENABLED(FWRETRACT)
    VPHELPER(VP_FWRETRACT_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_TUNEFWRETRACT>, nullptr),

    VPHELPER(VP_FWRETRACT_RETRACT_LENGTH, &fwretract.settings.retract_length, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_FWRETRACT_RETRACT_FEEDRATE, &fwretract.settings.retract_feedrate_mm_s, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_FWRETRACT_RETRACT_ZHOP, &fwretract.settings.retract_zraise, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),

    VPHELPER(VP_FWRETRACT_RESTART_LENGTH, &fwretract.settings.retract_recover_extra, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_FWRETRACT_RESTART_FEEDRATE, &fwretract.settings.retract_recover_feedrate_mm_s, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),

    #if ENABLED(FWRETRACT_AUTORETRACT)
      VPHELPER(VP_FWRETRACT_INDICATOR_ICON, &fwretract.autoretract_enabled, nullptr, (screen.sendIconValue<ICON_FWRETRACT_AUTO_ENGAGED, ICON_FWRETRACT_AUTO_DISENGAGED>)),
      VPHELPER(VP_FWRETRACT_TOGGLE_BUTTON_ICON, &fwretract.autoretract_enabled, nullptr, (screen.sendIconValue<ICON_FWRETRACT_AUTO_TOGGLE_ON, ICON_FWRETRACT_AUTO_TOGGLE_OFF>)),
      VPHELPER(VP_FWRETRACT_TOGGLE_BUTTON, &fwretract.autoretract_enabled, screen.toggleBoolean, nullptr),
    #endif
  #endif

  // Other tuning
  #if ENABLED(LIN_ADVANCE)
    VPHELPER(VP_LINEAR_ADVANCE_FACTOR, nullptr, screen.handleLinearAdvance, screen.sendLinearAdvance),
  #endif

  VPHELPER(VP_OTHER_TUNE_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_TUNING_EXTRA>, nullptr),

  // Additional buttons
  VPHELPER(VP_MUTE_TOGGLE, nullptr, screen.handleToggleTouchScreenMute, nullptr),
  VPHELPER(VP_STANDBY_BACKLIGHT_TOGGLE, nullptr, screen.handleToggleTouchScreenStandbySetting, nullptr),

  // Additional settings
  VPHELPER(VP_SCREEN_BACKLIGHT_STANDBY, &screen.lcd_settings.standby_screen_brightness, screen.handleTouchScreenStandbyBrightnessSetting, screen.sendWordValueToDisplay),
  VPHELPER(VP_SCREEN_BACKLIGHT, &screen.lcd_settings.screen_brightness, screen.handleTouchScreenBrightnessSetting, screen.sendWordValueToDisplay),
  VPHELPER(VP_SCREEN_STANDBY_TIME, &screen.lcd_settings.standby_time_seconds, screen.handleTouchScreenStandbyTimeSetting, screen.sendWordValueToDisplay),

  // RGB
  VPHELPER(VP_RGB_NAV_BUTTON_ICON, &screen.hasRGBSettings, nullptr, (screen.sendIconValue<ICON_RGB_SETTINGS_AVAILABLE, ICON_RGB_SETTINGS_UNAVAILABLE>)),

  #if HAS_COLOR_LEDS
    VPHELPER(VP_RGB_NAV_BUTTON, nullptr, screen.navigateToPage<DGUS_SCREEN_RGB>, nullptr),

    VPHELPER(VP_RGB_CONTROL_R, &leds.color.r, screen.handleLED, screen.sendLEDToDisplay),
    VPHELPER(VP_RGB_CONTROL_G, &leds.color.g, screen.handleLED, screen.sendLEDToDisplay),
    VPHELPER(VP_RGB_CONTROL_B, &leds.color.b, screen.handleLED, screen.sendLEDToDisplay),
    #if HAS_WHITE_LED
      VPHELPER(VP_RGB_CONTROL_W, &leds.color.w, screen.handleLED, screen.sendLEDToDisplay),
    #endif
    #if ENABLED(NEOPIXEL_LED)
      VPHELPER(VP_RGB_CONTROL_I, &leds.color.i, screen.handleLED, screen.sendLEDToDisplay),
    #endif
  #endif

  #if HAS_EXTRUDERS
    // Filament load/unload
    VPHELPER(VP_FILCHANGE_NAV_BUTTON, nullptr, (screen.navigateToPage<DGUS_SCREEN_FEED, FilamentLoadUnloadHandler>), nullptr),

    VPHELPER(VP_FILCHANGE_LENGTH, &FilamentLoadUnloadHandler::length, screen.setFloatAsIntFromDisplay<1>, screen.sendFloatAsIntValueToDisplay<1>),
    VPHELPER(VP_FILCHANGE_NOZZLE_TEMP, &FilamentLoadUnloadHandler::nozzle_temperature, FilamentLoadUnloadHandler::handleTemperature, screen.sendWordValueToDisplay),
    VPHELPER(VP_FILCHANGE_ACTION_BUTTON, nullptr, FilamentLoadUnloadHandler::handleLoadUnloadButton, nullptr),
  #endif

  // Icons
  VPHELPER(VP_LED_TOGGLE, &caselight.on, nullptr, (screen.sendIconValue<ICON_LED_TOGGLE_ON, ICON_LED_TOGGLE_OFF>)),
  VPHELPER(VP_STANDBY_BACKLIGHT_ICON, &screen.lcd_settings.display_standby, nullptr, (screen.sendIconValue<ICON_TOGGLE_ON, ICON_TOGGLE_OFF>)),
  VPHELPER(VP_MUTE_ICON, &screen.lcd_settings.display_sound, nullptr, (screen.sendIconValue<ICON_SOUND_TOGGLE_ON, ICON_SOUND_TOGGLE_OFF>)),

  // Development test button
  VPHELPER(VP_DEVELOPMENT_HELPER_BUTTON, nullptr, screen.handleDevelopmentTestButton, nullptr),

  // Mesh override input
  #ifdef _MESH_INPUT_VP
    REPEAT(MESH_INPUT_SUPPORTED_SIZE, _MESH_INPUT_VP)
  #endif

  // M117 LCD String (We don't need the string in memory but "just" push it to the display on demand, hence the nullptr
  { .VP = VP_M117, .memadr = nullptr, .size = VP_M117_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = screen.sendStringToDisplay },
  { .VP = VP_M117_STATIC, .memadr = nullptr, .size = VP_M117_STATIC_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = screen.sendStringToDisplay },

  // Messages for the User, shared by the popup and the kill screen. They cant be autouploaded as we do not buffer content.
  { .VP = VP_MSGSTR1, .memadr = nullptr, .size = VP_MSGSTR1_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = screen.sendStringToDisplayPGM },
  { .VP = VP_MSGSTR2, .memadr = nullptr, .size = VP_MSGSTR2_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = screen.sendStringToDisplayPGM },
  { .VP = VP_MSGSTR3, .memadr = nullptr, .size = VP_MSGSTR3_LEN, .set_by_display_handler = nullptr, .send_to_display_handler = screen.sendStringToDisplayPGM },

  VPHELPER(0, 0, 0, 0)  // must be last entry.
};

#endif // DGUS_LCD_UI_CREALITY_TOUCH
