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
			Screen & press();
			void add(Screen & component);

		private:
			Screen * m_items[max_items];
			uint16_t m_index;
			uint16_t m_num_items;
	};
}

#endif //SCREEN_MENU_H
