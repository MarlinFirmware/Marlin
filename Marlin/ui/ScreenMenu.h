///////////////////////////////////////////////////////////////////////////////
/// \file ScreenMenu.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of menu-type screens.
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

#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include <stdint.h>

#include "Screen.h"
#include "Icon.h"
#include "GuiImpl_witbox_2.h"

namespace ui
{
	class ScreenMenu : public Screen
	{
		public:
			ScreenMenu(const char * title = 0, const char * text = 0);
			virtual ~ScreenMenu();

			void init(uint16_t index = 0);
			void left();
			void right();
			void press();
			virtual void draw();
			void add(ScreenIndex_t const & component);
			void icon(Icon * component);

		protected:
			uint16_t m_index;
			uint16_t m_num_items;
			uint16_t m_num_icons;
			ScreenIndex_t m_items[max_items];
			Icon * m_icons[max_items];
			const char * m_text;
	};
}
#endif //SCREEN_MENU_H
