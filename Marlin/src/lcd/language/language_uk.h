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
 * Ukrainian
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_UK_H
#define LANGUAGE_UK_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5
#define CHARSIZE 2

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" готовий.")
#define MSG_SD_INSERTED                     _UxGT("Картка вставлена")
#define MSG_SD_REMOVED                      _UxGT("Картка видалена")
#define MSG_LCD_ENDSTOPS                    _UxGT("Кінцевик") // Max length 8 characters
#define MSG_MAIN                            _UxGT("Меню")
#define MSG_AUTOSTART                       _UxGT("Автостарт")
#define MSG_DISABLE_STEPPERS                _UxGT("Вимк. двигуни")
#define MSG_AUTO_HOME                       _UxGT("Авто паркування")
#define MSG_AUTO_HOME_X                     _UxGT("Паркування X")
#define MSG_AUTO_HOME_Y                     _UxGT("Паркування Y")
#define MSG_AUTO_HOME_Z                     _UxGT("Паркування Z")
#define MSG_LEVEL_BED_HOMING                _UxGT("Паркування XYZ")
#define MSG_LEVEL_BED_WAITING               _UxGT("Почати")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Слідуюча Точка")
#define MSG_LEVEL_BED_DONE                  _UxGT("Завершено!")
#define MSG_SET_HOME_OFFSETS                _UxGT("Зберегти паркув.")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Зміщення застос.")
#define MSG_SET_ORIGIN                      _UxGT("Встанов. початок")
#define MSG_PREHEAT_1                       _UxGT("Нагрів PLA")
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" Все")
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" Стіл")
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" нал.")
#define MSG_PREHEAT_2                       _UxGT("Нагрів ABS")
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" Все")
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" Стіл")
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" нал.")
#define MSG_COOLDOWN                        _UxGT("Охолодження")
#define MSG_SWITCH_PS_ON                    _UxGT("Увімкнути живлення")
#define MSG_SWITCH_PS_OFF                   _UxGT("Вимкнути живлення")
#define MSG_EXTRUDE                         _UxGT("Екструзія")
#define MSG_RETRACT                         _UxGT("Втягування")
#define MSG_MOVE_AXIS                       _UxGT("Рух по осям")
#define MSG_BED_LEVELING                    _UxGT("Нівелювання столу")
#define MSG_LEVEL_BED                       _UxGT("Нівелювання столу")
#define MSG_MOVE_X                          _UxGT("Рух по X")
#define MSG_MOVE_Y                          _UxGT("Рух по Y")
#define MSG_MOVE_Z                          _UxGT("Рух по Z")
#define MSG_MOVE_E                          _UxGT("Екструдер")
#define MSG_MOVE_01MM                       _UxGT("Рух по 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Рух по 1mm")
#define MSG_MOVE_10MM                       _UxGT("Рух по 10mm")
#define MSG_SPEED                           _UxGT("Швидкість")
#define MSG_BED_Z                           _UxGT("Z Столу")
#define MSG_NOZZLE                          _UxGT("Сопло")
#define MSG_BED                             _UxGT("Стіл")
#define MSG_FAN_SPEED                       _UxGT("Охолодж.")
#define MSG_FLOW                            _UxGT("Потік")
#define MSG_CONTROL                         _UxGT("Налаштування")
#define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Мін")
#define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Макс")
#define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Факт")
#define MSG_AUTOTEMP                        _UxGT("Автотемпер.")
#define MSG_ON                              _UxGT("Увімк.")
#define MSG_OFF                             _UxGT("Вимк. ")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_SELECT                          _UxGT("Вибрати")
#define MSG_ACC                             _UxGT("Приск.")
#define MSG_JERK                            _UxGT("Ривок")
#if IS_KINEMATIC
  #define MSG_VA_JERK                       _UxGT("Va-ривок")
  #define MSG_VB_JERK                       _UxGT("Vb-ривок")
  #define MSG_VC_JERK                       _UxGT("Vc-ривок")
