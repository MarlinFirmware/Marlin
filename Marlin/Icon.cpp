#include "Icon.h"
#include "GuiPainter.h"

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

	void Icon::draw(uint8_t x, uint8_t y)
	{
		painter.drawXBMP(x, y, m_width, m_height, m_bitmap);
	}
}
