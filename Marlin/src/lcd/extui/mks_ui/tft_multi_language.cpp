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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include "tft_multi_language.h"

// ********************************************* //

common_menu_def              common_menu;
main_menu_def                main_menu;
preheat_menu_def             preheat_menu;
move_menu_def                move_menu;
home_menu_def                home_menu;
file_menu_def                file_menu;
extrude_menu_def             extrude_menu;
leveling_menu_def            leveling_menu;
set_menu_def                 set_menu;
more_menu_def                more_menu;
wifi_menu_def                wifi_menu;
cloud_menu_def               cloud_menu;
about_menu_def               about_menu;
fan_menu_def                 fan_menu;
filament_menu_def            filament_menu;
printing_menu_def            printing_menu;
operation_menu_def           operation_menu;
pause_menu_def               pause_menu;
speed_menu_def               speed_menu;
printing_more_menu_def       printing_more_menu;
dialog_menu_def              dialog_menu;
language_menu_def            language_menu;
print_file_dialog_menu_def   print_file_dialog_menu;
filesys_menu_def             filesys_menu;
tool_menu_def                tool_menu;
MachinePara_menu_def         MachinePara_menu;
pause_msg_def                pause_msg_menu;
eeprom_def                   eeprom_menu;
media_select_menu_def        media_select_menu;

// TODO: Make all strings PSTR and update accessors for the benefit of AVR

