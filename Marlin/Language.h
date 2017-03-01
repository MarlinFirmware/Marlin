///////////////////////////////////////////////////////////////////////////////
/// \file Language.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief String definitions for UI.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "Configuration.h"

#include "Language_EN.h"
#include "Language_ES.h"
#include "Language_DE.h"
#include "Language_FR.h"
#include "Language_PT.h"
#include "Language_IT.h"
#include "Language_SE.h"
#include "Language_RU.h"

#undef SE;

enum class Language
{
	EN = 0, // English
	ES,     // Spanish
	DE,     // German
	FR,     // French
	PT,     // Portuguese
	IT,     // Italian
	SE,     // Swedish
	RU,		// Russian
	MAX_LANGUAGES
};
extern Language LANG;

#define TR(label) \
const char * MSG_##label() \
{ \
   const char * text = 0; \
   if (LANG == Language::EN) \
   { \
      text = EN_##label; \
   } \
   else if (LANG == Language::ES) \
   { \
      text = ES_##label; \
   } \
   else if (LANG == Language::DE) \
   { \
      text = DE_##label; \
   } \
   else if (LANG == Language::FR) \
   { \
      text = FR_##label; \
   } \
   else if (LANG == Language::PT) \
   { \
      text = PT_##label; \
   } \
   else if (LANG == Language::IT) \
   { \
      text = IT_##label; \
   } \
   else if (LANG == Language::SE) \
   { \
      text = SE_##label; \
   } \
   else if (LANG == Language::RU) \
   { \
      text = RU_##label; \
   } \
   return text; \
}

