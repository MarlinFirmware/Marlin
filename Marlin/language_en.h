/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * English
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_EN_H
#define LANGUAGE_EN_H

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" ready.")
#endif
#ifndef MSG_BACK
  #define MSG_BACK                            _UxGT("Back")
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     _UxGT("Card inserted")
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      _UxGT("Card removed")
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            _UxGT("Main")
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       _UxGT("Autostart")
#endif
#ifndef MSG_DISABLE_STEPPERS
  #define MSG_DISABLE_STEPPERS                _UxGT("Disable steppers")
#endif
#ifndef MSG_DEBUG_MENU
  #define MSG_DEBUG_MENU                      _UxGT("Debug Menu")
#endif
#ifndef MSG_PROGRESS_BAR_TEST
  #define MSG_PROGRESS_BAR_TEST               _UxGT("Progress Bar Test")
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME                       _UxGT("Auto home")
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X                     _UxGT("Home X")
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y                     _UxGT("Home Y")
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z                     _UxGT("Home Z")
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Next Point")
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#endif
#ifndef MSG_LEVEL_BED_CANCEL
  #define MSG_LEVEL_BED_CANCEL                _UxGT("Cancel")
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS                _UxGT("Set home offsets")
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN                      _UxGT("Set origin")
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1                       _UxGT("Preheat PLA")
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" All")
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Bed")
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" conf")
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2                       _UxGT("Preheat ABS")
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" All")
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Bed")
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" conf")
#endif
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        _UxGT("Cooldown")
#endif
#ifndef MSG_SWITCH_PS_ON
  #define MSG_SWITCH_PS_ON                    _UxGT("Switch power on")
#endif
#ifndef MSG_SWITCH_PS_OFF
  #define MSG_SWITCH_PS_OFF                   _UxGT("Switch power off")
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE                         _UxGT("Extrude")
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT                         _UxGT("Retract")
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS                       _UxGT("Move axis")
#endif
#ifndef MSG_LEVEL_BED
  #define MSG_LEVEL_BED                       _UxGT("Level bed")
#endif
#ifndef MSG_MOVING
  #define MSG_MOVING                          _UxGT("Moving...")
#endif
#ifndef MSG_FREE_XY
  #define MSG_FREE_XY                         _UxGT("Free XY")
#endif
#ifndef MSG_MOVE_X
  #define MSG_MOVE_X                          _UxGT("Move X")
#endif
#ifndef MSG_MOVE_Y
  #define MSG_MOVE_Y                          _UxGT("Move Y")
#endif
#ifndef MSG_MOVE_Z
  #define MSG_MOVE_Z                          _UxGT("Move Z")
#endif
#ifndef MSG_MOVE_E
  #define MSG_MOVE_E                          _UxGT("Extruder")
#endif
#ifndef MSG_MOVE_01MM
  #define MSG_MOVE_01MM                       _UxGT("Move 0.1mm")
#endif
#ifndef MSG_MOVE_1MM
  #define MSG_MOVE_1MM                        _UxGT("Move 1mm")
#endif
#ifndef MSG_MOVE_10MM
  #define MSG_MOVE_10MM                       _UxGT("Move 10mm")
#endif
#ifndef MSG_SPEED
  #define MSG_SPEED                           _UxGT("Speed")
#endif
#ifndef MSG_BED_Z
  #define MSG_BED_Z                           _UxGT("Bed Z")
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE                          _UxGT("Nozzle")
#endif
#ifndef MSG_BED
  #define MSG_BED                             _UxGT("Bed")
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       _UxGT("Fan speed")
#endif
#ifndef MSG_FLOW
  #define MSG_FLOW                            _UxGT("Flow")
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL                         _UxGT("Control")
#endif
#ifndef MSG_MIN
  #define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Min")
#endif
#ifndef MSG_MAX
  #define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Max")
#endif
#ifndef MSG_FACTOR
  #define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Fact")
#endif
#ifndef MSG_AUTOTEMP
  #define MSG_AUTOTEMP                        _UxGT("Autotemp")
#endif
#ifndef MSG_ON
  #define MSG_ON                              _UxGT("On ")
#endif
#ifndef MSG_OFF
  #define MSG_OFF                             _UxGT("Off")
#endif
#ifndef MSG_PID_P
  #define MSG_PID_P                           _UxGT("PID-P")
#endif
#ifndef MSG_PID_I
  #define MSG_PID_I                           _UxGT("PID-I")
#endif
#ifndef MSG_PID_D
  #define MSG_PID_D                           _UxGT("PID-D")
#endif
#ifndef MSG_PID_C
  #define MSG_PID_C                           _UxGT("PID-C")