machine_common_def machine_menu;
void machine_setting_disp() {
  if (gCfgItems.language == LANG_SIMPLE_CHINESE) {
    MachinePara_menu.title              = MACHINE_PARA_TITLE_CN;
    MachinePara_menu.MachineSetting     = MACHINE_TYPE_CNOFIG_CN;
    MachinePara_menu.MotorSetting       = MOTOR_CONFIG_CN;
    MachinePara_menu.leveling           = MACHINE_LEVELING_CONFIG_CN;
    MachinePara_menu.AdvanceSetting     = ADVANCE_CONFIG_CN;

    machine_menu.default_value = DEFAULT_CN;
    machine_menu.next          = NEXT_CN;
    machine_menu.previous      = PREVIOUS_CN;

    machine_menu.MachineConfigTitle = MACHINE_CONFIG_TITLE_CN;
    machine_menu.MachineType        = MACHINE_TYPE_CN;
    machine_menu.Stroke             = MACHINE_STROKE_CN;
    machine_menu.HomeDir            = MACHINE_HOMEDIR_CN;
    machine_menu.EndStopType        = MACHINE_ENDSTOP_TYPE_CN;
    machine_menu.FilamentConf       = MACHINE_FILAMENT_CONFIG_CN;

    machine_menu.MachineTypeConfTitle = MACHINE_TYPE_CONFIG_TITLE_CN;
    machine_menu.xyz                  = MACHINE_TYPE_XYZ_CN;
    machine_menu.delta                = MACHINE_TYPE_DELTA_CN;
    machine_menu.corexy               = MACHINE_TYPE_COREXY_CN;

    machine_menu.StrokeConfTitle = MACHINE_STROKE_CONF_TITLE_CN;
    machine_menu.xStroke         = X_MAX_LENGTH_CN;
    machine_menu.yStroke         = Y_MAX_LENGTH_CN;
    machine_menu.zStroke         = Z_MAX_LENGTH_CN;

    machine_menu.xmin = X_MIN_LENGTH_CN;
    machine_menu.ymin = Y_MIN_LENGTH_CN;
    machine_menu.zmin = Z_MIN_LENGTH_CN;

    machine_menu.HomeDirConfTitle = HOME_DIR_CONF_TITLE_CN;
    machine_menu.xHomeDir         = HOME_DIR_X_CN;
    machine_menu.yHomeDir         = HOME_DIR_Y_CN;
    machine_menu.zHomeDir         = HOME_DIR_Z_CN;
    machine_menu.min              = HOME_MIN_CN;
    machine_menu.max              = HOME_MAX_CN;

    machine_menu.EndstopConfTitle = ENDSTOP_CONF_TITLE_CN;
    machine_menu.xEndstop_min     = MIN_ENDSTOP_X_CN;
    machine_menu.yEndstop_min     = MIN_ENDSTOP_Y_CN;
    machine_menu.zEndstop_min     = MIN_ENDSTOP_Z_CN;
    machine_menu.xEndstop_max     = MAX_ENDSTOP_X_CN;
    machine_menu.yEndstop_max     = MAX_ENDSTOP_Y_CN;
    machine_menu.zEndstop_max     = MAX_ENDSTOP_Z_CN;
    machine_menu.FilamentEndstop  = ENDSTOP_FIL_CN;
    machine_menu.LevelingEndstop  = ENDSTOP_LEVEL_CN;
    machine_menu.opened           = ENDSTOP_OPENED_CN;
    machine_menu.closed           = ENDSTOP_CLOSED_CN;

    machine_menu.FilamentConfTitle   = FILAMENT_CONF_TITLE_CN;
    machine_menu.InLength            = FILAMENT_IN_LENGTH_CN;
    machine_menu.InSpeed             = FILAMENT_IN_SPEED_CN;
    machine_menu.FilamentTemperature = FILAMENT_TEMPERATURE_CN;
    machine_menu.OutLength           = FILAMENT_OUT_LENGTH_CN;
    machine_menu.OutSpeed            = FILAMENT_OUT_SPEED_CN;

    machine_menu.LevelingParaConfTitle   = LEVELING_CONF_TITLE_CN;
    machine_menu.LevelingParaConf        = LEVELING_PARA_CONF_CN;
    machine_menu.TrammingPosConf         = TRAMMING_POS_CN;
    machine_menu.LevelingAutoCommandConf = LEVELING_AUTO_COMMAND_CN;
    machine_menu.LevelingAutoZoffsetConf = LEVELING_AUTO_ZOFFSET_CN;

    machine_menu.LevelingSubConfTitle = LEVELING_PARA_CONF_TITLE_CN;
    machine_menu.AutoLevelEnable      = AUTO_LEVELING_ENABLE_CN;
    machine_menu.BLtouchEnable        = BLTOUCH_LEVELING_ENABLE_CN;
    machine_menu.ProbePort            = PROBE_PORT_CN;
    machine_menu.ProbeXoffset         = PROBE_X_OFFSET_CN;
    machine_menu.ProbeYoffset         = PROBE_Y_OFFSET_CN;
    machine_menu.ProbeZoffset         = PROBE_Z_OFFSET_CN;
    machine_menu.ProbeXYspeed         = PROBE_XY_SPEED_CN;
    machine_menu.ProbeZspeed          = PROBE_Z_SPEED_CN;
    machine_menu.enable               = ENABLE_CN;
    machine_menu.disable              = DISABLE_CN;
    machine_menu.locked               = LOCKED_CN;
    machine_menu.z_min                = Z_MIN_CN;
    machine_menu.z_max                = Z_MAX_CN;

    machine_menu.LevelingSubDeltaConfTitle = DELTA_LEVEL_CONF_TITLE_CN;
    machine_menu.MachineRadius             = DELTA_MACHINE_RADIUS_CN;
    machine_menu.DiagonalRod               = DELTA_DIAGONAL_ROD_CN;
    machine_menu.PrintableRadius           = DELTA_PRINT_RADIUS_CN;
    machine_menu.DeltaHeight               = DELTA_HEIGHT_CN;
    machine_menu.SmoothRodOffset           = SMOOTH_ROD_OFFSET_CN;
    machine_menu.EffectorOffset            = EFFECTOR_OFFSET_CN;
    machine_menu.CalibrationRadius         = CALIBRATION_RADIUS_CN;

    machine_menu.LevelingSubXYZConfTitle = XYZ_LEVEL_CONF_TITLE_CN;

    machine_menu.TemperatureConfTitle = TEMPERATURE_CONF_TITLE_CN;
    machine_menu.NozzleConf           = NOZZLE_CONF_CN;
    machine_menu.HotBedConf           = HOTBED_CONF_CN;
    machine_menu.PreheatTemperConf    = PREHEAT_TEMPER_CN;

    machine_menu.NozzleConfTitle      = NOZZLE_CONF_TITLE_CN;
    machine_menu.NozzleCnt            = NOZZLECNT_CN;
    machine_menu.NozzleType           = NOZZLE_TYPE_CN;
    machine_menu.NozzleAdjustType     = NOZZLE_ADJUST_TYPE_CN;
    machine_menu.NozzleMinTemperature = NOZZLE_MIN_TEMPERATURE_CN;
    machine_menu.NozzleMaxTemperature = NOZZLE_MAX_TEMPERATURE_CN;
    machine_menu.Extrude_Min_Temper   = EXTRUD_MIN_TEMPER_CN;

    machine_menu.HotbedConfTitle      = HOTBED_CONF_TITLE_CN;
    machine_menu.HotbedAdjustType     = HOTBED_ADJUST_CN;
    machine_menu.HotbedMinTemperature = HOTBED_MIN_TEMPERATURE_CN;
    machine_menu.HotbedMaxTemperature = HOTBED_MAX_TEMPERATURE_CN;

    machine_menu.MotorConfTitle        = MOTOR_CONF_TITLE_CN;
    machine_menu.MaxFeedRateConf       = MAXFEEDRATE_CONF_CN;
    machine_menu.AccelerationConf      = ACCELERATION_CONF_CN;
    machine_menu.JerkConf              = JERKCONF_CN;
    machine_menu.StepsConf             = STEPSCONF_CN;
    machine_menu.TMCcurrentConf        = TMC_CURRENT_CN;
    machine_menu.TMCStepModeConf       = TMC_STEP_MODE_CN;
    machine_menu.MotorDirConf          = MOTORDIRCONF_CN;
    machine_menu.HomeFeedRateConf      = HOMEFEEDRATECONF_CN;
    machine_menu.PausePosition         = PAUSE_POSITION_CN;
    machine_menu.WifiSettings          = WIFI_SETTINGS_CN;
    machine_menu.HomingSensitivityConf = HOMING_SENSITIVITY_CONF_CN;
    machine_menu.EncoderSettings         = ENCODER_SETTINGS_CN;

    machine_menu.MaxFeedRateConfTitle = MAXFEEDRATE_CONF_TITLE_CN;
    machine_menu.XMaxFeedRate         = X_MAXFEEDRATE_CN;
    machine_menu.YMaxFeedRate         = Y_MAXFEEDRATE_CN;
    machine_menu.ZMaxFeedRate         = Z_MAXFEEDRATE_CN;
    machine_menu.E0MaxFeedRate        = E0_MAXFEEDRATE_CN;
    machine_menu.E1MaxFeedRate        = E1_MAXFEEDRATE_CN;

    machine_menu.AccelerationConfTitle = ACCELERATION_CONF_TITLE_CN;
    machine_menu.PrintAcceleration     = PRINT_ACCELERATION_CN;
    machine_menu.RetractAcceleration   = RETRACT_ACCELERATION_CN;
    machine_menu.TravelAcceleration    = TRAVEL_ACCELERATION_CN;
    machine_menu.X_Acceleration        = X_ACCELERATION_CN;
    machine_menu.Y_Acceleration        = Y_ACCELERATION_CN;
    machine_menu.Z_Acceleration        = Z_ACCELERATION_CN;
    machine_menu.E0_Acceleration       = E0_ACCELERATION_CN;
    machine_menu.E1_Acceleration       = E1_ACCELERATION_CN;

    machine_menu.JerkConfTitle = JERK_CONF_TITLE_CN;
    machine_menu.X_Jerk        = X_JERK_CN;
    machine_menu.Y_Jerk        = Y_JERK_CN;
    machine_menu.Z_Jerk        = Z_JERK_CN;
    machine_menu.E_Jerk        = E_JERK_CN;

    machine_menu.StepsConfTitle = STEPS_CONF_TITLE_CN;
    machine_menu.X_Steps        = X_STEPS_CN;
    machine_menu.Y_Steps        = Y_STEPS_CN;
    machine_menu.Z_Steps        = Z_STEPS_CN;
    machine_menu.E0_Steps       = E0_STEPS_CN;
    machine_menu.E1_Steps       = E1_STEPS_CN;

    machine_menu.TmcCurrentConfTitle = TMC_CURRENT_CONF_TITLE_CN;
    machine_menu.X_Current           = X_TMC_CURRENT_CN;
    machine_menu.Y_Current           = Y_TMC_CURRENT_CN;
    machine_menu.Z_Current           = Z_TMC_CURRENT_CN;
    machine_menu.E0_Current          = E0_TMC_CURRENT_CN;
    machine_menu.E1_Current          = E1_TMC_CURRENT_CN;

    machine_menu.TmcStepModeConfTitle = TMC_MODE_CONF_TITLE_CN;
    machine_menu.X_StepMode           = X_TMC_MODE_CN;
    machine_menu.Y_StepMode           = Y_TMC_MODE_CN;
    machine_menu.Z_StepMode           = Z_TMC_MODE_CN;
    machine_menu.E0_StepMode          = E0_TMC_MODE_CN;
    machine_menu.E1_StepMode          = E1_TMC_MODE_CN;

    machine_menu.MotorDirConfTitle = MOTORDIR_CONF_TITLE_CN;
    machine_menu.X_MotorDir        = X_MOTORDIR_CN;
    machine_menu.Y_MotorDir        = Y_MOTORDIR_CN;
    machine_menu.Z_MotorDir        = Z_MOTORDIR_CN;
    machine_menu.E0_MotorDir       = E0_MOTORDIR_CN;
    machine_menu.E1_MotorDir       = E1_MOTORDIR_CN;
    machine_menu.Invert_0          = INVERT_P_CN;
    machine_menu.Invert_1          = INVERT_N_CN;

    machine_menu.HomeFeedRateConfTitle = HOMEFEEDRATE_CONF_TITLE_CN;
    machine_menu.XY_HomeFeedRate       = X_HOMESPEED_CN;
    machine_menu.Z_HomeFeedRate = Z_HOMESPEED_CN;

    machine_menu.AdvancedConfTitle = ADVANCED_CONF_TITLE_CN;
    machine_menu.PwrOffDection     = PWROFF_DECTION_CN;
    machine_menu.PwrOffAfterPrint  = PWROFF_AFTER_PRINT_CN;
    machine_menu.HaveUps           = HAVE_UPS_CN;
    machine_menu.Z2andZ2Endstop    = Z2_AND_Z2ENDSTOP_CONF_CN;
    machine_menu.EnablePinsInvert  = ENABLE_PINS_CONF_CN;

    machine_menu.Z2ConfTitle     = Z2_AND_Z2ENDSTOP_CONF_TITLE_CN;
    machine_menu.Z2Enable        = Z2_ENABLE_CN;
    machine_menu.Z2EndstopEnable = Z2_ENDSTOP_CN;
    machine_menu.Z2Port          = Z2_PORT_CN;

    machine_menu.EnablePinsInvertTitle = ENABLE_PINS_CONF_TITLE_CN;
    machine_menu.XInvert               = X_ENABLE_PINS_INVERT_CN;
    machine_menu.YInvert               = Y_ENABLE_PINS_INVERT_CN;
    machine_menu.ZInvert               = Z_ENABLE_PINS_INVERT_CN;
    machine_menu.EInvert               = E_ENABLE_PINS_INVERT_CN;

    machine_menu.key_back    = KEY_BACK_CN;
    machine_menu.key_reset   = KEY_REST_CN;
    machine_menu.key_confirm = KEY_CONFIRM_CN;

    machine_menu.PausePosText   = PAUSE_POSITION_CN;
    machine_menu.xPos           = PAUSE_POSITION_X_CN;
    machine_menu.yPos           = PAUSE_POSITION_Y_CN;
    machine_menu.zPos           = PAUSE_POSITION_Z_CN;
    machine_menu.WifiConfTitle  = WIFI_SETTINGS_TITLE_CN;
    machine_menu.wifiMode       = WIFI_SETTINGS_MODE_CN;
    machine_menu.wifiName       = WIFI_SETTINGS_NAME_CN;
    machine_menu.wifiPassWord   = WIFI_SETTINGS_PASSWORD_CN;
    machine_menu.wifiCloud      = WIFI_SETTINGS_CLOUD_CN;
    machine_menu.wifiConfig     = WIFI_SETTINGS_CONFIG_CN;
    machine_menu.wifiEdit       = WIFI_SETTINGS_EDIT_CN;
    machine_menu.wifiConfigTips = WIFI_CONFIG_TIPS_CN;

    machine_menu.OffsetConfTitle = OFFSET_TITLE_CN;
    machine_menu.Xoffset         = OFFSET_X_CN;
    machine_menu.Yoffset         = OFFSET_Y_CN;
    machine_menu.Zoffset         = OFFSET_Z_CN;

    machine_menu.HomingSensitivityConfTitle = HOMING_SENSITIVITY_CONF_TITLE_CN;
    machine_menu.X_Sensitivity              = X_SENSITIVITY_CN;
    machine_menu.Y_Sensitivity              = Y_SENSITIVITY_CN;
    machine_menu.Z_Sensitivity              = Z_SENSITIVITY_CN;
    machine_menu.Z2_Sensitivity             = Z2_SENSITIVITY_CN;

    machine_menu.EncoderConfTitle = ENCODER_CONF_TITLE_CN;
    machine_menu.EncoderConfText              = ENCODER_CONF_TEXT_CN;
  }
  else if (gCfgItems.language == LANG_COMPLEX_CHINESE) {
    MachinePara_menu.title              = MACHINE_PARA_TITLE_T_CN;
    MachinePara_menu.MachineSetting     = MACHINE_TYPE_CNOFIG_T_CN;
    MachinePara_menu.MotorSetting       = MOTOR_CONFIG_T_CN;
    MachinePara_menu.leveling           = MACHINE_LEVELING_CONFIG_T_CN;
    MachinePara_menu.AdvanceSetting     = ADVANCE_CONFIG_T_CN;

    machine_menu.default_value = DEFAULT_T_CN;
    machine_menu.next          = NEXT_T_CN;
    machine_menu.previous      = PREVIOUS_T_CN;

    machine_menu.MachineConfigTitle = MACHINE_CONFIG_TITLE_T_CN;
    machine_menu.MachineType        = MACHINE_TYPE_T_CN;
    machine_menu.Stroke             = MACHINE_STROKE_T_CN;
    machine_menu.HomeDir            = MACHINE_HOMEDIR_T_CN;
    machine_menu.EndStopType        = MACHINE_ENDSTOP_TYPE_T_CN;
    machine_menu.FilamentConf       = MACHINE_FILAMENT_CONFIG_T_CN;

    machine_menu.MachineTypeConfTitle = MACHINE_TYPE_CONFIG_TITLE_T_CN;
    machine_menu.xyz                  = MACHINE_TYPE_XYZ_T_CN;
    machine_menu.delta                = MACHINE_TYPE_DELTA_T_CN;
    machine_menu.corexy               = MACHINE_TYPE_COREXY_T_CN;

    machine_menu.StrokeConfTitle = MACHINE_STROKE_CONF_TITLE_T_CN;
    machine_menu.xStroke         = X_MAX_LENGTH_T_CN;
    machine_menu.yStroke         = Y_MAX_LENGTH_T_CN;
    machine_menu.zStroke         = Z_MAX_LENGTH_T_CN;

    machine_menu.xmin = X_MIN_LENGTH_T_CN;
    machine_menu.ymin = Y_MIN_LENGTH_T_CN;
    machine_menu.zmin = Z_MIN_LENGTH_T_CN;

    machine_menu.HomeDirConfTitle = HOME_DIR_CONF_TITLE_T_CN;
    machine_menu.xHomeDir         = HOME_DIR_X_T_CN;
    machine_menu.yHomeDir         = HOME_DIR_Y_T_CN;
    machine_menu.zHomeDir         = HOME_DIR_Z_T_CN;
    machine_menu.min              = HOME_MIN_T_CN;
    machine_menu.max              = HOME_MAX_T_CN;

    machine_menu.EndstopConfTitle = ENDSTOP_CONF_TITLE_T_CN;
    machine_menu.xEndstop_min     = MIN_ENDSTOP_X_T_CN;
    machine_menu.yEndstop_min     = MIN_ENDSTOP_Y_T_CN;
    machine_menu.zEndstop_min     = MIN_ENDSTOP_Z_T_CN;
    machine_menu.xEndstop_max     = MAX_ENDSTOP_X_T_CN;
    machine_menu.yEndstop_max     = MAX_ENDSTOP_Y_T_CN;
    machine_menu.zEndstop_max     = MAX_ENDSTOP_Z_T_CN;
    machine_menu.FilamentEndstop  = ENDSTOP_FIL_T_CN;
    machine_menu.LevelingEndstop  = ENDSTOP_LEVEL_T_CN;
    machine_menu.opened           = ENDSTOP_OPENED_T_CN;
    machine_menu.closed           = ENDSTOP_CLOSED_T_CN;

    machine_menu.FilamentConfTitle   = FILAMENT_CONF_TITLE_T_CN;
    machine_menu.InLength            = FILAMENT_IN_LENGTH_T_CN;
    machine_menu.InSpeed             = FILAMENT_IN_SPEED_T_CN;
    machine_menu.FilamentTemperature = FILAMENT_TEMPERATURE_T_CN;
    machine_menu.OutLength           = FILAMENT_OUT_LENGTH_T_CN;
    machine_menu.OutSpeed            = FILAMENT_OUT_SPEED_T_CN;

    machine_menu.LevelingParaConfTitle   = LEVELING_CONF_TITLE_T_CN;
    machine_menu.LevelingParaConf        = LEVELING_PARA_CONF_T_CN;
    machine_menu.TrammingPosConf         = TRAMMING_POS_T_CN;
    machine_menu.LevelingAutoCommandConf = LEVELING_AUTO_COMMAND_T_CN;
    machine_menu.LevelingAutoZoffsetConf = LEVELING_AUTO_ZOFFSET_T_CN;

    machine_menu.LevelingSubConfTitle = LEVELING_PARA_CONF_TITLE_T_CN;
    machine_menu.AutoLevelEnable      = AUTO_LEVELING_ENABLE_T_CN;
    machine_menu.BLtouchEnable        = BLTOUCH_LEVELING_ENABLE_T_CN;
    machine_menu.ProbePort            = PROBE_PORT_T_CN;
    machine_menu.ProbeXoffset         = PROBE_X_OFFSET_T_CN;
    machine_menu.ProbeYoffset         = PROBE_Y_OFFSET_T_CN;
    machine_menu.ProbeZoffset         = PROBE_Z_OFFSET_T_CN;
    machine_menu.ProbeXYspeed         = PROBE_XY_SPEED_T_CN;
    machine_menu.ProbeZspeed          = PROBE_Z_SPEED_T_CN;
    machine_menu.enable               = ENABLE_T_CN;
    machine_menu.disable              = DISABLE_T_CN;
    machine_menu.locked               = LOCKED_T_CN;
    machine_menu.z_min                = Z_MIN_T_CN;
    machine_menu.z_max                = Z_MAX_T_CN;

    machine_menu.LevelingSubDeltaConfTitle = DELTA_LEVEL_CONF_TITLE_T_CN;
    machine_menu.MachineRadius             = DELTA_MACHINE_RADIUS_T_CN;
    machine_menu.DiagonalRod               = DELTA_DIAGONAL_ROD_T_CN;
    machine_menu.PrintableRadius           = DELTA_PRINT_RADIUS_T_CN;
    machine_menu.DeltaHeight               = DELTA_HEIGHT_T_CN;
    machine_menu.SmoothRodOffset           = SMOOTH_ROD_OFFSET_T_CN;
    machine_menu.EffectorOffset            = EFFECTOR_OFFSET_T_CN;
    machine_menu.CalibrationRadius         = CALIBRATION_RADIUS_T_CN;

    machine_menu.LevelingSubXYZConfTitle = XYZ_LEVEL_CONF_TITLE_T_CN;

    machine_menu.TemperatureConfTitle = TEMPERATURE_CONF_TITLE_T_CN;
    machine_menu.NozzleConf           = NOZZLE_CONF_T_CN;
    machine_menu.HotBedConf           = HOTBED_CONF_T_CN;
    machine_menu.PreheatTemperConf    = PREHEAT_TEMPER_T_CN;

    machine_menu.NozzleConfTitle      = NOZZLE_CONF_TITLE_T_CN;
    machine_menu.NozzleCnt            = NOZZLECNT_T_CN;
    machine_menu.NozzleType           = NOZZLE_TYPE_T_CN;
    machine_menu.NozzleAdjustType     = NOZZLE_ADJUST_TYPE_T_CN;
    machine_menu.NozzleMinTemperature = NOZZLE_MIN_TEMPERATURE_T_CN;
    machine_menu.NozzleMaxTemperature = NOZZLE_MAX_TEMPERATURE_T_CN;
    machine_menu.Extrude_Min_Temper   = EXTRUD_MIN_TEMPER_T_CN;

    machine_menu.HotbedConfTitle      = HOTBED_CONF_TITLE_T_CN;
    machine_menu.HotbedAdjustType     = HOTBED_ADJUST_T_CN;
    machine_menu.HotbedMinTemperature = HOTBED_MIN_TEMPERATURE_T_CN;
    machine_menu.HotbedMaxTemperature = HOTBED_MAX_TEMPERATURE_T_CN;

    machine_menu.MotorConfTitle        = MOTOR_CONF_TITLE_T_CN;
    machine_menu.MaxFeedRateConf       = MAXFEEDRATE_CONF_T_CN;
    machine_menu.AccelerationConf      = ACCELERATION_CONF_T_CN;
    machine_menu.JerkConf              = JERKCONF_T_CN;
    machine_menu.StepsConf             = STEPSCONF_T_CN;
    machine_menu.TMCcurrentConf        = TMC_CURRENT_T_CN;
    machine_menu.TMCStepModeConf       = TMC_STEP_MODE_T_CN;
    machine_menu.MotorDirConf          = MOTORDIRCONF_T_CN;
    machine_menu.HomeFeedRateConf      = HOMEFEEDRATECONF_T_CN;
    machine_menu.PausePosition         = PAUSE_POSITION_T_CN;
    machine_menu.WifiSettings          = WIFI_SETTINGS_T_CN;
    machine_menu.HomingSensitivityConf = HOMING_SENSITIVITY_CONF_T_CN;
    machine_menu.EncoderSettings         = ENCODER_SETTINGS_T_CN;

    machine_menu.MaxFeedRateConfTitle = MAXFEEDRATE_CONF_TITLE_T_CN;
    machine_menu.XMaxFeedRate         = X_MAXFEEDRATE_T_CN;
    machine_menu.YMaxFeedRate         = Y_MAXFEEDRATE_T_CN;
    machine_menu.ZMaxFeedRate         = Z_MAXFEEDRATE_T_CN;
    machine_menu.E0MaxFeedRate        = E0_MAXFEEDRATE_T_CN;
    machine_menu.E1MaxFeedRate        = E1_MAXFEEDRATE_T_CN;

    machine_menu.AccelerationConfTitle = ACCELERATION_CONF_TITLE_T_CN;
    machine_menu.PrintAcceleration     = PRINT_ACCELERATION_T_CN;
    machine_menu.RetractAcceleration   = RETRACT_ACCELERATION_T_CN;
    machine_menu.TravelAcceleration    = TRAVEL_ACCELERATION_T_CN;
    machine_menu.X_Acceleration        = X_ACCELERATION_T_CN;
    machine_menu.Y_Acceleration        = Y_ACCELERATION_T_CN;
    machine_menu.Z_Acceleration        = Z_ACCELERATION_T_CN;
    machine_menu.E0_Acceleration       = E0_ACCELERATION_T_CN;
    machine_menu.E1_Acceleration       = E1_ACCELERATION_T_CN;

    machine_menu.JerkConfTitle = JERK_CONF_TITLE_T_CN;
    machine_menu.X_Jerk        = X_JERK_T_CN;
    machine_menu.Y_Jerk        = Y_JERK_T_CN;
    machine_menu.Z_Jerk        = Z_JERK_T_CN;
    machine_menu.E_Jerk        = E_JERK_T_CN;

    machine_menu.StepsConfTitle = STEPS_CONF_TITLE_T_CN;
    machine_menu.X_Steps        = X_STEPS_T_CN;
    machine_menu.Y_Steps        = Y_STEPS_T_CN;
    machine_menu.Z_Steps        = Z_STEPS_T_CN;
    machine_menu.E0_Steps       = E0_STEPS_T_CN;
    machine_menu.E1_Steps       = E1_STEPS_T_CN;

    machine_menu.TmcCurrentConfTitle = TMC_CURRENT_CONF_TITLE_T_CN;
    machine_menu.X_Current           = X_TMC_CURRENT_T_CN;
    machine_menu.Y_Current           = Y_TMC_CURRENT_T_CN;
    machine_menu.Z_Current           = Z_TMC_CURRENT_T_CN;
    machine_menu.E0_Current          = E0_TMC_CURRENT_T_CN;
    machine_menu.E1_Current          = E1_TMC_CURRENT_T_CN;

    machine_menu.TmcStepModeConfTitle = TMC_MODE_CONF_TITLE_T_CN;
    machine_menu.X_StepMode           = X_TMC_MODE_T_CN;
    machine_menu.Y_StepMode           = Y_TMC_MODE_T_CN;
    machine_menu.Z_StepMode           = Z_TMC_MODE_T_CN;
    machine_menu.E0_StepMode          = E0_TMC_MODE_T_CN;
    machine_menu.E1_StepMode          = E1_TMC_MODE_T_CN;

    machine_menu.MotorDirConfTitle = MOTORDIR_CONF_TITLE_T_CN;
    machine_menu.X_MotorDir        = X_MOTORDIR_T_CN;
    machine_menu.Y_MotorDir        = Y_MOTORDIR_T_CN;
    machine_menu.Z_MotorDir        = Z_MOTORDIR_T_CN;
    machine_menu.E0_MotorDir       = E0_MOTORDIR_T_CN;
    machine_menu.E1_MotorDir       = E1_MOTORDIR_T_CN;
    machine_menu.Invert_0          = INVERT_P_T_CN;
    machine_menu.Invert_1          = INVERT_N_T_CN;

    machine_menu.HomeFeedRateConfTitle = HOMEFEEDRATE_CONF_TITLE_T_CN;
    machine_menu.XY_HomeFeedRate       = X_HOMESPEED_T_CN;
    machine_menu.Z_HomeFeedRate = Z_HOMESPEED_T_CN;

    machine_menu.AdvancedConfTitle = ADVANCED_CONF_TITLE_T_CN;
    machine_menu.PwrOffDection     = PWROFF_DECTION_T_CN;
    machine_menu.PwrOffAfterPrint  = PWROFF_AFTER_PRINT_T_CN;
    machine_menu.HaveUps           = HAVE_UPS_T_CN;
    machine_menu.Z2andZ2Endstop    = Z2_AND_Z2ENDSTOP_CONF_T_CN;
    machine_menu.EnablePinsInvert  = ENABLE_PINS_CONF_T_CN;

    machine_menu.Z2ConfTitle     = Z2_AND_Z2ENDSTOP_CONF_TITLE_T_CN;
    machine_menu.Z2Enable        = Z2_ENABLE_T_CN;
    machine_menu.Z2EndstopEnable = Z2_ENDSTOP_T_CN;
    machine_menu.Z2Port          = Z2_PORT_T_CN;

    machine_menu.EnablePinsInvertTitle = ENABLE_PINS_CONF_TITLE_T_CN;
    machine_menu.XInvert               = X_ENABLE_PINS_INVERT_T_CN;
    machine_menu.YInvert               = Y_ENABLE_PINS_INVERT_T_CN;
    machine_menu.ZInvert               = Z_ENABLE_PINS_INVERT_T_CN;
    machine_menu.EInvert               = E_ENABLE_PINS_INVERT_T_CN;

    machine_menu.key_back    = KEY_BACK_T_CN;
    machine_menu.key_reset   = KEY_REST_T_CN;
    machine_menu.key_confirm = KEY_CONFIRM_T_CN;

    machine_menu.PausePosText = PAUSE_POSITION_T_CN;
    machine_menu.xPos         = PAUSE_POSITION_X_T_CN;
    machine_menu.yPos         = PAUSE_POSITION_Y_T_CN;
    machine_menu.zPos         = PAUSE_POSITION_Z_T_CN;

    machine_menu.WifiConfTitle  = WIFI_SETTINGS_TITLE_T_CN;
    machine_menu.wifiMode       = WIFI_SETTINGS_MODE_T_CN;
    machine_menu.wifiName       = WIFI_SETTINGS_NAME_T_CN;
    machine_menu.wifiPassWord   = WIFI_SETTINGS_PASSWORD_T_CN;
    machine_menu.wifiCloud      = WIFI_SETTINGS_CLOUD_T_CN;
    machine_menu.wifiConfig     = WIFI_SETTINGS_CONFIG_T_CN;
    machine_menu.wifiEdit       = WIFI_SETTINGS_EDIT_T_CN;
    machine_menu.wifiConfigTips = WIFI_CONFIG_TIPS_T_CN;

    machine_menu.OffsetConfTitle = OFFSET_TITLE_T_CN;
    machine_menu.Xoffset         = OFFSET_X_T_CN;
    machine_menu.Yoffset         = OFFSET_Y_T_CN;
    machine_menu.Zoffset         = OFFSET_Z_T_CN;

    machine_menu.HomingSensitivityConfTitle = HOMING_SENSITIVITY_CONF_TITLE_T_CN;
    machine_menu.X_Sensitivity              = X_SENSITIVITY_T_CN;
    machine_menu.Y_Sensitivity              = Y_SENSITIVITY_T_CN;
    machine_menu.Z_Sensitivity              = Z_SENSITIVITY_T_CN;
    machine_menu.Z2_Sensitivity             = Z2_SENSITIVITY_T_CN;

    machine_menu.EncoderConfTitle = ENCODER_CONF_TITLE_T_CN;
    machine_menu.EncoderConfText              = ENCODER_CONF_TEXT_T_CN;
  }
  else {
    MachinePara_menu.title              = MACHINE_PARA_TITLE_EN;
    MachinePara_menu.MachineSetting     = MACHINE_TYPE_CNOFIG_EN;
    MachinePara_menu.MotorSetting       = MOTOR_CONFIG_EN;
    MachinePara_menu.leveling           = MACHINE_LEVELING_CONFIG_EN;
    MachinePara_menu.AdvanceSetting     = ADVANCE_CONFIG_EN;

    machine_menu.default_value = DEFAULT_EN;
    machine_menu.next          = NEXT_EN;
    machine_menu.previous      = PREVIOUS_EN;

    machine_menu.MachineConfigTitle = MACHINE_CONFIG_TITLE_EN;
    machine_menu.MachineType        = MACHINE_TYPE_EN;
    machine_menu.Stroke             = MACHINE_STROKE_EN;
    machine_menu.HomeDir            = MACHINE_HOMEDIR_EN;
    machine_menu.EndStopType        = MACHINE_ENDSTOP_TYPE_EN;
    machine_menu.FilamentConf       = MACHINE_FILAMENT_CONFIG_EN;

    machine_menu.MachineTypeConfTitle = MACHINE_TYPE_CONFIG_TITLE_EN;
    machine_menu.xyz                  = MACHINE_TYPE_XYZ_EN;
    machine_menu.delta                = MACHINE_TYPE_DELTA_EN;
    machine_menu.corexy               = MACHINE_TYPE_COREXY_EN;

    machine_menu.StrokeConfTitle = MACHINE_STROKE_CONF_TITLE_EN;
    machine_menu.xStroke         = X_MAX_LENGTH_EN;
    machine_menu.yStroke         = Y_MAX_LENGTH_EN;
    machine_menu.zStroke         = Z_MAX_LENGTH_EN;

    machine_menu.xmin = X_MIN_LENGTH_EN;
    machine_menu.ymin = Y_MIN_LENGTH_EN;
    machine_menu.zmin = Z_MIN_LENGTH_EN;

    machine_menu.HomeDirConfTitle = HOME_DIR_CONF_TITLE_EN;
    machine_menu.xHomeDir         = HOME_DIR_X_EN;
    machine_menu.yHomeDir         = HOME_DIR_Y_EN;
    machine_menu.zHomeDir         = HOME_DIR_Z_EN;
    machine_menu.min              = HOME_MIN_EN;
    machine_menu.max              = HOME_MAX_EN;

    machine_menu.EndstopConfTitle = ENDSTOP_CONF_TITLE_EN;
    machine_menu.xEndstop_min     = MIN_ENDSTOP_X_EN;
    machine_menu.yEndstop_min     = MIN_ENDSTOP_Y_EN;
    machine_menu.zEndstop_min     = MIN_ENDSTOP_Z_EN;
    machine_menu.xEndstop_max     = MAX_ENDSTOP_X_EN;
    machine_menu.yEndstop_max     = MAX_ENDSTOP_Y_EN;
    machine_menu.zEndstop_max     = MAX_ENDSTOP_Z_EN;
    machine_menu.FilamentEndstop  = ENDSTOP_FIL_EN;
    machine_menu.LevelingEndstop  = ENDSTOP_LEVEL_EN;
    machine_menu.opened           = ENDSTOP_OPENED_EN;
    machine_menu.closed           = ENDSTOP_CLOSED_EN;

    machine_menu.FilamentConfTitle   = FILAMENT_CONF_TITLE_EN;
    machine_menu.InLength            = FILAMENT_IN_LENGTH_EN;
    machine_menu.InSpeed             = FILAMENT_IN_SPEED_EN;
    machine_menu.FilamentTemperature = FILAMENT_TEMPERATURE_EN;
    machine_menu.OutLength           = FILAMENT_OUT_LENGTH_EN;
    machine_menu.OutSpeed            = FILAMENT_OUT_SPEED_EN;

    machine_menu.LevelingZoffsetTitle = LEVELING_ZOFFSET_TITLE_EN;

    machine_menu.LevelingParaConfTitle   = LEVELING_CONF_TITLE_EN;
    machine_menu.LevelingParaConf        = LEVELING_PARA_CONF_EN;
    machine_menu.TrammingPosConf         = TRAMMING_POS_EN;
    machine_menu.LevelingAutoCommandConf = LEVELING_AUTO_COMMAND_EN;
    machine_menu.LevelingAutoZoffsetConf = LEVELING_AUTO_ZOFFSET_EN;

    machine_menu.LevelingSubConfTitle = LEVELING_PARA_CONF_TITLE_EN;
    machine_menu.AutoLevelEnable      = AUTO_LEVELING_ENABLE_EN;
    machine_menu.BLtouchEnable        = BLTOUCH_LEVELING_ENABLE_EN;
    machine_menu.ProbePort            = PROBE_PORT_EN;
    machine_menu.ProbeXoffset         = PROBE_X_OFFSET_EN;
    machine_menu.ProbeYoffset         = PROBE_Y_OFFSET_EN;
    machine_menu.ProbeZoffset         = PROBE_Z_OFFSET_EN;
    machine_menu.ProbeXYspeed         = PROBE_XY_SPEED_EN;
    machine_menu.ProbeZspeed          = PROBE_Z_SPEED_EN;
    machine_menu.enable               = ENABLE_EN;
    machine_menu.disable              = DISABLE_EN;
    machine_menu.locked               = LOCKED_EN;
    machine_menu.z_min                = Z_MIN_EN;
    machine_menu.z_max                = Z_MAX_EN;

    machine_menu.LevelingSubDeltaConfTitle = DELTA_LEVEL_CONF_TITLE_EN;
    machine_menu.MachineRadius             = DELTA_MACHINE_RADIUS_EN;
    machine_menu.DiagonalRod               = DELTA_DIAGONAL_ROD_EN;
    machine_menu.PrintableRadius           = DELTA_PRINT_RADIUS_EN;
    machine_menu.DeltaHeight               = DELTA_HEIGHT_EN;
    machine_menu.SmoothRodOffset           = SMOOTH_ROD_OFFSET_EN;
    machine_menu.EffectorOffset            = EFFECTOR_OFFSET_EN;
    machine_menu.CalibrationRadius         = CALIBRATION_RADIUS_EN;

    machine_menu.LevelingSubXYZConfTitle = XYZ_LEVEL_CONF_TITLE_EN;

    machine_menu.TemperatureConfTitle = TEMPERATURE_CONF_TITLE_EN;
    machine_menu.NozzleConf           = NOZZLE_CONF_EN;
    machine_menu.HotBedConf           = HOTBED_CONF_EN;
    machine_menu.PreheatTemperConf    = PREHEAT_TEMPER_EN;

    machine_menu.NozzleConfTitle      = NOZZLE_CONF_TITLE_EN;
    machine_menu.NozzleCnt            = NOZZLECNT_EN;
    machine_menu.NozzleType           = NOZZLE_TYPE_EN;
    machine_menu.NozzleAdjustType     = NOZZLE_ADJUST_TYPE_EN;
    machine_menu.NozzleMinTemperature = NOZZLE_MIN_TEMPERATURE_EN;
    machine_menu.NozzleMaxTemperature = NOZZLE_MAX_TEMPERATURE_EN;
    machine_menu.Extrude_Min_Temper   = EXTRUD_MIN_TEMPER_EN;

    machine_menu.HotbedEnable         = HOTBED_ENABLE_EN;
    machine_menu.HotbedConfTitle      = HOTBED_CONF_TITLE_EN;
    machine_menu.HotbedAdjustType     = HOTBED_ADJUST_EN;
    machine_menu.HotbedMinTemperature = HOTBED_MIN_TEMPERATURE_EN;
    machine_menu.HotbedMaxTemperature = HOTBED_MAX_TEMPERATURE_EN;

    machine_menu.MotorConfTitle        = MOTOR_CONF_TITLE_EN;
    machine_menu.MaxFeedRateConf       = MAXFEEDRATE_CONF_EN;
    machine_menu.AccelerationConf      = ACCELERATION_CONF_EN;
    machine_menu.JerkConf              = JERKCONF_EN;
    machine_menu.StepsConf             = STEPSCONF_EN;
    machine_menu.TMCcurrentConf        = TMC_CURRENT_EN;
    machine_menu.TMCStepModeConf       = TMC_STEP_MODE_EN;
    machine_menu.MotorDirConf          = MOTORDIRCONF_EN;
    machine_menu.HomeFeedRateConf      = HOMEFEEDRATECONF_EN;
    machine_menu.PausePosition         = PAUSE_POSITION_EN;
    machine_menu.WifiSettings          = WIFI_SETTINGS_EN;
    machine_menu.HomingSensitivityConf = HOMING_SENSITIVITY_CONF_EN;
    machine_menu.EncoderSettings         = ENCODER_SETTINGS_EN;

    machine_menu.MaxFeedRateConfTitle = MAXFEEDRATE_CONF_TITLE_EN;
    machine_menu.XMaxFeedRate         = X_MAXFEEDRATE_EN;
    machine_menu.YMaxFeedRate         = Y_MAXFEEDRATE_EN;
    machine_menu.ZMaxFeedRate         = Z_MAXFEEDRATE_EN;
    machine_menu.E0MaxFeedRate        = E0_MAXFEEDRATE_EN;
    machine_menu.E1MaxFeedRate        = E1_MAXFEEDRATE_EN;

    machine_menu.AccelerationConfTitle = ACCELERATION_CONF_TITLE_EN;
    machine_menu.PrintAcceleration     = PRINT_ACCELERATION_EN;
    machine_menu.RetractAcceleration   = RETRACT_ACCELERATION_EN;
    machine_menu.TravelAcceleration    = TRAVEL_ACCELERATION_EN;
    machine_menu.X_Acceleration        = X_ACCELERATION_EN;
    machine_menu.Y_Acceleration        = Y_ACCELERATION_EN;
    machine_menu.Z_Acceleration        = Z_ACCELERATION_EN;
    machine_menu.E0_Acceleration       = E0_ACCELERATION_EN;
    machine_menu.E1_Acceleration       = E1_ACCELERATION_EN;

    machine_menu.JerkConfTitle = JERK_CONF_TITLE_EN;
    machine_menu.X_Jerk        = X_JERK_EN;
    machine_menu.Y_Jerk        = Y_JERK_EN;
    machine_menu.Z_Jerk        = Z_JERK_EN;
    machine_menu.E_Jerk        = E_JERK_EN;

    machine_menu.StepsConfTitle = STEPS_CONF_TITLE_EN;
    machine_menu.X_Steps        = X_STEPS_EN;
    machine_menu.Y_Steps        = Y_STEPS_EN;
    machine_menu.Z_Steps        = Z_STEPS_EN;
    machine_menu.E0_Steps       = E0_STEPS_EN;
    machine_menu.E1_Steps       = E1_STEPS_EN;

    machine_menu.TmcCurrentConfTitle = TMC_CURRENT_CONF_TITLE_EN;
    machine_menu.X_Current           = X_TMC_CURRENT_EN;
    machine_menu.Y_Current           = Y_TMC_CURRENT_EN;
    machine_menu.Z_Current           = Z_TMC_CURRENT_EN;
    machine_menu.E0_Current          = E0_TMC_CURRENT_EN;
    machine_menu.E1_Current          = E1_TMC_CURRENT_EN;

    machine_menu.TmcStepModeConfTitle = TMC_MODE_CONF_TITLE_EN;
    machine_menu.X_StepMode           = X_TMC_MODE_EN;
    machine_menu.Y_StepMode           = Y_TMC_MODE_EN;
    machine_menu.Z_StepMode           = Z_TMC_MODE_EN;
    machine_menu.E0_StepMode          = E0_TMC_MODE_EN;
    machine_menu.E1_StepMode          = E1_TMC_MODE_EN;

    machine_menu.MotorDirConfTitle = MOTORDIR_CONF_TITLE_EN;
    machine_menu.X_MotorDir        = X_MOTORDIR_EN;
    machine_menu.Y_MotorDir        = Y_MOTORDIR_EN;
    machine_menu.Z_MotorDir        = Z_MOTORDIR_EN;
    machine_menu.E0_MotorDir       = E0_MOTORDIR_EN;
    machine_menu.E1_MotorDir       = E1_MOTORDIR_EN;
    machine_menu.Invert_0          = INVERT_P_EN;
    machine_menu.Invert_1          = INVERT_N_EN;

    machine_menu.HomeFeedRateConfTitle = HOMEFEEDRATE_CONF_TITLE_EN;
    machine_menu.XY_HomeFeedRate       = X_HOMESPEED_EN;
    machine_menu.Z_HomeFeedRate = Z_HOMESPEED_EN;

    machine_menu.AdvancedConfTitle = ADVANCED_CONF_TITLE_EN;
    machine_menu.PwrOffDection     = PWROFF_DECTION_EN;
    machine_menu.PwrOffAfterPrint  = PWROFF_AFTER_PRINT_EN;
    machine_menu.HaveUps           = HAVE_UPS_EN;
    machine_menu.Z2andZ2Endstop    = Z2_AND_Z2ENDSTOP_CONF_EN;
    machine_menu.EnablePinsInvert  = ENABLE_PINS_CONF_EN;

    machine_menu.Z2ConfTitle     = Z2_AND_Z2ENDSTOP_CONF_TITLE_EN;
    machine_menu.Z2Enable        = Z2_ENABLE_EN;
    machine_menu.Z2EndstopEnable = Z2_ENDSTOP_EN;
    machine_menu.Z2Port          = Z2_PORT_EN;

    machine_menu.EnablePinsInvertTitle = ENABLE_PINS_CONF_TITLE_EN;
    machine_menu.XInvert               = X_ENABLE_PINS_INVERT_EN;
    machine_menu.YInvert               = Y_ENABLE_PINS_INVERT_EN;
    machine_menu.ZInvert               = Z_ENABLE_PINS_INVERT_EN;
    machine_menu.EInvert               = E_ENABLE_PINS_INVERT_EN;

    machine_menu.key_back    = KEY_BACK_EN;
    machine_menu.key_reset   = KEY_REST_EN;
    machine_menu.key_confirm = KEY_CONFIRM_EN;
    //
    machine_menu.high_level = MOTOR_EN_HIGH_LEVEL_EN;
    machine_menu.low_level  = MOTOR_EN_LOW_LEVEL_EN;

    machine_menu.PausePosText   = PAUSE_POSITION_EN;
    machine_menu.xPos           = PAUSE_POSITION_X_EN;
    machine_menu.yPos           = PAUSE_POSITION_Y_EN;
    machine_menu.zPos           = PAUSE_POSITION_Z_EN;
    machine_menu.WifiConfTitle  = WIFI_SETTINGS_TITLE_EN;
    machine_menu.wifiMode       = WIFI_SETTINGS_MODE_EN;
    machine_menu.wifiName       = WIFI_SETTINGS_NAME_EN;
    machine_menu.wifiPassWord   = WIFI_SETTINGS_PASSWORD_EN;
    machine_menu.wifiCloud      = WIFI_SETTINGS_CLOUD_EN;
    machine_menu.wifiConfig     = WIFI_SETTINGS_CONFIG_EN;
    machine_menu.wifiEdit       = WIFI_SETTINGS_EDIT_EN;
    machine_menu.wifiConfigTips = WIFI_CONFIG_TIPS_EN;

    machine_menu.OffsetConfTitle = OFFSET_TITLE_EN;
    machine_menu.Xoffset         = OFFSET_X_EN;
    machine_menu.Yoffset         = OFFSET_Y_EN;
    machine_menu.Zoffset         = OFFSET_Z_EN;

    machine_menu.HomingSensitivityConfTitle = HOMING_SENSITIVITY_CONF_TITLE_EN;
    machine_menu.X_Sensitivity              = X_SENSITIVITY_EN;
    machine_menu.Y_Sensitivity              = Y_SENSITIVITY_EN;
    machine_menu.Z_Sensitivity              = Z_SENSITIVITY_EN;
    machine_menu.Z2_Sensitivity             = Z2_SENSITIVITY_EN;

    machine_menu.EncoderConfTitle = ENCODER_CONF_TITLE_EN;
    machine_menu.EncoderConfText              = ENCODER_CONF_TEXT_EN;
  }
}

