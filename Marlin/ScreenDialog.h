#ifndef SCREEN_DIALOG_H
#define SCREEN_DIALOG_H

#include "Screen.h"

namespace screen
{
	class ScreenDialog : public Screen
	{
		public:
			ScreenDialog(const char * title);
			virtual ~ScreenDialog();

			void left();
			void right();
			void draw();
			Screen & press(Screen * parent_view);
			void add(Screen & component);

		private:
			Screen * m_next_screen;
	};
}

#endif //SCREEN_DIALOG_H
