#include "GuiPainter.h"
#include "GuiBitmaps_witbox_2.h"

#include <avr/pgmspace.h>

#include "PrintManager.h"

extern "C" void	atexit( void ) { }

namespace screen
{
	GuiPainter::GuiPainter()
		: m_impl(0)
		, m_x_init(0)
		, m_y_init(0)
		, m_x_end(screen_width)
		, m_y_end(screen_height)
	{ }

	GuiPainter::~GuiPainter()
	{ }

	void GuiPainter::title(const char * title)
	{
		if ( (title != NULL) && (strlen_P(title) > 0) )
		{
			uint8_t x_init = coordinateXInit();
			uint8_t y_init = coordinateYInit();
			uint8_t x_end = coordinateXEnd();

			setColorIndex(1);
			setFont(u8g_font_6x9);
			setPrintPos(x_init, y_init + 3);
			print_P(title);
			drawLine(x_init, y_init + 13, x_end, y_init + 13);

			coordinateYInit(14);
		}
	}

	void GuiPainter::text(const char * msg, uint8_t v_pad)
	{
		if ( (msg != NULL) && (strlen(msg) > 0) )
		{
			uint8_t x_init = coordinateXInit();
			uint8_t y_init = coordinateYInit() + v_pad;
			uint8_t x_end = coordinateXEnd();

			setColorIndex(1);
			setFont(u8g_font_6x9);
			setPrintPos(x_end/2 - (strlen(msg)*6)/2, y_init);
			print(msg);

			coordinateYInit(y_init + 10);
		}
	}

	void GuiPainter::text_P(const char * msg)
	{
		if ( (msg != NULL) && (strlen_P(msg) > 0) )
		{
			uint8_t x_init = coordinateXInit();
			uint8_t y_init = coordinateYInit() + 3;
			uint8_t x_end = coordinateXEnd();

			setColorIndex(1);
			setFont(u8g_font_6x9);
			setPrintPos(x_end/2 - (strlen_P(msg)*6)/2, y_init + 2);
			print_P(msg);

			coordinateYInit(y_init + 12);
		}
	}

	void GuiPainter::printing_status(const uint8_t percentage, const uint16_t time)
	{
		uint8_t x_init = coordinateXInit();
		uint8_t y_init = coordinateYInit() + 5;
		uint8_t x_end = coordinateXEnd();
		m_impl.drawXBMP(x_init, y_init, little_icon_width, little_icon_height, bits_sd_small);
		setColorIndex(1);
		setFont(u8g_font_6x9);
		setPrintPos(x_init + 7, y_init);
		print(itostr3left(percentage));
		print("%");
		coordinateXInit(x_init + (strlen(itostr3left(percentage))+strlen("%"))*6 + 7+1);
		x_init = coordinateXInit();

		if (PrintManager::getInstance().state() == PRINTING)
		{
			setColorIndex(1);
			setFont(u8g_font_6x9);
			uint8_t x = x_end - (strlen(itostr2(time/60)) + strlen(":") + strlen(itostr2(time%60))) * 6;
			setPrintPos(x, y_init);
			print(itostr2(time/60));
			print(":");
			print(itostr2(time%60));

			coordinateXEnd(x-2);
			x_end = coordinateXEnd();
		}
		setColorIndex(1);
		m_impl.drawBox(x_init, y_init + 1,x_end-x_init,  6);
		setColorIndex(0);
		m_impl.drawBox(x_init + 1, y_init + 2,x_end-x_init -2, 4);
		setColorIndex(1);
		m_impl.drawBox(x_init + 2, y_init + 3,(x_end-x_init - 4) * percentage/100, 2);

		coordinateXInit(0);
		coordinateXEnd(screen_width);
		coordinateYInit(y_init + 9);
	}

