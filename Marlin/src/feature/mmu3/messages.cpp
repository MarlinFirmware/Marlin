/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * messages.cpp
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_PRUSA_MMU3

#include "messages.h"
#include "mmu_hw/errors_list.h"
#include "mmu2_supported_version.h"

#include "../../core/language.h"
#include "../../lcd/marlinui.h" // For the custom characters

namespace MMU2 {

FSTR_P MSG_TITLE_FINDA_DIDNT_TRIGGER     = F("FINDA DIDNT TRIGGER");
FSTR_P MSG_TITLE_FINDA_FILAMENT_STUCK    = F("FINDA FILAM. STUCK");
FSTR_P MSG_TITLE_FSENSOR_DIDNT_TRIGGER   = F("FSENSOR DIDNT TRIGG.");
FSTR_P MSG_TITLE_FSENSOR_FILAMENT_STUCK  = F("FSENSOR FIL. STUCK");
FSTR_P MSG_TITLE_PULLEY_CANNOT_MOVE      = F("PULLEY CANNOT MOVE");
FSTR_P MSG_TITLE_FSENSOR_TOO_EARLY       = F("FSENSOR TOO EARLY");
FSTR_P MSG_TITLE_INSPECT_FINDA           = F("INSPECT FINDA");
FSTR_P MSG_TITLE_LOAD_TO_EXTRUDER_FAILED = F("LOAD TO EXTR. FAILED");
FSTR_P MSG_TITLE_SELECTOR_CANNOT_MOVE    = F("SELECTOR CANNOT MOVE");
FSTR_P MSG_TITLE_SELECTOR_CANNOT_HOME    = F("SELECTOR CANNOT HOME");
FSTR_P MSG_TITLE_IDLER_CANNOT_MOVE       = F("IDLER CANNOT MOVE");
FSTR_P MSG_TITLE_IDLER_CANNOT_HOME       = F("IDLER CANNOT HOME");
FSTR_P MSG_TITLE_TMC_WARNING_TMC_TOO_HOT = F("WARNING TMC TOO HOT");
FSTR_P MSG_TITLE_TMC_OVERHEAT_ERROR      = F("TMC OVERHEAT ERROR");
FSTR_P MSG_TITLE_TMC_DRIVER_ERROR        = F("TMC DRIVER ERROR");
FSTR_P MSG_TITLE_TMC_DRIVER_RESET        = F("TMC DRIVER RESET");
FSTR_P MSG_TITLE_TMC_UNDERVOLTAGE_ERROR  = F("TMC UNDERVOLTAGE ERR");
FSTR_P MSG_TITLE_TMC_DRIVER_SHORTED      = F("TMC DRIVER SHORTED");
FSTR_P MSG_TITLE_SELFTEST_FAILED         = F("MMU SELFTEST FAILED");
FSTR_P MSG_TITLE_MMU_MCU_ERROR           = F("MMU MCU ERROR");
FSTR_P MSG_TITLE_MMU_NOT_RESPONDING      = F("MMU NOT RESPONDING");
FSTR_P MSG_TITLE_COMMUNICATION_ERROR     = F("COMMUNICATION ERROR");
FSTR_P MSG_TITLE_FILAMENT_ALREADY_LOADED = F("FIL. ALREADY LOADED");
FSTR_P MSG_TITLE_INVALID_TOOL            = F("INVALID TOOL");
FSTR_P MSG_TITLE_QUEUE_FULL              = F("QUEUE FULL");
FSTR_P MSG_TITLE_FW_UPDATE_NEEDED        = F("MMU FW UPDATE NEEDED");
FSTR_P MSG_TITLE_FW_RUNTIME_ERROR        = F("FW RUNTIME ERROR");
FSTR_P MSG_TITLE_UNLOAD_MANUALLY         = F("UNLOAD MANUALLY");
FSTR_P MSG_TITLE_FILAMENT_EJECTED        = F("FILAMENT EJECTED");
FSTR_P MSG_TITLE_FILAMENT_CHANGE         = F("FILAMENT CHANGE");
FSTR_P MSG_TITLE_UNKNOWN_ERROR           = F("UNKNOWN ERROR");

FSTR_P MSG_DESC_FINDA_DIDNT_TRIGGER     = F("FINDA didn't trigger while loading the filament. Ensure the filament can move and FINDA works.");
FSTR_P MSG_DESC_FINDA_FILAMENT_STUCK    = F("FINDA didn't switch off while unloading filament. Try unloading manually. Ensure filament can move and FINDA works.");
FSTR_P MSG_DESC_FSENSOR_DIDNT_TRIGGER   = F("Filament sensor didn't trigger while loading the filament. Ensure the sensor is calibrated and the filament reached it.");
FSTR_P MSG_DESC_FSENSOR_FILAMENT_STUCK  = F("Filament sensor didn't switch off while unloading filament. Ensure filament can move and the sensor works.");
FSTR_P MSG_DESC_PULLEY_CANNOT_MOVE      = F("Pulley motor stalled. Ensure the pulley can move and check the wiring.");
FSTR_P MSG_DESC_FSENSOR_TOO_EARLY       = F("Filament sensor triggered too early while loading to extruder. Check there isn't anything stuck in PTFE tube. Check that sensor reads properly.");
FSTR_P MSG_DESC_INSPECT_FINDA           = F("Selector can't move due to FINDA detecting a filament. Make sure no filament is in Selector and FINDA works properly.");
FSTR_P MSG_DESC_LOAD_TO_EXTRUDER_FAILED = F("Loading to extruder failed. Inspect the filament tip shape. Refine the sensor calibration, if needed.");
FSTR_P MSG_DESC_SELECTOR_CANNOT_HOME    = F("The Selector cannot home properly. Check for anything blocking its movement.");
FSTR_P MSG_DESC_CANNOT_MOVE             = F("Can't move Selector or Idler.");
FSTR_P MSG_DESC_IDLER_CANNOT_HOME       = F("The Idler cannot home properly. Check for anything blocking its movement.");
FSTR_P MSG_DESC_TMC                     = F("More details online.");
FSTR_P MSG_DESC_MMU_NOT_RESPONDING      = F("MMU not responding. Check the wiring and connectors.");
FSTR_P MSG_DESC_COMMUNICATION_ERROR     = F("MMU not responding correctly. Check the wiring and connectors.");
FSTR_P MSG_DESC_FILAMENT_ALREADY_LOADED = F("Cannot perform the action, filament is already loaded. Unload it first.");
FSTR_P MSG_DESC_INVALID_TOOL            = F("Requested filament tool is not available on this hardware. Check the G-code for tool index out of range (T0-T4).");
FSTR_P MSG_DESC_QUEUE_FULL              = F("MMU Firmware internal error, please reset the MMU.");
FSTR_P MSG_DESC_FW_RUNTIME_ERROR        = F("Internal runtime error. Try resetting the MMU or updating the firmware.");
FSTR_P MSG_DESC_UNLOAD_MANUALLY         = F("Filament detected unexpectedly. Ensure no filament is loaded. Check the sensors and wiring.");
FSTR_P MSG_DESC_FILAMENT_EJECTED        = F("Remove the ejected filament from the front of the MMU.");
FSTR_P MSG_DESC_FILAMENT_CHANGE         = F("M600 Filament Change. Load a new filament or eject the old one.");
FSTR_P MSG_DESC_UNKNOWN_ERROR           = F("Unexpected error occurred.");

FSTR_P MSG_DESC_FW_UPDATE_NEEDED        = F("MMU FW version is not supported. Update to version " STRINGIFY(mmuVersionMajor) "." STRINGIFY(mmuVersionMinor) "." STRINGIFY(mmuVersionPatch) ".");

FSTR_P MSG_BTN_RETRY       = F("Retry");
FSTR_P MSG_BTN_RESET_MMU   = F("ResetMMU");
FSTR_P MSG_BTN_UNLOAD      = F("Unload");
FSTR_P MSG_BTN_LOAD        = F("Load");
FSTR_P MSG_BTN_EJECT       = F("Eject");
FSTR_P MSG_BTN_STOP        = F("Stop");
FSTR_P MSG_BTN_DISABLE_MMU = F("Disable");
FSTR_P MSG_BTN_MORE        = F("More Info");

} // namespace MMU2

