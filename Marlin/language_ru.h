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

#define WELCOME_MSG                         MACHINE_NAME _UxGT(" Готов.")
#define MSG_SD_INSERTED                     _UxGT("Карта вставлена")
#define MSG_SD_REMOVED                      _UxGT("Карта извлечена")
#define MSG_MAIN                            _UxGT("Меню")
#define MSG_LCD_ENDSTOPS                    _UxGT("концевик")
#define MSG_AUTOSTART                       _UxGT("Автостарт")
#define MSG_DISABLE_STEPPERS                _UxGT("Выкл. двигатели")
#define MSG_AUTO_HOME                       _UxGT("Парковка")
#define MSG_LEVEL_BED_HOMING                _UxGT("Нулевое полож")
#define MSG_LEVEL_BED_WAITING               _UxGT("Нажмите начать")
#define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Следующая точка")
#define MSG_LEVEL_BED_DONE                  _UxGT("Уровень!")
#define MSG_LEVEL_BED_CANCEL                _UxGT("Отменить")
#define MSG_SET_HOME_OFFSETS                _UxGT("Запомнить парковку")
#define MSG_HOME_OFFSETS_APPLIED            _UxGT("Коррекции примен")
#define MSG_SET_ORIGIN                      _UxGT("Запомнить ноль")
#define MSG_PREHEAT_1                       _UxGT("Преднагрев PLA")
#define MSG_PREHEAT_1_N                     _UxGT("Греть PLA Сопло ")
#define MSG_PREHEAT_1_ALL                   _UxGT("Греть PLA Все")
#define MSG_PREHEAT_1_BEDONLY               _UxGT("Греть PLA Стол")
#define MSG_PREHEAT_1_SETTINGS              _UxGT("Настройки PLA")
#define MSG_PREHEAT_2                       _UxGT("Преднагрев ABS")
#define MSG_PREHEAT_2_N                     _UxGT("Греть ABS Сопло ")
#define MSG_PREHEAT_2_ALL                   _UxGT("Греть ABS Все")
#define MSG_PREHEAT_2_BEDONLY               _UxGT("Греть ABS Стол")
#define MSG_PREHEAT_2_SETTINGS              _UxGT("Настройки ABS")
#define MSG_COOLDOWN                        _UxGT("Охлаждение")
#define MSG_SWITCH_PS_ON                    _UxGT("Включить Питание")
#define MSG_SWITCH_PS_OFF                   _UxGT("Отключить Питание")
#define MSG_EXTRUDE                         _UxGT("Экструзия")
#define MSG_RETRACT                         _UxGT("Втягивание")
#define MSG_MOVE_AXIS                       _UxGT("Движение по осям")
#define MSG_LEVEL_BED                       _UxGT("Калибровать стол")
#define MSG_MOVE_X                          _UxGT("Движение по X")
#define MSG_MOVE_Y                          _UxGT("Движение по Y")
#define MSG_MOVE_Z                          _UxGT("Движение по Z")
#define MSG_MOVE_E                          _UxGT("Экструдер")
#define MSG_MOVE_01MM                       _UxGT("Движение XYZ 0.1mm")
#define MSG_MOVE_1MM                        _UxGT("Движение XYZ 1mm")
#define MSG_MOVE_10MM                       _UxGT("Движение XY 10mm")
#define MSG_SPEED                           _UxGT("Скорость")
#define MSG_BED_Z                           _UxGT("Z стола")
#define MSG_NOZZLE                          LCD_STR_THERMOMETER _UxGT(" Сопло")
#define MSG_BED                             LCD_STR_THERMOMETER _UxGT(" Стол")
#define MSG_FAN_SPEED                       _UxGT("Кулер")
#define MSG_FLOW                            _UxGT("Поток")
#define MSG_CONTROL                         _UxGT("Настройки")
#define MSG_MIN                             LCD_STR_THERMOMETER _UxGT(" Минимум")
#define MSG_MAX                             LCD_STR_THERMOMETER _UxGT(" Максимум")
#define MSG_FACTOR                          LCD_STR_THERMOMETER _UxGT(" Фактор")
#define MSG_AUTOTEMP                        _UxGT("Автотемпература")
#define MSG_ON                              _UxGT("Вкл. ")
#define MSG_OFF                             _UxGT("Откл. ")
#define MSG_PID_P                           _UxGT("PID-P")
#define MSG_PID_I                           _UxGT("PID-I")
#define MSG_PID_D                           _UxGT("PID-D")
#define MSG_PID_C                           _UxGT("PID-C")
#define MSG_ACC                             _UxGT("Acc")
#define MSG_VX_JERK                         _UxGT("Vx-рывок")
#define MSG_VY_JERK                         _UxGT("Vy-рывок")
#define MSG_VZ_JERK                         _UxGT("Vz-рывок")
#define MSG_VE_JERK                         _UxGT("Ve-рывок")
#define MSG_VMAX                            _UxGT("Vмакс ")
#define MSG_VMIN                            _UxGT("Vмин")
#define MSG_VTRAV_MIN                       _UxGT("Vпутеш. мин")
#define MSG_AMAX                            _UxGT("Aмакс")
#define MSG_A_RETRACT                       _UxGT("A-втягивание")
#define MSG_A_TRAVEL                        _UxGT("A-путеш.")
#define MSG_XSTEPS                          _UxGT("X шаг/мм")
#define MSG_YSTEPS                          _UxGT("Y шаг/мм")
#define MSG_ZSTEPS                          _UxGT("Z шаг/мм")
#define MSG_ESTEPS                          _UxGT("E шаг/мм")
#define MSG_E1STEPS                         _UxGT("E1 шаг/мм")
#define MSG_E2STEPS                         _UxGT("E2 шаг/мм")
#define MSG_E3STEPS                         _UxGT("E3 шаг/мм")
#define MSG_E4STEPS                         _UxGT("E4 шаг/мм")
#define MSG_TEMPERATURE                     _UxGT("Температура")
#define MSG_MOTION                          _UxGT("Механика")
#define MSG_VOLUMETRIC                      _UxGT("Пруток")
#define MSG_VOLUMETRIC_ENABLED              _UxGT("E в mm3")
#define MSG_FILAMENT_DIAM                   _UxGT("Диаметр прутка")
#define MSG_CONTRAST                        _UxGT("Контраст LCD")
#define MSG_STORE_EPROM                     _UxGT("Сохранить в EEPROM")
#define MSG_LOAD_EPROM                      _UxGT("Считать из EEPROM")
#define MSG_RESTORE_FAILSAFE                _UxGT("Сброс EEPROM")
#define MSG_REFRESH                         _UxGT("Обновить")
#define MSG_WATCH                           _UxGT("Обзор")
#define MSG_PREPARE                         _UxGT("Действия")
#define MSG_TUNE                            _UxGT("Настройки")
#define MSG_PAUSE_PRINT                     _UxGT("Пауза печати")
#define MSG_RESUME_PRINT                    _UxGT("Продолжить печать")
#define MSG_STOP_PRINT                      _UxGT("Остановить печать")
#define MSG_CARD_MENU                       _UxGT("Обзор карты")
#define MSG_NO_CARD                         _UxGT("Нет карты")
#define MSG_DWELL                           _UxGT("Сон...")
#define MSG_USERWAIT                        _UxGT("Ожиданиие")
#define MSG_RESUMING                        _UxGT("Возобновление...")
#define MSG_PRINT_ABORTED                   _UxGT("Отмена печати")
#define MSG_NO_MOVE                         _UxGT("Нет движения.")
#define MSG_KILLED                          _UxGT("УБИТО.")
#define MSG_STOPPED                         _UxGT("ОСТАНОВЛЕНО.")
#define MSG_CONTROL_RETRACT                 _UxGT("Втягивание mm")
#define MSG_CONTROL_RETRACT_SWAP            _UxGT("Втяг. смены mm")
#define MSG_CONTROL_RETRACTF                _UxGT("Втягивание V")
#define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Втяг. прыжка mm")
#define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Возврат mm")
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Возврат смены mm")
#define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Возврат  V")
#define MSG_AUTORETRACT                     _UxGT("Авто Втягивание")
#define MSG_FILAMENTCHANGE                  _UxGT("Смена прутка")
#define MSG_INIT_SDCARD                     _UxGT("Иниц. карту")
#define MSG_CNG_SDCARD                      _UxGT("Сменить карту")
#define MSG_ZPROBE_OUT                      _UxGT("Z датчик вне стола")
#define MSG_HOME                            _UxGT("Паркуй X/Y перед Z")
#define MSG_FIRST                           _UxGT("первый")
#define MSG_ZPROBE_ZOFFSET                  _UxGT("Смещение Z")
#define MSG_BABYSTEP_X                      _UxGT("Микрошаг X")
#define MSG_BABYSTEP_Y                      _UxGT("Микрошаг Y")
#define MSG_BABYSTEP_Z                      _UxGT("Микрошаг Z")
#define MSG_ENDSTOP_ABORT                   _UxGT("Сработал концевик")
#define MSG_HEATING_FAILED_LCD              _UxGT("Разогрев не удался")
#define MSG_ERR_REDUNDANT_TEMP              _UxGT("Ошибка:Слишком горячо")
#define MSG_THERMAL_RUNAWAY                 _UxGT("ТЕПЛО УБЕГАНИЯ!")
#define MSG_ERR_MAXTEMP                     _UxGT("Ошибка: Т макс.")
#define MSG_ERR_MINTEMP                     _UxGT("Ошибка: Т мин.")
#define MSG_ERR_MAXTEMP_BED                 _UxGT("Ошибка:Т макс.стол")
#define MSG_ERR_MINTEMP_BED                 _UxGT("Ошибка:Т мин.стол")
#define MSG_HEATING                         _UxGT("Нагреваю сопло...")
#define MSG_HEATING_COMPLETE                _UxGT("Нагрев выполнен")
#define MSG_BED_HEATING                     _UxGT("Нагреваю стол")
#define MSG_BED_DONE                        _UxGT("Стол разогрет")
#define MSG_DELTA_CALIBRATE                 _UxGT("Калибровка Delta")
#define MSG_DELTA_CALIBRATE_X               _UxGT("Калибровать X")
#define MSG_DELTA_CALIBRATE_Y               _UxGT("Калибровать Y")
#define MSG_DELTA_CALIBRATE_Z               _UxGT("Калибровать Z")
#define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Калибровать центр")

#endif // LANGUAGE_RU_H
