#ifndef SCREEN_COMPLETE_H
#define SCREEN_COMPLETE_H

#include "ScreenDialog.h"

#include "PrintManager.h"

namespace screen
{
	class ScreenComplete : public ScreenDialog<void>
	{
		public:
			ScreenComplete(const char * title, const char * message,  const char * box, Time_t time);
			virtual ~ScreenComplete();

			void draw();

		private:
			Time_t m_time;
	};
}

#endif // SCREEN_COMPLETE_H