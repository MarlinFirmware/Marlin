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

#include "Screen.h"

#include "TemperatureManager.h"
#include "ViewManager.h"

namespace screen
{
	template <typename T>
		class ScreenAnimation : public Screen , public Observer<T>
	{
		public:
			typedef enum
			{
				LESS_OR_EQUAL,
				GREATER_OR_EQUAL,
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

		private:
			bool isConditionMeet();

		private:
			const char * m_text;

			uint16_t m_target;
			T m_observed;

			ScreenIndex_t m_back_screen;
			uint8_t m_num_item_added;

			Condition_t m_condition;
	};

	template <typename T>
		ScreenAnimation<T>::ScreenAnimation(const char * title, const char * text, Condition_t condition, uint16_t target, Subject<T> * model)
		: Screen(title, SELECTOR)
		, Observer<T>(model)
		, m_text(text)
		, m_num_item_added(0)
		, m_observed(0)
		, m_condition(condition)
		, m_target(target)
	{
		this->connect();
	}

	template <typename T>
		ScreenAnimation<T>::~ScreenAnimation()
	{
		this->m_model->dettach();
	}

	template <typename T>
		void ScreenAnimation<T>::init(uint16_t index)
	{
		this->m_model->attach(this);
		this->connect();
	}

	template <typename T>
		void ScreenAnimation<T>::draw()
	{
		char c_target[4] = { 0 };
		snprintf(c_target, 4, "%d", m_target);

		char c_current[4] = { 0 };
		dtostrf(m_observed, 3, 0, c_current);

		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_text);

			uint8_t y_init = painter.coordinateYInit();
			uint8_t y_end = painter.coordinateYEnd();

			painter.setColorIndex(1);
			if(m_observed > 99.0)
			{
				painter.setPrintPos(31,(y_end + y_init)/2 - 9/2);
			}
			else if(m_observed > 9.0)
			{
				painter.setPrintPos(37,(y_end + y_init)/2 - 9/2);
			}
			else
			{
				painter.setPrintPos(43,(y_end + y_init)/2 - 9/2);
			}
			painter.print(c_current);
			painter.print(" / ");
			painter.print(c_target);

		} while ( painter.nextPage() );

		if ( isConditionMeet() )
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	template <typename T>
		void ScreenAnimation<T>::press()
	{
		ViewManager::getInstance().activeView(m_back_screen);
	}

	template <typename T>
		void ScreenAnimation<T>::add(ScreenIndex_t const & component)
	{
		if (m_num_item_added % 2)
		{
			m_next_screen = component;
		}
		else
		{
			m_back_screen = component;
		}
		m_num_item_added++;
	}

	template<typename T>
		void ScreenAnimation<T>::update(T value)
	{
		m_observed = value;
	}

	template<typename T>
		bool ScreenAnimation<T>::isConditionMeet()
	{
		switch(m_condition)
		{
			case LESS_OR_EQUAL:
				return ((uint16_t) m_observed <= m_target);
			case GREATER_OR_EQUAL:
				return ((uint16_t) m_observed >= m_target);
			default:
				return false;
		}
	}
}
#endif //SCREEN_ANIMATION_H
