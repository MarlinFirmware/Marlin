#include "ScreenEmergency.h"

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
				painter.multiText(m_message);
			} while (painter.nextPage());
		}
	}
}