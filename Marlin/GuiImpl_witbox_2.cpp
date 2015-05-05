#include "GuiImpl_witbox_2.h"
#include "GuiBitmaps_witbox_2.h"

#include "ScreenMenu.h"
#include "ScreenDialog.h"
#include "ScreenSelector.h"
#include "ScreenList.h"

///////////////////////
// Instantiate Icons //
///////////////////////
screen::Icon icon_sd_normal = screen::Icon(icon_width, icon_heigth, sd_normal_bits);
screen::Icon icon_sd_hover = screen::Icon(icon_width, icon_heigth, sd_hover_bits);

/////////////////////////
// Instantiate Screens //
/////////////////////////
//Logo Splasg screen
screen::ScreenDialog screen_logo = screen::ScreenDialog("BQ Logo");
//Main Menu screen
screen::ScreenMenu screen_main = screen::ScreenMenu("Main Menu");
//SD Card screens
screen::ScreenList screen_SD_list = screen::ScreenList("SD Card");
screen::ScreenMenu screen_SD_confirm = screen::ScreenMenu("Comfirm Print");
//Unload Filament screens
screen::ScreenSelector screen_unload_select = screen::ScreenSelector("Unload filament");
screen::ScreenDialog screen_unload_heating = screen::ScreenDialog("Heating");
screen::ScreenDialog screen_unload_pull = screen::ScreenDialog("Extrude and pull");
screen::ScreenMenu screen_unload_confirm = screen::ScreenMenu("Finished?");
//Load Filament screens
screen::ScreenSelector screen_load_select = screen::ScreenSelector("Load filament");
screen::ScreenDialog screen_load_heating = screen::ScreenDialog("Heating");
screen::ScreenDialog screen_load_pull = screen::ScreenDialog("Insert and press");
screen::ScreenMenu screen_load_confirm = screen::ScreenMenu("Finished?");
//Level Plate screens
screen::ScreenMenu screen_level_confirm = screen::ScreenMenu("Level Plate");
screen::ScreenDialog screen_level1 = screen::ScreenDialog("Screen1");
screen::ScreenDialog screen_level2 = screen::ScreenDialog("Screen2");
screen::ScreenDialog screen_level3 = screen::ScreenDialog("Screen3");
screen::ScreenDialog screen_level4 = screen::ScreenDialog("Screen4");
screen::ScreenMenu screen_level_retry = screen::ScreenMenu("Push to Cotinue");
//AutoHome screen
screen::ScreenMenu screen_autohome = screen::ScreenMenu("Auto-home");
//Stepper screen
screen::ScreenMenu screen_stepper = screen::ScreenMenu("Steper on");
//Move Axis screens
screen::ScreenMenu screen_move = screen::ScreenMenu("Move axis");
screen::ScreenMenu screen_move_back2main = screen::ScreenMenu("Back");
screen::ScreenMenu screen_move_x = screen::ScreenMenu("Move X");
screen::ScreenMenu screen_move_y = screen::ScreenMenu("Move Y");
screen::ScreenMenu screen_move_z = screen::ScreenMenu("Move Z");
screen::ScreenMenu screen_move_e = screen::ScreenMenu("Move Extruder");
screen::ScreenMenu screen_move_back2move = screen::ScreenMenu("Back");
screen::ScreenSelector screen_move_10 = screen::ScreenSelector("Move 10mm");
screen::ScreenSelector screen_move_1 = screen::ScreenSelector("Move 1mm");
screen::ScreenSelector screen_move_01 = screen::ScreenSelector("Move 01mm");
//Temperature screen
screen::ScreenSelector screen_temperature = screen::ScreenSelector("Temp 0/200ºC");
//Light screen
screen::ScreenMenu screen_light = screen::ScreenMenu("Led light on");
//Info screen
screen::ScreenDialog screen_info = screen::ScreenDialog("FW info");

namespace screen
{
	Screen * GuiBuild()
	{
		//Create screens 
		//SD card screens
		screen_main.add(screen_SD_list);
		screen_SD_list.add(screen_SD_confirm);
		screen_SD_confirm.add(screen_SD_list);
		screen_SD_confirm.add(screen_main);
		//Unload filament screens
		screen_main.add(screen_unload_select);
		screen_unload_select.add(screen_unload_heating);
		screen_unload_heating.add(screen_unload_pull);
		screen_unload_pull.add(screen_unload_confirm);
		screen_unload_confirm.add(screen_unload_pull);
		screen_unload_confirm.add(screen_main);
		//Load filament screens
		screen_main.add(screen_load_select);
		screen_load_select.add(screen_load_heating);
		screen_load_heating.add(screen_load_pull);
		screen_load_pull.add(screen_load_confirm);
		screen_load_confirm.add(screen_load_pull);
		screen_load_confirm.add(screen_main);
		//Level screen
		screen_main.add(screen_level_confirm);
		screen_level_confirm.add(screen_main);
		screen_main.add(screen_autohome);
		screen_autohome.add(screen_main);
		screen_main.add(screen_stepper);
		screen_stepper.add(screen_main);
		screen_main.add(screen_move);
		screen_move.add(screen_main);
		screen_main.add(screen_temperature);
		screen_temperature.add(screen_main);
		screen_main.add(screen_light);
		screen_light.add(screen_main);
		screen_main.add(screen_info);
		screen_info.add(screen_main);

		return new ScreenMenu(screen_main);
	}
}
