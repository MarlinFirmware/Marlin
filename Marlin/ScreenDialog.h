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
	};
}

#endif //SCREEN_DIALOG_H
