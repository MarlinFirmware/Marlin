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
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_UK_H
#define LANGUAGE_UK_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5

#define WELCOME_MSG                         MACHINE_NAME " готовий."
#define MSG_SD_INSERTED                     "Картка вставлена"
#define MSG_SD_REMOVED                      "Картка видалена"
#define MSG_LCD_ENDSTOPS                    "Кінцевик" // Max length 8 characters
#define MSG_MAIN                            "Меню"
#define MSG_AUTOSTART                       "Автостарт"
#define MSG_DISABLE_STEPPERS                "Вимк. двигуни"
#define MSG_AUTO_HOME                       "Авто паркування"
#define MSG_AUTO_HOME_X                     "Паркування X"
#define MSG_AUTO_HOME_Y                     "Паркування Y"
#define MSG_AUTO_HOME_Z                     "Паркування Z"
#define MSG_LEVEL_BED_HOMING                "Паркування XYZ"
#define MSG_LEVEL_BED_WAITING               "Почати"
#define MSG_LEVEL_BED_NEXT_POINT            "Слідуюча Точка"
#define MSG_LEVEL_BED_DONE                  "Завершено!"
#define MSG_LEVEL_BED_CANCEL                "Відміна"
#define MSG_SET_HOME_OFFSETS                "Зберегти паркув."
#define MSG_HOME_OFFSETS_APPLIED            "Зміщення застос."
#define MSG_SET_ORIGIN                      "Встанов. початок"
#define MSG_PREHEAT_1                       "Нагрів PLA"
#define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 " "
#define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 " Все"
#define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 " Стіл"
#define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 " нал."
#define MSG_PREHEAT_2                       "Нагрів ABS"
#define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 " "
#define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 " Все"
#define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 " Стіл"
#define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 " нал."
#define MSG_COOLDOWN                        "Охолодження"
#define MSG_SWITCH_PS_ON                    "Увімкнути живлення"
#define MSG_SWITCH_PS_OFF                   "Вимкнути живлення"
#define MSG_EXTRUDE                         "Екструзія"
#define MSG_RETRACT                         "Втягування"
#define MSG_MOVE_AXIS                       "Рух по осям"
#define MSG_LEVEL_BED                       "Нівелювання столу"
#define MSG_MOVE_X                          "Рух по X"
#define MSG_MOVE_Y                          "Рух по Y"
#define MSG_MOVE_Z                          "Рух по Z"
#define MSG_MOVE_E                          "Екструдер"
#define MSG_MOVE_01MM                       "Рух по 0.1mm"
#define MSG_MOVE_1MM                        "Рух по 1mm"
#define MSG_MOVE_10MM                       "Рух по 10mm"
#define MSG_SPEED                           "Швидкість"
#define MSG_BED_Z                           "Z Столу"
#define MSG_NOZZLE                          "Сопло"
#define MSG_BED                             "Стіл"
#define MSG_FAN_SPEED                       "Охолодж."
#define MSG_FLOW                            "Потік"
#define MSG_CONTROL                         "Налаштування"
#define MSG_MIN                             " " LCD_STR_THERMOMETER " Мін"
#define MSG_MAX                             " " LCD_STR_THERMOMETER " Макс"
#define MSG_FACTOR                          " " LCD_STR_THERMOMETER " Факт"
#define MSG_AUTOTEMP                        "Автотемпер."
#define MSG_ON                              "Увімк."
#define MSG_OFF                             "Вимк. "
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_SELECT                          "Вибрати"
#define MSG_ACC                             "Приск."
#define MSG_VX_JERK                         "Vx-ривок"
#define MSG_VY_JERK                         "Vy-ривок"
#define MSG_VZ_JERK                         "Vz-ривок"
#define MSG_VE_JERK                         "Ve-ривок"
#define MSG_VMAX                            "Vмакс"
#define MSG_VMIN                            "Vмін"
#define MSG_VTRAV_MIN                       "Vруху мін"
#define MSG_AMAX                            "Aмакс "
#define MSG_A_RETRACT                       "A-втягув."
#define MSG_A_TRAVEL                        "A-руху"
#define MSG_XSTEPS                          "Xкроків/мм"
#define MSG_YSTEPS                          "Yкроків/мм"
#define MSG_ZSTEPS                          "Zкроків/мм"
#define MSG_ESTEPS                          "Eкроків/мм"
#define MSG_TEMPERATURE                     "Температура"
#define MSG_MOTION                          "Рух"
#define MSG_VOLUMETRIC                      "Волокно"
#define MSG_VOLUMETRIC_ENABLED              "E в мм3"
#define MSG_FILAMENT_DIAM                   "Діам. волок."
#define MSG_CONTRAST                        "контраст LCD"
#define MSG_STORE_EPROM                     "Зберегти в ПЗП"
#define MSG_LOAD_EPROM                      "Зчитати з ПЗП"
#define MSG_RESTORE_FAILSAFE                "Відновити базові"
#define MSG_REFRESH                         "Поновити"
#define MSG_WATCH                           "Інформація"
#define MSG_PREPARE                         "Підготувати"
#define MSG_TUNE                            "Підлаштування"
#define MSG_PAUSE_PRINT                     "Призупинити друк"
#define MSG_RESUME_PRINT                    "Відновити друк"
#define MSG_STOP_PRINT                      "Скасувати друк"
#define MSG_CARD_MENU                       "Друкувати з SD"
#define MSG_NO_CARD                         "Відсутня SD карт."
#define MSG_DWELL                           "Сплячка..."
#define MSG_USERWAIT                        "Очікування дій..."
#define MSG_RESUMING                        "Відновлення друку"
#define MSG_PRINT_ABORTED                   "Друк скасовано"
#define MSG_NO_MOVE                         "Немає руху."
#define MSG_KILLED                          "ПЕРЕРВАНО. "
#define MSG_STOPPED                         "ЗУПИНЕНО. "
#define MSG_FILAMENTCHANGE                  "Зміна волокна"
#define MSG_INIT_SDCARD                     "Старт SD картки"
#define MSG_CNG_SDCARD                      "Заміна SD карти"
#define MSG_ZPROBE_OUT                      "Z дет. не в межах"
#define MSG_BLTOUCH_SELFTEST                "BLTouch Само-Тест"
#define MSG_BLTOUCH_RESET                   "Скинути BLTouch"
#define MSG_HOME                            "Дім"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "перший"
#define MSG_ZPROBE_ZOFFSET                  "Зміщення Z"
#define MSG_BABYSTEP_X                      "Мікрокрок X"
#define MSG_BABYSTEP_Y                      "Мікрокрок Y"
#define MSG_BABYSTEP_Z                      "Мікрокрок Z"
#define MSG_ENDSTOP_ABORT                   "невдача кінцевика"
#define MSG_HEATING_FAILED_LCD              "Невдалий нагрів"
#define MSG_THERMAL_RUNAWAY                 "ЗБІЙ ТЕМПЕРАТУРИ"
#define MSG_ERR_Z_HOMING                    "G28 Z Відмовлено"
#define MSG_HALTED                          "ПРИНТЕР ЗУПИНЕНО"
#define MSG_PLEASE_RESET                    "Перезавантажте"
#define MSG_SHORT_DAY                       "д" // One character only
#define MSG_SHORT_HOUR                      "г" // One character only
#define MSG_SHORT_MINUTE                    "х" // One character only
#define MSG_HEATING                         "Нагрівання..."
#define MSG_HEATING_COMPLETE                "Нагріто."
#define MSG_BED_HEATING                     "Нагрівання столу."
#define MSG_BED_DONE                        "Стіл нагрітий."
#define MSG_DELTA_CALIBRATE                 "Калібр. Delta"
#define MSG_DELTA_CALIBRATE_X               "Калібрування X"
#define MSG_DELTA_CALIBRATE_Y               "Калібрування Y"
#define MSG_DELTA_CALIBRATE_Z               "Калібрування Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Калібр. Центру"

