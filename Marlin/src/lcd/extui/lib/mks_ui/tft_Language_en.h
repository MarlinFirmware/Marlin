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
#pragma once

//****************英文***************************//
#define MACHINE_CONFIG_EN              "Config"

#define NEXT_EN                 "Next"
#define PREVIOUS_EN             "Previous"
#define DEFAULT_EN              "Default"
#define KEY_BACK_EN             "Del"
#define KEY_REST_EN             "Reset"
#define KEY_CONFIRM_EN          "Confirm"

#define KEYBOARD_KEY0_EN "0"
#define KEYBOARD_KEY1_EN "1"
#define KEYBOARD_KEY2_EN "2"
#define KEYBOARD_KEY3_EN "3"
#define KEYBOARD_KEY4_EN "4"
#define KEYBOARD_KEY5_EN "5"
#define KEYBOARD_KEY6_EN "6"
#define KEYBOARD_KEY7_EN "7"
#define KEYBOARD_KEY8_EN "8"
#define KEYBOARD_KEY9_EN "9"
#define KEYBOARD_KEY_POINT_EN "."
#define KEYBOARD_KEY_NEGATIVE_EN "-"

#define MACHINE_PARA_TITLE_EN       "Config"
#define MACHINE_TYPE_CNOFIG_EN      "Machine settings"
#define MOTOR_CONFIG_EN             "Motor settings"
#define MACHINE_LEVELING_CONFIG_EN  "Leveling settings"
#define ADVANCE_CONFIG_EN           "Advanced settings"

#define MACHINE_CONFIG_TITLE_EN     "Machine Settings"
#define MACHINE_TYPE_EN             "Machine type"
#define MACHINE_STROKE_EN           "Machine Size"
#define MACHINE_HOMEDIR_EN          "Home direction"
#define MACHINE_ENDSTOP_TYPE_EN     "Endstop type"
#define MACHINE_FILAMENT_CONFIG_EN  "Filament settings"

#define MACHINE_TYPE_CONFIG_TITLE_EN    "Machine Settings>Machine type"
#define MACHINE_TYPE_XYZ_EN             "XYZ Machine"
#define MACHINE_TYPE_DELTA_EN           "Delta Machine"
#define MACHINE_TYPE_COREXY_EN          "Corexy Machine"

#define MACHINE_STROKE_CONF_TITLE_EN    "Machine Settings>Machine Size"
#define X_MAX_LENGTH_EN                 "X-axis maximum stroke"
#define Y_MAX_LENGTH_EN                 "Y-axis maximum stroke"
#define Z_MAX_LENGTH_EN                 "Z-axis maximum stroke"

#define X_MIN_LENGTH_EN                 "X-axis minimum stroke"
#define Y_MIN_LENGTH_EN                 "Y-axis minimum stroke"
#define Z_MIN_LENGTH_EN                 "Z-axis minimum stroke"

#define HOME_DIR_CONF_TITLE_EN          "Machine Settings>Home direction"
#define HOME_DIR_X_EN                   "X-axis home direction"
#define HOME_DIR_Y_EN                   "Y-axis home direction"
#define HOME_DIR_Z_EN                   "Z-axis home direction"
#define HOME_MIN_EN                     "MIN"
#define HOME_MAX_EN                     "MAX"

#define ENDSTOP_CONF_TITLE_EN           "Machine Settings>Endstop type"
#define MIN_ENDSTOP_X_EN                "X-axis minimum Endstop"
#define MIN_ENDSTOP_Y_EN                "Y-axis minimum Endstop"
#define MIN_ENDSTOP_Z_EN                "Z-axis minimum Endstop"
#define MAX_ENDSTOP_X_EN                "X-axis maximum Endstop"
#define MAX_ENDSTOP_Y_EN                "Y-axis maximum Endstop"
#define MAX_ENDSTOP_Z_EN                "Z-axis maximum Endstop"
#define ENDSTOP_FIL_EN                  "Filament sensor"
#define ENDSTOP_LEVEL_EN                "Leveling sensor"
#define ENDSTOP_OPENED_EN               "Open"
#define ENDSTOP_CLOSED_EN               "Close"

#define FILAMENT_CONF_TITLE_EN          "Machine Settings>Filament settings"
#define FILAMENT_IN_LENGTH_EN           "Load length"
#define FILAMENT_IN_SPEED_EN            "Load speed"
#define FILAMENT_TEMPERATURE_EN         "Filament temperature"
#define FILAMENT_OUT_LENGTH_EN          "Unload length"
#define FILAMENT_OUT_SPEED_EN           "Unload speed"

#define LEVELING_CONF_TITLE_EN          "Machine Settings>Leveling settings"
#define LEVELING_PARA_CONF_EN           "Leveling settings"
#define TRAMMING_POS_EN                 "Manual leveling coordinate settings"
#define LEVELING_AUTO_COMMAND_EN        "AutoLeveling command settings"
#define LEVELING_AUTO_ZOFFSET_EN        "Nozzle-to-probe offsets settings"

