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

namespace screen
{
	ScreenTransition::ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr, Subject<PrinterState_t> * model)
		: Screen(title, DIALOG)
		, Functor<void>(fptr)
		, Observer<PrinterState_t>(model)
		, m_message(message)
		, m_box(box)
		, m_printing_status(PRINTING)
	{ }

	ScreenTransition::~ScreenTransition()
	{ }

	void ScreenTransition::init(uint16_t index)
	{
		draw();
		this->action();
		if (this->m_model == 0)
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	void ScreenTransition::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.multiText(m_message);
			painter.box(m_box);
		} while ( painter.nextPage() );
		if (this->m_model != 0 && m_printing_status == PAUSED)
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}
	void ScreenTransition::update(PrinterState_t value)
	{
		m_printing_status = value;
	}
}
