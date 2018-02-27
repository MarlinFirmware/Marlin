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
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#ifndef LANGUAGE_RU_H
#define LANGUAGE_RU_H

#define MAPPER_D0D1                // For Cyrillic
#define DISPLAY_CHARSET_ISO10646_5

#ifndef WELCOME_MSG
  #define WELCOME_MSG                         MACHINE_NAME _UxGT(" готов.")
#endif
#ifndef MSG_BACK
  #define MSG_BACK                            _UxGT("Назад")
#endif
#ifndef MSG_SD_INSERTED
  #define MSG_SD_INSERTED                     _UxGT("Карта вставлена")
#endif
#ifndef MSG_SD_REMOVED
  #define MSG_SD_REMOVED                      _UxGT("Карта извлечена")
#endif
#ifndef MSG_LCD_ENDSTOPS
  #define MSG_LCD_ENDSTOPS                    _UxGT("Эндстопы") // Max length 8 characters
#endif
#ifndef MSG_MAIN
  #define MSG_MAIN                            _UxGT("Меню")
#endif
#ifndef MSG_AUTOSTART
  #define MSG_AUTOSTART                       _UxGT("Автостарт")
#endif
#ifndef MSG_DISABLE_STEPPERS
  #define MSG_DISABLE_STEPPERS                _UxGT("Выкл. двигатели")
#endif
#ifndef MSG_DEBUG_MENU
  #define MSG_DEBUG_MENU                      _UxGT("Меню отладки")
#endif
#ifndef MSG_PROGRESS_BAR_TEST
  #define MSG_PROGRESS_BAR_TEST               _UxGT("Тест индикатора")
#endif
#ifndef MSG_AUTO_HOME
  #define MSG_AUTO_HOME                       _UxGT("Авто парковка")
#endif
#ifndef MSG_AUTO_HOME_X
  #define MSG_AUTO_HOME_X                     _UxGT("Парковка X")
#endif
#ifndef MSG_AUTO_HOME_Y
  #define MSG_AUTO_HOME_Y                     _UxGT("Парковка Y")
#endif
#ifndef MSG_AUTO_HOME_Z
  #define MSG_AUTO_HOME_Z                     _UxGT("Парковка Z")
#endif
#ifndef MSG_LEVEL_BED_HOMING
  #define MSG_LEVEL_BED_HOMING                _UxGT("Нулевое положение")
#endif
#ifndef MSG_LEVEL_BED_WAITING
  #define MSG_LEVEL_BED_WAITING               _UxGT("Нажмите чтобы начать")
#endif
#ifndef MSG_LEVEL_BED_NEXT_POINT
  #define MSG_LEVEL_BED_NEXT_POINT            _UxGT("Следующая точка")
#endif
#ifndef MSG_LEVEL_BED_DONE
  #define MSG_LEVEL_BED_DONE                  _UxGT("Выравнивание готово!")
#endif
#ifndef MSG_Z_FADE_HEIGHT
  #define MSG_Z_FADE_HEIGHT                   _UxGT("Высота спада")
#endif
#ifndef MSG_SET_HOME_OFFSETS
  #define MSG_SET_HOME_OFFSETS                _UxGT("Установить парковку")
#endif
#ifndef MSG_HOME_OFFSETS_APPLIED
  #define MSG_HOME_OFFSETS_APPLIED            _UxGT("Коррекции применены")
#endif
#ifndef MSG_SET_ORIGIN
  #define MSG_SET_ORIGIN                      _UxGT("Установить ноль")
#endif
#ifndef MSG_PREHEAT_1
  #define MSG_PREHEAT_1                       _UxGT("Преднагрев PLA")
#endif
#ifndef MSG_PREHEAT_1_N
  #define MSG_PREHEAT_1_N                     MSG_PREHEAT_1 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_1_ALL
  #define MSG_PREHEAT_1_ALL                   MSG_PREHEAT_1 _UxGT(" всё")
#endif
#ifndef MSG_PREHEAT_1_END
  #define MSG_PREHEAT_1_END                   MSG_PREHEAT_1 _UxGT(" сопло")
#endif
#ifndef MSG_PREHEAT_1_BEDONLY
  #define MSG_PREHEAT_1_BEDONLY               MSG_PREHEAT_1 _UxGT(" стол")
#endif
#ifndef MSG_PREHEAT_1_SETTINGS
  #define MSG_PREHEAT_1_SETTINGS              MSG_PREHEAT_1 _UxGT(" настр.")
#endif
#ifndef MSG_PREHEAT_2
  #define MSG_PREHEAT_2                       _UxGT("Преднагрев ABS")
#endif
#ifndef MSG_PREHEAT_2_N
  #define MSG_PREHEAT_2_N                     MSG_PREHEAT_2 _UxGT(" ")
#endif
#ifndef MSG_PREHEAT_2_ALL
  #define MSG_PREHEAT_2_ALL                   MSG_PREHEAT_2 _UxGT(" всё")
#endif
#ifndef MSG_PREHEAT_2_END
  #define MSG_PREHEAT_2_END                   MSG_PREHEAT_2 _UxGT(" сопло")
#endif
#ifndef MSG_PREHEAT_2_BEDONLY
  #define MSG_PREHEAT_2_BEDONLY               MSG_PREHEAT_2 _UxGT(" стол")
#endif
#ifndef MSG_PREHEAT_2_SETTINGS
  #define MSG_PREHEAT_2_SETTINGS              MSG_PREHEAT_2 _UxGT(" настр.")
#endif
#ifndef MSG_COOLDOWN
  #define MSG_COOLDOWN                        _UxGT("Охлаждение")
#endif
#ifndef MSG_SWITCH_PS_ON
  #define MSG_SWITCH_PS_ON                    _UxGT("Включить питание")
#endif
#ifndef MSG_SWITCH_PS_OFF
  #define MSG_SWITCH_PS_OFF                   _UxGT("Выключить питание")
