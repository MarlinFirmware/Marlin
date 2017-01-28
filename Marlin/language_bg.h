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
 * Bulgarian
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_BG_H
#define LANGUAGE_BG_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5

#define WELCOME_MSG                         MACHINE_NAME " Готов."
#define MSG_SD_INSERTED                     "Картата е поставена"
#define MSG_SD_REMOVED                      "Картата е извадена"
#define MSG_LCD_ENDSTOPS                    "Endstops" // Max length 8 characters
#define MSG_MAIN                            "Меню"
#define MSG_AUTOSTART                       "Автостарт"
#define MSG_DISABLE_STEPPERS                "Изкл. двигатели"
#define MSG_AUTO_HOME                       "Паркиране"
#define MSG_AUTO_HOME_X                     "Home X"
#define MSG_AUTO_HOME_Y                     "Home Y"
#define MSG_AUTO_HOME_Z                     "Home Z"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Click to Begin"
#define MSG_LEVEL_BED_NEXT_POINT            "Next Point"
#define MSG_LEVEL_BED_DONE                  "Leveling Done!"
#define MSG_LEVEL_BED_CANCEL                "Cancel"
#define MSG_SET_HOME_OFFSETS                "Задай Начало"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets applied"
#define MSG_SET_ORIGIN                      "Изходна точка"
#define MSG_PREHEAT_1                       "Подгряване PLA"
#define MSG_PREHEAT_1_N                     "Подгряване PLA"
#define MSG_PREHEAT_1_ALL                   "Подгр. PLA Всички"
#define MSG_PREHEAT_1_BEDONLY               "Подгр. PLA Легло"
#define MSG_PREHEAT_1_SETTINGS              "Настройки PLA"
#define MSG_PREHEAT_2                       "Подгряване ABS"
#define MSG_PREHEAT_2_N                     "Подгряване ABS"
#define MSG_PREHEAT_2_ALL                   "Подгр. ABS Всички"
#define MSG_PREHEAT_2_BEDONLY               "Подгр. ABS Легло"
#define MSG_PREHEAT_2_SETTINGS              "Настройки ABS"
#define MSG_COOLDOWN                        "Охлаждане"
#define MSG_SWITCH_PS_ON                    "Вкл. захранване"
#define MSG_SWITCH_PS_OFF                   "Изкл. захранване"
#define MSG_EXTRUDE                         "Екструзия"
#define MSG_RETRACT                         "Откат"
#define MSG_MOVE_AXIS                       "Движение по ос"
#define MSG_LEVEL_BED                       "Нивелиране"
#define MSG_MOVE_X                          "Движение по X"
#define MSG_MOVE_Y                          "Движение по Y"
#define MSG_MOVE_Z                          "Движение по Z"
#define MSG_MOVE_E                          "Екструдер"
#define MSG_MOVE_01MM                       "Премести с 0.1mm"
#define MSG_MOVE_1MM                        "Премести с 1mm"
#define MSG_MOVE_10MM                       "Премести с 10mm"
#define MSG_SPEED                           "Скорост"
#define MSG_BED_Z                           "Bed Z"
#define MSG_NOZZLE                          LCD_STR_THERMOMETER " Дюза"
#define MSG_BED                             LCD_STR_THERMOMETER " Легло"
#define MSG_FAN_SPEED                       "Вентилатор"
#define MSG_FLOW                            "Поток"
#define MSG_CONTROL                         "Управление"
#define MSG_MIN                             LCD_STR_THERMOMETER " Минимум"
#define MSG_MAX                             LCD_STR_THERMOMETER " Максимум"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Фактор"
#define MSG_AUTOTEMP                        "Авто-темп."
#define MSG_ON                              "Вкл. "
#define MSG_OFF                             "Изкл. "
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Select"
#define MSG_ACC                             "Acc"
#define MSG_VX_JERK                         "Vx-jerk"
#define MSG_VY_JERK                         "Vy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-откат"
#define MSG_A_TRAVEL                        "A-travel"
#define MSG_XSTEPS                          "X стъпки/mm"
#define MSG_YSTEPS                          "Y стъпки/mm"
#define MSG_ZSTEPS                          "Z стъпки/mm"
#define MSG_ESTEPS                          "E стъпки/mm"
#define MSG_TEMPERATURE                     "Температура"
#define MSG_MOTION                          "Движение"
#define MSG_VOLUMETRIC                      "Нишка"
#define MSG_VOLUMETRIC_ENABLED              "E in mm3"
#define MSG_FILAMENT_DIAM                   "Диам. нишка"
#define MSG_CONTRAST                        "LCD контраст"
#define MSG_STORE_EPROM                     "Запази в EPROM"
#define MSG_LOAD_EPROM                      "Зареди от EPROM"
#define MSG_RESTORE_FAILSAFE                "Фабрични настройки"
#define MSG_REFRESH                         LCD_STR_REFRESH "Обнови"
#define MSG_WATCH                           "Преглед"
#define MSG_PREPARE                         "Действия"
#define MSG_TUNE                            "Настройка"
#define MSG_PAUSE_PRINT                     "Пауза"
#define MSG_RESUME_PRINT                    "Възобнови печата"
#define MSG_STOP_PRINT                      "Спри печата"
#define MSG_CARD_MENU                       "Меню карта"
#define MSG_NO_CARD                         "Няма карта"
#define MSG_DWELL                           "Почивка..."
#define MSG_USERWAIT                        "Изчакване"
#define MSG_RESUMING                        "Продълж. печата"
#define MSG_PRINT_ABORTED                   "Печатът е прекъснат"
#define MSG_NO_MOVE                         "Няма движение"
#define MSG_KILLED                          "УБИТО."
#define MSG_STOPPED                         "СПРЯНО."
#define MSG_CONTROL_RETRACT                 "Откат mm"
#define MSG_CONTROL_RETRACT_SWAP            "Смяна Откат mm"
#define MSG_CONTROL_RETRACTF                "Откат  V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Скок mm"
#define MSG_CONTROL_RETRACT_RECOVER         "Възврат +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Смяна Възврат +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "Възврат  V"
#define MSG_AUTORETRACT                     "Автоoткат"
#define MSG_FILAMENTCHANGE                  "Смяна нишка"
#define MSG_INIT_SDCARD                     "Иниц. SD-Карта"
#define MSG_CNG_SDCARD                      "Смяна SD-Карта"
#define MSG_ZPROBE_OUT                      "Z-сондата е извадена"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Self-Test"
#define MSG_BLTOUCH_RESET                   "Reset BLTouch"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Z Отстояние"
#define MSG_BABYSTEP_X                      "Министъпка X"
#define MSG_BABYSTEP_Y                      "Министъпка Y"
#define MSG_BABYSTEP_Z                      "Министъпка Z"
#define MSG_ENDSTOP_ABORT                   "Стоп Кр.Изключватели"
#define MSG_HEATING_FAILED_LCD              "Heating failed"
#define MSG_ERR_REDUNDANT_TEMP              "Err: REDUNDANT TEMP"
#define MSG_THERMAL_RUNAWAY                 "THERMAL RUNAWAY"
#define MSG_ERR_MAXTEMP                     "Err: MAXTEMP"
#define MSG_ERR_MINTEMP                     "Err: MINTEMP"
#define MSG_ERR_MAXTEMP_BED                 "Err: MAXTEMP BED"
#define MSG_ERR_MINTEMP_BED                 "Err: MINTEMP BED"
#define MSG_ERR_Z_HOMING                    "G28 Z Forbidden"
#define MSG_HALTED                          "PRINTER HALTED"
#define MSG_PLEASE_RESET                    "Please reset"
#define MSG_SHORT_DAY                       "d" // One character only
#define MSG_SHORT_HOUR                      "h" // One character only
#define MSG_SHORT_MINUTE                    "m" // One character only
#define MSG_HEATING                         "Heating..."
#define MSG_HEATING_COMPLETE                "Heating done."
#define MSG_BED_HEATING                     "Bed Heating."
#define MSG_BED_DONE                        "Bed done."
#define MSG_DELTA_CALIBRATE                 "Делта Калибровка"
#define MSG_DELTA_CALIBRATE_X               "Калибровка X"
#define MSG_DELTA_CALIBRATE_Y               "Калибровка Y"
#define MSG_DELTA_CALIBRATE_Z               "Калибровка Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Калибровка Център"

