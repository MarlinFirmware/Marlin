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

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" Готов.")
#define MSG_SD_INSERTED                     _UxGT("Картата е поставена")
#define MSG_SD_REMOVED                      _UxGT("Картата е извадена")
#define MSG_LCD_ENDSTOPS                    _UxGT("Endstops") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Меню")
#define MSG_AUTOSTART                       _UxGT("Автостарт")
#define MSG_DISABLE_STEPPERS                _UxGT("Изкл. двигатели")
#define MSG_AUTO_HOME                       _UxGT("Паркиране")
#define MSG_AUTO_HOME_X                     _UxGT("Home X")
#define MSG_AUTO_HOME_Y                     _UxGT("Home Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Home Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Homing XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Click to Begin")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Next Point")
#define MSG_LEVEL_BED_DONE                  _UxGT("Leveling Done!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Cancel")
#define MSG_SET_HOME_OFFSETS                _UxGT("Задай Начало")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Offsets applied")
#define MSG_SET_ORIGIN                      _UxGT("Изходна точка")
#define MSG_PREHEAT_1                       _UxGT("Подгряване PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Подгряване PLA")
#define MSG_PREHEAT_1_ALL                   _UxGT("Подгр. PLA Всички")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Подгр. PLA Легло")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Настройки PLA")
#define MSG_PREHEAT_2                       _UxGT("Подгряване ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Подгряване ABS")
#define MSG_PREHEAT_2_ALL                   _UxGT("Подгр. ABS Всички")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Подгр. ABS Легло")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Настройки ABS")
#define MSG_COOLDOWN                        _UxGT("Охлаждане")
#define MSG_SWITCH_PS_ON                    _UxGT("Вкл. захранване")
#define MSG_SWITCH_PS_OFF                   _UxGT("Изкл. захранване")
#define MSG_EXTRUDE                         _UxGT("Екструзия")
#define MSG_RETRACT                         _UxGT("Откат")
#define MSG_MOVE_AXIS                       _UxGT("Движение по ос")
#define MSG_LEVEL_BED                       _UxGT("Нивелиране")
#define MSG_MOVE_X                          _UxGT("Движение по X")
#define MSG_MOVE_Y                          _UxGT("Движение по Y")
#define MSG_MOVE_Z                          _UxGT("Движение по Z")
#define MSG_MOVE_E                          _UxGT("Екструдер")
#define MSG_MOVE_01MM                       _UxGT("Премести с 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Премести с 1mm")
#define MSG_MOVE_10MM                       _UxGT("Премести с 10mm")
#define MSG_SPEED                           _UxGT("Скорост")
#define MSG_BED_Z                           _UxGT("Bed Z")
#define MSG_NOZZLE                          LCD_STR_THERMOMETER _UxGT(" Дюза")
#define MSG_BED                             LCD_STR_THERMOMETER _UxGT(" Легло")
#define MSG_FAN_SPEED                       _UxGT("Вентилатор")
#define MSG_FLOW                            _UxGT("Поток")
#define MSG_CONTROL                         _UxGT("Управление")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Минимум")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Максимум")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Фактор")
#define MSG_AUTOTEMP                        _UxGT("Авто-темп.")
#define MSG_ON                              _UxGT("Вкл. ")
#define MSG_OFF                             _UxGT("Изкл. ")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Select")
#define MSG_ACC                             _UxGT("Acc")
#define MSG_VX_JERK                         _UxGT("Vx-jerk")
#define MSG_VY_JERK                         _UxGT("Vy-jerk")
#define MSG_VZ_JERK                         _UxGT("Vz-jerk")
#define MSG_VE_JERK                         _UxGT("Ve-jerk")
#define MSG_VMAX                            _UxGT("Vmax ")
#define MSG_VMIN                            _UxGT("Vmin")
#define MSG_VTRAV_MIN                       _UxGT("VTrav min")
#define MSG_AMAX                            _UxGT("Amax ")
#define MSG_A_RETRACT                       _UxGT("A-откат")
#define MSG_A_TRAVEL                        _UxGT("A-travel")
#define MSG_XSTEPS                          _UxGT("X стъпки/mm")
#define MSG_YSTEPS                          _UxGT("Y стъпки/mm")
#define MSG_ZSTEPS                          _UxGT("Z стъпки/mm")
#define MSG_ESTEPS                          _UxGT("E стъпки/mm")
#define MSG_E1STEPS                         _UxGT("E1 стъпки/mm")
#define MSG_E2STEPS                         _UxGT("E2 стъпки/mm")
#define MSG_E3STEPS                         _UxGT("E3 стъпки/mm")
#define MSG_E4STEPS                         _UxGT("E4 стъпки/mm")
#define MSG_TEMPERATURE                     _UxGT("Температура")
#define MSG_MOTION                          _UxGT("Движение")
#define MSG_VOLUMETRIC                      _UxGT("Нишка")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E in mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Диам. нишка")
#define MSG_CONTRAST                        _UxGT("LCD контраст")
#define MSG_STORE_EPROM                     _UxGT("Запази в EPROM")
#define MSG_LOAD_EPROM                      _UxGT("Зареди от EPROM")
#define MSG_RESTORE_FAILSAFE                _UxGT("Фабрични настройки")
#define MSG_REFRESH                         LCD_STR_REFRESH _UxGT("Обнови")
#define MSG_WATCH                           _UxGT("Преглед")
#define MSG_PREPARE                         _UxGT("Действия")
#define MSG_TUNE                            _UxGT("Настройка")
#define MSG_PAUSE_PRINT                     _UxGT("Пауза")
#define MSG_RESUME_PRINT                    _UxGT("Възобнови печата")
#define MSG_STOP_PRINT                      _UxGT("Спри печата")
#define MSG_CARD_MENU                       _UxGT("Меню карта")
#define MSG_NO_CARD                         _UxGT("Няма карта")
#define MSG_DWELL                           _UxGT("Почивка...")
#define MSG_USERWAIT                        _UxGT("Изчакване")
#define MSG_RESUMING                        _UxGT("Продълж. печата")
#define MSG_PRINT_ABORTED                   _UxGT("Печатът е прекъснат")
#define MSG_NO_MOVE                         _UxGT("Няма движение")
#define MSG_KILLED                          _UxGT("УБИТО.")
#define MSG_STOPPED                         _UxGT("СПРЯНО.")
#define MSG_CONTROL_RETRACT                 _UxGT("Откат mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Смяна Откат mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Откат  V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Скок mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Възврат mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Смяна Възврат mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Възврат  V")
#define MSG_AUTORETRACT                     _UxGT("Автоoткат")
#define MSG_FILAMENTCHANGE                  _UxGT("Смяна нишка")
#define MSG_INIT_SDCARD                     _UxGT("Иниц. SD-Карта")
#define MSG_CNG_SDCARD                      _UxGT("Смяна SD-Карта")
#define MSG_ZPROBE_OUT                      _UxGT("Z-сондата е извадена")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Self-Test")
#define MSG_BLTOUCH_RESET                   _UxGT("Reset BLTouch")
#define MSG_HOME                            _UxGT("Home")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("first")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Z Отстояние")
#define MSG_BABYSTEP_X                      _UxGT("Министъпка X")
#define MSG_BABYSTEP_Y                      _UxGT("Министъпка Y")
#define MSG_BABYSTEP_Z                      _UxGT("Министъпка Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Стоп Кр.Изключватели")
#define MSG_HEATING_FAILED_LCD              _UxGT("Heating failed")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Err: REDUNDANT TEMP")
#define MSG_THERMAL_RUNAWAY                 _UxGT("THERMAL RUNAWAY")
#define MSG_ERR_MAXTEMP                     _UxGT("Err: MAXTEMP")
#define MSG_ERR_MINTEMP                     _UxGT("Err: MINTEMP")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Err: MAXTEMP BED")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Err: MINTEMP BED")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Forbidden")
#define MSG_HALTED                          _UxGT("PRINTER HALTED")
#define MSG_PLEASE_RESET                    _UxGT("Please reset")
#define MSG_SHORT_DAY                       _UxGT("d") // One character only
#define MSG_SHORT_HOUR                      _UxGT("h") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("m") // One character only
#define MSG_HEATING                         _UxGT("Heating...")
#define MSG_HEATING_COMPLETE                _UxGT("Heating done.")
#define MSG_BED_HEATING                     _UxGT("Bed Heating.")
#define MSG_BED_DONE                        _UxGT("Bed done.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Делта Калибровка")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Калибровка X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Калибровка Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Калибровка Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Калибровка Център")

