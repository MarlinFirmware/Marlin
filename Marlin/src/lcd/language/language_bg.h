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

/**
 * Bulgarian
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */

#define DISPLAY_CHARSET_ISO10646_5

namespace Language_bg {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Bulgarian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Готов.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Картата е поставена");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Картата е извадена");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Меню");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Автостарт");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Изкл. двигатели");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Паркиране");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Задай Начало");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Изходна точка");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Подгряване ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Подгряване ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Подгряване ") PREHEAT_1_LABEL _UxGT(" Дюза");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Подгряване ") PREHEAT_1_LABEL _UxGT(" Дюза ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Подгр. ") PREHEAT_1_LABEL _UxGT(" Всички");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Подгр. ") PREHEAT_1_LABEL _UxGT(" Легло");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Настройки ") PREHEAT_1_LABEL;

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Подгряване $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Подгряване $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Подгряване $ Дюза");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Подгряване $ Дюза ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Подгр. $ Всички");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Подгр. $ Легло");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Настройки $");
  #endif
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Охлаждане");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Вкл. захранване");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Изкл. захранване");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Екструзия");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Откат");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Движение по ос");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Нивелиране");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Нивелиране");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Движение по X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Движение по Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Движение по Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Екструдер");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Екструдер *");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Премести с %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Премести с 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Премести с 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Премести с 10mm");
  PROGMEM Language_Str MSG_MOVE_100MM                      = _UxGT("Премести с 100mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Скорост");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Bed Z");
  PROGMEM Language_Str MSG_NOZZLE                          = " " LCD_STR_THERMOMETER _UxGT(" Дюза");
  PROGMEM Language_Str MSG_NOZZLE_N                        = " " LCD_STR_THERMOMETER _UxGT(" Дюза ~");
  PROGMEM Language_Str MSG_BED                             = " " LCD_STR_THERMOMETER _UxGT(" Легло");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Вентилатор");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Вентилатор ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Поток");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Поток ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Управление");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Минимум");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Максимум");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Фактор");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Авто-темп.");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Вкл.");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Изкл.");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-откат");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-travel");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Стъпки/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("стъпки/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("стъпки/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("стъпки/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E стъпки/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* стъпки/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Температура");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Движение");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Нишка");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Диам. нишка");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Диам. нишка *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD контраст");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Запази в EPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Зареди от EPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Фабрични настройки");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH _UxGT("Обнови");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Преглед");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Действия");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Настройка");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Пауза");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Възобнови печата");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Спри печата");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Меню карта");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Няма карта");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Почивка...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Изчакване");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Печатът е прекъснат");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Няма движение");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("УБИТО.");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("СПРЯНО.");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Откат mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Смяна Откат mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Откат  V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Скок mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Възврат mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Смяна Възврат mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Възврат  V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Автоoткат");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Смяна нишка");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Смяна нишка *");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Иниц. SD-Карта");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Смяна SD-Карта");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z-сондата е извадена");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Z Отстояние");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Министъпка X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Министъпка Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Министъпка Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Стоп Кр.Изключватели");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Делта Калибровка");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Калибровка X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Калибровка Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Калибровка Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Калибровка Център");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Неправилен принтер");
}
