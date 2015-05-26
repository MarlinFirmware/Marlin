#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include "Screen.h"

namespace screen
{
	class ScreenSelector : public Screen
	{
		public:
			ScreenSelector(const char * title, uint16_t min, uint16_t max, uint16_t dflt);
			virtual ~ScreenSelector();

			void left();
			void right();
			void draw();
			Screen & press(Screen * parent_view);
         void add(Screen & component);

		private:
         uint16_t m_select;
         uint16_t m_minimum_value;
         uint16_t m_maximum_value;
         Screen * m_next_screen;
	};
}

#endif //SCREEN_SELECTOR_H
