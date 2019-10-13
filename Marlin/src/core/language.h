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
#pragma once

#include "../inc/MarlinConfig.h"

#define _UxGT(a) a

// Fallback if no language is set. DON'T CHANGE
#ifndef LCD_LANGUAGE
  #define LCD_LANGUAGE en
#endif

// For character-based LCD controllers (DISPLAY_CHARSET_HD44780)
#define JAPANESE 1
#define WESTERN  2
#define CYRILLIC 3

// NOTE: IF YOU CHANGE LANGUAGE FILES OR MERGE A FILE WITH CHANGES
//
//   ==> ALWAYS TRY TO COMPILE MARLIN WITH/WITHOUT "ULTIPANEL" / "ULTRA_LCD" / "SDSUPPORT" #define IN "Configuration.h"
//   ==> ALSO TRY ALL AVAILABLE LANGUAGE OPTIONS
// See also http://marlinfw.org/docs/development/lcd_language.html

// Languages
// an         Aragonese
// bg         Bulgarian
// ca         Catalan
// cz         Czech
// da         Danish
// de         German
// el         Greek
// el_gr      Greek (Greece)
// en         English
// es         Spanish
// eu         Basque-Euskera
// fi         Finnish
// fr         French
// gl         Galician
// hr         Croatian
// it         Italian
// jp_kana    Japanese
// ko_KR      Korean (South Korea)
// nl         Dutch
// pl         Polish
// pt         Portuguese
// pt_br      Portuguese (Brazilian)
// ru         Russian
// sk         Slovak
// tr         Turkish
// uk         Ukrainian
// vi         Vietnamese
// zh_CN      Chinese (Simplified)
// zh_TW      Chinese (Traditional)

#ifdef DEFAULT_SOURCE_CODE_URL
  #undef  SOURCE_CODE_URL
  #define SOURCE_CODE_URL DEFAULT_SOURCE_CODE_URL
#endif

#ifdef CUSTOM_MACHINE_NAME
  #undef  MACHINE_NAME
  #define MACHINE_NAME CUSTOM_MACHINE_NAME
#else
  #ifdef DEFAULT_MACHINE_NAME
    #undef  MACHINE_NAME
    #define MACHINE_NAME DEFAULT_MACHINE_NAME
  #endif
#endif

#ifndef MACHINE_UUID
  #define MACHINE_UUID DEFAULT_MACHINE_UUID
#endif

#define MARLIN_WEBSITE_URL "http://marlinfw.org"

//#if !defined(STRING_SPLASH_LINE3) && defined(WEBSITE_URL)
//  #define STRING_SPLASH_LINE3 WEBSITE_URL
//#endif

#if HAS_CHARACTER_LCD

  // Custom characters defined in the first 8 characters of the LCD
  #define LCD_STR_BEDTEMP     "\x00" // Print only as a char. This will have 'unexpected' results when used in a string!
  #define LCD_STR_DEGREE      "\x01"
  #define LCD_STR_THERMOMETER "\x02" // Still used with string concatenation
  #define LCD_STR_UPLEVEL     "\x03"
  #define LCD_STR_REFRESH     "\x04"
  #define LCD_STR_FOLDER      "\x05"
  #define LCD_STR_FEEDRATE    "\x06"
  #define LCD_STR_CLOCK       "\x07"
  #define LCD_STR_ARROW_RIGHT ">"  /* from the default character set */

