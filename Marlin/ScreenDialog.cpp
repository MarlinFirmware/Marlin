#include "ScreenDialog.h"

namespace screen
{
	ScreenDialog::ScreenDialog(const char * title, const char * box)
		: Screen(title, DIALOG)
		, m_box(box)
	{
	}

	ScreenDialog::~ScreenDialog()
	{
	}

	void ScreenDialog::left()
	{
	}

	void ScreenDialog::right()
	{
	}

	void ScreenDialog::draw()
	{
		SERIAL_ECHO("DRAW: ");
		SERIAL_ECHOLN(m_title);

		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

		} while ( painter.nextPage() );
	}
}
