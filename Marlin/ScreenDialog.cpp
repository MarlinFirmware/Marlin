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

	Screen & ScreenDialog::press()
	{
		return * m_next_screen;
	}

	void ScreenDialog::add(Screen & component)
	{
		m_next_screen = &component;
	}
}
