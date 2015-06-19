#ifndef SCREEN_ABOUT_H
#define SCREEN_ABOUT_H

#include <stdint.h>

#include "ScreenDialog.h"

namespace screen
{
	class ScreenAbout : public ScreenDialog<void>
	{
		public:
			ScreenAbout(const char * title = 0, const char * message = 0, const char * box = 0, const unsigned char * bitmap = 0);
			virtual ~ScreenAbout();

			virtual void draw();

		protected:
			const unsigned char * m_bitmap;
	};
}

#endif //SCREEN_ABOUT_H
