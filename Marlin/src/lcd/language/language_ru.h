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
 * Russian
 *
 * LCD Menu Messages
 * See also http://marlinfw.org/docs/development/lcd_language.html
 *
 */
#define DISPLAY_CHARSET_ISO10646_5

namespace Language_ru {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Russian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" готов.");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Назад");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Карта вставлена");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Карта извлечена");
  PROGMEM Language_Str MSG_MEDIA_RELEASED                  = _UxGT("SD карта не активна");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Эндстопы"); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Прогр. эндстопы");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Меню");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Другие настройки");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Настройки");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Автостарт");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Выкл. двигатели");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Меню отладки");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Тест индикатора");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Авто парковка");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Парковка X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Парковка Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Парковка Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Нулевое положение");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Нажмите чтобы начать");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Следующая точка");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Выравнивание готово!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Высота спада");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Запомнить парковку");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Коррекции применены");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Запомнить ноль");
  PROGMEM Language_Str MSG_PREHEAT_1                       = _UxGT("Преднагрев ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_1_H                     = _UxGT("Преднагрев ") PREHEAT_1_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_1_END                   = _UxGT("Преднагрев ") PREHEAT_1_LABEL _UxGT(" сопло");
  PROGMEM Language_Str MSG_PREHEAT_1_END_E                 = _UxGT("Преднагрев ") PREHEAT_1_LABEL _UxGT(" сопло ~");
  PROGMEM Language_Str MSG_PREHEAT_1_ALL                   = _UxGT("Преднагрев ") PREHEAT_1_LABEL _UxGT(" всё");
  PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY               = _UxGT("Преднагрев ") PREHEAT_1_LABEL _UxGT(" стол");
  PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS              = _UxGT("Преднагрев ") PREHEAT_1_LABEL _UxGT(" настр.");
  PROGMEM Language_Str MSG_PREHEAT_2                       = _UxGT("Преднагрев ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_PREHEAT_2_H                     = _UxGT("Преднагрев ") PREHEAT_2_LABEL " ~";
  PROGMEM Language_Str MSG_PREHEAT_2_END                   = _UxGT("Преднагрев ") PREHEAT_2_LABEL _UxGT(" сопло");
  PROGMEM Language_Str MSG_PREHEAT_2_END_E                 = _UxGT("Преднагрев ") PREHEAT_2_LABEL _UxGT(" сопло ~");
  PROGMEM Language_Str MSG_PREHEAT_2_ALL                   = _UxGT("Преднагрев ") PREHEAT_2_LABEL _UxGT(" всё");
  PROGMEM Language_Str MSG_PREHEAT_2_BEDONLY               = _UxGT("Преднагрев ") PREHEAT_2_LABEL _UxGT(" стол");
  PROGMEM Language_Str MSG_PREHEAT_2_SETTINGS              = _UxGT("Преднагрев ") PREHEAT_2_LABEL _UxGT(" настр.");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Охлаждение");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Включить питание");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Выключить питание");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Экструзия");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Втягивание");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Движение по осям");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Калибровка стола");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Калибровать стол");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Выровнять углы");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Следующий угол");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Редактировать сетку");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Ред. сетки завершено");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Свои команды");

  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("Проверка датчика Z");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Отклонение");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("Измерение");

  // TODO: IDEX Menu
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Размещение сопел");

  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2-е сопло X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2-е сопло Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2-е сопло Z");

  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Выполняем G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Утилиты UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Калибровка UBL");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Постр. сетку от руки");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Пост. шимм и измер.");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Измерение");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Удал. и измер. стол");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Двигаемся дальше");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Активировать UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Деактивировать UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Температура стола");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Температура сопла");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Редактор сеток");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Редакт. свою сетку");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Точная настр. сетки");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Ред. сетки завершено");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Построить свою сетку");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Построить сетку");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M1               = _UxGT("Построить сетку ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_M2               = _UxGT("Построить сетку ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Построить хол. сетку");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Устан. высоту сетки");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Высота");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Проверить сетку");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M1            = _UxGT("Проверить сетку ") PREHEAT_1_LABEL;
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M2            = _UxGT("Проверить сетку ") PREHEAT_2_LABEL;
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Проверить свою сетку");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Продолжить сетку");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Калибровка сетки");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("Калибровка 3-х точек");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Калибровка растера");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Выровнить сетку");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Крайние точки");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Тип карты");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Вывести карту сетки");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Вывести на хост");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Вывести в CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Забекапить сетку");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Вывод информации UBL");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Кол-во заполнителя");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Ручное заполнение");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Умное заполнение");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Заполнить сетку");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Аннулировать всё");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Аннулир. ближ. точку");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Точная настр. всего");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Настр. ближ. точки");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Хранилище сетей");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Слот памяти");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Загрузить сетку стола");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Сохранить сетку стола");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("M117 Сетка %i загружена");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("M117 Сетка %i сохранена");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Нет хранилища");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Ошибка: Сохран. UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Ошибка: Восстан. UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Смещение Z останов.");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Пошаговое UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Построить хол. сетку");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Умное заполнение");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Проверить сетку");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Точная настр. всего");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Проверить сетку");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Точная настр. всего");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Сохранить сетку стола");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Настройки LED");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Подсветку");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Предустановки света");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Красный свет");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Оранжевый свет");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Жёлтый свет");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Зелёный свет");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Синий свет");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Индиго свет");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Фиолетовый свет");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Белый свет");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Свет по умолчанию");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Свои настр. света");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Интенсивн. красного");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Интенсивн. зелёного");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Интенсивн. синего");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Интенсивн. белого");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Яркость");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Движемся...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Освобождаем XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Движение по X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Движение по Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Движение по Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Экструдер");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Экструдер *");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Движение %sмм");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Движение 0.1мм");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Движение 1мм");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Движение 10мм");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Скорость");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z стола");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Сопло");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Сопло ~");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Стол");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Кулер");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Кулер ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Кулер доп.");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Кулер доп. ~");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Поток");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Поток ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Настройки");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Мин");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Макс");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Фактор");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Автотемпература");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Вкл.");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Выкл.");
  PROGMEM Language_Str MSG_PID_P                           = _UxGT("PID-P");
  PROGMEM Language_Str MSG_PID_P_E                         = _UxGT("PID-P *");
  PROGMEM Language_Str MSG_PID_I                           = _UxGT("PID-I");
  PROGMEM Language_Str MSG_PID_I_E                         = _UxGT("PID-I *");
  PROGMEM Language_Str MSG_PID_D                           = _UxGT("PID-D");
  PROGMEM Language_Str MSG_PID_D_E                         = _UxGT("PID-D *");
  PROGMEM Language_Str MSG_PID_C                           = _UxGT("PID-C");
  PROGMEM Language_Str MSG_PID_C_E                         = _UxGT("PID-C *");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Выбор");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Выбор *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Ускорение");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Рывок");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-рывок");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-рывок");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-рывок");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-рывок");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Быстрота");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vмакс ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vмакс ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vмакс ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vмакс ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vмакс *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vмин");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Vпутеш. мин");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Ускорение");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Aмакс ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Aмакс ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Aмакс ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Aмакс ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Aмакс *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-втягивание");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-путеш.");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Шаг/мм");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("шаг/мм");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("шаг/мм");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("шаг/мм");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Eшаг/мм");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*шаг/мм");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Температура");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Движение");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Филамент");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E в мм3");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Диаметр филамента");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Диаметр филамента *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Загрузка мм");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Выгрузка мм");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("K продвижения");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("K продвижения *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Контраст LCD");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Сохранить настройки");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Загрузить настройки");
  PROGMEM Language_Str MSG_RESTORE_FAILSAFE                = _UxGT("Вернуть настройки");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Инициализация EEPROM");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Обновление прошивки");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Сброс принтера");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Обновить");
  PROGMEM Language_Str MSG_WATCH                           = _UxGT("Информационный экран");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Подготовить");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Настроить");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Начало печати");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Дальше");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Инициализация");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Остановить");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Печать");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Сброс");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Отмена");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Готово");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Пауза печати");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Продолжить печать");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Остановить печать");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Восстановение сбоя");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Печать с SD карты");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("Нет SD карты");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Сон...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Продолжить...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Печать на паузе");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Печать отменена");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Нет движения.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("УБИТО. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ОСТАНОВЛЕНО. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Втягивание мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Втягивание смены мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Втягивание V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Втяг. прыжка мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Возврат мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("Возврат смены мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Возврат V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("Возврат смены V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Авто Втягивание");

  // TODO: Filament Change Swap / Purge Length

  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Смена сопел");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Поднятие по Z");

  // TODO: Singlenozzle, nozzle standby

  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Смена филамента");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Смена филамента *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Загрузка филамента");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Загрузка филамента *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Выгрузка филамента *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Выгрузить всё");
  PROGMEM Language_Str MSG_INIT_MEDIA                      = _UxGT("Активировать SD");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Сменить SD карту");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Деактивировать SD");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z датчик вне стола");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Фактор наклона");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Тестирование BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Сброс BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Установка BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Набивка BLTouch");

  // TODO: TouchMI Probe, Manual deploy/stow

  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Паркуй %s%s%s сначала");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Смещение Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Микрошаг X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Микрошаг Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Микрошаг Z");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Сработал концевик");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Разогрев не удался");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Неудача нагрева стола");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Ошибка: Избыточная Т");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("УБЕГАНИЕ ТЕПЛА");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("УБЕГАНИЕ ТЕПЛА СТОЛА");
  // TODO: Heated chamber
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Ошибка: Т макс.");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Ошибка: Т мин.");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("Ошибка: Т стола макс");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("Ошибка: Т стола мин.");
  // TODO: Heated chamber
  PROGMEM Language_Str MSG_ERR_Z_HOMING                    = _UxGT("Паркуй XY сначала");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("ПРИНТЕР ОСТАНОВЛЕН");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Сделайте сброс");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("д"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("ч"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("м"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Нагрев...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Охлаждение...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Нагрев стола...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Охлаждение стола...");
  // TODO: Heated chamber
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Калибровка Delta");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Калибровать X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Калибровать Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Калибровать Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Калибровать центр");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Настройки Delta");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Авто калибровка");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Задать высоту Delta");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Задать Z-смещение");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Диаг. стержень");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Высота");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Радиус");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("О принтере");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Инф. о принтере");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("Калибровка 3-х точек");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Линейная калибровка");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Билинейная калибр.");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Калибровка UBL");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Калибровка сетки");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Статистика принтера");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Информация о плате");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Термисторы");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Экструдеры");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Бод");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Протокол");
  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Подсветка корпуса");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Яркость подсветки");

  PROGMEM Language_Str MSG_EXPECTED_PRINTER                = _UxGT("Неверный принтер");

  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Счётчик печати");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Закончено");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Общее время печати");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Наидольшее задание");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Длина филамента");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Отпечатков");
    PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS         = _UxGT("Закончено");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Всего");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Наидольшее");
    PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT           = _UxGT("Выдавлено");
  #endif
  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Мин. Т");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Макс. Т");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("БП");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Сила привода");
  PROGMEM Language_Str MSG_DAC_PERCENT                     = _UxGT("Привод %");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Запись DAC EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("ПЕЧАТЬ НА ПАУЗЕ");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("ЗАГРУЗКА ФИЛАМЕНТА");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("ВЫГРУЗКА ФИЛАМЕНТА");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("ОПЦИИ ВОЗОБНОВЛЕНИЯ:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Выдавить ещё");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Возобновить печать");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Сопла: ");
  PROGMEM Language_Str MSG_LCD_HOMING_FAILED               = _UxGT("Возврат не удался");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Не удалось прощупать");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: Низкая Т");

  // TODO: MMU2

  // TODO: Mixing

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Игры");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Кирпичи");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Вторжение");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Змейка");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Лабиринт");

  //
  // Filament Change screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
  #if LCD_HEIGHT >= 4
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Ожидайте", "начала смены", "филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_3_LINE("Ожидайте", "выгрузки", "филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Вставьте пруток", "и нажмите кнопку", "для продолжения"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Нажмите кнопку для", "нагрева сопла..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Нагрев сопла", "Ждите..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Ожидайте", "загрузки прутка"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_3_LINE("Ожидайте", "экструзии", "филамента"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Ожидайте", "возобновления", "печати"));
  #else
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Ожидайте..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Выгрузка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Вставь и нажми"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Нагрев..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Загрузка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Выдавливание..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Возобновление..."));
  #endif

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Драйвера TMC");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Текущие настройки");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Гибридный режим");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Режим без эндстопов");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Режим шага");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("Тихий режим вкл");

  // TODO: Service

  // TODO: Backlash
}
