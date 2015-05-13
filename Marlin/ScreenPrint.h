#ifndef SCREEN_PRINT_H
#define SCREEN_PRINT_H

#include <stdint.h>

#include "Screen.h"

namespace screen
{
	class ScreenPrint : public Screen
	{
		public:
			ScreenPrint(const char * title = 0);
			virtual ~ScreenPrint();

			void left();
			void right();
			void draw();
			Screen & press(Screen * parent_view);
			void add(Screen & component);

		private:
			uint16_t m_index;
			uint16_t m_num_items;
			Screen * m_items[max_items];
	};
}

#endif //SCREEN_PRINT_H