#else
  //
  // Custom characters from Marlin_symbols.fon which was merged into ISO10646-0-3.bdf
  // \x00 intentionally skipped to avoid problems in strings
  //
  #define LCD_STR_REFRESH     "\x01"
  #define LCD_STR_FOLDER      "\x02"
  #define LCD_STR_ARROW_RIGHT "\x03"
  #define LCD_STR_UPLEVEL     "\x04"
  #define LCD_STR_CLOCK       "\x05"
  #define LCD_STR_FEEDRATE    "\x06"
  #define LCD_STR_BEDTEMP     "\x07"
  #define LCD_STR_THERMOMETER "\x08"
  #define LCD_STR_DEGREE      "\x09"

  #define LCD_STR_SPECIAL_MAX '\x09'
  // Maximum here is 0x1F because 0x20 is ' ' (space) and the normal charsets begin.
  // Better stay below 0x10 because DISPLAY_CHARSET_HD44780_WESTERN begins here.

  // Symbol characters
  #define LCD_STR_FILAM_DIA   "\xF8"
  #define LCD_STR_FILAM_MUL   "\xA4"

#endif

// Common LCD messages

  /* nothing here yet */

// Common serial messages
#define MSG_MARLIN "Marlin"

// Serial Console Messages (do not translate those!)

#define MSG_ENQUEUEING                      "enqueueing \""
#define MSG_POWERUP                         "PowerUp"
#define MSG_EXTERNAL_RESET                  " External Reset"
#define MSG_BROWNOUT_RESET                  " Brown out Reset"
#define MSG_WATCHDOG_RESET                  " Watchdog Reset"
#define MSG_SOFTWARE_RESET                  " Software Reset"
#define MSG_AUTHOR                          " | Author: "
#define MSG_CONFIGURATION_VER               " Last Updated: "
#define MSG_FREE_MEMORY                     " Free Memory: "
#define MSG_PLANNER_BUFFER_BYTES            "  PlannerBufferBytes: "
#define MSG_OK                              "ok"
#define MSG_WAIT                            "wait"
#define MSG_STATS                           "Stats: "
#define MSG_FILE_SAVED                      "Done saving file."
#define MSG_ERR_LINE_NO                     "Line Number is not Last Line Number+1, Last Line: "
#define MSG_ERR_CHECKSUM_MISMATCH           "checksum mismatch, Last Line: "
#define MSG_ERR_NO_CHECKSUM                 "No Checksum with line number, Last Line: "
#define MSG_FILE_PRINTED                    "Done printing file"
#define MSG_BEGIN_FILE_LIST                 "Begin file list"
#define MSG_END_FILE_LIST                   "End file list"
#define MSG_INVALID_EXTRUDER                "Invalid extruder"
#define MSG_INVALID_E_STEPPER               "Invalid E stepper"
#define MSG_E_STEPPER_NOT_SPECIFIED         "E stepper not specified"
#define MSG_INVALID_SOLENOID                "Invalid solenoid"
#define MSG_ERR_NO_THERMISTORS              "No thermistors - no temperature"
#define MSG_M115_REPORT                     "FIRMWARE_NAME:Marlin " DETAILED_BUILD_VERSION " SOURCE_CODE_URL:" SOURCE_CODE_URL " PROTOCOL_VERSION:" PROTOCOL_VERSION " MACHINE_TYPE:" MACHINE_NAME " EXTRUDER_COUNT:" STRINGIFY(EXTRUDERS) " UUID:" MACHINE_UUID
#define MSG_COUNT_X                         " Count X:"
#define MSG_COUNT_A                         " Count A:"
#define MSG_WATCHDOG_FIRED                  "Watchdog timeout. Reset required."
#define MSG_ERR_KILLED                      "Printer halted. kill() called!"
#define MSG_ERR_STOPPED                     "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define MSG_BUSY_PROCESSING                 "busy: processing"
#define MSG_BUSY_PAUSED_FOR_USER            "busy: paused for user"
#define MSG_BUSY_PAUSED_FOR_INPUT           "busy: paused for input"
#define MSG_Z_MOVE_COMP                     "Z_move_comp"
#define MSG_RESEND                          "Resend: "
#define MSG_UNKNOWN_COMMAND                 "Unknown command: \""
#define MSG_ACTIVE_EXTRUDER                 "Active Extruder: "
#define MSG_X_MIN                           "x_min"
#define MSG_X_MAX                           "x_max"
#define MSG_X2_MIN                          "x2_min"
#define MSG_X2_MAX                          "x2_max"
#define MSG_Y_MIN                           "y_min"
#define MSG_Y_MAX                           "y_max"
#define MSG_Y2_MIN                          "y2_min"
#define MSG_Y2_MAX                          "y2_max"
#define MSG_Z_MIN                           "z_min"
#define MSG_Z_MAX                           "z_max"
#define MSG_Z2_MIN                          "z2_min"
#define MSG_Z2_MAX                          "z2_max"
#define MSG_Z3_MIN                          "z3_min"
#define MSG_Z3_MAX                          "z3_max"
#define MSG_Z_PROBE                         "z_probe"
#define MSG_FILAMENT_RUNOUT_SENSOR          "filament"
#define MSG_PROBE_OFFSET                    "Probe Offset"
#define MSG_SKEW_MIN                        "min_skew_factor: "
#define MSG_SKEW_MAX                        "max_skew_factor: "
#define MSG_ERR_MATERIAL_INDEX              "M145 S<index> out of range (0-1)"
#define MSG_ERR_M421_PARAMETERS             "M421 incorrect parameter usage"
#define MSG_ERR_BAD_PLANE_MODE              "G5 requires XY plane mode"
#define MSG_ERR_MESH_XY                     "Mesh point cannot be resolved"
#define MSG_ERR_ARC_ARGS                    "G2/G3 bad parameters"
#define MSG_ERR_PROTECTED_PIN               "Protected Pin"
#define MSG_ERR_M420_FAILED                 "Failed to enable Bed Leveling"
#define MSG_ERR_M428_TOO_FAR                "Too far from reference point"
#define MSG_ERR_M303_DISABLED               "PIDTEMP disabled"
#define MSG_M119_REPORT                     "Reporting endstop status"
#define MSG_ON                              "ON"
#define MSG_OFF                             "OFF"
#define MSG_ENDSTOP_HIT                     "TRIGGERED"
#define MSG_ENDSTOP_OPEN                    "open"
#define MSG_HOTEND_OFFSET                   "Hotend offsets:"
#define MSG_DUPLICATION_MODE                "Duplication mode: "
#define MSG_SOFT_ENDSTOPS                   "Soft endstops: "
#define MSG_SOFT_MIN                        "  Min: "
#define MSG_SOFT_MAX                        "  Max: "

