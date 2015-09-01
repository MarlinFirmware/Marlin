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

namespace screen
{
	ScreenLanguage::ScreenLanguage(const char * title, Language dflt)
		: Screen(title, SELECTOR)
		, m_default(dflt)
		, m_language(0)
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
		if (m_language != m_min)
		{
			m_language--;
			m_needs_drawing = true;
		}

		if (m_language < m_min)
		{
			m_language = m_min;
		}
	}

	void ScreenLanguage::right()
	{
		if (m_language != m_max)
		{
			m_language++;
			m_needs_drawing = true;
		}

		if (m_language > m_max)
		{
			m_language = m_max;
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
				painter.title(m_title);
				painter.box(MSG_PUSH_TO_CONFIRM());

				uint8_t x_init = painter.coordinateXInit();
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_init = painter.coordinateYInit();
				uint8_t y_end = painter.coordinateYEnd();

				painter.setColorIndex(1);
				painter.setFont(u8g_font_6x9);

				char info[21] = { 0 };
				char value[17] = { 0 };

				switch(m_language)
				{
					case 0:
						LANG = Language::EN;
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 1:
						LANG = Language::ES;
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 2:
						LANG = Language::DE;
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 3:
						LANG = Language::FR;
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 4:
						LANG = Language::PT;
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 5:
						LANG = Language::IT;
						strcpy_P(value, MSG_SCREEN_WIZARD_LANGUAGE());
						break;
					case 6:
						LANG = Language::PL;
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
				
				Area info_area(0, ((y_end + y_init)/2 - 9/2 - 2), 128, 9);
				painter.setWorkingArea(info_area);
				painter.text_P(MSG_SCREEN_WIZARD_SELECT());

				Area select_area(0, ((y_end + y_init)/2 + 9/2 + 2), 128, 9);
				painter.setWorkingArea(select_area);
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
