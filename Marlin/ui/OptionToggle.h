///////////////////////////////////////////////////////////////////////////////
/// \file OptionToggle.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Interface for toggle Option objects.
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

#ifndef OPTION_TOGGLE_H
#define OPTION_TOGGLE_H

#include "Option.h"
#include "Observer.h"
#include "Functor.h"

#include "GuiPainter.h"
#include "ViewManager.h"

namespace ui
{
	typedef enum
	{
		OFF,
		ON,
		AUTO,
		NUM_STATES
	} ToggleState_t;

	template <typename T>
		class OptionToggle : public Option , public Functor<void>, public Observer<T>
	{
		public:
			OptionToggle(option::Size const & size, const char * text, Functor<void>::FuncPtr fptr = do_nothing, Subject<T> * model = 0);
			virtual ~OptionToggle();

			const char * text() const;
			virtual void draw();
			virtual void press();

			void update(T value);

		private:
			ToggleState_t m_state;
	};

	template <typename T>
	OptionToggle<T>::OptionToggle(option::Size const & size, const char * text, Functor<void>::FuncPtr fptr, Subject<T> * model)
		: Option(size, text)
		, Functor<void>(fptr)
		, Observer<T>(model)
	{
		this->connect();
	}

	template <typename T>
	OptionToggle<T>::~OptionToggle()
	{ }

	template <typename T>
	const char * OptionToggle<T>::text() const
	{
		return m_text;
	}

	template <typename T>
	void OptionToggle<T>::draw()
	{
		Area draw_area = painter.getWorkingArea();
		painter.setPrintPos(draw_area.x_init, draw_area.y_init);
		painter.print_P(m_text);

		switch(m_state)
		{
			case ON:
				painter.setPrintPos(draw_area.x_end - 4*6, draw_area.y_init);
				painter.print("[On]");
				break;
			case OFF:
				painter.setPrintPos(draw_area.x_end - 5*6, draw_area.y_init);
				painter.print("[Off]");
				break;
			case AUTO:
				painter.setPrintPos(draw_area.x_end - 6*6, draw_area.y_init);
				painter.print("[Auto]");
				break;
			default:
				break;
		}
	}

	template <typename T>
	void OptionToggle<T>::press()
	{
		this->action();
	}

	template <typename T>
	void OptionToggle<T>::update(T value)
	{
		if(value == 0)
		{
			m_state = OFF;
		}
		else if(value == 1)
		{
			m_state = ON;
		}
		else if(value == 2)
		{
			m_state = AUTO;
		}
		ViewManager::getInstance().displayRefresh();
	}
	
}

#endif //OPTION_TOGGLE_H