#define MSG_SD_CANT_OPEN_SUBDIR             "Cannot open subdir "
#define MSG_SD_INIT_FAIL                    "SD init fail"
#define MSG_SD_VOL_INIT_FAIL                "volume.init failed"
#define MSG_SD_OPENROOT_FAIL                "openRoot failed"
#define MSG_SD_CARD_OK                      "SD card ok"
#define MSG_SD_WORKDIR_FAIL                 "workDir open failed"
#define MSG_SD_OPEN_FILE_FAIL               "open failed, File: "
#define MSG_SD_FILE_OPENED                  "File opened: "
#define MSG_SD_SIZE                         " Size: "
#define MSG_SD_FILE_SELECTED                "File selected"
#define MSG_SD_WRITE_TO_FILE                "Writing to file: "
#define MSG_SD_PRINTING_BYTE                "SD printing byte "
#define MSG_SD_NOT_PRINTING                 "Not SD printing"
#define MSG_SD_ERR_WRITE_TO_FILE            "error writing to file"
#define MSG_SD_ERR_READ                     "SD read error"
#define MSG_SD_CANT_ENTER_SUBDIR            "Cannot enter subdir: "

#define MSG_STEPPER_TOO_HIGH                "Steprate too high: "
#define MSG_ENDSTOPS_HIT                    "endstops hit: "
#define MSG_ERR_COLD_EXTRUDE_STOP           " cold extrusion prevented"
#define MSG_ERR_LONG_EXTRUDE_STOP           " too long extrusion prevented"
#define MSG_ERR_HOTEND_TOO_COLD             "Hotend too cold"
#define MSG_ERR_Z_HOMING_SER                "Home XY first"
#define MSG_ERR_EEPROM_WRITE                "Error writing to EEPROM!"

