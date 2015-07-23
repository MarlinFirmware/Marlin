///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSerial.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of serial screen.
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

#include "ScreenSerial.h"
#include "Language.h"

namespace screen
{
	ScreenSerial::ScreenSerial(const char * title, const char * text)
		: Screen(title, MENU)
	{
		strncpy(m_text, text, sizeof(m_text));
	}

	ScreenSerial::~ScreenSerial()
	{ }

	void ScreenSerial::init(uint16_t index)
	{ }

	void ScreenSerial::left()
	{ }

	void ScreenSerial::right()
	{ }

	void ScreenSerial::press()
	{ }

	void ScreenSerial::draw()
	{
		if ( !m_text || m_needs_drawing)
		{
			m_needs_drawing = false;
			
			//Start painting sequence
			painter.firstPage();
			do
			{
				painter.setColorIndex(1);
				painter.drawBox(0,0,128,64);
				painter.setColorIndex(0);
				painter.drawBox(5,5,118,54);
				painter.setColorIndex(1);
				painter.drawBox(5,22,118,3);

				Area title_area(5,10,118,22);
				painter.setWorkingArea(title_area);
				painter.text_P(MSG_SCREEN_SERIAL_TITLE());

				Area text_area(5, 25, 118, 59);
				painter.setWorkingArea(text_area);
				painter.multiText(m_text);
			} while( painter.nextPage() );
		}
	}

	void ScreenSerial::text(const char * text)
	{
		strncpy(m_text, text, sizeof(m_text));
		m_needs_drawing = true;
	}
}