#define LEVELING_PARA_CONF_TITLE_EN     "leveling setting"
#define AUTO_LEVELING_ENABLE_EN         "Enable auto leveling"
#define BLTOUCH_LEVELING_ENABLE_EN      "Enable BLtouch"
#define PROBE_PORT_EN                   "Probe connector"
#define PROBE_X_OFFSET_EN               "Probe X-axis offset"
#define PROBE_Y_OFFSET_EN               "Probe Y-axis offset"
#define PROBE_Z_OFFSET_EN               "Probe Z-axis offset"
#define PROBE_XY_SPEED_EN               "Probe XY-axis speed"
#define PROBE_Z_SPEED_EN                "Probe Z-axis speed"
#define ENABLE_EN                       "YES"
#define DISABLE_EN                      "NO"
#define LOCKED_EN                       "N/A"
#define Z_MIN_EN                        "ZMin"
#define Z_MAX_EN                        "ZMax"

#define DELTA_LEVEL_CONF_TITLE_EN       "Delta Machine settings"
#define DELTA_LEVEL_CONF_EN             "Delta Machine Leveling"
#define DELTA_MACHINE_RADIUS_EN         "Machine Radius"
#define DELTA_DIAGONAL_ROD_EN           "Machine rod length"
#define DELTA_PRINT_RADIUS_EN           "Print radius"
#define DELTA_HEIGHT_EN                 "Print height"
#define SMOOTH_ROD_OFFSET_EN            "Slider offset"
#define EFFECTOR_OFFSET_EN              "Effector offset"
#define CALIBRATION_RADIUS_EN           "Leveling radius"

#define XYZ_LEVEL_CONF_TITLE_EN         "Cartesian Machine Settings"
#define PROBE_REACH_MAX_LEFT_EN         "Probe reaches leftmost position"
#define PROBE_REACH_MAX_RIGHT_EN        "Probe reaches rightmost position"
#define PROBE_REACH_MAX_FRONT_EN        "Probe reaches front position"
#define PROBE_REACH_MAX_BACK_EN         "Probe reaches final position"

#define TEMPERATURE_CONF_TITLE_EN       "Machine Settings>Temperature settings"
#define NOZZLE_CONF_EN                  "Nozzle settings"
#define HOTBED_CONF_EN                  "Hotbed settings"
#define PREHEAT_TEMPER_EN               "Preset temperature"

#define NOZZLE_CONF_TITLE_EN            "Machine Settings>Nozzle settings"
#define NOZZLECNT_EN                    "Number of nozzles"
#define NOZZLE_TYPE_EN                  "E0 Temperature type"
#define NOZZLE_ADJUST_TYPE_EN           "PID thermostat"
#define NOZZLE_MIN_TEMPERATURE_EN       "lowest temperature"
#define NOZZLE_MAX_TEMPERATURE_EN       "Maximum temperature"
#define EXTRUD_MIN_TEMPER_EN            "Minimum extrusion temperature"

#define HOTBED_CONF_TITLE_EN            "Machine Settings>Hotbed settings"
#define HOTBED_ADJUST_EN                "PID thermostat"
#define HOTBED_MIN_TEMPERATURE_EN       "lowest temperature"
#define HOTBED_MAX_TEMPERATURE_EN       "Maximum temperature"

#define MOTOR_CONF_TITLE_EN             "Machine Settings>Motor settings"
#define MAXFEEDRATE_CONF_EN             "Maximum speed settings"
#define ACCELERATION_CONF_EN            "Acceleration settings"
#define JERKCONF_EN                     "Jerk settings"
#define STEPSCONF_EN                    "Steps settings"
#define TMC_CURRENT_EN                  "TMC Current settings"
#define TMC_STEP_MODE_EN                "TMC Step mode settings"
#define MOTORDIRCONF_EN                 "Motor direction settings"
#define HOMEFEEDRATECONF_EN             "Home speed setting"

#define MAXFEEDRATE_CONF_TITLE_EN       "Machine Settings>Maximum speed"
#define X_MAXFEEDRATE_EN                "X-axis maximum speed"
#define Y_MAXFEEDRATE_EN                "Y-axis maximum speed"
#define Z_MAXFEEDRATE_EN                "Z-axis maximum speed"
#define E0_MAXFEEDRATE_EN               "E0 maximum speed"
#define E1_MAXFEEDRATE_EN               "E1 maximum speed"

#define ACCELERATION_CONF_TITLE_EN      "Machine Settings>Acceleration"
#define PRINT_ACCELERATION_EN           "Print acceleration"
#define RETRACT_ACCELERATION_EN         "Retraction acceleration"
#define TRAVEL_ACCELERATION_EN          "Travel acceleration"
#define X_ACCELERATION_EN               "X-axis acceleration"
#define Y_ACCELERATION_EN               "Y-axis acceleration"
#define Z_ACCELERATION_EN               "Z-axis acceleration"
#define E0_ACCELERATION_EN              "E0 acceleration"
#define E1_ACCELERATION_EN              "E1 acceleration"

#define JERK_CONF_TITLE_EN              "Machine Settings>Jerk speed"
#define X_JERK_EN                       "X-axis jerk speed"
#define Y_JERK_EN                       "Y-axis jerk speed"
#define Z_JERK_EN                       "Z-axis jerk speed"
#define E_JERK_EN                       "Extruder jerk speed"

#define STEPS_CONF_TITLE_EN             "Machine Settings>Steps settings"
#define X_STEPS_EN                      "X-axis steps"
#define Y_STEPS_EN                      "Y-axis steps"
#define Z_STEPS_EN                      "Z-axis steps"
#define E0_STEPS_EN                     "E0 steps"
#define E1_STEPS_EN                     "E1 steps"