#define MSG_FILAMENT_CHANGE_HEAT_LCD        "Press button to heat nozzle"
#define MSG_FILAMENT_CHANGE_INSERT_LCD      "Insert filament and press button"
#define MSG_FILAMENT_CHANGE_WAIT_LCD        "Press button to resume"
#define MSG_FILAMENT_CHANGE_HEAT_M108       "Send M108 to heat nozzle"
#define MSG_FILAMENT_CHANGE_INSERT_M108     "Insert filament and send M108"
#define MSG_FILAMENT_CHANGE_WAIT_M108       "Send M108 to resume"

#define MSG_STOP_BLTOUCH                    "STOP called because of BLTouch error - restart with M999"
#define MSG_STOP_UNHOMED                    "STOP called because of unhomed error - restart with M999"
#define MSG_KILL_INACTIVE_TIME              "KILL caused by too much inactive time - current command: "
#define MSG_KILL_BUTTON                     "KILL caused by KILL button/pin"

// temperature.cpp strings
#define MSG_PID_AUTOTUNE                    "PID Autotune"
#define MSG_PID_AUTOTUNE_START              MSG_PID_AUTOTUNE " start"
#define MSG_PID_AUTOTUNE_FAILED             MSG_PID_AUTOTUNE " failed!"
#define MSG_PID_BAD_EXTRUDER_NUM            MSG_PID_AUTOTUNE_FAILED " Bad extruder number"
#define MSG_PID_TEMP_TOO_HIGH               MSG_PID_AUTOTUNE_FAILED " Temperature too high"
#define MSG_PID_TIMEOUT                     MSG_PID_AUTOTUNE_FAILED " timeout"
#define MSG_BIAS                            " bias: "
#define MSG_D                               " d: "
#define MSG_T_MIN                           " min: "
#define MSG_T_MAX                           " max: "
#define MSG_KU                              " Ku: "
#define MSG_TU                              " Tu: "
#define MSG_CLASSIC_PID                     " Classic PID "
#define MSG_KP                              " Kp: "
#define MSG_KI                              " Ki: "
#define MSG_KD                              " Kd: "
#define MSG_AT                              " @:"
#define MSG_PID_AUTOTUNE_FINISHED           MSG_PID_AUTOTUNE " finished! Put the last Kp, Ki and Kd constants from below into Configuration.h"
#define MSG_PID_DEBUG                       " PID_DEBUG "
#define MSG_PID_DEBUG_INPUT                 ": Input "
#define MSG_PID_DEBUG_OUTPUT                " Output "
#define MSG_PID_DEBUG_PTERM                 " pTerm "
#define MSG_PID_DEBUG_ITERM                 " iTerm "
#define MSG_PID_DEBUG_DTERM                 " dTerm "
#define MSG_PID_DEBUG_CTERM                 " cTerm "
#define MSG_INVALID_EXTRUDER_NUM            " - Invalid extruder number !"

#define MSG_HEATER_BED                      "bed"
#define MSG_HEATER_CHAMBER                  "chamber"

#define MSG_STOPPED_HEATER                  ", system stopped! Heater_ID: "
#define MSG_REDUNDANCY                      "Heater switched off. Temperature difference between temp sensors is too high !"
#define MSG_T_HEATING_FAILED                "Heating failed"
#define MSG_T_THERMAL_RUNAWAY               "Thermal Runaway"
#define MSG_T_MAXTEMP                       "MAXTEMP triggered"
#define MSG_T_MINTEMP                       "MINTEMP triggered"
#define MSG_ERR_PROBING_FAILED              "Probing Failed"
#define MSG_ZPROBE_OUT_SER                  "Z Probe Past Bed"

