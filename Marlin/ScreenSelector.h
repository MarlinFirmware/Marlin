#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include "Screen.h"

namespace screen
{
	class ScreenSelector : public Screen
	{
		public:
			ScreenSelector(const char * label);
			~ScreenSelector();

			void left();
			void right();
			void draw();
			Screen & press();
         void add(Screen & component);

		private:
         Screen * m_next_screen;
	};
}

#endif //SCREEN_SELECTOR_H
