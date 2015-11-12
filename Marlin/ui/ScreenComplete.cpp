///////////////////////////////////////////////////////////////////////////////
/// \file ScreenComplete.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of Print Complete screen.
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

#include "ScreenComplete.h"

#include "Language.h"

namespace ui
{
	ScreenComplete::ScreenComplete(const char * title, const char * message,  const char * box, Time_t time)
		: ScreenDialog<void>(title, message, box)
		, m_time(time)
	{ }

	ScreenComplete::~ScreenComplete()
	{ }

	void ScreenComplete::draw()
	{
		char c_time[24];
		char total_time[24];
		strcpy_P(total_time, MSG_TOTAL_TIME());
		const char space[2] {' '};
		strcat(total_time, space);
		snprintf(c_time, 24, "%02d:%02d", m_time.hours, m_time.minutes);
		strcat(total_time, c_time);
		
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);

			Area text_area(0, 14, 127, 41);
			painter.setWorkingArea(text_area);
			painter.multiText_P(m_message);

			Area time_area(0, 42, 127, 54);
			painter.setWorkingArea(time_area);
			painter.text(total_time);

		} while( painter.nextPage() );
	}
}
