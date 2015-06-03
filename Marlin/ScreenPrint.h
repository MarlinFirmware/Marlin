#ifndef SCREEN_PRINT_H
#define SCREEN_PRINT_H

#include <stdint.h>

#include "ScreenMenu.h"

namespace screen
{
	class ScreenPrint : public ScreenMenu
	{
		public:
			ScreenPrint(const char * title = 0);
			virtual ~ScreenPrint();

			void draw();

		private:
	};
}

#endif //SCREEN_PRINT_H
