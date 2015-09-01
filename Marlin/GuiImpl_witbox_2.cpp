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
#include "ScreenDynamicAxis.h"
#include "ScreenDynamicFeedrate.h"
#include "ScreenAbout.h"
#include "ScreenPrint.h"
#include "ScreenFile.h"
#include "ScreenSplash.h"
#include "ScreenComplete.h"
#include "ScreenEmergency.h"
#include "ScreenSerial.h"
#include "ScreenLanguage.h"

#include "AutoLevelManager.h"
#include "LightManager.h"
#include "PrintManager.h"
#include "SDManager.h"
#include "SteppersManager.h"
#include "TemperatureManager.h"

#include "Language.h"

namespace screen
{
	///////////////////////
	// Instantiate Icons //
	///////////////////////

	Size icon_size = Size(icon_width, icon_height);
	Size widget_size = Size(widget_width, widget_height);



	///////////////////////////////
	// Instantiation of  Screens //
	///////////////////////////////

	static ScreenSplash * make_screen_splash()
	{
		ScreenSplash * local_view = new ScreenSplash(2000);
		local_view->add(screen_main);
		local_view->add(screen_wizard_language);
		local_view->add(screen_emergency);
		return local_view;
	}

	static ScreenLanguage * make_screen_wizard_language()
	{
		ScreenLanguage * local_view = new ScreenLanguage(NULL, Language::ES);
		local_view->add(screen_wizard_step1);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_wizard_step1()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_WIZARD_TITLE(), MSG_SCREEN_WIZARD_TEXT1(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_wizard_step2);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_wizard_step2()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_WIZARD_TITLE(), MSG_SCREEN_WIZARD_TEXT2(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_wizard_offset_home);
		return local_view;
	}

