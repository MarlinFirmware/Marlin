#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"
#include "GuiAction.h"

#include "Screen.h"
#include "ScreenMenu.h"
#include "ScreenPrint.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenList.h"



namespace screen
{
	Size icon_size = Size(icon_width, icon_height);
	///////////////////////
	// Instantiate Icons //
	///////////////////////
	Icon icon_nosd                = Icon(icon_size, bits_nosd_normal, bits_nosd_focused);
	Icon icon_sd                  = Icon(icon_size, bits_sd_normal, bits_sd_focused);
	Icon icon_filament_unload     = Icon(icon_size, bits_filament_unload_normal, bits_filament_unload_focused);
	Icon icon_filament_load       = Icon(icon_size, bits_filament_load_normal, bits_filament_load_focused);
	Icon icon_leveling            = Icon(icon_size, bits_leveling_normal, bits_leveling_focused);
	Icon icon_homing              = Icon(icon_size, bits_homing_normal, bits_homing_focused);
	Icon icon_steppers            = Icon(icon_size, bits_steppers_normal, bits_steppers_focused);
	Icon icon_steppers_off        = Icon(icon_size, bits_steppers_off_normal, bits_steppers_off_focused);
	Icon icon_moveaxis            = Icon(icon_size, bits_moveaxis_normal, bits_moveaxis_focused);
	Icon icon_temperature         = Icon(icon_size, bits_temperature_normal, bits_temperature_focused);
	Icon icon_lightled_disable    = Icon(icon_size, bits_lightled_disable_normal, bits_lightled_disable_focused);
	Icon icon_lightled            = Icon(icon_size, bits_lightled_normal, bits_lightled_focused);
	Icon icon_info                = Icon(icon_size, bits_info_normal, bits_info_focused);
	Icon icon_play                = Icon(icon_size, bits_play_normal, bits_play_focused);
	Icon icon_pause               = Icon(icon_size, bits_pause_normal, bits_pause_focused);
	Icon icon_stop                = Icon(icon_size, bits_stop_normal, bits_stop_focused);
	Icon icon_change_filament     = Icon(icon_size, bits_change_filament_normal, bits_change_filament_focused);
	Icon icon_change_speed        = Icon(icon_size, bits_change_speed_normal, bits_change_speed_focused);
	Icon icon_back                = Icon(icon_size, bits_back_normal, bits_back_focused);
	Icon icon_retry               = Icon(icon_size, bits_retry_normal, bits_retry_focused);
	Icon icon_ok                  = Icon(icon_size, bits_ok_normal, bits_ok_focused);


	/////////////////////////
	// Instantiate Screens //
	/////////////////////////
	//Logo Splash
	ScreenDialog screen_logo            = ScreenDialog("BQ Logo");
	//Main Menu
	ScreenMenu screen_main              = ScreenMenu("");
	//SD Card screens
	ScreenList screen_SD_list           = ScreenList("SD Card");
	ScreenMenu screen_SD_confirm        = ScreenMenu("Comfirm Print");
	//Screen screen_SD_OK                 = Screen("Confirm", Screen::ACTION,  action_print);
	ScreenList screen_SD_back           = ScreenList("Back");
	//Unload Filament screens
	ScreenSelector screen_unload_select = ScreenSelector("Unload filament");
	ScreenDialog screen_unload_heating  = ScreenDialog("Heating");
	ScreenDialog screen_unload_pull     = ScreenDialog("Extrude and pull");
	ScreenMenu screen_unload_confirm    = ScreenMenu("Finished?");
	//Load Filament screens
	ScreenSelector screen_load_select   = ScreenSelector("Load filament");
	ScreenDialog screen_load_heating    = ScreenDialog("Heating");
	ScreenDialog screen_load_press      = ScreenDialog("Insert and press");
	ScreenMenu screen_load_confirm      = ScreenMenu("Finished?");
	//Level Plate screens
	ScreenMenu screen_level_confirm     = ScreenMenu("Level Plate");
	ScreenDialog screen_level1          = ScreenDialog("Screen1");
	ScreenDialog screen_level2          = ScreenDialog("Screen2");
	ScreenDialog screen_level3          = ScreenDialog("Screen3");
	ScreenDialog screen_level4          = ScreenDialog("Screen4");
	ScreenMenu screen_level_retry       = ScreenMenu("Finished?");
	//AutoHome
	Screen screen_autohome              = Screen("Auto-home", Screen::SIMPLE, action_homing);
	//Stepper
	Screen screen_stepper               = Screen("Steper on");
	//Move Axis screens
	ScreenMenu screen_move              = ScreenMenu("Move axis");
	ScreenMenu screen_move_back2main    = ScreenMenu("Back");
	ScreenMenu screen_move_x            = ScreenMenu("Move X");
	ScreenMenu screen_move_y            = ScreenMenu("Move Y");
	ScreenMenu screen_move_z            = ScreenMenu("Move Z");
	ScreenMenu screen_move_e            = ScreenMenu("Move Extruder");
	ScreenMenu screen_move_back2move    = ScreenMenu("Back");
	ScreenSelector screen_move_10       = ScreenSelector("Move 10mm");
	ScreenSelector screen_move_1        = ScreenSelector("Move 1mm");
	ScreenSelector screen_move_01       = ScreenSelector("Move 01mm");
	//Temperature
	ScreenSelector screen_temperature   = ScreenSelector("Temp 0/200C");
	//Light
	Screen screen_light                 = Screen("Led light on");
	//Info
	ScreenDialog screen_info            = ScreenDialog("FW info");

