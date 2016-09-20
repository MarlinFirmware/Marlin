///////////////////////////////////////////////////////////////////////////////
/// \file GuiImpl_witbox_2.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Specific implementation of UI for BQ Witbox 2 and Hephestos 2 printers
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

#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"
#include "GuiAction.h"
#include "Action.h"

#include "Icon.h"
#include "IconWidgetBed.h"
#include "IconStatus.h"

#include "Option.h"
#include "OptionToggle.h"
#include "OptionLaunch.h"

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
#include "ScreenInactivity.h"
#include "ScreenSwitch.h"
#include "ScreenSetting.h"
#include "ScreenLanguage.h"
#include "ScreenTemperature.h"
#include "ScreenCooldown.h"
#include "ScreenStop.h"
#include "ScreenStats.h"
#include "ScreenError.h"

#include "AutoLevelManager.h"
#include "LightManager.h"
#include "PrintManager.h"
#include "SDManager.h"
#include "SteppersManager.h"
#include "TemperatureManager.h"
#include "SerialManager.h"
#include "FanManager.h"
#include "HeatedbedManager.h"

#include "Language.h"

namespace ui
{
	///////////////////////
	// Instantiate Icons //
	///////////////////////

	Size icon_size = Size(icon_width, icon_height);
	Size widget_size = Size(widget_width, widget_height);
	option::Size option_size = option::Size(10,10);


	///////////////////////////////
	// Instantiation of  Screens //
	///////////////////////////////

	static ScreenSplash * make_screen_splash()
	{
		ScreenSplash * local_view = new ScreenSplash(2000);
		local_view->add(screen_main);
		local_view->add(screen_wizard_init);
		local_view->add(screen_emergency);
		return local_view;
	}

	static ScreenAction<void> * make_screen_wizard_init()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_wizard_init);
		local_view->add(screen_wizard_language);
		return local_view;
	}

	static ScreenLanguage * make_screen_wizard_language()
	{
		ScreenLanguage * local_view = new ScreenLanguage(NULL, Language::EN);
		local_view->add(screen_wizard_switch);
		return local_view;
	}

	static ScreenSwitch * make_screen_wizard_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_wizard);
		local_view->add(screen_wizard_step3);
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
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_OFFSET_SET_TITLE(), Z_AXIS, 0.0, 5.0, 0.02, action_set_offset, true);
		local_view->add(screen_wizard_offset_rest);
		return local_view;
	}

	static ScreenAction<void> * make_screen_wizard_offset_rest()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_offset_rest);
		local_view->add(screen_wizard_offset_finish);
		return local_view;
	}

	static ScreenMenu * make_screen_wizard_offset_finish()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_FINISH_TITLE(), MSG_SCREEN_OFFSET_FINISH_TEXT());
		local_view->add(screen_wizard_offset_home);
		local_view->icon(icon_retry);
		local_view->add(screen_wizard_offset_save);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_wizard_offset_save()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_save_offset);
		local_view->add(screen_wizard_step3);
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
		local_view->add(screen_wizard_finish);
		return local_view;
	}

	static ScreenAction<void> * make_screen_wizard_finish()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_wizard_finish);
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
		IconStatus<bool> * icon_sd = new IconStatus<bool>(icon_size, bits_nosd_normal, bits_nosd_focused, bits_sd_normal, bits_sd_focused, MSG_ICON_SD_NOSD(), MSG_ICON_SD_SD(), &SDManager::single::instance());
		Icon * icon_filament_unload = new Icon(icon_size, bits_filament_unload_normal, bits_filament_unload_focused, MSG_ICON_FILAMENT_UNLOAD());
		Icon * icon_filament_load = new Icon(icon_size, bits_filament_load_normal, bits_filament_load_focused, MSG_ICON_FILAMENT_LOAD());
		Icon * icon_leveling = new Icon(icon_size, bits_leveling_normal, bits_leveling_focused, MSG_ICON_LEVELING());
		Icon * icon_homing = new Icon(icon_size, bits_homing_normal, bits_homing_focused, MSG_ICON_HOMING());
		Icon * icon_settings = new Icon(icon_size, bits_settings_normal, bits_settings_focused, MSG_ICON_SETTINGS());
		Icon * icon_moveaxis = new Icon(icon_size, bits_moveaxis_normal, bits_moveaxis_focused, MSG_ICON_MOVEAXIS());
		IconStatus<bool> * icon_steppers = new IconStatus<bool>(icon_size, bits_steppers_normal, bits_steppers_focused, bits_steppers_off_normal, bits_steppers_off_focused, MSG_ICON_STEPPERS(), MSG_ICON_STEPPERS_OFF(), &SteppersManager::single::instance());
	
		Icon * widget_temperature;
