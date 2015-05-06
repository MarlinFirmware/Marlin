#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"

#include "ScreenMenu.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenList.h"

namespace screen
{
	///////////////////////
	// Instantiate Icons //
	///////////////////////
	Icon icon_nosd_normal             = Icon(icon_width, icon_heigth, bits_nosd_normal);
	Icon icon_nosd_hover              = Icon(icon_width, icon_heigth, bits_nosd_hover);
	Icon icon_sd_normal               = Icon(icon_width, icon_heigth, bits_sd_normal);
	Icon icon_sd_hover                = Icon(icon_width, icon_heigth, bits_sd_hover);
	Icon icon_filament_unload_normal  = Icon(icon_width, icon_heigth, bits_filament_unload_normal);
	Icon icon_filament_unload_hover   = Icon(icon_width, icon_heigth, bits_filament_unload_hover);
	Icon icon_filament_load_normal    = Icon(icon_width, icon_heigth, bits_filament_load_normal);
	Icon icon_filament_load_hover     = Icon(icon_width, icon_heigth, bits_filament_load_hover);
	Icon icon_leveling_normal         = Icon(icon_width, icon_heigth, bits_leveling_normal);
	Icon icon_leveling_hover          = Icon(icon_width, icon_heigth, bits_leveling_hover);
	Icon icon_homing_normal           = Icon(icon_width, icon_heigth, bits_homing_normal);
	Icon icon_homing_hover            = Icon(icon_width, icon_heigth, bits_homing_hover);
	Icon icon_steppers_normal         = Icon(icon_width, icon_heigth, bits_steppers_normal);
	Icon icon_steppers_hover          = Icon(icon_width, icon_heigth, bits_steppers_hover);
	Icon icon_steppers_off_normal     = Icon(icon_width, icon_heigth, bits_steppers_off_normal);
	Icon icon_steppers_off_hover      = Icon(icon_width, icon_heigth, bits_steppers_off_hover);
	Icon icon_moveaxis_normal         = Icon(icon_width, icon_heigth, bits_moveaxis_normal);
	Icon icon_moveaxis_hover          = Icon(icon_width, icon_heigth, bits_moveaxis_hover);
	Icon icon_heat_normal             = Icon(icon_width, icon_heigth, bits_heat_normal);
	Icon icon_heat_hover              = Icon(icon_width, icon_heigth, bits_heat_hover);
	Icon icon_cold_normal             = Icon(icon_width, icon_heigth, bits_cold_normal);
	Icon icon_cold_hover              = Icon(icon_width, icon_heigth, bits_cold_hover);
	Icon icon_lightled_disable_normal = Icon(icon_width, icon_heigth, bits_lightled_disable_normal);
	Icon icon_lightled_disable_hover  = Icon(icon_width, icon_heigth, bits_lightled_disable_hover);
	Icon icon_lightled_normal         = Icon(icon_width, icon_heigth, bits_lightled_normal);
	Icon icon_lightled_hover          = Icon(icon_width, icon_heigth, bits_lightled_hover);
	Icon icon_info_normal             = Icon(icon_width, icon_heigth, bits_info_normal);
	Icon icon_info_hover              = Icon(icon_width, icon_heigth, bits_info_hover);

	/////////////////////////
	// Instantiate Screens //
	/////////////////////////
	//Logo Splash
	ScreenDialog screen_logo            = ScreenDialog("BQ Logo");
	//Main Menu
	ScreenMenu screen_main              = ScreenMenu("Main Menu");
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
	ScreenMenu screen_level_retry       = ScreenMenu("Push to Cotinue");
	//AutoHome
	ScreenMenu screen_autohome          = ScreenMenu("Auto-home");
	//Stepper
	ScreenMenu screen_stepper           = ScreenMenu("Steper on");
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
	ScreenSelector screen_temperature   = ScreenSelector("Temp 0/200ºC");
	//Light
	ScreenMenu screen_light             = ScreenMenu("Led light on");
	//Info
	ScreenDialog screen_info            = ScreenDialog("FW info");

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
		screen_SD_list.icon(icon_nosd_hover);
		screen_SD_list.icon(icon_sd_normal);
		screen_SD_list.icon(icon_sd_hover);
		//SD Card Confirm
		screen_SD_confirm.add(screen_SD_list);
		screen_SD_confirm.add(screen_main);
		//Unload Filament Select
		screen_unload_select.add(screen_unload_heating);
		screen_unload_select.icon(icon_filament_unload_normal);
		screen_unload_select.icon(icon_filament_unload_hover);
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
		screen_load_select.icon(icon_filament_load_hover);
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
		screen_level_confirm.icon(icon_leveling_hover);
		//AutoHome
		screen_autohome.add(screen_main);
		screen_autohome.icon(icon_homing_normal);
		screen_autohome.icon(icon_homing_hover);
		//Stepper
		screen_stepper.add(screen_main);
		screen_stepper.icon(icon_steppers_normal);
		screen_stepper.icon(icon_steppers_hover);
		screen_stepper.icon(icon_steppers_off_normal);
		screen_stepper.icon(icon_steppers_off_hover);
		//Move Axis
		screen_move.add(screen_main);
		screen_move.icon(icon_moveaxis_normal);
		screen_move.icon(icon_moveaxis_hover);
		//Temperature
		screen_temperature.add(screen_main);
		screen_temperature.icon(icon_heat_normal);
		screen_temperature.icon(icon_heat_hover);
		//Light
		screen_light.add(screen_main);
		screen_light.icon(icon_lightled_disable_normal);
		screen_light.icon(icon_lightled_disable_hover);
		screen_light.icon(icon_lightled_normal);
		screen_light.icon(icon_lightled_hover);
		//Info
		screen_info.add(screen_main);
		screen_info.icon(icon_info_normal);
		screen_info.icon(icon_info_hover);

		return new ScreenMenu(screen_main);
	}
}
