#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "Configuration.h"

#define LANGUAGE_CONCAT(M)       #M
#define GENERATE_LANGUAGE_INCLUDE(M)  LANGUAGE_CONCAT(language_##M.h)


// NOTE: IF YOU CHANGE LANGUAGE FILES OR MERGE A FILE WITH CHANGES
//
//   ==> ALWAYS TRY TO COMPILE MARLIN WITH/WITHOUT "ULTIPANEL" / "ULTRALCD" / "SDSUPPORT" #define IN "Configuration.h"
//   ==> ALSO TRY ALL AVAILABLE LANGUAGE OPTIONS
// See also documentation/LCDLanguageFont.md

// Languages
// en       English
// pl       Polish
// fr       French
// de       German
// es       Spanish
// ru       Russian
// bg       Bulgarian
// it       Italian
// pt       Portuguese
// pt-br    Portuguese (Brazil)
// fi       Finnish
// an       Aragonese
// nl       Dutch
// ca       Catalan
// eu       Basque-Euskera
// kana     Japanese
// kana_utf Japanese
// cn       Chinese

// fallback if no language is set, don't change
#ifndef LANGUAGE_INCLUDE
  #define LANGUAGE_INCLUDE GENERATE_LANGUAGE_INCLUDE(en)
#endif

#if ENABLED(USE_AUTOMATIC_VERSIONING)
  #include "_Version.h"
#else
  #include "Default_Version.h"
#endif

#define PROTOCOL_VERSION "1.0"

#if MB(ULTIMAKER)|| MB(ULTIMAKER_OLD)|| MB(ULTIMAIN_2)
  #define MACHINE_NAME "Ultimaker"
  #define SOURCE_CODE_URL "https://github.com/Ultimaker/Marlin"
#elif MB(RUMBA)
  #define MACHINE_NAME "Rumba"
#elif MB(3DRAG)
  #define MACHINE_NAME "3Drag"
  #define SOURCE_CODE_URL "http://3dprint.elettronicain.it/"
#elif MB(K8200)
  #define MACHINE_NAME "K8200"
  #define SOURCE_CODE_URL "https://github.com/CONSULitAS/Marlin-K8200"
#elif MB(5DPRINT)
  #define MACHINE_NAME "Makibox"
#elif MB(SAV_MKI)
  #define MACHINE_NAME "SAV MkI"
  #define SOURCE_CODE_URL "https://github.com/fmalpartida/Marlin/tree/SAV-MkI-config"
#elif !defined(MACHINE_NAME)
  #define MACHINE_NAME "3D Printer"
#endif

#ifdef CUSTOM_MACHINE_NAME
  #undef MACHINE_NAME
  #define MACHINE_NAME CUSTOM_MACHINE_NAME
#endif

#ifndef SOURCE_CODE_URL
  #define SOURCE_CODE_URL "https://github.com/MarlinFirmware/Marlin"
#endif

#ifndef DETAILED_BUILD_VERSION
  #error BUILD_VERSION Information must be specified
#endif

#ifndef MACHINE_UUID
   #define MACHINE_UUID "00000000-0000-0000-0000-000000000000"
#endif


#define STRINGIFY_(n) #n
#define STRINGIFY(n) STRINGIFY_(n)


// Common LCD messages

  /* nothing here yet */

// Common serial messages
#define MSG_MARLIN "Marlin"

// Serial Console Messages (do not translate those!)

