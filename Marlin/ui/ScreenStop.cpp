#include "ScreenStop.h"

#include "Language.h"

namespace screen
{
	ScreenStop::ScreenStop(const char * title, const char * message,  const char * box, float height, Time_t time)
		: ScreenDialog<void>(title, message, box)
		, m_height(height)
		, m_time(time)
	{ }

	ScreenStop::~ScreenStop()
	{ }

	void ScreenStop::draw()
	{
		char c_time[24];
		char total_time[24];
		strcpy(total_time, "T:");
		snprintf(c_time, 24, "%02d:%02d", m_time.hours, m_time.minutes);
		strcat(total_time, c_time);

		char height[8] = "Z:";
		char z_value[6] = { 0 };
		dtostrf(m_height, 5, 2, z_value);
		strcat(height, z_value);
		
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);

			Area text_area(0, 14, 127, 41);
			painter.setWorkingArea(text_area);
			painter.multiText_P(m_message);

			painter.setPrintPos( 0, 41);
			painter.print(height);

			painter.setPrintPos( 127 - strlen(total_time) * 6, 41);
			painter.print(total_time);

		} while( painter.nextPage() );
	}
}
