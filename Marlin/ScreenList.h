#ifndef SCREEN_LIST_H
#define SCREEN_LIST_H

#include "Screen.h"

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
			Screen & press(Screen * parent_view);
			void add(Screen & component);

		private:
			uint16_t m_index;
			uint16_t m_num_list;
			Screen * m_next_screen;
			Screen * m_items[max_list];
			char m_directory[10];
	};
}

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
} Event_t;

typedef State_t State_func_t(Event_t event);

// State functions
State_t do_state_prepare(Event_t event);
State_t do_state_paint(Event_t event);

// State table
extern State_func_t * const state_table[NUM_STATES];

// State runner
State_t run_state(State_t current_state, Event_t event);

#endif //SCREEN_LIST_H