FSTR_P MSG_ALWAYS                                   = F("Always");
FSTR_P MSG_AUTO_HOME                                = F("Auto home");
FSTR_P MSG_BABYSTEP_Z                               = F("Live adjust Z");
FSTR_P MSG_BABYSTEP_Z_NOT_SET                       = F("Distance between tip of the nozzle and the bed surface has not been set yet. Please follow the manual, chapter First steps, section First layer calibration.");
FSTR_P MSG_BED                                      = F("Bed");
FSTR_P MSG_BED_DONE                                 = F("Bed done");
FSTR_P MSG_BED_HEATING                              = F("Bed Heating");
FSTR_P MSG_BED_LEVELING_FAILED_POINT_LOW            = F("Bed leveling failed. Sensor didn't trigger. Debris on nozzle? Waiting for reset.");
FSTR_P MSG_BED_SKEW_OFFSET_DETECTION_FITTING_FAILED = F("XYZ calibration failed. Please consult the manual.");
FSTR_P MSG_BELT_STATUS                              = F("Belt status");
FSTR_P MSG_CANCEL                                   = F(">Cancel");
FSTR_P MSG_CALIBRATE_Z_AUTO                         = F("Calibrating Z");
FSTR_P MSG_CARD_MENU                                = F("Print from SD");
FSTR_P MSG_CHECKING_X                               = F("Checking X axis");
FSTR_P MSG_CHECKING_Y                               = F("Checking Y axis");
FSTR_P MSG_COMMUNITY_MADE                           = F("Community made");
FSTR_P MSG_CONFIRM_NOZZLE_CLEAN                     = F("Please clean the nozzle for calibration. Click when done.");
FSTR_P MSG_COOLDOWN                                 = F("Cooldown");
FSTR_P MSG_CRASH                                    = F("Crash");
FSTR_P MSG_CRASH_DETECTED                           = F("Crash detected.");
FSTR_P MSG_CRASHDETECT                              = F("Crash det.");
FSTR_P MSG_DONE                                     = F("Done");
FSTR_P MSG_ERROR                                    = F("ERROR:");
FSTR_P MSG_EXTRUDER                                 = F("Extruder");
FSTR_P MSG_FANS_CHECK                               = F("Fans check");
FSTR_P MSG_FIL_RUNOUTS                              = F("Fil. runouts");
FSTR_P MSG_FILAMENT                                 = F("Filament");
FSTR_P MSG_FAN_SPEED                                = F("Fan speed");
FSTR_P MSG_HOTEND_FAN_SPEED                         = F("Hotend fan:");
FSTR_P MSG_PRINT_FAN_SPEED                          = F("Print fan:");
FSTR_P MSG_FILAMENT_CLEAN                           = F("Filament extruding & with correct color?");
FSTR_P MSG_FILAMENT_LOADED                          = F("Is filament loaded?");
FSTR_P MSG_FILAMENTCHANGE                           = F("Change filament");
FSTR_P MSG_FIND_BED_OFFSET_AND_SKEW_LINE1           = F("Searching bed calibration point");
FSTR_P MSG_FINISHING_MOVEMENTS                      = F("Finishing movements");
FSTR_P MSG_FOLLOW_CALIBRATION_FLOW                  = F("Printer has not been calibrated yet. Please follow the manual, chapter First steps, section Calibration flow.");
FSTR_P MSG_FOLLOW_Z_CALIBRATION_FLOW                = F("There is still a need to make Z calibration. Please follow the manual, chapter First steps, section Calibration flow.");
FSTR_P MSG_FSENSOR_RUNOUT                           = F("F. runout");
FSTR_P MSG_FSENSOR_AUTOLOAD                         = F("F. autoload");
FSTR_P MSG_FSENSOR_JAM_DETECTION                    = F("F. jam detect");
FSTR_P MSG_FSENSOR                                  = F("Fil. sensor");
FSTR_P MSG_HEATING                                  = F("Heating");
FSTR_P MSG_HEATING_COMPLETE                         = F("Heating done.");
FSTR_P MSG_HOMEYZ                                   = F("Calibrate Z");
FSTR_P MSG_ITERATION                                = F("Iteration");
FSTR_P MSG_SELECT_FILAMENT                          = F("Select filament:");
FSTR_P MSG_LAST_PRINT                               = F("Last print");
FSTR_P MSG_LAST_PRINT_FAILURES                      = F("Last print failures");
FSTR_P MSG_PRELOAD_TO_MMU                           = F("Preload to MMU");
FSTR_P MSG_LOAD_FILAMENT                            = F("Load filament");
FSTR_P MSG_LOADING_TEST                             = F("Loading Test");
FSTR_P MSG_LOADING_FILAMENT                         = F("Loading filament");
FSTR_P MSG_TESTING_FILAMENT                         = F("Testing filament");
FSTR_P MSG_EJECT_FROM_MMU                           = F("Eject from MMU");
FSTR_P MSG_CUT_FILAMENT                             = F("Cut filament");
FSTR_P MSG_MAIN                                     = F("Main");
FSTR_P MSG_BACK                                     = F("Back");
FSTR_P MSG_SHEET                                    = F("Sheet");
FSTR_P MSG_STEEL_SHEETS                             = F("Steel sheets");
FSTR_P MSG_MEASURE_BED_REFERENCE_HEIGHT_LINE1       = F("Measuring reference height of calibration point");
FSTR_P MSG_CALIBRATION                              = F("Calibration");
FSTR_P MSG_MMU_FAILS                                = F("MMU fails");
FSTR_P MSG_MMU_LOAD_FAILS                           = F("MMU load fails");
FSTR_P MSG_MMU_POWER_FAILS                          = F("MMU power fails");
FSTR_P MSG_NO                                       = F("No");
FSTR_P MSG_NOZZLE                                   = F("Nozzle");
FSTR_P MSG_PAPER                                    = F("Place a sheet of paper under the nozzle during the calibration of first 4 points. If the nozzle catches the paper, power off the printer immediately.");
FSTR_P MSG_PAUSE_PRINT                              = F("Pause print");
FSTR_P MSG_PRINT_PAUSED                             = F("Print paused");
FSTR_P MSG_PLACE_STEEL_SHEET                        = F("Please place steel sheet on heatbed.");
FSTR_P MSG_PLEASE_WAIT                              = F("Please wait");
FSTR_P MSG_POWER_FAILURES                           = F("Power failures");
FSTR_P MSG_PREHEAT_NOZZLE                           = F("Preheat the nozzle!");
FSTR_P MSG_PRESS_TO_UNLOAD                          = F("Please press the knob to unload filament");
FSTR_P MSG_PRINT_ABORTED                            = F("Print aborted");
FSTR_P MSG_PULL_OUT_FILAMENT                        = F("Please pull out filament immediately");
FSTR_P MSG_RECOVER_PRINT                            = F("Blackout occurred. Recover print?");
FSTR_P MSG_REFRESH                                  = F(LCD_STR_REFRESH "Refresh");
FSTR_P MSG_REMOVE_STEEL_SHEET                       = F("Please remove steel sheet from heatbed.");
FSTR_P MSG_RESET                                    = F("Reset");
FSTR_P MSG_RESUME_PRINT                             = F("Resume print");
FSTR_P MSG_RESUMING_PRINT                           = F("Resuming print");
FSTR_P MSG_SELFTEST_PART_FAN                        = F("Front print fan?");
FSTR_P MSG_SELFTEST_HOTEND_FAN                      = F("Left hotend fan?");
FSTR_P MSG_SELFTEST_FAILED                          = F("Selftest failed");
FSTR_P MSG_SELFTEST_FAN                             = F("Fan test");
FSTR_P MSG_SELFTEST_FAN_NO                          = F("Not spinning");
FSTR_P MSG_SELFTEST_FAN_YES                         = F("Spinning");
FSTR_P MSG_SELFTEST_CHECK_BED                       = F("Checking bed");
FSTR_P MSG_SELFTEST_CHECK_FSENSOR                   = F("Checking sensors");
FSTR_P MSG_SELFTEST_MOTOR                           = F("Motor");
FSTR_P MSG_SELFTEST_FILAMENT_SENSOR                 = F("Filament sensor");
FSTR_P MSG_SELFTEST_WIRINGERROR                     = F("Wiring error");
FSTR_P MSG_SETTINGS                                 = F("Settings");
FSTR_P MSG_SET_READY                                = F("Set Ready");
FSTR_P MSG_SET_NOT_READY                            = F("Set not Ready");
FSTR_P MSG_SELECT_LANGUAGE                          = F("Select language");
FSTR_P MSG_SORTING_FILES                            = F("Sorting files");
FSTR_P MSG_TOTAL                                    = F("Total");
FSTR_P MSG_MATERIAL_CHANGES                         = F("Material changes");
FSTR_P MSG_TOTAL_FAILURES                           = F("Total failures");
FSTR_P MSG_HW_SETUP                                 = F("HW Setup");
FSTR_P MSG_TUNE                                     = F("Tune");
FSTR_P MSG_MODE                                     = F("Mode");
FSTR_P MSG_HIGH_POWER                               = F("High power");
FSTR_P MSG_AUTO_POWER                               = F("Auto power");
FSTR_P MSG_SILENT                                   = F("Silent");
FSTR_P MSG_NORMAL                                   = F("Normal");
FSTR_P MSG_STEALTH                                  = F("Stealth");
FSTR_P MSG_STEEL_SHEET_CHECK                        = F("Is steel sheet on heatbed?");
FSTR_P MSG_STOP_PRINT                               = F("Stop print");
FSTR_P MSG_STOPPED                                  = F("STOPPED.");
FSTR_P MSG_PINDA_CALIBRATION                        = F("PINDA cal.");
FSTR_P MSG_PINDA_CALIBRATION_DONE                   = F("PINDA calibration is finished and active. It can be disabled in menu Settings->PINDA cal.");
FSTR_P MSG_UNLOAD_FILAMENT                          = F("Unload filament");
FSTR_P MSG_UNLOADING_FILAMENT                       = F("Unloading filament");
FSTR_P MSG_INFO_SCREEN                              = F("Info screen");
FSTR_P MSG_WIZARD_CALIBRATION_FAILED                = F("Please check our handbook and fix the problem. Then resume the Wizard by rebooting the printer.");
FSTR_P MSG_WIZARD_DONE                              = F("All is done. Happy printing!");
FSTR_P MSG_WIZARD_HEATING                           = F("Preheating nozzle. Please wait.");
FSTR_P MSG_WIZARD_QUIT                              = F("You can always resume the Wizard from Calibration -> Wizard.");
FSTR_P MSG_WIZARD_WELCOME                           = F("Hi, I am your Original Prusa i3 printer. Would you like me to guide you through the setup process?");
FSTR_P MSG_WIZARD_WELCOME_SHIPPING                  = F("Hi, I am your Original Prusa i3 printer. I will guide you through a short setup process, in which the Z-axis will be calibrated. Then, you will be ready to print.");
FSTR_P MSG_YES                                      = F("Yes");
FSTR_P MSG_V2_CALIBRATION                           = F("First layer cal.");
FSTR_P MSG_OFF                                      = F("Off");
FSTR_P MSG_ON                                       = F("On");
FSTR_P MSG_NA                                       = F("N/A");
FSTR_P MSG_CUTTER                                   = F("Cutter");
FSTR_P MSG_NONE                                     = F("None");
FSTR_P MSG_WARN                                     = F("Warn");
FSTR_P MSG_STRICT                                   = F("Strict");
FSTR_P MSG_MODEL                                    = F("Model");
FSTR_P MSG_GCODE_DIFF_PRINTER_CONTINUE              = F("G-code sliced for a different printer type. Continue?");
FSTR_P MSG_GCODE_DIFF_PRINTER_CANCELLED             = F("G-code sliced for a different printer type. Please re-slice the model again. Print cancelled.");
FSTR_P MSG_GCODE_NEWER_FIRMWARE_CONTINUE            = F("G-code sliced for a newer firmware. Continue?");
FSTR_P MSG_GCODE_NEWER_FIRMWARE_CANCELLED           = F("G-code sliced for a newer firmware. Please update the firmware. Print cancelled.");
FSTR_P MSG_GCODE_DIFF_CONTINUE                      = F("G-code sliced for a different level. Continue?");
FSTR_P MSG_GCODE_DIFF_CANCELLED                     = F("G-code sliced for a different level. Please re-slice the model again. Print cancelled.");
FSTR_P MSG_NOZZLE_DIFFERS_CONTINUE                  = F("Nozzle diameter differs from the G-code. Continue?");
FSTR_P MSG_NOZZLE_DIFFERS_CANCELLED                 = F("Nozzle diameter differs from the G-code. Please check the value in settings. Print cancelled.");
FSTR_P MSG_NOZZLE_DIAMETER                          = F("Nozzle d.");
FSTR_P MSG_MMU_MODE                                 = F("MMU Mode");
FSTR_P MSG_SD_CARD                                  = F("SD card");
FSTR_P MSG_SORT                                     = F("Sort");
FSTR_P MSG_SORT_TIME                                = F("Time");
FSTR_P MSG_SORT_ALPHA                               = F("Alphabet");
FSTR_P MSG_RPI_PORT                                 = F("RPi port");
FSTR_P MSG_SOUND                                    = F("Sound");
FSTR_P MSG_SOUND_LOUD                               = F("Loud");
FSTR_P MSG_SOUND_ONCE                               = F("Once");
FSTR_P MSG_SOUND_BLIND                              = F("Assist");
FSTR_P MSG_MESH                                     = F("Mesh");
FSTR_P MSG_MESH_BED_LEVELING                        = F("Mesh Bed Leveling");
FSTR_P MSG_Z_PROBE_NR                               = F("Z-probe nr.");
FSTR_P MSG_MAGNETS_COMP                             = F("Magnets comp.");
FSTR_P MSG_FS_ACTION                                = F("FS Action");
FSTR_P MSG_CONTINUE_SHORT                           = F("Cont.");
FSTR_P MSG_PAUSE                                    = F("Pause");
FSTR_P MSG_BRIGHTNESS                               = F("Brightness");
FSTR_P MSG_BL_HIGH                                  = F("Level Bright");
FSTR_P MSG_BL_LOW                                   = F("Level Dimmed");
FSTR_P MSG_TIMEOUT                                  = F("Timeout");
FSTR_P MSG_BRIGHT                                   = F("Bright");
FSTR_P MSG_DIM                                      = F("Dim");
FSTR_P MSG_AUTO                                     = F("Auto");
#if (FILAMENT_SENSOR_TYPE == FSENSOR_IR_ANALOG)
  // Beware - The space at the beginning is necessary since it is reused in LCD menu items which are to be with a space
  FSTR_P MSG_IR_04_OR_NEWER                         = F(" 0.4 or newer");
  FSTR_P MSG_IR_03_OR_OLDER                         = F(" 0.3 or older");
  FSTR_P MSG_IR_UNKNOWN                             = F("unknown state");