#define TMC_CURRENT_CONF_TITLE_EN       "Machine Settings>TMC current settings"
#define X_TMC_CURRENT_EN                "X-axis current (mA)"
#define Y_TMC_CURRENT_EN                "Y-axis current (mA)"
#define Z_TMC_CURRENT_EN                "Z-axis current (mA)"
#define E0_TMC_CURRENT_EN               "E0 current (mA)"
#define E1_TMC_CURRENT_EN               "E1 current (mA)"

#define TMC_MODE_CONF_TITLE_EN          "Machine Settings>TMC step mode settings"
#define X_TMC_MODE_EN                   "Whether X-axis enables stealthChop mode"
#define Y_TMC_MODE_EN                   "Whether Y-axis enables stealthChop mode"
#define Z_TMC_MODE_EN                   "Whether Z-axis enables stealthChop mode"
#define E0_TMC_MODE_EN                  "Whether E0 enables stealthChop mode"
#define E1_TMC_MODE_EN                  "Whether E1 enables stealthChop mode"

#define MOTORDIR_CONF_TITLE_EN          "Machine Settings>Motor direction"
#define X_MOTORDIR_EN                   "X-axis motor direction invert"
#define Y_MOTORDIR_EN                   "Y-axis motor direction invert"
#define Z_MOTORDIR_EN                   "Z-axis motor direction invert"
#define E0_MOTORDIR_EN                  "E0 motor direction invert"
#define E1_MOTORDIR_EN                  "E1 motor direction invert"
#define INVERT_P_EN                     "YES"
#define INVERT_N_EN                     "NO"

#define HOMEFEEDRATE_CONF_TITLE_EN      "Machine Settings>Home speed"
#define X_HOMESPEED_EN                  "XY-axis home speed"
#define Y_HOMESPEED_EN                  "Y-axis home speed"
#define Z_HOMESPEED_EN                  "Z-axis home speed"

#define ADVANCED_CONF_TITLE_EN          "Machine Settings>Advance"
#define PWROFF_DECTION_EN               "power off dection module"
#define PWROFF_AFTER_PRINT_EN           "Auto Shutdown after print"
#define HAVE_UPS_EN                     "Has UPS power supply"
#define Z2_AND_Z2ENDSTOP_CONF_EN        "Z2 Settings"
#define ENABLE_PINS_CONF_EN             "Enable pins level settings"
#define WIFI_SETTINGS_EN                "Wi-Fi parameter settings"
#define HOMING_SENSITIVITY_CONF_EN      "Homing sensitivity settings"
#define ENCODER_SETTINGS_EN             "Rotary encoder settings"

#define Z2_AND_Z2ENDSTOP_CONF_TITLE_EN  "Z2 Settings"
#define Z2_ENABLE_EN                    "Z2 Enable"
#define Z2_ENDSTOP_EN                   "Z2_EndStop Enable"
#define Z2_PORT_EN                      "Z2 Connector"

#define ENABLE_PINS_CONF_TITLE_EN       "ENABLE_PINS_LEVEL"
#define X_ENABLE_PINS_INVERT_EN         "X_ENABLE_PIN_INVERT"
#define Y_ENABLE_PINS_INVERT_EN         "Y_ENABLE_PIN_INVERT"
#define Z_ENABLE_PINS_INVERT_EN         "Z_ENABLE_PIN_INVERT"
#define E_ENABLE_PINS_INVERT_EN         "E_ENABLE_PIN_INVERT"

#define PAUSE_POSITION_EN    "Printing pause position settings"
#define PAUSE_POSITION_X_EN  "X-axis position (Absolute position,-1 invalid)"
#define PAUSE_POSITION_Y_EN  "Y-axis position (Absolute position,-1 invalid)"
#define PAUSE_POSITION_Z_EN  "Z-axis position (Relative position,-1 invalid)"

#define WIFI_SETTINGS_TITLE_EN    "Machine Settings>Wi-Fi Parameter"
#define WIFI_SETTINGS_MODE_EN     "Wi-Fi Mode"
#define WIFI_SETTINGS_NAME_EN     "Wi-Fi Name: "
#define WIFI_SETTINGS_PASSWORD_EN "Wi-Fi Password: "
#define WIFI_SETTINGS_CLOUD_EN    "Do you use cloud services?"
#define WIFI_SETTINGS_CONFIG_EN   "Config"
#define WIFI_SETTINGS_EDIT_EN     "Edit"
#define WIFI_CONFIG_TIPS_EN       "Wi-Fi configuration?"

#define OFFSET_TITLE_EN  "Machine Settings>Offset"
#define OFFSET_X_EN      "X offset"
#define OFFSET_Y_EN      "Y offset"
#define OFFSET_Z_EN      "Z offset"

#define HOMING_SENSITIVITY_CONF_TITLE_EN      "Machine Settings>Sensitivity"
#define X_SENSITIVITY_EN                      "X-axis sensitivity"
#define Y_SENSITIVITY_EN                      "Y-axis sensitivity"
#define Z_SENSITIVITY_EN                      "Z-axis sensitivity"
#define Z2_SENSITIVITY_EN                     "Z2-axis sensitivity"

#define ENCODER_CONF_TITLE_EN                 "Machine Settings>Rotary encoder settings"
#define ENCODER_CONF_TEXT_EN                  "Is the encoder function used?"

