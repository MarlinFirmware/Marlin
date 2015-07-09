///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSplash.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of Splash screen.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or 
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#include "ScreenSplash.h"
#include "OffsetManager.h"
#include "StorageManager.h"

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
			if (eeprom::StorageManager::getEmergencyFlag() != eeprom::EMERGENCY_STOP_INACTIVE)
			{
				ViewManager::getInstance().activeView(m_block_screen);
			}
			else if (!OffsetManager::single::instance().isOffsetOnEEPROM())
			{
				ViewManager::getInstance().activeView(m_alt_screen);
			}
			else
			{
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
		switch (m_num_item_added % 3)
		{
			case 0:
				m_next_screen = component;
				break;

			case 1:
				m_alt_screen = component;
				break;

			case 2:
				m_block_screen = component;
				break;
		}
		m_num_item_added++;
	}
}
