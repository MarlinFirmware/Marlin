#include "ScreenDialog.h"

namespace screen
{
	ScreenDialog::ScreenDialog(const char * label)
		: Screen(label, DIALOG)
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