#define MSG_INFO_MENU                       "About Printer"
#define MSG_INFO_PRINTER_MENU               "Printer Info"
#define MSG_INFO_STATS_MENU                 "Printer Stats"
#define MSG_INFO_BOARD_MENU                 "Board Info"
#define MSG_INFO_THERMISTOR_MENU            "Thermistors"
#define MSG_INFO_EXTRUDERS                  "Extruders"
#define MSG_INFO_BAUDRATE                   "Baud"
#define MSG_INFO_PROTOCOL                   "Protocol"
#define MSG_LIGHTS_ON                       "Case light on"
#define MSG_LIGHTS_OFF                      "Case light off"

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "Print Count"
  #define MSG_INFO_COMPLETED_PRINTS         "Completed"
  #define MSG_INFO_PRINT_TIME               "Total print time"
  #define MSG_INFO_PRINT_LONGEST            "Longest job time"
  #define MSG_INFO_PRINT_FILAMENT           "Extruded total"
#else
  #define MSG_INFO_PRINT_COUNT              "Prints"
  #define MSG_INFO_COMPLETED_PRINTS         "Completed"
  #define MSG_INFO_PRINT_TIME               "Total"
  #define MSG_INFO_PRINT_LONGEST            "Longest"
  #define MSG_INFO_PRINT_FILAMENT           "Extruded"
#endif

#define MSG_INFO_MIN_TEMP                   "Min Temp"
#define MSG_INFO_MAX_TEMP                   "Max Temp"
#define MSG_INFO_PSU                        "Power Supply"

#define MSG_DRIVE_STRENGTH                  "Drive Strength"
#define MSG_DAC_PERCENT                     "Driver %"
#define MSG_DAC_EEPROM_WRITE                "DAC EEPROM Write"

#define MSG_FILAMENT_CHANGE_HEADER          "CHANGE FILAMENT"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "CHANGE OPTIONS:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Extrude more"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Resume print"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Wait for start"
  #define MSG_FILAMENT_CHANGE_INIT_2          "of the filament"
  #define MSG_FILAMENT_CHANGE_INIT_3          "change"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Wait for"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "filament unload"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insert filament"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "and press button"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "to continue..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Wait for"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "filament load"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Wait for"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "filament extrude"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Wait for print"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "to resume"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Please wait..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Ejecting..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Insert and Click"
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Loading..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Extruding..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Resuming..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_BG_H
