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

//****************俄语***************************//
#define TOOL_TEXT_RU                    "инструмент"
#define PREHEAT_TEXT_RU                 " нагрев"
#define MOVE_TEXT_RU                    "движение"
#define HOME_TEXT_RU                    "домой"
#define PRINT_TEXT_RU                   " печать"
#define EXTRUDE_TEXT_RU                 "экструзия"
#define LEVELING_TEXT_RU                "уровень"
#define AUTO_LEVELING_TEXT_RU           "aвтоуровень"
#define SET_TEXT_RU                     "настройки"
#define MORE_TEXT_RU                    "больше"
#define MORE_GCODE_RU                   "G-код"
#define MORE_ENTER_GCODE_RU             "Введите G-код"

#define ADD_TEXT_RU                     "добавить"
#define DEC_TEXT_RU                     "уменьшить"
#define EXTRUDER_1_TEXT_RU              "экструдер1"
#define EXTRUDER_2_TEXT_RU              "экструдер2"
#define HEATBED_TEXT_RU                 "стол"
#define TEXT_1C_RU                      "1℃"
#define TEXT_5C_RU                      "5℃"
#define TEXT_10C_RU                     "10℃"
#define CLOSE_TEXT_RU                   "выкл"

#define BACK_TEXT_RU                    "назад"

#define TOOL_PREHEAT_RU                 "нагрев"
#define TOOL_EXTRUDE_RU                 "экструдер"
#define TOOL_MOVE_RU                    "движение"
#define TOOL_HOME_RU                    "домой"
#define TOOL_LEVELING_RU                "уровень"
#define TOOL_AUTO_LEVELING_RU           "aвтоуровень"
#define TOOL_FILAMENT_RU                "замена"
#define TOOL_MORE_RU                    "больше"

#define AXIS_X_ADD_TEXT_RU              "X +"
#define AXIS_X_DEC_TEXT_RU              "X -"
#define AXIS_Y_ADD_TEXT_RU              "Y +"
#define AXIS_Y_DEC_TEXT_RU              "Y -"
#define AXIS_Z_ADD_TEXT_RU              "Z +"
#define AXIS_Z_DEC_TEXT_RU              "Z -"
#define TEXT_01MM_RU                    "0.1 mm"
#define TEXT_1MM_RU                     "1 mm"
#define TEXT_10MM_RU                    "10 mm"

#define HOME_X_TEXT_RU                  "X"
#define HOME_Y_TEXT_RU                  "Y"
#define HOME_Z_TEXT_RU                  "Z"
#define HOME_ALL_TEXT_RU                "Home"
#define HOME_STOPMOVE_RU                "Quickstop"

#define PAGE_UP_TEXT_RU                 "вверх"
#define PAGE_DOWN_TEXT_RU               "вниз"

#define EXTRUDER_IN_TEXT_RU             "втянуть"
#define EXTRUDER_OUT_TEXT_RU            "выдавить"
#define EXTRUDE_1MM_TEXT_RU             "1 mm"
#define EXTRUDE_5MM_TEXT_RU             "5 mm"
#define EXTRUDE_10MM_TEXT_RU            "10 mm"
#define EXTRUDE_LOW_SPEED_TEXT_RU       "мин"
#define EXTRUDE_MEDIUM_SPEED_TEXT_RU    "сред"
#define EXTRUDE_HIGH_SPEED_TEXT_RU      "выс"

#define LEVELING_POINT1_TEXT_RU         "1 точка"
#define LEVELING_POINT2_TEXT_RU         "2 точка"
#define LEVELING_POINT3_TEXT_RU         "3 точка"
#define LEVELING_POINT4_TEXT_RU         "4 точка"
#define LEVELING_POINT5_TEXT_RU         "5 точка"

#define FILESYS_TEXT_RU                 "система"
#define WIFI_TEXT_RU                    "WiFi"
#define FAN_TEXT_RU                     "вентилятор"
#define ABOUT_TEXT_RU                   "инфо"
#define BREAK_POINT_TEXT_RU             "продолжить"
#define FILAMENT_TEXT_RU                "замена"
#define LANGUAGE_TEXT_RU                "язык"
#define MOTOR_OFF_TEXT_RU               "откл. мотор"
#define MOTOR_OFF_XY_TEXT_RU            "Off-XY"
#define SHUTDOWN_TEXT_RU                "выключение"
#define MACHINE_PARA_RU                 "конфиг"

