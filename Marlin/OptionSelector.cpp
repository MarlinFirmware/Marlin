///////////////////////////////////////////////////////////////////////////////
/// \file OptionSelector.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Implementation for selector Option objects.
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

#include "OptionSelector.h"

#include "GuiPainter.h"
#include "ViewManager.h"
#include "StorageManager.h"

namespace screen
{
	OptionSelector::OptionSelector(option::Size const & size, const char * text)
		: Option(size, text)
	{ 
		m_state = static_cast<Language>(eeprom::StorageManager::getLanguage());
	}

	OptionSelector::~OptionSelector()
	{ }

	const char * OptionSelector::text() const
	{
		return m_text;
	}

	void OptionSelector::draw()
	{	
		Area draw_area = painter.getWorkingArea();
		painter.setPrintPos(draw_area.x_init, draw_area.y_init);
		painter.print_P(m_text);

		m_state = static_cast<Language>(eeprom::StorageManager::getLanguage());

		switch(m_state)
		{
			case Language::EN:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< EN  >");
				break;
			case Language::ES:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< ES >");
				break;
			case Language::DE:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< DE  >");
				break;
			case Language::FR:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< FR  >");
				break;
			case Language::PT:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< PT  >");
				break;
			case Language::IT:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< IT  >");
				break;
			case Language::PL:
				painter.setPrintPos(draw_area.x_end - 7*6, draw_area.y_init);
				painter.print("< PL  >");
				break;
			default:
				break;
		}
	}

	void OptionSelector::press()
	{	
		ViewManager::getInstance().activeView(m_next_screen);
	}

	void OptionSelector::add(ScreenIndex_t const & view)
	{
		m_next_screen = view;
	}

}