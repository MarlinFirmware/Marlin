#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include <stdint.h>

#include "Screen.h"

namespace screen
{
	class ScreenMenu : public Screen
	{
		public:
			ScreenMenu(const char * title = 0, const char * text = 0);
			virtual ~ScreenMenu();

			void left();
			void right();
			virtual void draw();
			void press();
			void add(Screen & component);

		protected:
			uint16_t m_index;
			uint16_t m_num_items;
			Screen * m_items[max_items];
			const char * m_text;
	};
}

#endif //SCREEN_MENU_H
