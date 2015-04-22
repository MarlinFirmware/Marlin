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
			Screen & press(void (*action)(void));

		private:
	};
}

#endif //SCREEN_LIST_H
