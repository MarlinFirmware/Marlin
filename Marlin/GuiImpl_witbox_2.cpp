#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"

#include "ScreenMenu.h"
#include "ScreenPrint.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenList.h"

namespace screen
{
	///////////////////////
	// Instantiate Icons //
	///////////////////////
	Icon icon_nosd_normal                = Icon(Icon::NORMAL, icon_width, icon_height, bits_nosd_normal);
	Icon icon_nosd_selected              = Icon(Icon::SELECTED, icon_width, icon_height, bits_nosd_selected);
	Icon icon_sd_normal                  = Icon(Icon::NORMAL, icon_width, icon_height, bits_sd_normal);
	Icon icon_sd_selected                = Icon(Icon::SELECTED, icon_width, icon_height, bits_sd_selected);
	Icon icon_filament_unload_normal     = Icon(Icon::NORMAL, icon_width, icon_height, bits_filament_unload_normal);
	Icon icon_filament_unload_selected   = Icon(Icon::SELECTED, icon_width, icon_height, bits_filament_unload_selected);
	Icon icon_filament_load_normal       = Icon(Icon::NORMAL, icon_width, icon_height, bits_filament_load_normal);
	Icon icon_filament_load_selected     = Icon(Icon::SELECTED, icon_width, icon_height, bits_filament_load_selected);
	Icon icon_leveling_normal            = Icon(Icon::NORMAL, icon_width, icon_height, bits_leveling_normal);
	Icon icon_leveling_selected          = Icon(Icon::SELECTED, icon_width, icon_height, bits_leveling_selected);
	Icon icon_homing_normal              = Icon(Icon::NORMAL, icon_width, icon_height, bits_homing_normal);
	Icon icon_homing_selected            = Icon(Icon::SELECTED, icon_width, icon_height, bits_homing_selected);
	Icon icon_steppers_normal            = Icon(Icon::NORMAL, icon_width, icon_height, bits_steppers_normal);
	Icon icon_steppers_selected          = Icon(Icon::SELECTED, icon_width, icon_height, bits_steppers_selected);
	Icon icon_steppers_off_normal        = Icon(Icon::NORMAL, icon_width, icon_height, bits_steppers_off_normal);
	Icon icon_steppers_off_selected      = Icon(Icon::SELECTED, icon_width, icon_height, bits_steppers_off_selected);
	Icon icon_moveaxis_normal            = Icon(Icon::NORMAL, icon_width, icon_height, bits_moveaxis_normal);
	Icon icon_moveaxis_selected          = Icon(Icon::SELECTED, icon_width, icon_height, bits_moveaxis_selected);
	Icon icon_temperature_normal         = Icon(Icon::NORMAL, icon_width, icon_height, bits_temperature_normal);
	Icon icon_temperature_selected       = Icon(Icon::SELECTED, icon_width, icon_height, bits_temperature_selected);
	Icon icon_lightled_disable_normal    = Icon(Icon::NORMAL, icon_width, icon_height, bits_lightled_disable_normal);
	Icon icon_lightled_disable_selected  = Icon(Icon::SELECTED, icon_width, icon_height, bits_lightled_disable_selected);
	Icon icon_lightled_normal            = Icon(Icon::NORMAL, icon_width, icon_height, bits_lightled_normal);
	Icon icon_lightled_selected          = Icon(Icon::SELECTED, icon_width, icon_height, bits_lightled_selected);
	Icon icon_info_normal                = Icon(Icon::NORMAL, icon_width, icon_height, bits_info_normal);
	Icon icon_info_selected              = Icon(Icon::SELECTED, icon_width, icon_height, bits_info_selected);
	Icon icon_play_normal                = Icon(Icon::NORMAL, icon_width, icon_height, bits_play_normal);
	Icon icon_play_selected              = Icon(Icon::SELECTED, icon_width, icon_height, bits_play_selected);
	Icon icon_pause_normal               = Icon(Icon::NORMAL, icon_width, icon_height, bits_pause_normal);
	Icon icon_pause_selected             = Icon(Icon::SELECTED, icon_width, icon_height, bits_pause_selected);
	Icon icon_stop_normal                = Icon(Icon::NORMAL, icon_width, icon_height, bits_stop_normal);
	Icon icon_stop_selected              = Icon(Icon::SELECTED, icon_width, icon_height, bits_stop_selected);
	Icon icon_change_filament_normal     = Icon(Icon::NORMAL, icon_width, icon_height, bits_change_filament_normal);
	Icon icon_change_filament_selected   = Icon(Icon::SELECTED, icon_width, icon_height, bits_change_filament_selected);
	Icon icon_change_speed_normal        = Icon(Icon::NORMAL, icon_width, icon_height, bits_change_speed_normal);
	Icon icon_change_speed_selected      = Icon(Icon::SELECTED, icon_width, icon_height, bits_change_speed_selected);
	Icon icon_back_normal                = Icon(Icon::NORMAL, icon_width, icon_height, bits_back_normal);
	Icon icon_back_selected              = Icon(Icon::SELECTED, icon_width, icon_height, bits_back_selected);
	Icon icon_retry_normal               = Icon(Icon::NORMAL, icon_width, icon_height, bits_retry_normal);
	Icon icon_retry_selected             = Icon(Icon::SELECTED, icon_width, icon_height, bits_retry_selected);
	Icon icon_ok_normal                  = Icon(Icon::NORMAL, icon_width, icon_height, bits_ok_normal);
	Icon icon_ok_selected                = Icon(Icon::SELECTED, icon_width, icon_height, bits_ok_selected);


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
	//Unload Filament screens
	ScreenSelector screen_unload_select = ScreenSelector("Unload filament");
	ScreenDialog screen_unload_heating  = ScreenDialog("Heating");
	ScreenDialog screen_unload_pull     = ScreenDialog("Extrude and pull");
	ScreenMenu screen_unload_confirm    = ScreenMenu("Finished?");
	//Load Filament screens
	ScreenSelector screen_load_select   = ScreenSelector("Load filament");
	ScreenDialog screen_load_heating    = ScreenDialog("Heating");
	ScreenDialog screen_load_pull       = ScreenDialog("Insert and press");
	ScreenMenu screen_load_confirm      = ScreenMenu("Finished?");
	//Level Plate screens
	ScreenMenu screen_level_confirm     = ScreenMenu("Level Plate");
	ScreenDialog screen_level1          = ScreenDialog("Screen1");
	ScreenDialog screen_level2          = ScreenDialog("Screen2");
	ScreenDialog screen_level3          = ScreenDialog("Screen3");
	ScreenDialog screen_level4          = ScreenDialog("Screen4");
	ScreenMenu screen_level_retry       = ScreenMenu("Finished?");
	//AutoHome
	Screen screen_autohome              = Screen("Auto-home");
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
	ScreenPrint screen_print            = ScreenPrint("");
	//Play/Pause
	Screen screen_play_pause			= Screen("Print");
	//Stop
	ScreenMenu screen_stop				= ScreenMenu("Stop");
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
		screen_SD_list.add(screen_SD_confirm);
		screen_SD_list.icon(icon_nosd_normal);
		screen_SD_list.icon(icon_nosd_selected);
		screen_SD_list.icon(icon_sd_normal);
		screen_SD_list.icon(icon_sd_selected);
		//SD Card Confirm
		screen_SD_confirm.add(screen_SD_list);
		screen_SD_confirm.add(screen_print);
		//Unload Filament Select
		screen_unload_select.add(screen_unload_heating);
		screen_unload_select.icon(icon_filament_unload_normal);
		screen_unload_select.icon(icon_filament_unload_selected);
		//Unload Filament Heating
		screen_unload_heating.add(screen_unload_pull);
		//Unload Filament Pull
		screen_unload_pull.add(screen_unload_confirm);
		//Unload Filament Confirm
		screen_unload_confirm.add(screen_unload_pull);
		screen_unload_confirm.add(screen_main);
		//Load Filament Select
		screen_load_select.add(screen_load_heating);
		screen_load_select.icon(icon_filament_load_normal);
		screen_load_select.icon(icon_filament_load_selected);
		//Load Filament Heating
		screen_load_heating.add(screen_load_pull);
		//Load Filament Pull
		screen_load_pull.add(screen_load_confirm);
		//Load Filament Confirm
		screen_load_confirm.add(screen_load_pull);
		screen_load_confirm.add(screen_main);
		//Level Plate
		screen_level_confirm.add(screen_main);
		screen_level_confirm.icon(icon_leveling_normal);
		screen_level_confirm.icon(icon_leveling_selected);
		//AutoHome
		screen_autohome.add(screen_main);
		screen_autohome.icon(icon_homing_normal);
		screen_autohome.icon(icon_homing_selected);
		//Stepper
		screen_stepper.add(screen_main);
		screen_stepper.icon(icon_steppers_normal);
		screen_stepper.icon(icon_steppers_selected);
		screen_stepper.icon(icon_steppers_off_normal);
		screen_stepper.icon(icon_steppers_off_selected);
		//Move Axis
		screen_move.add(screen_main);
		screen_move.icon(icon_moveaxis_normal);
		screen_move.icon(icon_moveaxis_selected);
		//Temperature
		screen_temperature.add(screen_main);
		screen_temperature.icon(icon_temperature_normal);
		screen_temperature.icon(icon_temperature_selected);
		//Light
		screen_light.add(screen_main);
		screen_light.icon(icon_lightled_disable_normal);
		screen_light.icon(icon_lightled_disable_selected);
		screen_light.icon(icon_lightled_normal);
		screen_light.icon(icon_lightled_selected);
		//Info
		screen_info.add(screen_main);
		screen_info.icon(icon_info_normal);
		screen_info.icon(icon_info_selected);

		//Print Menu
		screen_print.add(screen_play_pause);
		screen_print.add(screen_stop);
		screen_print.add(screen_change_confirm);
		screen_print.add(screen_speed);
		screen_print.add(screen_temperature);
		//Play/Pause
		screen_play_pause.add(screen_print);
		screen_play_pause.icon(icon_play_normal); 
		screen_play_pause.icon(icon_play_selected);
		screen_play_pause.icon(icon_pause_normal);
		screen_play_pause.icon(icon_pause_selected);
		//Stop
		screen_stop.add(screen_print);
		screen_stop.add(screen_main);
		screen_stop.icon(icon_stop_normal);
		screen_stop.icon(icon_stop_selected);
		//Change filament
		screen_change_confirm.add(screen_print);
		screen_change_confirm.add(screen_change_start);
		screen_change_confirm.icon(icon_change_filament_normal);
		screen_change_confirm.icon(icon_change_filament_selected);
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
		screen_speed.icon(icon_change_speed_normal);
		screen_speed.icon(icon_change_speed_selected);
		//

		return new ScreenMenu(screen_main);
	}
}
