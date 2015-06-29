#ifndef SCREEN_SPLASH_H
#define SCREEN_SPLASH_H

#include "Screen.h"

namespace screen
{
	class ScreenSplash : public Screen
	{
		public:
			ScreenSplash(uint32_t timeout);
			~ScreenSplash();

			void draw();
			void add(ScreenIndex_t const & component);

		private:
			uint32_t m_destroy_time;

			ScreenIndex_t m_alt_screen;
			uint8_t m_num_item_added;
	};
}

#endif // SCREEN_SPLASH_H