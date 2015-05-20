#include "Icon.h"
#include "GuiPainter.h"

namespace screen
{
	Icon::Icon (IconType_t const & type, Size const & size, const unsigned char* bitmap)
		: m_type(type)
		, m_size(size)
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
		return m_size.width;
	}

	uint8_t const & Icon::height() const
	{
		return m_size.height;
	}

	void Icon::draw(uint8_t x, uint8_t y)
	{
		painter.icon(x, y, m_size.width, m_size.height, m_bitmap);
	}
}