// Debug
#define MSG_DEBUG_PREFIX                    "DEBUG:"
#define MSG_DEBUG_OFF                       "off"
#define MSG_DEBUG_ECHO                      "ECHO"
#define MSG_DEBUG_INFO                      "INFO"
#define MSG_DEBUG_ERRORS                    "ERRORS"
#define MSG_DEBUG_DRYRUN                    "DRYRUN"
#define MSG_DEBUG_COMMUNICATION             "COMMUNICATION"
#define MSG_DEBUG_LEVELING                  "LEVELING"

// LCD Menu Messages

#define LANGUAGE_DATA_INCL_(M) STRINGIFY_(fontdata/langdata_##M.h)
#define LANGUAGE_DATA_INCL(M) LANGUAGE_DATA_INCL_(M)

#define LANGUAGE_INCL_(M) STRINGIFY_(../lcd/language/language_##M.h)
#define LANGUAGE_INCL(M) LANGUAGE_INCL_(M)

// Never translate these strings
#define MSG_X "X"
#define MSG_Y "Y"
#define MSG_Z "Z"
#define MSG_E "E"
#if IS_KINEMATIC
  #define MSG_A "A"
  #define MSG_B "B"
  #define MSG_C "C"
#else
  #define MSG_A "X"
  #define MSG_B "Y"
  #define MSG_C "Z"
#endif
#define MSG_X2 "X2"
#define MSG_Y2 "Y2"
#define MSG_Z2 "Z2"
#define MSG_Z3 "Z3"

#define LCD_STR_A MSG_A
#define LCD_STR_B MSG_B
#define LCD_STR_C MSG_C
#define LCD_STR_E MSG_E

/**
 * Tool indexes for LCD display only
 *
 * By convention the LCD shows "E1" for the first extruder.
 * However, internal to Marlin E0/T0 is the first tool, and
 * most board silkscreens say "E0." Zero-based labels will
 * make these indexes consistent but this defies expectation.
 *
 */
#if ENABLED(NUMBER_TOOLS_FROM_0)
  #define LCD_STR_N0 "0"
  #define LCD_STR_N1 "1"
  #define LCD_STR_N2 "2"
  #define LCD_STR_N3 "3"
  #define LCD_STR_N4 "4"
  #define LCD_STR_N5 "5"
#else
  #define LCD_STR_N0 "1"
  #define LCD_STR_N1 "2"
  #define LCD_STR_N2 "3"
  #define LCD_STR_N3 "4"
  #define LCD_STR_N4 "5"
  #define LCD_STR_N5 "6"
#endif

#define LCD_STR_E0 "E" LCD_STR_N0
#define LCD_STR_E1 "E" LCD_STR_N1
#define LCD_STR_E2 "E" LCD_STR_N2
#define LCD_STR_E3 "E" LCD_STR_N3
#define LCD_STR_E4 "E" LCD_STR_N4
#define LCD_STR_E5 "E" LCD_STR_N5

#include "multi_language.h"   // Allow multiple languages

#include "../lcd/language/language_en.h"
#include LANGUAGE_INCL(LCD_LANGUAGE)
#include LANGUAGE_INCL(LCD_LANGUAGE_2)
#include LANGUAGE_INCL(LCD_LANGUAGE_3)
#include LANGUAGE_INCL(LCD_LANGUAGE_4)
#include LANGUAGE_INCL(LCD_LANGUAGE_5)

#if NONE(DISPLAY_CHARSET_ISO10646_1, \
         DISPLAY_CHARSET_ISO10646_5, \
         DISPLAY_CHARSET_ISO10646_KANA, \
         DISPLAY_CHARSET_ISO10646_GREEK, \
         DISPLAY_CHARSET_ISO10646_CN, \
         DISPLAY_CHARSET_ISO10646_TR, \
         DISPLAY_CHARSET_ISO10646_PL, \
         DISPLAY_CHARSET_ISO10646_CZ, \
         DISPLAY_CHARSET_ISO10646_SK)
  #define DISPLAY_CHARSET_ISO10646_1 // use the better font on full graphic displays.
#endif
