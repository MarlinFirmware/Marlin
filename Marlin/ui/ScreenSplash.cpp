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
#include "LightManager.h"
#include "PrintManager.h"
#include "GuiManager.h"
#include "GuiAction.h"
#include "StatsManager.h"

namespace ui
{
	ScreenSplash::ScreenSplash(uint32_t timeout)
		: Screen()
		, m_num_item_added(0)
	{
		m_destroy_time = millis() + timeout;

		LightManager::single::instance().state();
	}

	ScreenSplash::~ScreenSplash()
	{ }

	void ScreenSplash::init(uint16_t index)
	{
		lcd_disable_button();
		
		if (eeprom::StorageManager::getEEPROMVersion() != BQ_EEPROM_VERSION || eeprom::StorageManager::checkEEPROMState() == eeprom::EEPROM_DISABLED)
		{
			eeprom::StorageManager::eraseEEPROM();
		}
		
		if(eeprom::StorageManager::checkStatsInitialized() == false)
		{
			SERIAL_ECHOLN("Screen splash initializing stats");
			eeprom::StorageManager::InitilializeStats();
			StatsManager::single::instance().loadStats();
		}
		
		lcd_enable_button();
	}

	void ScreenSplash::draw()
	{
		if (millis() > m_destroy_time)
		{
			if (action_check_auto_gcode())
			{
				ViewManager::getInstance().activeView(ui::screen_print);
			}
			else if (eeprom::StorageManager::getEmergency() == eeprom::EMERGENCY_STOP_ACTIVE)
			{
				ViewManager::getInstance().activeView(m_block_screen);
			}
			else if (!eeprom::StorageManager::single::instance().getInitialized())
			{
				ViewManager::getInstance().activeView(m_alt_screen);
			}
			else
			{
				PrintManager::single::instance().resetInactivity();
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