#endif
#ifndef MSG_EXTRUDE
  #define MSG_EXTRUDE                         _UxGT("Экструзия")
#endif
#ifndef MSG_RETRACT
  #define MSG_RETRACT                         _UxGT("Втягивание")
#endif
#ifndef MSG_MOVE_AXIS
  #define MSG_MOVE_AXIS                       _UxGT("Движение по осям")
#endif
#ifndef MSG_BED_LEVELING
  #define MSG_BED_LEVELING                    _UxGT("Калибровка стола")
#endif
#ifndef MSG_LEVEL_BED
  #define MSG_LEVEL_BED                       _UxGT("Калибровать стол")
#endif
#ifndef MSG_LEVEL_CORNERS
  #define MSG_LEVEL_CORNERS                   _UxGT("Выровнять углы")
#endif
#ifndef MSG_NEXT_CORNER
  #define MSG_NEXT_CORNER                     _UxGT("Следующий угол")
#endif
#ifndef MSG_EDITING_STOPPED
  #define MSG_EDITING_STOPPED                 _UxGT("Ред. сетки завершено")
#endif
#ifndef MSG_USER_MENU
  #define MSG_USER_MENU                       _UxGT("Свои комманды")
#endif

#ifndef MSG_UBL_DOING_G29
  #define MSG_UBL_DOING_G29                   _UxGT("Выполняем G29")
#endif
#ifndef MSG_UBL_UNHOMED
  #define MSG_UBL_UNHOMED                     _UxGT("Паркуем сначала XYZ")
#endif
#ifndef MSG_UBL_TOOLS
  #define MSG_UBL_TOOLS                       _UxGT("Утилиты UBL")
#endif
#ifndef MSG_UBL_LEVEL_BED
  #define MSG_UBL_LEVEL_BED                   _UxGT("Калибровка UBL")
#endif
#ifndef MSG_UBL_MANUAL_MESH
  #define MSG_UBL_MANUAL_MESH                 _UxGT("Постр. сетку от руки")
#endif
#ifndef MSG_UBL_BC_INSERT
  #define MSG_UBL_BC_INSERT                   _UxGT("Пост. шимм и измер.")
#endif
#ifndef MSG_UBL_BC_INSERT2
  #define MSG_UBL_BC_INSERT2                  _UxGT("Измерение")
#endif
#ifndef MSG_UBL_BC_REMOVE
  #define MSG_UBL_BC_REMOVE                   _UxGT("Удал. и измер. стол")
#endif
#ifndef MSG_UBL_MOVING_TO_NEXT
  #define MSG_UBL_MOVING_TO_NEXT              _UxGT("Двигаемся дальше")
#endif
#ifndef MSG_UBL_ACTIVATE_MESH
  #define MSG_UBL_ACTIVATE_MESH               _UxGT("Активировать UBL")
#endif
#ifndef MSG_UBL_DEACTIVATE_MESH
  #define MSG_UBL_DEACTIVATE_MESH             _UxGT("Деактивировать UBL")
#endif
#ifndef MSG_UBL_SET_BED_TEMP
  #define MSG_UBL_SET_BED_TEMP                _UxGT("Температура стола")
#endif
#ifndef MSG_UBL_CUSTOM_BED_TEMP
  #define MSG_UBL_CUSTOM_BED_TEMP             MSG_UBL_SET_BED_TEMP
#endif
#ifndef MSG_UBL_SET_HOTEND_TEMP
  #define MSG_UBL_SET_HOTEND_TEMP             _UxGT("Температура сопла")
#endif
#ifndef MSG_UBL_CUSTOM_HOTEND_TEMP
  #define MSG_UBL_CUSTOM_HOTEND_TEMP          MSG_UBL_SET_HOTEND_TEMP
#endif
#ifndef MSG_UBL_MESH_EDIT
  #define MSG_UBL_MESH_EDIT                   _UxGT("Редактор сеток")
#endif
#ifndef MSG_UBL_EDIT_CUSTOM_MESH
  #define MSG_UBL_EDIT_CUSTOM_MESH            _UxGT("Редакт. свою сетку")
#endif
#ifndef MSG_UBL_FINE_TUNE_MESH
  #define MSG_UBL_FINE_TUNE_MESH              _UxGT("Точная настр. сетки")
#endif
#ifndef MSG_UBL_DONE_EDITING_MESH
  #define MSG_UBL_DONE_EDITING_MESH           _UxGT("Ред. сетки завершено")
#endif
#ifndef MSG_UBL_BUILD_CUSTOM_MESH
  #define MSG_UBL_BUILD_CUSTOM_MESH           _UxGT("Построить свою сетку")
#endif
#ifndef MSG_UBL_BUILD_MESH_MENU
  #define MSG_UBL_BUILD_MESH_MENU             _UxGT("Построить сетку")
#endif
#ifndef MSG_UBL_BUILD_PLA_MESH
  #define MSG_UBL_BUILD_PLA_MESH              _UxGT("Построить сетку PLA")
#endif
#ifndef MSG_UBL_BUILD_ABS_MESH
  #define MSG_UBL_BUILD_ABS_MESH              _UxGT("Построить сетку ABS")
#endif
#ifndef MSG_UBL_BUILD_COLD_MESH
  #define MSG_UBL_BUILD_COLD_MESH             _UxGT("Построить хол. сетку")
#endif
#ifndef MSG_UBL_MESH_HEIGHT_ADJUST
  #define MSG_UBL_MESH_HEIGHT_ADJUST          _UxGT("Устан. высоту сетки")
#endif
#ifndef MSG_UBL_MESH_HEIGHT_AMOUNT
  #define MSG_UBL_MESH_HEIGHT_AMOUNT          _UxGT("Высота")
#endif
#ifndef MSG_UBL_VALIDATE_MESH_MENU
  #define MSG_UBL_VALIDATE_MESH_MENU          _UxGT("Проверить сетку")
