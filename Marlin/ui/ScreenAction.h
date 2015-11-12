///////////////////////////////////////////////////////////////////////////////
/// \file ScreenAction.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of action-type screens.
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

#ifndef SCREEN_ACTION_H
#define SCREEN_ACTION_H

#include <stdint.h>

#include "Screen.h"
#include "Functor.h"

namespace ui
{
	template <typename R, typename... Args>
		class ScreenAction : public Screen, public Functor<R, Args...>
	{
		public:
			ScreenAction(const char * title = 0, typename Functor<R, Args...>::FuncPtr fptr = do_nothing);
			virtual ~ScreenAction();

			void init(uint16_t index = 0);
	};

	template <typename R, typename... Args>
		ScreenAction<R, Args...>::ScreenAction(const char * title, typename Functor<R, Args...>::FuncPtr fptr)
		: Screen(title, ACTION)
		, Functor<R, Args...>(fptr)
	{ }

	template <typename R, typename... Args>
		ScreenAction<R, Args...>::~ScreenAction()
	{ }

	template <typename R, typename... Args>
		void ScreenAction<R, Args...>::init(uint16_t index)
	{
		ViewManager::getInstance().setLastFocus(0);
		this->action();
		ViewManager::getInstance().activeView(m_next_screen);
	}
}
#endif //SCREEN_ACTION_H
