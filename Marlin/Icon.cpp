#include "Icon.h"

namespace screen
{
	Icon::Icon (IconType_t const & type, uint8_t width, uint8_t height, const unsigned char* bitmap)
		: m_type(type)
		  , m_width(width)
		  , m_height(height)
		  , m_bitmap(bitmap)
	{ }

	Icon::~Icon()
	{ }

	Icon::IconType_t const & Icon::type() const
	{
		return m_type;
	}

	uint8_t const & Icon::width() const
	{
		return m_width;
	}

	uint8_t const & Icon::height() const
	{
		return m_height;
	}
}
