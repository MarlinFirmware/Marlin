#include "Icon.h"
#include "GuiPainter.h"

namespace screen
{
	Icon::Icon (Size const & size, const unsigned char* bitmap, const unsigned char * focused_bitmap, const char * text)
		: m_size(size)
		, m_bitmap(bitmap)
		, m_focused_bitmap(focused_bitmap)
		, m_text(text)
	{ }

	Icon::~Icon()
	{ }

	uint8_t const & Icon::width() const
	{
		return m_size.width;
	}

	uint8_t const & Icon::height() const
	{
		return m_size.height;
	}

	const char * Icon::text() const
	{
		return m_text;
	}

	void Icon::draw(uint8_t x, uint8_t y, bool focused)
	{
		painter.setColorIndex(1);
		painter.drawBitmap(x, y, m_size.width, m_size.height, (focused) ? m_focused_bitmap : m_bitmap);
	}

	void Icon::show()
	{ }
}