	//Print screen
	ScreenPrint screen_print            = ScreenPrint("Confirm", Screen::ACTION, action_print);
	//Play/Pause
	Screen screen_play_pause            = Screen("Pause");
	//Stop
	ScreenMenu screen_stop              = ScreenMenu("Stop", action_stop_print);
	//Change Filament Screens
	ScreenMenu screen_change_confirm	= ScreenMenu("Change filament");
	ScreenDialog screen_change_start    = ScreenDialog("Start");
	ScreenDialog screen_change_pullout  = ScreenDialog("Pull out");
	ScreenDialog screen_change_insert   = ScreenDialog("Insert & press");
	ScreenMenu screen_change_retry      = ScreenMenu("Finished?");
	//Change Speed screen
	ScreenSelector screen_speed			= ScreenSelector("Change speed");			


	Screen * GuiBuild()
	{
		// Build the UI
		//Main Menu
		screen_main.add(screen_SD_list);
		screen_main.add(screen_unload_select);
		screen_main.add(screen_load_select);
		screen_main.add(screen_level_confirm);
		screen_main.add(screen_autohome);
		screen_main.add(screen_stepper);
		screen_main.add(screen_move);
		screen_main.add(screen_temperature);
		screen_main.add(screen_light);
		screen_main.add(screen_info);
		//SD Card List
		screen_SD_list.add(screen_main);
		screen_SD_list.add(screen_SD_confirm);
		screen_SD_list.icon(icon_nosd);
		screen_SD_list.icon(icon_sd);
		//SD Card Confirm
		screen_SD_confirm.add(screen_SD_back);
		screen_SD_confirm.add(screen_print);
		//SD Confirm back
		screen_SD_back.add(screen_SD_list);
		screen_SD_back.icon(icon_back);
		//SD Confirm OK
		screen_print.icon(icon_ok);
		//Unload Filament Select
		screen_unload_select.add(screen_unload_heating);
		screen_unload_select.icon(icon_filament_unload);
		//Unload Filament Heating
		screen_unload_heating.add(screen_unload_pull);
		//Unload Filament Pull
		screen_unload_pull.add(screen_unload_confirm);
		//Unload Filament Confirm
		screen_unload_confirm.add(screen_unload_pull);
		screen_unload_confirm.add(screen_main);
		//Load Filament Select
		screen_load_select.add(screen_load_heating);
		screen_load_select.icon(icon_filament_load);
		//Load Filament Heating
		screen_load_heating.add(screen_load_press);
		//Load Filament Pull
		screen_load_press.add(screen_load_confirm);
		//Load Filament Confirm
		screen_load_confirm.add(screen_load_press);
		screen_load_confirm.add(screen_main);
		//Level Plate
		screen_level_confirm.add(screen_main);
		screen_level_confirm.icon(icon_leveling);
		//AutoHome
		screen_autohome.add(screen_main);
		screen_autohome.icon(icon_homing);
		//Stepper
		screen_stepper.add(screen_main);
		screen_stepper.icon(icon_steppers);
		screen_stepper.icon(icon_steppers_off);
		//Move Axis
		screen_move.add(screen_main);
		screen_move.icon(icon_moveaxis);
		//Temperature
		screen_temperature.add(screen_main);
		screen_temperature.icon(icon_temperature);
		//Light
		screen_light.add(screen_main);
		screen_light.icon(icon_lightled_disable);
		screen_light.icon(icon_lightled);
		//Info
		screen_info.add(screen_main);
		screen_info.icon(icon_info);

		//Print Menu
		screen_print.add(screen_play_pause);
		screen_print.add(screen_stop);
		screen_print.add(screen_change_confirm);
		screen_print.add(screen_speed);
		screen_print.add(screen_temperature);
		//Play/Pause
		screen_play_pause.add(screen_print);
		screen_play_pause.icon(icon_pause);
		screen_play_pause.icon(icon_play); 
		//Stop
		screen_stop.add(screen_print);
		screen_stop.add(screen_main);
		screen_stop.icon(icon_stop);
		//Change filament
		screen_change_confirm.add(screen_print);
		screen_change_confirm.add(screen_change_start);
		screen_change_confirm.icon(icon_change_filament);
		//Change filament start
		screen_change_start.add(screen_change_pullout);
		//Change filament pullout
		screen_change_pullout.add(screen_change_insert);
		//Change filament insert
		screen_change_insert.add(screen_change_retry);
		//Change filament retry
		screen_change_retry.add(screen_change_start);
		screen_change_retry.add(screen_print);
		//Change speed screens
		screen_speed.add(screen_print);
		screen_speed.icon(icon_change_speed);
		//

		return new ScreenMenu(screen_main);
	}
}
