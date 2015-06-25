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

		private:
			uint32_t m_destroy_time;
	};
}

#endif // SCREEN_SPLASH_H