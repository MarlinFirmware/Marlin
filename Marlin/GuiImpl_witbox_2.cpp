#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"
#include "GuiAction.h"

#include "Icon.h"
#include "IconWidget.h"
#include "IconStatus.h"

#include "Screen.h"
#include "ScreenMenu.h"
#include "ScreenPrint.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenList.h"
#include "ScreenAnimation.h"
#include "ScreenTransition.h"
#include "ScreenAction.h"
#include "ScreenPrint.h"
#include "ScreenDynamic.h"
#include "ScreenAbout.h"

#include "AutoLevelManager.h"
#include "LightManager.h"
#include "PrintManager.h"
#include "SDManager.h"
#include "SteppersManager.h"
#include "TemperatureManager.h"

#include "language.h"

namespace screen
{
	///////////////////////
	// Instantiate Icons //
	///////////////////////

	Size icon_size = Size(icon_width, icon_height);
	Size widget_size = Size(widget_width, widget_height);

	IconStatus<SDState_t> icon_sd = IconStatus<SDState_t>(icon_size, bits_nosd_normal, bits_nosd_focused, bits_sd_normal, bits_sd_focused, MSG_NOSD, MSG_SD, &SDManager::single::instance());
	Icon icon_filament_unload   = Icon(icon_size, bits_filament_unload_normal,   bits_filament_unload_focused,   MSG_FILAMENT_UNLOAD);
	Icon icon_filament_load     = Icon(icon_size, bits_filament_load_normal,     bits_filament_load_focused,     MSG_FILAMENT_LOAD);
	Icon icon_leveling          = Icon(icon_size, bits_leveling_normal,          bits_leveling_focused,          MSG_LEVELING);
	Icon icon_homing            = Icon(icon_size, bits_homing_normal,            bits_homing_focused,            MSG_HOMING);
	Icon icon_settings          = Icon(icon_size, bits_settings_normal,          bits_settings_focused,          MSG_SETTINGS);
	IconStatus<bool> icon_steppers = IconStatus<bool>(icon_size, bits_steppers_normal, bits_steppers_focused, bits_steppers_off_normal, bits_steppers_off_focused, MSG_STEPPERS, MSG_STEPPERS_OFF, &SteppersManager::single::instance());
	Icon icon_moveaxis          = Icon(icon_size, bits_moveaxis_normal,          bits_moveaxis_focused,          MSG_MOVEAXIS);
	Icon icon_temperature       = Icon(icon_size, bits_temperature_normal,       bits_temperature_focused,       MSG_TEMPERATURE);
	IconWidget<float> widget_temperature = IconWidget<float>(widget_size, bits_temperature_widget_normal, bits_temperature_widget_focused, MSG_TEMPERATURE, &TemperatureManager::single::instance());
	IconStatus<bool> icon_lightled = IconStatus<bool>(icon_size, bits_lightled_disable_normal, bits_lightled_disable_focused, bits_lightled_normal, bits_lightled_focused, MSG_LIGHTLED_DISABLE, MSG_LIGHTLED, &LightManager::single::instance());
	Icon icon_info              = Icon(icon_size, bits_info_normal,              bits_info_focused,              MSG_INFO);
	IconStatus<PrinterState_t> icon_play_pause = IconStatus<PrinterState_t>(icon_size, bits_pause_normal, bits_pause_focused, bits_play_normal, bits_play_focused, MSG_PAUSE, MSG_PLAY, &PrintManager::single::instance());
	Icon icon_stop              = Icon(icon_size, bits_stop_normal,              bits_stop_focused,              MSG_STOP);
	Icon icon_change_filament   = Icon(icon_size, bits_change_filament_normal,   bits_change_filament_focused,   MSG_CHANGE_FILAMENT);
	Icon icon_change_speed      = Icon(icon_size, bits_change_speed_normal,      bits_change_speed_focused,      MSG_CHANGE_SPEED);
	Icon icon_back              = Icon(icon_size, bits_back_normal,              bits_back_focused,              MSG_BACK);
	Icon icon_retry             = Icon(icon_size, bits_retry_normal,             bits_retry_focused,             MSG_RETRY);
	Icon icon_ok                = Icon(icon_size, bits_ok_normal,                bits_ok_focused,                MSG_OK2);
	Icon icon_move_x            = Icon(icon_size, bits_x_axis_normal,            bits_x_axis_focused,            MSG_MOVE_X);
	Icon icon_move_y            = Icon(icon_size, bits_y_axis_normal,            bits_y_axis_focused,            MSG_MOVE_Y);
	Icon icon_move_z            = Icon(icon_size, bits_z_axis_normal,            bits_z_axis_focused,            MSG_MOVE_Z);
	Icon icon_move_e            = Icon(icon_size, bits_e_axis_normal,            bits_e_axis_focused,            MSG_MOVE_E);
	Icon icon_move_01mm         = Icon(icon_size, bits_01mm_normal,              bits_01mm_focused,              MSG_SCREEN_MOVE_01MM);
	Icon icon_move_1mm          = Icon(icon_size, bits_1mm_normal,               bits_1mm_focused,               MSG_SCREEN_MOVE_1MM);
	Icon icon_move_10mm         = Icon(icon_size, bits_10mm_normal,              bits_10mm_focused,              MSG_SCREEN_MOVE_10MM);
	IconStatus<bool> icon_autolevel = IconStatus<bool>(icon_size, bits_autolevel_disable_normal, bits_autolevel_disable_focused, bits_autolevel_normal, bits_autolevel_focused, MSG_AUTOLEVEL_DISABLE, MSG_AUTOLEVEL, &AutoLevelManager::single::instance());
	Icon icon_offset            = Icon(icon_size, bits_offset_normal,            bits_offset_focused,            MSG_OFFSET);


