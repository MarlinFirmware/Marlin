#include "ScreenEmergency.h"
#include "GuiBitmaps_witbox_2.h"

namespace screen
{
	ScreenEmergency::ScreenEmergency(const char * title, const char * message, const char * box, const unsigned char * bitmap)
		: Screen(title, SIMPLE)
		, m_message(message)
		, m_box(box)
		, m_bitmap(bitmap)
	{ }

	ScreenEmergency::~ScreenEmergency()
	{ }

	void ScreenEmergency::draw()
	{
		while (1)
		{
			painter.firstPage();
			do
			{
				painter.title(m_title);
				painter.box(m_box);

				uint8_t x_init = painter.coordinateXInit();
				uint8_t y_init = painter.coordinateYInit();
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_end = painter.coordinateYEnd();

				uint8_t x_offset = 12;
				uint8_t y_offset = 9;

				painter.setColorIndex(1);
				painter.drawBitmap(x_init + x_offset, y_init + y_offset, emergency_width, emergency_height, bits_emergency);

				painter.setPrintPos(x_offset + logo_width + 8, y_init + 8);
				painter.multiText(m_message);
			} while (painter.nextPage());
		}
	}
}