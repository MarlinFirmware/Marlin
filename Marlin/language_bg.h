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
// Define SIMULATE_ROMFONT to see what is seen on the character based display defined in Configuration.h
//#define SIMULATE_ROMFONT
#define DISPLAY_CHARSET_ISO10646_5

#define WELCOME_MSG                         MACHINE_NAME " Готов."
#define MSG_SD_INSERTED                     "Картата е поставена"
#define MSG_SD_REMOVED                      "Картата е извадена"
#define MSG_MAIN                            "Меню"
#define MSG_AUTOSTART                       "Автостарт"
#define MSG_DISABLE_STEPPERS                "Изкл. двигатели"
#define MSG_AUTO_HOME                       "Паркиране"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_SET_HOME_OFFSETS                "Задай Начало"
#define MSG_SET_ORIGIN                      "Изходна точка"
#define MSG_PREHEAT_PLA                     "Подгряване PLA"
#define MSG_PREHEAT_PLA_N                   "Подгряване PLA"
#define MSG_PREHEAT_PLA_ALL                 "Подгр. PLA Всички"
#define MSG_PREHEAT_PLA_BEDONLY             "Подгр. PLA Легло"
#define MSG_PREHEAT_PLA_SETTINGS            "Настройки PLA"
#define MSG_PREHEAT_ABS                     "Подгряване ABS"
#define MSG_PREHEAT_ABS_N                   "Подгряване ABS"
#define MSG_PREHEAT_ABS_ALL                 "Подгр. ABS Всички"
#define MSG_PREHEAT_ABS_BEDONLY             "Подгр. ABS Легло"
#define MSG_PREHEAT_ABS_SETTINGS            "Настройки ABS"
#define MSG_COOLDOWN                        "Охлаждане"
#define MSG_SWITCH_PS_ON                    "Вкл. захранване"
#define MSG_SWITCH_PS_OFF                   "Изкл. захранване"
#define MSG_EXTRUDE                         "Екструзия"
#define MSG_RETRACT                         "Откат"
#define MSG_MOVE_AXIS                       "Движение по ос"
#define MSG_MOVE_X                          "Движение по X"
#define MSG_MOVE_Y                          "Движение по Y"
#define MSG_MOVE_Z                          "Движение по Z"
#define MSG_MOVE_E                          "Екструдер"
#define MSG_MOVE_01MM                       "Премести с 0.1mm"
#define MSG_MOVE_1MM                        "Премести с 1mm"
#define MSG_MOVE_10MM                       "Премести с 10mm"
#define MSG_LEVEL_BED                       "Нивелиране"
#define MSG_SPEED                           "Скорост"
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
#define MSG_ACC                             "Acc"
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_X                               "x"
#define MSG_Y                               "y"
#define MSG_Z                               "z"
#define MSG_E                               "e"
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VTrav min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-откат"
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
#define MSG_YX_UNHOMED                      "Задайте X/Y преди Z"
#define MSG_ZPROBE_ZOFFSET                  "Z Отстояние"
#define MSG_BABYSTEP_X                      "Министъпка X"
#define MSG_BABYSTEP_Y                      "Министъпка Y"
#define MSG_BABYSTEP_Z                      "Министъпка Z"
#define MSG_ENDSTOP_ABORT                   "Стоп Кр.Изключватели"
#define MSG_END_HOUR                        "часа"
#define MSG_END_MINUTE                      "минути"

#if ENABLED(DELTA_CALIBRATION_MENU)
  #define MSG_DELTA_CALIBRATE               "Делта Калибровка"
  #define MSG_DELTA_CALIBRATE_X             "Калибровка X"
  #define MSG_DELTA_CALIBRATE_Y             "Калибровка Y"
  #define MSG_DELTA_CALIBRATE_Z             "Калибровка Z"
  #define MSG_DELTA_CALIBRATE_CENTER        "Калибровка Център"
#endif // DELTA_CALIBRATION_MENU

#endif // LANGUAGE_BG_H
