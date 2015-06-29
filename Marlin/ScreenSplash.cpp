#include "ScreenSplash.h"

#include "OffsetManager.h"

namespace screen
{
	ScreenSplash::ScreenSplash(uint32_t timeout)
		: Screen()
		, m_num_item_added(0)
	{
		m_destroy_time = millis() + timeout;
	}

	ScreenSplash::~ScreenSplash()
	{ }

	void ScreenSplash::draw()
	{
		if (millis() > m_destroy_time)
		{
			if(!OffsetManager::single::instance().isOffsetOnEEPROM())
			{
				SERIAL_ECHOLN("OFFSET NOT SET");
				ViewManager::getInstance().activeView(m_alt_screen);
			}			
			else
			{
				SERIAL_ECHOLN("OFFSET PREVIOUSLY SET");
				ViewManager::getInstance().activeView(m_next_screen);
			}	
			return;
		}

		painter.firstPage();
		do 
		{
			painter.drawBitmap(34,10,splash_width,splash_height,bits_logo_splash);
		} while(painter.nextPage());

	}

	void ScreenSplash::add(ScreenIndex_t const & component)
	{
		if (m_num_item_added % 2)
		{
			m_alt_screen = component;
		}
		else
		{
			m_next_screen = component;
		}
		m_num_item_added++;
	}
}