#endif
FSTR_P MSG_PAUSED_THERMAL_ERROR                     = F("PAUSED THERMAL ERROR");
#ifdef THERMAL_MODEL
  FSTR_P MSG_THERMAL_ANOMALY                        = F("THERMAL ANOMALY");
  FSTR_P MSG_TM_NOT_CAL                             = F("Thermal model not calibrated yet.");
  FSTR_P MSG_TM_ACK_ERROR                           = F("Clear TM error");
#endif
FSTR_P MSG_LOAD_ALL                                 = F("Load All");
FSTR_P MSG_NOZZLE_CNG_MENU                          = F("Nozzle change");
FSTR_P MSG_NOZZLE_CNG_READ_HELP                     = F("For a Nozzle change please read\nprusa.io/nozzle-mk3s");
FSTR_P MSG_NOZZLE_CNG_CHANGED                       = F("Hotend at 280C! Nozzle changed and tightened to specs?");
FSTR_P MSG_REPRINT                                  = F("Reprint");

// Non-internationalized messages
#if 0
  FSTR_P MSG_FW_VERSION_BETA = F("You are using a BETA firmware version! It is in a development state! Use this version with CAUTION as it may DAMAGE the printer!");
#endif
FSTR_P MSG_SPOOL_JOIN                       = F("SpoolJoin");
FSTR_P MSG_FIRMWARE                         = F("Firmware");
FSTR_P MSG_TOSHIBA_FLASH_AIR_COMPATIBILITY  = F("FlashAir");
FSTR_P MSG_PINDA                            = F("PINDA");
FSTR_P MSG_SD_WORKDIR_FAIL                  = F("workDir open failed");
FSTR_P MSG_BROWNOUT_RESET                   = F(" Brown out Reset");
FSTR_P MSG_EXTERNAL_RESET                   = F(" External Reset");
FSTR_P MSG_FILE_SAVED                       = F("Done saving file.");
FSTR_P MSG_POSITION_UNKNOWN                 = F("Home X/Y before Z");
FSTR_P MSG_SOFTWARE_RESET                   = F(" Software Reset");
FSTR_P MSG_UNKNOWN_COMMAND                  = F("Unknown command: \"");
FSTR_P MSG_WATCHDOG_RESET                   = F(" Watchdog Reset");
FSTR_P MSG_Z_MAX                            = F("z_max: ");
FSTR_P MSG_Z_MIN                            = F("z_min: ");
FSTR_P MSG_ZPROBE_OUT                       = F("Z probe out. bed");
#if HAS_LEVELING
  FSTR_P MSG_ZPROBE_ZOFFSET                 = F("Z Offset");
