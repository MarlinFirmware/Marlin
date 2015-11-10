///////////////////////////////////////////////////////////////////////////////
/// \file ScreenDialog.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of dialog-type screens.
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

#ifndef SCREEN_DIALOG_H
#define SCREEN_DIALOG_H

#include "Screen.h"
#include "Functor.h"

#include "GuiManager.h"

namespace ui
{
	template <typename R, typename... Args>
		class ScreenDialog : public Screen , public Functor<R, Args ...>
	{
		public:
			ScreenDialog(const char * title, const char * message, const char * box, typename Functor<R, Args...>::FuncPtr fptr = do_nothing);
			virtual ~ScreenDialog();

			void init(uint16_t index = 0);
			void press();
			virtual void draw();

		protected:
			const char * m_message;
			const char * m_box;
	};

	template <typename R, typename... Args>
		ScreenDialog<R, Args...>::ScreenDialog(const char * title, const char * message, const char * box, typename Functor<R, Args...>::FuncPtr fptr)
		: Screen(title, DIALOG)
		, Functor<R, Args...>(fptr)
		, m_message(message)
		, m_box(box)
	{ }

	template <typename R, typename... Args>
		ScreenDialog<R, Args...>::~ScreenDialog()
	{ }

	template <typename R, typename... Args>
		void ScreenDialog<R, Args...>::init(uint16_t index)
	{
		lcd_disable_button();
		draw();
		this->action();
		lcd_enable_button();
	}

	template <typename R, typename... Args>
		void ScreenDialog<R, Args...>::press()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}

	template <typename R, typename... Args>
		void ScreenDialog<R, Args...>::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			Area text_area(0, 14, 127, 54);
			painter.setWorkingArea(text_area);
			painter.multiText_P(m_message);
		} while ( painter.nextPage() );
	}
}
#endif //SCREEN_DIALOG_H
