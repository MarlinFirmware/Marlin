///////////////////////////////////////////////////////////////////////////////
/// \file ScreenTransition.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of transition-type screens.
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

#include "ScreenTransition.h"

#include "GuiManager.h"
#include "Language.h"

namespace ui
{
	ScreenTransition::ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr, Subject<PrinterState_t> * model)
		: Screen(title, DIALOG)
		, Functor<void>(fptr)
		, Observer<PrinterState_t>(model)
		, m_box(box)
		, m_printing_status(PRINTING)
	{
		memset(m_message, 0, sizeof(m_message));

		if( (message != NULL) && (strlen_P(message) > 0) )
		{
			strcpy_P(m_message, message);
		}
	}

	ScreenTransition::~ScreenTransition()
	{ }

	void ScreenTransition::init(uint16_t index)
	{
		lcd_disable_button();
		draw();
		this->action();

		if (this->m_model == 0)
		{
			lcd_enable_button();
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	void ScreenTransition::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			Area text_area(0, 14, 127, 54);
			painter.setWorkingArea(text_area);
			painter.multiText(m_message);
		} while ( painter.nextPage() );

		if (this->m_model != 0 && m_printing_status == PAUSED || m_printing_status == READY)
		{
			lcd_enable_button();
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}
	void ScreenTransition::update(PrinterState_t value)
	{
		m_printing_status = value;

		switch(m_printing_status)
		{
			case HOMING:
			case LEVELING:
				strcpy_P(m_message, MSG_SCREEN_AUTOHOME_HOMING_TEXT());
				break;
			case READY:
				strcpy_P(m_message, MSG_PLEASE_WAIT());
				break;
			default:
			break;
		}
	}
}