#define TOOL_TEXT_EN            "Tool"
#define PREHEAT_TEXT_EN         "Preheat"
#define MOVE_TEXT_EN            "Move"
#define HOME_TEXT_EN            "Home"
#define PRINT_TEXT_EN           "Printing"
#define EXTRUDE_TEXT_EN         "Extrusion"
#define LEVELING_TEXT_EN        "Leveling"
#define AUTO_LEVELING_TEXT_EN   "AutoLevel"
#define SET_TEXT_EN             "Settings"
#define MORE_TEXT_EN            "More"
#define MORE_GCODE_EN           "G-Code"
#define MORE_ENTER_GCODE_EN     "Enter G-Code"

#define ADD_TEXT_EN             "Add"
#define DEC_TEXT_EN             "Dec"
#define EXTRUDER_1_TEXT_EN      "Extrusion1"
#define EXTRUDER_2_TEXT_EN      "Extrusion2"
#define HEATBED_TEXT_EN         "HeatBed"
#define TEXT_1C_EN              "1℃"
#define TEXT_5C_EN              "5℃"
#define TEXT_10C_EN             "10℃"
#define CLOSE_TEXT_EN           "Close"

#define BACK_TEXT_EN            "Back"

#define TOOL_PREHEAT_EN         "Preheat"
#define TOOL_EXTRUDE_EN         "Extrusion"
#define TOOL_MOVE_EN            "Move"
#define TOOL_HOME_EN            "Home"
#define TOOL_LEVELING_EN        "Leveling"
#define TOOL_AUTO_LEVELING_EN   "AutoLevel"
#define TOOL_FILAMENT_EN        "Filament"
#define TOOL_MORE_EN            "More"

#define AXIS_X_ADD_TEXT_EN      "X+"
#define AXIS_X_DEC_TEXT_EN      "X-"
#define AXIS_Y_ADD_TEXT_EN      "Y+"
#define AXIS_Y_DEC_TEXT_EN      "Y-"
#define AXIS_Z_ADD_TEXT_EN      "Z+"
#define AXIS_Z_DEC_TEXT_EN      "Z-"
#define TEXT_01MM_EN            "0.1mm"
#define TEXT_1MM_EN             "1mm"
#define TEXT_10MM_EN            "10mm"

#define HOME_X_TEXT_EN          "X"
#define HOME_Y_TEXT_EN          "Y"
#define HOME_Z_TEXT_EN          "Z"
#define HOME_ALL_TEXT_EN        "Home"
#define HOME_STOPMOVE_EN        "Quickstop"

#define PAGE_UP_TEXT_EN         "Page up"
#define PAGE_DOWN_TEXT_EN       "Page down"

#define EXTRUDER_IN_TEXT_EN           "In"
#define EXTRUDER_OUT_TEXT_EN          "Out"
#define EXTRUDE_1MM_TEXT_EN           "1mm"
#define EXTRUDE_5MM_TEXT_EN           "5mm"
#define EXTRUDE_10MM_TEXT_EN          "10mm"
#define EXTRUDE_LOW_SPEED_TEXT_EN     "Low"
#define EXTRUDE_MEDIUM_SPEED_TEXT_EN  "Normal"
#define EXTRUDE_HIGH_SPEED_TEXT_EN    "High"

#define LEVELING_POINT1_TEXT_EN       "Point1"
#define LEVELING_POINT2_TEXT_EN       "Point2"
#define LEVELING_POINT3_TEXT_EN       "Point3"
#define LEVELING_POINT4_TEXT_EN       "Point4"
#define LEVELING_POINT5_TEXT_EN       "Point5"

#define FILESYS_TEXT_EN               "FileSys"
#define WIFI_TEXT_EN                  "WiFi"
#define FAN_TEXT_EN                   "Fan"
#define ABOUT_TEXT_EN                 "About"
#define BREAK_POINT_TEXT_EN           "Continue"
#define FILAMENT_TEXT_EN              "Filament"
#define LANGUAGE_TEXT_EN              "Language"
#define MOTOR_OFF_TEXT_EN             "Motor-off"
#define MOTOR_OFF_XY_TEXT_EN          "Off-XY"
#define SHUTDOWN_TEXT_EN              "Shutdown"
#define MACHINE_PARA_EN               "Config"
#define EEPROM_SETTINGS_EN            "Eeprom Set"

#define U_DISK_TEXT_EN                "USB"
#define SD_CARD_TEXT_EN               "SD"
#define WIFI_NAME_TEXT_EN             "WiFi: "
#define WIFI_KEY_TEXT_EN              "Key: "
#define WIFI_IP_TEXT_EN               "IP: "
#define WIFI_AP_TEXT_EN               "State: AP"
#define WIFI_STA_TEXT_EN              "State: STA"
#define WIFI_CONNECTED_TEXT_EN        "Connected"
#define WIFI_DISCONNECTED_TEXT_EN     "Disconnected"
#define WIFI_EXCEPTION_TEXT_EN        "Exception"
#define WIFI_RECONNECT_TEXT_EN        "Reconnect"
#define CLOUD_TEXT_EN                 "Cloud"
#define CLOUD_BIND_EN                 "Bind"
#define CLOUD_UNBIND_EN               "Unbind"
#define CLOUD_UNBINDING_EN            "Unbinding"
#define CLOUD_DISCONNECTED_EN         "Disconnected"
#define CLOUD_UNBINDED_EN             "Unbinded"
#define CLOUD_BINDED_EN               "Binded"
#define CLOUD_DISABLE_EN              "Disable"

#define FAN_ADD_TEXT_EN               "Add"
#define FAN_DEC_TEXT_EN               "Dec"
#define FAN_OPEN_TEXT_EN              "100%"
#define FAN_HALF_TEXT_EN              "50%"
#define FAN_CLOSE_TEXT_EN             "Close"
#define FAN_TIPS1_TEXT_EN             "FAN"
#define FAN_TIPS2_TEXT_EN             "FAN\nClose"

