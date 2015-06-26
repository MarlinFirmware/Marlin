#ifndef SCREEN_PRINT_H
#define SCREEN_PRINT_H

#include <stdint.h>

#include "ScreenMenu.h"
#include "PrintManager.h"


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
			Time_t m_printed_time;
			uint8_t m_percent_done;
			uint16_t m_target_temperature;
			PrinterState_t m_printing_status;
	};
}

#endif //SCREEN_PRINT_H
