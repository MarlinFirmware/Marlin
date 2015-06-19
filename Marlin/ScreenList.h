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
			void add(Screen & component);
//			void icon(Icon & component);
//			Icon & icon();

		private:
			void updateSdcardStatus();

		private:
			uint16_t m_index;
			uint16_t m_num_list;
			uint8_t m_icon_index;
//			Icon * m_icon_alternate;

			Screen * m_back_screen;
			uint8_t m_num_item_added;

			bool m_sdcard_inserted;

			char m_directory[10];
			bool m_directory_is_root;
			uint8_t m_offset;

			float m_scroll_size;
	};
}

#endif //SCREEN_LIST_H
