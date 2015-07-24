///////////////////////////////////////////////////////////////////////////////
/// \file ScreenDynamicFeedrateFeedrate.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of dynamic-type screensto change the feedrate.
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

#ifndef SCREEN_DYNAMIC_FEEDRATE_H
#define SCREEN_DYNAMIC_FEEDRATE_H

#include "Language.h"

namespace screen
{
	template <typename T>
		class ScreenDynamicFeedrate : public Screen , public Functor<void, T>
	{
		public:
			ScreenDynamicFeedrate(const char * title, T min, T max, T scale, typename Functor<void, T>::FuncPtr fptr);
			virtual ~ScreenDynamicFeedrate();

			void init(uint16_t index = 0);

			void left();
			void right();
			void draw();
			void press();

		private:
			T m_select;
			T m_minimum_value;
			T m_maximum_value;
			T m_scale;
	};

	template <typename T>
		ScreenDynamicFeedrate<T>::ScreenDynamicFeedrate(const char * title, T min, T max, T scale, typename Functor<void, T>::FuncPtr fptr)
		: Screen(title, SELECTOR)
		, Functor<void, T>(fptr)
		, m_minimum_value(min)
		, m_maximum_value(max)
		, m_scale(scale)
	{ }

	template <typename T>
		ScreenDynamicFeedrate<T>::~ScreenDynamicFeedrate()
	{ }

	template <typename T>
		void ScreenDynamicFeedrate<T>::init(uint16_t index)
	{
		m_select = action_get_feedrate_multiply();
	}

	template <typename T>
		void ScreenDynamicFeedrate<T>::left()
	{
		if (m_select != m_minimum_value)
		{
			m_select -= m_scale;
		}

		if (m_select < m_minimum_value)
		{
			m_select = m_minimum_value;
		}
	}

	template <typename T>
		void ScreenDynamicFeedrate<T>::right()
	{
		if (m_select != m_maximum_value)
		{
			m_select += m_scale;
		}

		if (m_select > m_maximum_value)
		{
			m_select = m_maximum_value;
		}
	}

	template <typename T>
		void ScreenDynamicFeedrate<T>::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.arrowBox(MSG_SELECTOR_BOX());

			uint8_t x_init = painter.coordinateXInit();
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t y_end = painter.coordinateYEnd();

			painter.setColorIndex(1);
			painter.setFont(u8g_font_6x9);

			char tmp_selected[4] = { 0 };
			dtostrf(m_select, 3, 0, tmp_selected);

			painter.setPrintPos((x_end + x_init)/2 - (strlen("<  >")*6)/2 - (strlen(tmp_selected)*6)/2, (y_end + y_init)/2 - 9/2);

			if (m_select != m_minimum_value)
			{
				painter.print("< ");
			}
			else
			{
				painter.print("  ");
			}

			painter.print(tmp_selected);

			if (m_select != m_maximum_value)
			{
				painter.print(" >");
			}
			else
			{
				painter.print("  ");
			}

		} while( painter.nextPage() );

		this->action(m_select);
	}

	template <typename T>
		void ScreenDynamicFeedrate<T>::press()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}
}
#endif // SCREEN_DYNAMIC_FEEDRATE_H
