#ifndef GUI_IMPL_WITBOX_2_H
#define GUI_IMPL_WITBOX_2_H

#include "GuiBitmaps_witbox_2.h"
#include "dogm_font_data_marlin.h"

namespace screen
{
	typedef enum
	{
		screen_splash = 0,
		screen_main,
		screen_SD_list,
		screen_SD_confirm,
		screen_SD_OK,
		screen_SD_back,
		screen_unload_init,
		screen_unload_select,
		screen_unload_heating,
		screen_unload_info,
		screen_unloading,
		screen_unload_confirm,
		screen_load_init,
		screen_load_select,
		screen_load_heating,
		screen_load_info,
		screen_loading,
		screen_load_confirm,
		screen_level_init,
		screen_level_cooling,
		screen_level_animation,
		screen_level_homing,
		screen_level1,
		screen_level2,
		screen_level3,
		screen_level4,
		screen_level5,
		screen_level_confirm,
		screen_autohome,
		screen_settings,
		screen_move,
		screen_move_x,
		screen_move_y,
		screen_move_z,
		screen_move_e,
		screen_move_x_01,
		screen_move_y_01,
		screen_move_z_01,
		screen_move_e_01,
		screen_move_x_1,
		screen_move_y_1,
		screen_move_z_1,
		screen_move_e_1,
		screen_move_x_10,
		screen_move_y_10,
		screen_move_z_10,
		screen_move_heat_confirm,
		screen_move_heat,
		screen_move_heating,
		screen_stepper,
		screen_temperature_main,
		screen_autolevel,
		screen_light,
		screen_info,
		screen_offset,
		screen_offset_home,
		screen_offset_calculate,
		screen_offset_info,
		screen_offset_set,
		screen_offset_finish,
		screen_offset_save,
		screen_print,
		screen_print_complete,
		screen_play_pause,
		screen_stop_confirm,
		screen_stop_OK,
		screen_change_confirm_first,
		screen_change_pausing,
		screen_change_selector,
		screen_change_animation,
		screen_change_info,
		screen_change_pullout_info,
		screen_change_unloading,
		screen_change_insert_info,
		screen_change_loading,
		screen_change_confirm_second,
		screen_change_ok2print,
		screen_speed,
		screen_temperature_print,
	} ScreenIndex_t;

	class Screen;
	extern Screen * GuiBuild(ScreenIndex_t const & screen_index);

	// Screen constants
	const static uint8_t screen_width = 128;
	const static uint8_t screen_height = 64;
	const static uint8_t max_items = 10;
	const static uint8_t max_list = 100;

	// Font constants
	const static uint8_t max_font_height = 9;

	// Temperature parameters
	const static uint8_t default_temp_change_filament = 210;
}

#endif //GUI_IMPL_WITBOX_2_H

