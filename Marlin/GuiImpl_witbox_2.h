#ifndef GUI_IMPL_WITBOX_2_H
#define GUI_IMPL_WITBOX_2_H

#include "GuiBitmaps_witbox_2.h"
#include "dogm_font_data_marlin.h"

namespace screen
{
	class Screen;
	extern Screen * GuiBuild();

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

