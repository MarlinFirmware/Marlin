#include "ScreenComplete.h"

#include "Language.h"

namespace screen
{
	ScreenComplete::ScreenComplete(const char * title, const char * message,  const char * box, Time_t time)
		: ScreenDialog<void>(title, message, box)
		, m_time(time)
	{ }

	ScreenComplete::~ScreenComplete()
	{ }

	void ScreenComplete::draw()
	{
		char c_time[24];
		char total_time[24];
		strcpy_P(total_time, MSG_TOTAL_TIME());
		const char space[2] {' '};
		strcat(total_time, space);
		snprintf(c_time, 24, "%02d:%02d", m_time.hours, m_time.minutes);
		strcat(total_time, c_time);
		
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);

			Area text_area(0, 14, 127, 41);
			painter.setWorkingArea(text_area);
			painter.multiText_P(m_message);

			Area time_area(0, 42, 127, 54);
			painter.setWorkingArea(time_area);
			painter.text(total_time);

		} while( painter.nextPage() );
	}
}