#define U_DISK_TEXT_RU                  "U диск"
#define SD_CARD_TEXT_RU                 "SD диск"
#define WIFI_NAME_TEXT_RU               "WiFi: "
#define WIFI_KEY_TEXT_RU                "пароль: "
#define WIFI_IP_TEXT_RU                 "IP: "
#define WIFI_AP_TEXT_RU                 "режим: AP"
#define WIFI_STA_TEXT_RU                "режим: STA"
#define WIFI_CONNECTED_TEXT_RU          "подключен"
#define WIFI_DISCONNECTED_TEXT_RU       "не подключен"
#define WIFI_EXCEPTION_TEXT_RU          "исключение"
#define WIFI_RECONNECT_TEXT_RU          "выбор сети"
#define CLOUD_TEXT_RU                   "облако"
#define CLOUD_BIND_RU                   "соединён"
#define CLOUD_UNBIND_RU                 "отсоед."
#define CLOUD_UNBINDING_RU              "отвязано"
#define CLOUD_DISCONNECTED_RU           "отключено"
#define CLOUD_UNBINDED_RU               "несвяз."
#define CLOUD_BINDED_RU                 "связано"
#define CLOUD_DISABLE_RU                "Disable"

#define FAN_ADD_TEXT_RU                 "добавить"
#define FAN_DEC_TEXT_RU                 "уменьшить"
#define FAN_OPEN_TEXT_RU                "100%"
#define FAN_HALF_TEXT_RU                "50%"
#define FAN_CLOSE_TEXT_RU               "откл"
#define FAN_TIPS1_TEXT_RU               "вентилятор"
#define FAN_TIPS2_TEXT_RU               "вентилятор\nоткл"

#define FILAMENT_IN_TEXT_RU             "втянуть"
#define FILAMENT_OUT_TEXT_RU            "выдавить"
#define FILAMENT_EXT0_TEXT_RU           "экструдер1"
#define FILAMENT_EXT1_TEXT_RU           "экструдер2"
#define FILAMENT_HEAT_TEXT_RU           "нагрев"
#define FILAMENT_STOP_TEXT_RU           "стоп"
#define FILAMENT_TIPS2_TEXT_RU          "T:"
#define FILAMENT_TIPS3_TEXT_RU          "втянуть..."
#define FILAMENT_TIPS4_TEXT_RU          "вядавить..."
#define FILAMENT_TIPS5_TEXT_RU          "Низкая температура, \nнеобходим нагрев"
#define FILAMENT_TIPS6_TEXT_RU          "завершено"

#define FILAMENT_CHANGE_TEXT_RU                       "Please click <Load> \nor <unload>,After \npinter pause."
#define FILAMENT_DIALOG_LOAD_HEAT_TIPS_RU             "Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_UNLOAD_HEAT_TIPS_RU           "Heating up the nozzle,\nplease wait..."
#define FILAMENT_DIALOG_LOAD_CONFIRM1_TIPS_RU         "Heat completed,please load filament \nto extruder,and click <confirm> \nfor start loading."
#define FILAMENT_DIALOG_LOAD_CONFIRM2_TIPS_RU         "Please load filament to extruder,\nand click <confirm> for start loading."
#define FILAMENT_DIALOG_UNLOAD_CONFIRM_TIPS_RU        "Heat completed,please \nclick <confirm> for start unloading.!"
#define FILAMENT_DIALOG_LOADING_TIPS_RU               "Is loading ,please wait!"
#define FILAMENT_DIALOG_UNLOADING_TIPS_RU             "Is unloading,please wait!"
#define FILAMENT_DIALOG_LOAD_COMPLETE_TIPS_RU         "Load filament completed,\nclick <confirm> for return!"
#define FILAMENT_DIALOG_UNLOAD_COMPLETE_TIPS_RU       "Unload filament completed,\nclick <confirm> for return!"

#define PRE_HEAT_EXT_TEXT_RU                "E"
#define PRE_HEAT_BED_TEXT_RU                "стол"

#define FILE_LOADING_RU                     "загрузка......"
#define NO_FILE_AND_CHECK_RU                "нет файла,попробуйте ещё раз!"

#define NO_FILE_RU                          "нет файла!"

#define EXTRUDER_TEMP_TEXT_RU               "температура"
#define EXTRUDER_E_LENGTH1_TEXT_RU          "экструзия1"
#define EXTRUDER_E_LENGTH2_TEXT_RU          "экструзия2"
#define EXTRUDER_E_LENGTH3_TEXT_RU          "экструзия3"