	/////////////////////////
	// Instantiate Screens //
	/////////////////////////

	// Logo Splash
	ScreenDialog<void> screen_logo = ScreenDialog<void>(MSG_SCREEN_LOGO, MSG_SCREEN_LOGO_TEXT, MSG_SCREEN_LOGO_BOX, do_nothing);

	// Main Menu
	ScreenMenu screen_main = ScreenMenu();

	// SD Card screens
	ScreenList screen_SD_list       = ScreenList(MSG_SCREEN_SD_LIST);
	ScreenMenu screen_SD_confirm    = ScreenMenu(MSG_SCREEN_SD_CONFIRM);
	ScreenAction<void> screen_SD_OK = ScreenAction<void>(MSG_SCREEN_SD_BACK, PrintManager::startPrint);

	// Unload Filament screens
	ScreenMenu screen_unload_init                       = ScreenMenu(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_TEXT2);
	ScreenSelector<void, uint16_t> screen_unload_select = ScreenSelector<void, uint16_t>(MSG_SCREEN_UNLOAD_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
	ScreenAnimation<float> screen_unload_heating        = ScreenAnimation<float>(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_ABORT, &TemperatureManager::single::instance());
	ScreenDialog<void> screen_unload_info               = ScreenDialog<void>(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_TEXT1, MSG_SCREEN_UNLOAD_CONTINUE, do_nothing);
	ScreenTransition screen_unloading                   = ScreenTransition(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_unload);
	ScreenMenu screen_unload_confirm                    = ScreenMenu(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_CONFIRM);

	// Load Filament screens
	ScreenMenu screen_load_init                        = ScreenMenu(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_TEXT2);
	ScreenSelector<void, uint16_t> screen_load_select  = ScreenSelector<void, uint16_t>(MSG_SCREEN_LOAD_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
	ScreenAnimation<float> screen_load_heating         = ScreenAnimation<float>(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_ABORT, &TemperatureManager::single::instance());
	ScreenDialog<void> screen_load_info                = ScreenDialog<void>(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_TEXT1, MSG_SCREEN_LOAD_CONTINUE, do_nothing);
	ScreenTransition screen_loading                    = ScreenTransition(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
	ScreenMenu screen_load_confirm                     = ScreenMenu(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_CONFIRM);

	// Level Plate screens
	ScreenMenu screen_level_init                   = ScreenMenu(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT);
	ScreenAction<void> screen_level_cooling        = ScreenAction<void>(NULL, action_cooldown);
	ScreenAnimation<float> screen_level_animation  = ScreenAnimation<float>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_ABORT, &TemperatureManager::single::instance());
	ScreenTransition screen_level_homing           = ScreenTransition(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT0, MSG_SCREEN_LEVEL_BOX0, action_homing);
	ScreenDialog<void> screen_level1               = ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT1, MSG_SCREEN_LEVEL_BOX1, action_level_plate);
	ScreenDialog<void> screen_level2               = ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT2, MSG_SCREEN_LEVEL_BOX2, action_level_plate);
	ScreenDialog<void> screen_level3               = ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT3, MSG_SCREEN_LEVEL_BOX3, action_level_plate);
	ScreenDialog<void> screen_level4               = ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT4, MSG_SCREEN_LEVEL_BOX4, action_level_plate);
	ScreenAction<void> screen_level5               = ScreenAction<void>(MSG_SCREEN_AUTOHOME, action_level_plate);
	ScreenMenu screen_level_confirm                = ScreenMenu(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_CONFIRM);


	// AutoHome
	ScreenAction<void> screen_autohome = ScreenAction<void>(MSG_SCREEN_AUTOHOME, action_homing);

	// Settings
	ScreenMenu screen_settings = ScreenMenu(MSG_SCREEN_SETTINGS, MSG_SCREEN_SETTINGS_TEXT);

	// Steppers
	ScreenAction<void> screen_stepper = ScreenAction<void>(MSG_SCREEN_STEPPER, SteppersManager::disableAllSteppers);

	// Move Axis screens