#endif
FSTR_P MSG_TMC_OVERTEMP                     = F("TMC DRIVER OVERTEMP");
FSTR_P MSG_Enqueing                         = F("enqueing \"");
FSTR_P MSG_ENDSTOPS_HIT                     = F("endstops hit: ");
FSTR_P MSG_SD_ERR_WRITE_TO_FILE             = F("error writing to file");
FSTR_P MSG_OK                               = F("ok");
FSTR_P MSG_OK_CAPS                          = F("OK");

FSTR_P MSG_SD_OPEN_FILE_FAIL                = F("open failed, File: ");
FSTR_P MSG_ENDSTOP_OPEN                     = F("open");
FSTR_P MSG_POWERUP                          = F("PowerUp");
FSTR_P MSG_ERR_STOPPED                      = F("Printer stopped due to errors. Supervision required.");
FSTR_P MSG_ENDSTOP_HIT                      = F("TRIGGERED");

FSTR_P MSG_OCTOPRINT_ASK_PAUSE              = F("// action:pause");
FSTR_P MSG_OCTOPRINT_PAUSED                 = F("// action:paused");
FSTR_P MSG_OCTOPRINT_ASK_RESUME             = F("// action:resume");
FSTR_P MSG_OCTOPRINT_RESUMED                = F("// action:resumed");
FSTR_P MSG_OCTOPRINT_CANCEL                 = F("// action:cancel");
FSTR_P MSG_OCTOPRINT_READY                  = F("// action:ready");
FSTR_P MSG_OCTOPRINT_NOT_READY              = F("// action:not_ready");
FSTR_P MSG_OCTOPRINT_START                  = F("// action:start");
FSTR_P MSG_OCTOPRINT_UVLO_RECOVERY_READY    = F("// action:uvlo_recovery_ready");

