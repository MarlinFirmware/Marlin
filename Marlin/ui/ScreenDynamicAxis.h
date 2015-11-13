///////////////////////////////////////////////////////////////////////////////
/// \file ScreenDynamicAxis.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of dynamic-type screens to change the axis positions.
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

#ifndef SCREEN_DYNAMIC_AXIS_H
#define SCREEN_DYNAMIC_AXIS_H

#include "Language.h"

namespace ui
{
	template <typename T>
		class ScreenDynamicAxis : public Screen , public Functor<void, uint8_t, T>
	{
		public:
			ScreenDynamicAxis(const char * title, uint8_t axis, T min, T max, T scale, typename Functor<void, uint8_t, T>::FuncPtr fptr, bool reset = false);
			virtual ~ScreenDynamicAxis();

			void init(uint16_t index = 0);

			void left();
			void right();
			void draw();
			void press();

		private:
			uint8_t m_axis;
			T m_select;
			T m_minimum_value;
			T m_maximum_value;
			T m_scale;
			bool m_reset;

			uint32_t m_next_time;
	};

	template <typename T>
		ScreenDynamicAxis<T>::ScreenDynamicAxis(const char * title, uint8_t axis, T min, T max, T scale, typename Functor<void, uint8_t, T>::FuncPtr fptr, bool reset)
		: Screen(title, SELECTOR)
		, Functor<void, uint8_t, T>(fptr)
		, m_minimum_value(min)
		, m_maximum_value(max)
		, m_scale(scale)
		, m_axis(axis)
		, m_next_time(0)
		, m_reset(reset)
	{ }

	template <typename T>
		ScreenDynamicAxis<T>::~ScreenDynamicAxis()
	{ }

	template <typename T>
		void ScreenDynamicAxis<T>::init(uint16_t index)
	{
		if(m_reset == true)
		{
			m_select = 0;
		}
		else
		{
			m_select = current_position[m_axis];
		}
		m_next_time = millis() + 500;
	}

	template <typename T>
		void ScreenDynamicAxis<T>::left()
	{
		if (m_select != m_minimum_value)
		{
			m_select -= m_scale;
		}

		if (m_select < m_minimum_value)
		{
			m_select = m_minimum_value;
		}

		m_next_time = millis() + 500;
	}

	template <typename T>
		void ScreenDynamicAxis<T>::right()
	{
		if (m_select != m_maximum_value)
		{
			m_select += m_scale;
		}

		if (m_select > m_maximum_value)
		{
			m_select = m_maximum_value;
		}

		m_next_time = millis() + 500;
	}

	template <typename T>
		void ScreenDynamicAxis<T>::draw()
	{
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
			painter.setFont(FontType_t::BODY_FONT);

			char tmp_selected[7] = { 0 };
			int number_size = 4;
			if(m_select >= 100)
			{
				number_size = 6;
			}
			else if(m_select >= 10 && m_select < 100)
			{
				number_size = 5;
			}

			dtostrf(m_select, number_size, 2, tmp_selected);
			char info[20] = { 0 };
			char units[] = "mm";

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
				strcat(info, tmp_selected);
				strcat(info, units);
				strcat(info, " >");
			}

			Area info_area(0, ((y_end + y_init)/2 - 9/2), 128, 9);
			painter.setWorkingArea(info_area);
			painter.text(info);

		} while( painter.nextPage() );

		if (m_next_time < millis())
		{
			this->action(m_axis, m_select);
		}
	}

	template <typename T>
		void ScreenDynamicAxis<T>::press()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}
}
#endif // SCREEN_DYNAMIC_AXIS_H
