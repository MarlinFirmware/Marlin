#include "Screen.h"

namespace screen
{
	Screen::Screen(const char * title, ScreenType_t const & type)
		: m_title(title)
		, m_type(type)
	{ }

	Screen::~Screen()
	{ }

	const char * Screen::title() const
	{
		return m_title;
	}

	Screen::ScreenType_t const & Screen::type() const
	{
		return m_type;
	}

   void Screen::add(Screen & component)
   {
      m_next_screen = &component;
   }

	void Screen::icon(Icon & component)
	{
		m_icon = &component;
	}

	Icon & Screen::icon()
	{
		return * m_icon;
	}

	void Screen::press()
	{ }
}
