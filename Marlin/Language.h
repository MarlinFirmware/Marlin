#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "Configuration.h"

#include "Language_EN.h"
#include "Language_ES.h"
#include "Language_DE.h"
#include "Language_FR.h"
#include "Language_PT.h"
#include "Language_IT.h"
#include "Language_PL.h"

enum class Language
{
	EN = 0, // English
	ES,     // Spanish
	DE,     // German
	FR,     // French
	PT,     // Portuguese
	IT,     // Italian
	PL,     // Polish
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
   else if (LANG == Language::PL) \
   { \
      text = PL_##label; \
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
	SCREEN_LEVEL_COOLING_TITLE,
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
	SCREEN_AUTOHOME_TITLE,
	SCREEN_AUTOHOME_TEXT,

	SCREEN_SETTINGS,
	SCREEN_SETTINGS_TEXT,
	SCREEN_STEPPER,
	SCREEN_MOVE_TITLE,
	SCREEN_MOVE_TEXT,
	SCREEN_MOVE_X,
	SCREEN_MOVE_Y,
	SCREEN_MOVE_Z,
	SCREEN_MOVE_E,
	SCREEN_MOVE_2MOVE,
	SCREEN_MOVE_01MM,
	SCREEN_MOVE_1MM,
	SCREEN_MOVE_10MM,
	SCREEN_MOVE_HEAT_CONFIRM,
	SCREEN_MOVE_BOX0,
	SCREEN_BACK2MAIN,
	SCREEN_PRINT_PRINTING,
	SCREEN_PRINT_TITLE,
	SCREEN_PRINT_COMPLETED,
	SCREEN_PRINT_COMPLETED_BOX,
	SCREEN_PRINT_PAUSE,
	SCREEN_PRINT_PAUSED,
	SCREEN_PRINT_PAUSING,
	SCREEN_PRINT_STOPPED,
	SCREEN_TEMP_TITLE,
	SCREEN_LIGHT,
	SCREEN_AUTOLEVEL,
	SCREEN_PRINT,
	SCREEN_INFO,
	SCREEN_INFO_TEXT,
	SCREEN_INFO_BOX,
	SCREEN_OFFSET_TITLE,
	SCREEN_OFFSET_TEXT,
	SCREEN_OFFSET_DIALOG_TEXT,
	SCREEN_OFFSET_DIALOG_BOX,
	SCREEN_OFFSET_FINISH,
	SCREEN_OFFSET_WAIT,
	SCREEN_OFFSET_WAIT_BOX,
	SCREEN_OFFSET_HOME_TEXT,
	SCREEN_OFFSET_PLANE_TEXT,
	SCREEN_STOP_CONFIRM,
	SCREEN_STOP_BACK,
	SCREEN_STOP_OK,
	SCREEN_CHANGE_TITLE,
	SCREEN_CHANGE_BOX,
	SCREEN_CHANGE_CONFIRM,
	SCREEN_CHANGE_START,
	SCREEN_CHANGE_PULLOUT,
	SCREEN_CHANGE_INSERT,
	SCREEN_CHANGE_RETRY,
	SCREEN_CHANGE_PAUSING,
	SCREEN_HEATING_MAIN_TITLE,
	SCREEN_HEATING_MAIN_BOX,
	SCREEN_SPEED,
	SCREEN_SERIAL_TITLE,
	ICON_SD_NOSD,
	ICON_SD_SD,
	ICON_FILAMENT_UNLOAD,
	ICON_FILAMENT_LOAD,
	ICON_LEVELING,
	ICON_HOMING,
	ICON_SETTINGS,
	ICON_STEPPERS_ON,
	ICON_STEPPERS_OFF,
	ICON_MOVEAXIS,
	ICON_TEMPERATURE,
	ICON_LIGHTLED_DISABLE,
	ICON_LIGHTLED,
	ICON_INFO,
	ICON_PLAY,
	ICON_PAUSE,
	ICON_STOP,
	ICON_CHANGE_FILAMENT,
	ICON_CHANGE_SPEED,
	ICON_BACK,
	ICON_RETRY,
	ICON_OK2,
	ICON_MOVE_X,
	ICON_MOVE_Y,
	ICON_MOVE_Z,
	ICON_MOVE_E,
	ICON_MOVE_10MM,
	ICON_MOVE_1MM,
	ICON_MOVE_01MM,
	ICON_AUTOLEVEL,
	ICON_AUTOLEVEL_DISABLE,
	ICON_OFFSET,
	SELECTOR_BOX,
	PUSH_TO_CONFIRM,
	PUSH_TO_CONTINUE,
	PUSH_TO_START,
	PLEASE_WAIT,
	EMPTY
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
extern const char * MSG_SCREEN_UNLOAD_SELECT_TITLE();
extern const char * MSG_SCREEN_UNLOAD_HEATING_TITLE();
extern const char * MSG_SCREEN_UNLOAD_INFO_TITLE();
extern const char * MSG_SCREEN_UNLOAD_INFO_TEXT();
extern const char * MSG_SCREEN_UNLOADING_TITLE();
extern const char * MSG_SCREEN_UNLOADING_TEXT();
extern const char * MSG_SCREEN_UNLOAD_CONFIRM_TITLE();
extern const char * MSG_SCREEN_UNLOAD_CONFIRM_TEXT();
extern const char * MSG_SCREEN_LOAD_INIT_TITLE();
extern const char * MSG_SCREEN_LOAD_INIT_TEXT();
extern const char * MSG_SCREEN_LOAD_SELECT_TITLE();
extern const char * MSG_SCREEN_LOAD_HEATING_TITLE();
extern const char * MSG_SCREEN_LOAD_INFO_TITLE();
extern const char * MSG_SCREEN_LOAD_INFO_TEXT();
extern const char * MSG_SCREEN_LOADING_TITLE();
extern const char * MSG_SCREEN_LOADING_TEXT();
extern const char * MSG_SCREEN_LOAD_CONFIRM_TITLE();
extern const char * MSG_SCREEN_LOAD_CONFIRM_TEXT();
extern const char * MSG_SCREEN_LEVEL_INIT_TITLE();
extern const char * MSG_SCREEN_LEVEL_INIT_TEXT();
extern const char * MSG_SCREEN_LEVEL_COOLING_TITLE();
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
extern const char * MSG_SCREEN_AUTOHOME_TITLE();
extern const char * MSG_SCREEN_AUTOHOME_TEXT();

extern const char * MSG_SCREEN_SETTINGS();
extern const char * MSG_SCREEN_SETTINGS_TEXT();
extern const char * MSG_SCREEN_STEPPER();
extern const char * MSG_SCREEN_MOVE_TITLE();
extern const char * MSG_SCREEN_MOVE_TEXT();
extern const char * MSG_SCREEN_MOVE_X();
extern const char * MSG_SCREEN_MOVE_Y();
extern const char * MSG_SCREEN_MOVE_Z();
extern const char * MSG_SCREEN_MOVE_E();
extern const char * MSG_SCREEN_MOVE_2MOVE();
extern const char * MSG_SCREEN_MOVE_01MM();
extern const char * MSG_SCREEN_MOVE_1MM();
extern const char * MSG_SCREEN_MOVE_10MM();
extern const char * MSG_SCREEN_MOVE_HEAT_CONFIRM();
extern const char * MSG_SCREEN_MOVE_BOX0();
extern const char * MSG_SCREEN_BACK2MAIN();
extern const char * MSG_SCREEN_PRINT_PRINTING();
extern const char * MSG_SCREEN_PRINT_TITLE();
extern const char * MSG_SCREEN_PRINT_COMPLETED();
extern const char * MSG_SCREEN_PRINT_COMPLETED_BOX();
extern const char * MSG_SCREEN_PRINT_PAUSE();
extern const char * MSG_SCREEN_PRINT_PAUSED();
extern const char * MSG_SCREEN_PRINT_PAUSING();
extern const char * MSG_SCREEN_PRINT_STOPPED();
extern const char * MSG_SCREEN_TEMP_TITLE();
extern const char * MSG_SCREEN_LIGHT();
extern const char * MSG_SCREEN_AUTOLEVEL();
extern const char * MSG_SCREEN_PRINT();
extern const char * MSG_SCREEN_INFO();
extern const char * MSG_SCREEN_INFO_TEXT();
extern const char * MSG_SCREEN_INFO_BOX();
extern const char * MSG_SCREEN_OFFSET_TITLE();
extern const char * MSG_SCREEN_OFFSET_TEXT();
extern const char * MSG_SCREEN_OFFSET_DIALOG_TEXT();
extern const char * MSG_SCREEN_OFFSET_DIALOG_BOX();
extern const char * MSG_SCREEN_OFFSET_FINISH();
extern const char * MSG_SCREEN_OFFSET_WAIT();
extern const char * MSG_SCREEN_OFFSET_WAIT_BOX();
extern const char * MSG_SCREEN_OFFSET_HOME_TEXT();
extern const char * MSG_SCREEN_OFFSET_PLANE_TEXT();
extern const char * MSG_SCREEN_STOP_CONFIRM();
extern const char * MSG_SCREEN_STOP_BACK();
extern const char * MSG_SCREEN_STOP_OK();
extern const char * MSG_SCREEN_CHANGE_TITLE();
extern const char * MSG_SCREEN_CHANGE_BOX();
extern const char * MSG_SCREEN_CHANGE_CONFIRM();
extern const char * MSG_SCREEN_CHANGE_START();
extern const char * MSG_SCREEN_CHANGE_PULLOUT();
extern const char * MSG_SCREEN_CHANGE_INSERT();
extern const char * MSG_SCREEN_CHANGE_RETRY();
extern const char * MSG_SCREEN_CHANGE_PAUSING();
extern const char * MSG_SCREEN_HEATING_MAIN_TITLE();
extern const char * MSG_SCREEN_HEATING_MAIN_BOX();
extern const char * MSG_SCREEN_SPEED();
extern const char * MSG_SCREEN_SERIAL_TITLE();
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
extern const char * MSG_ICON_LIGHTLED_DISABLE();
extern const char * MSG_ICON_LIGHTLED();
extern const char * MSG_ICON_INFO();
extern const char * MSG_ICON_PLAY();
extern const char * MSG_ICON_PAUSE();
extern const char * MSG_ICON_STOP();
extern const char * MSG_ICON_CHANGE_FILAMENT();
extern const char * MSG_ICON_CHANGE_SPEED();
extern const char * MSG_ICON_BACK();
extern const char * MSG_ICON_RETRY();
extern const char * MSG_ICON_OK2();
extern const char * MSG_ICON_MOVE_X();
extern const char * MSG_ICON_MOVE_Y();
extern const char * MSG_ICON_MOVE_Z();
extern const char * MSG_ICON_MOVE_E();
extern const char * MSG_ICON_MOVE_10MM();
extern const char * MSG_ICON_MOVE_1MM();
extern const char * MSG_ICON_MOVE_01MM();
extern const char * MSG_ICON_AUTOLEVEL();
extern const char * MSG_ICON_AUTOLEVEL_DISABLE();
extern const char * MSG_ICON_OFFSET();
extern const char * MSG_SELECTOR_BOX();
extern const char * MSG_PUSH_TO_CONFIRM();
extern const char * MSG_PUSH_TO_CONTINUE();
extern const char * MSG_PUSH_TO_START();
extern const char * MSG_PLEASE_WAIT();
extern const char * MSG_EMPTY();

#endif // ifndef LANGUAGE_H
