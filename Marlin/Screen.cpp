#include "Screen.h"

namespace screen
{
	Screen::Screen(const char * label, ScreenType_t const & type)
		: m_label(label)
		, m_type(type)
	{
	}

	Screen::~Screen()
	{
	}

	const char * Screen::label() const
	{
		return m_label;
	}

	ScreenType_t const & Screen::type() const
	{
		return m_type;
	}
}