#endif
#ifndef MSG_UBL_VALIDATE_PLA_MESH
  #define MSG_UBL_VALIDATE_PLA_MESH           _UxGT("Проверить сетку PLA")
#endif
#ifndef MSG_UBL_VALIDATE_ABS_MESH
  #define MSG_UBL_VALIDATE_ABS_MESH           _UxGT("Проверить сетку ABS")
#endif
#ifndef MSG_UBL_VALIDATE_CUSTOM_MESH
  #define MSG_UBL_VALIDATE_CUSTOM_MESH        _UxGT("Проверить свою сетку")
#endif
#ifndef MSG_UBL_CONTINUE_MESH
  #define MSG_UBL_CONTINUE_MESH               _UxGT("Продолжить сетку")
#endif
#ifndef MSG_UBL_MESH_LEVELING
  #define MSG_UBL_MESH_LEVELING               _UxGT("Калибровка сетки")
#endif
#ifndef MSG_UBL_3POINT_MESH_LEVELING
  #define MSG_UBL_3POINT_MESH_LEVELING        _UxGT("Калибровка 3-х точек")
#endif
#ifndef MSG_UBL_GRID_MESH_LEVELING
  #define MSG_UBL_GRID_MESH_LEVELING          _UxGT("Калибровка растера")
#endif
#ifndef MSG_UBL_MESH_LEVEL
  #define MSG_UBL_MESH_LEVEL                  _UxGT("Выровнить сетку")
#endif
#ifndef MSG_UBL_SIDE_POINTS
  #define MSG_UBL_SIDE_POINTS                 _UxGT("Крайние точки")
#endif
#ifndef MSG_UBL_MAP_TYPE
  #define MSG_UBL_MAP_TYPE                    _UxGT("Тип карты")
#endif
#ifndef MSG_UBL_OUTPUT_MAP
  #define MSG_UBL_OUTPUT_MAP                  _UxGT("Вывести карту сетки")
#endif
#ifndef MSG_UBL_OUTPUT_MAP_HOST
  #define MSG_UBL_OUTPUT_MAP_HOST             _UxGT("Вывести на хост")
#endif
#ifndef MSG_UBL_OUTPUT_MAP_CSV
  #define MSG_UBL_OUTPUT_MAP_CSV              _UxGT("Вывести в CSV")
#endif
#ifndef MSG_UBL_OUTPUT_MAP_BACKUP
  #define MSG_UBL_OUTPUT_MAP_BACKUP           _UxGT("Забекапить сетку")
#endif
#ifndef MSG_UBL_INFO_UBL
  #define MSG_UBL_INFO_UBL                    _UxGT("Вывод информации UBL")
#endif
#ifndef MSG_UBL_EDIT_MESH_MENU
  #define MSG_UBL_EDIT_MESH_MENU              _UxGT("Редактировать сетку")
#endif
#ifndef MSG_UBL_FILLIN_AMOUNT
  #define MSG_UBL_FILLIN_AMOUNT               _UxGT("Кол-во заполнителя")
#endif
#ifndef MSG_UBL_MANUAL_FILLIN
  #define MSG_UBL_MANUAL_FILLIN               _UxGT("Ручное заполнение")
#endif
#ifndef MSG_UBL_SMART_FILLIN
  #define MSG_UBL_SMART_FILLIN                _UxGT("Умное заполнение")
#endif
#ifndef MSG_UBL_FILLIN_MESH
  #define MSG_UBL_FILLIN_MESH                 _UxGT("Заполнить сетку")
#endif
#ifndef MSG_UBL_INVALIDATE_ALL
  #define MSG_UBL_INVALIDATE_ALL              _UxGT("Аннулировать всё")
#endif
#ifndef MSG_UBL_INVALIDATE_CLOSEST
  #define MSG_UBL_INVALIDATE_CLOSEST          _UxGT("Аннулир. ближ. точку")
#endif
#ifndef MSG_UBL_FINE_TUNE_ALL
  #define MSG_UBL_FINE_TUNE_ALL               _UxGT("Точная настр. всего")
#endif
#ifndef MSG_UBL_FINE_TUNE_CLOSEST
  #define MSG_UBL_FINE_TUNE_CLOSEST           _UxGT("Настр. ближ. точки")
#endif
#ifndef MSG_UBL_STORAGE_MESH_MENU
  #define MSG_UBL_STORAGE_MESH_MENU           _UxGT("Хранилище сетей")
#endif
#ifndef MSG_UBL_STORAGE_SLOT
  #define MSG_UBL_STORAGE_SLOT                _UxGT("Слот памяти")
#endif
#ifndef MSG_UBL_LOAD_MESH
  #define MSG_UBL_LOAD_MESH                   _UxGT("Загрузить сетку стола")
#endif
#ifndef MSG_UBL_SAVE_MESH
  #define MSG_UBL_SAVE_MESH                   _UxGT("Сохранить сетку стола")
#endif
#ifndef MSG_MESH_LOADED
  #define MSG_MESH_LOADED                     _UxGT("Сетка %i загружена")
#endif
#ifndef MSG_MESH_SAVED
  #define MSG_MESH_SAVED                      _UxGT("Сетка %i сохранена")
#endif
#ifndef MSG_NO_STORAGE
  #define MSG_NO_STORAGE                      _UxGT("Нет хранилища")
#endif
#ifndef MSG_UBL_SAVE_ERROR
  #define MSG_UBL_SAVE_ERROR                  _UxGT("Ошибка: Сохран. UBL")
#endif
#ifndef MSG_UBL_RESTORE_ERROR
  #define MSG_UBL_RESTORE_ERROR               _UxGT("Ошибка: Восстан. UBL")
#endif
#ifndef MSG_UBL_Z_OFFSET_STOPPED
  #define MSG_UBL_Z_OFFSET_STOPPED            _UxGT("Смещение Z останов.")
