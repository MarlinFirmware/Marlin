#include "ScreenStop.h"

#include "Language.h"
#include "GuiBitmaps_witbox_2.h"

namespace ui
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
		uint8_t padding = 13;

		char c_time[24];
		char total_time[24] = "";
		snprintf(c_time, 24, "%02d:%02d", m_time.hours, m_time.minutes);
		strcat(total_time, c_time);

		char height[9] = "Z:";
		char z_value[7] = { 0 };
		uint8_t number_size = 5;

		if(m_height < 100)
		{
			number_size = 4;
		}
		else if(m_height < 10)
		{
			number_size = 3;
		}

		dtostrf(m_height, number_size, 2, z_value);
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

			painter.setPrintPos(padding, 43);
			painter.print(height);

			painter.drawBitmap(127 - strlen(total_time) * 6 - padding - icon_clock_width - 2, 43, icon_clock_width, icon_clock_height, icon_clock);
			painter.setPrintPos( 127 - strlen(total_time) * 6 - padding, 43);
			painter.print(total_time);

		} while( painter.nextPage() );
	}
}