void disp_language_init() {
  preheat_menu.value_state = TEXT_VALUE;
  preheat_menu.step_1c     = TEXT_1C;
  preheat_menu.step_5c     = TEXT_5C;
  preheat_menu.step_10c    = TEXT_10C;

  move_menu.x_add = AXIS_X_ADD_TEXT;
  move_menu.x_dec = AXIS_X_DEC_TEXT;
  move_menu.y_add = AXIS_Y_ADD_TEXT;
  move_menu.y_dec = AXIS_Y_DEC_TEXT;
  move_menu.z_add = AXIS_Z_ADD_TEXT;
  move_menu.z_dec = AXIS_Z_DEC_TEXT;

  move_menu.step_001mm  = TEXT_001MM;
  move_menu.step_0025mm = TEXT_0025MM;
  move_menu.step_005mm  = TEXT_005MM;
  move_menu.step_01mm   = TEXT_01MM;
  move_menu.step_1mm    = TEXT_1MM;
  move_menu.step_10mm   = TEXT_10MM;

  home_menu.home_x   = HOME_X_TEXT;
  home_menu.home_y   = HOME_Y_TEXT;
  home_menu.home_z   = HOME_Z_TEXT;
  home_menu.home_all = HOME_ALL_TEXT;

  extrude_menu.temp_value     = TEXT_VALUE_T;
  extrude_menu.count_value_mm = TEXT_VALUE_mm;
  extrude_menu.count_value_cm = TEXT_VALUE_cm;
  extrude_menu.count_value_m  = TEXT_VALUE_m;
  extrude_menu.step_1mm       = EXTRUDE_1MM_TEXT;
  extrude_menu.step_5mm       = EXTRUDE_5MM_TEXT;
  extrude_menu.step_10mm      = EXTRUDE_10MM_TEXT;

  fan_menu.full = FAN_OPEN_TEXT;
  fan_menu.half = FAN_HALF_TEXT;
  fan_menu.off  = FAN_CLOSE_TEXT;

  speed_menu.step_1percent  = STEP_1PERCENT;
  speed_menu.step_5percent  = STEP_5PERCENT;
  speed_menu.step_10percent = STEP_10PERCENT;

  language_menu.chinese_s  = LANGUAGE_S_CN;
  language_menu.chinese_t  = LANGUAGE_T_CN;
  language_menu.english    = LANGUAGE_EN;
  language_menu.russian    = LANGUAGE_RU;
  language_menu.spanish    = LANGUAGE_SP;
  language_menu.german     = LANGUAGE_GE;
  language_menu.japan      = LANGUAGE_JP;
  language_menu.korean     = LANGUAGE_KR;
  language_menu.portuguese = LANGUAGE_PR;
  language_menu.italy      = LANGUAGE_IT;
  language_menu.brazil     = LANGUAGE_BR;
  language_menu.french     = LANGUAGE_FR;

  about_menu.type_name  = ABOUT_TYPE_TEXT;
  about_menu.firmware_v = ABOUT_VERSION_TEXT;

  wifi_menu.ip           = WIFI_IP_TEXT;
  wifi_menu.wifi         = WIFI_NAME_TEXT;
  wifi_menu.key          = WIFI_KEY_TEXT;
  wifi_menu.state_ap     = WIFI_STATE_AP_TEXT;
  wifi_menu.state_sta    = WIFI_STATE_STA_TEXT;
  wifi_menu.connected    = WIFI_CONNECTED_TEXT;
  wifi_menu.disconnected = WIFI_DISCONNECTED_TEXT;
  wifi_menu.exception    = WIFI_EXCEPTION_TEXT;

  printing_menu.temp1    = TEXT_VALUE_TARGET;
  printing_menu.temp2    = TEXT_VALUE_TARGET;
  printing_menu.bed_temp = TEXT_VALUE_TARGET;

  filament_menu.stat_temp = TEXT_VALUE;

  media_select_menu.title    = MEDIA_SELECT_TITLE_EN;
  media_select_menu.sd_disk  = SD_CARD_TITLE_EN;
  media_select_menu.usb_disk = USB_DRIVE_TITLE_EN;

  machine_menu.key_0     = KEYBOARD_KEY0_EN;
  machine_menu.key_1     = KEYBOARD_KEY1_EN;
  machine_menu.key_2     = KEYBOARD_KEY2_EN;
  machine_menu.key_3     = KEYBOARD_KEY3_EN;
  machine_menu.key_4     = KEYBOARD_KEY4_EN;
  machine_menu.key_5     = KEYBOARD_KEY5_EN;
  machine_menu.key_6     = KEYBOARD_KEY6_EN;
  machine_menu.key_7     = KEYBOARD_KEY7_EN;
  machine_menu.key_8     = KEYBOARD_KEY8_EN;
  machine_menu.key_9     = KEYBOARD_KEY9_EN;
  machine_menu.key_point = KEYBOARD_KEY_POINT_EN;
  machine_menu.negative  = KEYBOARD_KEY_NEGATIVE_EN;
  // wifi-list
  #if ENABLED(MKS_WIFI_MODULE)
    list_menu.title        = TEXT_WIFI_MENU_TITLE_EN;
    list_menu.file_pages   = FILE_PAGES_EN;

    // tips
    tips_menu.joining      = TEXT_WIFI_JOINING_EN;
    tips_menu.failedJoin   = TEXT_WIFI_FAILED_JOIN_EN;
    tips_menu.wifiConected = TEXT_WIFI_WIFI_CONECTED_EN;
  #endif
  machine_setting_disp();

  operation_menu.babystep = TEXT_BABY_STEP_EN;

  switch (gCfgItems.language) {
    case LANG_SIMPLE_CHINESE:
      common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_CN;
      common_menu.text_back             = BACK_TEXT_CN;
      common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_CN;
      common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_CN;
      common_menu.print_special_title   = PRINTING_GBK;
      common_menu.pause_special_title   = PRINTING_PAUSE_GBK;
      common_menu.operate_special_title = PRINTING_OPERATION_GBK;
      //
      main_menu.title        = TITLE_READYPRINT_CN;
      main_menu.preheat      = PREHEAT_TEXT_CN;
      main_menu.move         = MOVE_TEXT_CN;
      main_menu.home         = HOME_TEXT_CN;
      main_menu.print        = PRINT_TEXT_CN;
      main_menu.extrude      = EXTRUDE_TEXT_CN;
      main_menu.leveling     = LEVELING_TEXT_CN;
      main_menu.autoleveling = AUTO_LEVELING_TEXT_CN;
      main_menu.fan          = FAN_TEXT_CN;
      main_menu.set          = SET_TEXT_CN;
      main_menu.more         = MORE_TEXT_CN;
      main_menu.tool         = TOOL_TEXT_CN;
      // TOOL
      tool_menu.title        = TOOL_TEXT_CN;
      tool_menu.preheat      = TOOL_PREHEAT_CN;
      tool_menu.extrude      = TOOL_EXTRUDE_CN;
      tool_menu.move         = TOOL_MOVE_CN;
      tool_menu.home         = TOOL_HOME_CN;
      tool_menu.leveling     = TOOL_LEVELING_CN;
      tool_menu.autoleveling = TOOL_AUTO_LEVELING_CN;
      tool_menu.filament     = TOOL_FILAMENT_CN;
      tool_menu.more         = TOOL_MORE_CN;
      //
      preheat_menu.adjust_title = TITLE_ADJUST_CN;
      preheat_menu.title        = TITLE_PREHEAT_CN;
      preheat_menu.add          = ADD_TEXT_CN;
      preheat_menu.dec          = DEC_TEXT_CN;
      preheat_menu.ext1         = EXTRUDER_1_TEXT_CN;
      preheat_menu.ext2         = EXTRUDER_2_TEXT_CN;
      preheat_menu.hotbed       = HEATBED_TEXT_CN;
      preheat_menu.off          = CLOSE_TEXT_CN;

      preheat_menu.value_state = TEXT_VALUE_CN;
      preheat_menu.step_1c     = TEXT_1C_CN;
      preheat_menu.step_5c     = TEXT_5C_CN;
      preheat_menu.step_10c    = TEXT_10C_CN;
      //
      move_menu.title = MOVE_TEXT_CN;
      //
      home_menu.title    = TITLE_HOME_CN;
      home_menu.stopmove = HOME_STOPMOVE_CN;
      //
      file_menu.title             = TITLE_CHOOSEFILE_CN;
      file_menu.page_up           = PAGE_UP_TEXT_CN;
      file_menu.page_down         = PAGE_DOWN_TEXT_CN;
      file_menu.file_loading      = FILE_LOADING_CN;
      file_menu.no_file           = NO_FILE_CN;
      file_menu.no_file_and_check = NO_FILE_CN;
      //
      extrude_menu.title       = TITLE_EXTRUDE_CN;
      extrude_menu.in          = EXTRUDER_IN_TEXT_CN;
      extrude_menu.out         = EXTRUDER_OUT_TEXT_CN;
      extrude_menu.ext1        = EXTRUDER_1_TEXT_CN;
      extrude_menu.ext2        = EXTRUDER_2_TEXT_CN;
      extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_CN;
      extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_CN;
      extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_CN;
      extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_CN;
      extrude_menu.temp_value  = EXTRUDE_TEXT_VALUE_T_CN;
      //
      leveling_menu.title     = TITLE_LEVELING_CN;
      leveling_menu.position1 = LEVELING_POINT1_TEXT_CN;
      leveling_menu.position2 = LEVELING_POINT2_TEXT_CN;
      leveling_menu.position3 = LEVELING_POINT3_TEXT_CN;
      leveling_menu.position4 = LEVELING_POINT4_TEXT_CN;
      leveling_menu.position5 = LEVELING_POINT5_TEXT_CN;
      //
      set_menu.title        = TITLE_SET_CN;
      set_menu.filesys      = FILESYS_TEXT_CN;
      set_menu.wifi         = WIFI_TEXT_CN;
      set_menu.about        = ABOUT_TEXT_CN;
      set_menu.fan          = FAN_TEXT_CN;
      set_menu.filament     = FILAMENT_TEXT_CN;
      set_menu.breakpoint   = BREAK_POINT_TEXT_CN;
      set_menu.motoroff     = MOTOR_OFF_TEXT_CN;
      set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_CN;
      set_menu.language     = LANGUAGE_TEXT_CN;
      set_menu.shutdown     = SHUTDOWN_TEXT_CN;
      set_menu.machine_para = MACHINE_PARA_CN;
      set_menu.eepromSet    = EEPROM_SETTINGS_CN;
      //
      filesys_menu.title   = TITLE_FILESYS_CN;
      filesys_menu.sd_sys  = SD_CARD_TEXT_CN;
      filesys_menu.usb_sys = U_DISK_TEXT_CN;
      //
      more_menu.title       = TITLE_MORE_CN;
      more_menu.gcode       = MORE_GCODE_CN;
      more_menu.entergcode  = MORE_ENTER_GCODE_CN;
      #if HAS_USER_ITEM(1)
        more_menu.custom1 = MORE_CUSTOM1_TEXT_CN;
      #endif
      #if HAS_USER_ITEM(2)
        more_menu.custom2 = MORE_CUSTOM2_TEXT_CN;
      #endif
      #if HAS_USER_ITEM(3)
        more_menu.custom3 = MORE_CUSTOM3_TEXT_CN;
      #endif
      #if HAS_USER_ITEM(4)
        more_menu.custom4 = MORE_CUSTOM4_TEXT_CN;
      #endif
      #if HAS_USER_ITEM(5)
        more_menu.custom5 = MORE_CUSTOM5_TEXT_CN;
      #endif
      #if HAS_USER_ITEM(6)
        more_menu.custom6 = MORE_CUSTOM6_TEXT_CN;
      #endif
      // WIFI
      wifi_menu.title = WIFI_TEXT;
      wifi_menu.cloud     = CLOUD_TEXT_CN;
      wifi_menu.reconnect = WIFI_RECONNECT_TEXT_CN;
      // CLOUD
      cloud_menu.title        = TITLE_CLOUD_TEXT_CN;
      cloud_menu.bind         = CLOUD_BINDED_CN;
      cloud_menu.binded       = CLOUD_BINDED_CN;
      cloud_menu.unbind       = CLOUD_UNBIND_CN;
      cloud_menu.unbinding    = CLOUD_UNBINDED_CN;
      cloud_menu.disconnected = CLOUD_DISCONNECTED_CN;
      cloud_menu.unbinded     = CLOUD_UNBINDED_CN;
      cloud_menu.disable      = CLOUD_DISABLE_CN;
      //
      about_menu.title   = ABOUT_TEXT_CN;
      about_menu.type    = ABOUT_TYPE_TEXT_CN;
      about_menu.version = ABOUT_VERSION_TEXT_CN;
      about_menu.wifi    = ABOUT_WIFI_TEXT_CN;

      //
      fan_menu.title = FAN_TEXT_CN;
      fan_menu.add   = FAN_ADD_TEXT_CN;
      fan_menu.dec   = FAN_DEC_TEXT_CN;
      fan_menu.state = FAN_TIPS1_TEXT_CN;
      //
      filament_menu.title                               = TITLE_FILAMENT_CN;
      filament_menu.in                                  = FILAMENT_IN_TEXT_CN;
      filament_menu.out                                 = FILAMENT_OUT_TEXT_CN;
      filament_menu.ext1                                = FILAMENT_EXT0_TEXT_CN;
      filament_menu.ext2                                = FILAMENT_EXT1_TEXT_CN;
      filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_CN;
      filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_CN;
      filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_CN;
      filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_CN;
      filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_CN;
      filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_CN;
      filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_CN;
      filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_CN;
      filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_CN;

      //
      language_menu.title = TITLE_LANGUAGE_CN;
      language_menu.next  = PAGE_DOWN_TEXT_CN;
      language_menu.up    = PAGE_UP_TEXT_CN;

      //
      printing_menu.title  = TITLE_PRINTING_CN;
      printing_menu.option = PRINTING_OPERATION_CN;
      printing_menu.stop   = PRINTING_STOP_CN;
      printing_menu.pause  = PRINTING_PAUSE_CN;
      printing_menu.resume = PRINTING_RESUME_CN;

      //
      operation_menu.title      = TITLE_OPERATION_CN;
      operation_menu.pause      = PRINTING_PAUSE_CN;
      operation_menu.stop       = PRINTING_STOP_CN;
      operation_menu.temp       = PRINTING_TEMP_CN;
      operation_menu.fan        = FAN_TEXT_CN;
      operation_menu.filament   = FILAMENT_TEXT_CN;
      operation_menu.extr       = PRINTING_EXTRUDER_CN;
      operation_menu.speed      = PRINTING_CHANGESPEED_CN;
      operation_menu.more       = PRINTING_MORE_CN;
      operation_menu.move       = PRINTING_MOVE_CN;
      operation_menu.auto_off   = AUTO_SHUTDOWN_CN;
      operation_menu.manual_off = MANUAL_SHUTDOWN_CN;
      //
      pause_menu.title    = TITLE_PAUSE_CN;
      pause_menu.resume   = PRINTING_RESUME_CN;
      pause_menu.stop     = PRINTING_STOP_CN;
      pause_menu.extrude  = PRINTING_EXTRUDER_CN;
      pause_menu.move     = PRINTING_MOVE_CN;
      pause_menu.filament = FILAMENT_TEXT_CN;
      pause_menu.more     = PRINTING_MORE_CN;

      //
      speed_menu.title         = PRINTING_CHANGESPEED_CN;
      speed_menu.add           = ADD_TEXT_CN;
      speed_menu.dec           = DEC_TEXT_CN;
      speed_menu.move          = MOVE_SPEED_CN;
      speed_menu.extrude       = EXTRUDER_SPEED_CN;
      speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_CN;
      speed_menu.move_speed    = MOVE_SPEED_STATE_CN;
      //
      printing_more_menu.title      = TITLE_MORE_CN;
      printing_more_menu.fan        = FAN_TEXT_CN;
      printing_more_menu.auto_close = AUTO_SHUTDOWN_CN;
      printing_more_menu.manual     = MANUAL_SHUTDOWN_CN;
      printing_more_menu.speed      = PRINTING_CHANGESPEED_CN;
      printing_more_menu.temp       = PRINTING_TEMP_CN;

      print_file_dialog_menu.confirm               = DIALOG_CONFIRM_CN;
      print_file_dialog_menu.cancel                = DIALOG_CANCLE_CN;
      print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_CN;
      print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_CN;
      print_file_dialog_menu.retry                 = DIALOG_RETRY_CN;
      print_file_dialog_menu.stop                  = DIALOG_STOP_CN;
      print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_CN;
      print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_CN;

      print_file_dialog_menu.close_machine_error = DIALOG_ERROR_TIPS2_CN;
      print_file_dialog_menu.filament_no_press   = DIALOG_FILAMENT_NO_PRESS_CN;

      print_file_dialog_menu.print_finish     = DIALOG_PRINT_FINISH_CN;
      print_file_dialog_menu.print_time       = DIALOG_PRINT_TIME_CN;
      print_file_dialog_menu.reprint          = DIALOG_REPRINT_CN;
      print_file_dialog_menu.wifi_enable_tips = DIALOG_WIFI_ENABLE_TIPS_CN;

      pause_msg_menu.pausing       = MESSAGE_PAUSING_CN;
      pause_msg_menu.changing      = MESSAGE_CHANGING_CN;
      pause_msg_menu.unload        = MESSAGE_UNLOAD_CN;
      pause_msg_menu.waiting       = MESSAGE_WAITING_CN;
      pause_msg_menu.insert        = MESSAGE_INSERT_CN;
      pause_msg_menu.load          = MESSAGE_LOAD_CN;
      pause_msg_menu.purge         = MESSAGE_PURGE_CN;
      pause_msg_menu.resume        = MESSAGE_RESUME_CN;
      pause_msg_menu.heat          = MESSAGE_HEAT_CN;
      pause_msg_menu.heating       = MESSAGE_HEATING_CN;
      pause_msg_menu.option        = MESSAGE_OPTION_CN;
      pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_CN;
      pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_CN;
      eeprom_menu.title            = EEPROM_SETTINGS_TITLE_CN;
      eeprom_menu.store            = EEPROM_SETTINGS_STORE_CN;
      eeprom_menu.read             = EEPROM_SETTINGS_READ_CN;
      eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_CN;
      eeprom_menu.storeTips        = EEPROM_STORE_TIPS_CN;
      eeprom_menu.readTips         = EEPROM_READ_TIPS_CN;
      eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_CN;
      break;

      #if 1
        #if 1

          case LANG_COMPLEX_CHINESE:
            common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_T_CN;
            common_menu.text_back             = BACK_TEXT_T_CN;
            common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_T_CN;
            common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_T_CN;
            common_menu.print_special_title   = PRINTING_GBK;
            common_menu.pause_special_title   = PRINTING_PAUSE_GBK;
            common_menu.operate_special_title = PRINTING_OPERATION_GBK;
            //
            main_menu.title        = TITLE_READYPRINT_T_CN;
            main_menu.preheat      = PREHEAT_TEXT_T_CN;
            main_menu.move         = MOVE_TEXT_T_CN;
            main_menu.home         = HOME_TEXT_T_CN;
            main_menu.print        = PRINT_TEXT_T_CN;
            main_menu.extrude      = EXTRUDE_TEXT_T_CN;
            main_menu.leveling     = LEVELING_TEXT_T_CN;
            main_menu.autoleveling = AUTO_LEVELING_TEXT_T_CN;
            main_menu.fan          = FAN_TEXT_T_CN;
            main_menu.set          = SET_TEXT_T_CN;
            main_menu.more         = MORE_TEXT_T_CN;
            main_menu.tool         = TOOL_TEXT_T_CN;
            // TOOL
            tool_menu.title        = TOOL_TEXT_T_CN;
            tool_menu.preheat      = TOOL_PREHEAT_T_CN;
            tool_menu.extrude      = TOOL_EXTRUDE_T_CN;
            tool_menu.move         = TOOL_MOVE_T_CN;
            tool_menu.home         = TOOL_HOME_T_CN;
            tool_menu.leveling     = TOOL_LEVELING_T_CN;
            tool_menu.autoleveling = TOOL_AUTO_LEVELING_T_CN;
            tool_menu.filament     = TOOL_FILAMENT_T_CN;
            tool_menu.more         = TOOL_MORE_T_CN;
            //
            preheat_menu.adjust_title = TITLE_ADJUST_T_CN;
            preheat_menu.title        = TITLE_PREHEAT_T_CN;
            preheat_menu.add          = ADD_TEXT_T_CN;
            preheat_menu.dec          = DEC_TEXT_T_CN;
            preheat_menu.ext1         = EXTRUDER_1_TEXT_T_CN;
            preheat_menu.ext2         = EXTRUDER_2_TEXT_T_CN;
            preheat_menu.hotbed       = HEATBED_TEXT_T_CN;
            preheat_menu.off          = CLOSE_TEXT_T_CN;
            preheat_menu.value_state  = TEXT_VALUE_T_CN;
            preheat_menu.step_1c      = TEXT_1C_T_CN;
            preheat_menu.step_5c      = TEXT_5C_T_CN;
            preheat_menu.step_10c     = TEXT_10C_T_CN;
            //
            move_menu.title   = MOVE_TEXT_T_CN;
            //
            home_menu.title    = TITLE_HOME_T_CN;
            home_menu.stopmove = HOME_STOPMOVE_T_CN;
            //
            file_menu.title             = TITLE_CHOOSEFILE_T_CN;
            file_menu.page_up           = PAGE_UP_TEXT_T_CN;
            file_menu.page_down         = PAGE_DOWN_TEXT_T_CN;
            file_menu.file_loading      = FILE_LOADING_T_CN;
            file_menu.no_file           = NO_FILE_T_CN;
            file_menu.no_file_and_check = NO_FILE_T_CN;
            //
            extrude_menu.title       = TITLE_EXTRUDE_T_CN;
            extrude_menu.in          = EXTRUDER_IN_TEXT_T_CN;
            extrude_menu.out         = EXTRUDER_OUT_TEXT_T_CN;
            extrude_menu.ext1        = EXTRUDER_1_TEXT_T_CN;
            extrude_menu.ext2        = EXTRUDER_2_TEXT_T_CN;
            extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_T_CN;
            extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_T_CN;
            extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_T_CN;
            extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_T_CN;
            extrude_menu.temp_value  = EXTRUDE_TEXT_VALUE_T_T_CN;
            //
            leveling_menu.title     = TITLE_LEVELING_CN;
            leveling_menu.position1 = LEVELING_POINT1_TEXT_T_CN;
            leveling_menu.position2 = LEVELING_POINT2_TEXT_T_CN;
            leveling_menu.position3 = LEVELING_POINT3_TEXT_T_CN;
            leveling_menu.position4 = LEVELING_POINT4_TEXT_T_CN;
            leveling_menu.position5 = LEVELING_POINT5_TEXT_T_CN;
            //
            set_menu.title        = TITLE_SET_T_CN;
            set_menu.filesys      = FILESYS_TEXT_T_CN;
            set_menu.wifi         = WIFI_TEXT_T_CN;
            set_menu.about        = ABOUT_TEXT_T_CN;
            set_menu.fan          = FAN_TEXT_T_CN;
            set_menu.filament     = FILAMENT_TEXT_T_CN;
            set_menu.breakpoint   = BREAK_POINT_TEXT_T_CN;
            set_menu.motoroff     = MOTOR_OFF_TEXT_T_CN;
            set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_T_CN;
            set_menu.language     = LANGUAGE_TEXT_T_CN;
            set_menu.shutdown     = SHUTDOWN_TEXT_T_CN;
            set_menu.machine_para = MACHINE_PARA_T_CN;
            set_menu.eepromSet    = EEPROM_SETTINGS_T_CN;
            filesys_menu.title    = TITLE_FILESYS_T_CN;
            filesys_menu.sd_sys   = SD_CARD_TEXT_T_CN;
            filesys_menu.usb_sys  = U_DISK_TEXT_T_CN;
            //
            more_menu.title       = TITLE_MORE_T_CN;
            more_menu.gcode       = MORE_GCODE_T_CN;
            more_menu.entergcode  = MORE_ENTER_GCODE_T_CN;
            #if HAS_USER_ITEM(1)
              more_menu.custom1 = MORE_CUSTOM1_TEXT_CN;
            #endif
            #if HAS_USER_ITEM(2)
              more_menu.custom2 = MORE_CUSTOM2_TEXT_CN;
            #endif
            #if HAS_USER_ITEM(3)
              more_menu.custom3 = MORE_CUSTOM3_TEXT_CN;
            #endif
            #if HAS_USER_ITEM(4)
              more_menu.custom4 = MORE_CUSTOM4_TEXT_CN;
            #endif
            #if HAS_USER_ITEM(5)
              more_menu.custom5 = MORE_CUSTOM5_TEXT_CN;
            #endif
            #if HAS_USER_ITEM(6)
              more_menu.custom6 = MORE_CUSTOM6_TEXT_CN;
            #endif
            // WIFI
            wifi_menu.title     = WIFI_TEXT;
            wifi_menu.cloud     = CLOUD_TEXT_T_CN;
            wifi_menu.reconnect = WIFI_RECONNECT_TEXT_T_CN;
            // CLOUD
            cloud_menu.title        = TITLE_CLOUD_TEXT_T_CN;
            cloud_menu.bind         = CLOUD_BINDED_T_CN;
            cloud_menu.binded       = CLOUD_BINDED_T_CN;
            cloud_menu.unbind       = CLOUD_UNBIND_T_CN;
            cloud_menu.unbinding    = CLOUD_UNBINDED_T_CN;
            cloud_menu.disconnected = CLOUD_DISCONNECTED_T_CN;
            cloud_menu.unbinded     = CLOUD_UNBINDED_T_CN;
            cloud_menu.disable      = CLOUD_DISABLE_T_CN;
            //
            about_menu.title   = ABOUT_TEXT_T_CN;
            about_menu.type    = ABOUT_TYPE_TEXT_T_CN;
            about_menu.version = ABOUT_VERSION_TEXT_T_CN;
            about_menu.wifi    = ABOUT_WIFI_TEXT_T_CN;

            //
            fan_menu.title = FAN_TEXT_T_CN;
            fan_menu.add   = FAN_ADD_TEXT_T_CN;
            fan_menu.dec   = FAN_DEC_TEXT_T_CN;
            fan_menu.state = FAN_TIPS1_TEXT_T_CN;
            //
            filament_menu.title                               = TITLE_FILAMENT_T_CN;
            filament_menu.in                                  = FILAMENT_IN_TEXT_T_CN;
            filament_menu.out                                 = FILAMENT_OUT_TEXT_T_CN;
            filament_menu.ext1                                = FILAMENT_EXT0_TEXT_T_CN;
            filament_menu.ext2                                = FILAMENT_EXT1_TEXT_T_CN;
            filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_T_CN;
            filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_T_CN;
            filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_T_CN;
            filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_T_CN;
            filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_T_CN;
            filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_T_CN;
            filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_T_CN;
            filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_T_CN;
            filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_T_CN;

            //
            language_menu.title = TITLE_LANGUAGE_T_CN;
            language_menu.next  = PAGE_DOWN_TEXT_T_CN;
            language_menu.up    = PAGE_UP_TEXT_T_CN;

            //
            printing_menu.title  = TITLE_PRINTING_T_CN;
            printing_menu.option = PRINTING_OPERATION_T_CN;
            printing_menu.stop   = PRINTING_STOP_T_CN;
            printing_menu.pause  = PRINTING_PAUSE_T_CN;
            printing_menu.resume = PRINTING_RESUME_T_CN;

            //
            operation_menu.title      = TITLE_OPERATION_T_CN;
            operation_menu.pause      = PRINTING_PAUSE_T_CN;
            operation_menu.stop       = PRINTING_STOP_T_CN;
            operation_menu.temp       = PRINTING_TEMP_T_CN;
            operation_menu.fan        = FAN_TEXT_T_CN;
            operation_menu.extr       = PRINTING_EXTRUDER_T_CN;
            operation_menu.speed      = PRINTING_CHANGESPEED_T_CN;
            operation_menu.filament   = FILAMENT_TEXT_T_CN;
            operation_menu.more       = PRINTING_MORE_T_CN;
            operation_menu.move       = PRINTING_MOVE_T_CN;
            operation_menu.auto_off   = AUTO_SHUTDOWN_T_CN;
            operation_menu.manual_off = MANUAL_SHUTDOWN_T_CN;
            //
            pause_menu.title    = TITLE_PAUSE_T_CN;
            pause_menu.resume   = PRINTING_RESUME_T_CN;
            pause_menu.stop     = PRINTING_STOP_T_CN;
            pause_menu.extrude  = PRINTING_EXTRUDER_T_CN;
            pause_menu.move     = PRINTING_MOVE_T_CN;
            pause_menu.filament = FILAMENT_TEXT_T_CN;
            pause_menu.more     = PRINTING_MORE_T_CN;

            //
            speed_menu.title         = PRINTING_CHANGESPEED_T_CN;
            speed_menu.add           = ADD_TEXT_T_CN;
            speed_menu.dec           = DEC_TEXT_T_CN;
            speed_menu.move          = MOVE_SPEED_T_CN;
            speed_menu.extrude       = EXTRUDER_SPEED_T_CN;
            speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_T_CN;
            speed_menu.move_speed    = MOVE_SPEED_STATE_T_CN;
            //
            printing_more_menu.title      = TITLE_MORE_T_CN;
            printing_more_menu.fan        = FAN_TEXT_T_CN;
            printing_more_menu.auto_close = AUTO_SHUTDOWN_T_CN;
            printing_more_menu.manual     = MANUAL_SHUTDOWN_T_CN;
            printing_more_menu.speed      = PRINTING_CHANGESPEED_T_CN;
            printing_more_menu.temp       = PRINTING_TEMP_T_CN;

            print_file_dialog_menu.confirm               = DIALOG_CONFIRM_T_CN;
            print_file_dialog_menu.cancel                = DIALOG_CANCLE_T_CN;
            print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_T_CN;
            print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_T_CN;
            print_file_dialog_menu.retry                 = DIALOG_RETRY_T_CN;
            print_file_dialog_menu.stop                  = DIALOG_STOP_T_CN;
            print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_T_CN;
            print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_T_CN;

            print_file_dialog_menu.close_machine_error = DIALOG_ERROR_TIPS2_T_CN;
            print_file_dialog_menu.filament_no_press   = DIALOG_FILAMENT_NO_PRESS_T_CN;
            print_file_dialog_menu.print_finish        = DIALOG_PRINT_FINISH_T_CN;
            print_file_dialog_menu.print_time          = DIALOG_PRINT_TIME_T_CN;
            print_file_dialog_menu.reprint             = DIALOG_REPRINT_T_CN;
            print_file_dialog_menu.wifi_enable_tips    = DIALOG_WIFI_ENABLE_TIPS_T_CN;

            pause_msg_menu.pausing       = MESSAGE_PAUSING_T_CN;
            pause_msg_menu.changing      = MESSAGE_CHANGING_T_CN;
            pause_msg_menu.unload        = MESSAGE_UNLOAD_T_CN;
            pause_msg_menu.waiting       = MESSAGE_WAITING_T_CN;
            pause_msg_menu.insert        = MESSAGE_INSERT_T_CN;
            pause_msg_menu.load          = MESSAGE_LOAD_T_CN;
            pause_msg_menu.purge         = MESSAGE_PURGE_T_CN;
            pause_msg_menu.resume        = MESSAGE_RESUME_T_CN;
            pause_msg_menu.heat          = MESSAGE_HEAT_T_CN;
            pause_msg_menu.heating       = MESSAGE_HEATING_T_CN;
            pause_msg_menu.option        = MESSAGE_OPTION_T_CN;
            pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_T_CN;
            pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_T_CN;
            eeprom_menu.title            = EEPROM_SETTINGS_TITLE_T_CN;
            eeprom_menu.store            = EEPROM_SETTINGS_STORE_T_CN;
            eeprom_menu.read             = EEPROM_SETTINGS_READ_T_CN;
            eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_T_CN;
            eeprom_menu.storeTips        = EEPROM_STORE_TIPS_T_CN;
            eeprom_menu.readTips         = EEPROM_READ_TIPS_T_CN;
            eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_T_CN;
            break;
          case LANG_ENGLISH:
            common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_EN;
            common_menu.text_back             = BACK_TEXT_EN;
            common_menu.text_save             = SAVE_TEXT_EN;
            common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_EN;
            common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_EN;
            common_menu.print_special_title   = PRINTING_OTHER_LANGUGE;
            common_menu.pause_special_title   = PRINTING_PAUSE_OTHER_LANGUGE;
            common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;
            //
            main_menu.title        = TITLE_READYPRINT_EN;
            main_menu.preheat      = PREHEAT_TEXT_EN;
            main_menu.move         = MOVE_TEXT_EN;
            main_menu.home         = HOME_TEXT_EN;
            main_menu.print        = PRINT_TEXT_EN;
            main_menu.extrude      = EXTRUDE_TEXT_EN;
            main_menu.leveling     = LEVELING_TEXT_EN;
            main_menu.autoleveling = AUTO_LEVELING_TEXT_EN;
            main_menu.fan          = FAN_TEXT_EN;
            main_menu.set          = SET_TEXT_EN;
            main_menu.more         = MORE_TEXT_EN;
            main_menu.tool         = TOOL_TEXT_EN;
            // TOOL
            tool_menu.title        = TOOL_TEXT_EN;
            tool_menu.preheat      = TOOL_PREHEAT_EN;
            tool_menu.extrude      = TOOL_EXTRUDE_EN;
            tool_menu.move         = TOOL_MOVE_EN;
            tool_menu.home         = TOOL_HOME_EN;
            tool_menu.leveling     = TOOL_LEVELING_EN;
            tool_menu.autoleveling = TOOL_AUTO_LEVELING_EN;
            tool_menu.filament     = TOOL_FILAMENT_EN;
            tool_menu.more         = TOOL_MORE_EN;
            //
            preheat_menu.adjust_title = TITLE_ADJUST_EN;
            preheat_menu.title        = TITLE_PREHEAT_EN;
            preheat_menu.add          = ADD_TEXT_EN;
            preheat_menu.dec          = DEC_TEXT_EN;
            preheat_menu.ext1         = EXTRUDER_1_TEXT_EN;
            preheat_menu.ext2         = EXTRUDER_2_TEXT_EN;
            preheat_menu.hotbed       = HEATBED_TEXT_EN;
            preheat_menu.off          = CLOSE_TEXT_EN;
            //
            move_menu.title    = TITLE_MOVE_EN;
            //
            home_menu.title    = TITLE_HOME_EN;
            home_menu.stopmove = HOME_STOPMOVE_EN;
            //
            file_menu.title             = TITLE_CHOOSEFILE_EN;
            file_menu.page_up           = PAGE_UP_TEXT_EN;
            file_menu.page_down         = PAGE_DOWN_TEXT_EN;
            file_menu.file_loading      = FILE_LOADING_EN;
            file_menu.no_file           = NO_FILE_EN;
            file_menu.no_file_and_check = NO_FILE_EN;
            //
            extrude_menu.title       = TITLE_EXTRUDE_EN;
            extrude_menu.in          = EXTRUDER_IN_TEXT_EN;
            extrude_menu.out         = EXTRUDER_OUT_TEXT_EN;
            extrude_menu.ext1        = EXTRUDER_1_TEXT_EN;
            extrude_menu.ext2        = EXTRUDER_2_TEXT_EN;
            extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_EN;
            extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_EN;
            extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_EN;
            extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_EN;
            //
            leveling_menu.title     = TITLE_LEVELING_EN;
            leveling_menu.position1 = LEVELING_POINT1_TEXT_EN;
            leveling_menu.position2 = LEVELING_POINT2_TEXT_EN;
            leveling_menu.position3 = LEVELING_POINT3_TEXT_EN;
            leveling_menu.position4 = LEVELING_POINT4_TEXT_EN;
            leveling_menu.position5 = LEVELING_POINT5_TEXT_EN;
            //
            set_menu.title        = TITLE_SET_EN;
            set_menu.filesys      = FILESYS_TEXT_EN;
            set_menu.wifi         = WIFI_TEXT_EN;
            set_menu.about        = ABOUT_TEXT_EN;
            set_menu.fan          = FAN_TEXT_EN;
            set_menu.filament     = FILAMENT_TEXT_EN;
            set_menu.breakpoint   = BREAK_POINT_TEXT_EN;
            set_menu.motoroff     = MOTOR_OFF_TEXT_EN;
            set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_EN;
            set_menu.language     = LANGUAGE_TEXT_EN;
            set_menu.shutdown     = SHUTDOWN_TEXT_EN;
            set_menu.machine_para = MACHINE_PARA_EN;
            set_menu.eepromSet    = EEPROM_SETTINGS_EN;
            //
            more_menu.title       = TITLE_MORE_EN;
            more_menu.gcode       = MORE_GCODE_EN;
            more_menu.entergcode  = MORE_ENTER_GCODE_EN;
            #if HAS_USER_ITEM(1)
              more_menu.custom1 = MORE_CUSTOM1_TEXT_EN;
            #endif
            #if HAS_USER_ITEM(2)
              more_menu.custom2 = MORE_CUSTOM2_TEXT_EN;
            #endif
            #if HAS_USER_ITEM(3)
              more_menu.custom3 = MORE_CUSTOM3_TEXT_EN;
            #endif
            #if HAS_USER_ITEM(4)
              more_menu.custom4 = MORE_CUSTOM4_TEXT_EN;
            #endif
            #if HAS_USER_ITEM(5)
              more_menu.custom5 = MORE_CUSTOM5_TEXT_EN;
            #endif
            #if HAS_USER_ITEM(6)
              more_menu.custom6 = MORE_CUSTOM6_TEXT_EN;
            #endif

            //
            filesys_menu.title   = TITLE_FILESYS_EN;
            filesys_menu.sd_sys  = SD_CARD_TEXT_EN;
            filesys_menu.usb_sys = U_DISK_TEXT_EN;
            // WIFI
            wifi_menu.title     = WIFI_TEXT;
            wifi_menu.cloud     = CLOUD_TEXT_EN;
            wifi_menu.reconnect = WIFI_RECONNECT_TEXT_EN;

            cloud_menu.title        = TITLE_CLOUD_TEXT_EN;
            cloud_menu.bind         = CLOUD_BINDED_EN;
            cloud_menu.binded       = CLOUD_BINDED_EN;
            cloud_menu.unbind       = CLOUD_UNBIND_EN;
            cloud_menu.unbinding    = CLOUD_UNBINDED_EN;
            cloud_menu.disconnected = CLOUD_DISCONNECTED_EN;
            cloud_menu.unbinded     = CLOUD_UNBINDED_EN;
            cloud_menu.disable      = CLOUD_DISABLE_EN;
            //
            about_menu.title   = TITLE_ABOUT_EN;
            about_menu.type    = ABOUT_TYPE_TEXT_EN;
            about_menu.version = ABOUT_VERSION_TEXT_EN;
            about_menu.wifi    = ABOUT_WIFI_TEXT_EN;
            //
            fan_menu.title = TITLE_FAN_EN;
            fan_menu.add   = FAN_ADD_TEXT_EN;
            fan_menu.dec   = FAN_DEC_TEXT_EN;
            fan_menu.state = FAN_TIPS1_TEXT_EN;
            //
            filament_menu.title                               = TITLE_FILAMENT_EN;
            filament_menu.in                                  = FILAMENT_IN_TEXT_EN;
            filament_menu.out                                 = FILAMENT_OUT_TEXT_EN;
            filament_menu.ext1                                = FILAMENT_EXT0_TEXT_EN;
            filament_menu.ext2                                = FILAMENT_EXT1_TEXT_EN;
            filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_EN;
            filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_EN;
            filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_EN;
            filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_EN;
            filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_EN;
            filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_EN;
            filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_EN;
            filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_EN;
            filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_EN;

            //
            language_menu.title = TITLE_LANGUAGE_EN;
            language_menu.next  = PAGE_DOWN_TEXT_EN;
            language_menu.up    = PAGE_UP_TEXT_EN;
            //
            printing_menu.title  = TITLE_PRINTING_EN;
            printing_menu.option = PRINTING_OPERATION_EN;
            printing_menu.stop   = PRINTING_STOP_EN;
            printing_menu.pause  = PRINTING_PAUSE_EN;
            printing_menu.resume = PRINTING_RESUME_EN;

            //
            operation_menu.title      = TITLE_OPERATION_EN;
            operation_menu.pause      = PRINTING_PAUSE_EN;
            operation_menu.stop       = PRINTING_STOP_EN;
            operation_menu.temp       = PRINTING_TEMP_EN;
            operation_menu.fan        = FAN_TEXT_EN;
            operation_menu.extr       = PRINTING_EXTRUDER_EN;
            operation_menu.speed      = PRINTING_CHANGESPEED_EN;
            operation_menu.filament   = FILAMENT_TEXT_EN;
            operation_menu.more       = PRINTING_MORE_EN;
            operation_menu.move       = PRINTING_MOVE_EN;
            operation_menu.auto_off   = AUTO_SHUTDOWN_EN;
            operation_menu.manual_off = MANUAL_SHUTDOWN_EN;
            //
            pause_menu.title    = TITLE_PAUSE_EN;
            pause_menu.resume   = PRINTING_RESUME_EN;
            pause_menu.stop     = PRINTING_STOP_EN;
            pause_menu.extrude  = PRINTING_EXTRUDER_EN;
            pause_menu.move     = PRINTING_MOVE_EN;
            pause_menu.filament = FILAMENT_TEXT_EN;
            pause_menu.more     = PRINTING_MORE_EN;

            //
            speed_menu.title         = TITLE_CHANGESPEED_EN;
            speed_menu.add           = ADD_TEXT_EN;
            speed_menu.dec           = DEC_TEXT_EN;
            speed_menu.move          = MOVE_SPEED_EN;
            speed_menu.extrude       = EXTRUDER_SPEED_EN;
            speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_EN;
            speed_menu.move_speed    = MOVE_SPEED_STATE_EN;
            //
            printing_more_menu.title      = TITLE_MORE_EN;
            printing_more_menu.fan        = FAN_TEXT_EN;
            printing_more_menu.auto_close = AUTO_SHUTDOWN_EN;
            printing_more_menu.manual     = MANUAL_SHUTDOWN_EN;
            printing_more_menu.speed      = PRINTING_CHANGESPEED_EN;
            printing_more_menu.temp       = PRINTING_TEMP_EN;

            print_file_dialog_menu.confirm               = DIALOG_CONFIRM_EN;
            print_file_dialog_menu.cancel                = DIALOG_CANCLE_EN;
            print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_EN;
            print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_EN;
            print_file_dialog_menu.retry                 = DIALOG_RETRY_EN;
            print_file_dialog_menu.stop                  = DIALOG_STOP_EN;
            print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_EN;
            print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_EN;
            print_file_dialog_menu.close_machine_error   = DIALOG_ERROR_TIPS2_EN;
            print_file_dialog_menu.filament_no_press     = DIALOG_FILAMENT_NO_PRESS_EN;
            print_file_dialog_menu.print_finish          = DIALOG_PRINT_FINISH_EN;
            print_file_dialog_menu.print_time            = DIALOG_PRINT_TIME_EN;
            print_file_dialog_menu.reprint               = DIALOG_REPRINT_EN;
            print_file_dialog_menu.wifi_enable_tips      = DIALOG_WIFI_ENABLE_TIPS_EN;

            pause_msg_menu.pausing       = MESSAGE_PAUSING_EN;
            pause_msg_menu.changing      = MESSAGE_CHANGING_EN;
            pause_msg_menu.unload        = MESSAGE_UNLOAD_EN;
            pause_msg_menu.waiting       = MESSAGE_WAITING_EN;
            pause_msg_menu.insert        = MESSAGE_INSERT_EN;
            pause_msg_menu.load          = MESSAGE_LOAD_EN;
            pause_msg_menu.purge         = MESSAGE_PURGE_EN;
            pause_msg_menu.resume        = MESSAGE_RESUME_EN;
            pause_msg_menu.heat          = MESSAGE_HEAT_EN;
            pause_msg_menu.heating       = MESSAGE_HEATING_EN;
            pause_msg_menu.option        = MESSAGE_OPTION_EN;
            pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_EN;
            pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_EN;
            eeprom_menu.title            = EEPROM_SETTINGS_TITLE_EN;
            eeprom_menu.store            = EEPROM_SETTINGS_STORE_EN;
            eeprom_menu.read             = EEPROM_SETTINGS_READ_EN;
            eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_EN;
            eeprom_menu.storeTips        = EEPROM_STORE_TIPS_EN;
            eeprom_menu.readTips         = EEPROM_READ_TIPS_EN;
            eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_EN;
            break;
          case LANG_RUSSIAN:
            common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_RU;
            common_menu.text_back             = BACK_TEXT_RU;
            common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_RU;
            common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_RU;
            common_menu.print_special_title   = PRINTING_OTHER_LANGUGE;
            common_menu.pause_special_title   = PRINTING_PAUSE_OTHER_LANGUGE;
            common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;
            //
            main_menu.title        = TITLE_READYPRINT_RU;
            main_menu.preheat      = PREHEAT_TEXT_RU;
            main_menu.move         = MOVE_TEXT_RU;
            main_menu.home         = HOME_TEXT_RU;
            main_menu.print        = PRINT_TEXT_RU;
            main_menu.extrude      = EXTRUDE_TEXT_RU;
            main_menu.leveling     = LEVELING_TEXT_RU;
            main_menu.autoleveling = AUTO_LEVELING_TEXT_RU;
            main_menu.fan          = FAN_TEXT_RU;
            main_menu.set          = SET_TEXT_RU;
            main_menu.more         = MORE_TEXT_RU;
            main_menu.tool         = TOOL_TEXT_RU;
            // TOOL
            tool_menu.title        = TOOL_TEXT_RU;
            tool_menu.preheat      = TOOL_PREHEAT_RU;
            tool_menu.extrude      = TOOL_EXTRUDE_RU;
            tool_menu.move         = TOOL_MOVE_RU;
            tool_menu.home         = TOOL_HOME_RU;
            tool_menu.leveling     = TOOL_LEVELING_RU;
            tool_menu.autoleveling = TOOL_AUTO_LEVELING_RU;
            tool_menu.filament     = TOOL_FILAMENT_RU;
            tool_menu.more         = TOOL_MORE_RU;
            //
            preheat_menu.adjust_title = TITLE_ADJUST_RU;
            preheat_menu.title        = TITLE_PREHEAT_RU;
            preheat_menu.add          = ADD_TEXT_RU;
            preheat_menu.dec          = DEC_TEXT_RU;
            preheat_menu.ext1         = EXTRUDER_1_TEXT_RU;
            preheat_menu.ext2         = EXTRUDER_2_TEXT_RU;
            preheat_menu.hotbed       = HEATBED_TEXT_RU;
            preheat_menu.off          = CLOSE_TEXT_RU;
            //
            move_menu.title    = MOVE_TEXT_RU;
            //
            home_menu.title    = TITLE_HOME_RU;
            home_menu.stopmove = HOME_STOPMOVE_RU;
            //
            file_menu.title             = TITLE_CHOOSEFILE_RU;
            file_menu.page_up           = PAGE_UP_TEXT_RU;
            file_menu.page_down         = PAGE_DOWN_TEXT_RU;
            file_menu.file_loading      = FILE_LOADING_RU;
            file_menu.no_file           = NO_FILE_RU;
            file_menu.no_file_and_check = NO_FILE_RU;
            //
            extrude_menu.title       = TITLE_EXTRUDE_RU;
            extrude_menu.in          = EXTRUDER_IN_TEXT_RU;
            extrude_menu.out         = EXTRUDER_OUT_TEXT_RU;
            extrude_menu.ext1        = EXTRUDER_1_TEXT_RU;
            extrude_menu.ext2        = EXTRUDER_2_TEXT_RU;
            extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_RU;
            extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_RU;
            extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_RU;
            extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_RU;
            //
            leveling_menu.title     = TITLE_LEVELING_RU;
            leveling_menu.position1 = LEVELING_POINT1_TEXT_RU;
            leveling_menu.position2 = LEVELING_POINT2_TEXT_RU;
            leveling_menu.position3 = LEVELING_POINT3_TEXT_RU;
            leveling_menu.position4 = LEVELING_POINT4_TEXT_RU;
            leveling_menu.position5 = LEVELING_POINT5_TEXT_RU;
            //
            set_menu.title        = TITLE_SET_RU;
            set_menu.filesys      = FILESYS_TEXT_RU;
            set_menu.wifi         = WIFI_TEXT_RU;
            set_menu.about        = ABOUT_TEXT_RU;
            set_menu.fan          = FAN_TEXT_RU;
            set_menu.filament     = FILAMENT_TEXT_RU;
            set_menu.breakpoint   = BREAK_POINT_TEXT_RU;
            set_menu.motoroff     = MOTOR_OFF_TEXT_RU;
            set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_RU;
            set_menu.language     = LANGUAGE_TEXT_RU;
            set_menu.shutdown     = SHUTDOWN_TEXT_RU;
            set_menu.machine_para = MACHINE_PARA_RU;
            set_menu.eepromSet    = EEPROM_SETTINGS_RU;
            more_menu.title       = TITLE_MORE_RU;
            more_menu.gcode       = MORE_GCODE_RU;
            more_menu.entergcode  = MORE_ENTER_GCODE_RU;
            #if HAS_USER_ITEM(1)
              more_menu.custom1 = MORE_CUSTOM1_TEXT_RU;
            #endif
            #if HAS_USER_ITEM(2)
              more_menu.custom2 = MORE_CUSTOM2_TEXT_RU;
            #endif
            #if HAS_USER_ITEM(3)
              more_menu.custom3 = MORE_CUSTOM3_TEXT_RU;
            #endif
            #if HAS_USER_ITEM(4)
              more_menu.custom4 = MORE_CUSTOM4_TEXT_RU;
            #endif
            #if HAS_USER_ITEM(5)
              more_menu.custom5 = MORE_CUSTOM5_TEXT_RU;
            #endif
            #if HAS_USER_ITEM(6)
              more_menu.custom6 = MORE_CUSTOM6_TEXT_RU;
            #endif
            //
            filesys_menu.title   = TITLE_FILESYS_RU;
            filesys_menu.sd_sys  = SD_CARD_TEXT_RU;
            filesys_menu.usb_sys = U_DISK_TEXT_RU;
            // WIFI
            wifi_menu.title     = WIFI_TEXT;
            wifi_menu.cloud     = CLOUD_TEXT_RU;
            wifi_menu.reconnect = WIFI_RECONNECT_TEXT_RU;

            machine_menu.next          = NEXT_RU;
            machine_menu.previous      = PREVIOUS_RU;
            machine_menu.enable        = ENABLE_RU;
            machine_menu.disable       = DISABLE_RU;
            machine_menu.key_confirm   = KEY_CONFIRM_RU;

            MachinePara_menu.MachineSetting    = MACHINE_TYPE_CNOFIG_RU;
            MachinePara_menu.title             = MACHINE_PARA_TITLE_RU;
            machine_menu.MachineConfigTitle    = MACHINE_CONFIG_TITLE_RU;
            MachinePara_menu.MotorSetting      = MOTOR_CONFIG_RU;
            MachinePara_menu.leveling          = MACHINE_LEVELING_CONFIG_RU;
            MachinePara_menu.AdvanceSetting    = ADVANCE_CONFIG_RU;
            machine_menu.MotorConfTitle        = MOTOR_CONF_TITLE_RU;
            machine_menu.MaxFeedRateConf       = MAXFEEDRATE_CONF_RU;
            machine_menu.AccelerationConf      = ACCELERATION_CONF_RU;
            machine_menu.JerkConf              = JERKCONF_RU;
            machine_menu.StepsConf             = STEPSCONF_RU;
            machine_menu.TMCcurrentConf        = TMC_CURRENT_RU;
            machine_menu.TMCStepModeConf       = TMC_STEP_MODE_RU;
            machine_menu.PausePosition         = PAUSE_POSITION_RU;
            machine_menu.FilamentConf          = MACHINE_FILAMENT_CONFIG_RU;
            machine_menu.EncoderSettings       = ENCODER_SETTINGS_RU;
            machine_menu.AdvancedConfTitle     = ADVANCED_CONF_TITLE_RU;

            machine_menu.LevelingParaConfTitle   = LEVELING_CONF_TITLE_RU;
            machine_menu.LevelingParaConf        = LEVELING_PARA_CONF_RU;
            machine_menu.TrammingPosConf         = TRAMMING_POS_RU;
            machine_menu.LevelingAutoCommandConf = LEVELING_AUTO_COMMAND_RU;
            machine_menu.LevelingAutoZoffsetConf = LEVELING_AUTO_ZOFFSET_RU;

            machine_menu.AccelerationConfTitle = ACCELERATION_CONF_TITLE_RU;
            machine_menu.PrintAcceleration     = PRINT_ACCELERATION_RU;
            machine_menu.RetractAcceleration   = RETRACT_ACCELERATION_RU;
            machine_menu.TravelAcceleration    = TRAVEL_ACCELERATION_RU;
            machine_menu.X_Acceleration        = X_ACCELERATION_RU;
            machine_menu.Y_Acceleration        = Y_ACCELERATION_RU;
            machine_menu.Z_Acceleration        = Z_ACCELERATION_RU;
            machine_menu.E0_Acceleration       = E0_ACCELERATION_RU;
            machine_menu.E1_Acceleration       = E1_ACCELERATION_RU;

            machine_menu.MaxFeedRateConfTitle = MAXFEEDRATE_CONF_TITLE_RU;
            machine_menu.XMaxFeedRate         = X_MAXFEEDRATE_RU;
            machine_menu.YMaxFeedRate         = Y_MAXFEEDRATE_RU;
            machine_menu.ZMaxFeedRate         = Z_MAXFEEDRATE_RU;
            machine_menu.E0MaxFeedRate        = E0_MAXFEEDRATE_RU;
            machine_menu.E1MaxFeedRate        = E1_MAXFEEDRATE_RU;

            machine_menu.JerkConfTitle = JERK_CONF_TITLE_RU;
            machine_menu.X_Jerk        = X_JERK_RU;
            machine_menu.Y_Jerk        = Y_JERK_RU;
            machine_menu.Z_Jerk        = Z_JERK_RU;
            machine_menu.E_Jerk        = E_JERK_RU;

            machine_menu.StepsConfTitle = STEPS_CONF_TITLE_RU;
            machine_menu.X_Steps        = X_STEPS_RU;
            machine_menu.Y_Steps        = Y_STEPS_RU;
            machine_menu.Z_Steps        = Z_STEPS_RU;
            machine_menu.E0_Steps       = E0_STEPS_RU;
            machine_menu.E1_Steps       = E1_STEPS_RU;

            machine_menu.TmcCurrentConfTitle = TMC_CURRENT_CONF_TITLE_RU;
            machine_menu.X_Current           = X_TMC_CURRENT_RU;
            machine_menu.Y_Current           = Y_TMC_CURRENT_RU;
            machine_menu.Z_Current           = Z_TMC_CURRENT_RU;
            machine_menu.E0_Current          = E0_TMC_CURRENT_RU;
            machine_menu.E1_Current          = E1_TMC_CURRENT_RU;

            machine_menu.TmcStepModeConfTitle = TMC_MODE_CONF_TITLE_RU;
            machine_menu.X_StepMode           = X_TMC_MODE_RU;
            machine_menu.Y_StepMode           = Y_TMC_MODE_RU;
            machine_menu.Z_StepMode           = Z_TMC_MODE_RU;
            machine_menu.E0_StepMode          = E0_TMC_MODE_RU;
            machine_menu.E1_StepMode          = E1_TMC_MODE_RU;

            machine_menu.PausePosText   = PAUSE_POSITION_RU;
            machine_menu.xPos           = PAUSE_POSITION_X_RU;
            machine_menu.yPos           = PAUSE_POSITION_Y_RU;
            machine_menu.zPos           = PAUSE_POSITION_Z_RU;

            machine_menu.OffsetConfTitle = OFFSET_TITLE_RU;
            machine_menu.Xoffset         = OFFSET_X_RU;
            machine_menu.Yoffset         = OFFSET_Y_RU;
            machine_menu.Zoffset         = OFFSET_Z_RU;

            machine_menu.FilamentConfTitle   = FILAMENT_CONF_TITLE_RU;
            machine_menu.InLength            = FILAMENT_IN_LENGTH_RU;
            machine_menu.InSpeed             = FILAMENT_IN_SPEED_RU;
            machine_menu.FilamentTemperature = FILAMENT_TEMPERATURE_RU;
            machine_menu.OutLength           = FILAMENT_OUT_LENGTH_RU;
            machine_menu.OutSpeed            = FILAMENT_OUT_SPEED_RU;

            machine_menu.EncoderConfTitle    = ENCODER_CONF_TITLE_RU;
            machine_menu.EncoderConfText     = ENCODER_CONF_TEXT_RU;

            cloud_menu.title        = TITLE_CLOUD_TEXT_RU;
            cloud_menu.bind         = CLOUD_BINDED_RU;
            cloud_menu.binded       = CLOUD_BINDED_RU;
            cloud_menu.unbind       = CLOUD_UNBIND_RU;
            cloud_menu.unbinding    = CLOUD_UNBINDED_RU;
            cloud_menu.disconnected = CLOUD_DISCONNECTED_RU;
            cloud_menu.unbinded     = CLOUD_UNBINDED_RU;
            cloud_menu.disable      = CLOUD_DISABLE_RU;
            //
            about_menu.title   = ABOUT_TEXT_RU;
            about_menu.type    = ABOUT_TYPE_TEXT_RU;
            about_menu.version = ABOUT_VERSION_TEXT_RU;
            about_menu.wifi    = ABOUT_WIFI_TEXT_RU;
            //
            fan_menu.title = FAN_TEXT_RU;
            fan_menu.add   = FAN_ADD_TEXT_RU;
            fan_menu.dec   = FAN_DEC_TEXT_RU;
            fan_menu.state = FAN_TIPS1_TEXT_RU;
            //
            filament_menu.title                               = TITLE_FILAMENT_RU;
            filament_menu.in                                  = FILAMENT_IN_TEXT_RU;
            filament_menu.out                                 = FILAMENT_OUT_TEXT_RU;
            filament_menu.ext1                                = FILAMENT_EXT0_TEXT_RU;
            filament_menu.ext2                                = FILAMENT_EXT1_TEXT_RU;
            filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_RU;
            filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_RU;
            filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_RU;
            filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_RU;
            filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_RU;
            filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_RU;
            filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_RU;
            filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_RU;
            filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_RU;

            //
            language_menu.title = LANGUAGE_TEXT_RU;
            language_menu.next  = PAGE_DOWN_TEXT_RU;
            language_menu.up    = PAGE_UP_TEXT_RU;
            //
            printing_menu.title  = TITLE_PRINTING_RU;
            printing_menu.option = PRINTING_OPERATION_RU;
            printing_menu.stop   = PRINTING_STOP_RU;
            printing_menu.pause  = PRINTING_PAUSE_RU;
            printing_menu.resume = PRINTING_RESUME_RU;

            //
            operation_menu.title      = TITLE_OPERATION_RU;
            operation_menu.pause      = PRINTING_PAUSE_RU;
            operation_menu.stop       = PRINTING_STOP_RU;
            operation_menu.temp       = PRINTING_TEMP_RU;
            operation_menu.fan        = FAN_TEXT_RU;
            operation_menu.extr       = PRINTING_EXTRUDER_RU;
            operation_menu.speed      = PRINTING_CHANGESPEED_RU;
            operation_menu.filament   = FILAMENT_TEXT_RU;
            operation_menu.more       = PRINTING_MORE_RU;
            operation_menu.move       = PRINTING_MOVE_RU;
            operation_menu.auto_off   = AUTO_SHUTDOWN_RU;
            operation_menu.manual_off = MANUAL_SHUTDOWN_RU;
            //
            pause_menu.title    = TITLE_PAUSE_RU;
            pause_menu.resume   = PRINTING_RESUME_RU;
            pause_menu.stop     = PRINTING_STOP_RU;
            pause_menu.extrude  = PRINTING_EXTRUDER_RU;
            pause_menu.move     = PRINTING_MOVE_RU;
            pause_menu.filament = FILAMENT_TEXT_RU;
            pause_menu.more     = PRINTING_MORE_RU;

            //
            speed_menu.title         = PRINTING_CHANGESPEED_RU;
            speed_menu.add           = ADD_TEXT_RU;
            speed_menu.dec           = DEC_TEXT_RU;
            speed_menu.move          = MOVE_SPEED_RU;
            speed_menu.extrude       = EXTRUDER_SPEED_RU;
            speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_RU;
            speed_menu.move_speed    = MOVE_SPEED_STATE_RU;
            //
            printing_more_menu.title      = TITLE_MORE_RU;
            printing_more_menu.fan        = FAN_TEXT_RU;
            printing_more_menu.auto_close = AUTO_SHUTDOWN_RU;
            printing_more_menu.manual     = MANUAL_SHUTDOWN_RU;
            printing_more_menu.speed      = PRINTING_CHANGESPEED_RU;
            printing_more_menu.temp       = PRINTING_TEMP_RU;
            print_file_dialog_menu.confirm               = DIALOG_CONFIRM_RU;
            print_file_dialog_menu.cancel                = DIALOG_CANCLE_RU;
            print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_RU;
            print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_RU;
            print_file_dialog_menu.retry                 = DIALOG_RETRY_RU;
            print_file_dialog_menu.stop                  = DIALOG_STOP_RU;
            print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_RU;
            print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_RU;
            print_file_dialog_menu.close_machine_error   = DIALOG_ERROR_TIPS2_RU;
            print_file_dialog_menu.filament_no_press     = DIALOG_FILAMENT_NO_PRESS_RU;
            print_file_dialog_menu.print_finish          = DIALOG_PRINT_FINISH_RU;
            print_file_dialog_menu.print_time            = DIALOG_PRINT_TIME_RU;
            print_file_dialog_menu.reprint               = DIALOG_REPRINT_RU;
            print_file_dialog_menu.wifi_enable_tips      = DIALOG_WIFI_ENABLE_TIPS_RU;

            pause_msg_menu.pausing       = MESSAGE_PAUSING_RU;
            pause_msg_menu.changing      = MESSAGE_CHANGING_RU;
            pause_msg_menu.unload        = MESSAGE_UNLOAD_RU;
            pause_msg_menu.waiting       = MESSAGE_WAITING_RU;
            pause_msg_menu.insert        = MESSAGE_INSERT_RU;
            pause_msg_menu.load          = MESSAGE_LOAD_RU;
            pause_msg_menu.purge         = MESSAGE_PURGE_RU;
            pause_msg_menu.resume        = MESSAGE_RESUME_RU;
            pause_msg_menu.heat          = MESSAGE_HEAT_RU;
            pause_msg_menu.heating       = MESSAGE_HEATING_RU;
            pause_msg_menu.option        = MESSAGE_OPTION_RU;
            pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_RU;
            pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_RU;
            eeprom_menu.title            = EEPROM_SETTINGS_TITLE_RU;
            eeprom_menu.store            = EEPROM_SETTINGS_STORE_RU;
            eeprom_menu.read             = EEPROM_SETTINGS_READ_RU;
            eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_RU;
            eeprom_menu.storeTips        = EEPROM_STORE_TIPS_RU;
            eeprom_menu.readTips         = EEPROM_READ_TIPS_RU;
            eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_RU;
            break;
          case LANG_SPANISH:
            common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_SP;
            common_menu.text_back             = BACK_TEXT_SP;
            common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_SP;
            common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_SP;
            common_menu.print_special_title   = PRINTING_SP;
            common_menu.pause_special_title   = PRINTING_PAUSAR_SP;
            common_menu.operate_special_title = PRINTING_AJUSTES_SP;
            //
            main_menu.title        = TITLE_READYPRINT_SP;
            main_menu.preheat      = PREHEAT_TEXT_SP;
            main_menu.move         = MOVE_TEXT_SP;
            main_menu.home         = HOME_TEXT_SP;
            main_menu.print        = PRINT_TEXT_SP;
            main_menu.extrude      = EXTRUDE_TEXT_SP;
            main_menu.leveling     = LEVELING_TEXT_SP;
            main_menu.autoleveling = AUTO_LEVELING_TEXT_SP;
            main_menu.fan          = FAN_TEXT_SP;
            main_menu.set          = SET_TEXT_SP;
            main_menu.more         = MORE_TEXT_SP;
            main_menu.tool         = TOOL_TEXT_SP;
            // TOOL
            tool_menu.title        = TOOL_TEXT_SP;
            tool_menu.preheat      = TOOL_PREHEAT_SP;
            tool_menu.extrude      = TOOL_EXTRUDE_SP;
            tool_menu.move         = TOOL_MOVE_SP;
            tool_menu.home         = TOOL_HOME_SP;
            tool_menu.leveling     = TOOL_LEVELING_SP;
            tool_menu.autoleveling = TOOL_AUTO_LEVELING_SP;
            tool_menu.filament     = TOOL_FILAMENT_SP;
            tool_menu.more         = TOOL_MORE_SP;
            //
            preheat_menu.adjust_title = TITLE_ADJUST_SP;
            preheat_menu.title        = TITLE_PREHEAT_SP;
            preheat_menu.add          = ADD_TEXT_SP;
            preheat_menu.dec          = DEC_TEXT_SP;
            preheat_menu.ext1         = EXTRUDER_1_TEXT_SP;
            preheat_menu.ext2         = EXTRUDER_2_TEXT_SP;
            preheat_menu.hotbed       = HEATBED_TEXT_SP;
            preheat_menu.off          = CLOSE_TEXT_SP;
            //
            move_menu.title = MOVE_TEXT_SP;
            //
            home_menu.title    = TITLE_HOME_SP;
            home_menu.home_x   = HOME_X_TEXT_SP;
            home_menu.home_y   = HOME_Y_TEXT_SP;
            home_menu.home_z   = HOME_Z_TEXT_SP;
            home_menu.home_all = HOME_ALL_TEXT_SP;
            home_menu.stopmove = HOME_STOPMOVE_SP;
            //
            file_menu.title             = TITLE_CHOOSEFILE_SP;
            file_menu.page_up           = PAGE_UP_TEXT_SP;
            file_menu.page_down         = PAGE_DOWN_TEXT_SP;
            file_menu.file_loading      = FILE_LOADING_SP;
            file_menu.no_file           = NO_FILE_SP;
            file_menu.no_file_and_check = NO_FILE_SP;
            //
            extrude_menu.title       = TITLE_EXTRUDE_SP;
            extrude_menu.in          = EXTRUDER_IN_TEXT_SP;
            extrude_menu.out         = EXTRUDER_OUT_TEXT_SP;
            extrude_menu.ext1        = EXTRUDER_1_TEXT_SP;
            extrude_menu.ext2        = EXTRUDER_2_TEXT_SP;
            extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_SP;
            extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_SP;
            extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_SP;
            extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_SP;
            //
            leveling_menu.title     = TITLE_LEVELING_SP;
            leveling_menu.position1 = LEVELING_POINT1_TEXT_SP;
            leveling_menu.position2 = LEVELING_POINT2_TEXT_SP;
            leveling_menu.position3 = LEVELING_POINT3_TEXT_SP;
            leveling_menu.position4 = LEVELING_POINT4_TEXT_SP;
            leveling_menu.position5 = LEVELING_POINT5_TEXT_SP;
            //
            set_menu.title        = TITLE_SET_SP;
            set_menu.filesys      = FILESYS_TEXT_SP;
            set_menu.wifi         = WIFI_TEXT_SP;
            set_menu.about        = ABOUT_TEXT_SP;
            set_menu.fan          = FAN_TEXT_SP;
            set_menu.filament     = FILAMENT_TEXT_SP;
            set_menu.breakpoint   = BREAK_POINT_TEXT_SP;
            set_menu.motoroff     = MOTOR_OFF_TEXT_SP;
            set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_SP;
            set_menu.language     = LANGUAGE_TEXT_SP;
            set_menu.shutdown     = SHUTDOWN_TEXT_SP;
            set_menu.machine_para = MACHINE_PARA_SP;
            set_menu.eepromSet    = EEPROM_SETTINGS_SP;
            more_menu.title       = TITLE_MORE_SP;
            more_menu.gcode       = MORE_GCODE_SP;
            more_menu.entergcode  = MORE_ENTER_GCODE_SP;
            #if HAS_USER_ITEM(1)
              more_menu.custom1 = MORE_CUSTOM1_TEXT_SP;
            #endif
            #if HAS_USER_ITEM(2)
              more_menu.custom2 = MORE_CUSTOM2_TEXT_SP;
            #endif
            #if HAS_USER_ITEM(3)
              more_menu.custom3 = MORE_CUSTOM3_TEXT_SP;
            #endif
            #if HAS_USER_ITEM(4)
              more_menu.custom4 = MORE_CUSTOM4_TEXT_SP;
            #endif
            #if HAS_USER_ITEM(5)
              more_menu.custom5 = MORE_CUSTOM5_TEXT_SP;
            #endif
            #if HAS_USER_ITEM(6)
              more_menu.custom6 = MORE_CUSTOM6_TEXT_SP;
            #endif
            //
            filesys_menu.title   = TITLE_FILESYS_SP;
            filesys_menu.sd_sys  = SD_CARD_TEXT_SP;
            filesys_menu.usb_sys = U_DISK_TEXT_SP;

            // WIFI
            wifi_menu.title     = WIFI_TEXT;
            wifi_menu.cloud     = CLOUD_TEXT_SP;
            wifi_menu.reconnect = WIFI_RECONNECT_TEXT_SP;

            cloud_menu.title        = TITLE_CLOUD_TEXT_SP;
            cloud_menu.bind         = CLOUD_BINDED_SP;
            cloud_menu.binded       = CLOUD_BINDED_SP;
            cloud_menu.unbind       = CLOUD_UNBIND_SP;
            cloud_menu.unbinding    = CLOUD_UNBINDED_SP;
            cloud_menu.disconnected = CLOUD_DISCONNECTED_SP;
            cloud_menu.unbinded     = CLOUD_UNBINDED_SP;
            cloud_menu.disable      = CLOUD_DISABLE_SP;
            //
            about_menu.title   = ABOUT_TEXT_SP;
            about_menu.type    = ABOUT_TYPE_TEXT_SP;
            about_menu.version = ABOUT_VERSION_TEXT_SP;
            about_menu.wifi    = ABOUT_WIFI_TEXT_SP;
            //
            fan_menu.title = FAN_TEXT_SP;
            fan_menu.add   = FAN_ADD_TEXT_SP;
            fan_menu.dec   = FAN_DEC_TEXT_SP;
            fan_menu.state = FAN_TIPS1_TEXT_SP;
            //
            filament_menu.title                               = TITLE_FILAMENT_SP;
            filament_menu.in                                  = FILAMENT_IN_TEXT_SP;
            filament_menu.out                                 = FILAMENT_OUT_TEXT_SP;
            filament_menu.ext1                                = FILAMENT_EXT0_TEXT_SP;
            filament_menu.ext2                                = FILAMENT_EXT1_TEXT_SP;
            filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_SP;
            filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_SP;
            filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_SP;
            filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_SP;
            filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_SP;
            filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_SP;
            filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_SP;
            filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_SP;
            filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_SP;

            //
            language_menu.title = LANGUAGE_TEXT_SP;
            language_menu.next  = PAGE_DOWN_TEXT_SP;
            language_menu.up    = PAGE_UP_TEXT_SP;
            //
            printing_menu.title  = TITLE_PRINTING_SP;
            printing_menu.option = PRINTING_OPERATION_SP;
            printing_menu.stop   = PRINTING_STOP_SP;
            printing_menu.pause  = PRINTING_PAUSE_SP;
            printing_menu.resume = PRINTING_RESUME_SP;

            //
            operation_menu.title      = TITLE_OPERATION_SP;
            operation_menu.pause      = PRINTING_PAUSE_SP;
            operation_menu.stop       = PRINTING_STOP_SP;
            operation_menu.temp       = PRINTING_TEMP_SP;
            operation_menu.fan        = FAN_TEXT_SP;
            operation_menu.extr       = PRINTING_EXTRUDER_SP;
            operation_menu.speed      = PRINTING_CHANGESPEED_SP;
            operation_menu.filament   = FILAMENT_TEXT_SP;
            operation_menu.more       = PRINTING_MORE_SP;
            operation_menu.move       = PRINTING_MOVE_SP;
            operation_menu.auto_off   = AUTO_SHUTDOWN_SP;
            operation_menu.manual_off = MANUAL_SHUTDOWN_SP;
            //
            pause_menu.title    = TITLE_PAUSE_RU;
            pause_menu.resume   = PRINTING_RESUME_SP;
            pause_menu.stop     = PRINTING_STOP_SP;
            pause_menu.extrude  = PRINTING_EXTRUDER_SP;
            pause_menu.move     = PRINTING_MOVE_SP;
            pause_menu.filament = FILAMENT_TEXT_SP;
            pause_menu.more     = PRINTING_MORE_SP;

            //
            speed_menu.title         = PRINTING_CHANGESPEED_SP;
            speed_menu.add           = ADD_TEXT_SP;
            speed_menu.dec           = DEC_TEXT_SP;
            speed_menu.move          = MOVE_SPEED_SP;
            speed_menu.extrude       = EXTRUDER_SPEED_SP;
            speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_SP;
            speed_menu.move_speed    = MOVE_SPEED_STATE_SP;
            //
            printing_more_menu.title      = TITLE_MORE_SP;
            printing_more_menu.fan        = FAN_TEXT_SP;
            printing_more_menu.auto_close = AUTO_SHUTDOWN_SP;
            printing_more_menu.manual     = MANUAL_SHUTDOWN_SP;
            printing_more_menu.speed      = PRINTING_CHANGESPEED_SP;
            printing_more_menu.temp       = PRINTING_TEMP_SP;

            print_file_dialog_menu.confirm               = DIALOG_CONFIRM_SP;
            print_file_dialog_menu.cancel                = DIALOG_CANCLE_SP;
            print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_SP;
            print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_SP;
            print_file_dialog_menu.retry                 = DIALOG_RETRY_SP;
            print_file_dialog_menu.stop                  = DIALOG_STOP_SP;
            print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_SP;
            print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_SP;
            print_file_dialog_menu.close_machine_error   = DIALOG_ERROR_TIPS2_SP;
            print_file_dialog_menu.filament_no_press     = DIALOG_FILAMENT_NO_PRESS_SP;
            print_file_dialog_menu.print_finish          = DIALOG_PRINT_FINISH_SP;
            print_file_dialog_menu.print_time            = DIALOG_PRINT_TIME_SP;
            print_file_dialog_menu.reprint               = DIALOG_REPRINT_SP;
            print_file_dialog_menu.wifi_enable_tips      = DIALOG_WIFI_ENABLE_TIPS_SP;

            pause_msg_menu.pausing       = MESSAGE_PAUSING_SP;
            pause_msg_menu.changing      = MESSAGE_CHANGING_SP;
            pause_msg_menu.unload        = MESSAGE_UNLOAD_SP;
            pause_msg_menu.waiting       = MESSAGE_WAITING_SP;
            pause_msg_menu.insert        = MESSAGE_INSERT_SP;
            pause_msg_menu.load          = MESSAGE_LOAD_SP;
            pause_msg_menu.purge         = MESSAGE_PURGE_SP;
            pause_msg_menu.resume        = MESSAGE_RESUME_SP;
            pause_msg_menu.heat          = MESSAGE_HEAT_SP;
            pause_msg_menu.heating       = MESSAGE_HEATING_SP;
            pause_msg_menu.option        = MESSAGE_OPTION_SP;
            pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_SP;
            pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_SP;
            eeprom_menu.title            = EEPROM_SETTINGS_TITLE_SP;
            eeprom_menu.store            = EEPROM_SETTINGS_STORE_SP;
            eeprom_menu.read             = EEPROM_SETTINGS_READ_SP;
            eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_SP;
            eeprom_menu.storeTips        = EEPROM_STORE_TIPS_SP;
            eeprom_menu.readTips         = EEPROM_READ_TIPS_SP;
            eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_SP;
            break;

        #endif // if 1

        case LANG_FRENCH:
          common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_FR;
          common_menu.text_back             = BACK_TEXT_FR;
          common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_FR;
          common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_FR;
          common_menu.print_special_title   = PRINTING_OTHER_LANGUGE;
          common_menu.pause_special_title   = PRINTING_PAUSE_OTHER_LANGUGE;
          common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;

          //
          main_menu.title        = TITLE_READYPRINT_FR;
          main_menu.preheat      = PREHEAT_TEXT_FR;
          main_menu.move         = MOVE_TEXT_FR;
          main_menu.home         = HOME_TEXT_FR;
          main_menu.print        = PRINT_TEXT_FR;
          main_menu.extrude      = EXTRUDE_TEXT_FR;
          main_menu.leveling     = LEVELING_TEXT_FR;
          main_menu.autoleveling = AUTO_LEVELING_TEXT_FR;
          main_menu.fan          = FAN_TEXT_FR;
          main_menu.set          = SET_TEXT_FR;
          main_menu.more         = MORE_TEXT_FR;
          main_menu.tool         = TOOL_TEXT_FR;
          // TOOL
          tool_menu.title        = TOOL_TEXT_FR;
          tool_menu.preheat      = TOOL_PREHEAT_FR;
          tool_menu.extrude      = TOOL_EXTRUDE_FR;
          tool_menu.move         = TOOL_MOVE_FR;
          tool_menu.home         = TOOL_HOME_FR;
          tool_menu.leveling     = TOOL_LEVELING_FR;
          tool_menu.autoleveling = TOOL_AUTO_LEVELING_FR;
          tool_menu.filament     = TOOL_FILAMENT_FR;
          tool_menu.more         = TOOL_MORE_FR;
          //
          preheat_menu.adjust_title = TITLE_ADJUST_FR;
          preheat_menu.title        = TITLE_PREHEAT_FR;
          preheat_menu.add          = ADD_TEXT_FR;
          preheat_menu.dec          = DEC_TEXT_FR;
          preheat_menu.ext1         = EXTRUDER_1_TEXT_FR;
          preheat_menu.ext2         = EXTRUDER_2_TEXT_FR;
          preheat_menu.hotbed       = HEATBED_TEXT_FR;
          preheat_menu.off          = CLOSE_TEXT_FR;
          //
          move_menu.title = MOVE_TEXT_FR;
          //
          home_menu.title    = TITLE_HOME_FR;
          home_menu.stopmove = HOME_STOPMOVE_FR;
          //
          file_menu.title     = TITLE_CHOOSEFILE_FR;
          file_menu.page_up   = PAGE_UP_TEXT_FR;
          file_menu.page_down = PAGE_DOWN_TEXT_FR;
          //
          extrude_menu.title       = TITLE_EXTRUDE_FR;
          extrude_menu.in          = EXTRUDER_IN_TEXT_FR;
          extrude_menu.out         = EXTRUDER_OUT_TEXT_FR;
          extrude_menu.ext1        = EXTRUDER_1_TEXT_FR;
          extrude_menu.ext2        = EXTRUDER_2_TEXT_FR;
          extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_FR;
          extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_FR;
          extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_FR;
          extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_FR;
          //
          leveling_menu.title     = TITLE_LEVELING_FR;
          leveling_menu.position1 = LEVELING_POINT1_TEXT_FR;
          leveling_menu.position2 = LEVELING_POINT2_TEXT_FR;
          leveling_menu.position3 = LEVELING_POINT3_TEXT_FR;
          leveling_menu.position4 = LEVELING_POINT4_TEXT_FR;
          leveling_menu.position5 = LEVELING_POINT5_TEXT_FR;
          //
          set_menu.title        = TITLE_SET_FR;
          set_menu.filesys      = FILESYS_TEXT_FR;
          set_menu.wifi         = WIFI_TEXT_FR;
          set_menu.about        = ABOUT_TEXT_FR;
          set_menu.fan          = FAN_TEXT_FR;
          set_menu.filament     = FILAMENT_TEXT_FR;
          set_menu.breakpoint   = BREAK_POINT_TEXT_FR;
          set_menu.motoroff     = MOTOR_OFF_TEXT_FR;
          set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_FR;
          set_menu.language     = LANGUAGE_TEXT_FR;
          set_menu.shutdown     = SHUTDOWN_TEXT_FR;
          set_menu.machine_para = MACHINE_PARA_FR;
          set_menu.eepromSet    = EEPROM_SETTINGS_FR;
          more_menu.title       = TITLE_MORE_FR;
          more_menu.gcode       = MORE_GCODE_FR;
          more_menu.entergcode  = MORE_ENTER_GCODE_FR;
          #if HAS_USER_ITEM(1)
            more_menu.custom1 = MORE_CUSTOM1_TEXT_FR;
          #endif
          #if HAS_USER_ITEM(2)
            more_menu.custom2 = MORE_CUSTOM2_TEXT_FR;
          #endif
          #if HAS_USER_ITEM(3)
            more_menu.custom3 = MORE_CUSTOM3_TEXT_FR;
          #endif
          #if HAS_USER_ITEM(4)
            more_menu.custom4 = MORE_CUSTOM4_TEXT_FR;
          #endif
          #if HAS_USER_ITEM(5)
            more_menu.custom5 = MORE_CUSTOM5_TEXT_FR;
          #endif
          #if HAS_USER_ITEM(6)
            more_menu.custom6 = MORE_CUSTOM6_TEXT_FR;
          #endif
          //
          filesys_menu.title          = TITLE_FILESYS_FR;
          filesys_menu.sd_sys         = SD_CARD_TEXT_FR;
          filesys_menu.usb_sys        = U_DISK_TEXT_FR;
          file_menu.file_loading      = FILE_LOADING_FR;
          file_menu.no_file           = NO_FILE_FR;
          file_menu.no_file_and_check = NO_FILE_FR;
          // WIFI
          wifi_menu.title     = WIFI_NAME_TEXT_FR;
          wifi_menu.cloud     = CLOUD_TEXT_FR;
          wifi_menu.reconnect = WIFI_RECONNECT_TEXT_FR;

          cloud_menu.title        = TITLE_CLOUD_TEXT_FR;
          cloud_menu.bind         = CLOUD_BINDED_FR;
          cloud_menu.binded       = CLOUD_BINDED_FR;
          cloud_menu.unbind       = CLOUD_UNBIND_FR;
          cloud_menu.unbinding    = CLOUD_UNBINDED_FR;
          cloud_menu.disconnected = CLOUD_DISCONNECTED_FR;
          cloud_menu.unbinded     = CLOUD_UNBINDED_FR;
          cloud_menu.disable      = CLOUD_DISABLE_FR;
          //
          about_menu.title   = ABOUT_TEXT_FR;
          about_menu.type    = ABOUT_TYPE_TEXT_FR;
          about_menu.version = ABOUT_VERSION_TEXT_FR;
          about_menu.wifi    = ABOUT_WIFI_TEXT_FR;
          //
          fan_menu.title = FAN_TEXT_FR;
          fan_menu.add   = FAN_ADD_TEXT_FR;
          fan_menu.dec   = FAN_DEC_TEXT_FR;
          fan_menu.state = FAN_TIPS1_TEXT_FR;
          //
          filament_menu.title                               = TITLE_FILAMENT_FR;
          filament_menu.in                                  = FILAMENT_IN_TEXT_FR;
          filament_menu.out                                 = FILAMENT_OUT_TEXT_FR;
          filament_menu.ext1                                = FILAMENT_EXT0_TEXT_FR;
          filament_menu.ext2                                = FILAMENT_EXT1_TEXT_FR;
          filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_FR;
          filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_FR;
          filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_FR;
          filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_FR;
          filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_FR;
          filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_FR;
          filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_FR;
          filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_FR;
          filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_FR;

          //
          language_menu.title = LANGUAGE_TEXT_FR;

          //
          printing_menu.title  = TITLE_PRINTING_FR;
          printing_menu.option = PRINTING_OPERATION_FR;
          printing_menu.stop   = PRINTING_STOP_FR;
          printing_menu.pause  = PRINTING_PAUSE_FR;
          printing_menu.resume = PRINTING_RESUME_FR;

          //
          operation_menu.title      = TITLE_OPERATION_FR;
          operation_menu.pause      = PRINTING_PAUSE_FR;
          operation_menu.stop       = PRINTING_STOP_FR;
          operation_menu.temp       = PRINTING_TEMP_FR;
          operation_menu.fan        = FAN_TEXT_FR;
          operation_menu.extr       = PRINTING_EXTRUDER_FR;
          operation_menu.speed      = PRINTING_CHANGESPEED_FR;
          operation_menu.filament   = FILAMENT_TEXT_FR;
          operation_menu.more       = PRINTING_MORE_FR;
          operation_menu.move       = PRINTING_MOVE_FR;
          operation_menu.auto_off   = AUTO_SHUTDOWN_FR;
          operation_menu.manual_off = MANUAL_SHUTDOWN_FR;
          //
          pause_menu.title    = TITLE_PAUSE_FR;
          pause_menu.resume   = PRINTING_RESUME_FR;
          pause_menu.stop     = PRINTING_STOP_FR;
          pause_menu.extrude  = PRINTING_EXTRUDER_FR;
          pause_menu.move     = PRINTING_MOVE_FR;
          pause_menu.filament = FILAMENT_TEXT_FR;
          pause_menu.more     = PRINTING_MORE_FR;

          //
          speed_menu.title         = PRINTING_CHANGESPEED_FR;
          speed_menu.add           = ADD_TEXT_FR;
          speed_menu.dec           = DEC_TEXT_FR;
          speed_menu.move          = MOVE_SPEED_FR;
          speed_menu.extrude       = EXTRUDER_SPEED_FR;
          speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_FR;
          speed_menu.move_speed    = MOVE_SPEED_STATE_FR;
          //
          printing_more_menu.fan        = FAN_TEXT_FR;
          printing_more_menu.auto_close = AUTO_SHUTDOWN_FR;
          printing_more_menu.manual     = MANUAL_SHUTDOWN_FR;
          printing_more_menu.speed      = PRINTING_CHANGESPEED_FR;
          printing_more_menu.temp       = PRINTING_TEMP_FR;

          print_file_dialog_menu.confirm               = DIALOG_CONFIRM_FR;
          print_file_dialog_menu.cancel                = DIALOG_CANCLE_FR;
          print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_FR;
          print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_FR;
          print_file_dialog_menu.retry                 = DIALOG_RETRY_FR;
          print_file_dialog_menu.stop                  = DIALOG_STOP_FR;
          print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_FR;
          print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_FR;
          print_file_dialog_menu.close_machine_error   = DIALOG_ERROR_TIPS2_FR;
          print_file_dialog_menu.filament_no_press     = DIALOG_FILAMENT_NO_PRESS_FR;
          print_file_dialog_menu.print_finish          = DIALOG_PRINT_FINISH_FR;
          print_file_dialog_menu.print_time            = DIALOG_PRINT_TIME_FR;
          print_file_dialog_menu.reprint               = DIALOG_REPRINT_FR;
          print_file_dialog_menu.wifi_enable_tips      = DIALOG_WIFI_ENABLE_TIPS_FR;

          pause_msg_menu.pausing       = MESSAGE_PAUSING_FR;
          pause_msg_menu.changing      = MESSAGE_CHANGING_FR;
          pause_msg_menu.unload        = MESSAGE_UNLOAD_FR;
          pause_msg_menu.waiting       = MESSAGE_WAITING_FR;
          pause_msg_menu.insert        = MESSAGE_INSERT_FR;
          pause_msg_menu.load          = MESSAGE_LOAD_FR;
          pause_msg_menu.purge         = MESSAGE_PURGE_FR;
          pause_msg_menu.resume        = MESSAGE_RESUME_FR;
          pause_msg_menu.heat          = MESSAGE_HEAT_FR;
          pause_msg_menu.heating       = MESSAGE_HEATING_FR;
          pause_msg_menu.option        = MESSAGE_OPTION_FR;
          pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_FR;
          pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_FR;
          eeprom_menu.title            = EEPROM_SETTINGS_TITLE_FR;
          eeprom_menu.store            = EEPROM_SETTINGS_STORE_FR;
          eeprom_menu.read             = EEPROM_SETTINGS_READ_FR;
          eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_FR;
          eeprom_menu.storeTips        = EEPROM_STORE_TIPS_FR;
          eeprom_menu.readTips         = EEPROM_READ_TIPS_FR;
          eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_FR;
          break;

        case LANG_ITALY:
          common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_IT;
          common_menu.text_back             = BACK_TEXT_IT;
          common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_IT;
          common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_IT;
          common_menu.print_special_title   = PRINTING_OTHER_LANGUGE;
          common_menu.pause_special_title   = PRINTING_PAUSE_OTHER_LANGUGE;
          common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;

          //
          main_menu.title        = TITLE_READYPRINT_IT;
          main_menu.preheat      = PREHEAT_TEXT_IT;
          main_menu.move         = MOVE_TEXT_IT;
          main_menu.home         = HOME_TEXT_IT;
          main_menu.print        = PRINT_TEXT_IT;
          main_menu.extrude      = EXTRUDE_TEXT_IT;
          main_menu.leveling     = LEVELING_TEXT_IT;
          main_menu.autoleveling = AUTO_LEVELING_TEXT_IT;
          main_menu.fan          = FAN_TEXT_IT;
          main_menu.set          = SET_TEXT_IT;
          main_menu.more         = MORE_TEXT_IT;
          main_menu.tool         = TOOL_TEXT_IT;
          // TOOL
          tool_menu.title        = TOOL_TEXT_IT;
          tool_menu.preheat      = TOOL_PREHEAT_IT;
          tool_menu.extrude      = TOOL_EXTRUDE_IT;
          tool_menu.move         = TOOL_MOVE_IT;
          tool_menu.home         = TOOL_HOME_IT;
          tool_menu.leveling     = TOOL_LEVELING_IT;
          tool_menu.autoleveling = TOOL_AUTO_LEVELING_IT;
          tool_menu.filament     = TOOL_FILAMENT_IT;
          tool_menu.more         = TOOL_MORE_IT;
          //
          preheat_menu.adjust_title = TITLE_ADJUST_IT;
          preheat_menu.title        = TITLE_PREHEAT_IT;
          preheat_menu.add          = ADD_TEXT_IT;
          preheat_menu.dec          = DEC_TEXT_IT;
          preheat_menu.ext1         = EXTRUDER_1_TEXT_IT;
          preheat_menu.ext2         = EXTRUDER_2_TEXT_IT;
          preheat_menu.hotbed       = HEATBED_TEXT_IT;
          preheat_menu.off          = CLOSE_TEXT_IT;
          //
          move_menu.title = MOVE_TEXT_IT;
          //
          home_menu.title    = TITLE_HOME_IT;
          home_menu.stopmove = HOME_STOPMOVE_IT;
          //
          file_menu.title             = TITLE_CHOOSEFILE_IT;
          file_menu.page_up           = PAGE_UP_TEXT_IT;
          file_menu.page_down         = PAGE_DOWN_TEXT_IT;
          file_menu.file_loading      = FILE_LOADING_IT;
          file_menu.no_file           = NO_FILE_IT;
          file_menu.no_file_and_check = NO_FILE_IT;
          //
          extrude_menu.title       = TITLE_EXTRUDE_IT;
          extrude_menu.in          = EXTRUDER_IN_TEXT_IT;
          extrude_menu.out         = EXTRUDER_OUT_TEXT_IT;
          extrude_menu.ext1        = EXTRUDER_1_TEXT_IT;
          extrude_menu.ext2        = EXTRUDER_2_TEXT_IT;
          extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_IT;
          extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_IT;
          extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_IT;
          extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_IT;
          //
          leveling_menu.title     = TITLE_LEVELING_IT;
          leveling_menu.position1 = LEVELING_POINT1_TEXT_IT;
          leveling_menu.position2 = LEVELING_POINT2_TEXT_IT;
          leveling_menu.position3 = LEVELING_POINT3_TEXT_IT;
          leveling_menu.position4 = LEVELING_POINT4_TEXT_IT;
          leveling_menu.position5 = LEVELING_POINT5_TEXT_IT;
          //
          set_menu.title        = TITLE_SET_IT;
          set_menu.filesys      = FILESYS_TEXT_IT;
          set_menu.wifi         = WIFI_TEXT_IT;
          set_menu.about        = ABOUT_TEXT_IT;
          set_menu.fan          = FAN_TEXT_IT;
          set_menu.filament     = FILAMENT_TEXT_IT;
          set_menu.breakpoint   = BREAK_POINT_TEXT_IT;
          set_menu.motoroff     = MOTOR_OFF_TEXT_IT;
          set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_IT;
          set_menu.language     = LANGUAGE_TEXT_IT;
          set_menu.shutdown     = SHUTDOWN_TEXT_IT;
          set_menu.machine_para = MACHINE_PARA_IT;
          set_menu.eepromSet    = EEPROM_SETTINGS_IT;
          more_menu.title       = TITLE_MORE_IT;
          more_menu.gcode       = MORE_GCODE_IT;
          more_menu.entergcode  = MORE_ENTER_GCODE_IT;
          #if HAS_USER_ITEM(1)
            more_menu.custom1 = MORE_CUSTOM1_TEXT_IT;
          #endif
          #if HAS_USER_ITEM(2)
            more_menu.custom2 = MORE_CUSTOM2_TEXT_IT;
          #endif
          #if HAS_USER_ITEM(3)
            more_menu.custom3 = MORE_CUSTOM3_TEXT_IT;
          #endif
          #if HAS_USER_ITEM(4)
            more_menu.custom4 = MORE_CUSTOM4_TEXT_IT;
          #endif
          #if HAS_USER_ITEM(5)
            more_menu.custom5 = MORE_CUSTOM5_TEXT_IT;
          #endif
          #if HAS_USER_ITEM(6)
            more_menu.custom6 = MORE_CUSTOM6_TEXT_IT;
          #endif
          //
          filesys_menu.title   = TITLE_FILESYS_IT;
          filesys_menu.sd_sys  = SD_CARD_TEXT_IT;
          filesys_menu.usb_sys = U_DISK_TEXT_IT;

          // WIFI
          wifi_menu.title     = WIFI_NAME_TEXT_IT;
          wifi_menu.cloud     = CLOSE_TEXT_IT;
          wifi_menu.reconnect = WIFI_RECONNECT_TEXT_IT;

          cloud_menu.title        = TITLE_CLOUD_TEXT_IT;
          cloud_menu.bind         = CLOUD_BINDED_IT;
          cloud_menu.binded       = CLOUD_BINDED_IT;
          cloud_menu.unbind       = CLOUD_UNBIND_IT;
          cloud_menu.unbinding    = CLOUD_UNBINDED_IT;
          cloud_menu.disconnected = CLOUD_DISCONNECTED_IT;
          cloud_menu.unbinded     = CLOUD_UNBINDED_IT;
          cloud_menu.disable      = CLOUD_DISABLE_IT;
          //
          about_menu.title   = ABOUT_TEXT_IT;
          about_menu.type    = ABOUT_TYPE_TEXT_IT;
          about_menu.version = ABOUT_VERSION_TEXT_IT;
          about_menu.wifi    = ABOUT_WIFI_TEXT_IT;
          //
          fan_menu.title = FAN_TEXT_IT;
          fan_menu.add   = FAN_ADD_TEXT_IT;
          fan_menu.dec   = FAN_DEC_TEXT_IT;
          fan_menu.state = FAN_TIPS1_TEXT_IT;
          //
          filament_menu.title                               = TITLE_FILAMENT_IT;
          filament_menu.in                                  = FILAMENT_IN_TEXT_IT;
          filament_menu.out                                 = FILAMENT_OUT_TEXT_IT;
          filament_menu.ext1                                = FILAMENT_EXT0_TEXT_IT;
          filament_menu.ext2                                = FILAMENT_EXT1_TEXT_IT;
          filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_IT;
          filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_IT;
          filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_IT;
          filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_IT;
          filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_IT;
          filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_IT;
          filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_IT;
          filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_IT;
          filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_IT;

          //
          language_menu.title = LANGUAGE_TEXT_IT;

          //
          printing_menu.title  = TITLE_PRINTING_IT;
          printing_menu.option = PRINTING_OPERATION_IT;
          printing_menu.stop   = PRINTING_STOP_IT;
          printing_menu.pause  = PRINTING_PAUSE_IT;
          printing_menu.resume = PRINTING_RESUME_IT;

          //
          operation_menu.title      = TITLE_OPERATION_IT;
          operation_menu.pause      = PRINTING_PAUSE_IT;
          operation_menu.stop       = PRINTING_STOP_IT;
          operation_menu.temp       = PRINTING_TEMP_IT;
          operation_menu.fan        = FAN_TEXT_IT;
          operation_menu.extr       = PRINTING_EXTRUDER_IT;
          operation_menu.speed      = PRINTING_CHANGESPEED_IT;
          operation_menu.filament   = FILAMENT_TEXT_IT;
          operation_menu.more       = PRINTING_MORE_IT;
          operation_menu.move       = PRINTING_MOVE_IT;
          operation_menu.auto_off   = AUTO_SHUTDOWN_IT;
          operation_menu.manual_off = MANUAL_SHUTDOWN_IT;
          //
          pause_menu.title    = TITLE_PAUSE_IT;
          pause_menu.resume   = PRINTING_RESUME_IT;
          pause_menu.stop     = PRINTING_STOP_IT;
          pause_menu.extrude  = PRINTING_EXTRUDER_IT;
          pause_menu.move     = PRINTING_MOVE_IT;
          pause_menu.filament = FILAMENT_TEXT_IT;
          pause_menu.more     = PRINTING_MORE_IT;

          //
          speed_menu.title         = PRINTING_CHANGESPEED_IT;
          speed_menu.add           = ADD_TEXT_IT;
          speed_menu.dec           = DEC_TEXT_IT;
          speed_menu.move          = MOVE_SPEED_IT;
          speed_menu.extrude       = EXTRUDER_SPEED_IT;
          speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_IT;
          speed_menu.move_speed    = MOVE_SPEED_STATE_IT;
          //
          printing_more_menu.fan        = FAN_TEXT_IT;
          printing_more_menu.auto_close = AUTO_SHUTDOWN_IT;
          printing_more_menu.manual     = MANUAL_SHUTDOWN_IT;
          printing_more_menu.temp       = PRINTING_TEMP_IT;
          printing_more_menu.speed      = PRINTING_CHANGESPEED_IT;

          print_file_dialog_menu.confirm               = DIALOG_CONFIRM_IT;
          print_file_dialog_menu.cancel                = DIALOG_CANCLE_IT;
          print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_IT;
          print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_IT;
          print_file_dialog_menu.retry                 = DIALOG_RETRY_IT;
          print_file_dialog_menu.stop                  = DIALOG_STOP_IT;
          print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_IT;
          print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_IT;
          print_file_dialog_menu.close_machine_error   = DIALOG_ERROR_TIPS2_IT;
          print_file_dialog_menu.filament_no_press     = DIALOG_FILAMENT_NO_PRESS_IT;
          print_file_dialog_menu.print_finish          = DIALOG_PRINT_FINISH_IT;
          print_file_dialog_menu.print_time            = DIALOG_PRINT_TIME_IT;
          print_file_dialog_menu.reprint               = DIALOG_REPRINT_IT;
          print_file_dialog_menu.wifi_enable_tips      = DIALOG_WIFI_ENABLE_TIPS_IT;

          pause_msg_menu.pausing       = MESSAGE_PAUSING_IT;
          pause_msg_menu.changing      = MESSAGE_CHANGING_IT;
          pause_msg_menu.unload        = MESSAGE_UNLOAD_IT;
          pause_msg_menu.waiting       = MESSAGE_WAITING_IT;
          pause_msg_menu.insert        = MESSAGE_INSERT_IT;
          pause_msg_menu.load          = MESSAGE_LOAD_IT;
          pause_msg_menu.purge         = MESSAGE_PURGE_IT;
          pause_msg_menu.resume        = MESSAGE_RESUME_IT;
          pause_msg_menu.heat          = MESSAGE_HEAT_IT;
          pause_msg_menu.heating       = MESSAGE_HEATING_IT;
          pause_msg_menu.option        = MESSAGE_OPTION_IT;
          pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_IT;
          pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_IT;
          eeprom_menu.title            = EEPROM_SETTINGS_TITLE_IT;
          eeprom_menu.store            = EEPROM_SETTINGS_STORE_IT;
          eeprom_menu.read             = EEPROM_SETTINGS_READ_IT;
          eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_IT;
          eeprom_menu.storeTips        = EEPROM_STORE_TIPS_IT;
          eeprom_menu.readTips         = EEPROM_READ_TIPS_IT;
          eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_IT;
          break;

      #endif // if 1

    default:
      common_menu.dialog_confirm_title  = TITLE_DIALOG_CONFIRM_EN;
      common_menu.text_back             = BACK_TEXT_EN;
      common_menu.close_machine_tips    = DIALOG_CLOSE_MACHINE_EN;
      common_menu.unbind_printer_tips   = DIALOG_UNBIND_PRINTER_EN;
      common_menu.print_special_title   = PRINTING_OTHER_LANGUGE;
      common_menu.pause_special_title   = PRINTING_PAUSE_OTHER_LANGUGE;
      common_menu.operate_special_title = PRINTING_OPERATION_OTHER_LANGUGE;
      //
      main_menu.title        = TITLE_READYPRINT_EN;
      main_menu.preheat      = PREHEAT_TEXT_EN;
      main_menu.move         = MOVE_TEXT_EN;
      main_menu.home         = HOME_TEXT_EN;
      main_menu.print        = PRINT_TEXT_EN;
      main_menu.extrude      = EXTRUDE_TEXT_EN;
      main_menu.leveling     = LEVELING_TEXT_EN;
      main_menu.autoleveling = AUTO_LEVELING_TEXT_EN;
      main_menu.fan          = FAN_TEXT_EN;
      main_menu.set          = SET_TEXT_EN;
      main_menu.more         = MORE_TEXT_EN;
      main_menu.tool         = TOOL_TEXT_EN;
      // TOOL
      tool_menu.title        = TOOL_TEXT_EN;
      tool_menu.preheat      = TOOL_PREHEAT_EN;
      tool_menu.extrude      = TOOL_EXTRUDE_EN;
      tool_menu.move         = TOOL_MOVE_EN;
      tool_menu.home         = TOOL_HOME_EN;
      tool_menu.leveling     = TOOL_LEVELING_EN;
      tool_menu.autoleveling = TOOL_AUTO_LEVELING_EN;
      tool_menu.filament     = TOOL_FILAMENT_EN;
      tool_menu.more         = TOOL_MORE_EN;
      //
      preheat_menu.adjust_title = TITLE_ADJUST_EN;
      preheat_menu.title        = TITLE_PREHEAT_EN;
      preheat_menu.add          = ADD_TEXT_EN;
      preheat_menu.dec          = DEC_TEXT_EN;
      preheat_menu.ext1         = EXTRUDER_1_TEXT_EN;
      preheat_menu.ext2         = EXTRUDER_2_TEXT_EN;
      preheat_menu.hotbed       = HEATBED_TEXT_EN;
      preheat_menu.off          = CLOSE_TEXT_EN;
      //
      move_menu.title = TITLE_MOVE_EN;
      //
      home_menu.title    = TITLE_HOME_EN;
      home_menu.stopmove = HOME_STOPMOVE_EN;
      //
      file_menu.title             = TITLE_CHOOSEFILE_EN;
      file_menu.page_up           = PAGE_UP_TEXT_EN;
      file_menu.page_down         = PAGE_DOWN_TEXT_EN;
      file_menu.file_loading      = FILE_LOADING_EN;
      file_menu.no_file           = NO_FILE_EN;
      file_menu.no_file_and_check = NO_FILE_EN;
      //
      extrude_menu.title       = TITLE_EXTRUDE_EN;
      extrude_menu.in          = EXTRUDER_IN_TEXT_EN;
      extrude_menu.out         = EXTRUDER_OUT_TEXT_EN;
      extrude_menu.ext1        = EXTRUDER_1_TEXT_EN;
      extrude_menu.ext2        = EXTRUDER_2_TEXT_EN;
      extrude_menu.low         = EXTRUDE_LOW_SPEED_TEXT_EN;
      extrude_menu.normal      = EXTRUDE_MEDIUM_SPEED_TEXT_EN;
      extrude_menu.high        = EXTRUDE_HIGH_SPEED_TEXT_EN;
      extrude_menu.temper_text = EXTRUDER_TEMP_TEXT_EN;
      //
      leveling_menu.title     = TITLE_LEVELING_EN;
      leveling_menu.position1 = LEVELING_POINT1_TEXT_EN;
      leveling_menu.position2 = LEVELING_POINT2_TEXT_EN;
      leveling_menu.position3 = LEVELING_POINT3_TEXT_EN;
      leveling_menu.position4 = LEVELING_POINT4_TEXT_EN;
      leveling_menu.position5 = LEVELING_POINT5_TEXT_EN;
      //
      set_menu.title        = TITLE_SET_EN;
      set_menu.filesys      = FILESYS_TEXT_EN;
      set_menu.wifi         = WIFI_TEXT_EN;
      set_menu.about        = ABOUT_TEXT_EN;
      set_menu.fan          = FAN_TEXT_EN;
      set_menu.filament     = FILAMENT_TEXT_EN;
      set_menu.breakpoint   = BREAK_POINT_TEXT_EN;
      set_menu.motoroff     = MOTOR_OFF_TEXT_EN;
      set_menu.motoroffXY   = MOTOR_OFF_XY_TEXT_EN;
      set_menu.language     = LANGUAGE_TEXT_EN;
      set_menu.shutdown     = SHUTDOWN_TEXT_EN;
      set_menu.machine_para = MACHINE_PARA_EN;
      set_menu.eepromSet    = EEPROM_SETTINGS_EN;
      //
      more_menu.title       = TITLE_MORE_EN;
      more_menu.gcode       = MORE_GCODE_EN;
      more_menu.entergcode  = MORE_ENTER_GCODE_EN;
      #if HAS_USER_ITEM(1)
        more_menu.custom1 = MORE_CUSTOM1_TEXT_EN;
      #endif
      #if HAS_USER_ITEM(2)
        more_menu.custom2 = MORE_CUSTOM2_TEXT_EN;
      #endif
      #if HAS_USER_ITEM(3)
        more_menu.custom3 = MORE_CUSTOM3_TEXT_EN;
      #endif
      #if HAS_USER_ITEM(4)
        more_menu.custom4 = MORE_CUSTOM4_TEXT_EN;
      #endif
      #if HAS_USER_ITEM(5)
        more_menu.custom5 = MORE_CUSTOM5_TEXT_EN;
      #endif
      #if HAS_USER_ITEM(6)
        more_menu.custom6 = MORE_CUSTOM6_TEXT_EN;
      #endif
      //
      filesys_menu.title   = TITLE_FILESYS_EN;
      filesys_menu.sd_sys  = SD_CARD_TEXT_EN;
      filesys_menu.usb_sys = U_DISK_TEXT_EN;
      // WIFI
      wifi_menu.title     = WIFI_TEXT;
      wifi_menu.cloud     = CLOUD_TEXT_EN;
      wifi_menu.reconnect = WIFI_RECONNECT_TEXT_EN;

      cloud_menu.title        = TITLE_CLOUD_TEXT_EN;
      cloud_menu.bind         = CLOUD_BINDED_EN;
      cloud_menu.binded       = CLOUD_BINDED_EN;
      cloud_menu.unbind       = CLOUD_UNBIND_EN;
      cloud_menu.unbinding    = CLOUD_UNBINDED_EN;
      cloud_menu.disconnected = CLOUD_DISCONNECTED_EN;
      cloud_menu.unbinded     = CLOUD_UNBINDED_EN;
      cloud_menu.disable      = CLOUD_DISABLE_EN;
      //
      about_menu.title   = TITLE_ABOUT_EN;
      about_menu.type    = ABOUT_TYPE_TEXT_EN;
      about_menu.version = ABOUT_VERSION_TEXT_EN;
      about_menu.wifi    = ABOUT_WIFI_TEXT_EN;
      //
      fan_menu.title = TITLE_FAN_EN;
      fan_menu.add   = FAN_ADD_TEXT_EN;
      fan_menu.dec   = FAN_DEC_TEXT_EN;
      fan_menu.state = FAN_TIPS1_TEXT_EN;
      //
      filament_menu.title                               = TITLE_FILAMENT_EN;
      filament_menu.in                                  = FILAMENT_IN_TEXT_EN;
      filament_menu.out                                 = FILAMENT_OUT_TEXT_EN;
      filament_menu.ext1                                = FILAMENT_EXT0_TEXT_EN;
      filament_menu.ext2                                = FILAMENT_EXT1_TEXT_EN;
      filament_menu.ready_replace                       = FILAMENT_CHANGE_TEXT_EN;
      filament_menu.filament_dialog_load_heat           = FILAMENT_DIALOG_LOAD_HEAT_TIPS_EN;
      filament_menu.filament_dialog_load_heat_confirm   = FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_EN;
      filament_menu.filament_dialog_loading             = FILAMENT_DIALOG_LOADING_TIPS_EN;
      filament_menu.filament_dialog_load_completed      = FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_EN;
      filament_menu.filament_dialog_unload_heat         = FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_EN;
      filament_menu.filament_dialog_unload_heat_confirm = FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_EN;
      filament_menu.filament_dialog_unloading           = FILAMENT_DIALOG_UNLOADING_TIPS_EN;
      filament_menu.filament_dialog_unload_completed    = FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_EN;

      //
      language_menu.title = TITLE_LANGUAGE_EN;
      language_menu.next  = PAGE_DOWN_TEXT_EN;
      language_menu.up    = PAGE_UP_TEXT_EN;
      //
      printing_menu.title  = TITLE_PRINTING_EN;
      printing_menu.option = PRINTING_OPERATION_EN;
      printing_menu.stop   = PRINTING_STOP_EN;
      printing_menu.pause  = PRINTING_PAUSE_EN;
      printing_menu.resume = PRINTING_RESUME_EN;

      //
      operation_menu.title      = TITLE_OPERATION_EN;
      operation_menu.pause      = PRINTING_PAUSE_EN;
      operation_menu.stop       = PRINTING_STOP_EN;
      operation_menu.temp       = PRINTING_TEMP_EN;
      operation_menu.fan        = FAN_TEXT_EN;
      operation_menu.extr       = PRINTING_EXTRUDER_EN;
      operation_menu.speed      = PRINTING_CHANGESPEED_EN;
      operation_menu.filament   = FILAMENT_TEXT_EN;
      operation_menu.more       = PRINTING_MORE_EN;
      operation_menu.move       = PRINTING_MOVE_EN;
      operation_menu.auto_off   = AUTO_SHUTDOWN_EN;
      operation_menu.manual_off = MANUAL_SHUTDOWN_EN;
      //
      pause_menu.title    = TITLE_PAUSE_EN;
      pause_menu.resume   = PRINTING_RESUME_EN;
      pause_menu.stop     = PRINTING_STOP_EN;
      pause_menu.extrude  = PRINTING_EXTRUDER_EN;
      pause_menu.move     = PRINTING_MOVE_EN;
      pause_menu.filament = FILAMENT_TEXT_EN;
      pause_menu.more     = PRINTING_MORE_EN;

      //
      speed_menu.title         = TITLE_CHANGESPEED_EN;
      speed_menu.add           = ADD_TEXT_EN;
      speed_menu.dec           = DEC_TEXT_EN;
      speed_menu.move          = MOVE_SPEED_EN;
      speed_menu.extrude       = EXTRUDER_SPEED_EN;
      speed_menu.extrude_speed = EXTRUDER_SPEED_STATE_EN;
      speed_menu.move_speed    = MOVE_SPEED_STATE_EN;
      //
      printing_more_menu.title      = TITLE_MORE_EN;
      printing_more_menu.fan        = FAN_TEXT_EN;
      printing_more_menu.auto_close = AUTO_SHUTDOWN_EN;
      printing_more_menu.manual     = MANUAL_SHUTDOWN_EN;
      printing_more_menu.speed      = PRINTING_CHANGESPEED_EN;
      printing_more_menu.temp       = PRINTING_TEMP_EN;

      print_file_dialog_menu.confirm               = DIALOG_CONFIRM_EN;
      print_file_dialog_menu.cancel                = DIALOG_CANCLE_EN;
      print_file_dialog_menu.print_file            = DIALOG_PRINT_MODEL_EN;
      print_file_dialog_menu.cancel_print          = DIALOG_CANCEL_PRINT_EN;
      print_file_dialog_menu.retry                 = DIALOG_RETRY_EN;
      print_file_dialog_menu.stop                  = DIALOG_STOP_EN;
      print_file_dialog_menu.no_file_print_tips    = DIALOG_ERROR_TIPS1_EN;
      print_file_dialog_menu.print_from_breakpoint = DIALOG_REPRINT_FROM_BREAKPOINT_EN;
      print_file_dialog_menu.close_machine_error   = DIALOG_ERROR_TIPS2_EN;
      print_file_dialog_menu.filament_no_press     = DIALOG_FILAMENT_NO_PRESS_EN;
      print_file_dialog_menu.print_finish          = DIALOG_PRINT_FINISH_EN;
      print_file_dialog_menu.print_time            = DIALOG_PRINT_TIME_EN;
      print_file_dialog_menu.reprint               = DIALOG_REPRINT_EN;
      print_file_dialog_menu.wifi_enable_tips      = DIALOG_WIFI_ENABLE_TIPS_EN;

      pause_msg_menu.pausing       = MESSAGE_PAUSING_EN;
      pause_msg_menu.changing      = MESSAGE_CHANGING_EN;
      pause_msg_menu.unload        = MESSAGE_UNLOAD_EN;
      pause_msg_menu.waiting       = MESSAGE_WAITING_EN;
      pause_msg_menu.insert        = MESSAGE_INSERT_EN;
      pause_msg_menu.load          = MESSAGE_LOAD_EN;
      pause_msg_menu.purge         = MESSAGE_PURGE_EN;
      pause_msg_menu.resume        = MESSAGE_RESUME_EN;
      pause_msg_menu.heat          = MESSAGE_HEAT_EN;
      pause_msg_menu.heating       = MESSAGE_HEATING_EN;
      pause_msg_menu.option        = MESSAGE_OPTION_EN;
      pause_msg_menu.purgeMore     = MESSAGE_PURGE_MORE_EN;
      pause_msg_menu.continuePrint = MESSAGE_CONTINUE_PRINT_EN;
      eeprom_menu.title            = EEPROM_SETTINGS_TITLE_EN;
      eeprom_menu.store            = EEPROM_SETTINGS_STORE_EN;
      eeprom_menu.read             = EEPROM_SETTINGS_READ_EN;
      eeprom_menu.revert           = EEPROM_SETTINGS_REVERT_EN;
      eeprom_menu.storeTips        = EEPROM_STORE_TIPS_EN;
      eeprom_menu.readTips         = EEPROM_READ_TIPS_EN;
      eeprom_menu.revertTips       = EEPROM_REVERT_TIPS_EN;
      break;
  }
}

#endif // HAS_TFT_LVGL_UI
