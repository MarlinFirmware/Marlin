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

namespace screen
{
	ScreenSerial::ScreenSerial(const char * title, const char * text)
		: Screen(title, MENU)
		, m_text(text)
	{ }

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
				painter.title(m_title);
				Area text_area(0, 18, 127, 55);
				painter.setWorkingArea(text_area);
				painter.text(m_text);
			} while( painter.nextPage() );
		}
	}

	void ScreenSerial::text(const char * text)
	{
		m_text = text;
		m_needs_drawing = true;
	}
}
