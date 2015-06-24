#ifndef SCREEN_PRINT_H
#define SCREEN_PRINT_H

#include <stdint.h>

#include "ScreenMenu.h"

namespace screen
{
	class ScreenPrint : public ScreenMenu , public Observer<float>
	{
		public:
			ScreenPrint(const char * title = 0, Subject<float> * model = 0);
			virtual ~ScreenPrint();

			void init();
			
			virtual void draw();

			void update(float value);
		
		private:
			float m_observed;
			uint16_t m_actual_time;
			uint8_t m_percent_done;
	};
}

#endif //SCREEN_PRINT_H
