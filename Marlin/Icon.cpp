#include "Icon.h"

namespace screen
{
	Icon::Icon (uint8_t width, uint8_t height, const unsigned char* bitmap)
		: m_width(width)
		, m_height(height)
		, m_bitmap(bitmap)
		{ }

}
