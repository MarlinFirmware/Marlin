///////////////////////////////////////////////////////////////////////////////
/// \file ScreenLanguage.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of language screen.
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

#include "ScreenLanguage.h"
#include "StorageManager.h"

namespace ui
{
	ScreenLanguage::ScreenLanguage(const char * title, Language dflt)
		: Screen(title, SELECTOR)
		, m_default(dflt)
		, m_language((uint8_t)dflt)
	{ }

	ScreenLanguage::~ScreenLanguage()
	{ }

	void ScreenLanguage::init(uint16_t index)
	{
		m_language = eeprom::StorageManager::getLanguage();

		if(m_language != 0xFF)
		{
			LANG = static_cast<Language>(m_language);
		}
		else
		{
			LANG = m_default;
			m_language = (uint8_t)m_default;
		}
	}

	void ScreenLanguage::left()
	{
		if (m_language > m_min)
		{
			--m_language;
			LANG = static_cast<Language>(m_language);
			m_needs_drawing = true;
		}
	}

	void ScreenLanguage::right()
	{
		if (m_language < m_max)
		{
			++m_language;
			LANG = static_cast<Language>(m_language);
			m_needs_drawing = true;
		}
	}

	void ScreenLanguage::draw()
	{
		if (m_needs_drawing)
		{
			m_needs_drawing = false;

			painter.firstPage();
			do
			{
				painter.title(MSG_SCREEN_WIZARD_SELECT());
				painter.box(MSG_PUSH_TO_CONFIRM());

				uint8_t x_init = painter.coordinateXInit();
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_init = painter.coordinateYInit();
				uint8_t y_end = painter.coordinateYEnd();

				painter.setColorIndex(1);
				painter.setFont(FontType_t::BODY_FONT);

				char info[21] = { 0 };
				char value[17] = { 0 };

				switch(m_language)
				{
					case 0:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 1:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 2:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 3:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 4:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 5:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 6:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 7:
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
				}
				if (m_language != 0 && m_language != m_max)
				{
					strcat(info, "< ");
					strcat(info, value);
					strcat(info, " >");
				}
				else if (m_language != 0)
				{
					strcat(info, "< ");
					strcat(info, value);
				}
				else if (m_language != m_max)
				{
					strcat(info, value);
					strcat(info, " >");
				}
				
				Area info_area(0, ((y_end + y_init)/2 - 9/2), 128, 9);
				painter.setWorkingArea(info_area);
				painter.text(info);

			} while( painter.nextPage() );
		}
	}

	void ScreenLanguage::press()
	{
		eeprom::StorageManager::setLanguage(m_language);
		
		ViewManager::getInstance().activeView(m_next_screen);
	}
}
