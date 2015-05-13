#include "ScreenList.h"
#include "cardreader.h"

State_t state;
Event_t event;

State_func_t * const state_table[NUM_STATES] =
{
	do_state_prepare,
	do_state_paint
};

State_t run_state(State_t current_state, Event_t event)
{
	return state_table[current_state](event);
};

State_t do_state_prepare(Event_t event)
{
	printf ("[do_state_prepare]\n");
	if (event == EVENT_PREPARED)
		return STATE_PAINT;

	return STATE_PREPARE;
}

State_t do_state_paint(Event_t event)
{
	printf ("[do_state_paint]\n");
	if (event == EVENT_KEYPRESS)
		return STATE_PREPARE;

	return STATE_PAINT;
}


namespace screen
{
	ScreenList::ScreenList(const char * title)
		: Screen(title, LIST)
		, m_index(0)
		, m_num_list(0)
	{
		state = STATE_PREPARE;
	}

	ScreenList::~ScreenList()
	{
	}

	void ScreenList::left()
	{
		if (m_index == 0)
		{
			m_index = 0;
		}
		else
		{
			--m_index;
		}
	}

	void ScreenList::right()
	{
		if ( m_index == (m_num_list -1) )
		{
			m_index = m_num_list -1;
        }
      else
		{
			++m_index;
		}
	}

	void ScreenList::draw()
	{
		if (state == STATE_PREPARE)
		{
			m_num_list = card.getnrfilenames();
			m_index = 0;

			card.getWorkDirName();
			strncpy(m_directory, card.filename, 9);
			m_directory[9] = '\0';

			state = run_state(state, EVENT_PREPARED);
		}

		card.getfilename(m_index);

		SERIAL_ECHO(m_title);
		SERIAL_ECHO(" ");
		SERIAL_ECHO(m_directory);
		SERIAL_ECHO("  (item ");
		SERIAL_ECHO(m_index + 1);
		SERIAL_ECHO(" / ");
		SERIAL_ECHO(m_num_list);
		SERIAL_ECHO(") : ");
		SERIAL_ECHO(card.longFilename);
		if (card.filenameIsDir == true)
		{
			SERIAL_ECHOLN(" [D]");
		} 
		else
		{
			SERIAL_ECHOLN("");
		}
	}

	Screen & ScreenList::press(Screen * parent_view)
	{
		state = run_state(state, EVENT_KEYPRESS);

		card.getfilename(m_index);

		if (card.filenameIsDir == true)
		{
			card.chdir(card.filename);
			return * this;
		}

		return * m_next_screen;
	}

   void ScreenList::add(Screen & component)
   {
      m_next_screen = &component;
   }
}