#if MB(BQ_ZUM_MEGA_3D)
		if(HeatedbedManager::single::instance().detected())
		{
			widget_temperature = new IconWidgetBed<float>(widget_size, bits_cold_bed_widget_normal, bits_cold_bed_widget_focused, bits_hot_bed_widget_normal, bits_hot_bed_widget_focused, MSG_ICON_TEMPERATURE(), &temp::TemperatureManager::single::instance());
		}
		else
#endif // MB(BQ_ZUM_MEGA_3D)
		{
			widget_temperature = new IconWidgetBed<float>(widget_size, bits_temperature_widget_normal, bits_temperature_widget_focused, NULL, NULL, MSG_ICON_TEMPERATURE(), &temp::TemperatureManager::single::instance());
		}

		ScreenMenu * local_view = new ScreenMenu();
		local_view->add(screen_SD_list);
		local_view->icon(icon_sd);
		local_view->add(screen_load_init);
		local_view->icon(icon_filament_load);
		local_view->add(screen_unload_init);
		local_view->icon(icon_filament_unload);
		local_view->add(screen_level_init);
		local_view->icon(icon_leveling);
		local_view->add(screen_autohome_init);
		local_view->icon(icon_homing);
		local_view->add(screen_settings);
		local_view->icon(icon_settings);
		local_view->add(screen_move_switch);
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
		local_view->add(screen_SD_name_error);
		return local_view;
	}
	
	static ScreenFile * make_screen_SD_confirm()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenFile * local_view = new ScreenFile(MSG_SCREEN_SD_LIST_CONFIRM(), &SDManager::single::instance());
		local_view->add(screen_SD_list);
		local_view->icon(icon_back);
		local_view->add(screen_print_switch);
		local_view->icon(icon_ok);
		return local_view;
	}
	
	static ScreenError * make_screen_SD_name_error()
	{
		ScreenError * local_view = new ScreenError(MSG_SCREEN_ERROR_TITLE(), MSG_SCREEN_NAME_ERROR_TEXT(), MSG_PUSH_TO_BACK(), bits_emergency);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_unload_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_UNLOAD_SELECT_TITLE(), MSG_PUSH_TO_CONFIRM(), temp::min_temp_operation, temp::max_temp_operation, 1, temp::default_temp_change_filament, action_set_temperature);
		local_view->add(screen_unload_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_unload_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_UNLOAD_HEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_unload_switch);
		return local_view;
	}

	static ScreenSwitch * make_screen_unload_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, PrintManager::knownPosition);
		local_view->add(screen_move_to_unload);
		local_view->add(screen_unload_home);
		return local_view;
	}

	static ScreenTransition * make_screen_unload_home()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_UNLOAD_HOME_TITLE(), MSG_SCREEN_UNLOAD_HOME_TEXT(), MSG_PLEASE_WAIT(), action_homing);
		local_view->add(screen_move_to_unload);
		return local_view;
	}

	static ScreenTransition * make_screen_move_to_unload()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_MOVE_TO_UNLOAD_TITLE(), MSG_SCREEN_MOVE_TO_UNLOAD_TEXT(), MSG_PLEASE_WAIT(), action_move_to_filament_change);
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
		local_view->add(screen_unload_rest);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_unload_rest()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_move_to_rest);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_load_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		ScreenSelector<void, uint16_t> * local_view  = new ScreenSelector<void, uint16_t>(MSG_SCREEN_LOAD_SELECT_TITLE(), MSG_PUSH_TO_CONFIRM(), temp::min_temp_operation, temp::max_temp_operation, 1, temp::default_temp_change_filament, action_set_temperature);
		local_view->add(screen_load_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_load_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_LOAD_HEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_load_switch);
		return local_view;
	}

	static ScreenSwitch * make_screen_load_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, PrintManager::knownPosition);
		local_view->add(screen_move_to_load);
		local_view->add(screen_load_home);
		return local_view;
	}

	static ScreenTransition * make_screen_load_home()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_LOAD_HOME_TITLE(), MSG_SCREEN_LOAD_HOME_TEXT(), MSG_PLEASE_WAIT(), action_homing);
		local_view->add(screen_move_to_load);
		return local_view;
	}

	static ScreenTransition * make_screen_move_to_load()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_MOVE_TO_LOAD_TITLE(), MSG_SCREEN_MOVE_TO_LOAD_TEXT(), MSG_PLEASE_WAIT(), action_move_to_filament_change);
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
		local_view->add(screen_load_rest);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_load_rest()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_move_to_rest);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_level_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LEVEL_INIT_TITLE(), MSG_SCREEN_LEVEL_INIT_TEXT());
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_level_switch_preheat);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSwitch * make_screen_level_switch_preheat()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_preheat_temp);
		local_view->add(screen_level_switch);
		local_view->add(screen_level_preheat);
		return local_view;
	}

	static ScreenAction<void> * make_screen_level_preheat()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_preheat);
		local_view->add(screen_level_preheating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_level_preheating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_LEVEL_PREHEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_level_switch);
		return local_view;
	}

	static ScreenSwitch * make_screen_level_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, PrintManager::knownPosition);
		local_view->add(screen_level1);
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
		local_view->add(screen_level_z_homing);
		return local_view;
	}

	static ScreenTransition * make_screen_level_z_homing()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_LEVEL_HOMING_TITLE(), MSG_SCREEN_LEVEL_HOMING_TEXT(), MSG_PLEASE_WAIT(), gui_action_z_homing);
		local_view->add(screen_level_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_level_confirm()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_LEVEL_CONFIRM_TITLE(), MSG_SCREEN_LEVEL_CONFIRM_TEXT());
		local_view->add(screen_level_homing);
		local_view->icon(icon_retry);
		local_view->add(screen_main);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenMenu * make_screen_autohome_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_AUTOHOME_INIT_TITLE(), MSG_SCREEN_AUTOHOME_INIT_TEXT());
		local_view->add(screen_main);
		local_view->icon(icon_back);
		local_view->add(screen_autohome_switch);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSwitch * make_screen_autohome_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_preheat_temp);
		local_view->add(screen_autohome_homing);
		local_view->add(screen_autohome_heating);
		return local_view;
	}

	static ScreenAction<void> * make_screen_autohome_heating()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_preheat);
		local_view->add(screen_autohome_animation);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_autohome_animation()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_AUTOHOME_HEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_autohome_homing);
		return local_view;
	}


	static ScreenTransition * make_screen_autohome_homing()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_AUTOHOME_HOMING_TITLE(), MSG_SCREEN_AUTOHOME_HOMING_TEXT(), MSG_PLEASE_WAIT(), gui_action_homing);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenSetting * make_screen_settings()
	{
		OptionLaunch * option_back        = new OptionLaunch(option_size, MSG_BACK());
		option_back->add(screen_main);
		OptionToggle<bool> * option_autolevel   = new OptionToggle<bool> (option_size, MSG_OPTION_AUTOLEVEL(), AutoLevelManager::setState, &AutoLevelManager::single::instance());
#ifdef LIGHT_ENABLED
		OptionToggle<uint8_t>  * option_led         = new OptionToggle<uint8_t>(option_size, MSG_OPTION_LIGHTLED(), LightManager::setMode, &LightManager::single::instance());
#endif // LIGHT_ENABLED
#ifdef FAN_BOX_PIN
		OptionToggle<bool>  * option_fan         = new OptionToggle<bool>(option_size, MSG_OPTION_BOX_FAN(), FanManager::toogleState, &FanManager::single::instance());
#endif // FAN_BOX_PIN
#if MB(BQ_ZUM_MEGA_3D)
		OptionToggle<uint8_t>  * option_hotbed   = new OptionToggle<uint8_t> (option_size, MSG_OPTION_HEATED_BED(), HeatedbedManager::toggleMode, &HeatedbedManager::single::instance());
#endif // MB(BQ_ZUM_MEGA_3D)
		OptionToggle<bool>  * option_serial      = new OptionToggle<bool> (option_size, MSG_OPTION_SERIAL(), SerialManager::setState, &SerialManager::single::instance());
		OptionLaunch * option_offset      = new OptionLaunch(option_size, MSG_OPTION_OFFSET());
		option_offset->add(screen_offset);
		OptionLaunch * option_about       = new OptionLaunch(option_size, MSG_OPTION_INFO());
		option_about->add(screen_info);
		OptionLaunch * option_contact     = new OptionLaunch(option_size, MSG_OPTION_CONTACT());
		option_contact->add(screen_contact);
		OptionLaunch * option_language    = new OptionLaunch(option_size, MSG_OPTION_LANGUAGE());
		option_language->add(screen_settings_language);
		OptionLaunch * option_reset       = new OptionLaunch(option_size, MSG_OPTION_RESET());
		option_reset->add(screen_reset_init);
		OptionLaunch * option_stats       = new OptionLaunch(option_size, MSG_OPTION_STATS());
		option_stats->add(screen_view_stats);

		ScreenSetting * local_view = new ScreenSetting(MSG_SCREEN_SETTINGS_TITLE());
		local_view->add(option_back);
		local_view->add(option_autolevel);
#ifdef LIGHT_ENABLED
		local_view->add(option_led);
#endif // LIGHT_ENABLED
#ifdef FAN_BOX_PIN
		local_view->add(option_fan);
#endif // FAN_BOX_PIN
#if MB(BQ_ZUM_MEGA_3D)
		if(HeatedbedManager::single::instance().detected())
		{
			local_view->add(option_hotbed);
		}
#endif // MB(BQ_ZUM_MEGA_3D)
		local_view->add(option_serial);
		local_view->add(option_offset);
		local_view->add(option_language);
		local_view->add(option_reset);
		local_view->add(option_stats);
		local_view->add(option_contact);
		local_view->add(option_about);
		return local_view;
	}

	static ScreenAction<void> * make_screen_stepper()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, SteppersManager::disableAllSteppers);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenSwitch * make_screen_move_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, PrintManager::knownPosition);
		local_view->add(screen_move);
		local_view->add(screen_move_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_move_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_MOVE_INFO_TITLE(), MSG_SCREEN_MOVE_INFO_TEXT(), MSG_PUSH_TO_BACK(), do_nothing);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_move()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		local_view->add(screen_move_heat_switch);
		local_view->icon(icon_move_e);
		return local_view;
	}

	static ScreenSwitch * make_screen_move_heat_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_preheat_temp);
		local_view->add(screen_move_e);
		local_view->add(screen_move_heat_confirm);
		return local_view;
	}

	static ScreenMenu * make_screen_move_heat_confirm()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_MOVE_HEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_move_e);
		return local_view;
	}

	static ScreenMenu * make_screen_move_x()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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

	static ScreenTemperature * make_screen_temperature_main()
	{
		ScreenTemperature * local_view = new ScreenTemperature(MSG_SCREEN_TEMP_TITLE(), MSG_PUSH_TO_CONFIRM(), temp::min_temp_cooling, temp::max_temp_operation, 10, temp::default_temp_change_filament, action_set_temperature);
		local_view->add(screen_temperature_main_switch);
		return local_view;
	}

	static ScreenSwitch * make_screen_temperature_main_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_cooling);
		local_view->add(screen_cooling_main);
		local_view->add(screen_heating_main);
		return local_view;
	}

	static ScreenCooldown * make_screen_cooling_main()
	{
		ScreenCooldown * local_view = new ScreenCooldown(MSG_SCREEN_TEMP_HEATING_TITLE(), MSG_PUSH_TO_CONTINUE(), temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_heating_main()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_TEMP_HEATING_TITLE(), MSG_PUSH_TO_CONTINUE(), ScreenAnimation<float>::GREATER_OR_EQUAL, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_main);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenAbout * make_screen_info()
	{
		ScreenAbout * local_view = new ScreenAbout(MSG_SCREEN_INFO_TITLE(), NULL, MSG_PUSH_TO_BACK(), bits_logo_about);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_contact()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CONTACT_TITLE(), MSG_SCREEN_CONTACT_TEXT(), MSG_PUSH_TO_BACK(), do_nothing);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenLanguage * make_screen_settings_language()
	{
		ScreenLanguage * local_view = new ScreenLanguage(NULL, Language::EN);
		local_view->add(screen_settings);
		return local_view;
	}

	static ScreenMenu * make_screen_offset()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_INIT_TITLE(), MSG_SCREEN_OFFSET_INIT_TEXT());
		local_view->add(screen_settings);
		local_view->icon(icon_back);
		local_view->add(screen_offset_switch);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenSwitch * make_screen_offset_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_preheat_temp);
		local_view->add(screen_offset_home);
		local_view->add(screen_offset_preheat);
		return local_view;
	}

	static ScreenAction<void> * make_screen_offset_preheat()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_preheat);
		local_view->add(screen_offset_preheating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_offset_preheating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_OFFSET_PREHEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_offset_home);
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
		ScreenDynamicAxis<float> * local_view = new ScreenDynamicAxis<float>(MSG_SCREEN_OFFSET_SET_TITLE(), Z_AXIS, -5.0, 5.0, 0.02, action_set_offset, true);
		local_view->add(screen_offset_rest);
		return local_view;
	}

	static ScreenAction<void> * make_screen_offset_rest()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_offset_rest);
		local_view->add(screen_offset_finish);
		return local_view;
	}

	static ScreenMenu * make_screen_offset_finish()
	{
		Icon * icon_retry = new Icon(icon_size, bits_retry_normal, bits_retry_focused, MSG_ICON_RETRY());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_OFFSET_FINISH_TITLE(), MSG_SCREEN_OFFSET_FINISH_TEXT());
		local_view->add(screen_offset_home);
		local_view->icon(icon_retry);
		local_view->add(screen_offset_save);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenAction<void> * make_screen_offset_save()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_save_offset);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenSwitch * make_screen_print_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_preheat_temp);
		local_view->add(screen_print_prepare);
		local_view->add(screen_print_preheat);
		return local_view;
	}

	static ScreenAction<void> * make_screen_print_preheat()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_preheat);
		local_view->add(screen_print_preheating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_print_preheating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_PRINT_HEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_print_prepare);
		return local_view;
	}

	static ScreenTransition * make_screen_print_prepare()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_SD_LIST_TITLE(), NULL, MSG_PLEASE_WAIT(), PrintManager::startPrint, &PrintManager::single::instance());
		local_view->add(screen_print);
		return local_view;
	}
	
	static ScreenPrint * make_screen_print()
	{
		IconStatus<PrinterState_t> * icon_play_pause = new IconStatus<PrinterState_t>(icon_size, bits_pause_normal, bits_pause_focused, bits_play_normal, bits_play_focused, MSG_ICON_PAUSE(), MSG_ICON_PLAY(), &PrintManager::single::instance());
		Icon * icon_stop = new Icon(icon_size, bits_stop_normal, bits_stop_focused, MSG_ICON_STOP());
		Icon * icon_change_filament = new Icon(icon_size, bits_change_filament_normal, bits_change_filament_focused, MSG_ICON_CHANGE_FILAMENT());
		Icon * icon_change_speed = new Icon(icon_size, bits_change_speed_normal, bits_change_speed_focused, MSG_ICON_CHANGE_SPEED());
		Icon * icon_temperature = new Icon(icon_size, bits_temperature_normal, bits_temperature_focused, MSG_ICON_TEMPERATURE());

		ScreenPrint * local_view = new ScreenPrint(MSG_SCREEN_PRINT_PRINTING(), &temp::TemperatureManager::single::instance());
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

	static ScreenAction<void> * make_screen_print_action_complete()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_finish_print);
		local_view->add(screen_print_complete);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_print_complete()
	{
		ScreenComplete * local_view = new ScreenComplete(MSG_SCREEN_PRINT_COMPLETE_TITLE(), MSG_SCREEN_PRINT_COMPLETE_TEXT(), MSG_PUSH_TO_CONTINUE(), PrintManager::completePrint());
		local_view->add(screen_print_maintenance_switch);
		return local_view;
	}
	
	static ScreenSwitch * make_screen_print_maintenance_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, PrintManager::single::instance().checkPTFEMaintenance);
		local_view->add(screen_print_maintenance_reminder);
		local_view->add(screen_close_inactivity);
		return local_view;
	}
	
	static ScreenError * make_screen_print_maintenance_reminder()
	{
		ScreenError * local_view = new ScreenError(MSG_SCREEN_PTFE_TITLE(), MSG_SCREEN_PTFE_TEXT(), MSG_PUSH_TO_CONTINUE(), bits_emergency);
		local_view->add(screen_close_inactivity);
		return local_view;
	}

	static ScreenAction<void> * make_screen_close_inactivity()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_close_inactivity);
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenEmergency * make_screen_error_temperature()
	{
		ScreenEmergency * local_view = new ScreenEmergency(MSG_SCREEN_ERROR_TITLE(), MSG_SCREEN_ERROR_TEMPERATURE_TEXT(), MSG_SCREEN_EMERGENCY_BOX(), bits_emergency);
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
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		local_view->add(screen_stop_info);
		return local_view;
	}

	static ScreenStop * make_screen_stop_info()
	{
		ScreenStop * local_view = new ScreenStop(MSG_SCREEN_PRINT_STOP_TITLE(), MSG_SCREEN_PRINT_STOP_TEXT(), MSG_PUSH_TO_CONTINUE(), action_get_height(), PrintManager::printingTime());
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_change_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
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
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_CHANGE_SELECT_TITLE(), MSG_PUSH_TO_CONFIRM(), temp::min_temp_operation, temp::max_temp_operation, 1, temp::default_temp_change_filament, action_set_temperature);
		local_view->add(screen_change_heating);
		return local_view;
	}

	static ScreenAnimation<float> * make_screen_change_heating()
	{
		ScreenAnimation<float> * local_view = new ScreenAnimation<float>(MSG_SCREEN_CHANGE_HEATING_TITLE(), MSG_PLEASE_WAIT(), ScreenAnimation<float>::RANGE, temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_change_pause_switch);
		return local_view;
	}
	
	static ScreenSwitch * make_screen_change_pause_switch()
	{
		ScreenSwitch * local_view = new ScreenSwitch(NULL, action_check_pause_state);
		local_view->add(screen_change_pullout_info);
		local_view->add(screen_change_wait_pause);
		return local_view;
	}
	
	static ScreenTransition * make_screen_change_wait_pause()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_CHANGE_WAIT_PAUSE_TITLE(), MSG_SCREEN_CHANGE_WAIT_PAUSE_TEXT(), MSG_PLEASE_WAIT(), do_nothing, &PrintManager::single::instance());
		local_view->add(screen_change_pullout_info);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_change_pullout_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_CHANGE_INFO1_TITLE(), MSG_SCREEN_CHANGE_INFO1_TEXT(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_move_to_change);
		return local_view;
	}

	static ScreenTransition * make_screen_move_to_change()
	{
		ScreenTransition * local_view = new ScreenTransition(MSG_SCREEN_MOVE_TO_CHANGE_TITLE(), MSG_SCREEN_MOVE_TO_CHANGE_TEXT(), MSG_PLEASE_WAIT(), action_move_to_filament_change);
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
		ScreenSelector<void, uint16_t> * local_view = new ScreenSelector<void, uint16_t>(MSG_SCREEN_TEMP_TITLE(), MSG_PUSH_TO_CONFIRM(), temp::min_temp_operation, temp::max_temp_operation, 1, temp::TemperatureManager::single::instance().getTargetTemperature(), action_set_temperature);
		local_view->add(screen_print);
		return local_view;
	}

	static ScreenSerial * make_screen_serial()
	{
		ScreenSerial * local_view = new ScreenSerial(MSG_SCREEN_SERIAL_TITLE(), NULL);
		return local_view;
	}

	static ScreenInactivity * make_screen_inactivity()
	{
		ScreenInactivity * local_view = new ScreenInactivity(NULL, MSG_PUSH_TO_BACK(), temp::TemperatureManager::single::instance().getTargetTemperature(), &temp::TemperatureManager::single::instance());
		local_view->add(screen_main);
		return local_view;
	}

	static ScreenMenu * make_screen_reset_init()
	{
		Icon * icon_back = new Icon(icon_size, bits_back_normal, bits_back_focused, MSG_BACK());
		Icon * icon_ok = new Icon(icon_size, bits_ok_normal, bits_ok_focused, MSG_ICON_OK2());

		ScreenMenu * local_view = new ScreenMenu(MSG_SCREEN_RESET_INIT_TITLE(), MSG_SCREEN_RESET_INIT_TEXT());
		local_view->add(screen_settings);
		local_view->icon(icon_back);
		local_view->add(screen_reset_info);
		local_view->icon(icon_ok);
		return local_view;
	}

	static ScreenDialog<void> * make_screen_reset_info()
	{
		ScreenDialog<void> * local_view = new ScreenDialog<void>(MSG_SCREEN_RESET_INFO_TITLE(), MSG_SCREEN_RESET_INFO_TEXT(), MSG_PUSH_TO_CONTINUE(), do_nothing);
		local_view->add(screen_reset);
		return local_view;
	}

	static ScreenEmergency * make_screen_reset()
	{
		ScreenEmergency * local_view = new ScreenEmergency(MSG_SCREEN_RESET_TITLE(), MSG_SCREEN_RESET_TEXT(), MSG_PLEASE_WAIT(), bits_emergency);
		local_view->add(screen_resetting);
		return local_view;
	}

	static ScreenAction<void> * make_screen_resetting()
	{
		ScreenAction<void> * local_view = new ScreenAction<void>(NULL, action_erase_EEPROM);
		return local_view;
	}
	
	static ScreenStats * make_screen_view_stats()
	{
		ScreenStats * local_view = new ScreenStats(MSG_SCREEN_VIEW_STATS_TITLE(), MSG_PUSH_TO_BACK());
		local_view->add(screen_settings);
		return local_view;
	}
	
