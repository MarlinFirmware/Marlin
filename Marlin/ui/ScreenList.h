///////////////////////////////////////////////////////////////////////////////
/// \file ScreenList.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of SD screen list class.
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

#ifndef SCREEN_LIST_H
#define SCREEN_LIST_H

#include <stdint.h>

#include "Screen.h"
#include "SDCache.h"
#include "SDManager.h"

namespace ui
{
	class ScreenList : public Screen , public Observer<bool>
	{
		public:
			ScreenList(const char * title, Subject<bool> * model);
			virtual ~ScreenList();

			void left();
			void right();
			void draw();
			void press();
			void add(ScreenIndex_t const & view);
			void init(uint16_t index = 0);

		private:
			//void updateSdcardStatus();
			void update(bool is_inserted);


		private:
			static uint8_t directory_index;
			static uint8_t directory_array[10];
			static bool from_updir;

			uint16_t m_index;
			uint16_t m_num_list;
			uint8_t m_icon_index;

			ScreenIndex_t m_back_screen;
			SDCache * m_browsing_cache;
			uint8_t m_num_item_added;

			char m_directory[20];
			bool m_directory_is_root;

			ScreenIndex_t m_error_screen;

			float m_scroll_size;

			uint32_t m_current_time;
			uint32_t m_previous_time;
	};
}
#endif //SCREEN_LIST_H