#endif
#ifndef MSG_UBL_STEP_BY_STEP_MENU
  #define MSG_UBL_STEP_BY_STEP_MENU           _UxGT("Пошаговое UBL")
#endif

#ifndef MSG_LED_CONTROL
  #define MSG_LED_CONTROL                     _UxGT("Настройки LED")
#endif
#ifndef MSG_LEDS_ON
  #define MSG_LEDS_ON                         _UxGT("Включить подсветку")
#endif
#ifndef MSG_LEDS_OFF
  #define MSG_LEDS_OFF                        _UxGT("Выключить подсветку")
#endif
#ifndef MSG_LED_PRESETS
  #define MSG_LED_PRESETS                     _UxGT("Предустановки света")
#endif
#ifndef MSG_SET_LEDS_RED
  #define MSG_SET_LEDS_RED                    _UxGT("Красный свет")
#endif
#ifndef MSG_SET_LEDS_ORANGE
  #define MSG_SET_LEDS_ORANGE                 _UxGT("Оранжевый свет")
#endif
#ifndef MSG_SET_LEDS_YELLOW
  #define MSG_SET_LEDS_YELLOW                 _UxGT("Жёлтый свет")
#endif
#ifndef MSG_SET_LEDS_GREEN
  #define MSG_SET_LEDS_GREEN                  _UxGT("Зелёный свет")
#endif
#ifndef MSG_SET_LEDS_BLUE
  #define MSG_SET_LEDS_BLUE                   _UxGT("Синий свет")
#endif
#ifndef MSG_SET_LEDS_INDIGO
  #define MSG_SET_LEDS_INDIGO                 _UxGT("Индиго свет")
#endif
#ifndef MSG_SET_LEDS_VIOLET
  #define MSG_SET_LEDS_VIOLET                 _UxGT("Фиолетовый свет")
#endif
#ifndef MSG_SET_LEDS_WHITE
  #define MSG_SET_LEDS_WHITE                  _UxGT("Белый свет")
#endif
#ifndef MSG_SET_LEDS_DEFAULT
  #define MSG_SET_LEDS_DEFAULT                _UxGT("Свет по умолчанию")
#endif
#ifndef MSG_CUSTOM_LEDS
  #define MSG_CUSTOM_LEDS                     _UxGT("Свои настр. света")
#endif
#ifndef MSG_INTENSITY_R
  #define MSG_INTENSITY_R                     _UxGT("Интенсивн. красного")
#endif
#ifndef MSG_INTENSITY_G
  #define MSG_INTENSITY_G                     _UxGT("Интенсивн. зелёного")
#endif
#ifndef MSG_INTENSITY_B
  #define MSG_INTENSITY_B                     _UxGT("Интенсивн. синего")
#endif
#ifndef MSG_INTENSITY_W
  #define MSG_INTENSITY_W                     _UxGT("Интенсивн. белого")
#endif
#ifndef MSG_LED_BRIGHTNESS
  #define MSG_LED_BRIGHTNESS                  _UxGT("Яркость")
#endif

#ifndef MSG_MOVING
  #define MSG_MOVING                          _UxGT("Движемся...")
#endif
#ifndef MSG_FREE_XY
  #define MSG_FREE_XY                         _UxGT("Освобождаем XY")
#endif
#ifndef MSG_MOVE_X
  #define MSG_MOVE_X                          _UxGT("Движение по X")
#endif
#ifndef MSG_MOVE_Y
  #define MSG_MOVE_Y                          _UxGT("Движение по Y")
#endif
#ifndef MSG_MOVE_Z
  #define MSG_MOVE_Z                          _UxGT("Движение по Z")
#endif
#ifndef MSG_MOVE_E
  #define MSG_MOVE_E                          _UxGT("Экструдер")
#endif
#ifndef MSG_MOVE_01MM
  #define MSG_MOVE_01MM                       _UxGT("Движение 0.1мм")
#endif
#ifndef MSG_MOVE_1MM
  #define MSG_MOVE_1MM                        _UxGT("Движение 1мм")
#endif
#ifndef MSG_MOVE_10MM
  #define MSG_MOVE_10MM                       _UxGT("Движение 10мм")
#endif
#ifndef MSG_SPEED
  #define MSG_SPEED                           _UxGT("Скорость")
#endif
#ifndef MSG_BED_Z
  #define MSG_BED_Z                           _UxGT("Z стола")
#endif
#ifndef MSG_NOZZLE
  #define MSG_NOZZLE                          _UxGT("Сопло")
#endif
#ifndef MSG_BED
  #define MSG_BED                             _UxGT("Стол")
#endif
#ifndef MSG_FAN_SPEED
  #define MSG_FAN_SPEED                       _UxGT("Кулер")
#endif
#ifndef MSG_EXTRA_FAN_SPEED
  #define MSG_EXTRA_FAN_SPEED                 _UxGT("Кулер доп.")
#endif
#ifndef MSG_FLOW
  #define MSG_FLOW                            _UxGT("Поток")
#endif
#ifndef MSG_CONTROL
  #define MSG_CONTROL                         _UxGT("Настройки")
#endif
#ifndef MSG_MIN
  #define MSG_MIN                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Мин")
#endif
#ifndef MSG_MAX
  #define MSG_MAX                             _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Макс")
#endif
#ifndef MSG_FACTOR
  #define MSG_FACTOR                          _UxGT(" ") LCD_STR_THERMOMETER _UxGT(" Фактор")
#endif
#ifndef MSG_AUTOTEMP
  #define MSG_AUTOTEMP                        _UxGT("Автотемпература")
#endif
#ifndef MSG_ON
  #define MSG_ON                              _UxGT("Вкл.")
#endif
#ifndef MSG_OFF
  #define MSG_OFF                             _UxGT("Выкл.")
#endif
#ifndef MSG_PID_P
  #define MSG_PID_P                           _UxGT("PID-P")
