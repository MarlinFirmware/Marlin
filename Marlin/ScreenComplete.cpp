#include "ScreenComplete.h"

namespace screen
{
	ScreenComplete::ScreenComplete(const char * title, const char * message,  const char * box, Time_t time)
		: ScreenDialog<void>(title, message, box)
		, m_time(time)
	{

	}

	ScreenComplete::~ScreenComplete()
	{ }

	void ScreenComplete::draw()
	{
		char c_time[24];
		snprintf(c_time, 24, "Total time: %02d:%02d", m_time.hours, m_time.minutes);
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);
			painter.multiText(m_message, true);

			painter.setPrintPos(13, 42);
			painter.print(c_time);

		} while( painter.nextPage() );
	}
}