#define FILAMENT_IN_TEXT_EN           "Load"
#define FILAMENT_OUT_TEXT_EN          "Unload"
#define FILAMENT_EXT0_TEXT_EN         "Extrusion1"
#define FILAMENT_EXT1_TEXT_EN         "Extrusion2"
#define FILAMENT_HEAT_TEXT_EN         "Preheat"
#define FILAMENT_STOP_TEXT_EN         "Stop"
#define FILAMENT_TIPS2_TEXT_EN        "T:"
#define FILAMENT_TIPS3_TEXT_EN        "Loading..."
#define FILAMENT_TIPS4_TEXT_EN        "Unloading..."
#define FILAMENT_TIPS5_TEXT_EN        "Temp is too low to go,please heat"
#define FILAMENT_TIPS6_TEXT_EN        "Completed"

#define FILAMENT_CHANGE_TEXT_EN                 "Please click <Load> \nor <unload>,After \npinter pause."
#define FILAMENT_DIALOG_LOAD_HEAT_TIPS_EN       "Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_EN     "Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_EN   "Heat completed,please load filament \nto extruder,and click <confirm> \nfor start loading."
#define FILAMENT_DIALOG_LOAD_CONFIRM2_TIPS_EN   "Please load filament to extruder,\nand click <confirm> for start loading."
#define FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_EN  "Heat completed,please \nclick <confirm> for start unloading.!"
#define FILAMENT_DIALOG_LOADING_TIPS_EN         "Is loading ,please wait!"
#define FILAMENT_DIALOG_UNLOADING_TIPS_EN       "Is unloading,please wait!"
#define FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_EN   "Load filament completed,\nclick <confirm> for return!"
#define FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_EN "Unload filament completed,\nclick <confirm> for return!"

#define PRE_HEAT_EXT_TEXT_EN            "E"
#define PRE_HEAT_BED_TEXT_EN            "Bed"

#define FILE_LOADING_EN                 "Loading......"
#define NO_FILE_AND_CHECK_EN          "  No files found!\n            Check the file system configuration!"

#define NO_FILE_EN                          "No files found!"

#define EXTRUDER_TEMP_TEXT_EN               "Temper"
#define EXTRUDER_E_LENGTH1_TEXT_EN          "Extrusion1"
#define EXTRUDER_E_LENGTH2_TEXT_EN          "Extrusion2"
#define EXTRUDER_E_LENGTH3_TEXT_EN          "Extrusion3"

#define ABOUT_TYPE_TEXT_EN                  "Type: "
#define ABOUT_VERSION_TEXT_EN               "Firmware: "
#define ABOUT_WIFI_TEXT_EN                  "WiFi: "

#define PRINTING_OPERATION_EN               "Option"
#define PRINTING_PAUSE_EN                   "Pause"
#define PRINTING_TEMP_EN                    "Temp."
#define PRINTING_CHANGESPEED_EN             "Speed"
#define PRINTING_RESUME_EN                  "Resume"
#define PRINTING_STOP_EN                    "Stop"
#define PRINTING_MORE_EN                    "More"
#define PRINTING_EXTRUDER_EN                "Extrusion"
#define PRINTING_MOVE_EN                    "Move"

#define EXTRUDER_SPEED_EN                   "Extrusion"
#define MOVE_SPEED_EN                       "Move"
#define EXTRUDER_SPEED_STATE_EN             "Extrude Speed"
#define MOVE_SPEED_STATE_EN                 "Move Speed"
#define STEP_1PERCENT_EN                    "1%"
#define STEP_5PERCENT_EN                    "5%"
#define STEP_10PERCENT_EN                   "10%"

#define TITLE_READYPRINT_EN                 "ReadyPrint"
#define TITLE_PREHEAT_EN                    "Preheat"
#define TITLE_MOVE_EN                       "Move"
#define TITLE_HOME_EN                       "Home"
#define TITLE_EXTRUDE_EN                    "Extrusion"
#define TITLE_LEVELING_EN                   "Leveling"
#define TITLE_SET_EN                        "Settings"
#define TITLE_MORE_EN                       "More"
#define TITLE_CHOOSEFILE_EN                 "ChooseFile"
#define TITLE_PRINTING_EN                   "Printing"
#define TITLE_OPERATION_EN                  "Operation"
#define TITLE_ADJUST_EN                     "Adjust"
#define TITLE_WIRELESS_EN                   "Wireless"
#define TITLE_FILAMENT_EN                   "Filament"
#define TITLE_ABOUT_EN                      "About"
#define TITLE_FAN_EN                        "Fan"
#define TITLE_LANGUAGE_EN                   "Language"
#define TITLE_PAUSE_EN                      "Pause"
#define TITLE_CHANGESPEED_EN                "Speed"
#define TITLE_CLOUD_TEXT_EN                 "Cloud"
#define TITLE_DIALOG_CONFIRM_EN             "Confirm"
#define TITLE_FILESYS_EN                    "FileSys"

#define AUTO_SHUTDOWN_EN                    "Auto"
#define MANUAL_SHUTDOWN_EN                  "Manual"

