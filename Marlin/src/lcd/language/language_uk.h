/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Ukrainian
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_5

namespace Language_uk {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Ukranian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" готовий.");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Картка вставлена");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Картка видалена");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Кінцевик"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Меню");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Автостарт");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Вимк. двигуни");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Авто паркування");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Паркування X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Паркування Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Паркування Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Паркування XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Почати");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Слідуюча Точка");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Завершено!");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Зберегти паркув.");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Зміщення застос.");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Встанов. початок");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Нагрів ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Нагрів ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" Сопло");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" Сопло ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" Все");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" Стіл");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" нал.");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Нагрів ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Нагрів ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Нагрів ") PREHEAT_2_LABEL _UxGT(" Сопло");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Нагрів ") PREHEAT_2_LABEL _UxGT(" Сопло ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Нагрів ") PREHEAT_2_LABEL _UxGT(" Все");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Нагрів ") PREHEAT_2_LABEL _UxGT(" Стіл");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Нагрів ") PREHEAT_2_LABEL _UxGT(" нал.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Охолодження");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Увімкнути живлення");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Вимкнути живлення");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Екструзія");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Втягування");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Рух по осям");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Нівелювання столу");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Нівелювання столу");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Рух по X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Рух по Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Рух по Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Екструдер");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Екструдер *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Рух по %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Рух по 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Рух по 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Рух по 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Швидкість");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z Столу");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Сопло");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Сопло ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Стіл");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Охолодж.");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Охолодж. =");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Потік");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Потік ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Налаштування");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Мін");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Макс");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Факт");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Автотемпер.");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Увімк.");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Вимк.");
  PROGMEM Language_Str MSG_PID_P                           = _UxGT("PID-P");
  PROGMEM Language_Str MSG_PID_P_E                         = _UxGT("PID-P *");
  PROGMEM Language_Str MSG_PID_I                           = _UxGT("PID-I");
  PROGMEM Language_Str MSG_PID_I_E                         = _UxGT("PID-I *");
  PROGMEM Language_Str MSG_PID_D                           = _UxGT("PID-D");
  PROGMEM Language_Str MSG_PID_D_E                         = _UxGT("PID-D *");
  PROGMEM Language_Str MSG_PID_C                           = _UxGT("PID-C");
  PROGMEM Language_Str MSG_PID_C_E                         = _UxGT("PID-C *");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Вибрати");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Вибрати *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Приск.");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Ривок");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-ривок");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-ривок");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-ривок");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-ривок");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vмакс") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vмакс") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vмакс") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vмакс") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vмакс *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vмін");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vруху мін");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Aмакс ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Aмакс ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Aмакс ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Aмакс ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Aмакс *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-втягув.");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-руху");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Кроків/мм");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("кроків/мм");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("кроків/мм");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("кроків/мм");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Eкроків/мм");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*кроків/мм");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Температура");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Рух");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Волокно");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E в мм³");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Діам. волок.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Діам. волок. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("контраст LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Зберегти в ПЗП");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Зчитати з ПЗП");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Відновити базові");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Поновити");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Інформація");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Підготувати");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Підлаштування");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Призупинити друк");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Відновити друк");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Скасувати друк");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Друкувати з SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Відсутня SD карт.");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Сплячка...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Очікування дій...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Друк скасовано");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Немає руху.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ПЕРЕРВАНО. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ЗУПИНЕНО. ");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Зміна волокна");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Зміна волокна *");
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Старт SD картки");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Заміна SD карти");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z дет. не в межах");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Само-Тест");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Скинути BLTouch");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Дім %s%s%s перший");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Зміщення Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Мікрокрок X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Мікрокрок Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Мікрокрок Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("невдача кінцевика");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Невдалий нагрів");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ЗБІЙ ТЕМПЕРАТУРИ");
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Дім XY перший");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("ПРИНТЕР ЗУПИНЕНО");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Перезавантажте");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("д"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("г"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("х"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Нагрівання...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Нагрівання столу...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Калібр. Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Калібрування X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Калібрування Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Калібрування Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Калібр. Центру");

  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Про принтер");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Інформація");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Статистика");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Про плату");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Термістори");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Екструдери");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("біт/с");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Протокол");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Підсвітка");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Неправильний принтер");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("К-сть друків");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Завершено");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Весь час друку");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Найдовший час");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Екструдовано");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Друків");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Завершено");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Загалом");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Найдовший");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Ексдруд.");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Мін Темп.");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Макс Темп.");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Джерело жив.");

  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Сила мотору");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X % мотору");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y % мотору");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z % мотору");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E % мотору");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Запис ЦАП на ПЗП");

  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Відновити друк");

  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Зачекайте на", "початок заміни", "волокна"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Зачекайте на", "вивід волокна"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Вставте волокно", "та натисніть для", "продовження..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Зачекайте на", "ввід волокна"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Зачекайте на", "відновлення", "друку"));
  #else
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Зачекайте..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Вивід..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Вставте і нат."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Ввід..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Відновлення..."));
  #endif
}
