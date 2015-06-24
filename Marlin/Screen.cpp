#include "Screen.h"

namespace screen
{
	Screen::Screen(const char * title, ScreenType_t const & type)
		: m_title(title)
		, m_type(type)
		, m_needs_drawing(true)
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

   void Screen::add(ScreenIndex_t const & view)
   {
      m_next_screen = view;
   }
}
