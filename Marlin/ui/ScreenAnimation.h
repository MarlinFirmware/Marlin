///////////////////////////////////////////////////////////////////////////////
/// \file ScreenAnimation.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of animation-type screens.
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

#ifndef SCREEN_ANIMATION_H
#define SCREEN_ANIMATION_H

#include <math.h>

#include "Screen.h"
#include "GuiManager.h"
#include "TemperatureManager.h"
#include "ViewManager.h"

namespace ui
{
	template <typename T>
		class ScreenAnimation : public Screen , public Observer<T>
	{
		public:
			typedef enum
			{
				LESS_OR_EQUAL,
				EQUAL,
				GREATER_OR_EQUAL,
				RANGE,
				NUM_CONDITIONS
			} Condition_t;

		public:
			ScreenAnimation(const char * title, const char * text, Condition_t condition, uint16_t target, Subject<T> * model = 0);
			virtual ~ScreenAnimation();

			void init(uint16_t index = 0);

			void draw();
			void press();

			void add(ScreenIndex_t const & component);
			void update(T value);

		protected:
			bool isConditionMet();

		protected:
			const char * m_text;

			uint16_t m_target;
			T m_observed;

			ScreenIndex_t m_back_screen;
			uint8_t m_num_item_added;

			Condition_t m_condition;
			uint32_t m_previous_time;
			uint32_t m_current_time;
			uint8_t m_frame;
	};

	template <typename T>
		ScreenAnimation<T>::ScreenAnimation(const char * title, const char * text, Condition_t condition, uint16_t target, Subject<T> * model)
		: Screen(title, SELECTOR)
		, Observer<T>(model)
		, m_text(text)
		, m_num_item_added(0)
		, m_observed(0)
		, m_back_screen(screen_none)
		, m_condition(condition)
		, m_target(target)
		, m_previous_time(0)
		, m_current_time(0)
		, m_frame(0)
	{
		this->connect();
		lcd_disable_button();
	}

	template <typename T>
		ScreenAnimation<T>::~ScreenAnimation()
	{ }

	template <typename T>
		void ScreenAnimation<T>::init(uint16_t index)
	{
		this->connect();
	}

	template <typename T>
		void ScreenAnimation<T>::draw()
	{
		char c_target[4] = { 0 };
		snprintf(c_target, 4, "%d", m_target);

		char c_current[4] = { 0 };
		dtostrf(m_observed, 3, 0, c_current);

		//Check time for next progress bar frame
		m_current_time = millis();

		if(m_current_time >= m_previous_time + 100)
		{
			m_current_time = millis();
			m_previous_time = m_current_time;
			m_frame = (m_frame + 1) % 8;

			painter.firstPage();
			do
			{
				painter.title(m_title);
				painter.box(m_text);

				//Draw temperature
				uint8_t y_init = painter.coordinateYInit();
				uint8_t y_end = painter.coordinateYEnd();

				painter.setColorIndex(1);
				painter.setPrintPos(31,(y_end + y_init)/2 - 9/2 - 3);

				painter.print(c_current);
				painter.print("\xb0");
				painter.print(" / ");
				painter.print(c_target);
				painter.print("\xb0");

				//Draw progress bar
				painter.setColorIndex(1);
				painter.drawBox(8,38,112,6);
				painter.setColorIndex(0);
				painter.drawBox(9,39,110,4);
				painter.setColorIndex(1);

				switch(m_frame)
				{
					case 0:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_1);
						break;
					case 1:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_2);
						break;
					case 2:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_3);
						break;
					case 3:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_4);
						break;
					case 4:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_5);
						break;
					case 5:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_6);
						break;
					case 6:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_7);
						break;
					case 7:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_8);
						break;
				}

			} while ( painter.nextPage() );
		}

		if ( isConditionMet() )
		{
			lcd_enable_button();
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	template <typename T>
		void ScreenAnimation<T>::press()
	{
		if(m_back_screen != screen_none)
		{
			ViewManager::getInstance().activeView(m_back_screen);
		}
	}

	template <typename T>
		void ScreenAnimation<T>::add(ScreenIndex_t const & component)
	{
		if ((m_num_item_added % 2) == 0)
		{
			m_next_screen = component;
		}
		else
		{
			m_back_screen = component;
			lcd_enable_button();
		}
		m_num_item_added++;
	}

	template<typename T>
		void ScreenAnimation<T>::update(T value)
	{
		m_observed = value;
	}

	template<typename T>
		bool ScreenAnimation<T>::isConditionMet()
	{
		switch(m_condition)
		{
			case LESS_OR_EQUAL:
			{
				return (round(m_observed) <= m_target);
				break;
			}
			case EQUAL:
			{
				return (round(m_observed) == m_target);
				break;
			}
			case GREATER_OR_EQUAL:
			{
				return (round(m_observed) >= m_target);
				break;
			}
			case RANGE:
				return ( ((round(m_observed) + 5) > m_target) &&
				         ((round(m_observed) - 5) < m_target) );
				break;
			default:
			{
				return false;
				break;
			}
		}
	}
}
#endif //SCREEN_ANIMATION_H