#endif
#ifndef MSG_SELECT
  #define MSG_SELECT                          _UxGT("Select")
#endif
#ifndef MSG_ACC
  #define MSG_ACC                             _UxGT("Accel")
#endif
#ifndef MSG_VX_JERK
  #define MSG_VX_JERK                         _UxGT("Vx-jerk")
#endif
#ifndef MSG_VY_JERK
  #define MSG_VY_JERK                         _UxGT("Vy-jerk")
#endif
#ifndef MSG_VZ_JERK
  #define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         _UxGT("Ve-jerk")
#endif
#ifndef MSG_VMAX
  #define MSG_VMAX                            _UxGT("Vmax ")
#endif
#ifndef MSG_VMIN
  #define MSG_VMIN                            _UxGT("Vmin")
#endif
#ifndef MSG_VTRAV_MIN
  #define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX                            _UxGT("Amax ")
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT                       _UxGT("A-retract")
#endif
#ifndef MSG_A_TRAVEL
  #define MSG_A_TRAVEL                        _UxGT("A-travel")
#endif
#ifndef MSG_XSTEPS
  #define MSG_XSTEPS                          _UxGT("Xsteps/mm")
#endif
#ifndef MSG_YSTEPS
  #define MSG_YSTEPS                          _UxGT("Ysteps/mm")
#endif
#ifndef MSG_ZSTEPS
  #define MSG_ZSTEPS                          _UxGT("Zsteps/mm")
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS                          _UxGT("Esteps/mm")
#endif
#ifndef MSG_E1STEPS
  #define MSG_E1STEPS                         _UxGT("E1steps/mm")
#endif
#ifndef MSG_E2STEPS
  #define MSG_E2STEPS                         _UxGT("E2steps/mm")
#endif
#ifndef MSG_E3STEPS
  #define MSG_E3STEPS                         _UxGT("E3steps/mm")
#endif
#ifndef MSG_E4STEPS
  #define MSG_E4STEPS                         _UxGT("E4steps/mm")
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE                     _UxGT("Temperature")
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION                          _UxGT("Motion")
#endif
#ifndef MSG_VOLUMETRIC
  #define MSG_VOLUMETRIC                      _UxGT("Filament")
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   _UxGT("Fil. Dia.")
#endif
#ifndef MSG_CONTRAST
  #define MSG_CONTRAST                        _UxGT("LCD contrast")
#endif
#ifndef MSG_STORE_EPROM
  #define MSG_STORE_EPROM                     _UxGT("Store memory")
#endif
#ifndef MSG_LOAD_EPROM
  #define MSG_LOAD_EPROM                      _UxGT("Load memory")
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE                _UxGT("Restore failsafe")
#endif
#ifndef MSG_REFRESH
  #define MSG_REFRESH                         _UxGT("Refresh")
#endif
#ifndef MSG_WATCH
  #define MSG_WATCH                           _UxGT("Info screen")
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE                         _UxGT("Prepare")
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE                            _UxGT("Tune")
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     _UxGT("Pause print")
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    _UxGT("Resume print")
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      _UxGT("Stop print")
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU                       _UxGT("Print from SD")
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD                         _UxGT("No SD card")
#endif
#ifndef MSG_DWELL
  #define MSG_DWELL                           _UxGT("Sleep...")
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT                        _UxGT("Wait for user...")
#endif
#ifndef MSG_RESUMING
  #define MSG_RESUMING                        _UxGT("Resuming print")
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED                   _UxGT("Print aborted")
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE                         _UxGT("No move.")
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED                          _UxGT("KILLED. ")
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED                         _UxGT("STOPPED. ")
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT                 _UxGT("Retract mm")
#endif
#ifndef MSG_CONTROL_RETRACT_SWAP
  #define MSG_CONTROL_RETRACT_SWAP            _UxGT("Swap Re.mm")
#endif
#ifndef MSG_CONTROL_RETRACTF
  #define MSG_CONTROL_RETRACTF                _UxGT("Retract  V")
#endif
#ifndef MSG_CONTROL_RETRACT_ZLIFT
  #define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Hop mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER         _UxGT("UnRet mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("S UnRet mm")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("UnRet  V")
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     _UxGT("AutoRetr.")
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  _UxGT("Change filament")
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD                     _UxGT("Init. SD card")
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD                      _UxGT("Change SD card")
#endif
#ifndef MSG_ZPROBE_OUT
  #define MSG_ZPROBE_OUT                      _UxGT("Z probe out. bed")
#endif
#ifndef MSG_BLTOUCH_SELFTEST
  #define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#endif
