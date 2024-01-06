//messages.c
#include "src/core/language.h"

//this is because we need CUSTOM_MACHINE_NAME
// #include "Configuration_var.h"
#include "messages.h"

// For the custom characters
// #include "lcd.h"
#include "src/lcd/marlinui.h"

//internationalized messages
const char MSG_ALWAYS[] PROGMEM = PSTR("Always"); ////MSG_ALWAYS c=6
const char MSG_AUTO_HOME[] PROGMEM = PSTR("Auto home"); ////MSG_AUTO_HOME c=18
const char MSG_BABYSTEP_Z[] PROGMEM = PSTR("Live adjust Z"); ////MSG_BABYSTEP_Z c=18
const char MSG_BABYSTEP_Z_NOT_SET[] PROGMEM = PSTR("Distance between tip of the nozzle and the bed surface has not been set yet. Please follow the manual, chapter First steps, section First layer calibration."); ////MSG_BABYSTEP_Z_NOT_SET c=20 r=12
const char MSG_BED[] PROGMEM = PSTR("Bed"); ////MSG_BED c=13
const char MSG_BED_DONE[] PROGMEM = PSTR("Bed done"); ////MSG_BED_DONE c=20
const char MSG_BED_HEATING[] PROGMEM = PSTR("Bed Heating"); ////MSG_BED_HEATING c=20
const char MSG_BED_LEVELING_FAILED_POINT_LOW[] PROGMEM = PSTR("Bed leveling failed. Sensor didn't trigger. Debris on nozzle? Waiting for reset."); ////MSG_BED_LEVELING_FAILED_POINT_LOW c=20 r=6
const char MSG_BED_SKEW_OFFSET_DETECTION_FITTING_FAILED[] PROGMEM = PSTR("XYZ calibration failed. Please consult the manual."); ////MSG_BED_SKEW_OFFSET_DETECTION_FITTING_FAILED c=20 r=8
const char MSG_BELT_STATUS[] PROGMEM = PSTR("Belt status");////MSG_BELT_STATUS c=18
const char MSG_CANCEL[] PROGMEM = PSTR(">Cancel");////MSG_CANCEL c=10
const char MSG_CALIBRATE_Z_AUTO[] PROGMEM = PSTR("Calibrating Z"); ////MSG_CALIBRATE_Z_AUTO c=20 r=2
const char MSG_CARD_MENU[] PROGMEM = PSTR("Print from SD"); ////MSG_CARD_MENU c=18
const char MSG_CHECKING_X[] PROGMEM = PSTR("Checking X axis"); ////MSG_CHECKING_X c=20
const char MSG_CHECKING_Y[] PROGMEM = PSTR("Checking Y axis"); ////MSG_CHECKING_Y c=20
const char MSG_COMMUNITY_MADE[] PROGMEM = PSTR("Community made"); ////MSG_COMMUNITY_MADE c=18
const char MSG_CONFIRM_NOZZLE_CLEAN[] PROGMEM = PSTR("Please clean the nozzle for calibration. Click when done."); ////MSG_CONFIRM_NOZZLE_CLEAN c=20 r=8
const char MSG_COOLDOWN[] PROGMEM = PSTR("Cooldown"); ////MSG_COOLDOWN c=18
const char MSG_CRASH[] PROGMEM = PSTR("Crash"); ////MSG_CRASH c=7
const char MSG_CRASH_DETECTED[] PROGMEM = PSTR("Crash detected."); ////MSG_CRASH_DETECTED c=20
const char MSG_CRASHDETECT[] PROGMEM = PSTR("Crash det."); ////MSG_CRASHDETECT c=13
const char MSG_DONE[] PROGMEM = PSTR("Done"); ////MSG_DONE c=8
const char MSG_ERROR[] PROGMEM = PSTR("ERROR:"); ////MSG_ERROR c=10
const char MSG_EXTRUDER[] PROGMEM = PSTR("Extruder"); ////MSG_EXTRUDER c=17
const char MSG_FANS_CHECK[] PROGMEM = PSTR("Fans check"); ////MSG_FANS_CHECK c=13
const char MSG_FIL_RUNOUTS[] PROGMEM = PSTR("Fil. runouts"); ////MSG_FIL_RUNOUTS c=15
const char MSG_FILAMENT[] PROGMEM = PSTR("Filament"); ////MSG_FILAMENT c=17
const char MSG_FAN_SPEED[] PROGMEM = PSTR("Fan speed"); ////MSG_FAN_SPEED c=14
const char MSG_HOTEND_FAN_SPEED[] PROGMEM = PSTR("Hotend fan:");////MSG_HOTEND_FAN_SPEED c=15
const char MSG_PRINT_FAN_SPEED[] PROGMEM = PSTR("Print fan:"); ////MSG_PRINT_FAN_SPEED c=15
const char MSG_FILAMENT_CLEAN[] PROGMEM = PSTR("Filament extruding & with correct color?"); ////MSG_FILAMENT_CLEAN c=20 r=3
const char MSG_FILAMENT_LOADED[] PROGMEM = PSTR("Is filament loaded?"); ////MSG_FILAMENT_LOADED c=20 r=3
const char MSG_FILAMENTCHANGE[] PROGMEM = PSTR("Change filament"); ////MSG_FILAMENTCHANGE c=18
const char MSG_FIND_BED_OFFSET_AND_SKEW_LINE1[] PROGMEM = PSTR("Searching bed calibration point"); ////MSG_FIND_BED_OFFSET_AND_SKEW_LINE1 c=20 r=3
const char MSG_FINISHING_MOVEMENTS[] PROGMEM = PSTR("Finishing movements"); ////MSG_FINISHING_MOVEMENTS c=20
const char MSG_FOLLOW_CALIBRATION_FLOW[] PROGMEM = PSTR("Printer has not been calibrated yet. Please follow the manual, chapter First steps, section Calibration flow."); ////MSG_FOLLOW_CALIBRATION_FLOW c=20 r=8
const char MSG_FOLLOW_Z_CALIBRATION_FLOW[] PROGMEM = PSTR("There is still a need to make Z calibration. Please follow the manual, chapter First steps, section Calibration flow."); ////MSG_FOLLOW_Z_CALIBRATION_FLOW c=20 r=8
const char MSG_FSENSOR_RUNOUT[] PROGMEM = PSTR("F. runout"); ////MSG_FSENSOR_RUNOUT c=13
const char MSG_FSENSOR_AUTOLOAD[] PROGMEM = PSTR("F. autoload"); ////MSG_FSENSOR_AUTOLOAD c=13
const char MSG_FSENSOR_JAM_DETECTION[] PROGMEM = PSTR("F. jam detect"); ////MSG_FSENSOR_JAM_DETECTION c=13
const char MSG_FSENSOR[] PROGMEM = PSTR("Fil. sensor"); ////MSG_FSENSOR c=12
const char MSG_HEATING[] PROGMEM = PSTR("Heating"); ////MSG_HEATING c=20
const char MSG_HEATING_COMPLETE[] PROGMEM = PSTR("Heating done."); ////MSG_HEATING_COMPLETE c=20
const char MSG_HOMEYZ[] PROGMEM = PSTR("Calibrate Z"); ////MSG_HOMEYZ c=18
const char MSG_ITERATION[] PROGMEM = PSTR("Iteration"); ////MSG_ITERATION c=12
const char MSG_SELECT_FILAMENT[] PROGMEM = PSTR("Select filament:"); ////MSG_SELECT_FILAMENT c=20
const char MSG_LAST_PRINT[] PROGMEM = PSTR("Last print"); ////MSG_LAST_PRINT c=18
const char MSG_LAST_PRINT_FAILURES[] PROGMEM = PSTR("Last print failures"); ////MSG_LAST_PRINT_FAILURES c=20
const char MSG_PRELOAD_TO_MMU[] PROGMEM = PSTR("Preload to MMU"); ////MSG_PRELOAD_TO_MMU c=17
const char MSG_LOAD_FILAMENT[] PROGMEM = PSTR("Load filament"); ////MSG_LOAD_FILAMENT c=16
const char MSG_LOADING_TEST[] PROGMEM = PSTR("Loading Test"); ////MSG_LOADING_TEST c=18
const char MSG_LOADING_FILAMENT[] PROGMEM = PSTR("Loading filament"); ////MSG_LOADING_FILAMENT c=20
const char MSG_TESTING_FILAMENT[] PROGMEM = PSTR("Testing filament"); ////MSG_TESTING_FILAMENT c=20
const char MSG_EJECT_FROM_MMU[] PROGMEM = PSTR("Eject from MMU"); ////MSG_EJECT_FROM_MMU c=16
const char MSG_CUT_FILAMENT[] PROGMEM = PSTR("Cut filament"); ////MSG_CUT_FILAMENT c=16
const char MSG_MAIN[] PROGMEM = PSTR("Main"); ////MSG_MAIN c=18
const char MSG_BACK[] PROGMEM = PSTR("Back"); ////MSG_BACK c=18
const char MSG_SHEET[] PROGMEM = PSTR("Sheet"); ////MSG_SHEET c=10
const char MSG_STEEL_SHEETS[] PROGMEM = PSTR("Steel sheets"); ////MSG_STEEL_SHEETS c=18
const char MSG_MEASURE_BED_REFERENCE_HEIGHT_LINE1[] PROGMEM = PSTR("Measuring reference height of calibration point"); ////MSG_MEASURE_BED_REFERENCE_HEIGHT_LINE1 c=20 r=3
const char MSG_CALIBRATION[] PROGMEM = PSTR("Calibration"); ////MSG_CALIBRATION c=18
const char MSG_MMU_FAILS[] PROGMEM = PSTR("MMU fails"); ////MSG_MMU_FAILS c=15
const char MSG_MMU_LOAD_FAILS[] PROGMEM = PSTR("MMU load fails"); ////MSG_MMU_LOAD_FAILS c=15
const char MSG_MMU_POWER_FAILS[] PROGMEM = PSTR("MMU power fails"); ////MSG_MMU_POWER_FAILS c=15
const char MSG_NO[] PROGMEM = PSTR("No"); ////MSG_NO c=4
const char MSG_NOZZLE[] PROGMEM = PSTR("Nozzle"); ////MSG_NOZZLE c=10
const char MSG_PAPER[] PROGMEM = PSTR("Place a sheet of paper under the nozzle during the calibration of first 4 points. If the nozzle catches the paper, power off the printer immediately."); ////MSG_PAPER c=20 r=8
const char MSG_PAUSE_PRINT[] PROGMEM = PSTR("Pause print");////MSG_PAUSE_PRINT c=18
const char MSG_PRINT_PAUSED[] PROGMEM = PSTR("Print paused");////MSG_PRINT_PAUSED c=20
const char MSG_PLACE_STEEL_SHEET[] PROGMEM = PSTR("Please place steel sheet on heatbed."); ////MSG_PLACE_STEEL_SHEET c=20 r=4
const char MSG_PLEASE_WAIT[] PROGMEM = PSTR("Please wait"); ////MSG_PLEASE_WAIT c=20
const char MSG_POWER_FAILURES[] PROGMEM = PSTR("Power failures"); ////MSG_POWER_FAILURES c=15
const char MSG_PREHEAT_NOZZLE[] PROGMEM = PSTR("Preheat the nozzle!"); ////MSG_PREHEAT_NOZZLE c=20
const char MSG_PRESS_TO_UNLOAD[] PROGMEM = PSTR("Please press the knob to unload filament"); ////MSG_PRESS_TO_UNLOAD c=20 r=4
const char MSG_PRINT_ABORTED[] PROGMEM = PSTR("Print aborted"); ////MSG_PRINT_ABORTED c=20
const char MSG_PULL_OUT_FILAMENT[] PROGMEM = PSTR("Please pull out filament immediately"); ////MSG_PULL_OUT_FILAMENT c=20 r=4
const char MSG_RECOVER_PRINT[] PROGMEM = PSTR("Blackout occurred. Recover print?"); ////MSG_RECOVER_PRINT c=20 r=3
const char MSG_REFRESH[] PROGMEM = PSTR(LCD_STR_REFRESH "Refresh"); ////MSG_REFRESH c=18
const char MSG_REMOVE_STEEL_SHEET[] PROGMEM = PSTR("Please remove steel sheet from heatbed."); ////MSG_REMOVE_STEEL_SHEET c=20 r=4
const char MSG_RESET[] PROGMEM = PSTR("Reset"); ////MSG_RESET c=14
const char MSG_RESUME_PRINT[] PROGMEM = PSTR("Resume print"); ////MSG_RESUME_PRINT c=18
const char MSG_RESUMING_PRINT[] PROGMEM = PSTR("Resuming print"); ////MSG_RESUMING_PRINT c=20
const char MSG_SELFTEST_PART_FAN[] PROGMEM = PSTR("Front print fan?"); ////MSG_SELFTEST_PART_FAN c=20
const char MSG_SELFTEST_HOTEND_FAN[] PROGMEM = PSTR("Left hotend fan?"); ////MSG_SELFTEST_HOTEND_FAN c=20
const char MSG_SELFTEST_FAILED[] PROGMEM = PSTR("Selftest failed"); ////MSG_SELFTEST_FAILED c=20
const char MSG_SELFTEST_FAN[] PROGMEM = PSTR("Fan test"); ////MSG_SELFTEST_FAN c=20
const char MSG_SELFTEST_FAN_NO[] PROGMEM = PSTR("Not spinning"); ////MSG_SELFTEST_FAN_NO c=19
const char MSG_SELFTEST_FAN_YES[] PROGMEM = PSTR("Spinning"); ////MSG_SELFTEST_FAN_YES c=19
const char MSG_SELFTEST_CHECK_BED[] PROGMEM = PSTR("Checking bed"); ////MSG_SELFTEST_CHECK_BED c=20
const char MSG_SELFTEST_CHECK_FSENSOR[] PROGMEM = PSTR("Checking sensors"); ////MSG_SELFTEST_CHECK_FSENSOR c=20
const char MSG_SELFTEST_MOTOR[] PROGMEM = PSTR("Motor"); ////MSG_SELFTEST_MOTOR c=18
const char MSG_SELFTEST_FILAMENT_SENSOR[] PROGMEM = PSTR("Filament sensor"); ////MSG_SELFTEST_FILAMENT_SENSOR c=17
const char MSG_SELFTEST_WIRINGERROR[] PROGMEM = PSTR("Wiring error"); ////MSG_SELFTEST_WIRINGERROR c=18
const char MSG_SETTINGS[] PROGMEM = PSTR("Settings"); ////MSG_SETTINGS c=18
const char MSG_SET_READY[] PROGMEM = PSTR("Set Ready"); ////MSG_SET_READY c=18
const char MSG_SET_NOT_READY[] PROGMEM = PSTR("Set not Ready"); ////MSG_SET_NOT_READY c=18
const char MSG_SELECT_LANGUAGE[] PROGMEM = PSTR("Select language"); ////MSG_SELECT_LANGUAGE c=18
const char MSG_SORTING_FILES[] PROGMEM = PSTR("Sorting files"); ////MSG_SORTING_FILES c=20
const char MSG_TOTAL[] PROGMEM = PSTR("Total"); ////MSG_TOTAL c=6
const char MSG_MATERIAL_CHANGES[] PROGMEM = PSTR("Material changes"); ////MSG_MATERIAL_CHANGES c=18
const char MSG_TOTAL_FAILURES[] PROGMEM = PSTR("Total failures"); ////MSG_TOTAL_FAILURES c=20
const char MSG_HW_SETUP[] PROGMEM = PSTR("HW Setup"); ////MSG_HW_SETUP c=18
const char MSG_TUNE[] PROGMEM =PSTR("Tune"); ////MSG_TUNE c=8
const char MSG_MODE[] PROGMEM = PSTR("Mode"); ////MSG_MODE c=6
const char MSG_HIGH_POWER[] PROGMEM = PSTR("High power"); ////MSG_HIGH_POWER c=10
const char MSG_AUTO_POWER[] PROGMEM = PSTR("Auto power"); ////MSG_AUTO_POWER c=10
const char MSG_SILENT[] PROGMEM = PSTR("Silent"); ////MSG_SILENT c=7
const char MSG_NORMAL[] PROGMEM = PSTR("Normal"); ////MSG_NORMAL c=7
const char MSG_STEALTH[] PROGMEM = PSTR("Stealth"); ////MSG_STEALTH c=7
const char MSG_STEEL_SHEET_CHECK[] PROGMEM = PSTR("Is steel sheet on heatbed?"); ////MSG_STEEL_SHEET_CHECK c=20 r=3
const char MSG_STOP_PRINT[] PROGMEM = PSTR("Stop print"); ////MSG_STOP_PRINT c=18
const char MSG_STOPPED[] PROGMEM = PSTR("STOPPED."); ////MSG_STOPPED c=20
const char MSG_PINDA_CALIBRATION[] PROGMEM = PSTR("PINDA cal."); ////MSG_PINDA_CALIBRATION c=13
const char MSG_PINDA_CALIBRATION_DONE[] PROGMEM = PSTR("PINDA calibration is finished and active. It can be disabled in menu Settings->PINDA cal."); ////MSG_PINDA_CALIBRATION_DONE c=20 r=8
const char MSG_UNLOAD_FILAMENT[] PROGMEM = PSTR("Unload filament"); ////MSG_UNLOAD_FILAMENT c=18
const char MSG_UNLOADING_FILAMENT[] PROGMEM = PSTR("Unloading filament"); ////MSG_UNLOADING_FILAMENT c=20
const char MSG_INFO_SCREEN[] PROGMEM = PSTR("Info screen"); ////MSG_INFO_SCREEN c=18
const char MSG_WIZARD_CALIBRATION_FAILED[] PROGMEM = PSTR("Please check our handbook and fix the problem. Then resume the Wizard by rebooting the printer."); ////MSG_WIZARD_CALIBRATION_FAILED c=20 r=8
const char MSG_WIZARD_DONE[] PROGMEM = PSTR("All is done. Happy printing!"); ////MSG_WIZARD_DONE c=20 r=3
const char MSG_WIZARD_HEATING[] PROGMEM = PSTR("Preheating nozzle. Please wait."); ////MSG_WIZARD_HEATING c=20 r=3
const char MSG_WIZARD_QUIT[] PROGMEM = PSTR("You can always resume the Wizard from Calibration -> Wizard."); ////MSG_WIZARD_QUIT c=20 r=8
const char MSG_WIZARD_WELCOME[] PROGMEM = PSTR("Hi, I am your Original Prusa i3 printer. Would you like me to guide you through the setup process?"); ////MSG_WIZARD_WELCOME c=20 r=7
const char MSG_WIZARD_WELCOME_SHIPPING[] PROGMEM = PSTR("Hi, I am your Original Prusa i3 printer. I will guide you through a short setup process, in which the Z-axis will be calibrated. Then, you will be ready to print."); ////MSG_WIZARD_WELCOME_SHIPPING c=20 r=12
const char MSG_YES[] PROGMEM = PSTR("Yes"); ////MSG_YES c=4
const char MSG_V2_CALIBRATION[] PROGMEM = PSTR("First layer cal."); ////MSG_V2_CALIBRATION c=18
const char MSG_OFF[] PROGMEM = PSTR("Off"); ////MSG_OFF c=3
const char MSG_ON[] PROGMEM = PSTR("On"); ////MSG_ON c=3
const char MSG_NA[] PROGMEM = PSTR("N/A"); ////MSG_NA c=3
const char MSG_CUTTER[] PROGMEM = PSTR("Cutter"); ////MSG_CUTTER c=9
const char MSG_NONE[] PROGMEM = PSTR("None"); ////MSG_NONE c=8
const char MSG_WARN[] PROGMEM = PSTR("Warn"); ////MSG_WARN c=8
const char MSG_STRICT[] PROGMEM = PSTR("Strict"); ////MSG_STRICT c=8
const char MSG_MODEL[] PROGMEM = PSTR("Model"); ////MSG_MODEL c=8
const char MSG_GCODE_DIFF_PRINTER_CONTINUE[] PROGMEM = PSTR("G-code sliced for a different printer type. Continue?"); ////MSG_GCODE_DIFF_PRINTER_CONTINUE c=20 r=3
const char MSG_GCODE_DIFF_PRINTER_CANCELLED[] PROGMEM =PSTR("G-code sliced for a different printer type. Please re-slice the model again. Print cancelled."); ////MSG_GCODE_DIFF_PRINTER_CANCELLED c=20 r=8
const char MSG_GCODE_NEWER_FIRMWARE_CONTINUE[] PROGMEM = PSTR("G-code sliced for a newer firmware. Continue?"); ////MSG_GCODE_NEWER_FIRMWARE_CONTINUE c=20 r=3
const char MSG_GCODE_NEWER_FIRMWARE_CANCELLED[] PROGMEM = PSTR("G-code sliced for a newer firmware. Please update the firmware. Print cancelled."); ////MSG_GCODE_NEWER_FIRMWARE_CANCELLED c=20 r=8
const char MSG_GCODE_DIFF_CONTINUE[] PROGMEM = PSTR("G-code sliced for a different level. Continue?"); ////MSG_GCODE_DIFF_CONTINUE c=20 r=3
const char MSG_GCODE_DIFF_CANCELLED[] PROGMEM = PSTR("G-code sliced for a different level. Please re-slice the model again. Print cancelled."); ////MSG_GCODE_DIFF_CANCELLED c=20 r=8
const char MSG_NOZZLE_DIFFERS_CONTINUE[] PROGMEM = PSTR("Nozzle diameter differs from the G-code. Continue?"); ////MSG_NOZZLE_DIFFERS_CONTINUE c=20 r=3
const char MSG_NOZZLE_DIFFERS_CANCELLED[] PROGMEM = PSTR("Nozzle diameter differs from the G-code. Please check the value in settings. Print cancelled."); ////MSG_NOZZLE_DIFFERS_CANCELLED c=20 r=8
const char MSG_NOZZLE_DIAMETER[] PROGMEM = PSTR("Nozzle d."); ////MSG_NOZZLE_DIAMETER c=10
const char MSG_MMU_MODE[] PROGMEM = PSTR("MMU Mode"); ////MSG_MMU_MODE c=8
const char MSG_SD_CARD[] PROGMEM = PSTR("SD card"); ////MSG_SD_CARD c=8
const char MSG_SORT[] PROGMEM = PSTR("Sort"); ////MSG_SORT c=7
const char MSG_SORT_TIME[] PROGMEM = PSTR("Time"); ////MSG_SORT_TIME c=8
const char MSG_SORT_ALPHA[] PROGMEM = PSTR("Alphabet"); ////MSG_SORT_ALPHA c=8
const char MSG_RPI_PORT[] PROGMEM = PSTR("RPi port"); ////MSG_RPI_PORT c=13
const char MSG_SOUND[] PROGMEM = PSTR("Sound"); ////MSG_SOUND c=9
const char MSG_SOUND_LOUD[] PROGMEM = PSTR("Loud"); ////MSG_SOUND_LOUD c=7
const char MSG_SOUND_ONCE[] PROGMEM = PSTR("Once"); ////MSG_SOUND_ONCE c=7
const char MSG_SOUND_BLIND[] PROGMEM = PSTR("Assist"); ////MSG_SOUND_BLIND c=7
const char MSG_MESH[] PROGMEM = PSTR("Mesh"); ////MSG_MESH c=12
const char MSG_MESH_BED_LEVELING[] PROGMEM = PSTR("Mesh Bed Leveling"); ////MSG_MESH_BED_LEVELING c=18
const char MSG_Z_PROBE_NR[] PROGMEM = PSTR("Z-probe nr."); ////MSG_Z_PROBE_NR c=14
const char MSG_MAGNETS_COMP[] PROGMEM = PSTR("Magnets comp."); ////MSG_MAGNETS_COMP c=13
const char MSG_FS_ACTION[] PROGMEM = PSTR("FS Action"); ////MSG_FS_ACTION c=10
const char MSG_CONTINUE_SHORT[] PROGMEM = PSTR("Cont."); ////MSG_CONTINUE_SHORT c=5
const char MSG_PAUSE[] PROGMEM = PSTR("Pause"); ////MSG_PAUSE c=5
const char MSG_BRIGHTNESS[] PROGMEM = PSTR("Brightness"); ////MSG_BRIGHTNESS c=18
const char MSG_BL_HIGH[] PROGMEM = PSTR("Level Bright"); ////MSG_BL_HIGH c=12
const char MSG_BL_LOW[] PROGMEM = PSTR("Level Dimmed"); ////MSG_BL_LOW c=12
const char MSG_TIMEOUT[] PROGMEM = PSTR("Timeout"); ////MSG_TIMEOUT c=12
const char MSG_BRIGHT[] PROGMEM = PSTR("Bright"); ////MSG_BRIGHT c=6
const char MSG_DIM[] PROGMEM = PSTR("Dim"); ////MSG_DIM c=6
const char MSG_AUTO[] PROGMEM = PSTR("Auto"); ////MSG_AUTO c=6
#if (FILAMENT_SENSOR_TYPE == FSENSOR_IR_ANALOG)
// Beware - the space at the beginning is necessary since it is reused in LCD menu items which are to be with a space
const char MSG_IR_04_OR_NEWER[] PROGMEM = PSTR(" 0.4 or newer");////MSG_IR_04_OR_NEWER c=18
const char MSG_IR_03_OR_OLDER[] PROGMEM = PSTR(" 0.3 or older");////MSG_IR_03_OR_OLDER c=18
const char MSG_IR_UNKNOWN[] PROGMEM = PSTR("unknown state");////MSG_IR_UNKNOWN c=18
#endif
extern const char MSG_PAUSED_THERMAL_ERROR[] PROGMEM = PSTR("PAUSED THERMAL ERROR");////MSG_PAUSED_THERMAL_ERROR c=20
#ifdef THERMAL_MODEL
extern const char MSG_THERMAL_ANOMALY[] PROGMEM = PSTR("THERMAL ANOMALY");////MSG_THERMAL_ANOMALY c=20
extern const char MSG_TM_NOT_CAL[] PROGMEM = PSTR("Thermal model not calibrated yet.");////MSG_TM_NOT_CAL c=20 r=4
extern const char MSG_TM_ACK_ERROR[] PROGMEM = PSTR("Clear TM error");////MSG_TM_ACK_ERROR c=18
#endif
extern const char MSG_LOAD_ALL[] PROGMEM = PSTR("Load All"); ////MSG_LOAD_ALL c=18
extern const char MSG_NOZZLE_CNG_MENU [] PROGMEM = PSTR("Nozzle change");////MSG_NOZZLE_CNG_MENU c=18
extern const char MSG_NOZZLE_CNG_READ_HELP [] PROGMEM = PSTR("For a Nozzle change please read\nprusa.io/nozzle-mk3s");////MSG_NOZZLE_CNG_READ_HELP c=20 r=4
extern const char MSG_NOZZLE_CNG_CHANGED [] PROGMEM = PSTR("Hotend at 280C! Nozzle changed and tightened to specs?");////MSG_NOZZLE_CNG_CHANGED c=20 r=6
extern const char MSG_REPRINT [] PROGMEM = PSTR("Reprint"); ////MSG_REPRINT c=18
//not internationalized messages
#if 0
const char MSG_FW_VERSION_BETA[] PROGMEM = "You are using a BETA firmware version! It is in a development state! Use this version with CAUTION as it may DAMAGE the printer!"; ////MSG_FW_VERSION_BETA c=20 r=8
#endif
const char MSG_SPOOL_JOIN[] PROGMEM = "SpoolJoin"; ////MSG_SPOOL_JOIN c=13
const char MSG_FIRMWARE[] PROGMEM = "Firmware"; ////MSG_FIRMWARE c=8
const char MSG_TOSHIBA_FLASH_AIR_COMPATIBILITY[] PROGMEM = "FlashAir"; ////MSG_TOSHIBA_FLASH_AIR_COMPATIBILITY c=8
const char MSG_PINDA[] PROGMEM = "PINDA"; ////MSG_PINDA c=5
const char MSG_WELCOME[] PROGMEM = WELCOME_MSG;
const char MSG_SD_WORKDIR_FAIL[] PROGMEM = "workDir open failed"; ////
const char MSG_BROWNOUT_RESET[] PROGMEM = " Brown out Reset"; ////
const char MSG_EXTERNAL_RESET[] PROGMEM = " External Reset"; ////
const char MSG_FILE_SAVED[] PROGMEM = "Done saving file."; ////
const char MSG_POSITION_UNKNOWN[] PROGMEM = "Home X/Y before Z"; ////
const char MSG_SOFTWARE_RESET[] PROGMEM = " Software Reset"; ////
const char MSG_UNKNOWN_COMMAND[] PROGMEM = "Unknown command: \""; ////
const char MSG_WATCHDOG_RESET[] PROGMEM = " Watchdog Reset"; ////
const char MSG_Z_MAX[] PROGMEM = "z_max: "; ////
const char MSG_Z_MIN[] PROGMEM = "z_min: "; ////
const char MSG_ZPROBE_OUT[] PROGMEM = "Z probe out. bed"; ////
#ifdef ENABLE_AUTO_BED_LEVELING
const char MSG_ZPROBE_ZOFFSET[] PROGMEM = "Z Offset"; ////
#endif
const char MSG_TMC_OVERTEMP[] PROGMEM = "TMC DRIVER OVERTEMP"; ////
const char MSG_Enqueing[] PROGMEM = "enqueing \""; ////
const char MSG_ENDSTOPS_HIT[] PROGMEM = "endstops hit: "; ////
const char MSG_SD_ERR_WRITE_TO_FILE[] PROGMEM = "error writing to file"; ////
const char MSG_OK[] PROGMEM = "ok"; ////
const char MSG_OK_CAPS[] PROGMEM = "OK"; ////
const char MSG_SD_OPEN_FILE_FAIL[] PROGMEM = "open failed, File: "; ////
const char MSG_ENDSTOP_OPEN[] PROGMEM = "open"; ////
const char MSG_POWERUP[] PROGMEM = "PowerUp"; ////
const char MSG_ERR_STOPPED[] PROGMEM = "Printer stopped due to errors. Supervision required."; ////
const char MSG_ENDSTOP_HIT[] PROGMEM = "TRIGGERED"; ////
const char MSG_OCTOPRINT_ASK_PAUSE[] PROGMEM = "// action:pause"; ////
const char MSG_OCTOPRINT_PAUSED[] PROGMEM = "// action:paused"; ////
const char MSG_OCTOPRINT_ASK_RESUME[] PROGMEM = "// action:resume"; ////
const char MSG_OCTOPRINT_RESUMED[] PROGMEM = "// action:resumed"; ////
const char MSG_OCTOPRINT_CANCEL[] PROGMEM = "// action:cancel"; ////
const char MSG_OCTOPRINT_READY[] PROGMEM = "// action:ready"; ////
const char MSG_OCTOPRINT_NOT_READY[] PROGMEM = "// action:not_ready"; ////
const char MSG_OCTOPRINT_START[] PROGMEM = "// action:start"; ////
const char MSG_OCTOPRINT_UVLO_RECOVERY_READY[] PROGMEM = "// action:uvlo_recovery_ready"; ////
const char MSG_FANCHECK_HOTEND[] PROGMEM = "Err:HOTEND FAN ERROR"; ////c=20
const char MSG_FANCHECK_PRINT[] PROGMEM = "Err:PRINT FAN ERROR"; ////c=20
const char MSG_M112_KILL[] PROGMEM = "M112 called. Emergency Stop."; ////c=20
const char MSG_ADVANCE_K[] PROGMEM = "Advance K:"; ////c=13
const char MSG_POWERPANIC_DETECTED[] PROGMEM = "POWER PANIC DETECTED"; ////c=20
const char MSG_LCD_STATUS_CHANGED[] PROGMEM = "LCD status changed";
const char MSG_UNKNOWN_CODE[] PROGMEM = "Unknown %c code: %s\n";

// Common G-gcodes
const char G1_E_F2700[] PROGMEM = "G1 E%-.3f F2700";
const char G28W[] PROGMEM = "G28 W";
const char MSG_M23[] PROGMEM = "M23 %s";
const char MSG_M24[] PROGMEM = "M24";
const char MSG_M83[] PROGMEM = "M83";
const char MSG_M84[] PROGMEM = "M84";
const char MSG_M107[] PROGMEM = "M107";
const char MSG_M220[] PROGMEM = "M220 S%d";
const char MSG_M500[] PROGMEM = "M500";
const char MSG_M600[] PROGMEM = "M600";
const char MSG_M701[] PROGMEM = "M701";
const char MSG_M702[] PROGMEM = "M702";
