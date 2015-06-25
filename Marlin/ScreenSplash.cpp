#include "ScreenSplash.h"

namespace screen
{
	ScreenSplash::ScreenSplash(uint32_t timeout)
		: Screen()
	{
		m_destroy_time = millis() + timeout;
	}

	ScreenSplash::~ScreenSplash()
	{ }

	void ScreenSplash::draw()
	{
		if (millis() > m_destroy_time)
		{
			ViewManager::getInstance().activeView(m_next_screen);
			return;
		}

		painter.firstPage();
		do 
		{
			painter.drawBitmap(34,10,splash_width,splash_height,bits_logo_splash);
		} while(painter.nextPage());

	}
}