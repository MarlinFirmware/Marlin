#include "ScreenEmergency.h"

#include "GuiBitmaps_witbox_2.h"
#include "GuiPainter.h"

extern bool stop_buffer;
extern uint16_t stop_buffer_code;

namespace screen
{
	ScreenEmergency::ScreenEmergency(const char * title, const char * message, const char * box, const unsigned char * bitmap)
		: Screen(title, SIMPLE)
		, m_message(message)
		, m_box(box)
		, m_bitmap(bitmap)
	{
		stop_buffer = true;
		stop_buffer_code = 999;
	}

	ScreenEmergency::~ScreenEmergency()
	{ }

	void ScreenEmergency::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);

			Area bitmap_area(0, 14, 39, 54);
			painter.setWorkingArea(bitmap_area);

			uint8_t x_offset = 12;
			uint8_t y_offset = 9;

			Area temp_area = painter.getWorkingArea();
			painter.drawBitmap(temp_area.x_init + x_offset, temp_area.y_init + y_offset, emergency_width, emergency_height, bits_emergency);

			Area text_area(40, 14, 127, 54);
			painter.setWorkingArea(text_area);
			painter.multiText(m_message);
		} while (painter.nextPage());
	}
}