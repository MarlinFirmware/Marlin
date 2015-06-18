#ifndef SCREEN_ABOUT_H
#define SCREEN_ABOUT_H

#include <stdint.h>

#include "Screen.h"

namespace screen
{
	class ScreenAbout : public Screen
	{
		public:
			ScreenAbout(const char * title = 0, const char * text = 0, const char * box = 0, const unsigned char * bitmap = 0);
			virtual ~ScreenAbout();

			void press();
			virtual void draw();

		protected:
			const char * m_text;
			const char * m_box;
			const unsigned char * m_bitmap;
	};
}

#endif //SCREEN_ABOUT_H
