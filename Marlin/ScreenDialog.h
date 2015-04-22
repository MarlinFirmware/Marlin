#ifndef SCREEN_DIALOG_H
#define SCREEN_DIALOG_H

#include "Screen.h"

namespace screen
{
	class ScreenDialog : public Screen
	{
		public:
			ScreenDialog(const char * label);
			virtual ~ScreenDialog();

			void left();
			void right();
			void draw();
			Screen & press(void (*action)(void));
			void add(Screen & component);

		private:
			Screen * m_back_screen;
	};
}

#endif //SCREEN_DIALOG_H