enum class Label
{
	NONE = 0,
	SCREEN_EMERGENCY_TITLE,
	SCREEN_EMERGENCY_TEXT,
	SCREEN_EMERGENCY_BOX,
	SCREEN_SD_LIST_TITLE,
	SCREEN_SD_LIST_CONFIRM,
	SCREEN_SD_LIST_BACK,
	SCREEN_SD_LIST_PREV,
	SCREEN_UNLOAD_INIT_TITLE,
	SCREEN_UNLOAD_INIT_TEXT,
	SCREEN_UNLOAD_HOME_TITLE,
	SCREEN_UNLOAD_HOME_TEXT,
	SCREEN_MOVE_TO_UNLOAD_TITLE,
	SCREEN_MOVE_TO_UNLOAD_TEXT,
	SCREEN_UNLOAD_SELECT_TITLE,
	SCREEN_UNLOAD_HEATING_TITLE,
	SCREEN_UNLOAD_INFO_TITLE,
	SCREEN_UNLOAD_INFO_TEXT,
	SCREEN_UNLOADING_TITLE,
	SCREEN_UNLOADING_TEXT,
	SCREEN_UNLOAD_CONFIRM_TITLE,
	SCREEN_UNLOAD_CONFIRM_TEXT,
	SCREEN_LOAD_INIT_TITLE,
	SCREEN_LOAD_INIT_TEXT,
	SCREEN_LOAD_HOME_TITLE,
	SCREEN_LOAD_HOME_TEXT,
	SCREEN_MOVE_TO_LOAD_TITLE,
	SCREEN_MOVE_TO_LOAD_TEXT,
	SCREEN_LOAD_SELECT_TITLE,
	SCREEN_LOAD_HEATING_TITLE,
	SCREEN_LOAD_INFO_TITLE,
	SCREEN_LOAD_INFO_TEXT,
	SCREEN_LOADING_TITLE,
	SCREEN_LOADING_TEXT,
	SCREEN_LOAD_CONFIRM_TITLE,
	SCREEN_LOAD_CONFIRM_TEXT,
	SCREEN_LEVEL_INIT_TITLE,
	SCREEN_LEVEL_INIT_TEXT,
	SCREEN_LEVEL_PREHEATING_TITLE,
	SCREEN_LEVEL_HOMING_TITLE,
	SCREEN_LEVEL_HOMING_TEXT,
	SCREEN_LEVEL1_TITLE,
	SCREEN_LEVEL1_TEXT,
	SCREEN_LEVEL2_TITLE,
	SCREEN_LEVEL2_TEXT,
	SCREEN_LEVEL3_TITLE,
	SCREEN_LEVEL3_TEXT,
	SCREEN_LEVEL_CONFIRM_TITLE,
	SCREEN_LEVEL_CONFIRM_TEXT,
	SCREEN_AUTOHOME_INIT_TITLE,
	SCREEN_AUTOHOME_INIT_TEXT,
	SCREEN_AUTOHOME_HEATING_TITLE,
	SCREEN_AUTOHOME_HOMING_TITLE,
	SCREEN_AUTOHOME_HOMING_TEXT,
	SCREEN_SETTINGS_TITLE,
	SCREEN_SETTINGS_TEXT,
	SCREEN_MOVE_INFO_TITLE,
	SCREEN_MOVE_INFO_TEXT,
	SCREEN_MOVE_TITLE,
	SCREEN_MOVE_TEXT,
	SCREEN_MOVE_CONFIRM_TITLE,
	SCREEN_MOVE_CONFIRM_TEXT,
	SCREEN_MOVE_HEATING_TITLE,
	SCREEN_MOVE_X_TITLE,
	SCREEN_MOVE_X_TEXT,
	SCREEN_MOVE_Y_TITLE,
	SCREEN_MOVE_Y_TEXT,
	SCREEN_MOVE_Z_TITLE,
	SCREEN_MOVE_Z_TEXT,
	SCREEN_MOVE_E_TITLE,
	SCREEN_MOVE_E_TEXT,
	SCREEN_MOVE_X_01MM_TITLE,
	SCREEN_MOVE_X_1MM_TITLE,
	SCREEN_MOVE_X_10MM_TITLE,
	SCREEN_MOVE_Y_01MM_TITLE,
	SCREEN_MOVE_Y_1MM_TITLE,
	SCREEN_MOVE_Y_10MM_TITLE,
	SCREEN_MOVE_Z_01MM_TITLE,
	SCREEN_MOVE_Z_1MM_TITLE,
	SCREEN_MOVE_Z_10MM_TITLE,
	SCREEN_MOVE_E_01MM_TITLE,
	SCREEN_MOVE_E_1MM_TITLE,
	SCREEN_TEMP_TITLE,
	SCREEN_TEMP_HEATING_TITLE,
	SCREEN_INFO_TITLE,
	SCREEN_CONTACT_TITLE,
	SCREEN_CONTACT_TEXT,
	SCREEN_OFFSET_INIT_TITLE,
	SCREEN_OFFSET_INIT_TEXT,
	SCREEN_OFFSET_PREHEATING_TITLE,
	SCREEN_OFFSET_HOME_TITLE,
	SCREEN_OFFSET_HOME_TEXT,
	SCREEN_OFFSET_PLANE_TITLE,
	SCREEN_OFFSET_PLANE_TEXT,
	SCREEN_OFFSET_INFO_TITLE,
	SCREEN_OFFSET_INFO_TEXT,
	SCREEN_OFFSET_SET_TITLE,
	SCREEN_OFFSET_FINISH_TITLE,
	SCREEN_OFFSET_FINISH_TEXT,
	SCREEN_PRINT_HEATING_TITLE,
	SCREEN_PRINT_PRINTING,
	SCREEN_PRINT_PAUSE,
	SCREEN_PRINT_PAUSED,
	SCREEN_PRINT_PAUSING,
	SCREEN_PRINT_COMPLETE_TITLE,
	SCREEN_PRINT_COMPLETE_TEXT,
	SCREEN_STOP_CONFIRM_TITLE,
	SCREEN_CHANGE_INIT_TITLE,
	SCREEN_CHANGE_INIT_TEXT,
	SCREEN_CHANGE_PAUSE_TITLE,
	SCREEN_CHANGE_PAUSE_TEXT,
	SCREEN_CHANGE_SELECT_TITLE,
	SCREEN_CHANGE_HEATING_TITLE,
	SCREEN_MOVE_TO_CHANGE_TITLE,
	SCREEN_MOVE_TO_CHANGE_TEXT,
	SCREEN_CHANGE_INFO1_TITLE,
	SCREEN_CHANGE_INFO1_TEXT,
	SCREEN_CHANGE_UNLOAD_TITLE,
	SCREEN_CHANGE_UNLOAD_TEXT,
	SCREEN_CHANGE_INFO2_TITLE,
	SCREEN_CHANGE_INFO2_TEXT,
	SCREEN_CHANGE_LOAD_TITLE,
	SCREEN_CHANGE_LOAD_TEXT,
	SCREEN_CHANGE_CONFIRM_TITLE,
	SCREEN_CHANGE_CONFIRM_TEXT,
	SCREEN_SPEED_TITLE,
	SCREEN_SERIAL_TITLE,
	SCREEN_WIZARD_TITLE,
	SCREEN_WIZARD_SELECT,
	SCREEN_WIZARD_LANGUAGE,
	SCREEN_WIZARD_TEXT1,
	SCREEN_WIZARD_TEXT2,
	SCREEN_WIZARD_TEXT3,
	SCREEN_WIZARD_TEXT4,
	SCREEN_RESET_INIT_TITLE,
	SCREEN_RESET_INIT_TEXT,
	SCREEN_RESET_INFO_TITLE,
	SCREEN_RESET_INFO_TEXT,
	SCREEN_RESET_TITLE,
	SCREEN_RESET_TEXT,
	SCREEN_ERROR_TITLE,
	SCREEN_ERROR_TEMPERATURE_TEXT,
	ICON_SD_NOSD,
	ICON_SD_SD,
	ICON_FILAMENT_UNLOAD,
	ICON_FILAMENT_LOAD,
	ICON_LEVELING,
	ICON_HOMING,
	ICON_SETTINGS,
	ICON_STEPPERS,
	ICON_STEPPERS_OFF,
	ICON_MOVEAXIS,
	ICON_TEMPERATURE,
	ICON_PLAY,
	ICON_PAUSE,
	ICON_STOP,
	ICON_CHANGE_FILAMENT,
	ICON_CHANGE_SPEED,
	ICON_RETRY,
	ICON_OK2,
	ICON_MOVE_X,
	ICON_MOVE_Y,
	ICON_MOVE_Z,
	ICON_MOVE_E,
	ICON_MOVE_10MM,
	ICON_MOVE_1MM,
	ICON_MOVE_01MM,
	OPTION_LIGHTLED,
	OPTION_BOX_FAN,
	OPTION_INFO,
	OPTION_CONTACT,
	OPTION_AUTOLEVEL,
	OPTION_SERIAL,
	OPTION_OFFSET,
	OPTION_LANGUAGE,
	OPTION_RESET,
	PUSH_TO_CONFIRM,
	PUSH_TO_CONTINUE,
	PUSH_TO_START,
	PUSH_TO_BACK,
	PUSH_TO_FINISH,
	PLEASE_WAIT,
	BACK,
	MODE,
	INACTIVE,
	TEMP_OFF,
	TOTAL_TIME,
	SCREEN_PRINT_STOP_TITLE,
	SCREEN_PRINT_STOP_TEXT,
	SCREEN_CHANGE_WAIT_PAUSE_TITLE,
	SCREEN_CHANGE_WAIT_PAUSE_TEXT,
	OPTION_STATS,
	SCREEN_VIEW_STATS_TITLE,
	SCREEN_VIEW_STATS_TEXT1,
	SCREEN_VIEW_STATS_TEXT2,
	SCREEN_VIEW_STATS_TEXT3,
	SCREEN_NAME_ERROR_TEXT,
	SCREEN_BASE_ERROR_TEXT,
	SCREEN_PTFE_TITLE,
	SCREEN_PTFE_TEXT,
	OPTION_HEATED_BED,
};

