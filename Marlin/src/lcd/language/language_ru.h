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
 * Russian
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 */
#define DISPLAY_CHARSET_ISO10646_5

namespace Language_ru {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Russian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Готов.");
  PROGMEM Language_Str MSG_YES                             = _UxGT("Да");
  PROGMEM Language_Str MSG_NO                              = _UxGT("Нет");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Назад");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Прерывание...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("SD-карта вставлена");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("SD-карта извлечена");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Вставьте SD-карту");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SD_INIT_FAIL                  = _UxGT("Сбой инициализации SD");
  #else
    PROGMEM Language_Str MSG_SD_INIT_FAIL                  = _UxGT("Сбой инициализ. SD");
  #endif
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Ошибка считывания");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB диск удалён");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Ошибка USB диска");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW         = _UxGT("Переполнение вызова");
  #else
    PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW         = _UxGT("Переполн. вызова");
  #endif
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Прогр. эндстопы");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Эндстопы"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Основное меню");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Другие настройки");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Конфигурация");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Автостарт");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Выключить двигатели");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Меню отладки");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Тест индикатора");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Парковка XYZ");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Парковка X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Парковка Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Парковка Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Авто Z-выравнивание");
  PROGMEM Language_Str MSG_ITERATION                       = _UxGT("G34 Итерация: %i");
  PROGMEM Language_Str MSG_DECREASING_ACCURACY             = _UxGT("Уменьшение точности!");
  PROGMEM Language_Str MSG_ACCURACY_ACHIEVED               = _UxGT("Точность достигнута");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Нулевое положение");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Нажмите чтобы начать");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Следующая точка");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Выравнивание готово!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Высота спада");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SET_HOME_OFFSETS              = _UxGT("Установ. смещения дома");
  #else
    PROGMEM Language_Str MSG_SET_HOME_OFFSETS              = _UxGT("Установ.смещ.дома");
  #endif
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Смещения применены");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Установить ноль");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Преднагрев ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Нагрев ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Нагрев ") PREHEAT_1_LABEL _UxGT(" сопло");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Нагрев ") PREHEAT_1_LABEL _UxGT(" сопло ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Нагрев ") PREHEAT_1_LABEL _UxGT(" всё");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Нагрев ") PREHEAT_1_LABEL _UxGT(" стол");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Нагрев ") PREHEAT_1_LABEL _UxGT(" правка");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Преднагрев $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Нагрев $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Нагрев $ сопло");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Нагрев $ сопло ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Нагрев $ всё");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Нагрев $ стол");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Нагрев $ правка");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Нагрев Свой");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Охлаждение");
  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Частота");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Управление лазером");
  PROGMEM Language_Str MSG_LASER_TOGGLE                    = _UxGT("Переключить лазер");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Мощность лазера");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Управление шпинделем");
  PROGMEM Language_Str MSG_SPINDLE_TOGGLE                  = _UxGT("Переключить шпиндель");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SPINDLE_POWER                 = _UxGT("Мощность шпинделя");
  #else
    PROGMEM Language_Str MSG_SPINDLE_POWER                 = _UxGT("Мощн.шпинделя");
  #endif
  PROGMEM Language_Str MSG_SPINDLE_FORWARD                 = _UxGT("Шпиндель вперёд");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Инверсия шпинделя");

  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Включить питание");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Выключить питание");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Экструзия");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Втягивание");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Движение по осям");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Выравнивание стола");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Выровнять стол");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Выровнять углы");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Следующий угол");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_MESH_EDITOR                   = _UxGT("Смещение по Z");
    PROGMEM Language_Str MSG_EDITING_STOPPED               = _UxGT("Правка сетки окончена");
  #else
    PROGMEM Language_Str MSG_MESH_EDITOR                   = _UxGT("Смещение Z");
    PROGMEM Language_Str MSG_EDITING_STOPPED               = _UxGT("Правка окончена");
  #endif
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Редактировать сетку");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Точка сетки");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Индекс X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Индекс Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Значение Z");
  PROGMEM Language_Str MSG_CUSTOM_COMMANDS                 = _UxGT("Свои команды");

  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 тест Z-зонда");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Отклонение");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 точка");
  PROGMEM Language_Str MSG_M48_OUT_OF_BOUNDS               = _UxGT("Зонд за пределами");

  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("Меню IDEX");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Размещение сопел");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Авто парковка");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Размножение");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Зеркальная копия");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Полный контроль");
  PROGMEM Language_Str MSG_IDEX_DUPE_GAP                   = _UxGT("Дублировать X-зазор");

  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2-е сопло X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2-е сопло Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2-е сопло Z");

  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Выполняем G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Инструменты UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Настройка UBL");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Точка разворота");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Ручной ввод сетки");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_BC_INSERT                 = _UxGT("Разместить шайбу и измерить");
    PROGMEM Language_Str MSG_UBL_BC_REMOVE                 = _UxGT("Убрать и замерить стол");
  #else
    PROGMEM Language_Str MSG_UBL_BC_INSERT                 = _UxGT("Разм.шайбу,измерить");
    PROGMEM Language_Str MSG_UBL_BC_REMOVE                 = _UxGT("Убрать, измер. стол");
  #endif
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Измерение");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Двигаемся дальше");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Активировать UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Деактивировать UBL");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Редактор сеток");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Править свою сетку");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_SET_TEMP_BED              = _UxGT("Температура стола");
    PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM           = _UxGT("Температура стола");
    PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND           = _UxGT("Температура сопла");
    PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM        = _UxGT("Температура сопла");
    PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH         = _UxGT("Построить свою сетку");
    PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH         = _UxGT("Правка сетки завершена");
  #else
    PROGMEM Language_Str MSG_UBL_SET_TEMP_BED              = LCD_STR_THERMOMETER _UxGT(" стола, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM           = LCD_STR_THERMOMETER _UxGT(" стола, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND           = LCD_STR_THERMOMETER _UxGT(" сопла, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM        = LCD_STR_THERMOMETER _UxGT(" сопла, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH         = _UxGT("Построить свою");
    PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH         = _UxGT("Правка завершена");
  #endif
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Точная правка сетки");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Построить сетку");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Построить сетку $");
  #endif
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH           = _UxGT("Построить холодную сетку");
  #else
    PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH           = _UxGT("Строить холод.сетку");
  #endif
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Правка высоты сетки");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Высота");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Проверить сетку");
  #if LCD_WIDTH > 21
    #if PREHEAT_COUNT
      PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M         = _UxGT("Проверить сетку $");
    #endif
    PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH      = _UxGT("Проверить свою сетку");
  #else
    #if PREHEAT_COUNT
      PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M         = _UxGT("Провер. сетку $");
    #endif
    PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH      = _UxGT("Провер. свою сетку");
  #endif
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 нагрев стола");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 нагрев сопла");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Ручная грунтовка");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Грунт фикс. длины");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Грунтовка сделана");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 завершена");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Выйти из G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Продолжить сетку");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Выравнивание сетки");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING      = _UxGT("3-х точечное выравнивание");
  #else
    PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING      = _UxGT("3-точечное выравн.");
  #endif
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Калибровка растера");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Выровнять сетку");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Крайние точки");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Тип карты");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Вывести карту сетки");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Вывести на хост");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Вывести в CSV");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP         = _UxGT("Сохранить сетку снаружи");
    PROGMEM Language_Str MSG_UBL_INFO_UBL                  = _UxGT("Вывод информации UBL");
  #else
    PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP         = _UxGT("Сохранить снаружи");
    PROGMEM Language_Str MSG_UBL_INFO_UBL                  = _UxGT("Информация UBL");
  #endif
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Кол-во заполнителя");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Ручное заполнение");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Умное заполнение");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Заполнить сетку");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Аннулировать всё");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Аннулир. ближайшую");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Точно править всё");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Настр. ближ. точку");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Хранилище сеток");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Слот памяти");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Загрузить сетку");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Сохранить сетку");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Сетка %i загружена");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Сетка %i сохранена");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Нет хранилища");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Ошибка: Сохран. UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Ошибка: Восстан.UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Смещение Z: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Смещение Z останов.");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("UBL пошагово");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Строить холодную");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Умное заполнение");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Проверить сетку");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Точно настр. всё");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Проверить сетку");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Точно настр. всё");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Сохранить сетку");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Настройка подсветки");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Подсветка");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Предустановки света");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Красный");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Оранжевый");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Жёлтый");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Зелёный");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Синий");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Индиго");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Фиолетовый");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Белый");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Свет по умолчанию");
  PROGMEM Language_Str MSG_LED_CHANNEL_N                   = _UxGT("Канал =");
  PROGMEM Language_Str MSG_LEDS2                           = _UxGT("Свет #2");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_NEO2_PRESETS                  = _UxGT("Свет #2 предустановки");
  #else
    PROGMEM Language_Str MSG_NEO2_PRESETS                  = _UxGT("Свет #2 предустан.");
  #endif
  PROGMEM Language_Str MSG_NEO2_BRIGHTNESS                 = _UxGT("Яркость");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Свой цвет подсветки");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Уровень красного");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Уровень зелёного");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Уровень синего");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Уровень белого");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Яркость");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Движемся...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Освободить XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Движение по X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Движение по Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Движение по Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Экструдер");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Экструдер *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Сопло не нагрето");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Движение %sмм");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Движение 0.1мм");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Движение 1мм");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Движение 10мм");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Скорость");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z стола");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Сопло, ") LCD_STR_DEGREE "C";
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Сопло ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Сопло запарковано");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Сопло ожидает");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Стол,  ") LCD_STR_DEGREE "C";
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Камера,") LCD_STR_DEGREE "C";
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Кулер");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Кулер ~");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Сохранённый кулер ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Дополн. кулер");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Дополн. кулер ~");
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Контроллер кулера");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Холостые обороты");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Автовключение");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Рабочие обороты");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Период простоя");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Поток");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Поток ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Настройки");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER ", " LCD_STR_DEGREE _UxGT("С  мин");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER ", " LCD_STR_DEGREE _UxGT("С макс");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Фактор");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Автотемпер.");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Вкл");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Выкл");

  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("Автоподбор PID");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("Автоподбор PID *");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("Подбор PID выполнен");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Сбой автоподбора. Плохой экструдер.");
  PROGMEM Language_Str MSG_PID_TEMP_TOO_HIGH               = _UxGT("Сбой автоподбора. Температура повышена.");
  PROGMEM Language_Str MSG_PID_TIMEOUT                     = _UxGT("Сбой автоподбора! Завершение времени.");

  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Выбор");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Выбор *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Ускорение");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Рывок");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-рывок");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-рывок");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-рывок");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-рывок");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_JUNCTION_DEVIATION            = _UxGT("Отклонение узла");
  #else
    PROGMEM Language_Str MSG_JUNCTION_DEVIATION            = _UxGT("Отклон. узла");
  #endif
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Скорость, мм/с");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Скор.макс ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Скор.макс ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Скор.макс ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Скор.макс ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Скор.макс *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Скор.мин");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Перемещение мин");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Ускорение, мм/с2");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Ускор.макс ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Ускор.макс ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Ускор.макс ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Ускор.макс ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Ускор.макс *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Ускор.втягив.");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Ускор.путеш.");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Частота макс.");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Подача мин.");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Шагов/мм");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" шаг/мм");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" шаг/мм");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" шаг/мм");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E шаг/мм");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* шаг/мм");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Температура");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Движение");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Филамент");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E в мм") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT                = _UxGT("E огран.,мм") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT_E              = _UxGT("E огран. *");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_FILAMENT_DIAM                 = _UxGT("Диам. филамента");
    PROGMEM Language_Str MSG_FILAMENT_DIAM_E               = _UxGT("Диам. филамента *");
  #else
    PROGMEM Language_Str MSG_FILAMENT_DIAM                 = _UxGT("Диам. филам.");
    PROGMEM Language_Str MSG_FILAMENT_DIAM_E               = _UxGT("Диам. филам. *");
  #endif
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Загрузка, мм");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Выгрузка, мм");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Kоэф. продвиж.");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Kоэф. продвиж. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Контраст экрана");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Сохранить настройки");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Загрузить настройки");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("На базовые параметры");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_INIT_EEPROM                   = _UxGT("Инициализация EEPROM");
  #else
    PROGMEM Language_Str MSG_INIT_EEPROM                   = _UxGT("Инициализ. EEPROM");
  #endif
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("Сбой EEPROM: CRC");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("Сбой EEPROM: индекс");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("Сбой EEPROM: версия");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Параметры сохранены");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Обновление прошивки");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Сброс принтера");
  PROGMEM Language_Str MSG_REFRESH                         =  LCD_STR_REFRESH  _UxGT(" Обновить");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Главный экран");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Подготовить");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Настроить");
  PROGMEM Language_Str MSG_POWER_MONITOR                   = _UxGT("Монитор питания");
  PROGMEM Language_Str MSG_CURRENT                         = _UxGT("Ток");
  PROGMEM Language_Str MSG_VOLTAGE                         = _UxGT("Напряжение");
  PROGMEM Language_Str MSG_POWER                           = _UxGT("Мощность");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Начало печати");

  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Дальше"); //short text for buttons
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Иниц-я");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Стоп");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Печать");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Сброс");
  PROGMEM Language_Str MSG_BUTTON_IGNORE                   = _UxGT("Игнорир.");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Отмена");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Готово");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Назад");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Продолжить");
  PROGMEM Language_Str MSG_BUTTON_SKIP                     = _UxGT("Пропустить");

  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Пауза печати");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Продолжить печать");
  PROGMEM Language_Str MSG_HOST_START_PRINT                = _UxGT("Старт с хоста");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Остановить печать");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Печать объекта");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Завершить объект");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Завершить объект =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Восстановение сбоя");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Печать с SD карты");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Нет SD карты");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Сон...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Продолжить...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Печать на паузе");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Печать...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Печать отменена");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Печать завершена");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Нет движения.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("УБИТО. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ОСТАНОВЛЕНО. ");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_CONTROL_RETRACT               = _UxGT("Втягивание, мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP          = _UxGT("Смена втягив., мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP  = _UxGT("Возврат смены, мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF = _UxGT("Возврат смены, V");
    PROGMEM Language_Str MSG_AUTORETRACT                   = _UxGT("Автовтягивание");
  #else
    PROGMEM Language_Str MSG_CONTROL_RETRACT               = _UxGT("Втягив., мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP          = _UxGT("Смена втяг.,мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP  = _UxGT("Возвр.смены,мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF = _UxGT("Возвр.смены V");
    PROGMEM Language_Str MSG_AUTORETRACT                   = _UxGT("Автовтягив.");
  #endif
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Подскок, мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Втягивание V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Возврат, мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Возврат V");

  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Поменять длины");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA           = _UxGT("Поменять дополнительно");
  #else
    PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA           = _UxGT("Поменять дополнит.");
  #endif
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Очистить длину");

  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Смена сопел");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Поднятие по Z");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED      = _UxGT("Начальная скор.");
    PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED    = _UxGT("Скорость втягив.");
  #else
    PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED      = _UxGT("Началь.скор.");
    PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED    = _UxGT("Скор.втягив.");
  #endif
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Парковать голову");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Вернуть скорость");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED          = _UxGT("Обороти кулера");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME           = _UxGT("Время кулера");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Авто Вкл.");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Авто Выкл.");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Замена инструмента");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Авто замена");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Последний экструдер");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Замена на *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Смена филамента");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Смена филамента *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Загрузить филамент");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_FILAMENTLOAD_E                = _UxGT("Загрузить филамент *");
    PROGMEM Language_Str MSG_FILAMENTUNLOAD_E              = _UxGT("Выгрузить филамент *");
  #else
    PROGMEM Language_Str MSG_FILAMENTLOAD_E                = _UxGT("Подать филамент *");
    PROGMEM Language_Str MSG_FILAMENTUNLOAD_E              = _UxGT("Убрать филамент *");
  #endif
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Выгрузить всё");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Установить SD карту");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Сменить SD карту");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Освободить SD карту");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z-зонд вне стола");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Фактор наклона");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("Z-зонд BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Тестирование BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Сброс BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Поднять BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Опустить BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("Режим SW");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("Режим 5V");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("Режим OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Режим сохранения");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Установить на 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Установить на OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Слив отчёта");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("ОПАСНОСТЬ: Неправильные параметры приводят к повреждениям! Продолжить?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("Z-Зонд TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Инициализация");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Тест Z-смещения");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Сохранить");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Установить TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Установить зонд");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Загрузить зонд");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Сначала паркуй %s%s%s");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Смещения Z-зонда");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Смещение X");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Смещение Y");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Смещение Z");
  PROGMEM Language_Str MSG_MOVE_NOZZLE_TO_BED              = _UxGT("Двигать сопло к столу");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Микрошаг X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Микрошаг Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Микрошаг Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Сумарно");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Сработал концевик");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Разогрев не удался");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Ошибка:Избыточная Т");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("УТЕЧКА ТЕПЛА");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("УТЕЧКА ТЕПЛА СТОЛА");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("УТЕЧКА ТЕПЛА КАМЕРЫ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Ошибка: Т макс.");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Ошибка: Т мин.");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("ПРИНТЕР ОСТАНОВЛЕН");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Сделайте сброс");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("д"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("ч"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("м"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Нагрев...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Охлаждение...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Нагрев стола...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Охлаждение стола...");
  PROGMEM Language_Str MSG_PROBE_HEATING                   = _UxGT("Нагрев зонда...");
  PROGMEM Language_Str MSG_PROBE_COOLING                   = _UxGT("Охлаждение зонда...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Нагрев камеры...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Охладжение камеры...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Калибровка Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Калибровать X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Калибровать Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Калибровать Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Калибровать центр");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Настройки Delta");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Авто калибровка");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Высота Delta");
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE      = _UxGT("Зондировать Z-смещение");
    PROGMEM Language_Str MSG_DELTA_DIAG_ROD                = _UxGT("Стержень диагонали");
  #else
    PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE      = _UxGT("Зондир. Z-смещения");
    PROGMEM Language_Str MSG_DELTA_DIAG_ROD                = _UxGT("Стержень диаг.");
  #endif
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Высота");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Радиус");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("О принтере");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Данные принтера");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_3POINT_LEVELING               = _UxGT("3-точечное выравнивание");
    PROGMEM Language_Str MSG_LINEAR_LEVELING               = _UxGT("Линейное выравнивание");
    PROGMEM Language_Str MSG_BILINEAR_LEVELING             = _UxGT("Билинейное выравнивание");
  #else
    PROGMEM Language_Str MSG_3POINT_LEVELING               = _UxGT("3-точ. выравнив.");
    PROGMEM Language_Str MSG_LINEAR_LEVELING               = _UxGT("Линейное выравн.");
    PROGMEM Language_Str MSG_BILINEAR_LEVELING             = _UxGT("Билин. выравнив.");
  #endif
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Управление UBL");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Выравнивание сетки");

  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Статистика принтера");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Данные платы");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Термисторы");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Экструдеры");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Скорость,БОД");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Протокол");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF              = _UxGT("Контроль утечки Т: Выкл");
    PROGMEM Language_Str MSG_INFO_RUNAWAY_ON               = _UxGT("Контроль утечки Т: Вкл");
    PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT           = _UxGT("Время простоя хотенда");
  #else
    PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF              = _UxGT("Контр.утечки Т:Выкл");
    PROGMEM Language_Str MSG_INFO_RUNAWAY_ON               = _UxGT("Контр.утечки Т:Вкл");
    PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT           = _UxGT("Время прост.хот-а");
  #endif
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Подсветка корпуса");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Яркость подсветки");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("Неверный принтер");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Счётчик печати");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Общее время печати");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Наидольшее задание");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Длина филамента");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Отпечатков");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Всего");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Наидольшее");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Выдавлено");
  #endif
  PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS           = _UxGT("Завершено");

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Мин.  ") LCD_STR_THERMOMETER;
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Макс. ") LCD_STR_THERMOMETER;
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("БП");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Сила привода");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Привод, %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Привод, %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Привод, %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Привод, %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("СБОЙ СВЯЗИ С TMC");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Запись DAC в EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("ЗАМЕНА ФИЛАМЕНТА");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("ПЕЧАТЬ НА ПАУЗЕ");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("ЗАГРУЗКА ФИЛАМЕНТА");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("ВЫГРУЗКА ФИЛАМЕНТА");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("ОПЦИИ ПРОДОЛЖЕНИЯ:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Выдавить ещё");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Возобновить печать");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Сопла: ");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_RUNOUT_SENSOR                 = _UxGT("Датчик оконч. филамента");
  #else
    PROGMEM Language_Str MSG_RUNOUT_SENSOR                 = _UxGT("Датчик оконч.филам.");
  #endif
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("До конца, мм");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Ошибка парковки");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Ошибка зондирования");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("ВЫБИРЕТЕ ФИЛАМЕНТ");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("Настройки MMU");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE            = _UxGT("Обновить прошивку MMU!");
  #else
    PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE            = _UxGT("Обнови прошивку MMU");
  #endif
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU требует внимания");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Продолжить печать");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Продолжение...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Загрузить филамент");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Загрузить всё");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Загрузить в сопло");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Извлечь филамент");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Извлечь филамент ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Выгрузить филамент");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Загрузка %i...");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT        = _UxGT("Извлечение филамента...");
  #else
    PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT        = _UxGT("Извлеч.филамента...");
  #endif
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Выгрузка....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Всё");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Филамент ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Перезапуск MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("Перезапуск MMU...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Удалите и нажмите");

  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_MIX                           = _UxGT("Смешивание");
  #else
    PROGMEM Language_Str MSG_MIX                           = _UxGT("Смешив.");
  #endif
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Компонент =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Смеситель");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Градиент");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Полный градиент");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Цикличное смешивание");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Градиент смешивания");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Сменить градиент");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_TOGGLE_MIX                    = _UxGT("Переключить смешивание");
    PROGMEM Language_Str MSG_ACTIVE_VTOOL                  = _UxGT("Активация В-инструм.");
    PROGMEM Language_Str MSG_START_VTOOL                   = _UxGT("Начало В-инструмента");
    PROGMEM Language_Str MSG_END_VTOOL                     = _UxGT("Конец В-инструмента");
    PROGMEM Language_Str MSG_GRADIENT_ALIAS                = _UxGT("Псевдоним В-инструмента");
    PROGMEM Language_Str MSG_RESET_VTOOLS                  = _UxGT("Сброс В-инструментов");
    PROGMEM Language_Str MSG_COMMIT_VTOOL                  = _UxGT("Смешать В-инструменты");
    PROGMEM Language_Str MSG_VTOOLS_RESET                  = _UxGT("В-инструменты сброшены");
  #else
    PROGMEM Language_Str MSG_TOGGLE_MIX                    = _UxGT("Перекл. смешивание");
    PROGMEM Language_Str MSG_ACTIVE_VTOOL                  = _UxGT("Актив.В-инструм.");
    PROGMEM Language_Str MSG_START_VTOOL                   = _UxGT("В-инструм.нач.");
    PROGMEM Language_Str MSG_END_VTOOL                     = _UxGT("В-инструм.кон.");
    PROGMEM Language_Str MSG_GRADIENT_ALIAS                = _UxGT("Псевдоним В-инстр.");
    PROGMEM Language_Str MSG_RESET_VTOOLS                  = _UxGT("Сброс В-инструм.");
    PROGMEM Language_Str MSG_COMMIT_VTOOL                  = _UxGT("Смешать В-инструм.");
    PROGMEM Language_Str MSG_VTOOLS_RESET                  = _UxGT("В-инструм. сброшены");
  #endif
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Начало Z");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT(" Конец Z");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Игры");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Кирпичи");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Вторжение");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Змейка");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Лабиринт");

  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_BAD_PAGE                      = _UxGT("Плохой индекс страницы");
    PROGMEM Language_Str MSG_BAD_PAGE_SPEED                = _UxGT("Плохая скорость страницы");
  #else
    PROGMEM Language_Str MSG_BAD_PAGE                      = _UxGT("Плохая страница");
    PROGMEM Language_Str MSG_BAD_PAGE_SPEED                = _UxGT("Плохая скор.стран.");
  #endif

  PROGMEM Language_Str MSG_EDIT_PASSWORD                   = _UxGT("Редактировать пароль");
  PROGMEM Language_Str MSG_LOGIN_REQUIRED                  = _UxGT("Нужен логин");
  PROGMEM Language_Str MSG_PASSWORD_SETTINGS               = _UxGT("Настройки пароля");
  PROGMEM Language_Str MSG_ENTER_DIGIT                     = _UxGT("Введите цифру");
  PROGMEM Language_Str MSG_CHANGE_PASSWORD                 = _UxGT("Смените пароль");
  PROGMEM Language_Str MSG_REMOVE_PASSWORD                 = _UxGT("Удалить пароль");
  PROGMEM Language_Str MSG_PASSWORD_SET                    = _UxGT("Пароль это ");
  PROGMEM Language_Str MSG_START_OVER                      = _UxGT("Старт через");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_REMINDER_SAVE_SETTINGS        = _UxGT("Запомни для сохранения!");
  #else
    PROGMEM Language_Str MSG_REMINDER_SAVE_SETTINGS        = _UxGT("Запомни, сохрани!");
  #endif
  PROGMEM Language_Str MSG_PASSWORD_REMOVED                = _UxGT("Пароль удалён");

  PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING             = _UxGT(MSG_1_LINE("Парковка..."));
  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_3_LINE("Нажмите кнопку", "для продолжения", "печати"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_2_LINE("Ожидайте начала", "смены филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Вставьте филамент", "и нажмите кнопку", "для продолжения"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_3_LINE("Нажмите кнопку", "для нагрева", "сопла..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Нагрев сопла", "Ждите..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Ожидайте", "выгрузки", "филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_3_LINE("Ожидайте", "загрузки", "филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_3_LINE("Ожидайте", "экструзии", "филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_3_LINE("Нажмите кнопку", "для завершения", "очистки филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Ожидайте", "возобновления", "печати"));
  #else
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Продолжить печать"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Ожидайте..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Вставь и нажми"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Нагреть сопло"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Нагрев..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Выгрузка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Загрузка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Выдавливание..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Завершить очистку"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Возобновление..."));
  #endif

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Драйвера TMC");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Ток двигателей");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Гибридный режим");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Режим без эндстопов");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Режим драйвера");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("Тихий режим вкл");

  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Сброс");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" в:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Люфт");
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Исправление");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Сглаживание");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("Уровень оси X");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Авто калибровка");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Время нагревателя вышло");
  #else
    PROGMEM Language_Str MSG_HEATER_TIMEOUT                = _UxGT("Время нагрев. вышло");
  #endif
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Возобновить нагрев");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Нагрев...");

  PROGMEM Language_Str MSG_PROBE_WIZARD                    = _UxGT("Мастер Z-зонда");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_PROBE_WIZARD_PROBING          = _UxGT("Зондиров. контрольной точки Z");
    PROGMEM Language_Str MSG_PROBE_WIZARD_MOVING           = _UxGT("Движение к точке зондирования");
  #else
    PROGMEM Language_Str MSG_PROBE_WIZARD_PROBING          = _UxGT("Зондир.контр.точки Z");
    PROGMEM Language_Str MSG_PROBE_WIZARD_MOVING           = _UxGT("Движ. к точке зондир.");
  #endif

  PROGMEM Language_Str MSG_SOUND                           = _UxGT("Звук");

  PROGMEM Language_Str MSG_TOP_LEFT                        = _UxGT("Верхний левый");
  PROGMEM Language_Str MSG_BOTTOM_LEFT                     = _UxGT("Нижний левый");
  PROGMEM Language_Str MSG_TOP_RIGHT                       = _UxGT("Верхний правый");
  PROGMEM Language_Str MSG_BOTTOM_RIGHT                    = _UxGT("Нижний правый");
  PROGMEM Language_Str MSG_CALIBRATION_COMPLETED           = _UxGT("Калибровка успешна");
  PROGMEM Language_Str MSG_CALIBRATION_FAILED              = _UxGT("Ошибка калибровки");
}

#if FAN_COUNT == 1
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED
  #define MSG_EXTRA_FIRST_FAN_SPEED MSG_EXTRA_FAN_SPEED
#else
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED_N
  #define MSG_EXTRA_FIRST_FAN_SPEED MSG_EXTRA_FAN_SPEED_N
#endif
