#ifndef SCREEN_STATUS_H
#define SCREEN_STATUS_H


#include <stdint.h>
#include "Screen.h"

namespace screen
{
	class ScreenStatus : public Screen
	{
		public:
			ScreenStatus(const char * title = 0, FuncPtr_t function = do_nothing);
			virtual ~ScreenStatus();

			void icon(Icon & component);
			Icon & icon();

		private:
			Icon * m_icon_alternate;
			uint8_t m_icon_index;
	};
}
#endif //SCREEN_STATUS_H
