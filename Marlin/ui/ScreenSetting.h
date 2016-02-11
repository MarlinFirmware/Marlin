///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSetting.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of Settings screen class.
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

#ifndef SCREEN_SETTING_H
#define SCREEN_SETTING_H

#include <stdint.h>

#include "Screen.h"
#include "Option.h"

namespace ui
{
	class ScreenSetting : public Screen
	{
		public:
			ScreenSetting(const char * title);
			virtual ~ScreenSetting();

			void left();
			void right();
			void draw();
			void press();
			void add(Option * view);
			void init(uint16_t index = 0);

		private:
			uint16_t m_index;

			static const uint16_t m_max_items = 11;
			Option * m_item[m_max_items];
			uint8_t m_num_items;

			float m_scroll_size;

	};
}
#endif //SCREEN_SETTING_H