/*	ScreenMenu screen_move                = ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_TEXT);
	ScreenMenu screen_move_x              = ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_X);
	ScreenMenu screen_move_y              = ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_Y);
	ScreenMenu screen_move_z              = ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_Z);
	ScreenMenu screen_move_e              = ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_E);
	ScreenDynamic<float> screen_move_x_10 = ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, X_AXIS, X_MIN_POS, X_MAX_POS, 10, action_move_axis_to);
	ScreenDynamic<float> screen_move_x_1  = ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, X_AXIS, X_MIN_POS, X_MAX_POS, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_x_01 = ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, X_AXIS, X_MIN_POS, X_MAX_POS, 0.1, action_move_axis_to);
	ScreenDynamic<float> screen_move_y_10 = ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 10, action_move_axis_to);
	ScreenDynamic<float> screen_move_y_1  = ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_y_01 = ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 0.1, action_move_axis_to);
	ScreenDynamic<float> screen_move_z_10 = ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 10, action_move_axis_to);
	ScreenDynamic<float> screen_move_z_1  = ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_z_01 = ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 0.1, action_move_axis_to);
	ScreenDynamic<float> screen_move_e_1  = ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, E_AXIS, -1E9, 1E9, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_e_01 = ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, E_AXIS, -1E9, 1E9, 0.1, action_move_axis_to);
*/

	// Temperature
	ScreenSelector<void, uint16_t> screen_temperature_main = ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 0, 250, default_temp_change_filament, action_set_temperature);

	// Light
	ScreenAction<void> screen_light = ScreenAction<void>(MSG_SCREEN_LIGHT, LightManager::setState);

	// Info
	ScreenAbout screen_info = ScreenAbout(MSG_SCREEN_INFO, MSG_SCREEN_INFO_TEXT, MSG_SCREEN_INFO_BOX, bits_logo_about);

	// Autolevel
	ScreenAction<void> screen_autolevel = ScreenAction<void>(MSG_SCREEN_AUTOLEVEL, AutoLevelManager::setState); 

	// Offset
	ScreenMenu screen_offset                 = ScreenMenu(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_TEXT);
	ScreenTransition screen_offset_home      = ScreenTransition(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_HOME_TEXT, MSG_SCREEN_OFFSET_WAIT, action_offset_homing);
	ScreenTransition screen_offset_calculate = ScreenTransition(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_PLANE_TEXT, MSG_SCREEN_OFFSET_WAIT, action_offset);
	ScreenDialog<void> screen_offset_info    = ScreenDialog<void>(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_DIALOG_TEXT, MSG_SCREEN_OFFSET_DIALOG_BOX, do_nothing);
	ScreenDynamic<float> screen_offset_set   = ScreenDynamic<float>(MSG_SCREEN_OFFSET_TITLE, Z_AXIS, 0.0, 4.0, 0.1, action_set_offset);
	ScreenAction<void> screen_offset_save    = ScreenAction<void>(MSG_SCREEN_OFFSET_TITLE,action_save_offset);
	ScreenMenu screen_offset_finish          = ScreenMenu(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_FINISH);

	// Print
	ScreenPrint screen_print          = ScreenPrint(MSG_SCREEN_PRINT_PRINTING, &TemperatureManager::single::instance());

	// Play/Pause
	ScreenAction<void> screen_play_pause = ScreenAction<void>(MSG_SCREEN_PRINT_PAUSE, PrintManager::togglePause);

	// Stop
	ScreenMenu screen_stop_confirm    = ScreenMenu(MSG_SCREEN_STOP_CONFIRM);
	ScreenAction<void> screen_stop_OK = ScreenAction<void>(MSG_SCREEN_STOP_OK, PrintManager::stopPrint);

	// Change Filament Screens