#else
  #define MSG_VA_JERK                       _UxGT("Vx-ривок")
  #define MSG_VB_JERK                       _UxGT("Vy-ривок")
  #define MSG_VC_JERK                       _UxGT("Vz-ривок")
#endif
#define MSG_VE_JERK                         _UxGT("Ve-ривок")
#define MSG_VMAX                            _UxGT("Vмакс")
#define MSG_VMIN                            _UxGT("Vмін")
#define MSG_VTRAV_MIN                       _UxGT("Vруху мін")
#define MSG_AMAX                            _UxGT("Aмакс ")
#define MSG_A_RETRACT                       _UxGT("A-втягув.")
#define MSG_A_TRAVEL                        _UxGT("A-руху")
#define MSG_STEPS_PER_MM                    _UxGT("Кроків/мм")
#if IS_KINEMATIC
  #define MSG_ASTEPS                        _UxGT("Aкроків/мм")
  #define MSG_BSTEPS                        _UxGT("Bкроків/мм")
  #define MSG_CSTEPS                        _UxGT("Cкроків/мм")
#else
  #define MSG_ASTEPS                        _UxGT("Xкроків/мм")
  #define MSG_BSTEPS                        _UxGT("Yкроків/мм")
  #define MSG_CSTEPS                        _UxGT("Zкроків/мм")
#endif
#define MSG_ESTEPS                          _UxGT("Eкроків/мм")
#define MSG_E1STEPS                         _UxGT("E1кроків/мм")
#define MSG_E2STEPS                         _UxGT("E2кроків/мм")
#define MSG_E3STEPS                         _UxGT("E3кроків/мм")
#define MSG_E4STEPS                         _UxGT("E4кроків/мм")
#define MSG_E5STEPS                         _UxGT("E5кроків/мм")
#define MSG_TEMPERATURE                     _UxGT("Температура")
#define MSG_MOTION                          _UxGT("Рух")
#define MSG_FILAMENT                        _UxGT("Волокно")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E в мм3")
#define MSG_FILAMENT_DIAM                   _UxGT("Діам. волок.")
#define MSG_CONTRAST                        _UxGT("контраст LCD")
#define MSG_STORE_EEPROM                    _UxGT("Зберегти в ПЗП")
#define MSG_LOAD_EEPROM                     _UxGT("Зчитати з ПЗП")
#define MSG_RESTORE_FAILSAFE                _UxGT("Відновити базові")
#define MSG_REFRESH                         _UxGT("Поновити")
#define MSG_WATCH                           _UxGT("Інформація")
#define MSG_PREPARE                         _UxGT("Підготувати")
#define MSG_TUNE                            _UxGT("Підлаштування")
#define MSG_PAUSE_PRINT                     _UxGT("Призупинити друк")
#define MSG_RESUME_PRINT                    _UxGT("Відновити друк")
#define MSG_STOP_PRINT                      _UxGT("Скасувати друк")
#define MSG_CARD_MENU                       _UxGT("Друкувати з SD")
#define MSG_NO_CARD                         _UxGT("Відсутня SD карт.")
#define MSG_DWELL                           _UxGT("Сплячка...")
#define MSG_USERWAIT                        _UxGT("Очікування дій...")
#define MSG_RESUMING                        _UxGT("Відновлення друку")
#define MSG_PRINT_ABORTED                   _UxGT("Друк скасовано")
#define MSG_NO_MOVE                         _UxGT("Немає руху.")
#define MSG_KILLED                          _UxGT("ПЕРЕРВАНО. ")
#define MSG_STOPPED                         _UxGT("ЗУПИНЕНО. ")
#define MSG_FILAMENTCHANGE                  _UxGT("Зміна волокна")
#define MSG_INIT_SDCARD                     _UxGT("Старт SD картки")
#define MSG_CNG_SDCARD                      _UxGT("Заміна SD карти")
#define MSG_ZPROBE_OUT                      _UxGT("Z дет. не в межах")
#define MSG_BLTOUCH_SELFTEST                _UxGT("BLTouch Само-Тест")
#define MSG_BLTOUCH_RESET                   _UxGT("Скинути BLTouch")
#define MSG_HOME                            _UxGT("Дім")  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           _UxGT("перший")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Зміщення Z")
#define MSG_BABYSTEP_X                      _UxGT("Мікрокрок X")
#define MSG_BABYSTEP_Y                      _UxGT("Мікрокрок Y")
#define MSG_BABYSTEP_Z                      _UxGT("Мікрокрок Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("невдача кінцевика")
#define MSG_HEATING_FAILED_LCD              _UxGT("Невдалий нагрів")
#define MSG_THERMAL_RUNAWAY                 _UxGT("ЗБІЙ ТЕМПЕРАТУРИ")
#define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Відмовлено")
#define MSG_HALTED                          _UxGT("ПРИНТЕР ЗУПИНЕНО")
#define MSG_PLEASE_RESET                    _UxGT("Перезавантажте")
#define MSG_SHORT_DAY                       _UxGT("д") // One character only
#define MSG_SHORT_HOUR                      _UxGT("г") // One character only
#define MSG_SHORT_MINUTE                    _UxGT("х") // One character only
#define MSG_HEATING                         _UxGT("Нагрівання...")
#define MSG_HEATING_COMPLETE                _UxGT("Нагріто.")
#define MSG_BED_HEATING                     _UxGT("Нагрівання столу.")
#define MSG_BED_DONE                        _UxGT("Стіл нагрітий.")
#define MSG_DELTA_CALIBRATE                 _UxGT("Калібр. Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Калібрування X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Калібрування Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Калібрування Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Калібр. Центру")

