///////////////////////////////////////////////////////////////////////////////
/// \file OptionToggle.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Implementation for toggle Option objects.
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

#include "OptionToggle.h"

#include "GuiPainter.h"
#include "ViewManager.h"

namespace screen
{
	OptionToggle::OptionToggle(option::Size const & size, const char * text, Functor<void>::FuncPtr fptr, Subject<bool> * model)
		: Option(size, text)
		, Functor<void>(fptr)
		, Observer<bool>(model)
		, m_state(OFF)
	{ }

	OptionToggle::~OptionToggle()
	{ }

	const char * OptionToggle::text() const
	{
		return m_text;
	}

	void OptionToggle::draw()
	{	
		Area draw_area = painter.getWorkingArea();
		painter.setPrintPos(draw_area.x_init, draw_area.y_init);
		painter.print_P(m_text);

		switch(m_state)
		{
			case ON:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< On  >");
				break;
			case OFF:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< Off >");
				break;
			default:
				break;
		}
	}

	void OptionToggle::press()
	{
		this->action();
	}

	void OptionToggle::update(bool value)
	{
		if(value)
		{
			m_state = ON;
		}
		else
		{
			m_state = OFF;
		}
		ViewManager::getInstance().displayRefresh();
	}
}