#define DIALOG_CONFIRM_EN                   "Confirm"
#define DIALOG_CANCLE_EN                    "Cancel"
#define DIALOG_OK_EN                        "OK"
#define DIALOG_RESET_EN                     "Reset"
#define DIALOG_RETRY_EN                     "Retry"
#define DIALOG_DISABLE_EN                   "Disable"
#define DIALOG_PRINT_MODEL_EN               "Print this model?"
#define DIALOG_CANCEL_PRINT_EN              "Stop print?"
#define DIALOG_RETRY_EN                     "Retry"
#define DIALOG_STOP_EN                      "Stop"
#define DIALOG_REPRINT_FROM_BREAKPOINT_EN   "Reprint from breakpoint?"
#define DIALOG_ERROR_TIPS1_EN               "Error:no file,please check it again."
#define DIALOG_ERROR_TIPS2_EN               "Error:transaction failed.please check display baudrate \nwhether as the same as mainboard!"
#define DIALOG_ERROR_TIPS3_EN               "Error:file name or path is too long!"
#define DIALOG_CLOSE_MACHINE_EN             "Closing machine......"
#define DIALOG_UNBIND_PRINTER_EN            "Unbind the printer?"
#define DIALOG_FILAMENT_NO_PRESS_EN         "Filament detection switch is not pressed"
#define DIALOG_PRINT_FINISH_EN              "Done print!"
#define DIALOG_PRINT_TIME_EN                "Print time: "
#define DIALOG_REPRINT_EN                   "Print again"
#define DIALOG_WIFI_ENABLE_TIPS_EN "The wifi module is being configured\nplease wait a moment....."

#define HOTBED_ENABLE_EN        "Enable heatbed"
#define MOTOR_EN_HIGH_LEVEL_EN  "High"
#define MOTOR_EN_LOW_LEVEL_EN   "Low"

#define TEXT_WIFI_MENU_TITLE_EN "WI-FI"
#define TEXT_WIFI_SAPCE_EN      "space"
#define TEXT_WIFI_LETTER_EN     "abc"
#define TEXT_WIFI_DIGITAL_EN    "123"
#define TEXT_WIFI_SYMBOL_EN     "#+="
#define TEXT_WIFI_PASSWORD_EN   "Password"

#define TEXT_WIFI_JOINING_EN  "Joining Network..."
#define TEXT_WIFI_FAILED_JOIN_EN  "Failed to Join Wi-Fi"
#define TEXT_WIFI_WIFI_CONECTED_EN "Wi-Fi Connected"

#define TEXT_BUTTON_DISCONECTED_EN  "Disconnect"
#define TEXT_WIFI_FORGET_EN         "Forget Network"
#define TEXT_DISCONECTED_EN         "Wi-Fi Connected"

//wifi-list
#define MAIN_BUILT_EN       "Build"
#define MAIN_FILAMENT_EN    "Filament"
#define MAIN_SETUP_EN       "Setup"
#define MAIN_ABOUT_EN       "About"
#define MAIN_MENU_EN        "Menu"
#define FILE_MENU_BUILD_EN  "Build"
#define FILE_MENU_MENU_EN   " < Menu"

//about
#define ABOUT_TITLE_EN                    "About"
#define ABOUT_BUILT_MACHINES_EN           "Built Machines"
#define ABOUT_SPARK_EN                    "Spark"
#define ABOUT_VERSION_EN                  "Version 1.1.0"
#define ABOUT_SERIAL_NUMBER_EN            "Serial Number:"
#define ABOUT_S_NUMBER_EN                 "DCPLX02KFC6P"

//set
#define SETUP_TITLE_EN                    "Setup"
#define SETUP_WIFI_EN                     "Wi-Fi"
#define SETUP_MANUAL_IP_EN                "Manual IP"
#define SETUP_WIFI_NOT_CONNECTED_EN       "Not Connected"
#define SETUP_WIFI_NETWORK_EN             "WiFi_Network"

//build
#define BUILD_TITLE_EN                    "Build"
#define BUILD_SD_CARD_EN                  "SD Card"
#define BUILD_USB_DRIVE_EN                "USB Drive"

//SD card
#define SD_CARD_TITLE_EN                  "SD Card"
#define SD_CARD_BACK_EN                   "< Back"
//USB Drive
#define USB_DRIVE_TITLE_EN                "USB Drive"
#define USB_DRIVE_BACK_EN                 "< Back"
#define FILE_PAGES_EN                     "%d/%d"
#define FILE_NEXT_PAGE_EN                 "Next Page"

//BUILD PLATE
#define PLATE_TITLE_EN                    "Build Plate"
#define PLATE_BACK_EN                     "< Back"
#define PLATE_CONFIRM_EN                  "Confirm >"
#define PLATE_TIPS_EN                     "Confirm that there is a Clear\nBuild Plate installed in the\nmachine."

//build model
#define MODEL_TITLE_EN                    "Build Model"
#define MODEL_START_BUILD_EN              "Start Build"
#define MODEL_BACK_EN                     "< Back"

//building
#define BUILDING_TITLE_EN                 "Building"
#define BUILDING_MENU_EN                  "Build Menu"
#define BUILDING_COMPLETED                "Build\nComplete"

//building menu
#define BUILDING_MENU_TITLE_EN            "Build Menu"
#define BUILDING_MENU_SETTINGS_EN         "Build Settings"
#define BUILDING_MENU_PAUSE_EN            "Pause Build"
#define BUILDING_MENU_CANCEL_EN           "Cancel Build"
#define BUILDING_MENU_BACK_EN             "< Back"