/*	ScreenMenu screen_change_confirm_first                = ScreenMenu(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_CONFIRM);
	ScreenDialog<void> screen_change_pausing              = ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_pause_print);
	ScreenSelector<void, uint16_t> screen_change_selector = ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 0, 250, default_temp_change_filament, action_set_temperature);
	ScreenAnimation<float> screen_change_animation        = ScreenAnimation<float>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_LOAD_ABORT, &TemperatureManager::single::instance());
	ScreenDialog<void> screen_change_info                 = ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_START, MSG_SCREEN_CHANGE_BOX, do_nothing);
	ScreenDialog<void> screen_change_pullout_info         = ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_PULLOUT, MSG_SCREEN_CHANGE_BOX, do_nothing);
	ScreenTransition screen_change_unloading              = ScreenTransition(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
	ScreenDialog<void> screen_change_insert_info          = ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_INSERT, MSG_SCREEN_CHANGE_BOX, do_nothing);
	ScreenTransition screen_change_loading                = ScreenTransition(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_LOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
	ScreenMenu screen_change_confirm_second               = ScreenMenu(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_CONFIRM);
	ScreenAction<void> screen_change_ok2print             = ScreenAction<void>(MSG_OK2, action_resume_print);
*/
	// Change Speed screen
	ScreenSelector<void, uint16_t> screen_speed = ScreenSelector<void, uint16_t>(MSG_SCREEN_SPEED, 10, 400, 100, action_set_feedrate_multiply);
	// Temperature
	ScreenSelector<void, uint16_t> screen_temperature_print = ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);

	// Build the UI
	Screen * GuiBuild()
	{
		// Main Menu
		screen_main.add(screen_SD_list);
		screen_main.icon(icon_sd);
		screen_main.add(screen_unload_init);
		screen_main.icon(icon_filament_unload);
		screen_main.add(screen_load_init);
		screen_main.icon(icon_filament_load);
		screen_main.add(screen_level_init);
		screen_main.icon(icon_leveling);
		screen_main.add(screen_autohome);
		screen_main.icon(icon_homing);
		screen_main.add(screen_settings);
		screen_main.icon(icon_settings);
//		screen_main.add(screen_move);
//		screen_main.icon(icon_moveaxis);
		screen_main.add(screen_stepper);
		screen_main.icon(icon_steppers);
		screen_main.add(screen_temperature_main);
		screen_main.icon(widget_temperature);

		// SD Card screens
		// SD Card List 
		screen_SD_list.add(screen_main);
		screen_SD_list.add(screen_SD_confirm);
		// SD Card Confirm
		screen_SD_confirm.add(screen_SD_list);
		screen_SD_confirm.icon(icon_back);
		screen_SD_confirm.add(screen_SD_OK);
		screen_SD_confirm.icon(icon_ok);
		// SD Confirm OK
		screen_SD_OK.add(screen_print);

		// Unload Filament Screens
		// Unload Filament Init
		screen_unload_init.add(screen_main);
		screen_unload_init.icon(icon_back);
		screen_unload_init.add(screen_unload_select);
		screen_unload_init.icon(icon_ok);
		// Unload Filament Select
		screen_unload_select.add(screen_unload_heating);
		// Unload Filament Heating
		screen_unload_heating.add(screen_main);
		screen_unload_heating.add(screen_unload_info);
		// Unload filament Info
		screen_unload_info.add(screen_unloading);
		// Unload filament Unloading
		screen_unloading.add(screen_unload_confirm);
		//Unload Filament Confirm
		screen_unload_confirm.add(screen_unload_info);
		screen_unload_confirm.icon(icon_retry);
		screen_unload_confirm.add(screen_main);
		screen_unload_confirm.icon(icon_ok);

		// Load Filament screens
		// Load Filament Init
		screen_load_init.add(screen_main);
		screen_load_init.icon(icon_back);
		screen_load_init.add(screen_load_select);
		screen_load_init.icon(icon_ok);
		// Load Filament Select
		screen_load_select.add(screen_load_heating);
		// Load Filament Heating
		screen_load_heating.add(screen_main);
		screen_load_heating.add(screen_load_info);
		// Load Filament Info
		screen_load_info.add(screen_loading);
		// Load Filament Loading
		screen_loading.add(screen_load_confirm);
		// Load Filament Confirm
		screen_load_confirm.add(screen_load_info);
		screen_load_confirm.icon(icon_retry);
		screen_load_confirm.add(screen_main);
		screen_load_confirm.icon(icon_ok);

		// Level Plate
		screen_level_init.add(screen_main);
		screen_level_init.icon(icon_back);
		screen_level_init.add(screen_level_cooling);
		screen_level_init.icon(icon_ok);
		// Level Plate Cooling
		screen_level_cooling.add(screen_level_animation);
		// Level Plate Animation
		screen_level_animation.add(screen_main);
		screen_level_animation.add(screen_level_homing);
		// Level Plate Homing
		screen_level_homing.add(screen_level1);
		// Level screen 1
		screen_level1.add(screen_level2);
		// Level screen 2
		screen_level2.add(screen_level3);
		// Level screen 3
		screen_level3.add(screen_level4);
		// Level screen 4
		screen_level4.add(screen_level5);
		// Level screen 5
		screen_level5.add(screen_level_confirm);
		// Level Plate Confirm
		screen_level_confirm.add(screen_level1);
		screen_level_confirm.icon(icon_retry);
		screen_level_confirm.add(screen_main);
		screen_level_confirm.icon(icon_ok);

		// AutoHome
		screen_autohome.add(screen_main);

		// Settings
		screen_settings.add(screen_main);
		screen_settings.icon(icon_back);
		screen_settings.add(screen_offset);
		screen_settings.icon(icon_offset);
		screen_settings.add(screen_autolevel);
		screen_settings.icon(icon_autolevel);
		screen_settings.add(screen_light);
		screen_settings.icon(icon_lightled);
		screen_settings.add(screen_info);
		screen_settings.icon(icon_info);

		// Stepper
		screen_stepper.add(screen_main);

		// Move Axis
/*
		screen_move.add(screen_main);
		screen_move.icon(icon_back);
		screen_move.add(screen_move_x);
		screen_move.icon(icon_move_x);
		screen_move.add(screen_move_y);
		screen_move.icon(icon_move_y);
		screen_move.add(screen_move_z);
		screen_move.icon(icon_move_z);
		screen_move.add(screen_move_e);
		screen_move.icon(icon_move_e);

		screen_move_x.add(screen_move);
		screen_move_x.icon(icon_back);
		screen_move_x.add(screen_move_x_10);
		screen_move_x.icon(icon_move_10mm);
		screen_move_x.add(screen_move_x_1);
		screen_move_x.icon(icon_move_1mm);
		screen_move_x.add(screen_move_x_01);
		screen_move_x.icon(icon_move_01mm);

		screen_move_y.add(screen_move);
		screen_move_y.icon(icon_back);
		screen_move_y.add(screen_move_y_10);
		screen_move_y.icon(icon_move_10mm);
		screen_move_y.add(screen_move_y_1);
		screen_move_y.icon(icon_move_1mm);
		screen_move_y.add(screen_move_y_01);
		screen_move_y.icon(icon_move_01mm);
		screen_move_z.add(screen_move);
		screen_move_z.icon(icon_back);
		screen_move_z.add(screen_move_z_10);
		screen_move_z.icon(icon_move_10mm);
		screen_move_z.add(screen_move_z_1);
		screen_move_z.icon(icon_move_1mm);
		screen_move_z.add(screen_move_z_01);
		screen_move_z.icon(icon_move_01mm);
		screen_move_e.add(screen_move);
		screen_move_e.icon(icon_back);
		screen_move_e.add(screen_move_e_1);
		screen_move_e.icon(icon_move_1mm);
		screen_move_e.add(screen_move_e_01);
		screen_move_e.icon(icon_move_01mm);

		screen_move_x_01.add(screen_move);
		screen_move_y_01.add(screen_move);
		screen_move_z_01.add(screen_move);
		screen_move_e_01.add(screen_move);

		screen_move_x_1.add(screen_move);
		screen_move_y_1.add(screen_move);
		screen_move_z_1.add(screen_move);
		screen_move_e_1.add(screen_move);

		screen_move_x_10.add(screen_move);
		screen_move_y_10.add(screen_move);
		screen_move_z_10.add(screen_move);
*/

		// Temperature
		screen_temperature_main.add(screen_main);

		// Autolevel
		screen_autolevel.add(screen_settings);

		// Light
		screen_light.add(screen_settings);

		// Info
		screen_info.add(screen_settings);

		// Offset
		screen_offset.add(screen_main);
		screen_offset.icon(icon_back);
		screen_offset.add(screen_offset_home);
		screen_offset.icon(icon_ok);

		screen_offset_home.add(screen_offset_calculate);
		screen_offset_calculate.add(screen_offset_info);
		screen_offset_info.add(screen_offset_set);
		screen_offset_set.add(screen_offset_finish);

		screen_offset_finish.add(screen_offset_info);
		screen_offset_finish.icon(icon_retry);
		screen_offset_finish.add(screen_offset_save);
		screen_offset_finish.icon(icon_ok);

		screen_offset_save.add(screen_main);

		// Print Menu
		screen_print.add(screen_play_pause);
		screen_print.icon(icon_play_pause);
		screen_print.add(screen_stop_confirm);
		screen_print.icon(icon_stop);
//		screen_print.add(screen_change_confirm_first);
//		screen_print.icon(icon_change_filament);
		screen_print.add(screen_speed);
		screen_print.icon(icon_change_speed);
		screen_print.add(screen_temperature_print);
		screen_print.icon(icon_temperature);
		// Play/Pause
		screen_play_pause.add(screen_print);
		// Stop Confirm
		screen_stop_confirm.add(screen_print);
		screen_stop_confirm.icon(icon_back);
		screen_stop_confirm.add(screen_stop_OK);
		screen_stop_confirm.icon(icon_ok);
		// Stop Confirm OK
		screen_stop_OK.add(screen_main);

		// Change filament screens
		// Change filament first confirm
/*		screen_change_confirm_first.add(screen_print);
		screen_change_confirm_first.icon(icon_back);
		screen_change_confirm_first.add(screen_change_pausing);
		screen_change_confirm_first.icon(icon_ok);
		// Change filament pausing
		screen_change_pausing.add(screen_change_selector);
		// Change filament selector
		screen_change_selector.add(screen_change_animation);
		// Change filament animation
		screen_change_animation.add(screen_print);
		screen_change_animation.add(screen_change_info);
		// Change filament Info
		screen_change_info.add(screen_change_pullout_info);
		// Change filament pullout
		screen_change_pullout_info.add(screen_change_unloading);
		// Change filament Unloading
		screen_change_unloading.add(screen_change_insert_info);
		//Change filament insert
		screen_change_insert_info.add(screen_change_loading);
		// Change filament loading
		screen_change_loading.add(screen_change_confirm_second);
		// Change filament second confirm
		screen_change_confirm_second.add(screen_change_info);
		screen_change_confirm_second.icon(icon_retry);
		screen_change_confirm_second.add(screen_change_ok2print);
		screen_change_confirm_second.icon(icon_ok);
		// Change filament OK to print
		screen_change_ok2print.add(screen_print);*/
		// Change speed screens
		screen_speed.add(screen_print);
		// Temperature
		screen_temperature_print.add(screen_print);

		return (Screen *) &screen_main;
	}
}