extern const char * MSG_SCREEN_EMERGENCY_TITLE();
extern const char * MSG_SCREEN_EMERGENCY_TEXT();
extern const char * MSG_SCREEN_EMERGENCY_BOX();
extern const char * MSG_SCREEN_SD_LIST_TITLE();
extern const char * MSG_SCREEN_SD_LIST_CONFIRM();
extern const char * MSG_SCREEN_SD_LIST_BACK();
extern const char * MSG_SCREEN_SD_LIST_PREV();
extern const char * MSG_SCREEN_UNLOAD_INIT_TITLE();
extern const char * MSG_SCREEN_UNLOAD_INIT_TEXT();
extern const char * MSG_SCREEN_UNLOAD_HOME_TITLE();
extern const char * MSG_SCREEN_UNLOAD_HOME_TEXT();
extern const char * MSG_SCREEN_UNLOAD_SELECT_TITLE();
extern const char * MSG_SCREEN_UNLOAD_HEATING_TITLE();
extern const char * MSG_SCREEN_MOVE_TO_UNLOAD_TITLE();
extern const char * MSG_SCREEN_MOVE_TO_UNLOAD_TEXT();
extern const char * MSG_SCREEN_UNLOAD_INFO_TITLE();
extern const char * MSG_SCREEN_UNLOAD_INFO_TEXT();
extern const char * MSG_SCREEN_UNLOADING_TITLE();
extern const char * MSG_SCREEN_UNLOADING_TEXT();
extern const char * MSG_SCREEN_UNLOAD_CONFIRM_TITLE();
extern const char * MSG_SCREEN_UNLOAD_CONFIRM_TEXT();
extern const char * MSG_SCREEN_LOAD_INIT_TITLE();
extern const char * MSG_SCREEN_LOAD_INIT_TEXT();
extern const char * MSG_SCREEN_LOAD_HOME_TITLE();
extern const char * MSG_SCREEN_LOAD_HOME_TEXT();
extern const char * MSG_SCREEN_LOAD_SELECT_TITLE();
extern const char * MSG_SCREEN_LOAD_HEATING_TITLE();
extern const char * MSG_SCREEN_MOVE_TO_LOAD_TITLE();
extern const char * MSG_SCREEN_MOVE_TO_LOAD_TEXT();
extern const char * MSG_SCREEN_LOAD_INFO_TITLE();
extern const char * MSG_SCREEN_LOAD_INFO_TEXT();
extern const char * MSG_SCREEN_LOADING_TITLE();
extern const char * MSG_SCREEN_LOADING_TEXT();
extern const char * MSG_SCREEN_LOAD_CONFIRM_TITLE();
extern const char * MSG_SCREEN_LOAD_CONFIRM_TEXT();
extern const char * MSG_SCREEN_LEVEL_INIT_TITLE();
extern const char * MSG_SCREEN_LEVEL_INIT_TEXT();
extern const char * MSG_SCREEN_LEVEL_PREHEATING_TITLE();
extern const char * MSG_SCREEN_LEVEL_HOMING_TITLE();
extern const char * MSG_SCREEN_LEVEL_HOMING_TEXT();
extern const char * MSG_SCREEN_LEVEL1_TITLE();
extern const char * MSG_SCREEN_LEVEL1_TEXT();
extern const char * MSG_SCREEN_LEVEL2_TITLE();
extern const char * MSG_SCREEN_LEVEL2_TEXT();
extern const char * MSG_SCREEN_LEVEL3_TITLE();
extern const char * MSG_SCREEN_LEVEL3_TEXT();
extern const char * MSG_SCREEN_LEVEL_CONFIRM_TITLE();
extern const char * MSG_SCREEN_LEVEL_CONFIRM_TEXT();
extern const char * MSG_SCREEN_AUTOHOME_INIT_TITLE();
extern const char * MSG_SCREEN_AUTOHOME_INIT_TEXT();
extern const char * MSG_SCREEN_AUTOHOME_HEATING_TITLE();
extern const char * MSG_SCREEN_AUTOHOME_HOMING_TITLE();
extern const char * MSG_SCREEN_AUTOHOME_HOMING_TEXT();
extern const char * MSG_SCREEN_SETTINGS_TITLE();
extern const char * MSG_SCREEN_SETTINGS_TEXT();
extern const char * MSG_SCREEN_MOVE_INFO_TITLE();
extern const char * MSG_SCREEN_MOVE_INFO_TEXT();
extern const char * MSG_SCREEN_MOVE_TITLE();
extern const char * MSG_SCREEN_MOVE_TEXT();
extern const char * MSG_SCREEN_MOVE_CONFIRM_TITLE();
extern const char * MSG_SCREEN_MOVE_CONFIRM_TEXT();
extern const char * MSG_SCREEN_MOVE_HEATING_TITLE();
extern const char * MSG_SCREEN_MOVE_X_TITLE();
extern const char * MSG_SCREEN_MOVE_X_TEXT();
extern const char * MSG_SCREEN_MOVE_Y_TITLE();
extern const char * MSG_SCREEN_MOVE_Y_TEXT();
extern const char * MSG_SCREEN_MOVE_Z_TITLE();
extern const char * MSG_SCREEN_MOVE_Z_TEXT();
extern const char * MSG_SCREEN_MOVE_E_TITLE();
extern const char * MSG_SCREEN_MOVE_E_TEXT();
extern const char * MSG_SCREEN_MOVE_X_01MM_TITLE();
extern const char * MSG_SCREEN_MOVE_X_1MM_TITLE();
extern const char * MSG_SCREEN_MOVE_X_10MM_TITLE();
extern const char * MSG_SCREEN_MOVE_Y_01MM_TITLE();
extern const char * MSG_SCREEN_MOVE_Y_1MM_TITLE();
extern const char * MSG_SCREEN_MOVE_Y_10MM_TITLE();
extern const char * MSG_SCREEN_MOVE_Z_01MM_TITLE();
extern const char * MSG_SCREEN_MOVE_Z_1MM_TITLE();
extern const char * MSG_SCREEN_MOVE_Z_10MM_TITLE();
extern const char * MSG_SCREEN_MOVE_E_01MM_TITLE();
extern const char * MSG_SCREEN_MOVE_E_1MM_TITLE();
extern const char * MSG_SCREEN_TEMP_TITLE();
extern const char * MSG_SCREEN_TEMP_HEATING_TITLE();
extern const char * MSG_SCREEN_INFO_TITLE();
extern const char * MSG_SCREEN_CONTACT_TITLE();
extern const char * MSG_SCREEN_CONTACT_TEXT();
extern const char * MSG_SCREEN_OFFSET_INIT_TITLE();
extern const char * MSG_SCREEN_OFFSET_INIT_TEXT();
extern const char * MSG_SCREEN_OFFSET_PREHEATING_TITLE();
extern const char * MSG_SCREEN_OFFSET_HOME_TITLE();
extern const char * MSG_SCREEN_OFFSET_HOME_TEXT();
extern const char * MSG_SCREEN_OFFSET_PLANE_TITLE();
extern const char * MSG_SCREEN_OFFSET_PLANE_TEXT();
extern const char * MSG_SCREEN_OFFSET_INFO_TITLE();
extern const char * MSG_SCREEN_OFFSET_INFO_TEXT();
extern const char * MSG_SCREEN_OFFSET_SET_TITLE();
extern const char * MSG_SCREEN_OFFSET_FINISH_TITLE();
extern const char * MSG_SCREEN_OFFSET_FINISH_TEXT();
extern const char * MSG_SCREEN_PRINT_HEATING_TITLE();
extern const char * MSG_SCREEN_PRINT_PRINTING();
extern const char * MSG_SCREEN_PRINT_PAUSE();
extern const char * MSG_SCREEN_PRINT_PAUSED();
extern const char * MSG_SCREEN_PRINT_PAUSING();
extern const char * MSG_SCREEN_PRINT_COMPLETE_TITLE();
extern const char * MSG_SCREEN_PRINT_COMPLETE_TEXT();
extern const char * MSG_SCREEN_STOP_CONFIRM_TITLE();
extern const char * MSG_SCREEN_CHANGE_INIT_TITLE();
extern const char * MSG_SCREEN_CHANGE_INIT_TEXT();
extern const char * MSG_SCREEN_CHANGE_PAUSE_TITLE();
extern const char * MSG_SCREEN_CHANGE_PAUSE_TEXT();
extern const char * MSG_SCREEN_CHANGE_SELECT_TITLE();
extern const char * MSG_SCREEN_CHANGE_HEATING_TITLE();
extern const char * MSG_SCREEN_MOVE_TO_CHANGE_TITLE();
extern const char * MSG_SCREEN_MOVE_TO_CHANGE_TEXT();
extern const char * MSG_SCREEN_CHANGE_INFO1_TITLE();
extern const char * MSG_SCREEN_CHANGE_INFO1_TEXT();
extern const char * MSG_SCREEN_CHANGE_UNLOAD_TITLE();
extern const char * MSG_SCREEN_CHANGE_UNLOAD_TEXT();
extern const char * MSG_SCREEN_CHANGE_INFO2_TITLE();
extern const char * MSG_SCREEN_CHANGE_INFO2_TEXT();
extern const char * MSG_SCREEN_CHANGE_LOAD_TITLE();
extern const char * MSG_SCREEN_CHANGE_LOAD_TEXT();
extern const char * MSG_SCREEN_CHANGE_CONFIRM_TITLE();
extern const char * MSG_SCREEN_CHANGE_CONFIRM_TEXT();
extern const char * MSG_SCREEN_SPEED_TITLE();
extern const char * MSG_SCREEN_SERIAL_TITLE();
extern const char * MSG_SCREEN_WIZARD_TITLE();
extern const char * MSG_SCREEN_WIZARD_SELECT();
extern const char * MSG_SCREEN_WIZARD_LANGUAGE();
extern const char * MSG_SCREEN_WIZARD_TEXT1();
extern const char * MSG_SCREEN_WIZARD_TEXT2();
extern const char * MSG_SCREEN_WIZARD_TEXT3();
extern const char * MSG_SCREEN_WIZARD_TEXT4();
extern const char * MSG_SCREEN_RESET_INIT_TITLE();
extern const char * MSG_SCREEN_RESET_INIT_TEXT();
extern const char * MSG_SCREEN_RESET_INFO_TITLE();
extern const char * MSG_SCREEN_RESET_INFO_TEXT();
extern const char * MSG_SCREEN_RESET_TITLE();
extern const char * MSG_SCREEN_RESET_TEXT();
extern const char * MSG_SCREEN_ERROR_TITLE();
extern const char * MSG_SCREEN_ERROR_TEMPERATURE_TEXT();
extern const char * MSG_ICON_SD_NOSD();
extern const char * MSG_ICON_SD_SD();
extern const char * MSG_ICON_FILAMENT_UNLOAD();
extern const char * MSG_ICON_FILAMENT_LOAD();
extern const char * MSG_ICON_LEVELING();
extern const char * MSG_ICON_HOMING();
extern const char * MSG_ICON_SETTINGS();
extern const char * MSG_ICON_STEPPERS();
extern const char * MSG_ICON_STEPPERS_OFF();
extern const char * MSG_ICON_MOVEAXIS();
extern const char * MSG_ICON_TEMPERATURE();
extern const char * MSG_ICON_PLAY();
extern const char * MSG_ICON_PAUSE();
extern const char * MSG_ICON_STOP();
extern const char * MSG_ICON_CHANGE_FILAMENT();
extern const char * MSG_ICON_CHANGE_SPEED();
extern const char * MSG_ICON_RETRY();
extern const char * MSG_ICON_OK2();
extern const char * MSG_ICON_MOVE_X();
extern const char * MSG_ICON_MOVE_Y();
extern const char * MSG_ICON_MOVE_Z();
extern const char * MSG_ICON_MOVE_E();
extern const char * MSG_ICON_MOVE_10MM();
extern const char * MSG_ICON_MOVE_1MM();
extern const char * MSG_ICON_MOVE_01MM();
extern const char * MSG_OPTION_LIGHTLED();
extern const char * MSG_OPTION_BOX_FAN();
extern const char * MSG_OPTION_INFO();
extern const char * MSG_OPTION_CONTACT();
extern const char * MSG_OPTION_AUTOLEVEL();
extern const char * MSG_OPTION_SERIAL();
extern const char * MSG_OPTION_OFFSET();
extern const char * MSG_OPTION_LANGUAGE();
extern const char * MSG_OPTION_RESET();
extern const char * MSG_PUSH_TO_CONFIRM();
extern const char * MSG_PUSH_TO_CONTINUE();
extern const char * MSG_PUSH_TO_START();
extern const char * MSG_PUSH_TO_BACK();
extern const char * MSG_PUSH_TO_FINISH();
extern const char * MSG_PLEASE_WAIT();
extern const char * MSG_BACK();
extern const char * MSG_MODE();
extern const char * MSG_INACTIVE();
extern const char * MSG_TEMP_OFF();
extern const char * MSG_TOTAL_TIME();
extern const char * MSG_SCREEN_PRINT_STOP_TITLE();
extern const char * MSG_SCREEN_PRINT_STOP_TEXT();
extern const char * MSG_SCREEN_CHANGE_WAIT_PAUSE_TITLE();
extern const char * MSG_SCREEN_CHANGE_WAIT_PAUSE_TEXT();
extern const char * MSG_OPTION_STATS();
extern const char * MSG_SCREEN_VIEW_STATS_TITLE();
extern const char * MSG_SCREEN_VIEW_STATS_TEXT1();
extern const char * MSG_SCREEN_VIEW_STATS_TEXT2();
extern const char * MSG_SCREEN_VIEW_STATS_TEXT3();
extern const char * MSG_SCREEN_NAME_ERROR_TEXT();
extern const char * MSG_SCREEN_BASE_ERROR_TEXT();
extern const char * MSG_SCREEN_PTFE_TITLE();
extern const char * MSG_SCREEN_PTFE_TEXT();
extern const char * MSG_OPTION_HEATED_BED();

#endif // ifndef LANGUAGE_H