//build settings
#define SETTINGS_TITLE_EN                 "Build Settings"
#define SETTINGS_NOZZLE_TEMPER_EN         "Nozzle Temp:"
#define SETTINGS_NOZZLE_VALUE_EN          "%d"
#define SETTINGS_BED_TEMPER_EN            "Bed Temp:"
#define SETTINGS_BED_VALUE_EN             "%d"
#define SETTINGS_BUILD_SPEED_EN           "Build Speed:"
#define SETTINGS_SPEED_VALUE_EN           "Standard"
#define SETTINGS_BACK_EN                  "< Back"

//build paused
#define PAUSED_TITLE_EN                   "Build Paused"
#define PAUSED_RESUME_EN                  "Resume Build"
#define PAUSED_CANCEL_EN                  "Cancel Build"
#define PAUSED_BACK_EN                    "< Back"

//build cancel
#define CANCEL_TITLE_EN                   "Cancel Build"
#define CANCEL_BUILD_EN                   "Cancel Build"
#define CANCEL_TIPS_EN                    "Are you sure you want to\ncancel this build? The model\nwill be deleted from this\nmachine. It will need to be\nresent from your computer\nbefore it can be built in the\nfuture."
#define CANCEL_BACK_EN                    "< Back"
#define CANCEL_BUILD_DISPLAY_EN           "Build\nCanceled"
#define CANCEL_OVER_PLATE_TIPS_EN         "Confirm that the Build Plate\nhas been removed from the\nmachine."

//filament model enter
#define FILAMENT_MODEL_ENTER_TITLE_EN     "Model-PLA"
#define FILAMENT_MODEL_ENTER_BACK_EN      "< Back"
#define FILAMENT_MODEL_ENTER_BEGIN_EN     "Begin >"
#define FILAMENT_MODEL_ENTER_TIPS_EN      "The Model Filament spool\ncompartment is located on\nthe right side of the machine."

//filament model PLA
#define FILAMENT_MODEL_PLA_TITLE_EN       "Model-PLA"
#define FILAMENT_PLA_LOAD_TITLE_EN        "Load Filament"
#define FILAMENT_PLA_UNLOAD_TITLE_EN      "Unload Filament"
#define FILAMENT_MODEL_PLA_LOAD_EN        "Load Filament"
#define FILAMENT_MODEL_PLA_UNLOAD_EN      "Unload Filament"
//filament support enter
#define FILAMENT_SUPPORT_ENTER_TITLE_EN   "Support-PVA"
#define FILAMENT_SUPPORT_ENTER_BACK_EN    "< Back"
#define FILAMENT_SUPPORT_ENTER_BEGIN_EN   "Begin >"
#define FILAMENT_SUPPORT_ENTER_TIPS_EN    "The Support Filament spool\ncompartment is located on\nthe left side of the machine."
//filament heating
#define FILAMENT_HEATING_LOAD_TITLE_EN    "Load Filament"
#define FILAMENT_HEATING_UNLOAD_TITLE_EN  "Unload Filament"
#define FILAMENT_HEATING_CANCEL_EN        "< Cancel"
#define FILAMENT_HEATING_MATERIAL_EN      "Material:"
#define FILAMENT_HEATING_PLA_EN           "Model-PLA"
#define FILAMENT_HEATING_TIPS_EN          "Print head is heating..."
//rotate left
#define ROTATE_LEFT_LOAD_TITLE_EN         "Load Filament"
#define ROTATE_LEFT_UNLOAD_TITLE_EN       "Unload Filament"
#define ROTATE_LEFT_CANCEL_EN             "< Cancel"
#define ROTATE_LEFT_MATERIAL_EN           "Material:"
#define ROTATE_LEFT_PLA_EN                "Model-PLA"
#define ROTATE_LEFT_NEXT_EN               "Next >"
#define ROTATE_LEFT_TIPS_EN               "Rotate extruder selection\ndial to the left."

//hang spool
#define HANG_SPOOL_TITLE_EN         "Load Filament"
#define HANG_SPOOL_PREVIOUS_EN      "< Previous"
#define HANG_SPOOL_MATERIAL_EN      "Material:"
#define HANG_SPOOL_PLA_EN           "Model-PLA"
#define HANG_SPOOL_NEXT_EN          "Next >"
#define HANG_SPOOL_TIPS_EN          "Hang the spool in the spool\ncompartment as shown."

//feed filament
#define FEED_FILAMENT_TITLE_EN      "Load Filament"
#define FEED_FILAMENT_PREVIOUS_EN   "< Previous"
#define FEED_FILAMENT_MATERIAL_EN   "Material:"
#define FEED_FILAMENT_PLA_EN        "Model-PLA"
#define FEED_FILAMENT_NEXT_EN       "Next >"
#define FEED_FILAMENT_TIPS_EN       "Feed filament into extruder\nup beyond the gears."

//feed filament
#define ROTATE_UP_TITLE_EN          "Load Filament"
#define ROTATE_UP_PREVIOUS_EN        "< Previous"
#define ROTATE_UP_MATERIAL_EN       "Material:"
#define ROTATE_UP_PLA_EN            "Model-PLA"
#define ROTATE_UP_NEXT_EN           "Next >"
#define ROTATE_UP_TIPS_EN           "Rotate extruder selection\ndial up."

//filament begin
#define FEED_BEGIN_TITLE_EN         "Load Filament"
#define FEED_BEGIN_MATERIAL_EN      "Material:"
#define FEED_BEGIN_PLA_EN           "Model-PLA"
#define FEED_BEGIN_NEXT_EN          "Next >"
#define FEED_BEGIN_TIPS_EN          "Press Next when filament\nbegins to extrude."

