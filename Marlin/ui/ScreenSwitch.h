///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSwitch.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of switch-type screens.
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

#ifndef SCREEN_SWITCH_H
#define SCREEN_SWITCH_H

#include <stdint.h>

#include "ScreenAction.h"

namespace ui
{
	class ScreenSwitch : public ScreenAction<bool>
	{
		public:
			ScreenSwitch(const char * title = 0, Functor<bool>::FuncPtr fptr = false);
			virtual ~ScreenSwitch();

			void init(uint16_t index = 0);
			void add(ScreenIndex_t const & component);

		private:
			uint8_t m_num_items;
			static const uint8_t m_max_items = 2;
			ScreenIndex_t m_items[m_max_items];
	};
}

#endif //SCREEN_SWITCH_H
