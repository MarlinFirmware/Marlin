#include "ScreenStatus.h"

namespace screen
{
	ScreenStatus::ScreenStatus(const char * title, FuncPtr_t function)
		: Screen(title, STATUS, function)
		, m_icon_index(0)
	{ }

	ScreenStatus::~ScreenStatus()
	{ }

   void ScreenStatus::icon(Icon & component)
   {
		if (m_icon_index < 1)
		{
			m_icon = &component;
		}
		else
		{
			m_icon_alternate = &component;
		}
		++m_icon_index;
   }

   Icon & ScreenStatus::icon()
   {
      return * m_icon;
   }
}
