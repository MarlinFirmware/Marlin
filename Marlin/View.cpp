//#include <iostream>

#include "View.h"

namespace screen
{
	View::View(Screen & start_screen)
		: m_start_screen(&start_screen)
	{
//		std::cout << "View::View(" << start_screen->label() << ")" << std::endl;
		m_active_screen = m_start_screen;
	}

	View::~View()
	{
	}
}