#define MSG_Enqueueing                      "enqueueing \""
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
#define MSG_FILE_SAVED                      "Done saving file."
#define MSG_ERR_LINE_NO                     "Line Number is not Last Line Number+1, Last Line: "
#define MSG_ERR_CHECKSUM_MISMATCH           "checksum mismatch, Last Line: "
#define MSG_ERR_NO_CHECKSUM                 "No Checksum with line number, Last Line: "
#define MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM "No Line Number with checksum, Last Line: "
#define MSG_FILE_PRINTED                    "Done printing file"
#define MSG_BEGIN_FILE_LIST                 "Begin file list"
#define MSG_END_FILE_LIST                   "End file list"
#define MSG_INVALID_EXTRUDER                "Invalid extruder"
#define MSG_INVALID_SOLENOID                "Invalid solenoid"
#define MSG_ERR_NO_THERMISTORS              "No thermistors - no temperature"
#define MSG_M115_REPORT                     "FIRMWARE_NAME:Marlin " DETAILED_BUILD_VERSION " SOURCE_CODE_URL:" SOURCE_CODE_URL " PROTOCOL_VERSION:" PROTOCOL_VERSION " MACHINE_TYPE:" MACHINE_NAME " EXTRUDER_COUNT:" STRINGIFY(EXTRUDERS) " UUID:" MACHINE_UUID "\n"
#define MSG_COUNT_X                         " Count X: "
#define MSG_ERR_KILLED                      "Printer halted. kill() called!"
#define MSG_ERR_STOPPED                     "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define MSG_RESEND                          "Resend: "
#define MSG_UNKNOWN_COMMAND                 "Unknown command: \""
#define MSG_ACTIVE_EXTRUDER                 "Active Extruder: "
#define MSG_X_MIN                           "x_min: "
#define MSG_X_MAX                           "x_max: "
#define MSG_Y_MIN                           "y_min: "
#define MSG_Y_MAX                           "y_max: "
#define MSG_Z_MIN                           "z_min: "
#define MSG_Z_MAX                           "z_max: "
#define MSG_Z2_MAX                          "z2_max: "
#define MSG_Z_PROBE                         "z_probe: "
#define MSG_ERR_MATERIAL_INDEX              "M145 S<index> out of range (0-1)"
#define MSG_ERR_M421_REQUIRES_XYZ           "M421 requires XYZ parameters"
#define MSG_ERR_MESH_INDEX_OOB              "Mesh XY index is out of bounds"
#define MSG_ERR_M428_TOO_FAR                "Too far from reference point"
#define MSG_M119_REPORT                     "Reporting endstop status"
#define MSG_ENDSTOP_HIT                     "TRIGGERED"
#define MSG_ENDSTOP_OPEN                    "open"
#define MSG_HOTEND_OFFSET                   "Hotend offsets:"

#define MSG_SD_CANT_OPEN_SUBDIR             "Cannot open subdir"
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
#define MSG_SD_CANT_ENTER_SUBDIR            "Cannot enter subdir: "

#define MSG_STEPPER_TOO_HIGH                "Steprate too high: "
#define MSG_ENDSTOPS_HIT                    "endstops hit: "
#define MSG_ERR_COLD_EXTRUDE_STOP           " cold extrusion prevented"
#define MSG_ERR_LONG_EXTRUDE_STOP           " too long extrusion prevented"
#define MSG_TOO_COLD_FOR_M600               "M600 Hotend too cold to change filament"
#define MSG_BABYSTEPPING_X                  "Babystepping X"
#define MSG_BABYSTEPPING_Y                  "Babystepping Y"
#define MSG_BABYSTEPPING_Z                  "Babystepping Z"
#define MSG_SERIAL_ERROR_MENU_STRUCTURE     "Error in menu structure"

#define MSG_ERR_EEPROM_WRITE                "Error writing to EEPROM!"

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
#define MSG_B                               "B:"
#define MSG_T                               "T:"
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
#define MSG_STOPPED_HEATER                  ", system stopped! Heater_ID: "
#define MSG_REDUNDANCY                      "Heater switched off. Temperature difference between temp sensors is too high !"
#define MSG_T_HEATING_FAILED                "Heating failed"
#define MSG_T_THERMAL_RUNAWAY               "Thermal Runaway"
#define MSG_T_MAXTEMP                       "MAXTEMP triggered"
#define MSG_T_MINTEMP                       "MINTEMP triggered"

// Debug
#define MSG_DEBUG_ECHO                      "DEBUG ECHO ENABLED"
#define MSG_DEBUG_INFO                      "DEBUG INFO ENABLED"
#define MSG_DEBUG_ERRORS                    "DEBUG ERRORS ENABLED"
#define MSG_DEBUG_DRYRUN                    "DEBUG DRYRUN ENABLED"

// LCD Menu Messages

#if DISABLED(DISPLAY_CHARSET_HD44780_JAPAN) && DISABLED(DISPLAY_CHARSET_HD44780_WESTERN) && DISABLED(DISPLAY_CHARSET_HD44780_CYRILLIC)
  #define DISPLAY_CHARSET_HD44780_JAPAN
#endif

#include LANGUAGE_INCLUDE
#include "language_en.h"

#endif //__LANGUAGE_H