#define ABOUT_TYPE_TEXT_RU                  "Type: "
#define ABOUT_VERSION_TEXT_RU               "Firmware: "
#define ABOUT_WIFI_TEXT_RU                  "WiFi: "

#define PRINTING_OPERATION_RU               "опции"
#define PRINTING_PAUSE_RU                   "пауза"
#define PRINTING_TEMP_RU                    "темп"
#define PRINTING_CHANGESPEED_RU             "скорости"
#define PRINTING_RESUME_RU                  "возобн.   "
#define PRINTING_STOP_RU                    "стоп"
#define PRINTING_MORE_RU                    "больше"
#define PRINTING_EXTRUDER_RU                "экстр"
#define PRINTING_MOVE_RU                    "движение"

#define EXTRUDER_SPEED_RU                   "экстр"
#define MOVE_SPEED_RU                       "движ"
#define EXTRUDER_SPEED_STATE_RU             "скорость экстр"
#define MOVE_SPEED_STATE_RU                 "скорость движ"
#define STEP_1PERCENT_RU                    "1%"
#define STEP_5PERCENT_RU                    "5%"
#define STEP_10PERCENT_RU                   "10%"

#define TITLE_READYPRINT_RU                 "готов к"
#define TITLE_PREHEAT_RU                    "движение"
#define TITLE_MOVE_RU                       "движение"
#define TITLE_HOME_RU                       "Home"
#define TITLE_EXTRUDE_RU                    "экструзия"
#define TITLE_LEVELING_RU                   "уровень"
#define TITLE_MLEVELING_RU                  "углы"
#define TITLE_SET_RU                        "настройки"
#define TITLE_MORE_RU                       "больше"
#define TITLE_CHOOSEFILE_RU                 "файла"
#define TITLE_PRINTING_RU                   "печать"
#define TITLE_OPERATION_RU                  "управление"
#define TITLE_ADJUST_RU                     "регулировать"
#define TITLE_WIRELESS_RU                   "Wireless"
#define TITLE_FILAMENT_RU                   "замена"
#define TITLE_ABOUT_RU                      "инфо"
#define TITLE_FAN_RU                        "вентилятор"
#define TITLE_LANGUAGE_RU                   "язык"
#define TITLE_PAUSE_RU                      "пауза"
#define TITLE_CHANGESPEED_RU                "скорости"
#define TILE_TOOL_RU                        "инструмент"
#define TITLE_CLOUD_TEXT_RU                 "Cloud"
#define TITLE_DIALOG_CONFIRM_RU             "Confirm"
#define TITLE_FILESYS_RU                    "FileSys"

#define AUTO_SHUTDOWN_RU                    "авто-откл"
#define MANUAL_SHUTDOWN_RU                  "ручн-откл"

#define DIALOG_CONFIRM_RU                   "да"//"подтвердить"
#define DIALOG_CANCLE_RU                    "отмена"
#define DIALOG_OK_RU                        "да"
#define DIALOG_RESET_RU                     "сброс"
#define DIALOG_RETRY_RU                     "повтор"
#define DIALOG_DISABLE_RU                   "запретить"
#define DIALOG_PRINT_MODEL_RU               "печать модели?"
#define DIALOG_CANCEL_PRINT_RU              "стоп?"
#define DIALOG_STOP_RU                      "стоп"
#define DIALOG_REPRINT_FROM_BREAKPOINT_RU   "продолжить?"
#define DIALOG_ERROR_TIPS1_RU               "ошибка:нет файла, попробуйте ещё раз."
#define DIALOG_ERROR_TIPS2_RU               "ошибка:сбой передачи. установите скорость \nпередачи данных как на плате управления!"
#define DIALOG_ERROR_TIPS3_RU               "ошибка: имя файла слишком длинное!"
#define DIALOG_CLOSE_MACHINE_RU             "Closing machine......"
#define DIALOG_UNBIND_PRINTER_RU            "Unbind the printer?"
#define DIALOG_FILAMENT_NO_PRESS_RU         "Filament detection switch is not pressed"
#define DIALOG_PRINT_FINISH_RU              "печать завершена!"
#define DIALOG_PRINT_TIME_RU                "Время печати: "
#define DIALOG_REPRINT_RU                   "Print again"
#define DIALOG_WIFI_ENABLE_TIPS_RU          "The wifi module is being configured,\nplease wait a moment....."