#ifndef MSG_BLTOUCH_RESET
  #define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#endif
#ifndef MSG_HOME
  #define MSG_HOME                            _UxGT("Home") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST                           _UxGT("first")
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Offset")
#endif
#ifndef MSG_BABYSTEP_X
  #define MSG_BABYSTEP_X                      _UxGT("Babystep X")
#endif
#ifndef MSG_BABYSTEP_Y
  #define MSG_BABYSTEP_Y                      _UxGT("Babystep Y")
#endif
#ifndef MSG_BABYSTEP_Z
  #define MSG_BABYSTEP_Z                      _UxGT("Babystep Z")
#endif
#ifndef MSG_ENDSTOP_ABORT
  #define MSG_ENDSTOP_ABORT                   _UxGT("Endstop abort")
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD              _UxGT("Heating failed")
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP                     _UxGT("Err: MAXTEMP")
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP                     _UxGT("Err: MINTEMP")
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: MAXTEMP BED")
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #define MSG_ERR_MINTEMP_BED                 _UxGT("Err: MINTEMP BED")
#endif
#ifndef MSG_ERR_Z_HOMING
  #define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Forbidden")
#endif
#ifndef MSG_HALTED
  #define MSG_HALTED                          _UxGT("PRINTER HALTED")
#endif
#ifndef MSG_PLEASE_RESET
  #define MSG_PLEASE_RESET                    _UxGT("Please reset")
#endif
#ifndef MSG_SHORT_DAY
  #define MSG_SHORT_DAY                       _UxGT("d") // One character only
#endif
#ifndef MSG_SHORT_HOUR
  #define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING                         _UxGT("Heating...")
#endif
#ifndef MSG_HEATING_COMPLETE
  #define MSG_HEATING_COMPLETE                _UxGT("Heating done.")
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     _UxGT("Bed Heating.")
#endif
#ifndef MSG_BED_DONE
  #define MSG_BED_DONE                        _UxGT("Bed done.")
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE                 _UxGT("Delta Calibration")
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X               _UxGT("Calibrate X")
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y               _UxGT("Calibrate Y")
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z               _UxGT("Calibrate Z")
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Calibrate Center")
#endif
#ifndef MSG_INFO_MENU
  #define MSG_INFO_MENU                       _UxGT("About Printer")
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#endif
#ifndef MSG_INFO_STATS_MENU
  #define MSG_INFO_STATS_MENU                 _UxGT("Printer Stats")
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU                 _UxGT("Board Info")
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#endif
#ifndef MSG_INFO_BAUDRATE
  #define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#endif
#ifndef MSG_INFO_PROTOCOL
  #define MSG_INFO_PROTOCOL                   _UxGT("Protocol")
#endif
#ifndef MSG_LIGHTS_ON
  #define MSG_LIGHTS_ON                       _UxGT("Case light on")
#endif
#ifndef MSG_LIGHTS_OFF
  #define MSG_LIGHTS_OFF                      _UxGT("Case light off")
#endif

#if LCD_WIDTH >= 20
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Print Count")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Total print time")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Longest job time")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruded total")
  #endif
#else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Prints")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Longest")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruded")
  #endif
#endif

#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU                        _UxGT("Power Supply")
#endif

#ifndef MSG_DRIVE_STRENGTH
  #define MSG_DRIVE_STRENGTH                  _UxGT("Drive Strength")
#endif
#ifndef MSG_DAC_PERCENT
  #define MSG_DAC_PERCENT                     _UxGT("Driver %")
#endif
#ifndef MSG_DAC_EEPROM_WRITE
  #define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM Write")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER
  #define MSG_FILAMENT_CHANGE_HEADER          _UxGT("CHANGE FILAMENT")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("CHANGE OPTIONS:")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_EXTRUDE
  #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Extrude more")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Resume print")
#endif
#ifndef MSG_FILAMENT_CHANGE_MINTEMP
  #define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Minimum Temp is ")
#endif
#ifndef MSG_FILAMENT_CHANGE_NOZZLE
  #define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Nozzle: ")
#endif

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Wait for start")
    #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("of the filament")
    #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("change")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament unload")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert filament")
    #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("and press button")
    #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("to continue...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEAT_1
    #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Press button to")
    #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("heat nozzle.")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Heating nozzle")
    #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Please wait...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament load")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Wait for")
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("filament extrude")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wait for print")
    #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("to resume")
  #endif
#else // LCD_HEIGHT < 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Please wait...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ejecting...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert and Click")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Heating...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Loading...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Extruding...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Resuming...")
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_EN_H
