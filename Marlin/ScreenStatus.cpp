#include "ScreenStatus.h"

namespace screen
{
	ScreenStatus::ScreenStatus(const char * title, FuncPtr_t function)
		: Screen(title, STATUS, function)
	{ }

	ScreenStatus::~ScreenStatus()
	{ }

}
