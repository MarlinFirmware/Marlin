#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include "Screen.h"

namespace screen
{

	class ScreenTransition : public Screen
	{
		public:
			ScreenTransition(const char * title, const char * text);
			virtual ~ScreenTransition();

			void draw();
			Screen & press(Screen * parent_view);

			void add(Screen & component);

		private:
			const char * m_text;

			Screen * m_back_screen;
			Screen * m_next_screen;
			uint8_t m_num_item_added;
	};
}


#endif //SCREEN_TRANSITION_H