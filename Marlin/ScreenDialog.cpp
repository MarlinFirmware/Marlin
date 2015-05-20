#include "ScreenDialog.h"

namespace screen
{
	ScreenDialog::ScreenDialog(const char * title)
		: Screen(title, DIALOG)
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
	}
}
