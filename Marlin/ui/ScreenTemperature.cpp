///////////////////////////////////////////////////////////////////////////////
/// \file ScreenTemperature.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of screen temperature.
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

#include "ScreenTemperature.h"

#include "TemperatureManager.h"
#include "Language.h"

namespace ui
{	
	ScreenTemperature::ScreenTemperature(const char * title, const char * box, uint16_t min, uint16_t max, uint16_t scale, uint16_t dflt, Functor<void, uint16_t>::FuncPtr fptr)
		: ScreenSelector<void, uint16_t>(title, box, min, max, scale, dflt, fptr)
	{
		m_minimum_value = min - scale;
	}

	ScreenTemperature::~ScreenTemperature()
	{ }

	void ScreenTemperature::draw()
	{
		if (m_needs_drawing)
		{
			m_needs_drawing = false;

			painter.firstPage();
			do
			{
				painter.title(m_title);
				painter.box(m_box);

				uint8_t x_init = painter.coordinateXInit();
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_init = painter.coordinateYInit();
				uint8_t y_end = painter.coordinateYEnd();

				painter.setColorIndex(1);
				painter.setFont(FontType_t::BODY_FONT);

				char tmp_selected[7] = { 0 };
				int number_size = 3;
				if(m_select < 100)
				{
					number_size = 2;
				}
				else if(m_select > 10)
				{
					number_size = 1;
				}

				dtostrf(m_select, number_size, 0, tmp_selected);
				char info[20] = { 0 };
				char units[] = "\xb0";

				if (m_select != m_minimum_value && m_select != m_maximum_value)
				{
					strcat(info, "< ");
					strcat(info, tmp_selected);
					strcat(info, units);
					strcat(info, " >");
				}
				else if (m_select != m_minimum_value)
				{
					strcat(info, "< ");
					strcat(info, tmp_selected);
					strcat(info, units);
				}
				else if (m_select != m_maximum_value)
				{
					strcat_P(info, MSG_TEMP_OFF());
					strcat(info, " >");
				}

				Area info_area(0, ((y_end + y_init)/2 - 9/2), 128, 9);
				painter.setWorkingArea(info_area);
				painter.text(info);

			} while( painter.nextPage() );
		}
	}

	void ScreenTemperature::press()
	{
		if(m_select == m_minimum_value)
		{
			this->action(0);
		}
		else
		{
			this->action(m_select);
		}

		ViewManager::getInstance().activeView(m_next_screen);
	}
}
