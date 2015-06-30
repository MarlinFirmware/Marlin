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
		snprintf(c_time, 24, "%d hours, %d minutes", m_time.hours, m_time.minutes);

		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);
			painter.setPrintPos(13, 20);
			painter.print_P(m_message);
			painter.setPrintPos(2, 36);
			painter.print("Total time:");
			painter.setPrintPos(2, 45);
			painter.print(c_time);
		} while( painter.nextPage() );
	}
}