#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"
#include "GuiAction.h"

#include "Icon.h"
#include "IconWidget.h"

#include "Screen.h"
#include "ScreenMenu.h"
#include "ScreenPrint.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenList.h"
#include "ScreenStatus.h"
#include "ScreenAnimation.h"
#include "ScreenTransition.h"
#include "ScreenAction.h"
#include "ScreenPrint.h"
#include "ScreenDynamic.h"
#include "ScreenAbout.h"

#include "AutoLevelManager.h"
#include "LightManager.h"
#include "PrintManager.h"
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

	Icon icon_nosd              = Icon(icon_size, bits_nosd_normal,              bits_nosd_focused,              MSG_NOSD);
	Icon icon_sd                = Icon(icon_size, bits_sd_normal,                bits_sd_focused,                MSG_SD);
	Icon icon_filament_unload   = Icon(icon_size, bits_filament_unload_normal,   bits_filament_unload_focused,   MSG_FILAMENT_UNLOAD);
	Icon icon_filament_load     = Icon(icon_size, bits_filament_load_normal,     bits_filament_load_focused,     MSG_FILAMENT_LOAD);
	Icon icon_leveling          = Icon(icon_size, bits_leveling_normal,          bits_leveling_focused,          MSG_LEVELING);
	Icon icon_homing            = Icon(icon_size, bits_homing_normal,            bits_homing_focused,            MSG_HOMING);
	Icon icon_settings          = Icon(icon_size, bits_settings_normal,          bits_settings_focused,          MSG_SETTINGS);
	Icon icon_steppers          = Icon(icon_size, bits_steppers_normal,          bits_steppers_focused,          MSG_STEPPERS);
	Icon icon_steppers_off      = Icon(icon_size, bits_steppers_off_normal,      bits_steppers_off_focused,      MSG_STEPPERS_OFF);
	Icon icon_moveaxis          = Icon(icon_size, bits_moveaxis_normal,          bits_moveaxis_focused,          MSG_MOVEAXIS);
	Icon icon_temperature       = Icon(icon_size, bits_temperature_normal,       bits_temperature_focused,       MSG_TEMPERATURE);
	IconWidget<float> widget_temperature = IconWidget<float>(widget_size, bits_temperature_widget_normal, bits_temperature_widget_focused,       MSG_TEMPERATURE, &TemperatureManager::single::instance());
	Icon icon_lightled_disable  = Icon(icon_size, bits_lightled_disable_normal,  bits_lightled_disable_focused,  MSG_LIGHTLED_DISABLE);
	Icon icon_lightled          = Icon(icon_size, bits_lightled_normal,          bits_lightled_focused,          MSG_LIGHTLED);
	Icon icon_info              = Icon(icon_size, bits_info_normal,              bits_info_focused,              MSG_INFO);
	Icon icon_play              = Icon(icon_size, bits_play_normal,              bits_play_focused,              MSG_PLAY);
	Icon icon_pause             = Icon(icon_size, bits_pause_normal,             bits_pause_focused,             MSG_PAUSE);
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
	Icon icon_autolevel         = Icon(icon_size, bits_autolevel_normal,         bits_autolevel_focused,         MSG_AUTOLEVEL);
	Icon icon_autolevel_disable = Icon(icon_size, bits_autolevel_disable_normal, bits_autolevel_disable_focused, MSG_AUTOLEVEL_DISABLE);
	Icon icon_offset            = Icon(icon_size, bits_offset_normal,            bits_offset_focused,            MSG_OFFSET);


	/////////////////////////
	// Instantiate Screens //
	/////////////////////////

	// Logo Splash
	ScreenDialog<void> screen_logo 							= ScreenDialog<void>(MSG_SCREEN_LOGO, MSG_SCREEN_LOGO_TEXT, MSG_SCREEN_LOGO_BOX, do_nothing);

	// Main menu screen
	ScreenMenu screen_main 									= ScreenMenu();

	// SD Card screens
	ScreenList screen_SD_list 								= ScreenList(MSG_SCREEN_SD_LIST);
	ScreenMenu screen_SD_confirm 							= ScreenMenu(MSG_SCREEN_SD_CONFIRM);
	ScreenAction<void> screen_SD_back 						= ScreenAction<void>(MSG_SCREEN_SD_BACK, do_nothing);
	ScreenAction<void> screen_SD_OK 						= ScreenAction<void>(MSG_SCREEN_SD_BACK, PrintManager::startPrint);

	// Unload Filament screens
	ScreenMenu screen_unload_init 							= ScreenMenu(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_TEXT2);
	ScreenSelector<void, uint16_t> screen_unload_select 	= ScreenSelector<void, uint16_t>(MSG_SCREEN_UNLOAD_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
	ScreenAnimation<float> screen_unload_heating 			= ScreenAnimation<float>(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_ABORT, &TemperatureManager::single::instance());
	ScreenDialog<void> screen_unload_info 					= ScreenDialog<void>(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_TEXT1, MSG_SCREEN_UNLOAD_CONTINUE, do_nothing);
	ScreenTransition screen_unloading 						= ScreenTransition(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_unload);
	ScreenMenu screen_unload_confirm 						= ScreenMenu(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_CONFIRM);

	// Load Filament screens
	ScreenMenu screen_load_init 							= ScreenMenu(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_TEXT2);
	ScreenSelector<void, uint16_t> screen_load_select 		= ScreenSelector<void, uint16_t>(MSG_SCREEN_LOAD_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
	ScreenAnimation<float> screen_load_heating 				= ScreenAnimation<float>(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_ABORT, &TemperatureManager::single::instance());
	ScreenDialog<void> screen_load_info 					= ScreenDialog<void>(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_TEXT1, MSG_SCREEN_LOAD_CONTINUE, do_nothing);
	ScreenTransition screen_loading 						= ScreenTransition(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
	ScreenMenu screen_load_confirm 							= ScreenMenu(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_CONFIRM);

	// Level Plate screens
	ScreenMenu screen_level_init 							= ScreenMenu(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT);
	ScreenAction<void> screen_level_cooling			= ScreenAction<void>(NULL, action_cooldown);
	ScreenAnimation<float> screen_level_animation 			= ScreenAnimation<float>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_ABORT, &TemperatureManager::single::instance());
	ScreenTransition screen_level_homing 					= ScreenTransition(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT0, MSG_SCREEN_LEVEL_BOX0, action_homing);
	ScreenDialog<void> screen_level1 						= ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT1, MSG_SCREEN_LEVEL_BOX1, action_level_plate);
	ScreenDialog<void> screen_level2 						= ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT2, MSG_SCREEN_LEVEL_BOX2, action_level_plate);
	ScreenDialog<void> screen_level3 						= ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT3, MSG_SCREEN_LEVEL_BOX3, action_level_plate);
	ScreenDialog<void> screen_level4 						= ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT4, MSG_SCREEN_LEVEL_BOX4, action_level_plate);
	ScreenAction<void> screen_level5 						= ScreenAction<void>(MSG_SCREEN_AUTOHOME, action_level_plate);
	ScreenMenu screen_level_confirm 						= ScreenMenu(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_CONFIRM);

	// Autohome screens
	ScreenAction<void> screen_autohome 						= ScreenAction<void>(MSG_SCREEN_AUTOHOME, action_homing);

	// Settings screens
	ScreenMenu screen_settings 								= ScreenMenu(MSG_SCREEN_SETTINGS, MSG_SCREEN_SETTINGS_TEXT);
	// Offset screens
	ScreenMenu screen_offset 								= ScreenMenu(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_TEXT);
	ScreenTransition screen_offset_home 					= ScreenTransition(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_HOME_TEXT, MSG_SCREEN_OFFSET_WAIT, action_homing);
	ScreenTransition screen_offset_calculate 				= ScreenTransition(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_PLANE_TEXT, MSG_SCREEN_OFFSET_WAIT, action_offset);
	ScreenDialog<void> screen_offset_info					= ScreenDialog<void>(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_DIALOG_TEXT, MSG_SCREEN_OFFSET_DIALOG_BOX, do_nothing);
	ScreenDynamic<float> screen_offset_set 					= ScreenDynamic<float>(MSG_SCREEN_OFFSET_TITLE, Z_AXIS, 0.0, 4.0, 0.1, action_set_offset);
	ScreenMenu screen_offset_finish 						= ScreenMenu(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_FINISH);

	// Autolevel screens
	ScreenStatus<bool, void> screen_autolevel 				= ScreenStatus<bool, void>(MSG_SCREEN_AUTOLEVEL, AutoLevelManager::setState, &AutoLevelManager::single::instance());

	// Light screens
	ScreenStatus<bool, void> screen_light 					= ScreenStatus<bool, void>(MSG_SCREEN_LIGHT, LightManager::setState, &LightManager::single::instance());

	// Info screens
	ScreenAbout screen_info 								= ScreenAbout(MSG_SCREEN_INFO, MSG_SCREEN_INFO_TEXT, MSG_SCREEN_INFO_BOX, bits_logo_about);

	// Steppers screens
	ScreenStatus<bool, void> screen_stepper 				= ScreenStatus<bool, void>(MSG_SCREEN_STEPPER, SteppersManager::disableAllSteppers, &SteppersManager::single::instance());

