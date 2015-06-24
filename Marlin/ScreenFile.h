#ifndef SCREEN_FILE_H
#define SCREEN_FILE_H

#include <stdint.h>

#include "ScreenMenu.h"

namespace screen
{
	class ScreenFile : public ScreenMenu
	{
		public:
			ScreenFile(const char * title = 0);
			virtual ~ScreenFile();

			virtual void draw();
	};
}

#endif //SCREEN_FILE_H
