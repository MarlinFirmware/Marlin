///////////////////////////////////////////////////////////////////////////////
/// \file ScreenEmergency.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of Emergency screen.
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

#include "ScreenEmergency.h"

#include "GuiBitmaps_witbox_2.h"
#include "GuiPainter.h"

extern bool stop_buffer;
extern uint16_t stop_buffer_code;

namespace ui
{
	ScreenEmergency::ScreenEmergency(const char * title, const char * message, const char * box, const unsigned char * bitmap)
		: Screen(title, SIMPLE)
		, m_message(message)
		, m_box(box)
		, m_bitmap(bitmap)
		, m_next_screen(screen_none)
	{
		stop_buffer = true;
		stop_buffer_code = 999;
	}

	ScreenEmergency::~ScreenEmergency()
	{ }

	void ScreenEmergency::init(uint16_t index)
	{
		draw();

		if(m_next_screen != screen_none)
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	void ScreenEmergency::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);

			Area bitmap_area(0, 14, 39, 54);
			painter.setWorkingArea(bitmap_area);

			uint8_t x_offset = 12;
			uint8_t y_offset = 9;

			Area temp_area = painter.getWorkingArea();
			painter.drawBitmap(temp_area.x_init + x_offset, temp_area.y_init + y_offset, emergency_width, emergency_height, bits_emergency);

			Area text_area(40, 14, 127, 54);
			painter.setWorkingArea(text_area);
			painter.multiText_P(m_message);
		} while (painter.nextPage());
	}

	void ScreenEmergency::add(ScreenIndex_t const & component)
	{
		m_next_screen = component;
	}
}
