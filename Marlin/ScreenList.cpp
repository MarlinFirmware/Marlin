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
	SERIAL_ECHOLN ("[do_state_prepare]");
	if (event == EVENT_PREPARED)
		return STATE_PAINT;

	return STATE_PREPARE;
}

State_t do_state_paint(Event_t event)
{
	SERIAL_ECHOLN ("[do_state_paint]");
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
		memset (m_directory, 0, sizeof(m_directory)); 
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

		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.setPrintPos(painter.coordinateXInit(), painter.coordinateYInit());
			SERIAL_ECHO(m_title);
			SERIAL_ECHO(" ");
			SERIAL_ECHO(m_directory);
			SERIAL_ECHO("  (item ");
			SERIAL_ECHO(m_index + 1);
			SERIAL_ECHO(" / ");
			SERIAL_ECHO(m_num_list);
			SERIAL_ECHO(") : ");
			SERIAL_ECHO(card.longFilename);
			painter.print(card.longFilename);
			if (card.filenameIsDir == true)
			{
				SERIAL_ECHOLN(" [D]");
			} 
			else
			{
				SERIAL_ECHOLN("");
			}
		} while( painter.nextPage() );
	}

	Screen & ScreenList::press(Screen * parent_view)
	{
		SERIAL_ECHOLN("ScreenList::press");
		state = run_state(state, EVENT_KEYPRESS);
		SERIAL_ECHOLN("ScreenList::press - State machine run");
		/*
			card.getfilename(m_index);
			SERIAL_ECHOLN("ScreenList::press - card.getfilename");
			*/

		if (card.filenameIsDir == true)
		{
			SERIAL_ECHOLN("ScreenList::press - isDir");
			card.chdir(card.filename);
			return * this;
		}
		SERIAL_ECHOLN("ScreenList::press - return next screen");
		return * m_next_screen;
	}

	void ScreenList::add(Screen & component)
	{
		m_next_screen = &component;
	}
}