#endif
#ifndef MSG_PID_I
  #define MSG_PID_I                           _UxGT("PID-I")
#endif
#ifndef MSG_PID_D
  #define MSG_PID_D                           _UxGT("PID-D")
#endif
#ifndef MSG_PID_C
  #define MSG_PID_C                           _UxGT("PID-C")
#endif
#ifndef MSG_SELECT
  #define MSG_SELECT                          _UxGT("Выбор")
#endif
#ifndef MSG_ACC
  #define MSG_ACC                             _UxGT("Ускорение")
#endif
#ifndef MSG_JERK
  #define MSG_JERK                            _UxGT("Рывок")
#endif
#ifndef MSG_VX_JERK
  #define MSG_VX_JERK                         _UxGT("Vx-рывок")
#endif
#ifndef MSG_VY_JERK
  #define MSG_VY_JERK                         _UxGT("Vy-рывок")
#endif
#ifndef MSG_VZ_JERK
  #define MSG_VZ_JERK                         _UxGT("Vz-рывок")
#endif
#ifndef MSG_VE_JERK
  #define MSG_VE_JERK                         _UxGT("Ve-рывок")
#endif
#ifndef MSG_VELOCITY
  #define MSG_VELOCITY                        _UxGT("Быстрота")
#endif
#ifndef MSG_VMAX
  #define MSG_VMAX                            _UxGT("Vмакс ")
#endif
#ifndef MSG_VMIN
  #define MSG_VMIN                            _UxGT("Vмин")
#endif
#ifndef MSG_VTRAV_MIN
  #define MSG_VTRAV_MIN                       _UxGT("Vпутеш. мин")
#endif
#ifndef MSG_ACCELERATION
  #define MSG_ACCELERATION                    _UxGT("Ускорение")
#endif
#ifndef MSG_AMAX
  #define MSG_AMAX                            _UxGT("Aмакс ")
#endif
#ifndef MSG_A_RETRACT
  #define MSG_A_RETRACT                       _UxGT("A-втягивание")
#endif
#ifndef MSG_A_TRAVEL
  #define MSG_A_TRAVEL                        _UxGT("A-путеш.")
#endif
#ifndef MSG_STEPS_PER_MM
  #define MSG_STEPS_PER_MM                    _UxGT("Шаг/мм")
#endif
#ifndef MSG_XSTEPS
  #define MSG_XSTEPS                          _UxGT("Xшаг/мм")
#endif
#ifndef MSG_YSTEPS
  #define MSG_YSTEPS                          _UxGT("Yшаг/мм")
#endif
#ifndef MSG_ZSTEPS
  #define MSG_ZSTEPS                          _UxGT("Zшаг/мм")
#endif
#ifndef MSG_ESTEPS
  #define MSG_ESTEPS                          _UxGT("Eшаг/мм")
#endif
#ifndef MSG_E1STEPS
  #define MSG_E1STEPS                         _UxGT("E1шаг/мм")
#endif
#ifndef MSG_E2STEPS
  #define MSG_E2STEPS                         _UxGT("E2шаг/мм")
#endif
#ifndef MSG_E3STEPS
  #define MSG_E3STEPS                         _UxGT("E3шаг/мм")
#endif
#ifndef MSG_E4STEPS
  #define MSG_E4STEPS                         _UxGT("E4шаг/мм")
#endif
#ifndef MSG_E5STEPS
  #define MSG_E5STEPS                         _UxGT("E5шаг/мм")
#endif
#ifndef MSG_TEMPERATURE
  #define MSG_TEMPERATURE                     _UxGT("Температура")
#endif
#ifndef MSG_MOTION
  #define MSG_MOTION                          _UxGT("Движение")
#endif
#ifndef MSG_FILAMENT
  #define MSG_FILAMENT                        _UxGT("Филамент")
#endif
#ifndef MSG_VOLUMETRIC_ENABLED
  #define MSG_VOLUMETRIC_ENABLED              _UxGT("E в мм3")
#endif
#ifndef MSG_FILAMENT_DIAM
  #define MSG_FILAMENT_DIAM                   _UxGT("Диаметр филамента")
#endif
#ifndef MSG_ADVANCE_K
  #define MSG_ADVANCE_K                       _UxGT("K продвижения")
#endif
#ifndef MSG_CONTRAST
  #define MSG_CONTRAST                        _UxGT("Контраст LCD")
#endif
#ifndef MSG_STORE_EEPROM
  #define MSG_STORE_EEPROM                    _UxGT("Сохранить настройки")
#endif
#ifndef MSG_LOAD_EEPROM
  #define MSG_LOAD_EEPROM                     _UxGT("Загрузить настройки")
#endif
#ifndef MSG_RESTORE_FAILSAFE
  #define MSG_RESTORE_FAILSAFE                _UxGT("Восстановить настр.")
#endif
#ifndef MSG_INIT_EEPROM
  #define MSG_INIT_EEPROM                     _UxGT("Инициализация EEPROM")
#endif
#ifndef MSG_REFRESH
  #define MSG_REFRESH                         _UxGT("Обновить")
#endif
#ifndef MSG_WATCH
  #define MSG_WATCH                           _UxGT("Информационный экран")
#endif
#ifndef MSG_PREPARE
  #define MSG_PREPARE                         _UxGT("Подготовить")
#endif
#ifndef MSG_TUNE
  #define MSG_TUNE                            _UxGT("Настроить")
#endif
#ifndef MSG_PAUSE_PRINT
  #define MSG_PAUSE_PRINT                     _UxGT("Пауза печати")
#endif
#ifndef MSG_RESUME_PRINT
  #define MSG_RESUME_PRINT                    _UxGT("Продолжить печать")
#endif
#ifndef MSG_STOP_PRINT
  #define MSG_STOP_PRINT                      _UxGT("Остановить печать")
#endif
#ifndef MSG_CARD_MENU
  #define MSG_CARD_MENU                       _UxGT("Печать с SD карты")
