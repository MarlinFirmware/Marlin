#ifndef SCREEN_EMERGENCY_H
#define SCREEN_EMERGENCY_H

#include "Screen.h"

namespace screen
{
	class ScreenEmergency : public Screen
	{
		public:
			ScreenEmergency(const char * title, const char * message, const char * box);
			~ScreenEmergency();

			void draw();

		public:
			const char * m_box;
			const char * m_message;
	};
}

#endif // SCREEN_EMERGENCY_H