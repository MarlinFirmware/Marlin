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
 * Russian
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_RU_H
#define LANGUAGE_RU_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5

#define WELCOME_MSG                         MACHINE_NAME " Готов."
#define MSG_SD_INSERTED                     "Карта вставлена"
#define MSG_SD_REMOVED                      "Карта извлечена"
#define MSG_MAIN                            "Меню"
#define MSG_LCD_ENDSTOPS                    "концевик"
#define MSG_AUTOSTART                       "Автостарт"
#define MSG_DISABLE_STEPPERS                "Выкл. двигатели"
#define MSG_AUTO_HOME                       "Парковка"
#define MSG_LEVEL_BED_HOMING                "Нулевое полож"
#define MSG_LEVEL_BED_WAITING               "Нажмите начать"
#define MSG_LEVEL_BED_NEXT_POINT            "Следующая точка"
#define MSG_LEVEL_BED_DONE                  "Уровень!"
#define MSG_LEVEL_BED_CANCEL                "Отменить"
#define MSG_SET_HOME_OFFSETS                "Запомнить парковку"
#define MSG_HOME_OFFSETS_APPLIED            "Коррекции примен"
#define MSG_SET_ORIGIN                      "Запомнить ноль"
#define MSG_PREHEAT_1                       "Преднагрев PLA"
#define MSG_PREHEAT_1_N                     "Греть PLA Сопло "
#define MSG_PREHEAT_1_ALL                   "Греть PLA Все"
#define MSG_PREHEAT_1_BEDONLY               "Греть PLA Стол"
#define MSG_PREHEAT_1_SETTINGS              "Настройки PLA"
#define MSG_PREHEAT_2                       "Преднагрев ABS"
#define MSG_PREHEAT_2_N                     "Греть ABS Сопло "
#define MSG_PREHEAT_2_ALL                   "Греть ABS Все"
#define MSG_PREHEAT_2_BEDONLY               "Греть ABS Стол"
#define MSG_PREHEAT_2_SETTINGS              "Настройки ABS"
#define MSG_COOLDOWN                        "Охлаждение"
#define MSG_SWITCH_PS_ON                    "Включить Питание"
#define MSG_SWITCH_PS_OFF                   "Отключить Питание"
#define MSG_EXTRUDE                         "Экструзия"
#define MSG_RETRACT                         "Втягивание"
#define MSG_MOVE_AXIS                       "Движение по осям"
#define MSG_LEVEL_BED                       "Калибровать стол"
#define MSG_MOVE_X                          "Движение по X"
#define MSG_MOVE_Y                          "Движение по Y"
#define MSG_MOVE_Z                          "Движение по Z"
#define MSG_MOVE_E                          "Экструдер"
#define MSG_MOVE_01MM                       "Движение XYZ 0.1mm"
#define MSG_MOVE_1MM                        "Движение XYZ 1mm"
#define MSG_MOVE_10MM                       "Движение XY 10mm"
#define MSG_SPEED                           "Скорость"
#define MSG_BED_Z                           "Z стола"
#define MSG_NOZZLE                          LCD_STR_THERMOMETER " Сопло"
#define MSG_BED                             LCD_STR_THERMOMETER " Стол"
#define MSG_FAN_SPEED                       "Кулер"
#define MSG_FLOW                            "Поток"
#define MSG_CONTROL                         "Настройки"
#define MSG_MIN                             LCD_STR_THERMOMETER " Минимум"
#define MSG_MAX                             LCD_STR_THERMOMETER " Максимум"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Фактор"
#define MSG_AUTOTEMP                        "Автотемпература"
#define MSG_ON                              "Вкл. "
#define MSG_OFF                             "Откл. "
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Acc"
#define MSG_VX_JERK                         "Vx-рывок"
#define MSG_VY_JERK                         "Vy-рывок"
#define MSG_VZ_JERK                         "Vz-рывок"
#define MSG_VE_JERK                         "Ve-рывок"
#define MSG_VMAX                            "Vмакс "
#define MSG_VMIN                            "Vмин"
#define MSG_VTRAV_MIN                       "Vпутеш. мин"
#define MSG_AMAX                            "Aмакс"
#define MSG_A_RETRACT                       "A-втягивание"
#define MSG_A_TRAVEL                        "A-путеш."
#define MSG_XSTEPS                          "X шаг/мм"
#define MSG_YSTEPS                          "Y шаг/мм"
#define MSG_ZSTEPS                          "Z шаг/мм"
#define MSG_ESTEPS                          "E шаг/мм"
#define MSG_E1STEPS                         "E1 шаг/мм"
#define MSG_E2STEPS                         "E2 шаг/мм"
#define MSG_E3STEPS                         "E3 шаг/мм"
#define MSG_E4STEPS                         "E4 шаг/мм"
#define MSG_TEMPERATURE                     "Температура"
#define MSG_MOTION                          "Механика"
#define MSG_VOLUMETRIC                      "Пруток"
#define MSG_VOLUMETRIC_ENABLED              "E в mm3"
#define MSG_FILAMENT_DIAM                   "Диаметр прутка"
#define MSG_CONTRAST                        "Контраст LCD"
#define MSG_STORE_EPROM                     "Сохранить в EEPROM"
#define MSG_LOAD_EPROM                      "Считать из EEPROM"
#define MSG_RESTORE_FAILSAFE                "Сброс EEPROM"
#define MSG_REFRESH                         "Обновить"
#define MSG_WATCH                           "Обзор"
#define MSG_PREPARE                         "Действия"
#define MSG_TUNE                            "Настройки"
#define MSG_PAUSE_PRINT                     "Пауза печати"
#define MSG_RESUME_PRINT                    "Продолжить печать"
#define MSG_STOP_PRINT                      "Остановить печать"
#define MSG_CARD_MENU                       "Обзор карты"
#define MSG_NO_CARD                         "Нет карты"
#define MSG_DWELL                           "Сон..."
#define MSG_USERWAIT                        "Ожиданиие"
#define MSG_RESUMING                        "Возобновление..."
#define MSG_PRINT_ABORTED                   "Отмена печати"
#define MSG_NO_MOVE                         "Нет движения."
#define MSG_KILLED                          "УБИТО."
#define MSG_STOPPED                         "ОСТАНОВЛЕНО."
#define MSG_CONTROL_RETRACT                 "Втягивание mm"
#define MSG_CONTROL_RETRACT_SWAP            "Втяг. смены mm"
#define MSG_CONTROL_RETRACTF                "Втягивание V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Втяг. прыжка mm"
#define MSG_CONTROL_RETRACT_RECOVER         "Возврат +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Возврат смены +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "Возврат  V"
#define MSG_AUTORETRACT                     "Авто Втягивание"
#define MSG_FILAMENTCHANGE                  "Смена прутка"
#define MSG_INIT_SDCARD                     "Иниц. карту"
#define MSG_CNG_SDCARD                      "Сменить карту"
#define MSG_ZPROBE_OUT                      "Z датчик вне стола"
#define MSG_HOME                            "Паркуй X/Y перед Z"
#define MSG_FIRST                           "первый"
#define MSG_ZPROBE_ZOFFSET                  "Смещение Z"
#define MSG_BABYSTEP_X                      "Микрошаг X"
#define MSG_BABYSTEP_Y                      "Микрошаг Y"
#define MSG_BABYSTEP_Z                      "Микрошаг Z"
#define MSG_ENDSTOP_ABORT                   "Сработал концевик"
#define MSG_HEATING_FAILED_LCD              "Разогрев не удался"
#define MSG_ERR_REDUNDANT_TEMP              "Ошибка:Слишком горячо"
#define MSG_THERMAL_RUNAWAY                 "ТЕПЛО УБЕГАНИЯ!"
#define MSG_ERR_MAXTEMP                     "Ошибка: Т макс."
#define MSG_ERR_MINTEMP                     "Ошибка: Т мин."
#define MSG_ERR_MAXTEMP_BED                 "Ошибка:Т макс.стол"
#define MSG_ERR_MINTEMP_BED                 "Ошибка:Т мин.стол"
#define MSG_HEATING                         "Нагреваю сопло..."
#define MSG_HEATING_COMPLETE                "Нагрев выполнен"
#define MSG_BED_HEATING                     "Нагреваю стол"
#define MSG_BED_DONE                        "Стол разогрет"
#define MSG_DELTA_CALIBRATE                 "Калибровка Delta"
#define MSG_DELTA_CALIBRATE_X               "Калибровать X"
#define MSG_DELTA_CALIBRATE_Y               "Калибровать Y"
#define MSG_DELTA_CALIBRATE_Z               "Калибровать Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Калибровать центр"

#endif // LANGUAGE_RU_H