	static ScreenTransition * make_screen_wizard_offset_home()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_OFFSET_HOME_TITLE(), MSG_SCREEN_OFFSET_HOME_TEXT(), MSG_PLEASE_WAIT(), action_offset_homing);
		local_view->add(screen_wizard_offset_calculate);
		return local_view;
	}

	static ScreenTransition * make_screen_wizard_offset_calculate()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_OFFSET_PLANE_TITLE(), MSG_SCREEN_OFFSET_PLANE_TEXT(), MSG_PLEASE_WAIT(), action_offset);
		local_view->add(screen_wizard_offset_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_wizard_offset_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_OFFSET_INFO_TITLE(), MSG_SCREEN_OFFSET_INFO_TEXT(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_wizard_offset_set);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_wizard_offset_set()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_OFFSET_SET_TITLE(), Z_AXIS, 0.0, 4.0, 0.02, action_set_offset);
		local_view->add(screen_wizard_offset_save);
		return local_view;
	}

	static ScreenAction<void> * make_screen_wizard_offset_save()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_save_offset);
		local_view->add(screen_wizard_offset_finish);
		return local_view;
	}

	static ScreenMenu * make_screen_wizard_offset_finish()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_FINISH_TITLE(), MSG_SCREEN_OFFSET_FINISH_TEXT());
		local_view->add(screen_wizard_offset_home);
		local_view->icon(icon_back);
		local_view->add(screen_wizard_step3);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_wizard_step3()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_WIZARD_TITLE(), MSG_SCREEN_WIZARD_TEXT3(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_wizard_step4);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_wizard_step4()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_WIZARD_TITLE(), MSG_SCREEN_WIZARD_TEXT4(), MSG_PUSH_TO_FINISH(), do_nothing);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenEmergency * make_screen_emergency()
	{
		ScreenEmergency * local_view = new ScreenEmergency(MSG_SCREEN_EMERGENCY_TITLE(), MSG_SCREEN_EMERGENCY_TEXT(), MSG_SCREEN_EMERGENCY_BOX(), bits_emergency);
		return local_view;
	}

	static ScreenMenu * make_screen_main()
	{
	IconStatus<SDState_t> * icon_sd = new IconStatus<SDState_t>(icon_size, bits_nosd_normal, bits_nosd_focused, bits_sd_normal, bits_sd_focused, MSG_ICON_SD_NOSD(), MSG_ICON_SD_SD(), &SDManager::single::instance());
		Icon * icon_filament_unload = new Icon(icon_size, bits_filament_unload_normal, bits_filament_unload_focused, MSG_ICON_FILAMENT_UNLOAD());
		Icon * icon_filament_load = new Icon(icon_size, bits_filament_load_normal, bits_filament_load_focused, MSG_ICON_FILAMENT_LOAD());
		Icon * icon_leveling = new Icon(icon_size, bits_leveling_normal, bits_leveling_focused, MSG_ICON_LEVELING());
		Icon * icon_homing = new Icon(icon_size, bits_homing_normal, bits_homing_focused, MSG_ICON_HOMING());
		Icon * icon_settings = new Icon(icon_size, bits_settings_normal, bits_settings_focused, MSG_ICON_SETTINGS());
		Icon * icon_moveaxis = new Icon(icon_size, bits_moveaxis_normal, bits_moveaxis_focused, MSG_ICON_MOVEAXIS());
		IconStatus<bool> * icon_steppers = new IconStatus<bool>(icon_size, bits_steppers_normal, bits_steppers_focused, bits_steppers_off_normal, bits_steppers_off_focused, MSG_ICON_STEPPERS(), MSG_ICON_STEPPERS_OFF(), &SteppersManager::single::instance());
		IconWidget<float> * widget_temperature = new IconWidget<float>(widget_size, bits_temperature_widget_normal, bits_temperature_widget_focused, MSG_ICON_TEMPERATURE(), &TemperatureManager::single::instance());

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
		ScreenList * local_view = new ScreenList(MSG_SCREEN_SD_LIST_TITLE(), &SDManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_SD_confirm);
		return local_view;
	}

	static ScreenFile * make_screen_SD_confirm()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenFile * local_view = new ScreenFile(MSG_SCREEN_SD_LIST_CONFIRM());
		local_view->add(screen_SD_list);
		local_view->icon(icon_back);
		local_view->add(screen_SD_OK);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenTransition * make_screen_SD_OK()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_SD_LIST_TITLE(), NULL, MSG_PLEASE_WAIT(), PrintManager::startPrint, &PrintManager::single::instance());
		local_view->add(screen_print_heating);
		return local_view;
	}

	static ScreenMenu * make_screen_unload_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_UNLOAD_INIT_TITLE(), MSG_SCREEN_UNLOAD_INIT_TEXT());
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_unload_select);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_unload_select()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_UNLOAD_SELECT_TITLE(), MSG_PUSH_TO_CONFIRM(), min_temp_operation, max_temp_operation, 1, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_unload_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_unload_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_UNLOAD_HEATING_TITLE(), MSG_PLEASE_WAIT(), screen::ScreenAnimation<float>::GREATER_OR_EQUAL, TemperatureManager::single::instance().getTargetTemperature(), &TemperatureManager::single::instance());
		local_view->add(screen_unload_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_unload_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_UNLOAD_INFO_TITLE(), MSG_SCREEN_UNLOAD_INFO_TEXT(), MSG_PUSH_TO_START(), do_nothing);
		local_view->add(screen_unloading);
		return local_view;
	}

	static ScreenTransition * make_screen_unloading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_UNLOADING_TITLE(), MSG_SCREEN_UNLOADING_TEXT(), MSG_PLEASE_WAIT(), action_filament_unload);
		local_view->add(screen_unload_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_unload_confirm()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_UNLOAD_CONFIRM_TITLE(), MSG_SCREEN_UNLOAD_CONFIRM_TEXT());
		local_view->add(screen_unload_info);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenMenu * make_screen_load_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LOAD_INIT_TITLE(), MSG_SCREEN_LOAD_INIT_TEXT());
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_load_select);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_load_select()
	{
		ScreenSelector<void, uint16_t> * local_view  = new ScreenSelector<void, uint16_t>(MSG_SCREEN_LOAD_SELECT_TITLE(), MSG_PUSH_TO_CONFIRM(), min_temp_operation, max_temp_operation, 1, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_load_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_load_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_LOAD_HEATING_TITLE(), MSG_PLEASE_WAIT(), screen::ScreenAnimation<float>::GREATER_OR_EQUAL, TemperatureManager::single::instance().getTargetTemperature(), &TemperatureManager::single::instance());
		local_view->add(screen_load_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_load_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LOAD_INFO_TITLE(), MSG_SCREEN_LOAD_INFO_TEXT(), MSG_PUSH_TO_START(), do_nothing);
		local_view->add(screen_loading);
		return local_view;
	}

	static ScreenTransition * make_screen_loading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_LOADING_TITLE(), MSG_SCREEN_LOADING_TEXT(), MSG_PLEASE_WAIT(), action_filament_load);
		local_view->add(screen_load_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_load_confirm()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LOAD_CONFIRM_TITLE(), MSG_SCREEN_LOAD_CONFIRM_TEXT());
		local_view->add(screen_load_info);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenMenu * make_screen_level_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LEVEL_INIT_TITLE(), MSG_SCREEN_LEVEL_INIT_TEXT());
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
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_LEVEL_HEATING_TITLE(), MSG_PLEASE_WAIT(), screen::ScreenAnimation<float>::LESS_OR_EQUAL, 70, &TemperatureManager::single::instance());
		local_view->add(screen_level_homing);
		return local_view;
	}

	static ScreenTransition * make_screen_level_homing()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_LEVEL_HOMING_TITLE(), MSG_SCREEN_LEVEL_HOMING_TEXT(), MSG_PLEASE_WAIT(), action_homing);
		local_view->add(screen_level1);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level1()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL1_TITLE(), MSG_SCREEN_LEVEL1_TEXT(), MSG_PUSH_TO_CONTINUE(), action_level_plate);
		local_view->add(screen_level2);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level2()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL2_TITLE(), MSG_SCREEN_LEVEL2_TEXT(), MSG_PUSH_TO_CONTINUE(), action_level_plate);
		local_view->add(screen_level3);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_level3()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_LEVEL3_TITLE(), MSG_SCREEN_LEVEL3_TEXT(), MSG_PUSH_TO_CONTINUE(), action_level_plate);
		local_view->add(screen_level4);
		return local_view;
	}

	static ScreenAction<void> * make_screen_level4()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_level_plate);
		local_view->add(screen_level_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_level_confirm()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LEVEL_CONFIRM_TITLE(), MSG_SCREEN_LEVEL_CONFIRM_TEXT());
		local_view->add(screen_level1);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenTransition * make_screen_autohome()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_AUTOHOME_HOMING_TITLE(), MSG_SCREEN_AUTOHOME_HOMING_TEXT(), MSG_PLEASE_WAIT(), gui_action_homing);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_settings()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_offset = new Icon(icon_size, bits_offset_normal, bits_offset_focused, MSG_ICON_OFFSET());
		IconStatus<bool> * icon_autolevel = new IconStatus<bool>(icon_size, bits_autolevel_disable_normal, bits_autolevel_disable_focused, bits_autolevel_normal, bits_autolevel_focused, MSG_ICON_AUTOLEVEL_DISABLE(), MSG_ICON_AUTOLEVEL(), &AutoLevelManager::single::instance());
		IconStatus<bool> * icon_lightled = new IconStatus<bool>(icon_size, bits_lightled_disable_normal, bits_lightled_disable_focused, bits_lightled_normal, bits_lightled_focused, MSG_ICON_LIGHTLED_DISABLE(), MSG_ICON_LIGHTLED(), &LightManager::single::instance());
		Icon * icon_info = new Icon(icon_size, bits_info_normal,              bits_info_focused,              MSG_ICON_INFO());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_SETTINGS_TITLE(), MSG_SCREEN_SETTINGS_TEXT());
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
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, SteppersManager::disableAllSteppers);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_move()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_move_x = new Icon(icon_size, bits_x_axis_normal, bits_x_axis_focused, MSG_ICON_MOVE_X());
		Icon * icon_move_y = new Icon(icon_size, bits_y_axis_normal, bits_y_axis_focused, MSG_ICON_MOVE_Y());
		Icon * icon_move_z = new Icon(icon_size, bits_z_axis_normal, bits_z_axis_focused, MSG_ICON_MOVE_Z());
		Icon * icon_move_e = new Icon(icon_size, bits_e_axis_normal, bits_e_axis_focused, MSG_ICON_MOVE_E());

		ScreenMenu *  local_view = new ScreenMenu(MSG_SCREEN_MOVE_TITLE(), MSG_SCREEN_MOVE_TEXT());
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_move_x);
		local_view->icon(icon_move_x);
		local_view->add(screen_move_y);
		local_view->icon(icon_move_y);
		local_view->add(screen_move_z);
		local_view->icon(icon_move_z);
		local_view->add(screen_move_heat_confirm);
		local_view->icon(icon_move_e);
		return local_view;
	}

	static ScreenMenu * make_screen_move_heat_confirm()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_CONFIRM_TITLE(), MSG_SCREEN_MOVE_CONFIRM_TEXT());
		local_view->add(screen_move);
		local_view->icon(icon_back);
		local_view->add(screen_move_heat);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_move_heat()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_preheat);
		local_view->add(screen_move_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_move_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_MOVE_HEATING_TITLE(), MSG_PLEASE_WAIT(), screen::ScreenAnimation<float>::GREATER_OR_EQUAL, TemperatureManager::single::instance().getTargetTemperature(), &TemperatureManager::single::instance());
		local_view->add(screen_move_e);
		return local_view;
	}

	static ScreenMenu * make_screen_move_x()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_move_10mm = new Icon(icon_size, bits_10mm_normal, bits_10mm_focused, MSG_ICON_MOVE_10MM());
		Icon * icon_move_1mm = new Icon(icon_size, bits_1mm_normal, bits_1mm_focused, MSG_ICON_MOVE_1MM());
		Icon * icon_move_01mm = new Icon(icon_size, bits_01mm_normal, bits_01mm_focused, MSG_ICON_MOVE_01MM());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_X_TITLE(), MSG_SCREEN_MOVE_X_TEXT());
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_move_10mm = new Icon(icon_size, bits_10mm_normal, bits_10mm_focused, MSG_ICON_MOVE_10MM());
		Icon * icon_move_1mm = new Icon(icon_size, bits_1mm_normal, bits_1mm_focused, MSG_ICON_MOVE_1MM());
		Icon * icon_move_01mm = new Icon(icon_size, bits_01mm_normal, bits_01mm_focused, MSG_ICON_MOVE_01MM());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_Y_TITLE(), MSG_SCREEN_MOVE_Y_TEXT());
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_move_10mm = new Icon(icon_size, bits_10mm_normal, bits_10mm_focused, MSG_ICON_MOVE_10MM());
		Icon * icon_move_1mm = new Icon(icon_size, bits_1mm_normal, bits_1mm_focused, MSG_ICON_MOVE_1MM());
		Icon * icon_move_01mm = new Icon(icon_size, bits_01mm_normal, bits_01mm_focused, MSG_ICON_MOVE_01MM());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_Z_TITLE(), MSG_SCREEN_MOVE_Z_TEXT());
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
		Icon * icon_move_1mm = new Icon(icon_size, bits_1mm_normal, bits_1mm_focused, MSG_ICON_MOVE_1MM());
		Icon * icon_move_01mm = new Icon(icon_size, bits_01mm_normal, bits_01mm_focused, MSG_ICON_MOVE_01MM());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_MOVE_E_TITLE(), MSG_SCREEN_MOVE_E_TEXT());
		local_view->add(screen_move);
		local_view->icon(icon_back);
		local_view->add(screen_move_e_1);
		local_view->icon(icon_move_1mm);
		local_view->add(screen_move_e_01);
		local_view->icon(icon_move_01mm);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_x_01()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_X_01MM_TITLE(), X_AXIS, X_MIN_POS, X_MAX_POS, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_y_01()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_Y_01MM_TITLE(), Y_AXIS, Y_MIN_POS, Y_MAX_POS, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_z_01()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_Z_01MM_TITLE(), Z_AXIS, Z_MIN_POS, Z_MAX_POS, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_e_01()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_E_01MM_TITLE(), E_AXIS, -1E9, 1E9, 0.1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_x_1()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_X_1MM_TITLE(), X_AXIS, X_MIN_POS, X_MAX_POS, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_y_1()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_Y_1MM_TITLE(), Y_AXIS, Y_MIN_POS, Y_MAX_POS, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_z_1()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_Z_1MM_TITLE(), Z_AXIS, Z_MIN_POS, Z_MAX_POS, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_e_1()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_E_1MM_TITLE(), E_AXIS, -1E9, 1E9, 1, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_x_10()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_X_10MM_TITLE(), X_AXIS, X_MIN_POS, X_MAX_POS, 10, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_y_10()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_Y_10MM_TITLE(), Y_AXIS, Y_MIN_POS, Y_MAX_POS, 10, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_move_z_10()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_MOVE_Z_10MM_TITLE(), Z_AXIS, Z_MIN_POS, Z_MAX_POS, 10, action_move_axis_to);
		local_view->add(screen_move);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_temperature_main()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE(), MSG_PUSH_TO_CONFIRM(), 0, max_temp_operation, 10, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_heating_main);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_heating_main()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_TEMP_HEATING_TITLE(), MSG_PUSH_TO_CONTINUE(), screen::ScreenAnimation<float>::EQUAL, TemperatureManager::single::instance().getTargetTemperature(), &TemperatureManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenAction<void> * make_screen_autolevel()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, AutoLevelManager::setState); 
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenAction<void> * make_screen_light()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, LightManager::setState);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenAbout * make_screen_info()
	{
		ScreenAbout * local_view = new ScreenAbout(MSG_SCREEN_INFO_TITLE(), NULL, MSG_PUSH_TO_BACK(), bits_logo_about);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenMenu * make_screen_offset()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_INIT_TITLE(), MSG_SCREEN_OFFSET_INIT_TEXT());
		local_view->add(screen_settings);
		local_view->icon(icon_back);
		local_view->add(screen_offset_home);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenTransition * make_screen_offset_home()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_OFFSET_HOME_TITLE(), MSG_SCREEN_OFFSET_HOME_TEXT(), MSG_PLEASE_WAIT(), action_offset_homing);
		local_view->add(screen_offset_calculate);
		return local_view;
	}

	static ScreenTransition * make_screen_offset_calculate()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_OFFSET_PLANE_TITLE(), MSG_SCREEN_OFFSET_PLANE_TEXT(), MSG_PLEASE_WAIT(), action_offset);
		local_view->add(screen_offset_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_offset_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_OFFSET_INFO_TITLE(), MSG_SCREEN_OFFSET_INFO_TEXT(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_offset_set);
		return local_view;
	}

	static ScreenDynamicAxis<float> * make_screen_offset_set()
	{
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_OFFSET_SET_TITLE(), Z_AXIS, 0.0, 4.0, 0.02, action_set_offset);
		local_view->add(screen_offset_save);
		return local_view;
	}

	static ScreenAction<void> * make_screen_offset_save()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_save_offset);
		local_view->add(screen_offset_finish);
		return local_view;
	}

	static ScreenMenu * make_screen_offset_finish()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_FINISH_TITLE(), MSG_SCREEN_OFFSET_FINISH_TEXT());
		local_view->add(screen_offset_home);
		local_view->icon(icon_back);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenPrint * make_screen_print()
	{
		IconStatus<PrinterState_t> * icon_play_pause = new IconStatus<PrinterState_t>(icon_size, bits_pause_normal, bits_pause_focused, bits_play_normal, bits_play_focused, MSG_ICON_PAUSE(), MSG_ICON_PLAY(), &PrintManager::single::instance());
		Icon * icon_stop = new Icon(icon_size, bits_stop_normal, bits_stop_focused, MSG_ICON_STOP());
		Icon * icon_change_filament = new Icon(icon_size, bits_change_filament_normal, bits_change_filament_focused, MSG_ICON_CHANGE_FILAMENT());
		Icon * icon_change_speed = new Icon(icon_size, bits_change_speed_normal, bits_change_speed_focused, MSG_ICON_CHANGE_SPEED());
		Icon * icon_temperature = new Icon(icon_size, bits_temperature_normal, bits_temperature_focused, MSG_ICON_TEMPERATURE());

		ScreenPrint * local_view = new ScreenPrint(MSG_SCREEN_PRINT_PRINTING(), &TemperatureManager::single::instance());
		local_view->add(screen_play_pause);
		local_view->icon(icon_play_pause);
		local_view->add(screen_stop_confirm);
		local_view->icon(icon_stop);
		local_view->add(screen_change_init);
		local_view->icon(icon_change_filament);
		local_view->add(screen_speed);
		local_view->icon(icon_change_speed);
		local_view->add(screen_temperature_print);
		local_view->icon(icon_temperature);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_print_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_PRINT_HEATING_TITLE(), MSG_PLEASE_WAIT(), screen::ScreenAnimation<float>::GREATER_OR_EQUAL, TemperatureManager::single::instance().getTargetTemperature(), &TemperatureManager::single::instance());
		local_view->add(screen_print);
		return local_view;
	}

	static ScreenAction<void> * make_screen_print_action_complete()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_stop_print);
		local_view->add(screen_print_complete);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_print_complete()
	{
		ScreenComplete * local_view = new ScreenComplete(MSG_SCREEN_PRINT_COMPLETE_TITLE(), MSG_SCREEN_PRINT_COMPLETE_TEXT(), MSG_PUSH_TO_CONTINUE(), PrintManager::printingTime());
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenAction<void> * make_screen_play_pause()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(MSG_SCREEN_PRINT_PAUSE(), PrintManager::togglePause);
		local_view->add(screen_print);
		return local_view;
	}

	static ScreenFile * make_screen_stop_confirm()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenFile * local_view = new ScreenFile(MSG_SCREEN_STOP_CONFIRM_TITLE());
		local_view->add(screen_print);
		local_view->icon(icon_back);
		local_view->add(screen_stop_OK);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_stop_OK()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, PrintManager::stopPrint);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_change_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_ICON_BACK());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_CHANGE_INIT_TITLE(), MSG_SCREEN_CHANGE_INIT_TEXT());
		local_view->add(screen_print);
		local_view->icon(icon_back);
		local_view->add(screen_change_pausing);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenTransition * make_screen_change_pausing()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_CHANGE_PAUSE_TITLE(), MSG_SCREEN_CHANGE_PAUSE_TEXT(), MSG_PLEASE_WAIT(), PrintManager::pausePrint);
		local_view->add(screen_change_select);
		return local_view;
	}

	static ScreenSelector<void, uint16_t> * make_screen_change_select()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_CHANGE_SELECT_TITLE(), MSG_PUSH_TO_CONFIRM(), min_temp_operation, max_temp_operation, 1, default_temp_change_filament, action_set_temperature);
		local_view->add(screen_change_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_change_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_CHANGE_HEATING_TITLE(), MSG_PLEASE_WAIT(), screen::ScreenAnimation<float>::GREATER_OR_EQUAL, TemperatureManager::single::instance().getTargetTemperature(), &TemperatureManager::single::instance());
		local_view->add(screen_change_pullout_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_pullout_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_INFO1_TITLE(), MSG_SCREEN_CHANGE_INFO1_TEXT(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_change_unloading);
		return local_view;
	}

	static ScreenTransition * make_screen_change_unloading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_CHANGE_UNLOAD_TITLE(), MSG_SCREEN_CHANGE_UNLOAD_TEXT(), MSG_PLEASE_WAIT(), action_filament_unload);
		local_view->add(screen_change_insert_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_insert_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_INFO2_TITLE(), MSG_SCREEN_CHANGE_INFO2_TEXT(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_change_loading);
		return local_view;
	}

	static ScreenTransition * make_screen_change_loading()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_CHANGE_LOAD_TITLE(), MSG_SCREEN_CHANGE_LOAD_TEXT(), MSG_PLEASE_WAIT(), action_filament_load);
		local_view->add(screen_change_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_change_confirm()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
      Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_CHANGE_CONFIRM_TITLE(), MSG_SCREEN_CHANGE_CONFIRM_TEXT());
		local_view->add(screen_change_pullout_info);
		local_view->icon(icon_retry);
		local_view->add(screen_change_ok2print);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_change_ok2print()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, PrintManager::resumePrint);
		local_view->add(screen_print);
		return local_view;
	}

   static ScreenDynamicFeedrate<uint16_t> * make_screen_speed()
	{
		ScreenDynamicFeedrate<uint16_t> * local_view = new ScreenDynamicFeedrate<uint16_t>(MSG_SCREEN_SPEED_TITLE(), 10, 400, 10, action_set_feedrate_multiply);
		local_view->add(screen_print);
		return local_view;
	}

   static ScreenSelector<void, uint16_t> * make_screen_temperature_print()
	{
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE(), MSG_PUSH_TO_CONFIRM(), min_temp_operation, max_temp_operation, 1, TemperatureManager::single::instance().getTargetTemperature(), action_set_temperature);
		local_view->add(screen_print);
		return local_view;
	}

	static ScreenSerial * make_screen_serial()
	{
		ScreenSerial * local_view = new ScreenSerial(MSG_SCREEN_SERIAL_TITLE(), NULL);
		return local_view;
	}

	Screen * new_view;

	// Build the UI
	Screen * GuiBuild(ScreenIndex_t const & screen_index)
	{
		switch (screen_index)
		{
			// Splash
			case screen_splash:
				new_view = make_screen_splash();
				break;
			
			//Initial wizard
			case screen_wizard_language:
				new_view = make_screen_wizard_language();
				break;
			case screen_wizard_step1:
				new_view = make_screen_wizard_step1();
				break;
			case screen_wizard_step2:
				new_view = make_screen_wizard_step2();
				break;
			case screen_wizard_offset_home:
				new_view = make_screen_wizard_offset_home();
				break;
			case screen_wizard_offset_calculate:
				new_view = make_screen_wizard_offset_calculate();
				break;
			case screen_wizard_offset_info:
				new_view = make_screen_wizard_offset_info();
				break;
			case screen_wizard_offset_set:
				new_view = make_screen_wizard_offset_set();
				break;
			case screen_wizard_offset_save:
				new_view = make_screen_wizard_offset_save();
				break;
			case screen_wizard_offset_finish:
				new_view = make_screen_wizard_offset_finish();
				break;
			case screen_wizard_step3:
				new_view = make_screen_wizard_step3();
				break;
			case screen_wizard_step4:
				new_view = make_screen_wizard_step4();
				break;

			// Emergency stop
			case screen_emergency:
				new_view = make_screen_emergency();
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
			case screen_move_heat_confirm:
				new_view = make_screen_move_heat_confirm();
				break;
			case screen_move_heat:
				new_view = make_screen_move_heat();
				break;
			case screen_move_heating:
				new_view = make_screen_move_heating();
				break;

			// Steppers
			case screen_stepper:
				new_view = make_screen_stepper();
				break;

			// Temperature
			case screen_temperature_main:
				new_view = make_screen_temperature_main();
				break;
			case screen_heating_main:
				new_view = make_screen_heating_main();
				break;
			case screen_print_heating:
				new_view = make_screen_print_heating();
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
			case screen_print_action_complete:
				new_view = make_screen_print_action_complete();
				break;
			case screen_print_complete:
				new_view = make_screen_print_complete();
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
			case screen_change_init:
				new_view = make_screen_change_init();
				break;
			case screen_change_pausing:
				new_view = make_screen_change_pausing();
				break;
			case screen_change_select:
				new_view = make_screen_change_select();
				break;
			case screen_change_heating:
				new_view = make_screen_change_heating();
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
			case screen_change_confirm:
				new_view = make_screen_change_confirm();
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

			// Serial screen
			case screen_serial:
				new_view = make_screen_serial();
				break;
		}
		return new_view; 
	}
}
