#include "ScreenEmergency.h"

#include "GuiPainter.h"

namespace screen
{
	ScreenEmergency::ScreenEmergency(const char * title, const char * message, const char * box)
		: Screen(title, SIMPLE)
		, m_message(message)
		, m_box(box)
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

				Area text_area(40, 14, 127, 54);
				painter.setWorkingArea(text_area);
				painter.multiText(m_message);
			} while (painter.nextPage());
		}
	}
}