#ifdef BED_DETECTION
	static ScreenError * make_screen_base_error()
	{
		ScreenError * local_view = new ScreenError(MSG_SCREEN_ERROR_TITLE(), MSG_SCREEN_BASE_ERROR_TEXT(), MSG_PUSH_TO_BACK(), bits_emergency);
		local_view->add(screen_main);
		return local_view;
	}
#endif // BED_DETECTION

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
			case screen_wizard_init:
				new_view = make_screen_wizard_init();
				break;
			case screen_wizard_language:
				new_view = make_screen_wizard_language();
				break;
			case screen_wizard_switch:
				new_view = make_screen_wizard_switch();
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
			case screen_wizard_offset_rest:
				new_view = make_screen_wizard_offset_rest();
				break;
			case screen_wizard_offset_finish:
				new_view = make_screen_wizard_offset_finish();
				break;
			case screen_wizard_offset_save:
				new_view = make_screen_wizard_offset_save();
				break;
			case screen_wizard_step3:
				new_view = make_screen_wizard_step3();
				break;
			case screen_wizard_step4:
				new_view = make_screen_wizard_step4();
				break;
			case screen_wizard_finish:
				new_view = make_screen_wizard_finish();
				break;

			// Emergency stop
			case screen_emergency:
				new_view = make_screen_emergency();
				break;

			// Reset EEPROM
			case screen_reset_init:
				new_view = make_screen_reset_init();
				break;
			case screen_reset_info:
				new_view = make_screen_reset_info();
				break;
			case screen_reset:
				new_view = make_screen_reset();
				break;
			case screen_resetting:
				new_view = make_screen_resetting();
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
			case screen_SD_name_error:
				new_view = make_screen_SD_name_error();
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
			case screen_unload_switch:
				new_view = make_screen_unload_switch();
				break;
			case screen_unload_home:
				new_view = make_screen_unload_home();
				break;
			case screen_move_to_unload:
				new_view = make_screen_move_to_unload();
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
			case screen_unload_rest:
				new_view = make_screen_unload_rest();
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
			case screen_load_switch:
				new_view = make_screen_load_switch();
				break;
			case screen_load_home:
				new_view = make_screen_load_home();
				break;
			case screen_move_to_load:
				new_view = make_screen_move_to_load();
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
			case screen_load_rest:
				new_view = make_screen_load_rest();
				break;

			// Level plate
			case screen_level_init:
				new_view = make_screen_level_init();
				break;
			case screen_level_switch_preheat:
				new_view = make_screen_level_switch_preheat();
				break;
			case screen_level_preheat:
				new_view = make_screen_level_preheat();
				break;
			case screen_level_preheating:
				new_view = make_screen_level_preheating();
				break;
			case screen_level_switch:
				new_view = make_screen_level_switch();
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
			case screen_level_z_homing:
				new_view = make_screen_level_z_homing();
				break;
			case screen_level_confirm:
				new_view = make_screen_level_confirm();
				break;

			// Auto home
			case screen_autohome_init:
				new_view = make_screen_autohome_init();
				break;
			case screen_autohome_switch:
				new_view = make_screen_autohome_switch();
				break;
			case screen_autohome_heating:
				new_view = make_screen_autohome_heating();
				break;

			case screen_autohome_animation:
				new_view = make_screen_autohome_animation();
				break;

			case screen_autohome_homing:
				new_view = make_screen_autohome_homing();
				break;

			// Settings
			case screen_settings:
				new_view = make_screen_settings();
				break;

			// Move Axis
			case screen_move_switch:
				new_view = make_screen_move_switch();
				break;
			case screen_move_info:
				new_view = make_screen_move_info();
				break;
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
			case screen_move_heat_switch:
				new_view = make_screen_move_heat_switch();
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
			case screen_temperature_main_switch:
				new_view = make_screen_temperature_main_switch();
				break;
			case screen_cooling_main:
				new_view = make_screen_cooling_main();
				break;
			case screen_heating_main:
				new_view = make_screen_heating_main();
				break;

			// Info
			case screen_info:
				new_view = make_screen_info();
				break;
			// Contact
			case screen_contact:
				new_view = make_screen_contact();
				break;

			//Language
			case screen_settings_language:
				new_view = make_screen_settings_language();
				break;

			// Offset
			case screen_offset:
				new_view = make_screen_offset();
				break;
			case screen_offset_switch:
				new_view = make_screen_offset_switch();
				break;
			case screen_offset_preheat:
				new_view = make_screen_offset_preheat();
				break;
			case screen_offset_preheating:
				new_view = make_screen_offset_preheating();
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
			case screen_offset_rest:
				new_view = make_screen_offset_rest();
				break;
      		case screen_offset_finish:
				new_view = make_screen_offset_finish();
				break;
      		case screen_offset_save:
				new_view = make_screen_offset_save();
				break;

			// Print menu and control
			case screen_print_switch:
				new_view = make_screen_print_switch();
				break;
			case screen_print_preheat:
				new_view = make_screen_print_preheat();
				break;
			case screen_print_preheating:
				new_view = make_screen_print_preheating();
				break;
			case screen_print_prepare:
				new_view = make_screen_print_prepare();
				break;
			case screen_print:
				new_view = make_screen_print();
				break;
			case screen_print_action_complete:
				new_view = make_screen_print_action_complete();
				break;
			case screen_print_complete:
				new_view = make_screen_print_complete();
				break;
			case screen_print_maintenance_switch:
				new_view = make_screen_print_maintenance_switch();
				break;
			case screen_print_maintenance_reminder:
				new_view = make_screen_print_maintenance_reminder();
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
			case screen_stop_info:
				new_view = make_screen_stop_info();
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
			case screen_change_pause_switch:
				new_view = make_screen_change_pause_switch();
				break;
			case screen_change_wait_pause:
				new_view = make_screen_change_wait_pause();
				break;
			case screen_change_pullout_info:
				new_view = make_screen_change_pullout_info();
				break;
			case screen_move_to_change:
				new_view = make_screen_move_to_change();
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

			// Inactivity screen
			case screen_inactivity:
				new_view = make_screen_inactivity();
				break;
			case screen_close_inactivity:
				new_view = make_screen_close_inactivity();
				break;

			// Error screens
			case screen_error_temperature:
				new_view = make_screen_error_temperature();
				break;

#ifdef BED_DETECTION
			case screen_base_error:
				new_view = make_screen_base_error();
				break;
#endif // BED_DETECTION
			
			// Statistics screen	
			case screen_view_stats:
				new_view = make_screen_view_stats();
				break;
				
		}
		return new_view; 
	}
}
