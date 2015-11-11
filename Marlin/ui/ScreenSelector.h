///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSelector.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of selector-type screens.
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

#ifndef SCREEN_SELECTOR_H
#define SCREEN_SELECTOR_H

#include <stdint.h>

#include "Screen.h"
#include "Functor.h"

#include "Language.h"

namespace ui
{
	template <typename R, typename... Args>
		class ScreenSelector : public Screen , public Functor<R, Args...>
	{
		public:
			ScreenSelector(const char * title, const char * box, uint16_t min, uint16_t max, uint16_t scale, uint16_t dflt, typename Functor<R, Args...>::FuncPtr fptr = do_nothing);
			virtual ~ScreenSelector();

			void init(uint16_t index = 0);

			void left();
			void right();
			void draw();
			void press();

		protected:
			uint16_t m_select;
			uint16_t m_minimum_value;
			uint16_t m_maximum_value;
			uint16_t m_scale;
			uint16_t m_default;
			const char * m_box;
	};

	template <typename R, typename... Args>
		ScreenSelector<R, Args...>::ScreenSelector(const char * title, const char * box, uint16_t min, uint16_t max, uint16_t scale, uint16_t dflt, typename Functor<R, Args...>::FuncPtr fptr)
		: Screen(title, SELECTOR)
		, m_box(box)
		, Functor<R, Args...>(fptr)
		, m_minimum_value(min)
		, m_maximum_value(max)
		, m_scale(scale)
		, m_default(dflt)
		, m_select(dflt)
	{ }

	template <typename R, typename... Args>
		ScreenSelector<R, Args...>::~ScreenSelector()
	{ }

	template <typename R, typename... Args>
		void ScreenSelector<R, Args...>::init(uint16_t index)
	{
		m_select = m_default;
	}

	template <typename R, typename... Args>
		void ScreenSelector<R, Args...>::left()
	{
		if (m_select != m_minimum_value)
		{
			m_select -= m_scale;
			m_needs_drawing = true;
		}

		if (m_select < m_minimum_value)
		{
			m_select = m_minimum_value;
		}
	}

	template <typename R, typename... Args>
		void ScreenSelector<R, Args...>::right()
	{
		if (m_select != m_maximum_value)
		{
			m_select += m_scale;
			m_needs_drawing = true;
		}

		if (m_select > m_maximum_value)
		{
			m_select = m_maximum_value;
		}

	}

	template <typename R, typename... Args>
		void ScreenSelector<R, Args...>::draw()
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
					strcat(info, tmp_selected);
					strcat(info, units);
					strcat(info, " >");
				}

				Area info_area(0, ((y_end + y_init)/2 - 9/2), 128, 9);
				painter.setWorkingArea(info_area);
				painter.text(info);

			} while( painter.nextPage() );
		}
	}

	template <typename R, typename... Args>
		void ScreenSelector<R, Args...>::press()
	{
		this->action(m_select);
		ViewManager::getInstance().activeView(m_next_screen);
	}
}
#endif //SCREEN_SELECTOR_H