#define MSG_INFO_MENU                       "Про принтер"
#define MSG_INFO_PRINTER_MENU               "Інформація"
#define MSG_INFO_STATS_MENU                 "Статистика"
#define MSG_INFO_BOARD_MENU                 "Про плату"
#define MSG_INFO_THERMISTOR_MENU            "Термістори"
#define MSG_INFO_EXTRUDERS                  "Екструдери"
#define MSG_INFO_BAUDRATE                   "біт/с"
#define MSG_INFO_PROTOCOL                   "Протокол"
#define MSG_LIGHTS_ON                       "Підсвітка увік."
#define MSG_LIGHTS_OFF                      "Підсвітка вимк."

#if LCD_WIDTH >= 20
  #define MSG_INFO_PRINT_COUNT              "К-сть друків"
  #define MSG_INFO_COMPLETED_PRINTS         "Завершено"
  #define MSG_INFO_PRINT_TIME               "Весь час друку"
  #define MSG_INFO_PRINT_LONGEST            "Найдовший час"
  #define MSG_INFO_PRINT_FILAMENT           "Екструдовано"
#else
  #define MSG_INFO_PRINT_COUNT              "Друків"
  #define MSG_INFO_COMPLETED_PRINTS         "Завершено"
  #define MSG_INFO_PRINT_TIME               "Загалом"
  #define MSG_INFO_PRINT_LONGEST            "Найдовший"
  #define MSG_INFO_PRINT_FILAMENT           "Ексдруд."