#define MSG_INFO_MENU                       _UxGT("About Printer")
#define MSG_INFO_PRINTER_MENU               _UxGT("Printer Info")
#define MSG_INFO_STATS_MENU                 _UxGT("Printer Stats")
#define MSG_INFO_BOARD_MENU                 _UxGT("Board Info")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Thermistors")
#define MSG_INFO_EXTRUDERS                  _UxGT("Extruders")
#define MSG_INFO_BAUDRATE                   _UxGT("Baud")
#define MSG_INFO_PROTOCOL                   _UxGT("Protocol")
#define MSG_LIGHTS_ON                       _UxGT("Case light on")
#define MSG_LIGHTS_OFF                      _UxGT("Case light off")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("Print Count")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total print time")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Longest job time")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruded total")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Prints")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Completed")
  #define MSG_INFO_PRINT_TIME               _UxGT("Total")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Longest")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Extruded")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Min Temp")
#define MSG_INFO_MAX_TEMP                   _UxGT("Max Temp")
#define MSG_INFO_PSU                        _UxGT("Power Supply")

#define MSG_DRIVE_STRENGTH                  _UxGT("Drive Strength")
#define MSG_DAC_PERCENT                     _UxGT("Driver %")
#define MSG_DAC_EEPROM_WRITE                _UxGT("DAC EEPROM Write")

#define MSG_FILAMENT_CHANGE_HEADER          _UxGT("CHANGE FILAMENT")
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("CHANGE OPTIONS:")
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Extrude more")
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Resume print")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Wait for start")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("of the filament")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("change")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Wait for")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("filament unload")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert filament")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("and press button")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("to continue...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Wait for")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("filament load")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Wait for")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("filament extrude")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Wait for print")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("to resume")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Please wait...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ejecting...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Insert and Click")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Loading...")
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Extruding...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Resuming...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_BG_H
