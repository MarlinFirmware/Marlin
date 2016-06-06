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

#ifndef GUI_IMPL_WITBOX_2_H
#define GUI_IMPL_WITBOX_2_H

#include "GuiBitmaps_witbox_2.h"
#include "dogm_font_data_marlin.h"

namespace ui
{
	typedef enum
	{
		screen_splash = 0,
		screen_wizard_init,
		screen_wizard_language,
		screen_wizard_switch,
		screen_wizard_step1,
		screen_wizard_step2,
		screen_wizard_offset_home,
		screen_wizard_offset_calculate,
		screen_wizard_offset_info,
		screen_wizard_offset_set,
		screen_wizard_offset_rest,
		screen_wizard_offset_finish,
		screen_wizard_offset_save,
		screen_wizard_step3,
		screen_wizard_step4,
		screen_wizard_finish,
		screen_emergency,
		screen_reset_init,
		screen_reset_info,
		screen_reset,
		screen_resetting,
		screen_main,
		screen_SD_list,
		screen_SD_name_error,
		screen_SD_confirm,
		screen_unload_init,
		screen_unload_switch,
		screen_unload_home,
		screen_move_to_unload,
		screen_unload_select,
		screen_unload_heating,
		screen_unload_info,
		screen_unloading,
		screen_unload_confirm,
		screen_unload_rest,
		screen_load_init,
		screen_load_switch,
		screen_load_home,
		screen_move_to_load,
		screen_load_select,
		screen_load_heating,
		screen_load_info,
		screen_loading,
		screen_load_confirm,
		screen_load_rest,
		screen_level_init,
		screen_level_switch_preheat,
		screen_level_preheat,
		screen_level_preheating,
		screen_level_switch,
		screen_level_homing,
		screen_level1,
		screen_level2,
		screen_level3,
		screen_level4,
		screen_level_z_homing,
		screen_level_confirm,
		screen_autohome_init,
		screen_autohome_switch,
		screen_autohome_heating,
		screen_autohome_animation,
		screen_autohome_homing,
		screen_settings,
		screen_move_switch,
		screen_move_info,
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
		screen_move_heat_switch,
		screen_move_heat_confirm,
		screen_move_heat,
		screen_move_heating,
		screen_stepper,
		screen_temperature_main,
		screen_temperature_main_switch,
		screen_cooling_main,
		screen_heating_main,
		screen_info,
		screen_contact,
		screen_settings_language,
		screen_offset,
		screen_offset_switch,
		screen_offset_preheat,
		screen_offset_preheating,
		screen_offset_home,
		screen_offset_calculate,
		screen_offset_info,
		screen_offset_set,
		screen_offset_rest,
		screen_offset_finish,
		screen_offset_save,
		screen_print_switch,
		screen_print_preheat,
		screen_print_preheating,
		screen_print_prepare,
		screen_print,
		screen_print_action_complete,
		screen_print_complete,
		screen_print_maintenance_switch,
		screen_print_maintenance_reminder,
		screen_play_pause,
		screen_stop_confirm,
		screen_stop_OK,
		screen_stop_info,
		screen_change_init,
		screen_change_pausing,
		screen_change_select,
		screen_change_heating,
		screen_change_pause_switch,
		screen_change_wait_pause,
		screen_move_to_change,
		screen_change_pullout_info,
		screen_change_unloading,
		screen_change_insert_info,
		screen_change_loading,
		screen_change_confirm,
		screen_change_ok2print,
		screen_speed,
		screen_temperature_print,
		screen_serial,
		screen_inactivity,
		screen_close_inactivity,
		screen_error_temperature,
		screen_none,
		screen_view_stats,
#ifdef BED_DETECTION
		screen_base_error,
#endif // BED_DETECTION
	} ScreenIndex_t;

	class Screen;
	extern Screen * GuiBuild(ScreenIndex_t const & screen_index);

	// Screen constants
	const static uint8_t screen_width = 128;
	const static uint8_t screen_height = 64;

	const static uint8_t title_width = 128;
	const static uint8_t title_height = 14;

	const static uint8_t box_width = 128;
	const static uint8_t box_height = 9;

	// Menu constants
	const static uint8_t max_items = 10;

	// Font constants
	const static uint8_t max_font_width = 6;
	const static uint8_t max_font_height = 9;

}
#endif //GUI_IMPL_WITBOX_2_H
