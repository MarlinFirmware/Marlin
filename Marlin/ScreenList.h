#ifndef SCREEN_LIST_H
#define SCREEN_LIST_H

#include "Screen.h"

namespace screen
{
	class ScreenList : public Screen
	{
		public:
			ScreenList(const char * label);
			virtual ~ScreenList();

			void left();
			void right();
			void draw();
			Screen & press()
         void add(Screen & component);

		private:
         Screen * m_next_screen;
	};
}

#endif //SCREEN_LIST_H