#endif
#ifndef MSG_NO_CARD
  #define MSG_NO_CARD                         _UxGT("Нет SD карты")
#endif
#ifndef MSG_DWELL
  #define MSG_DWELL                           _UxGT("Сон...")
#endif
#ifndef MSG_USERWAIT
  #define MSG_USERWAIT                        _UxGT("Продолжить...")
#endif
#ifndef MSG_PRINT_PAUSED
  #define MSG_PRINT_PAUSED                    _UxGT("Печать на паузе")
#endif
#ifndef MSG_RESUMING
  #define MSG_RESUMING                        _UxGT("Возобновление...")
#endif
#ifndef MSG_PRINT_ABORTED
  #define MSG_PRINT_ABORTED                   _UxGT("Печать отменена")
#endif
#ifndef MSG_NO_MOVE
  #define MSG_NO_MOVE                         _UxGT("Нет движения.")
#endif
#ifndef MSG_KILLED
  #define MSG_KILLED                          _UxGT("УБИТО. ")
#endif
#ifndef MSG_STOPPED
  #define MSG_STOPPED                         _UxGT("ОСТАНОВЛЕНО. ")
#endif
#ifndef MSG_CONTROL_RETRACT
  #define MSG_CONTROL_RETRACT                 _UxGT("Втягивание мм")
#endif
#ifndef MSG_CONTROL_RETRACT_SWAP
  #define MSG_CONTROL_RETRACT_SWAP            _UxGT("Втягивание смены мм")
#endif
#ifndef MSG_CONTROL_RETRACTF
  #define MSG_CONTROL_RETRACTF                _UxGT("Втягивание V")
#endif
#ifndef MSG_CONTROL_RETRACT_ZLIFT
  #define MSG_CONTROL_RETRACT_ZLIFT           _UxGT("Втяг. прыжка мм")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER
  #define MSG_CONTROL_RETRACT_RECOVER         _UxGT("Возврат мм")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAP
  #define MSG_CONTROL_RETRACT_RECOVER_SWAP    _UxGT("Возврат смены мм")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVERF
  #define MSG_CONTROL_RETRACT_RECOVERF        _UxGT("Возврат V")
#endif
#ifndef MSG_CONTROL_RETRACT_RECOVER_SWAPF
  #define MSG_CONTROL_RETRACT_RECOVER_SWAPF   _UxGT("Возврат смены V")
#endif
#ifndef MSG_AUTORETRACT
  #define MSG_AUTORETRACT                     _UxGT("Авто Втягивание")
#endif
#ifndef MSG_FILAMENTCHANGE
  #define MSG_FILAMENTCHANGE                  _UxGT("Смена филамента")
#endif
#ifndef MSG_INIT_SDCARD
  #define MSG_INIT_SDCARD                     _UxGT("Иниц. SD карту")
#endif
#ifndef MSG_CNG_SDCARD
  #define MSG_CNG_SDCARD                      _UxGT("Сменить SD карту")
#endif
#ifndef MSG_ZPROBE_OUT
  #define MSG_ZPROBE_OUT                      _UxGT("Z датчик вне стола")
#endif
#ifndef MSG_SKEW_FACTOR
  #define MSG_SKEW_FACTOR                     _UxGT("Фактор наклона")
#endif
#ifndef MSG_BLTOUCH
  #define MSG_BLTOUCH                         _UxGT("BLTouch")
#endif
#ifndef MSG_BLTOUCH_SELFTEST
  #define MSG_BLTOUCH_SELFTEST                _UxGT("Тестирование BLTouch")
#endif
#ifndef MSG_BLTOUCH_RESET
  #define MSG_BLTOUCH_RESET                   _UxGT("Сброс BLTouch")
#endif
#ifndef MSG_BLTOUCH_DEPLOY
  #define MSG_BLTOUCH_DEPLOY                  _UxGT("Установка BLTouch")
#endif
#ifndef MSG_BLTOUCH_STOW
  #define MSG_BLTOUCH_STOW                    _UxGT("Набивка BLTouch")
#endif
#ifndef MSG_HOME
  #define MSG_HOME                            _UxGT("Паркуй") // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#endif
#ifndef MSG_FIRST
  #define MSG_FIRST                           _UxGT("первый")
#endif
#ifndef MSG_ZPROBE_ZOFFSET
  #define MSG_ZPROBE_ZOFFSET                  _UxGT("Смещение Z")
#endif
#ifndef MSG_BABYSTEP_X
  #define MSG_BABYSTEP_X                      _UxGT("Микрошаг X")
#endif
#ifndef MSG_BABYSTEP_Y
  #define MSG_BABYSTEP_Y                      _UxGT("Микрошаг Y")
#endif
#ifndef MSG_BABYSTEP_Z
  #define MSG_BABYSTEP_Z                      _UxGT("Микрошаг Z")
#endif
#ifndef MSG_ENDSTOP_ABORT
  #define MSG_ENDSTOP_ABORT                   _UxGT("Сработал концевик")
#endif
#ifndef MSG_HEATING_FAILED_LCD
  #define MSG_HEATING_FAILED_LCD              _UxGT("Разогрев не удался")
#endif
#ifndef MSG_ERR_REDUNDANT_TEMP
  #define MSG_ERR_REDUNDANT_TEMP              _UxGT("Ошибка: Избыточная Т")
#endif
#ifndef MSG_THERMAL_RUNAWAY
  #define MSG_THERMAL_RUNAWAY                 _UxGT("УБЕГАНИЕ ТЕПЛА")
#endif
#ifndef MSG_ERR_MAXTEMP
  #define MSG_ERR_MAXTEMP                     _UxGT("Ошибка: Т макс.")
#endif
#ifndef MSG_ERR_MINTEMP
  #define MSG_ERR_MINTEMP                     _UxGT("Ошибка: Т мин.")
