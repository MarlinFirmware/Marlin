#include "ScreenSelector.h"

namespace screen
{
	ScreenSelector::ScreenSelector(const char * label)
		: Screen(label, SELECTOR)
	{
	}

	ScreenSelector::~ScreenSelector()
	{
	}

	void ScreenSelector::left()
	{
	}

	void ScreenSelector::right()
	{
	}

	void ScreenSelector::draw()
	{
	}

	Screen & ScreenSelector::press(void (*action)(void))
	{
		return * this;
	}
}