FSTR_P MSG_FANCHECK_HOTEND                  = F("Err:HOTEND FAN ERROR");
FSTR_P MSG_FANCHECK_PRINT                   = F("Err:PRINT FAN ERROR");

FSTR_P MSG_M112_KILL                        = F("M112 called. Emergency Stop.");
FSTR_P MSG_ADVANCE_K                        = F("Advance K:");
FSTR_P MSG_POWERPANIC_DETECTED              = F("POWER PANIC DETECTED");
FSTR_P MSG_LCD_STATUS_CHANGED               = F("LCD status changed");
FSTR_P MSG_UNKNOWN_CODE                     = F("Unknown %c code: %s\n");

// Common G-codes
FSTR_P G1_E_F2700 = F("G1 E%-.3f F2700");
FSTR_P G28W       = F("G28 W");
FSTR_P MSG_M23    = F("M23 %s");
FSTR_P MSG_M24    = F("M24");
FSTR_P MSG_M83    = F("M83");
FSTR_P MSG_M84    = F("M84");
FSTR_P MSG_M107   = F("M107");
FSTR_P MSG_M220   = F("M220 S%d");
FSTR_P MSG_M500   = F("M500");
FSTR_P MSG_M600   = F("M600");
FSTR_P MSG_M701   = F("M701");
FSTR_P MSG_M702   = F("M702");

#endif // HAS_PRUSA_MMU3