#endif
#ifndef MSG_ERR_MAXTEMP_BED
  #define MSG_ERR_MAXTEMP_BED                 _UxGT("Ошибка: Т стола макс")
#endif
#ifndef MSG_ERR_MINTEMP_BED
  #define MSG_ERR_MINTEMP_BED                 _UxGT("Ошибка: Т стола мин.")
#endif
#ifndef MSG_ERR_Z_HOMING
  #define MSG_ERR_Z_HOMING                    _UxGT("G28 Z Запрещено")
#endif
#ifndef MSG_HALTED
  #define MSG_HALTED                          _UxGT("ПРИНТЕР ОСТАНОВЛЕН")
#endif
#ifndef MSG_PLEASE_RESET
  #define MSG_PLEASE_RESET                    _UxGT("Сделайте сброс")
#endif
#ifndef MSG_SHORT_DAY
  #define MSG_SHORT_DAY                       _UxGT("д") // One character only
#endif
#ifndef MSG_SHORT_HOUR
  #define MSG_SHORT_HOUR                      _UxGT("ч") // One character only
#endif
#ifndef MSG_SHORT_MINUTE
  #define MSG_SHORT_MINUTE                    _UxGT("м") // One character only
#endif
#ifndef MSG_HEATING
  #define MSG_HEATING                         _UxGT("Нагрев...")
#endif
#ifndef MSG_HEATING_COMPLETE
  #define MSG_HEATING_COMPLETE                _UxGT("Нагрев выполнен")
#endif
#ifndef MSG_BED_HEATING
  #define MSG_BED_HEATING                     _UxGT("Нагрев стола")
#endif
#ifndef MSG_BED_DONE
  #define MSG_BED_DONE                        _UxGT("Стол разогрет")
#endif
#ifndef MSG_DELTA_CALIBRATE
  #define MSG_DELTA_CALIBRATE                 _UxGT("Калибровка Delta")
#endif
#ifndef MSG_DELTA_CALIBRATE_X
  #define MSG_DELTA_CALIBRATE_X               _UxGT("Калибровать X")
#endif
#ifndef MSG_DELTA_CALIBRATE_Y
  #define MSG_DELTA_CALIBRATE_Y               _UxGT("Калибровать Y")
#endif
#ifndef MSG_DELTA_CALIBRATE_Z
  #define MSG_DELTA_CALIBRATE_Z               _UxGT("Калибровать Z")
#endif
#ifndef MSG_DELTA_CALIBRATE_CENTER
  #define MSG_DELTA_CALIBRATE_CENTER          _UxGT("Калибровать центр")
#endif
#ifndef MSG_DELTA_SETTINGS
  #define MSG_DELTA_SETTINGS                  _UxGT("Настройки Delta")
#endif
#ifndef MSG_DELTA_AUTO_CALIBRATE
  #define MSG_DELTA_AUTO_CALIBRATE            _UxGT("Авто калибровка")
#endif
#ifndef MSG_DELTA_HEIGHT_CALIBRATE
  #define MSG_DELTA_HEIGHT_CALIBRATE          _UxGT("Задать высоту Delta")
#endif
#ifndef MSG_DELTA_DIAG_ROD
  #define MSG_DELTA_DIAG_ROD                  _UxGT("Диаг. стержень")
#endif
#ifndef MSG_DELTA_HEIGHT
  #define MSG_DELTA_HEIGHT                    _UxGT("Высота")
#endif
#ifndef MSG_DELTA_RADIUS
  #define MSG_DELTA_RADIUS                    _UxGT("Радиус")
#endif
#ifndef MSG_INFO_MENU
  #define MSG_INFO_MENU                       _UxGT("О принтере")
#endif
#ifndef MSG_INFO_PRINTER_MENU
  #define MSG_INFO_PRINTER_MENU               _UxGT("Инф. о принтере")
#endif
#ifndef MSG_3POINT_LEVELING
  #define MSG_3POINT_LEVELING                 _UxGT("Калибровка 3-х точек")
#endif
#ifndef MSG_LINEAR_LEVELING
  #define MSG_LINEAR_LEVELING                 _UxGT("Линейная калибровка")
#endif
#ifndef MSG_BILINEAR_LEVELING
  #define MSG_BILINEAR_LEVELING               _UxGT("Билинейная калибр.")
#endif
#ifndef MSG_UBL_LEVELING
  #define MSG_UBL_LEVELING                    _UxGT("Калибровка UBL")
#endif
#ifndef MSG_MESH_LEVELING
  #define MSG_MESH_LEVELING                   _UxGT("Калибровка сетки")
#endif
#ifndef MSG_INFO_STATS_MENU
  #define MSG_INFO_STATS_MENU                 _UxGT("Статистика принтера")
#endif
#ifndef MSG_INFO_BOARD_MENU
  #define MSG_INFO_BOARD_MENU                 _UxGT("Информация о плате")
#endif
#ifndef MSG_INFO_THERMISTOR_MENU
  #define MSG_INFO_THERMISTOR_MENU            _UxGT("Термисторы")
#endif
#ifndef MSG_INFO_EXTRUDERS
  #define MSG_INFO_EXTRUDERS                  _UxGT("Экструдеры")
#endif
#ifndef MSG_INFO_BAUDRATE
  #define MSG_INFO_BAUDRATE                   _UxGT("Бод")
#endif
#ifndef MSG_INFO_PROTOCOL
  #define MSG_INFO_PROTOCOL                   _UxGT("Протокол")
#endif
#ifndef MSG_CASE_LIGHT
  #define MSG_CASE_LIGHT                      _UxGT("Подсветка корпуса")
#endif
#ifndef MSG_CASE_LIGHT_BRIGHTNESS
  #define MSG_CASE_LIGHT_BRIGHTNESS           _UxGT("Яркость подсветки")