/*	// Move Axis screens
	ScreenMenu screen_move 									= ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_TEXT);
	ScreenMenu screen_move_x 								= ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_X);
	ScreenMenu screen_move_y 								= ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_Y);
	ScreenMenu screen_move_z 								= ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_Z);
	ScreenMenu screen_move_e 								= ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_E);
	ScreenAction<void> screen_move_back2move 				= ScreenAction<void>(MSG_SCREEN_MOVE_2MOVE, do_nothing);
	ScreenDynamic<float> screen_move_x_10 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, X_AXIS, X_MIN_POS, X_MAX_POS, 10, action_move_axis_to);
	ScreenDynamic<float> screen_move_x_1 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, X_AXIS, X_MIN_POS, X_MAX_POS, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_x_01 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, X_AXIS, X_MIN_POS, X_MAX_POS, 0.1, action_move_axis_to);
	ScreenDynamic<float> screen_move_y_10 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 10, action_move_axis_to);
	ScreenDynamic<float> screen_move_y_1 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_y_01 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 0.1, action_move_axis_to);
	ScreenDynamic<float> screen_move_z_10 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 10, action_move_axis_to);
	ScreenDynamic<float> screen_move_z_1 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_z_01 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 0.1, action_move_axis_to);
	ScreenDynamic<float> screen_move_e_1 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, E_AXIS, -1E9, 1E9, 1, action_move_axis_to);
	ScreenDynamic<float> screen_move_e_01 					= ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, E_AXIS, -1E9, 1E9, 0.1, action_move_axis_to);*/

	// Temperature screens
	ScreenSelector<void, uint16_t> screen_temperature_main 	= ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 0, 250, default_temp_change_filament, action_set_temperature);


	// Print Menu screens
	ScreenPrint screen_print 								= ScreenPrint(MSG_SCREEN_PRINT_PRINTING);

	// Play/Pause screens
	ScreenStatus<PrinterState_t, void> screen_play_pause 	= ScreenStatus<PrinterState_t, void>("Pause", PrintManager::togglePause, &PrintManager::single::instance());

	// Stop screens
	ScreenMenu screen_stop_confirm 							= ScreenMenu(MSG_SCREEN_STOP_CONFIRM);
	ScreenAction<void> screen_stop_back 					= ScreenAction<void>(MSG_SCREEN_STOP_BACK, do_nothing);
	ScreenAction<void> screen_stop_OK 						= ScreenAction<void>(MSG_SCREEN_STOP_OK, action_stop_print);

	// Change Filament screens
	ScreenMenu screen_change_confirm_first 					= ScreenMenu(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_CONFIRM);
	ScreenDialog<void> screen_change_pausing 				= ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_pause_print);
	ScreenSelector<void, uint16_t> screen_change_selector 	= ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 0, 250, default_temp_change_filament, action_set_temperature);
	ScreenAnimation<float> screen_change_animation 			= ScreenAnimation<float>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_LOAD_ABORT, &TemperatureManager::single::instance());
	ScreenDialog<void> screen_change_info 					= ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_START, MSG_SCREEN_CHANGE_BOX, do_nothing);
	ScreenDialog<void> screen_change_pullout_info 			= ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_PULLOUT, MSG_SCREEN_CHANGE_BOX, do_nothing);
	ScreenTransition screen_change_unloading 				= ScreenTransition(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
	ScreenDialog<void> screen_change_insert_info 			= ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_INSERT, MSG_SCREEN_CHANGE_BOX, do_nothing);
	ScreenTransition screen_change_loading 					= ScreenTransition(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_LOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
	ScreenMenu screen_change_confirm_second 				= ScreenMenu(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_CONFIRM);
	ScreenAction<void> screen_change_retry 					= ScreenAction<void>(MSG_SCREEN_CHANGE_RETRY, do_nothing);

	// Change Speed screens
	ScreenSelector<void, uint16_t> screen_speed 			= ScreenSelector<void, uint16_t>(MSG_SCREEN_SPEED, 10, 400, 100, action_set_feedrate_multiply);

	// Change Temperature screens
	ScreenSelector<void, uint16_t> screen_temperature_print = ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
	// Multiples backs to <T> screens
	ScreenAction<void> screen_back2main 					= ScreenAction<void>(MSG_SCREEN_BACK2MAIN, do_nothing);
	ScreenAction<void> screen_ok2main 						= ScreenAction<void>(MSG_OK2, do_nothing);
	ScreenAction<void> screen_back2print 					= ScreenAction<void>(MSG_BACK, do_nothing);
	ScreenAction<void> screen_ok2print 						= ScreenAction<void>(MSG_OK2, do_nothing);
	ScreenAction<void> screen_change_back2print 			= ScreenAction<void>(MSG_BACK, action_resume_print);
	ScreenAction<void> screen_change_ok2print 				= ScreenAction<void>(MSG_OK2, action_resume_print);


	// Build the UI
	Screen * GuiBuild()
	{
		// // Main menu screen
		// ScreenMenu screen_main
		screen_main.add(screen_SD_list);
		screen_main.add(screen_unload_init);
		screen_main.add(screen_load_init);
		screen_main.add(screen_level_init);
		screen_main.add(screen_autohome);
		screen_main.add(screen_settings);
//		screen_main.add(screen_move);
		screen_main.add(screen_stepper);
		screen_main.add(screen_temperature_main);

		// // SD Card screens
		// ScreenList screen_SD_list
		screen_SD_list.add(screen_main);
		screen_SD_list.add(screen_SD_confirm);
		screen_SD_list.icon(icon_nosd);
		screen_SD_list.icon(icon_sd);
		// ScreenMenu screen_SD_confirm
		screen_SD_confirm.add(screen_SD_back);
		screen_SD_confirm.add(screen_SD_OK);
		// ScreenAction screen_SD_back
		screen_SD_back.add(screen_SD_list);
		screen_SD_back.icon(icon_back);
		// ScreenAction screen_SD_OK
		screen_SD_OK.add(screen_print);
		screen_SD_OK.icon(icon_ok);

		// // Unload Filament screens
		// ScreenMenu screen_unload_init
		screen_unload_init.add(screen_back2main);
		screen_unload_init.add(screen_unload_select);
		screen_unload_init.icon(icon_filament_unload);
		// ScreenSelector screen_unload_select
		screen_unload_select.add(screen_unload_heating);
		screen_unload_select.icon(icon_ok);
		// ScreenAnimation screen_unload_heating
		screen_unload_heating.add(screen_main);
		screen_unload_heating.add(screen_unload_info);
		// ScreenDialog screen_unload_info
		screen_unload_info.add(screen_unloading);
		screen_unload_info.icon(icon_retry);
		// ScreenTransition screen_unloading
		screen_unloading.add(screen_unload_confirm);
		// ScreenMenu screen_unload_confirm
		screen_unload_confirm.add(screen_unload_info);
		screen_unload_confirm.add(screen_ok2main);

		// // Load Filament screens
		// ScreenMenu screen_load_init
		screen_load_init.add(screen_back2main);
		screen_load_init.add(screen_load_select);
		screen_load_init.icon(icon_filament_load);
		// ScreenSelector screen_load_select
		screen_load_select.add(screen_load_heating);
		screen_load_select.icon(icon_ok);
		// ScreenAnimation screen_load_heating
		screen_load_heating.add(screen_main);
		screen_load_heating.add(screen_load_info);
		// ScreenDialog screen_load_info
		screen_load_info.add(screen_loading);
		screen_load_info.icon(icon_retry);
		// ScreenTransition screen_loading
		screen_loading.add(screen_load_confirm);
		// ScreenMenu screen_load_confirm
		screen_load_confirm.add(screen_load_info);
		screen_load_confirm.add(screen_ok2main);

		// // Level Plate screens
		// ScreenMenu screen_level_init
		screen_level_init.add(screen_back2main);
		screen_level_init.add(screen_level_cooling);
		screen_level_init.icon(icon_leveling);
		// ScreenAction screen_level_cooling
		screen_level_cooling.add(screen_level_animation);
		screen_level_cooling.icon(icon_ok);
		// ScreenAnimation screen_level_animation
		screen_level_animation.add(screen_main);
		screen_level_animation.add(screen_level_homing);
		// ScreenTransition screen_level_homing
		screen_level_homing.add(screen_level1);
		// ScreenDialog screen_level1
		screen_level1.add(screen_level2);
		screen_level1.icon(icon_retry);
		// ScreenDialog screen_level2
		screen_level2.add(screen_level3);
		// ScreenDialog screen_level3
		screen_level3.add(screen_level4);
		// ScreenDialog screen_level4
		screen_level4.add(screen_level5);
		// ScreenDialog screen_level5
		screen_level5.add(screen_level_confirm);
		// ScreenMenu screen_level_confirm
		screen_level_confirm.add(screen_level1);
		screen_level_confirm.add(screen_ok2main);

		// // Autohome screens
		// ScreenAction screen_autohome
		screen_autohome.add(screen_main);
		screen_autohome.icon(icon_homing);

		// // Settings screens
		// ScreenMenu screen_settings
		screen_settings.add(screen_back2main);
		screen_settings.add(screen_offset);
		screen_settings.add(screen_autolevel);
		screen_settings.add(screen_light);
		screen_settings.add(screen_info);
		screen_settings.icon(icon_settings);

		// Offset screens
		// ScreenMenu screen_offset
		screen_offset.add(screen_back2main);
		screen_offset.add(screen_offset_home);
		screen_offset.icon(icon_offset);
		// ScreenTransition screen_offset_home
		screen_offset_home.add(screen_offset_calculate);
		screen_offset_home.icon(icon_ok);
		// ScreenTransition screen_offset_calculate
		screen_offset_calculate.add(screen_offset_info);
		// ScreenDialog screen_offset_info
		screen_offset_info.add(screen_offset_set);
		screen_offset_info.icon(icon_retry);
		// ScreenSelector screen_offset_set
		screen_offset_set.add(screen_offset_finish);
		// ScreenMenu screen_offset_finish
		screen_offset_finish.add(screen_offset_info);
		screen_offset_finish.add(screen_ok2main);

		// Autolevel screens
		// ScreenStatus screen_autolevel
		screen_autolevel.add(screen_settings);
		screen_autolevel.icon(icon_autolevel_disable);
		screen_autolevel.icon(icon_autolevel);

		// Light screens
		// ScreenStatus screen_light
		screen_light.add(screen_settings);
		screen_light.icon(icon_lightled_disable);
		screen_light.icon(icon_lightled);

		// Info screens
		// ScreenAbout screen_info
		screen_info.add(screen_main);
		screen_info.icon(icon_info);

		// // Stepper screens
		// ScreenStatus screen_stepper
		screen_stepper.add(screen_main);
		screen_stepper.icon(icon_steppers);
		screen_stepper.icon(icon_steppers_off);

/*		// // Move Axis screens
		// ScreenMenu screen_move
		screen_move.add(screen_back2main);
		screen_move.add(screen_move_x);
		screen_move.add(screen_move_y);
		screen_move.add(screen_move_z);
		screen_move.add(screen_move_e);
		screen_move.icon(icon_moveaxis);
		// ScreenAction screen_move_back2move
		screen_move_back2move.add(screen_move);
		screen_move_back2move.icon(icon_back);
		// ScreenMenu screen_move_x
		screen_move_x.add(screen_move_back2move);
		screen_move_x.add(screen_move_x_10);
		screen_move_x.add(screen_move_x_1);
		screen_move_x.add(screen_move_x_01);
		screen_move_x.icon(icon_move_x);
		// ScreenMenu screen_move_y
		screen_move_y.add(screen_move_back2move);
		screen_move_y.add(screen_move_y_10);
		screen_move_y.add(screen_move_y_1);
		screen_move_y.add(screen_move_y_01);
		screen_move_y.icon(icon_move_y);
		// ScreenMenu screen_move_z
		screen_move_z.add(screen_move_back2move);
		screen_move_z.add(screen_move_z_10);
		screen_move_z.add(screen_move_z_1);
		screen_move_z.add(screen_move_z_01);
		screen_move_z.icon(icon_move_z);
		// ScreenMenu screen_move_e
		screen_move_e.add(screen_move_back2move);
		screen_move_e.add(screen_move_e_1);
		screen_move_e.add(screen_move_e_01);
		screen_move_e.icon(icon_move_e);
		// ScreenDynamic screen_move_x_01
		screen_move_x_01.add(screen_move_back2move);
		screen_move_x_01.icon(icon_move_01mm);
		// ScreenDynamic screen_move_y_01
		screen_move_y_01.add(screen_move_back2move);
		screen_move_y_01.icon(icon_move_01mm);
		// ScreenDynamic screen_move_z_01
		screen_move_z_01.add(screen_move_back2move);
		screen_move_z_01.icon(icon_move_01mm);
		// ScreenDynamic screen_move_e_01
		screen_move_e_01.add(screen_move_back2move);
		screen_move_e_01.icon(icon_move_01mm);
		// ScreenDynamic screen_move_x_1
		screen_move_x_1.add(screen_move_back2move);
		screen_move_x_1.icon(icon_move_1mm);
		// ScreenDynamic screen_move_y_1
		screen_move_y_1.add(screen_move_back2move);
		screen_move_y_1.icon(icon_move_1mm);
		// ScreenDynamic screen_move_z_1
		screen_move_z_1.add(screen_move_back2move);
		screen_move_z_1.icon(icon_move_1mm);
		// ScreenDynamic screen_move_e_1
		screen_move_e_1.add(screen_move_back2move);
		screen_move_e_1.icon(icon_move_1mm);
		// ScreenDynamic screen_move_x_10
		screen_move_x_10.add(screen_move_back2move);
		screen_move_x_10.icon(icon_move_10mm);
		// ScreenDynamic screen_move_y_10
		screen_move_y_10.add(screen_move_back2move);
		screen_move_y_10.icon(icon_move_10mm);
		// ScreenDynamic screen_move_z_10
		screen_move_z_10.add(screen_move_back2move);
		screen_move_z_10.icon(icon_move_10mm);*/

		// // Temperature screens
		// ScreenSelector screen_temperature_main
		screen_temperature_main.add(screen_main);
		screen_temperature_main.icon(widget_temperature);


		// // Print menu screens
		// ScreenPrint screen_print
		screen_print.add(screen_play_pause);
		screen_print.add(screen_stop_confirm);
		screen_print.add(screen_change_confirm_first);
		screen_print.add(screen_speed);
		screen_print.add(screen_temperature_print);

		// // Play/Pause screens
		// ScreenStatus screen_play_pause
		screen_play_pause.add(screen_print);
		screen_play_pause.icon(icon_pause);
		screen_play_pause.icon(icon_play);

		// // Stop screens
		// ScreenMenu screen_stop_confirm
		screen_stop_confirm.add(screen_stop_back);
		screen_stop_confirm.add(screen_stop_OK);
		screen_stop_confirm.icon(icon_stop);
		// ScreenAction screen_stop_back
		screen_stop_back.add(screen_print);
		screen_stop_back.icon(icon_back);
		// ScreenAction screen_stop_OK
		screen_stop_OK.add(screen_main);
		screen_stop_OK.icon(icon_ok);

		// // Change Filament screens
		// ScreenMenu screen_change_confirm_first
		screen_change_confirm_first.add(screen_back2print);
		screen_change_confirm_first.add(screen_change_pausing);
		screen_change_confirm_first.icon(icon_change_filament);
		// ScreenTransition screen_change_pausing
		screen_change_pausing.add(screen_change_selector);
		screen_change_pausing.icon(icon_ok);
		// ScreenSelector screen_change_selector
		screen_change_selector.add(screen_change_animation);
		// ScreenAnimation screen_change_animation
		screen_change_animation.add(screen_change_back2print);
		screen_change_animation.add(screen_change_info);
		// ScreenDialog screen_change_info
		screen_change_info.add(screen_change_pullout_info);
		screen_change_info.icon(icon_ok);
		// ScreenDialog screen_change_pullout
		screen_change_pullout_info.add(screen_change_unloading);
		// ScreenTransition screen_change_unloading
		screen_change_unloading.add(screen_change_insert_info);
		// ScreenDialog screen_change_insert
		screen_change_insert_info.add(screen_change_loading);
		//ScreenTransition screen_change_loading
		screen_change_loading.add(screen_change_confirm_second);
		// ScreenMenu screen_change_confirm_second
		screen_change_confirm_second.add(screen_change_retry);
		screen_change_confirm_second.add(screen_change_ok2print);
		// ScreenAction screen_change_retry
		screen_change_retry.add(screen_change_info);
		screen_change_retry.icon(icon_retry);

		// // Change Speed screens
		// ScreenSelector screen_speed
		screen_speed.add(screen_print);
		screen_speed.icon(icon_change_speed);

		// // Change Temperature screens
		// ScreenSelector screen_temperature_print
		screen_temperature_print.add(screen_print);
		screen_temperature_print.icon(icon_temperature);

		// // Multiples backs to <T> screens
		// ScreenAction screen_back2main
		screen_back2main.add(screen_main);
		screen_back2main.icon(icon_back);
		// ScreenAction screen_ok2main
		screen_ok2main.add(screen_main);
		screen_ok2main.icon(icon_ok);
		// ScreenAction screen_back2print
		screen_back2print.add(screen_print);
		screen_back2print.icon(icon_back);
		// ScreenAction screen_ok2print
		screen_ok2print.add(screen_print);
		screen_ok2print.icon(icon_ok);
		// ScreenAction screen_change_back2print
		screen_change_back2print.add(screen_print);
		screen_change_back2print.icon(icon_back);
		// ScreenAction screen_change_ok2print
		screen_change_ok2print.add(screen_print);
		screen_change_ok2print.icon(icon_ok);


		return (Screen *) &screen_main;
	}
}