#define MESSAGE_PAUSING_RU            "Стоянка..."
#define MESSAGE_CHANGING_RU           "Подождите, пока начнется смена филамента"
#define MESSAGE_UNLOAD_RU             "Дождитесь выгрузки нити"
#define MESSAGE_WAITING_RU            "Нажмите кнопку,чтобы возобновить печать"
#define MESSAGE_INSERT_RU             "Вставьте нить и нажмите кнопку,чтобы продолжить"
#define MESSAGE_LOAD_RU               "Дождитесь загрузки нити"
#define MESSAGE_PURGE_RU              "Дождитесь чистки нити"
#define MESSAGE_RESUME_RU             "Подождите,пока печать возобновится ..."
#define MESSAGE_HEAT_RU               "Нажмите кнопку, чтобы нагреть форсунку"
#define MESSAGE_HEATING_RU            "Подогрев форсунки Пожалуйста, подождите ..."
#define MESSAGE_OPTION_RU             "Очистить больше или продолжить печать?"
#define MESSAGE_PURGE_MORE_RU         "чистка"
#define MESSAGE_CONTINUE_PRINT_RU     "Распечатать"
#define EEPROM_SETTINGS_TITLE_RU      "Настройки EEPROM"
#define EEPROM_SETTINGS_STORE_RU      "Cохранение настроек в EEPROM"
#define EEPROM_SETTINGS_READ_RU       "Чтение настроек из EEPROM"
#define EEPROM_SETTINGS_REVERT_RU     "Bосстановить заводские настройки по умолчанию"

#define MORE_CUSTOM1_TEXT_RU  USER_DESC_1
#define MORE_CUSTOM2_TEXT_RU  USER_DESC_2
#define MORE_CUSTOM3_TEXT_RU  USER_DESC_3
#define MORE_CUSTOM4_TEXT_RU  USER_DESC_4
#define MORE_CUSTOM5_TEXT_RU  USER_DESC_5
#define MORE_CUSTOM6_TEXT_RU  USER_DESC_6
#define MORE_CUSTOM7_TEXT_RU  USER_DESC_7

//Malderin translate
//
//
#define EEPROM_STORE_TIPS_RU          "Cохранить настройки в EEPROM?"
#define EEPROM_READ_TIPS_RU           "читать настройки из EEPROM?"
#define EEPROM_REVERT_TIPS_RU         "Cбросить настройки к значениям по умолчанию?"
#define EEPROM_SETTINGS_RU            "EEPROM"

#define NEXT_RU                       "след."
#define PREVIOUS_RU                   "пред."
#define ENABLE_RU                     "да "
#define DISABLE_RU                    "нет"
#define KEY_CONFIRM_RU                "OK"

#define MACHINE_PARA_TITLE_RU          "настройки"
#define MACHINE_TYPE_CNOFIG_RU         "Hастройки принтера"
#define MOTOR_CONFIG_RU                "Hастройки моторов"
#define MACHINE_LEVELING_CONFIG_RU     "Hастройки уровня"
#define ADVANCE_CONFIG_RU              "Pасширенные настройки"
#define MACHINE_FILAMENT_CONFIG_RU     "Hастройки филамента"
#define ENCODER_SETTINGS_RU            "Hастройки энкодера"

#define LEVELING_CONF_TITLE_RU          "Hастройки принтера>Hастройки уровня"
#define LEVELING_PARA_CONF_RU           "настройки уровня"
#define LEVELING_MANUAL_POS_RU          "настройки координат для уровня"
#define LEVELING_AUTO_COMMAND_RU        "настройки комманд увтоуровня"
#define LEVELING_AUTO_ZOFFSET_RU        "координаты смещения сопла"

#define MACHINE_CONFIG_TITLE_RU         "Hастройки принтера>настройки притера"
#define MAXFEEDRATE_CONF_RU             "настройки максимальной скорости"
#define ACCELERATION_CONF_RU            "настройки ускорений"
#define JERKCONF_RU                     "настройки рывков"

#define MOTOR_CONF_TITLE_RU             "Hастройки принтера>Hастройки моторов"
#define STEPSCONF_RU                    "настройки шагов"
#define TMC_CURRENT_RU                  "TMC настройки токов"
#define TMC_STEP_MODE_RU                "TMC настрйоки режима шагов"

