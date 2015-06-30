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

#include "Screen.h"

typedef enum
{
	STATE_PREPARE = 0,
	STATE_PAINT,
	NUM_STATES,
} State_t;

typedef enum
{
	EVENT_PREPARED,
	EVENT_KEYPRESS,
	EVENT_SDCHANGED
} Event_t;

typedef State_t State_func_t(Event_t event);

// State functions
State_t do_state_prepare(Event_t event);
State_t do_state_paint(Event_t event);

// State table
extern State_func_t * const state_table[NUM_STATES];

// State runner
State_t run_state(State_t current_state, Event_t event);

namespace screen
{
	class ScreenList : public Screen
	{
		public:
			ScreenList(const char * title);
			virtual ~ScreenList();

			void left();
			void right();
			void draw();
			void press();
			void add(ScreenIndex_t const & view);

		private:
			void updateSdcardStatus();

		private:
			uint16_t m_index;
			uint16_t m_num_list;
			uint8_t m_icon_index;

			ScreenIndex_t m_back_screen;
			uint8_t m_num_item_added;

			bool m_sdcard_inserted;

			char m_directory[10];
			bool m_directory_is_root;
			uint8_t m_offset;

			float m_scroll_size;
	};
}
#endif //SCREEN_LIST_H
