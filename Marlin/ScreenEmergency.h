#ifndef SCREEN_EMERGENCY_H
#define SCREEN_EMERGENCY_H

#include "Screen.h"

namespace screen
{
	class ScreenEmergency : public Screen
	{
		public:
			ScreenEmergency(const char * title, const char * message, const char * box, const unsigned char * bitmap);
			~ScreenEmergency();

			void draw();

		private:
			const char * m_box;
			const char * m_message;

			const unsigned char * m_bitmap;
	};
}

#endif // SCREEN_EMERGENCY_H