#ifndef SCREEN_STATUS_H
#define SCREEN_STATUS_H


#include <stdint.h>
#include "Screen.h"

namespace screen
{
	class ScreenStatus : public Screen
	{
		public:
			ScreenStatus(const char * title = 0, FuncPtr_t function = do_nothing);
			virtual ~ScreenStatus();


		private:
	};
}
#endif //SCREEN_STATUS_H
