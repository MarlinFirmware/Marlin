#ifndef SCREEN_STOP_H
#define SCREEN_STOP_H

#include "ScreenDialog.h"

#include "PrintManager.h"

namespace ui
{
	class ScreenStop : public ScreenDialog<void>
	{
		public:
			ScreenStop(const char * title, const char * message,  const char * box, float height, Time_t time);
			virtual ~ScreenStop();

			void draw();

		private:
			float m_height;
			Time_t m_time;
	};
}

#endif // SCREEN_STOP_H