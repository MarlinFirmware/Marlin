#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include "Screen.h"

namespace screen
{
	class ScreenSelector : public Screen
	{
		public:
			ScreenSelector(const char * label);
			virtual ~ScreenSelector();

			void left();
			void right();
			void draw();
			Screen & press(void (*action)(void));

		private:
	};
}

#endif //SCREEN_SELECTOR_H