#endif

#define MSG_INFO_MIN_TEMP                   "Мін Темп."
#define MSG_INFO_MAX_TEMP                   "Макс Темп."
#define MSG_INFO_PSU                        "Джерело жив."

#define MSG_DRIVE_STRENGTH                  "Сила мотору"
#define MSG_DAC_PERCENT                     "% мотору"
#define MSG_DAC_EEPROM_WRITE                "Запис ЦАП на ПЗП"

#define MSG_FILAMENT_CHANGE_HEADER          "ЗАМІНА ВОЛОКНА"
#define MSG_FILAMENT_CHANGE_OPTION_HEADER   "НАЛАШТ. ЗАМІНИ:"
#define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  "Екструдувати"
#define MSG_FILAMENT_CHANGE_OPTION_RESUME   "Відновити друк"

#if LCD_HEIGHT >= 4
  // Up to 3 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Зачекайте на"
  #define MSG_FILAMENT_CHANGE_INIT_2          "початок заміни"
  #define MSG_FILAMENT_CHANGE_INIT_3          "волокна"
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Зачекайте на"
  #define MSG_FILAMENT_CHANGE_UNLOAD_2        "вивід волокна"
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Вставте волокно"
  #define MSG_FILAMENT_CHANGE_INSERT_2        "та натисніть для"
  #define MSG_FILAMENT_CHANGE_INSERT_3        "продовження..."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Зачекайте на"
  #define MSG_FILAMENT_CHANGE_LOAD_2          "ввід волокна"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Зачекайте на"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_2       "екструзію"
  #define MSG_FILAMENT_CHANGE_EXTRUDE_3       "волокна"
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Зачекайте на"
  #define MSG_FILAMENT_CHANGE_RESUME_2        "відновлення"
  #define MSG_FILAMENT_CHANGE_RESUME_3        "друку"
#else // LCD_HEIGHT < 4
  // Up to 2 lines allowed
  #define MSG_FILAMENT_CHANGE_INIT_1          "Зачекайте..."
  #define MSG_FILAMENT_CHANGE_UNLOAD_1        "Вивід..."
  #define MSG_FILAMENT_CHANGE_INSERT_1        "Вставте і нат."
  #define MSG_FILAMENT_CHANGE_LOAD_1          "Ввід..."
  #define MSG_FILAMENT_CHANGE_EXTRUDE_1       "Екстузія..."
  #define MSG_FILAMENT_CHANGE_RESUME_1        "Відновлення..."
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_UK_H
