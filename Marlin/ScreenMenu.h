#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include <stdint.h>

#include "Screen.h"

namespace screen
{
	class ScreenMenu : public Screen
	{
		public:
			ScreenMenu(const char * label = 0);
			virtual ~ScreenMenu();

			void left();
			void right();
			void draw();
			Screen & press(void (*action)(void));
			void add(Screen & component);

		private:
			Screen * m_items[16];
			uint16_t m_index;
			uint16_t m_num_items;
	};
}

#endif //SCREEN_MENU_H
