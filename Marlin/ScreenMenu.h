#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include <stdint.h>

#include "Screen.h"

namespace screen
{
	class ScreenMenu : public Screen
	{
		public:
			ScreenMenu(const char * title = 0);
			virtual ~ScreenMenu();

			void left();
			void right();
			void draw();
			Screen & press(Screen & parent_view);
			void add(Screen & component);

		private:
			uint8_t m_icon_line;
			uint16_t m_index;
			uint16_t m_num_items;
			Screen * m_items[max_items];
	};
}

#endif //SCREEN_MENU_H
