#include "GuiPainter.h"

extern "C" void	atexit( void ) { }

namespace screen
{
	GuiPainter::GuiPainter()
		: m_impl(0)
		, m_x_init(0)
		, m_y_init(0)
		, m_x_end(screen_width)
		, m_y_end(screen_height)
	{
	}

	GuiPainter::~GuiPainter()
	{}

	void GuiPainter::drawXBMP(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const unsigned char* bitmap)
	{
		m_impl.drawXBMP(x, y, width, height, bitmap);
	}

	void GuiPainter::setColorIndex(uint8_t color)
	{
		m_impl.setColorIndex(color);
	}

	void GuiPainter::setFont(const u8g_fntpgm_uint8_t* font)
	{
		m_impl.setFont(font);
	}

	void GuiPainter::setPrintPos(uint8_t x, uint8_t y)
	{
		m_impl.setPrintPos(x, y);
	}

	void GuiPainter::print(const char * title)
	{
		m_impl.print(title);
	}

	void GuiPainter::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
	{
		m_impl.drawLine(x1, y1, x2, y2);
	}

	void GuiPainter::drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
	{
		m_impl.drawBox(x, y, width, height);
	}

	void GuiPainter::firstPage()
	{
		m_impl.firstPage();
	}

	bool GuiPainter::nextPage()
	{
		m_impl.nextPage();
	}
}