	void GuiPainter::selector(uint16_t number, uint16_t total)
	{
		uint8_t x_init = coordinateXInit();
		uint8_t x_end = coordinateXEnd();
		uint8_t y_init = coordinateYInit();
		uint8_t y_end = coordinateYEnd();

		setColorIndex(1);
		setFont(u8g_font_6x9);
		setPrintPos((x_end + x_init)/2 - (strlen("< / >")*6)/2 - (strlen(itostr2(number))*6)/2 - (strlen(itostr2(total))*6)/2, (y_end + y_init)/2 - 9/2);
		print ("< ");
		print(itostr2(number));
		print("/");
		print(itostr2(total));
		print (" >");
	}

	void GuiPainter::box(const char* nextScreen)
	{
		uint8_t x_init = coordinateXInit();
		uint8_t x_end = coordinateXEnd();
		uint8_t y_end = coordinateYEnd();

		//Print box
		setColorIndex(1);
		m_impl.drawBox(x_init, y_end - 9, x_end, y_end - 9);
		//Set font and color
		setFont(u8g_font_6x9);
		setColorIndex(0);
		//Print arrows
		setPrintPos(2, y_end - 9);
		print("<");
		setPrintPos(x_end-7, y_end - 9);
		print(">");
		//Print text label
		setPrintPos(x_end/2 - (strlen_P(nextScreen)*6)/2, y_end - 9);
		print_P(nextScreen);
		coordinateYEnd(51);
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
		m_impl.setFontPosTop();
		m_impl.setPrintPos(x, y);
	}

	void GuiPainter::print(const char * text)
	{
		m_impl.print(text);
	}

	void GuiPainter::print_P(const char * text)
	{
		char c;
		while ((c = pgm_read_byte(text)) != '\0')
		{
			m_impl.print(c);
			text++;
		}
	}

	void GuiPainter::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
	{
		m_impl.drawLine(x1, y1, x2, y2);
	}

	void GuiPainter::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
	{
		m_impl.drawBox(x, y, w, h);
	}

	void GuiPainter::drawBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const unsigned char* bitmap)
	{
		m_impl.drawXBMP(x, y, width, height, bitmap);
	}

	void GuiPainter::firstPage()
	{
		m_impl.firstPage();
	}

	bool GuiPainter::nextPage()
	{
		coordinateXInit(0);
		coordinateYInit(0);
		coordinateXEnd(screen_width);
		coordinateYEnd(screen_height);
		m_impl.nextPage();
	}

	void GuiPainter::coordinateXInit(uint8_t coordinate)
	{
		m_x_init = coordinate;
	}

	uint8_t GuiPainter::coordinateXInit()
	{
		return m_x_init;
	}

	void GuiPainter::coordinateYInit(uint8_t coordinate)
	{
		m_y_init = coordinate;
	}

	uint8_t GuiPainter::coordinateYInit()
	{
		return m_y_init;
	}

	void GuiPainter::coordinateXEnd(uint8_t coordinate)
	{
		m_x_end = coordinate;
	}

	uint8_t GuiPainter::coordinateXEnd()
	{
		return m_x_end;
	}

	void GuiPainter::coordinateYEnd(uint8_t coordinate)
	{
		m_y_end = coordinate;
	}

	uint8_t GuiPainter::coordinateYEnd()
	{
		return m_y_end;
	}

char conv[8];

char * GuiPainter::itostr2(const int &xx)
{
    if (xx >= 100)
    {
    	conv[0]=(xx/100)%10+'0';
        conv[1]=(xx/10)%10+'0';
        conv[2]=(xx)%10+'0';
        conv[3]=0;
    }
    else
    {
    	conv[0]=(xx/10)%10+'0';
    	conv[1]=(xx)%10+'0';
    	conv[2]=0;
    }
    return conv;
}

char * GuiPainter::itostr3left(const int &xx)
{
    if (xx >= 100) {
        conv[0]=(xx/100)%10+'0';
        conv[1]=(xx/10)%10+'0';
        conv[2]=(xx)%10+'0';
        conv[3]=0;
    } else if (xx >= 10) {
        conv[0]=(xx/10)%10+'0';
        conv[1]=(xx)%10+'0';
        conv[2]=0;
    } else {
        conv[0]=(xx)%10+'0';
        conv[1]=0;
    }
    return conv;
}
}
