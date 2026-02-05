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
 *
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

#define DISPLAY_CHARSET_ISO10646_5

namespace LanguageNarrow_bg {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Bulgarian");

  LSTR WELCOME_MSG                        = MACHINE_NAME_SUBST _UxGT(" Готов.");               // (MACHINE_NAME_SUBST) Ready.
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Картата е поставена");                      // (MEDIA_TYPE_EN) Inserted
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Картата е извадена");                       // (MEDIA_TYPE_EN) Removed
  LSTR MSG_MAIN_MENU                      = _UxGT("Меню");                                     // Main Menu
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Изкл. двигатели");                          // Disable Steppers
  LSTR MSG_AUTO_HOME                      = _UxGT("Паркиране");                                // Auto Home
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Задай Начало");                             // Set Home Offsets
  LSTR MSG_PREHEAT_1                      = _UxGT("Подгряване ") PREHEAT_1_LABEL;              // Preheat (PREHEAT_1_LABEL)
  LSTR MSG_PREHEAT_1_H                    = _UxGT("Подгряване ") PREHEAT_1_LABEL " ~";         // Preheat (PREHEAT_1_LABEL) ~
  LSTR MSG_PREHEAT_1_END                  = _UxGT("Подгряване ") PREHEAT_1_LABEL _UxGT(" Дюза"); // Preheat (PREHEAT_1_LABEL) End
  LSTR MSG_PREHEAT_1_END_E                = _UxGT("Подгряване ") PREHEAT_1_LABEL _UxGT(" Дюза ~"); // Preheat (PREHEAT_1_LABEL) End ~
  LSTR MSG_PREHEAT_1_ALL                  = _UxGT("Подгр. ") PREHEAT_1_LABEL _UxGT(" Всички"); // Preheat (PREHEAT_1_LABEL) All
  LSTR MSG_PREHEAT_1_BEDONLY              = _UxGT("Подгр. ") PREHEAT_1_LABEL _UxGT(" Легло");  // Preheat (PREHEAT_1_LABEL) Bed
  LSTR MSG_PREHEAT_1_SETTINGS             = _UxGT("Настройки ") PREHEAT_1_LABEL;               // Preheat (PREHEAT_1_LABEL) Conf
  LSTR MSG_PREHEAT_M                      = _UxGT("Подгряване $");                             // Preheat $
  LSTR MSG_PREHEAT_M_H                    = _UxGT("Подгряване $ ~");                           // Preheat $ ~
  LSTR MSG_PREHEAT_M_END                  = _UxGT("Подгряване $ Дюза");                        // Preheat $ End
  LSTR MSG_PREHEAT_M_END_E                = _UxGT("Подгряване $ Дюза ~");                      // Preheat $ End ~
  LSTR MSG_PREHEAT_M_ALL                  = _UxGT("Подгр. $ Всички");                          // Preheat $ All
  LSTR MSG_PREHEAT_M_BEDONLY              = _UxGT("Подгр. $ Легло");                           // Preheat $ Bed
  LSTR MSG_PREHEAT_M_SETTINGS             = _UxGT("Настройки $");                              // Preheat $ Conf
  LSTR MSG_COOLDOWN                       = _UxGT("Охлаждане");                                // Cooldown
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Вкл. захранване");                          // Switch Power On
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Изкл. захранване");                         // Switch Power Off
  LSTR MSG_EXTRUDE                        = _UxGT("Екструзия");                                // Extrude
  LSTR MSG_RETRACT                        = _UxGT("Откат");                                    // Retract
  LSTR MSG_MOVE_AXIS                      = _UxGT("Движение по ос");                           // Move Axis
  LSTR MSG_PROBE_AND_LEVEL                = _UxGT("Нивелиране");                               // Probe and Level
  LSTR MSG_BED_LEVELING                   = _UxGT("Нивелиране");                               // Bed Leveling
  LSTR MSG_LEVEL_BED                      = _UxGT("Нивелиране");                               // Level Bed
  LSTR MSG_MOVE_X                         = _UxGT("Движение по X");                            // Move X
  LSTR MSG_MOVE_Y                         = _UxGT("Движение по Y");                            // Move Y
  LSTR MSG_MOVE_Z                         = _UxGT("Движение по Z");                            // Move Z
  LSTR MSG_MOVE_N                         = _UxGT("Движение по @");                            // Move @
  LSTR MSG_MOVE_E                         = _UxGT("Екструдер");                                // Move Extruder
  LSTR MSG_MOVE_EN                        = _UxGT("Екструдер *");                              // Move *
  LSTR MSG_MOVE_N_MM                      = _UxGT("Премести с $mm");                           // Move $mm
  LSTR MSG_MOVE_N_IN                      = _UxGT("Премести с $in");                           // Move $in
  LSTR MSG_MOVE_N_DEG                     = _UxGT("Премести с $") LCD_STR_DEGREE;              // Move $(LCD_STR_DEGREE)
  LSTR MSG_SPEED                          = _UxGT("Скорост");                                  // Speed
  LSTR MSG_MESH_Z_OFFSET                  = _UxGT("Bed Z");                                    // Bed Z
  LSTR MSG_NOZZLE                         = " " LCD_STR_THERMOMETER _UxGT(" Дюза");            // Nozzle
  LSTR MSG_NOZZLE_N                       = " " LCD_STR_THERMOMETER _UxGT(" Дюза ~");          // Nozzle ~
  LSTR MSG_BED                            = " " LCD_STR_THERMOMETER _UxGT(" Легло");           // Bed
  LSTR MSG_FAN_SPEED                      = _UxGT("Вентилатор");                               // Fan Speed
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Вентилатор ~");                             // Fan Speed ~
  LSTR MSG_FLOW                           = _UxGT("Поток");                                    // Flow
  LSTR MSG_FLOW_N                         = _UxGT("Поток ~");                                  // Flow ~
  LSTR MSG_CONTROL                        = _UxGT("Управление");                               // Control
  LSTR MSG_AUTOTEMP                       = _UxGT("Авто-темп.");                               // Autotemp
  LSTR MSG_A_RETRACT                      = _UxGT("A-откат");                                  // Retract Accel
  LSTR MSG_A_TRAVEL                       = _UxGT("A-travel");                                 // Travel Accel
  LSTR MSG_STEPS_PER_MM                   = _UxGT("Стъпки/mm");                                // Steps/mm
  LSTR MSG_A_STEPS                        = STR_A _UxGT(" стъпки/mm");                         // (STR_A) Steps/mm
  LSTR MSG_B_STEPS                        = STR_B _UxGT(" стъпки/mm");                         // (STR_B) Steps/mm
  LSTR MSG_C_STEPS                        = STR_C _UxGT(" стъпки/mm");                         // (STR_C) Steps/mm
  LSTR MSG_N_STEPS                        = _UxGT("@ стъпки/mm");                              // @ steps/mm
  LSTR MSG_E_STEPS                        = _UxGT("E стъпки/mm");                              // E steps/mm
  LSTR MSG_EN_STEPS                       = _UxGT("* стъпки/mm");                              // * Steps/mm
  LSTR MSG_TEMPERATURE                    = _UxGT("Температура");                              // Temperature
  LSTR MSG_MOTION                         = _UxGT("Движение");                                 // Motion
  LSTR MSG_FILAMENT                       = _UxGT("Нишка");                                    // Filament
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E in mm") SUPERSCRIPT_THREE;                // E in mm(SUPERSCRIPT_THREE)
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Диам. нишка");                              // Fil. Dia.
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Диам. нишка *");                            // Fil. Dia. *
  LSTR MSG_CONTRAST                       = _UxGT("LCD контраст");                             // LCD Contrast
  LSTR MSG_STORE_EEPROM                   = _UxGT("Запази в EPROM");                           // Store Settings
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Зареди от EPROM");                          // Load Settings
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Фабрични настройки");                       // Restore Defaults
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH _UxGT("Обнови");                   // (LCD_STR_REFRESH)Refresh
  LSTR MSG_INFO_SCREEN                    = _UxGT("Преглед");                                  // Info Screen
  LSTR MSG_PREPARE                        = _UxGT("Действия");                                 // Prepare
  LSTR MSG_TUNE                           = _UxGT("Настройка");                                // Tune
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Пауза");                                    // Pause Print
  LSTR MSG_RESUME_PRINT                   = _UxGT("Възобнови печата");                         // Resume Print
  LSTR MSG_STOP_PRINT                     = _UxGT("Спри печата");                              // Stop Print
  LSTR MSG_MEDIA_MENU                     = _UxGT("Меню карта");                               // (MEDIA_TYPE_EN) Print
  LSTR MSG_NO_MEDIA                       = _UxGT("Няма карта");                               // No (MEDIA_TYPE_EN) Detected
  LSTR MSG_DWELL                          = _UxGT("Почивка...");                               // Sleep...
  LSTR MSG_USERWAIT                       = _UxGT("Изчакване");                                // Click to Resume...
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Печатът е прекъснат");                      // Print Aborted
  LSTR MSG_NO_MOVE                        = _UxGT("Няма движение");                            // No Move.
  LSTR MSG_KILLED                         = _UxGT("УБИТО.");                                   // KILLED.
  LSTR MSG_STOPPED                        = _UxGT("СПРЯНО.");                                  // STOPPED.
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Откат mm");                                 // Retract mm
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Смяна Откат mm");                           // Swap Re.mm
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Откат  V");                                 // Retract V
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Скок mm");                                  // Hop mm
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Възврат mm");                               // Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Смяна Възврат mm");                         // S Unretr. mm
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Възврат  V");                               // Unretract V
  LSTR MSG_AUTORETRACT                    = _UxGT("Автоoткат");                                // Auto-Retract
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Смяна нишка");                              // Change Filament
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Смяна нишка *");                            // Change * Filament
  LSTR MSG_ATTACH_MEDIA                   = _UxGT("Иниц. SD-Карта");                           // Attach (MEDIA_TYPE_EN)
  LSTR MSG_ATTACH_SD                      = _UxGT("Иниц. SD-Карта");                           // Attach SD Card
  LSTR MSG_ATTACH_USB                     = _UxGT("Иниц. USB-Карта");                          // Attach USB Drive
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Смяна SD-Карта");                           // Refresh (MEDIA_TYPE_EN)
  LSTR MSG_RUN_AUTOFILES                  = _UxGT("Автостарт");                                // Run Autofiles
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Z-сондата е извадена");                     // Z Probe Past Bed
  LSTR MSG_ZPROBE_XOFFSET                 = _UxGT("X Отстояние");                              // Probe X Offset
  LSTR MSG_ZPROBE_YOFFSET                 = _UxGT("Y Отстояние");                              // Probe Y Offset
  LSTR MSG_ZPROBE_ZOFFSET                 = _UxGT("Z Отстояние");                              // Probe Z Offset
  LSTR MSG_ZPROBE_OFFSET_N                = _UxGT("@ Отстояние");                              // Probe @ Offset
  LSTR MSG_BABYSTEP_PROBE_Z               = _UxGT("Промяна на Z");                             // Babystep Probe Z
  LSTR MSG_BABYSTEP_X                     = _UxGT("Министъпка X");                             // Babystep X
  LSTR MSG_BABYSTEP_Y                     = _UxGT("Министъпка Y");                             // Babystep Y
  LSTR MSG_BABYSTEP_Z                     = _UxGT("Министъпка Z");                             // Babystep Z
  LSTR MSG_BABYSTEP_N                     = _UxGT("Министъпка @");                             // Babystep @
  LSTR MSG_ENDSTOP_ABORT                  = _UxGT("Стоп Кр.Изключватели");                     // Endstop Abort
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Делта Калибровка");                         // Delta Calibration
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Калибровка X");                             // Calibrate X
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Калибровка Y");                             // Calibrate Y
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Калибровка Z");                             // Calibrate Z
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Калибровка Център");                        // Calibrate Center
  LSTR MSG_MIN                            = " " LCD_STR_THERMOMETER _UxGT(" Минимум");         // (LCD_STR_THERMOMETER) Min
  LSTR MSG_MAX                            = " " LCD_STR_THERMOMETER _UxGT(" Максимум");        // (LCD_STR_THERMOMETER) Max
  LSTR MSG_FACTOR                         = " " LCD_STR_THERMOMETER _UxGT(" Фактор");          // (LCD_STR_THERMOMETER) Fact
  LSTR MSG_LCD_ON                         = _UxGT("Вкл.");                                     // On
  LSTR MSG_LCD_OFF                        = _UxGT("Изкл.");                                    // Off
  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Неправилен принтер");                       // INCORRECT PRINTER
}

namespace LanguageWide_bg {
  using namespace LanguageNarrow_bg;
  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2
  #endif
}

namespace LanguageTall_bg {
  using namespace LanguageWide_bg;
  #if LCD_HEIGHT >= 4
    // Filament Change screens show up to 3 lines on a 4-line display
  #endif
}

namespace Language_bg {
  using namespace LanguageTall_bg;
}