#endif
#if LCD_WIDTH >= 20
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Счётчик печати")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Закончено")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Общее время печати")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Наидольшее задание")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Длинна филамента")
  #endif
#else
  #ifndef MSG_INFO_PRINT_COUNT
    #define MSG_INFO_PRINT_COUNT              _UxGT("Отпечатков")
  #endif
  #ifndef MSG_INFO_COMPLETED_PRINTS
    #define MSG_INFO_COMPLETED_PRINTS         _UxGT("Закончено")
  #endif
  #ifndef MSG_INFO_PRINT_TIME
    #define MSG_INFO_PRINT_TIME               _UxGT("Всего")
  #endif
  #ifndef MSG_INFO_PRINT_LONGEST
    #define MSG_INFO_PRINT_LONGEST            _UxGT("Наидольшее")
  #endif
  #ifndef MSG_INFO_PRINT_FILAMENT
    #define MSG_INFO_PRINT_FILAMENT           _UxGT("Выдавлено")
  #endif
#endif

#ifndef MSG_INFO_MIN_TEMP
  #define MSG_INFO_MIN_TEMP                   _UxGT("Мин. Т")
#endif
#ifndef MSG_INFO_MAX_TEMP
  #define MSG_INFO_MAX_TEMP                   _UxGT("Макс. Т")
#endif
#ifndef MSG_INFO_PSU
  #define MSG_INFO_PSU                        _UxGT("Блок питания")
#endif
#ifndef MSG_DRIVE_STRENGTH
  #define MSG_DRIVE_STRENGTH                  _UxGT("Сила привода")
#endif
#ifndef MSG_DAC_PERCENT
  #define MSG_DAC_PERCENT                     _UxGT("Привод %")
#endif
#ifndef MSG_DAC_EEPROM_WRITE
  #define MSG_DAC_EEPROM_WRITE                _UxGT("Запись DAC EEPROM")
#endif
#ifndef MSG_FILAMENT_CHANGE_HEADER
  #define MSG_FILAMENT_CHANGE_HEADER          _UxGT("ПЕЧАТЬ НА ПАУЗЕ")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_HEADER
  #define MSG_FILAMENT_CHANGE_OPTION_HEADER   _UxGT("ОПЦИИ ВОЗОБНОВЛЕНИЯ:")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_EXTRUDE
  #define MSG_FILAMENT_CHANGE_OPTION_EXTRUDE  _UxGT("Выдавить ещё")
#endif
#ifndef MSG_FILAMENT_CHANGE_OPTION_RESUME
  #define MSG_FILAMENT_CHANGE_OPTION_RESUME   _UxGT("Возобновить печать")
#endif
#ifndef MSG_FILAMENT_CHANGE_MINTEMP
  #define MSG_FILAMENT_CHANGE_MINTEMP         _UxGT("Мин. температура ")
#endif
#ifndef MSG_FILAMENT_CHANGE_NOZZLE
  #define MSG_FILAMENT_CHANGE_NOZZLE          _UxGT("  Сопла: ")
#endif
#ifndef MSG_ERR_HOMING_FAILED
  #define MSG_ERR_HOMING_FAILED               _UxGT("Возврат не удался")
#endif
#ifndef MSG_ERR_PROBING_FAILED
  #define MSG_ERR_PROBING_FAILED              _UxGT("Не удалось прощупать")
#endif

//
// Filament Change screens show up to 3 lines on a 4-line display
//                        ...or up to 2 lines on a 3-line display
//
#if LCD_HEIGHT >= 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Ожидайте")
    #define MSG_FILAMENT_CHANGE_INIT_2          _UxGT("начала смены")
    #define MSG_FILAMENT_CHANGE_INIT_3          _UxGT("филамента")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Ожидайте")
    #define MSG_FILAMENT_CHANGE_UNLOAD_2        _UxGT("выгрузки прутка")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Вставьте пруток")
    #define MSG_FILAMENT_CHANGE_INSERT_2        _UxGT("и нажмите кнопку")
    #define MSG_FILAMENT_CHANGE_INSERT_3        _UxGT("для продолжения")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEAT_1
    #define MSG_FILAMENT_CHANGE_HEAT_1          _UxGT("Нажмите кнопку для")
    #define MSG_FILAMENT_CHANGE_HEAT_2          _UxGT("нагрева сопла...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Нагрев сопла")
    #define MSG_FILAMENT_CHANGE_HEATING_2       _UxGT("Ждите...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Ожидайте")
    #define MSG_FILAMENT_CHANGE_LOAD_2          _UxGT("загрузки прутка")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Ожидайте")
    #define MSG_FILAMENT_CHANGE_EXTRUDE_2       _UxGT("экструзии прутка")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Ожидайте")
    #define MSG_FILAMENT_CHANGE_RESUME_2        _UxGT("возобновления")
  #endif
#else // LCD_HEIGHT < 4
  #ifndef MSG_FILAMENT_CHANGE_INIT_1
    #define MSG_FILAMENT_CHANGE_INIT_1          _UxGT("Ожидайте...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_UNLOAD_1
    #define MSG_FILAMENT_CHANGE_UNLOAD_1        _UxGT("Выгрузка...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_INSERT_1
    #define MSG_FILAMENT_CHANGE_INSERT_1        _UxGT("Вставь и нажми")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_HEATING_1
    #define MSG_FILAMENT_CHANGE_HEATING_1       _UxGT("Нагрев...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_LOAD_1
    #define MSG_FILAMENT_CHANGE_LOAD_1          _UxGT("Загрузка...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_EXTRUDE_1
    #define MSG_FILAMENT_CHANGE_EXTRUDE_1       _UxGT("Выдавливание...")
  #endif
  #ifndef MSG_FILAMENT_CHANGE_RESUME_1
    #define MSG_FILAMENT_CHANGE_RESUME_1        _UxGT("Возобновление...")
  #endif
#endif // LCD_HEIGHT < 4

#endif // LANGUAGE_RU_H
