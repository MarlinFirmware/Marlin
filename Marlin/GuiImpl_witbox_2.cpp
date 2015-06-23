#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"
#include "GuiAction.h"

#include "Icon.h"
#include "IconWidget.h"
#include "IconStatus.h"

#include "Screen.h"
#include "ScreenMenu.h"
#include "ScreenList.h"
#include "ScreenAction.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenAnimation.h"
#include "ScreenTransition.h"
#include "ScreenDynamic.h"
#include "ScreenAbout.h"
#include "ScreenPrint.h"

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

	Icon icon_filament_unload   = Icon(icon_size, bits_filament_unload_normal,   bits_filament_unload_focused,   MSG_FILAMENT_UNLOAD);
	Icon icon_filament_load     = Icon(icon_size, bits_filament_load_normal,     bits_filament_load_focused,     MSG_FILAMENT_LOAD);
	Icon icon_leveling          = Icon(icon_size, bits_leveling_normal,          bits_leveling_focused,          MSG_LEVELING);
	Icon icon_homing            = Icon(icon_size, bits_homing_normal,            bits_homing_focused,            MSG_HOMING);
	Icon icon_settings          = Icon(icon_size, bits_settings_normal,          bits_settings_focused,          MSG_SETTINGS);
	Icon icon_moveaxis          = Icon(icon_size, bits_moveaxis_normal,          bits_moveaxis_focused,          MSG_MOVEAXIS);
	Icon icon_temperature       = Icon(icon_size, bits_temperature_normal,       bits_temperature_focused,       MSG_TEMPERATURE);
	Icon icon_info              = Icon(icon_size, bits_info_normal,              bits_info_focused,              MSG_INFO);
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
	Icon icon_offset            = Icon(icon_size, bits_offset_normal,            bits_offset_focused,            MSG_OFFSET);
   IconStatus<SDState_t> icon_sd = IconStatus<SDState_t>(icon_size, bits_nosd_normal, bits_nosd_focused, bits_sd_normal, bits_sd_focused, MSG_NOSD, MSG_SD, &SDManager::single::instance());
	IconStatus<bool> icon_autolevel = IconStatus<bool>(icon_size, bits_autolevel_disable_normal, bits_autolevel_disable_focused, bits_autolevel_normal, bits_autolevel_focused, MSG_AUTOLEVEL_DISABLE, MSG_AUTOLEVEL, &AutoLevelManager::single::instance());
	IconStatus<bool> icon_steppers = IconStatus<bool>(icon_size, bits_steppers_normal, bits_steppers_focused, bits_steppers_off_normal, bits_steppers_off_focused, MSG_STEPPERS, MSG_STEPPERS_OFF, &SteppersManager::single::instance());
	IconStatus<bool> icon_lightled = IconStatus<bool>(icon_size, bits_lightled_disable_normal, bits_lightled_disable_focused, bits_lightled_normal, bits_lightled_focused, MSG_LIGHTLED_DISABLE, MSG_LIGHTLED, &LightManager::single::instance());
	IconStatus<PrinterState_t> icon_play_pause = IconStatus<PrinterState_t>(icon_size, bits_pause_normal, bits_pause_focused, bits_play_normal, bits_play_focused, MSG_PAUSE, MSG_PLAY, &PrintManager::single::instance());
	IconWidget<float> widget_temperature = IconWidget<float>(widget_size, bits_temperature_widget_normal, bits_temperature_widget_focused, MSG_TEMPERATURE, &TemperatureManager::single::instance());


	///////////////////////////////
	// Instantiation of  Screens //
	///////////////////////////////

	static ScreenDialog<void> * make_screen_logo()
	{
		return (new ScreenDialog<void>(MSG_SCREEN_LOGO, MSG_SCREEN_LOGO_TEXT, MSG_SCREEN_LOGO_BOX, do_nothing));
	}

	static ScreenMenu * make_screen_main()
	{
		ScreenMenu * local_view = new ScreenMenu();
		local_view->add(screen_SD_list);
		local_view->icon(icon_sd);
		local_view->add(screen_unload_init);
		local_view->icon(icon_filament_unload);
		local_view->add(screen_load_init);
		local_view->icon(icon_filament_load);
		local_view->add(screen_level_init);
		local_view->icon(icon_leveling);
		local_view->add(screen_autohome);
		local_view->icon(icon_homing);
		local_view->add(screen_settings);
		local_view->icon(icon_settings);
		local_view->add(screen_move);
		local_view->icon(icon_moveaxis);
		local_view->add(screen_stepper);
		local_view->icon(icon_steppers);
		local_view->add(screen_temperature_main);
		local_view->icon(widget_temperature);
		return local_view;
	}	

	static ScreenList * make_screen_SD_list()
	{
		ScreenList * local_view = new ScreenList(MSG_SCREEN_SD_LIST);
		local_view->add(screen_main);
		local_view->add(screen_SD_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_SD_confirm()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_SD_CONFIRM);
		local_view->add(screen_SD_list);
		local_view->icon(icon_back);
		local_view->add(screen_SD_OK);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_SD_OK()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_SD_BACK, PrintManager::startPrint);
		local_view->add(screen_print);
		return local_view;
	}

	static ScreenMenu * make_screen_unload_init()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_TEXT2);
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_unload_select);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_unload_select()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_UNLOAD_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_unload_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_unload_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_ABORT, &TemperatureManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_unload_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_unload_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_TEXT1, MSG_SCREEN_UNLOAD_CONTINUE, do_nothing);
		local_view->add(screen_unloading);
		return local_view;
	}

	static ScreenTransition * make_screen_unloading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_unload);
		local_view->add(screen_unload_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_unload_confirm()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_UNLOAD_TITLE, MSG_SCREEN_UNLOAD_CONFIRM);
		local_view->add(screen_unload_info);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenMenu * make_screen_load_init()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_TEXT2);
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_load_select);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_load_select()
	{
		ScreenSelector<void, uint16_t> * local_view  = new ScreenSelector<void, uint16_t>(MSG_SCREEN_LOAD_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_load_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_load_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_ABORT, &TemperatureManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_load_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_load_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_TEXT1, MSG_SCREEN_LOAD_CONTINUE, do_nothing);
		local_view->add(screen_loading);
		return local_view;
	}

	static ScreenTransition * make_screen_loading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
		local_view->add(screen_load_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_load_confirm()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LOAD_TITLE, MSG_SCREEN_LOAD_CONFIRM);
		local_view->add(screen_load_info);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenMenu * make_screen_level_init()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT);
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_level_cooling);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_level_cooling()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_cooldown);
		local_view->add(screen_level_animation);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_level_animation()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_ABORT, &TemperatureManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_level_homing);
		return local_view;
	}

	static ScreenTransition * make_screen_level_homing()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT0, MSG_SCREEN_LEVEL_BOX0, action_homing);
		local_view->add(screen_level1);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level1()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT1, MSG_SCREEN_LEVEL_BOX1, action_level_plate);
		local_view->add(screen_level2);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level2()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT2, MSG_SCREEN_LEVEL_BOX2, action_level_plate);
		local_view->add(screen_level3);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level3()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT3, MSG_SCREEN_LEVEL_BOX3, action_level_plate);
		local_view->add(screen_level4);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level4()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_TEXT4, MSG_SCREEN_LEVEL_BOX4, action_level_plate);
		local_view->add(screen_level5);
		return local_view;
	}

	static ScreenAction<void> * make_screen_level5()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_AUTOHOME, action_level_plate);
		local_view->add(screen_level_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_level_confirm()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LEVEL_TITLE, MSG_SCREEN_LEVEL_CONFIRM);
		local_view->add(screen_level1);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_autohome()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_AUTOHOME, action_homing);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_settings()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_SETTINGS, MSG_SCREEN_SETTINGS_TEXT);
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_offset);
		local_view->icon(icon_offset);
		local_view->add(screen_autolevel);
		local_view->icon(icon_autolevel);
		local_view->add(screen_light);
		local_view->icon(icon_lightled);
		local_view->add(screen_info);
		local_view->icon(icon_info);
		return local_view;
	}

	static ScreenAction<void> * make_screen_stepper()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_STEPPER, SteppersManager::disableAllSteppers);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_move()
	{
		ScreenMenu *  local_view = new ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_TEXT);
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_move_x);
		local_view->icon(icon_move_x);
		local_view->add(screen_move_y);
		local_view->icon(icon_move_y);
		local_view->add(screen_move_z);
		local_view->icon(icon_move_z);
		local_view->add(screen_move_e);
		local_view->icon(icon_move_e);
		return local_view;
	}

	static ScreenMenu * make_screen_move_x()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_X);
		local_view->add(screen_move);
		local_view->icon(icon_back);
		local_view->add(screen_move_x_10);
		local_view->icon(icon_move_10mm);
		local_view->add(screen_move_x_1);
		local_view->icon(icon_move_1mm);
		local_view->add(screen_move_x_01);
		local_view->icon(icon_move_01mm);
		return local_view;
	}

	static ScreenMenu * make_screen_move_y()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_Y);
		local_view->add(screen_move);
		local_view->icon(icon_back);
		local_view->add(screen_move_y_10);
		local_view->icon(icon_move_10mm);
		local_view->add(screen_move_y_1);
		local_view->icon(icon_move_1mm);
		local_view->add(screen_move_y_01);
		local_view->icon(icon_move_01mm);
		return local_view;
	}

	static ScreenMenu * make_screen_move_z()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_Z);
		local_view->add(screen_move);
		local_view->icon(icon_back);
		local_view->add(screen_move_z_10);
		local_view->icon(icon_move_10mm);
		local_view->add(screen_move_z_1);
		local_view->icon(icon_move_1mm);
		local_view->add(screen_move_z_01);
		local_view->icon(icon_move_01mm);
		return local_view;
	}

	static ScreenMenu * make_screen_move_e()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_TITLE, MSG_SCREEN_MOVE_E);
		local_view->add(screen_move);
		local_view->icon(icon_back);
		local_view->add(screen_move_e_1);
		local_view->icon(icon_move_1mm);
		local_view->add(screen_move_e_01);
		local_view->icon(icon_move_01mm);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_x_01()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, X_AXIS, X_MIN_POS, X_MAX_POS, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_y_01()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_z_01()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_e_01()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_01MM, E_AXIS, -1E9, 1E9, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_x_1()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, X_AXIS, X_MIN_POS, X_MAX_POS, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_y_1()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_z_1()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_e_1()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_1MM, E_AXIS, -1E9, 1E9, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_x_10()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, X_AXIS, X_MIN_POS, X_MAX_POS, 10, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_y_10()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, Y_AXIS, Y_MIN_POS, Y_MAX_POS, 10, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_move_z_10()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_MOVE_10MM, Z_AXIS, Z_MIN_POS, Z_MAX_POS, 10, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_temperature_main()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 0, 250, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenAction<void> * make_screen_autolevel()
	{
		static ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_AUTOLEVEL, AutoLevelManager::setState); 
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenAction<void> * make_screen_light()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_LIGHT, LightManager::setState);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenAbout * make_screen_info()
	{
		ScreenAbout * local_view = new ScreenAbout(MSG_SCREEN_INFO, MSG_SCREEN_INFO_TEXT, MSG_SCREEN_INFO_BOX, bits_logo_about);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenMenu * make_screen_offset()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_TEXT);
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_offset_home);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenTransition * make_screen_offset_home()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_HOME_TEXT, MSG_SCREEN_OFFSET_WAIT, action_homing);
		local_view->add(screen_offset_calculate);
		return local_view;
	}

	static ScreenTransition * make_screen_offset_calculate()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_PLANE_TEXT, MSG_SCREEN_OFFSET_WAIT, action_offset);
		local_view->add(screen_offset_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_offset_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_DIALOG_TEXT, MSG_SCREEN_OFFSET_DIALOG_BOX, do_nothing);
		local_view->add(screen_offset_set);
		return local_view;
	}

	static ScreenDynamic<float> * make_screen_offset_set()
	{
		ScreenDynamic<float> * local_view = new ScreenDynamic<float>(MSG_SCREEN_OFFSET_TITLE, Z_AXIS, 0.0, 4.0, 0.1, action_set_offset);
		local_view->add(screen_offset_finish);
		return local_view;
	}

	static ScreenAction<void> * make_screen_offset_save()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_OFFSET_TITLE, action_save_offset);
		local_view->add(screen_offset_info);
		local_view->add(screen_offset_save);
		return local_view;
	}

	static ScreenMenu * make_screen_offset_finish()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_TITLE, MSG_SCREEN_OFFSET_FINISH);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenPrint * make_screen_print()
	{
		ScreenPrint * local_view = new ScreenPrint(MSG_SCREEN_PRINT_PRINTING, &TemperatureManager::single::instance());
		local_view->add(screen_play_pause);
		local_view->icon(icon_play_pause);
		local_view->add(screen_stop_confirm);
		local_view->icon(icon_stop);
		local_view->add(screen_change_confirm_first);
		local_view->icon(icon_change_filament);
		local_view->add(screen_speed);
		local_view->icon(icon_change_speed);
		local_view->add(screen_temperature_print);
		local_view->icon(icon_temperature);
		return local_view;
	}

	static ScreenAction<void> * make_screen_play_pause()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_PRINT_PAUSE, PrintManager::togglePause);
		local_view->add(screen_print);
		return local_view;
	}

	static ScreenMenu * make_screen_stop_confirm()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_STOP_CONFIRM);
		local_view->add(screen_print);
		local_view->icon(icon_back);
		local_view->add(screen_stop_OK);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_stop_OK()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_STOP_OK, action_stop_print);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_change_confirm_first()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_CONFIRM);
		local_view->add(screen_print);
		local_view->icon(icon_back);
		local_view->add(screen_change_pausing);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_pausing()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_pause_print);
		local_view->add(screen_change_selector);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_change_selector()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 0, 250, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_change_animation);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_change_animation()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_LOAD_ABORT, &TemperatureManager::single::instance());
		local_view->add(screen_print);
		local_view->add(screen_change_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_START, MSG_SCREEN_CHANGE_BOX, do_nothing);
		local_view->add(screen_change_pullout_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_pullout_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_PULLOUT, MSG_SCREEN_CHANGE_BOX, do_nothing);
		local_view->add(screen_change_unloading);
		return local_view;
	}

	static ScreenTransition * make_screen_change_unloading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_UNLOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
		local_view->add(screen_change_insert_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_insert_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_INSERT, MSG_SCREEN_CHANGE_BOX, do_nothing);
		local_view->add(screen_change_loading);
		return local_view;
	}

	static ScreenTransition * make_screen_change_loading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_LOADING_TEXT, MSG_SCREEN_LEVEL_BOX0, action_filament_load);
		local_view->add(screen_change_confirm_second);
		return local_view;
	}

	static ScreenMenu * make_screen_change_confirm_second()
	{
		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_CHANGE_TITLE, MSG_SCREEN_CHANGE_CONFIRM);
		local_view->add(screen_change_info);
		local_view->icon(icon_retry);
		local_view->add(screen_change_ok2print);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_change_ok2print()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_OK2, action_resume_print);
		local_view->add(screen_print);
		return local_view;
	}

   static ScreenSelector<void, uint16_t> * make_screen_speed()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_SPEED, 10, 400, 100, action_set_feedrate_multiply);
		local_view->add(screen_print);
		return local_view;
	}

   static ScreenSelector<void, uint16_t> * make_screen_temperature_print()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE, 170, 230, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_print);
		return local_view;
	}

	Screen * new_view;

	// Build the UI
	Screen * GuiBuild(ScreenIndex_t const & screen_index)
	{
		switch (screen_index)
		{
			// BQ Logo
			case screen_logo:
				new_view = make_screen_logo();
				break;

			// Main menu
			case screen_main:
				new_view = make_screen_main();
				break;

			// SD Management
			case screen_SD_list:
				new_view = make_screen_SD_list();
				break;
			case screen_SD_confirm:
				new_view = make_screen_SD_confirm();
				break;
			case screen_SD_OK:
				new_view = make_screen_SD_OK();
				break;

			// Unload filament
			case screen_unload_init:
				new_view = make_screen_unload_init();
				break;
			case screen_unload_select:
				new_view = make_screen_unload_select();
				break;
			case screen_unload_heating:
				new_view = make_screen_unload_heating();
				break;
			case screen_unload_info:
				new_view = make_screen_unload_info();
				break;
			case screen_unloading:
				new_view = make_screen_unloading();
				break;
			case screen_unload_confirm:
				new_view = make_screen_unload_confirm();
				break;

			// Load filament
			case screen_load_init:
				new_view = make_screen_load_init();
				break;
			case screen_load_select:
				new_view = make_screen_load_select();
				break;
			case screen_load_heating:
				new_view = make_screen_load_heating();
				break;
			case screen_load_info:
				new_view = make_screen_load_info();
				break;
			case screen_loading:
				new_view = make_screen_loading();
				break;
			case screen_load_confirm:
				new_view = make_screen_load_confirm();
				break;

			// Level plate
			case screen_level_init:
				new_view = make_screen_level_init();
				break;
			case screen_level_cooling:
				new_view = make_screen_level_cooling();
				break;
			case screen_level_animation:
				new_view = make_screen_level_animation();
				break;
			case screen_level_homing:
				new_view = make_screen_level_homing();
				break;
			case screen_level1:
				new_view = make_screen_level1();
				break;
			case screen_level2:
				new_view = make_screen_level2();
				break;
			case screen_level3:
				new_view = make_screen_level3();
				break;
			case screen_level4:
				new_view = make_screen_level4();
				break;
			case screen_level5:
				new_view = make_screen_level5();
				break;
			case screen_level_confirm:
				new_view = make_screen_level_confirm();
				break;

			// Auto home
			case screen_autohome:
				new_view = make_screen_autohome();
				break;

			// Settings
			case screen_settings:
				new_view = make_screen_settings();
				break;

			// Move Axis
			case screen_move:
				new_view = make_screen_move();
				break;
			case screen_move_x:
				new_view = make_screen_move_x();
				break;
			case screen_move_y:
				new_view = make_screen_move_y();
				break;
			case screen_move_z:
				new_view = make_screen_move_z();
				break;
			case screen_move_e:
				new_view = make_screen_move_e();
				break;
			case screen_move_x_01:
				new_view = make_screen_move_x_01();
				break;
			case screen_move_y_01:
				new_view = make_screen_move_y_01();
				break;
			case screen_move_z_01:
				new_view = make_screen_move_z_01();
				break;
			case screen_move_e_01:
				new_view = make_screen_move_e_01();
				break;
			case screen_move_x_1:
				new_view = make_screen_move_x_1();
				break;
			case screen_move_y_1:
				new_view = make_screen_move_y_1();
				break;
			case screen_move_z_1:
				new_view = make_screen_move_z_1();
				break;
			case screen_move_e_1:
				new_view = make_screen_move_e_1();
				break;
			case screen_move_x_10:
				new_view = make_screen_move_x_10();
				break;
			case screen_move_y_10:
				new_view = make_screen_move_y_10();
				break;
			case screen_move_z_10:
				new_view = make_screen_move_z_10();
				break;

			// Steppers
			case screen_stepper:
				new_view = make_screen_stepper();
				break;

			// Temperature
			case screen_temperature_main:
				new_view = make_screen_temperature_main();
				break;

			// Autolevel
			case screen_autolevel:
				new_view = make_screen_autolevel();
				break;

			// Light
			case screen_light:
				new_view = make_screen_light();
				break;

			// Info
			case screen_info:
				new_view = make_screen_info();
				break;
	
			// Offset
			case screen_offset:
				new_view = make_screen_offset();
				break;
			case screen_offset_home:
				new_view = make_screen_offset_home();
				break;
			case screen_offset_calculate:
				new_view = make_screen_offset_calculate();
				break;
      	case screen_offset_info:
				new_view = make_screen_offset_info();
				break;
      	case screen_offset_set:
				new_view = make_screen_offset_set();
				break;
      	case screen_offset_finish:
				new_view = make_screen_offset_finish();
				break;
      	case screen_offset_save:
				new_view = make_screen_offset_save();
				break;

			// Print menu and control
			case screen_print:
				new_view = make_screen_print();
				break;
			case screen_play_pause:
				new_view = make_screen_play_pause();
				break;
			case screen_stop_confirm:
				new_view = make_screen_stop_confirm();
				break;
			case screen_stop_OK:
				new_view = make_screen_stop_OK();
				break;

			// Change filament
			case screen_change_confirm_first:
				new_view = make_screen_change_confirm_first();
				break;
			case screen_change_pausing:
				new_view = make_screen_change_pausing();
				break;
			case screen_change_selector:	
				new_view = make_screen_change_selector();
				break;
			case screen_change_animation:
				new_view = make_screen_change_animation();
				break;
			case screen_change_info:
				new_view = make_screen_change_info();
				break;
			case screen_change_pullout_info:
				new_view = make_screen_change_pullout_info();
				break;
			case screen_change_unloading:
				new_view = make_screen_change_unloading();
				break;
			case screen_change_insert_info:
				new_view = make_screen_change_insert_info();
				break;
			case screen_change_loading:
				new_view = make_screen_change_loading();
				break;
			case screen_change_confirm_second:
				new_view = make_screen_change_confirm_second();
				break;
			case screen_change_ok2print:
				new_view = make_screen_change_ok2print();
				break;

			// Change speed
			case screen_speed:
				new_view = make_screen_speed();
				break;

			// Temperature on print
			case screen_temperature_print:
				new_view = make_screen_temperature_print();
				break;
		}
		return new_view; 
	}
}
