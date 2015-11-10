///////////////////////////////////////////////////////////////////////////////
/// \file Screen.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of base class for Screen interface.
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

#ifndef SCREEN_H
#define SCREEN_H

#include <new.h>
#include <stdint.h>

#include "GuiPainter.h"
#include "ViewManager.h"

namespace ui
{
	class Screen
	{
		public:
			typedef enum
			{
				SIMPLE = 0,
				ACTION,
				STATUS,
				MENU,
				PRINT,
				DIALOG,
				LIST,
				SELECTOR,
				SWITCH,
				NUM_SCREEN_TYPES,
			} ScreenType_t;

		public:
			Screen(const char * title = 0, ScreenType_t const & type = SIMPLE);
			virtual ~Screen();

			const char * title() const;
			ScreenType_t const & type() const;

			virtual void left() {};
			virtual void right() {};
			virtual void press() {};
			virtual void draw() {};
			virtual void init(uint16_t index = 0) {};
			virtual void add(ScreenIndex_t const & view);
			virtual void text(const char * text) {};

		public:
			bool m_needs_drawing;

		protected:
			ScreenType_t m_type;
			const char * m_title;
			ScreenIndex_t m_next_screen;
	};
}
#endif //SCREEN_H