#define ACCELERATION_CONF_TITLE_RU      "Hастройки принтера>ускорения"
#define PRINT_ACCELERATION_RU           "ускорение печати"
#define RETRACT_ACCELERATION_RU         "ускорение ретракта"
#define TRAVEL_ACCELERATION_RU          "ускорение перемещений"
#define X_ACCELERATION_RU               "ускорение оси X"
#define Y_ACCELERATION_RU               "ускорение оси Y"
#define Z_ACCELERATION_RU               "ускорение оси Z"
#define E0_ACCELERATION_RU              "ускорение E0"
#define E1_ACCELERATION_RU              "ускорение E1"

#define MAXFEEDRATE_CONF_TITLE_RU       "Hастройки принтера>максимальная скорость"
#define X_MAXFEEDRATE_RU                "максимальная скорость оси X"
#define Y_MAXFEEDRATE_RU                "максимальная скорость оси Y"
#define Z_MAXFEEDRATE_RU                "максимальная скорость оси Z"
#define E0_MAXFEEDRATE_RU               "максимальная скорость E0"
#define E1_MAXFEEDRATE_RU               "максимальная скорость E1"

#define JERK_CONF_TITLE_RU              "Hастройки принтера>скорость рывка"
#define X_JERK_RU                       "скорость рывка оси X"
#define Y_JERK_RU                       "скорость рывка оси Y"
#define Z_JERK_RU                       "скорость рывка оси Z"
#define E_JERK_RU                       "скорость рывка оси E"

#define STEPS_CONF_TITLE_RU             "Hастройки принтера>настройки шагов"
#define X_STEPS_RU                      "шаги оси X"
#define Y_STEPS_RU                      "шаги оси Y"
#define Z_STEPS_RU                      "шаги оси Z"
#define E0_STEPS_RU                     "шаги E0"
#define E1_STEPS_RU                     "шаги E1"

#define TMC_CURRENT_CONF_TITLE_RU       "Hастройки принтера>TMC настройка токов"
#define X_TMC_CURRENT_RU                "ток оси X (mA)"
#define Y_TMC_CURRENT_RU                "ток оси Y (mA)"
#define Z_TMC_CURRENT_RU                "ток оси Z (mA)"
#define E0_TMC_CURRENT_RU               "ток E0 (mA)"
#define E1_TMC_CURRENT_RU               "ток E1 (mA)"

#define TMC_MODE_CONF_TITLE_RU          "Hастройки принтера>TMC настройки режима шагов"
#define X_TMC_MODE_RU                   "включает ли двигатель X режим StealthChop"
#define Y_TMC_MODE_RU                   "включает ли ось Y режим StealthChop"
#define Z_TMC_MODE_RU                   "включает ли ось Z режим StealthChop"
#define E0_TMC_MODE_RU                  "включает ли E0 режим StealthChop"
#define E1_TMC_MODE_RU                  "включает ли E1 режим StealthChop"

#define ADVANCED_CONF_TITLE_RU          "Hастройки принтера>Pасширенные"
#define PAUSE_POSITION_RU    "Hастройки позиции паузы печати"
#define PAUSE_POSITION_X_RU  "положение по X (абс. полож., -1 недействит.)"
#define PAUSE_POSITION_Y_RU  "положение по Y (абс. полож., -1 недействит.)"
#define PAUSE_POSITION_Z_RU  "положение по Z (абс. полож., -1 недействит.)"

#define OFFSET_TITLE_RU  "Hастройки принтера>отступ"
#define OFFSET_X_RU      "X отступ"
#define OFFSET_Y_RU      "Y отступ"
#define OFFSET_Z_RU      "Z отступ"

#define FILAMENT_CONF_TITLE_RU          "Hастройки принтера>Hастройки филамента"
#define FILAMENT_IN_LENGTH_RU           "длинна загрузки"
#define FILAMENT_IN_SPEED_RU            "скорость загрузки"
#define FILAMENT_TEMPERATURE_RU         "температура филамента"
#define FILAMENT_OUT_LENGTH_RU          "длинна извлечения"
#define FILAMENT_OUT_SPEED_RU           "скорость извлечения"

#define ENCODER_CONF_TITLE_RU           "Hастройки принтера>Hастройки энкодера"
#define ENCODER_CONF_TEXT_RU            "энкодер используется?"

//end of Malderin translate