//filament finish
#define FEED_FINISH_TITLE_EN        "Load Filament"
#define FEED_FINISH_MATERIAL_EN     "Material:"
#define FEED_FINISH_PLA_EN          "Model-PLA"
#define FEED_FINISH_NEXT_EN         "Finish >"
#define FEED_FINISH_TIPS_EN         "Remove filament from the\nnozzle and discard."
//fiament remove
#define REMOVE_SPOOL_TITLE_EN       "Unload Filament"
#define REMOVE_SPOOL_PREVIOUS_EN     "< Previous"
#define REMOVE_SPOOL_FINISH_EN      "Finish >"
#define REMOVE_SPOOL_MATERIAL_EN    "Material:"
#define REMOVE_SPOOL_PLA_EN         "Model-PLA"
#define REMOVE_SPOOL_TIPS_EN        "Remove the spool and pull\nfilament out of the machine."

#define FILAMENT_SUPPORT_PVA_EN     "Support-PVA"
#define LOAD_FINISH_EN              "Load\nFilament\nComplete"
#define UNLOAD_FINISH_EN            "Unload\nFilament\nComplete"

//manual ip
#define MANUAL_IP_TITLE_EN          "Manual IP"
#define MANUAL_IP_CANCEL_EN         "< Cancel"
#define MANUAL_IP_APPLY_EN          "Join >"
#define MANUAL_IP_ADDRESS_EN        "IP Address"
#define MANUAL_IP_MASK_EN           "Subnet Mask"
#define MANUAL_IP_GATEWAY_EN        "Default Gateway"
#define MANUAL_IP_SERVER_EN         "Name Server"
#define MANUAL_IP_INIT_DATA_EN      "0.0.0.0"
#define MANUAL_TEXT_POINT_EN        "."
#define MANUAL_TEXT_ENTER_EN        "enter"

#define TEXT_FORGET_TIPS_TITLE_EN "Forget Network"
#define TEXT_FORGET_NETWORK_TIPS1_EN "Are you sure you want to\nforget this network?"
#define TEXT_FORGET_NETWORK_TIPS2_EN "This machine will no longer\njoin this Wi-Fi Network."

#define TEXT_IPADDRESS_EN "IP Address: "

#define TEXT_BUILD_FROM_CURA_CANCEL_TIPS1_EN "Are you sure you want to\ncancel this build?"
#define TEXT_BUILD_FROM_CURA_CANCEL_TIPS2_EN "The model will be deleted\nfrom this machine.It will\nneed to be resent from your\ncomputer before it can be\nbuilt in the future."

#define DIALOG_CONFIRM_EN2              "Confirm"

#define HEATING_TITLE_EN  "Heating"
#define LEVELING_TITLE_EN  "Leveling"

#define ABOUT_SPARK_ADD_EN "Spark+"

#define TEXT_RECEIVING_DATA_EN "Receiving Data"

#define TEXT_BABY_STEP_EN "Babystep"

#define PRINTING_OTHER_LANGUGE              "Printing"
#define PRINTING_OPERATION_OTHER_LANGUGE    "Operation"
#define PRINTING_PAUSE_OTHER_LANGUGE        "Pause"

#define MESSAGE_PAUSING_EN        "Parking..."
#define MESSAGE_CHANGING_EN       "Wait for filament change to start"
#define MESSAGE_UNLOAD_EN         "Wait for filament unload"
#define MESSAGE_WAITING_EN        "Press Button to resume print"
#define MESSAGE_INSERT_EN         "Insert filament and press button to continue"
#define MESSAGE_LOAD_EN           "Wait for filament load"
#define MESSAGE_PURGE_EN          "Wait for filament purge"
#define MESSAGE_RESUME_EN         "Wait for print to resume..."
#define MESSAGE_HEAT_EN           "Press button to heat nozzle"
#define MESSAGE_HEATING_EN        "Nozzle heating Please wait..."
#define MESSAGE_OPTION_EN         "Purge more or continue print?"
#define MESSAGE_PURGE_MORE_EN     "Purge"
#define MESSAGE_CONTINUE_PRINT_EN "Print"
#define EEPROM_SETTINGS_TITLE_EN  "EEPROM Settings"
#define EEPROM_SETTINGS_STORE_EN  "Store settings to EEPROM"
#define EEPROM_SETTINGS_READ_EN   "Read settings from EEPROM"
#define EEPROM_SETTINGS_REVERT_EN "Revert settings to factory defaults"

#define EEPROM_STORE_TIPS_EN  "Store settings to EEPROM?"
#define EEPROM_READ_TIPS_EN   "Read settings from EEPROM?"
#define EEPROM_REVERT_TIPS_EN "Revert settings to factory defaults?"

#define MORE_CUSTOM1_TEXT_EN  MAIN_MENU_ITEM_1_DESC
#define MORE_CUSTOM2_TEXT_EN  MAIN_MENU_ITEM_2_DESC
#define MORE_CUSTOM3_TEXT_EN  MAIN_MENU_ITEM_3_DESC
#define MORE_CUSTOM4_TEXT_EN  MAIN_MENU_ITEM_4_DESC
#define MORE_CUSTOM5_TEXT_EN  MAIN_MENU_ITEM_5_DESC
#define MORE_CUSTOM6_TEXT_EN  MAIN_MENU_ITEM_6_DESC
