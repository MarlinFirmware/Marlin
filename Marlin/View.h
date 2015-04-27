#ifndef VIEW_H
#define VIEW_H

#include <stdint.h>

#include "Screen.h"

namespace screen
{
	class View
	{
		public:
			View(Screen & start_screen);
			~View();

		protected:
			Screen * m_start_screen;
			Screen * m_parent_screen;
			Screen * m_active_screen;
	};
}
#endif //VIEW_H