#define MSG_INFO_MENU                       _UxGT("Про принтер")
#define MSG_INFO_PRINTER_MENU               _UxGT("Інформація")
#define MSG_INFO_STATS_MENU                 _UxGT("Статистика")
#define MSG_INFO_BOARD_MENU                 _UxGT("Про плату")
#define MSG_INFO_THERMISTOR_MENU            _UxGT("Термістори")
#define MSG_INFO_EXTRUDERS                  _UxGT("Екструдери")
#define MSG_INFO_BAUDRATE                   _UxGT("біт/с")
#define MSG_INFO_PROTOCOL                   _UxGT("Протокол")
#define MSG_CASE_LIGHT                      _UxGT("Підсвітка")

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              _UxGT("К-сть друків")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Завершено")
  #define MSG_INFO_PRINT_TIME               _UxGT("Весь час друку")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Найдовший час")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Екструдовано")
#else
  #define MSG_INFO_PRINT_COUNT              _UxGT("Друків")
  #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Завершено")
  #define MSG_INFO_PRINT_TIME               _UxGT("Загалом")
  #define MSG_INFO_PRINT_LONGEST            _UxGT("Найдовший")
  #define MSG_INFO_PRINT_FILAMENT           _UxGT("Ексдруд.")
#endif

#define MSG_INFO_MIN_TEMP                   _UxGT("Мін Темп.")
#define MSG_INFO_MAX_TEMP                   _UxGT("Макс Темп.")
#define MSG_INFO_PSU                        _UxGT("Джерело жив.")

#define MSG_DRIVE_STRENGTH                  _UxGT("Сила мотору")
#define MSG_DAC_PERCENT                     _UxGT("% мотору")
#define MSG_DAC_EEPROM_WRITE                _UxGT("Запис ЦАП на ПЗП")

#define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Відновити друк")

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Зачекайте на")
  #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("початок заміни")
  #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("волокна")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Зачекайте на")
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("вивід волокна")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Вставте волокно")
  #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("та натисніть для")
  #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("продовження...")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Зачекайте на")
  #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("ввід волокна")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Зачекайте на")
  #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("відновлення")
  #define MSG_FILAMENT_CHANGE_RESUME_3        _UxGT("друку")
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Зачекайте...")
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Вивід...")
  #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Вставте і нат.")
  #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Ввід...")
  #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Відновлення...")
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_UK_H
