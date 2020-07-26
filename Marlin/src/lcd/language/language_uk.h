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
 * Ukrainian
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 */

#define DISPLAY_CHARSET_ISO10646_5

namespace Language_uk {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Ukranian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Готовий.");
  PROGMEM Language_Str MSG_YES                             = _UxGT("ТАК");
  PROGMEM Language_Str MSG_NO                              = _UxGT("НІ");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Назад");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Переривання...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("SD-картка вставлена");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("SD-картка видалена");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Вставте SD-картку");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Помилка зчитування");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("USB диск видалений");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Помилка USB диску");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW         = _UxGT("Переповнення виклику");
    PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS             = _UxGT("Програмні кінцевики");
  #else
    PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW         = _UxGT("Переповн. виклику");
    PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS             = _UxGT("Прогр.кінцевики");
  #endif
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Кінцевик"); // Max length 8 characters
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Основне меню");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Інші налаштування");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Конфігурація");
  PROGMEM Language_Str MSG_AUTOSTART                       = _UxGT("Автостарт");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Вимкнути двигуни");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Меню Debug");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Тест Progress Bar");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Авто паркування");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Паркування X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Паркування Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Паркування Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Авто Z-вирівнювання");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Паркування XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Почати");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Наступна точка");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Завершено!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Висота спаду");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SET_HOME_OFFSETS              = _UxGT("Встанов. зміщення дому");
  #else
    PROGMEM Language_Str MSG_SET_HOME_OFFSETS              = _UxGT("Встанов.зміщ.дому");
  #endif
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Зміщення прийняті");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Встановити ноль");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Нагрів ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Нагрів ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" сопло");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" сопло ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" все");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" стіл");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Нагрів ") PREHEAT_1_LABEL _UxGT(" налашт");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Нагрів $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Нагрів $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Нагрів $ сопло");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Нагрів $ сопло ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Нагрів $ все");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Нагрів $ стіл");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Нагрів $ налашт");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Нагрів Свій");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Вимкнути нагрів");
  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Частота");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Керування лазером");
  PROGMEM Language_Str MSG_LASER_OFF                       = _UxGT("Вимкнути лазер");
  PROGMEM Language_Str MSG_LASER_ON                        = _UxGT("Увімкнути лазер");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_LASER_POWER                   = _UxGT("Потужність лазера");
    PROGMEM Language_Str MSG_SPINDLE_MENU                  = _UxGT("Керування шпінделем");
  #else
    PROGMEM Language_Str MSG_LASER_POWER                   = _UxGT("Потуж.лазера");
    PROGMEM Language_Str MSG_SPINDLE_MENU                  = _UxGT("Керув. шпінделем");
  #endif
  PROGMEM Language_Str MSG_SPINDLE_OFF                     = _UxGT("Вимкнути шпіндель");
  PROGMEM Language_Str MSG_SPINDLE_ON                      = _UxGT("Увімкнути шпіндель");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SPINDLE_POWER                 = _UxGT("Потуж. шпінделя");
  #else
    PROGMEM Language_Str MSG_SPINDLE_POWER                 = _UxGT("Потуж. шпінд.");
  #endif
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Напрямок шпінделя");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Увімкнути живлення");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Вимкнути живлення");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Екструзія");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Втягування");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Рух по осям");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Вирівнювання столу");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Вирівняти стіл");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Вирівняти кути");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Наступний кут");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_MESH_EDITOR                   = _UxGT("Зміщення по Z");
  #else
    PROGMEM Language_Str MSG_MESH_EDITOR                   = _UxGT("Зміщення Z");
  #endif
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Редагувати сітку");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Редагув. зупинено");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Точка сітки");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Індекс X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Індекс Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Значення Z");
  PROGMEM Language_Str MSG_USER_MENU                       = _UxGT("Власні команди");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 тест зонду");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 точка");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Відхилення");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("Меню IDEX");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Зміщення сопел");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Авто паркування");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Розмноження");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Дзеркальна копія");
  PROGMEM Language_Str MSG_IDEX_MODE_FU1L_CTRL             = _UxGT("Повний контроль");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("Друге сопло X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("Друге сопло Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("Друге сопло Z");

  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Виконується G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("Інструменти UBL");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Налаштування UBL");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Точка розвороту");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_MANUAL_MESH               = _UxGT("Ручне введення сітки");
    PROGMEM Language_Str MSG_UBL_BC_INSERT                 = _UxGT("Розмістити шайбу і вимір.");
  #else
    PROGMEM Language_Str MSG_UBL_MANUAL_MESH               = _UxGT("Ручне введ. сітки");
    PROGMEM Language_Str MSG_UBL_BC_INSERT                 = _UxGT("Розм. шайбу і вимір.");
  #endif
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Вимірювання");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Видалити і виміряти");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("До наступної точки");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Активувати UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Деактивувати UBL");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_SET_TEMP_BED              = _UxGT("Температура столу");
    PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM           = _UxGT("Температура столу");
    PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND           = _UxGT("Температура сопла");
    PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM        = _UxGT("Температура сопла");
    PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH          = _UxGT("Редагувати свою сітку");
    PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH            = _UxGT("Точне редагування сітки");
    PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH         = _UxGT("Будувати свою сітку");
  #else
    PROGMEM Language_Str MSG_UBL_SET_TEMP_BED              = LCD_STR_THERMOMETER _UxGT(" столу, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM           = LCD_STR_THERMOMETER _UxGT(" столу, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND           = LCD_STR_THERMOMETER _UxGT(" сопла, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM        = LCD_STR_THERMOMETER _UxGT(" сопла, ") LCD_STR_DEGREE "C";
    PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH          = _UxGT("Редагувати свою");
    PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH            = _UxGT("Точне редаг. сітки");
    PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH         = _UxGT("Будувати свою");
  #endif
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Редагування сітки");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Сітка побудована");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Будувати сітку");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Будувати сітку $");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("Підтвердити $");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Буд. холодну сітку");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Встан.висоту сітки");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Висота");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Підтвердити сітку");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Підтвердити свою");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 нагрів столу");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 нагрів сопла");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Ручне грунтування");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Грунт фікс. довж.");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Грунтув. виконане");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 завершена");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Вийти з G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Продовжити сітку");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Вирівнювання сітки");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-точкове вирівн.");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING        = _UxGT("Вирівнювання растру");
  #else
    PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING        = _UxGT("Вирівнюв. растру");
  #endif
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Вирівняти сітку");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Крайні точки");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Тип мапи сітки");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Вивести мапу сітки");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Вивести на хост");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Вивести в CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Зберегти зовні");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Інформація по UBL");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT             = _UxGT("Обсяг заповнювача");
  #else
    PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT             = _UxGT("Обсяг заповн.");
  #endif
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Ручне заповнення");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Розумне заповнення");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Заповнити сітку");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Анулювати все");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Анулювати найближчу");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Точно налашт. все");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST         = _UxGT("Точно налашт.найближчу");
  #else
    PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST         = _UxGT("Точно найближчу");
  #endif
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Збереження сітки");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Слот пам'яті");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Завантажити сітку");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Зберегти сітку");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Сітка %i завантажена");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Сітка %i збережена");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("Немає носія");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Збій: збереж. UBL");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Збій: відновл. UBL");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Зміщення Z: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Зміщення Z зупинено");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("UBL покроково");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1.Будувати холодну");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2.Розумне заповн-я");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3.Затвердити сітку");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4.Точно налашт.все");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5.Затвердити сітку");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6.Точно налашт.все");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7.Зберегти сітку");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("Керування світлом");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Підсвітка");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_LED_PRESETS                   = _UxGT("Передустановки світла");
  #else
    PROGMEM Language_Str MSG_LED_PRESETS                   = _UxGT("Передустан. світла");
  #endif
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Червоний");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Оранжевий");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Жовтий");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Зелений");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Синій");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Індіго");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Фіолетовий");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("Білий");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("За умовчанням");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Свої кольори");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Рівень червоного");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Рівень зеленого");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Рівень синього");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("Рівень білого");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Яскравість");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Переміщення...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Звільнити XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Рух по X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Рух по Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Рух по Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Екструдер");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Екструдер *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Сопло дуже холодне");
  PROGMEM Language_Str MSG_MOVE_Z_DIST                     = _UxGT("Рух по %sмм");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Рух по 0.1мм");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Рух по 1мм");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Рух по 10мм");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Швидкість");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Z Столу");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Сопло, ") LCD_STR_DEGREE "C";
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Сопло ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Сопло запарковане");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Сопло очикує");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Стіл,  ") LCD_STR_DEGREE "C";
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Камера,") LCD_STR_DEGREE "C";
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Охолодження");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Охолодження ~");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_STORED_FAN_N                  = _UxGT("Збережене охолодж. ~");
    PROGMEM Language_Str MSG_EXTRA_FAN_SPEED               = _UxGT("Додаткове охолодж.");
    PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N             = _UxGT("Додаткове охолодж. ~");
    PROGMEM Language_Str MSG_CONTROLLER_FAN                = _UxGT("Контролер охолодження");
  #else
    PROGMEM Language_Str MSG_STORED_FAN_N                  = _UxGT("Збереж.охолодж. ~");
    PROGMEM Language_Str MSG_EXTRA_FAN_SPEED               = _UxGT("Додат. охолодж.");
    PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N             = _UxGT("Додат.охолодж ~");
    PROGMEM Language_Str MSG_CONTROLLER_FAN                = _UxGT("Контролер охолодж.");
  #endif
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Холості оберти");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Робочі оберти");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Авто-режим");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Період простою");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Потік");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Потік ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Налаштування");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER ", " LCD_STR_DEGREE _UxGT("С  мін");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER ", " LCD_STR_DEGREE _UxGT("С макс");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Фактор");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Автотемпер.");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("Увімк");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Вимк.");

  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("Автопідбір PID");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("Автопідбір PID *");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("Підбір PID виконано");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Збій автопідбору. Поганий екструдер.");
  PROGMEM Language_Str MSG_PID_TEMP_TOO_HIGH               = _UxGT("Збій автопідбору. Температура завищена.");
  PROGMEM Language_Str MSG_PID_TIMEOUT                     = _UxGT("Збій автопідбору! Завершення часу.");

  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Вибрати");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Вибрати *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Прискорорення");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Ривок");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-ривок");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-ривок");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-ривок");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-ривок");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_JUNCTION_DEVIATION            = _UxGT("Відхилення вузла");
  #else
    PROGMEM Language_Str MSG_JUNCTION_DEVIATION            = _UxGT("Відхил.вузла");
  #endif
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Швидкість, мм/с");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Швидк.макс ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Швидк.макс ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Швидк.макс ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Швидк.макс ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Швидк.макс *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Швидк.мін");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("Переміщення мін");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Прискорення, мм/с2");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Приск.макс ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Приск.макс ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Приск.макс ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Приск.макс ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Приск.макс *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("Приск.втягув.");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("Приск.переміщ.");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Частота макс.");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Подача мін.");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Кроків на мм");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT(" кроків/мм");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT(" кроків/мм");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT(" кроків/мм");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("E кроків/мм");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("* кроків/мм");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Температура");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Рух");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Пруток");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E в мм³");
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT                = _UxGT("E обмеж.,мм³");
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT_E              = _UxGT("E обмеж. *");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Діам. прутка");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Діам. прутка *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Вивантаж., мм");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Завантаж., мм");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Kоеф. просув.");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Kоеф. просув. *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("Контраст екрану");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Зберегти в EEPROM");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Зчитати з EEPROM");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("На базові параметри");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_INIT_EEPROM                   = _UxGT("Ініціалізація EEPROM");
  #else
    PROGMEM Language_Str MSG_INIT_EEPROM                   = _UxGT("Ініціаліз. EEPROM");
  #endif
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("Збій EEPROM: CRC");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("Збій EEPROM: індекс");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("Збій EEPROM: версія");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Параметри збережені");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Оновити SD-картку");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Зкинути принтер");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT(" Поновити");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Головний екран");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Підготувати");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Підлаштування");
  PROGMEM Language_Str MSG_POWER_MONITOR                   = _UxGT("Монітор живлення");
  PROGMEM Language_Str MSG_CURRENT                         = _UxGT("Струм");
  PROGMEM Language_Str MSG_VOLTAGE                         = _UxGT("Напруга");
  PROGMEM Language_Str MSG_POWER                           = _UxGT("Потужність");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Почати друк");

  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Далі");  //short text for buttons
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Ініц-я");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Зупинка");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Друк");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Зкинути");
  PROGMEM Language_Str MSG_BUTTON_IGNORE                   = _UxGT("Ігнорув.");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Відміна");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("Готово");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Назад");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Продовжити");

  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Призупинення...");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Призупинити друк");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Відновити друк");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Скасувати друк");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Друк об'єкта");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Завершити об'єкт");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Завершити об'єкт =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Віднов. після збою");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Друкувати з SD");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("SD-картки немає");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Сплячка...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Продовжити...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Друк призупинено");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Друк...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Друк скасовано");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Друк завершено");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("Немає руху.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("ПЕРЕРВАНО. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("ЗУПИНЕНО. ");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_CONTROL_RETRACT               = _UxGT("Втягування, мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP          = _UxGT("Зміна втягув.,мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER       = _UxGT("Повернення, мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP  = _UxGT("Поверн.зміни, мм");
    PROGMEM Language_Str MSG_AUTORETRACT                   = _UxGT("Автовтягування");
  #else
    PROGMEM Language_Str MSG_CONTROL_RETRACT               = _UxGT("Втягув., мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP          = _UxGT("Зміна втяг.мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER       = _UxGT("Поверн., мм");
    PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP  = _UxGT("Повер.зміни,мм");
    PROGMEM Language_Str MSG_AUTORETRACT                   = _UxGT("Автовтягув.");
  #endif
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Втягування V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Підскок, мм");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Повернення V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("Поверн.зміни V");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Поміняти довжини");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA             = _UxGT("Поміняти додатково");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Очистити довжину");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Зміна сопла");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Підняти по Z");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED      = _UxGT("Початк.швидкість");
    PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED    = _UxGT("Швидкість втягув.");
  #else
    PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED      = _UxGT("Початк.швидк.");
    PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED    = _UxGT("Швидк.втягув.");
  #endif
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Паркувати голову");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Відновити швидкість");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED        = _UxGT("Оберти охолодження");
    PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME         = _UxGT("Час охолодження");
  #else
    PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED        = _UxGT("Оберти охолодж.");
    PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME         = _UxGT("Час охолодж.");
  #endif
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Авто Увімк.");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Авто Вимкн.");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Заміна інструменту");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Авто заміна");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Останній екструдер");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Заміна на *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Заміна прутка");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Заміна прутка *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Подати пруток");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Подати пруток *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Видалити пруток");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Видалити пруток *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Видалити все");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Вставити SD-картку");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Заміна SD-картки");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Звільніть SD-картку");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z-Зонд поза столом");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Фактор нахилу");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("Z-зонд BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("BLTouch Само-Тест");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Зкинути BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Підняти зонд");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Опустити зонд");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("Режим SW");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("Режим 5V");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("Режим OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Режим збереження");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Встановити на 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Встановити на OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Злив звіту");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("НЕБЕЗПЕКА: Неправильні параметри приводять до пошкоджень! Продовжувати?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("Z-Зонд TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Ініціалізація");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Тест Z-зміщення");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Зберегти");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Установити TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Установити зонд");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Завантажити зонд");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Дім %s%s%s перший");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Зміщення зонду");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Тест зміщення X");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Тест зміщення Y");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Тест зміщення Z");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Мікрокрок X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Мікрокрок Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Мікрокрок Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Сумарно");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Кінцевик спрацював");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Збій нагріву");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_BED          = _UxGT("Збій нагріву столу");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD_CHAMBER      = _UxGT("Збій нагріву камери");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("ЗАВИЩЕНА Т") LCD_STR_DEGREE;
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("ВИТІК ТЕПЛА");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("ВИТІК ТЕПЛА СТОЛУ");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("ВИТІК ТЕПЛА КАМЕРИ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("ПЕРЕГРІВ");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("МІНІМАЛЬНА Т") LCD_STR_DEGREE;
  PROGMEM Language_Str MSG_ERR_MAXTEMP_BED                 = _UxGT("ПЕРЕГРІВ СТОЛУ");
  PROGMEM Language_Str MSG_ERR_MINTEMP_BED                 = _UxGT("МІНІМАЛЬНА Т") LCD_STR_DEGREE _UxGT(" СТОЛУ");
  PROGMEM Language_Str MSG_ERR_MAXTEMP_CHAMBER             = _UxGT("ПЕРЕГРІВ КАМЕРИ");
  PROGMEM Language_Str MSG_ERR_MINTEMP_CHAMBER             = _UxGT("МІНІМАЛЬНА Т") LCD_STR_DEGREE _UxGT(" КАМЕРИ");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("ПРИНТЕР ЗУПИНЕНО");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Перезавантажте");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("д"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("г"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("х"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Нагрівання...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Охолодження...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Нагрів столу...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Нагрів камери...");
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_BED_COOLING                   = _UxGT("Охолодження столу...");
    PROGMEM Language_Str MSG_CHAMBER_COOLING               = _UxGT("Охолодження камери...");
    PROGMEM Language_Str MSG_DELTA_CALIBRATE               = _UxGT("Калібрування Delta");
  #else
    PROGMEM Language_Str MSG_BED_COOLING                   = _UxGT("Охол. столу...");
    PROGMEM Language_Str MSG_CHAMBER_COOLING               = _UxGT("Охол. камери...");
    PROGMEM Language_Str MSG_DELTA_CALIBRATE               = _UxGT("Калібрув. Delta");
  #endif
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Калібрувати X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Калібрувати Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Калібрувати Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Калібр. центр");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Параметри Delta");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Автокалібрування");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Висота Delta");
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE      = _UxGT("Зондування Z-зміщ.");
    PROGMEM Language_Str MSG_DELTA_DIAG_ROD                = _UxGT("Стрижень діагоналі");
  #else
    PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE      = _UxGT("Зондув. Z-зміщ.");
    PROGMEM Language_Str MSG_DELTA_DIAG_ROD                = _UxGT("Стрижень діаг.");
  #endif
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Висота");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Радіус");

  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Про принтер");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Данні принтера");

  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_3POINT_LEVELING               = _UxGT("3-точкове вирівнювання");
    PROGMEM Language_Str MSG_LINEAR_LEVELING               = _UxGT("Лінійне вирівнювання");
    PROGMEM Language_Str MSG_BILINEAR_LEVELING             = _UxGT("Білінійне вирівнювання");
  #elif LCD_WIDTH == 20
    PROGMEM Language_Str MSG_3POINT_LEVELING               = _UxGT("3-точкове вирівнюв.");
    PROGMEM Language_Str MSG_LINEAR_LEVELING               = _UxGT("Лінійне вирівнюван.");
    PROGMEM Language_Str MSG_BILINEAR_LEVELING             = _UxGT("Білінійне вирівнюв.");
  #else
    PROGMEM Language_Str MSG_3POINT_LEVELING               = _UxGT("3-точк. вирівн.");
    PROGMEM Language_Str MSG_LINEAR_LEVELING               = _UxGT("Лінійне вирівн.");
    PROGMEM Language_Str MSG_BILINEAR_LEVELING             = _UxGT("Білін. вирівнюв.");
  #endif
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Керування UBL");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Вирівнювання сітки");

  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Статистика принтера");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Про плату");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Термістори");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Екструдери");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Біт/секунду");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Протокол");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF              = _UxGT("Контроль витіку ") LCD_STR_THERMOMETER _UxGT(" Вимк");
    PROGMEM Language_Str MSG_INFO_RUNAWAY_ON               = _UxGT("Контроль витіку ") LCD_STR_THERMOMETER _UxGT(" Увімк");
    PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT           = _UxGT("Час простою хотенду");
  #else
    PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF              = _UxGT("Контр.витіку ") LCD_STR_THERMOMETER _UxGT(" Вимк");
    PROGMEM Language_Str MSG_INFO_RUNAWAY_ON               = _UxGT("Контр.витіку ") LCD_STR_THERMOMETER _UxGT(" Увімк");
    PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT           = _UxGT("Час прост. хот-у");
  #endif

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Підсвітка");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Яскравість світла");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("НЕ ТОЙ ПРИНТЕР");
  PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS           = _UxGT("Завершено");
  PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT             = _UxGT("Екструдовано");
  #if LCD_WIDTH >= 20
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Кількість друків");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Весь час друку");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Найдовший час");
  #else
    PROGMEM Language_Str MSG_INFO_PRINT_COUNT              = _UxGT("Друків");
    PROGMEM Language_Str MSG_INFO_PRINT_TIME               = _UxGT("Загалом");
    PROGMEM Language_Str MSG_INFO_PRINT_LONGEST            = _UxGT("Найдовше");
  #endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Мін.  ") LCD_STR_THERMOMETER;
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Макс. ") LCD_STR_THERMOMETER;
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("Блок жив-ня");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Сила мотору");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("Драйвер X, %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Драйвер Y, %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Драйвер Z, %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("Драйвер E, %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("ЗБІЙ ЗВ'ЯЗКУ З TMC");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("Запис ЦАП у EEPROM");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("ЗАМІНА ПРУТКА");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("ЗУПИНКА ДРУКУ");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("ЗАВАНТАЖИТИ ПРУТОК");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("ВИВАНТАЖИТИ ПРУТОК");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER = _UxGT("ПАРАМЕТРИ ПРОДОВЖЕННЯ:");
  #else
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER = _UxGT("ПАРАМ.ПРОДОВЖЕННЯ:");
  #endif
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Видавити ще");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Відновити друк");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Сопло: ");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_RUNOUT_SENSOR                 = _UxGT("Датчик закінчення прутка");
    PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM            = _UxGT("Відстань закінч.,мм");
  #else
    PROGMEM Language_Str MSG_RUNOUT_SENSOR                 = _UxGT("Датчик закінч.прутка");
    PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM            = _UxGT("До закінч.,мм");
  #endif
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Помилка паркування");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Помилка зондування");
  PROGMEM Language_Str MSG_M600_TOO_COLD                   = _UxGT("M600: низька Т") LCD_STR_DEGREE;

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("ОБЕРІТЬ ПРУТОК");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("Налаштування MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("Понови прошивку MMU!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU потребує уваги");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("Продовжити друк");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("Продовження...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("Завантажити пруток");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("Завантажити все");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("Завантажити в сопло");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("Звільнити пруток");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("Звільнити пруток ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("Вивантажити пруток");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Завантаження %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Звільнення прутка...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Вивантаження ....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("Все");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Пруток ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Перезапуск MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("Перезапуск MMU...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Видаліть, натисніть");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_MIX                           = _UxGT("Змішування");
  #else
    PROGMEM Language_Str MSG_MIX                           = _UxGT("Змішув.");
  #endif
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Компонент =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Змішувач");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Градієнт");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Повний градієнт");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Циклічне змішування");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Градієнт змішування");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Змінити градієнт");
  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_TOGGLE_MIX                    = _UxGT("Перемкнути змішування");
    PROGMEM Language_Str MSG_ACTIVE_VTOOL                  = _UxGT("Активація В-інструменту");
    PROGMEM Language_Str MSG_START_VTOOL                   = _UxGT("Початок В-інструменту");
    PROGMEM Language_Str MSG_END_VTOOL                     = _UxGT("Кінець В-інструменту");
    PROGMEM Language_Str MSG_GRADIENT_ALIAS                = _UxGT("Псевдонім В-інструменту");
    PROGMEM Language_Str MSG_RESET_VTOOLS                  = _UxGT("Зкидання В-інструментів");
    PROGMEM Language_Str MSG_VTOOLS_RESET                  = _UxGT("В-інструменти зкинуті");
    PROGMEM Language_Str MSG_COMMIT_VTOOL                  = _UxGT("Змішати В-інструменти");
  #else
    PROGMEM Language_Str MSG_TOGGLE_MIX                    = _UxGT("Перемкнути змішув.");
    PROGMEM Language_Str MSG_ACTIVE_VTOOL                  = _UxGT("Актив.В-інструм.");
    PROGMEM Language_Str MSG_START_VTOOL                   = _UxGT("В-інструм. поч.");
    PROGMEM Language_Str MSG_END_VTOOL                     = _UxGT("В-інструм. кін.");
    PROGMEM Language_Str MSG_GRADIENT_ALIAS                = _UxGT("Псевдонім В-інстр.");
    PROGMEM Language_Str MSG_RESET_VTOOLS                  = _UxGT("Зкидання В-інструм");
    PROGMEM Language_Str MSG_VTOOLS_RESET                  = _UxGT("В-інструм. зкинуті");
    PROGMEM Language_Str MSG_COMMIT_VTOOL                  = _UxGT("Змішати В-інструм.");
  #endif
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Початок Z");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT(" Кінець Z");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Ігри");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Цеглини");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Вторгнення");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Змійка");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Лабіринт");

  #if LCD_WIDTH > 21
    PROGMEM Language_Str MSG_BAD_PAGE                      = _UxGT("Поганий індекс сторінки");
    PROGMEM Language_Str MSG_BAD_PAGE_SPEED                = _UxGT("Погана швидкість сторінки");
  #else
    PROGMEM Language_Str MSG_BAD_PAGE                      = _UxGT("Погана сторінка");
    PROGMEM Language_Str MSG_BAD_PAGE_SPEED                = _UxGT("Погана швидк.стор");
  #endif

  PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING             = _UxGT(MSG_1_LINE("Паркування..."));
  #if LCD_HEIGHT >= 4
    // Up to 3 lines allowed
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_3_LINE("Натисніть кнопку", "для продовження", "друку"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_3_LINE("Зачекайте", "на початок", "заміни прутка"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_3_LINE("Вставте пруток", "та натисніть", "для продовження..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_2_LINE("Натисніть кнопку", "для нагріву сопла"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_2_LINE("Сопло нагрівається", "зачекайте..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_2_LINE("Зачекайте", "на вивід прутка"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_2_LINE("Зачекайте", "на ввід прутка"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_2_LINE("Дочекайтесь", "очищення прутка"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_3_LINE("Натисніть кнопку", "для завершення", "очищення прутка"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_3_LINE("Зачекайте", "на відновлення", "друку"));
  #else
    // Up to 2 lines allowed
    PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING        = _UxGT(MSG_1_LINE("Продовжити друк"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT          = _UxGT(MSG_1_LINE("Зачекайте..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT        = _UxGT(MSG_1_LINE("Вставте і натисніть"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT          = _UxGT(MSG_1_LINE("Нагріти сопло"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING       = _UxGT(MSG_1_LINE("Нагрів сопла..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD        = _UxGT(MSG_1_LINE("Вивід прутка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD          = _UxGT(MSG_1_LINE("Ввід прутка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE         = _UxGT(MSG_1_LINE("Очищення прутка..."));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE    = _UxGT(MSG_1_LINE("Завершити очищення"));
    PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME        = _UxGT(MSG_1_LINE("Поновлення друку..."));
  #endif

  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("Драйвери TMC");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Струм драйвера");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Гібридний поріг");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Дім без кінцевиків");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Режим мікрокроку");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("Тихий режим увімк.");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Зкидання");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" в:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Люфт");
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Виправлення");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Зглажування");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("Рівень вісі X");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Авто калібрування");
  PROGMEM Language_Str MSG_HEATER_TIMEOUT                  = _UxGT("Час нагрівача збіг");
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Поновити нагрів");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Нагрівання...");
}

#if FAN_COUNT == 1
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED
  #define MSG_FIRST_EXTRA_FAN_SPEED MSG_EXTRA_FAN_SPEED
#else
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED_N
  #define MSG_FIRST_EXTRA_FAN_SPEED MSG_EXTRA_FAN_SPEED_N
#endif
