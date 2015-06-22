#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include <stdint.h>

#include "Screen.h"
#include "Icon.h"
#include "GuiImpl_witbox_2.h"

namespace screen
{
	class ScreenMenu : public Screen
	{
		public:
			ScreenMenu(const char * title = 0, const char * text = 0);
			virtual ~ScreenMenu();

			void init();

			void left();
			void right();
			virtual void draw();
			void press();
			void add(ScreenIndex_t const & component);
			void icon(Icon & component);

		protected:
			uint16_t m_index;
			uint16_t m_num_items;
			uint16_t m_num_icons;
			ScreenIndex_t m_items[max_items];
			Icon * m_icons[max_items];
			const char * m_text;
	};
}

#endif //SCREEN_MENU_H
