#ifndef SCREEN_DIALOG_H
#define SCREEN_DIALOG_H

#include "Screen.h"

namespace screen
{
	class ScreenDialog : public Screen
	{
		public:
			ScreenDialog(const char * title, const char * box);
			virtual ~ScreenDialog();

			void left();
			void right();
			void draw();

		private:
			const char * m_box;
	};
}

#endif //SCREEN_DIALOG_H
