#include "GuiPainter.h"
#include "GuiBitmaps_witbox_2.h"

#include <avr/pgmspace.h>

#include "PrintManager.h"

namespace screen
{
	GuiPainter::GuiPainter()
		: m_impl(0)
	{
		m_working_area = Area();
	}

	GuiPainter::~GuiPainter()
	{ }

	void GuiPainter::begin()
	{
		m_impl.begin();
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

	void GuiPainter::clearWorkingArea()
	{
		m_working_area = Area();
	}

	void GuiPainter::setWorkingArea(Area a)
	{
		m_working_area = a;
	}

	GuiPainter::Area GuiPainter::getWorkingArea()
	{
		return m_working_area;
	}


	void GuiPainter::title(const char * title)
	{
		if ( (title != NULL) && (strlen_P(title) > 0) )
		{
			uint8_t save_color_index = m_impl.getColorIndex();

			setColorIndex(1);
			setFont(u8g_font_6x9);
			setPrintPos(0, 3);
			print_P(title);

			drawLine(0, title_height - 1, title_width - 1, title_height - 1);

			coordinateYInit(14);
			setColorIndex(save_color_index);
		}
	}

	void GuiPainter::box(const char* text)
	{
		uint8_t save_color_index = m_impl.getColorIndex();

		//Print box
		setColorIndex(1);
		m_impl.drawBox(0, ((screen_height - 1) - (box_height - 1)), (box_width - 1), screen_height - 1);

		//Set font and color
		setFont(u8g_font_6x9);
		setColorIndex(0);

		//Print text label
		setPrintPos( (screen_width - strlen_P(text) * max_font_width) / 2, ((screen_height - 1) - (max_font_height - 1)));
		print_P(text);

		coordinateYEnd(51);
		setColorIndex(save_color_index);
	}

	void GuiPainter::arrowBox(const char* text)
	{
		uint8_t save_color_index = m_impl.getColorIndex();

		//Print box
		setColorIndex(1);
		m_impl.drawBox(0, ((screen_height - 1) - (box_height - 1)), (box_width - 1), screen_height - 1);

		//Set font and color
		setFont(u8g_font_6x9);
		setColorIndex(0);

		//Print arrows
		setPrintPos(2, ((screen_height - 1) - (max_font_height - 1)));
		print("<");
		setPrintPos(((screen_width - 1) - 2 - (max_font_width - 1)), ((screen_height - 1) - (max_font_height - 1)));
		print(">");

		//Print text label
		setPrintPos( (screen_width - strlen_P(text) * max_font_width) / 2, ((screen_height - 1) - (max_font_height - 1)));
		print_P(text);

		coordinateYEnd(51);
		setColorIndex(save_color_index);
	}

	void GuiPainter::printingStatus(const uint8_t percentage, const uint8_t hour, const uint8_t minute)
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

		//Print the elapsed time on the right of the progress bar
		setColorIndex(1);
		setFont(u8g_font_6x9);
		uint8_t x = x_end - (strlen(itostr2(hour)) + strlen(":") + strlen(itostr2(minute))) * 6;
		setPrintPos(x, y_init);
		print(itostr2(hour));
		print(":");
		print(itostr2(minute));

		coordinateXEnd(x-2);
		x_end = coordinateXEnd();

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

	void GuiPainter::text(const char * msg, uint8_t h_pad, uint8_t v_pad)
	{
		if ( (msg != NULL) && (strlen(msg) > 0) )
		{
			uint8_t x_init = coordinateXInit();
			uint8_t y_init = coordinateYInit() + v_pad;
			uint8_t x_end = coordinateXEnd();

			setColorIndex(1);
			setFont(u8g_font_6x9);
			setPrintPos(h_pad + x_end/2 - (strlen(msg)*6)/2, y_init);
			print(msg);

			coordinateYInit(y_init + 10);
		}
	}

	void GuiPainter::text_P(const char * msg, uint8_t h_pad, uint8_t v_pad)
	{
		if ( (msg != NULL) && (strlen_P(msg) > 0) )
		{
			uint8_t x_init = coordinateXInit();
			uint8_t y_init = coordinateYInit() + v_pad;
			uint8_t x_end = coordinateXEnd();

			setColorIndex(1);
			setFont(u8g_font_6x9);
			setPrintPos(h_pad + x_end/2 - (strlen_P(msg)*6)/2, y_init);
			print_P(msg);

			coordinateYInit(y_init + 10);
		}
	}

	void GuiPainter::multiText(const char * msg, bool align_top)
	{
		if ( (msg != NULL) && (strlen_P(msg) > 0) )
		{
			const int MAX_LINE_CHARS = 20;
			int n_words = 0;
			int n_lines = 1;
			int total_lines = 1;

			char * buffer = (char *) malloc (1 + strlen_P(msg));
			char * word = (char *) malloc (MAX_LINE_CHARS);
			char * safeBuffer = buffer;
			char * safeWord = word;
			char phrase[64] = "";

			if (buffer)
			{
				memset(phrase, 0, 64);

				strcpy_P(buffer, msg);

				//First pass to get number of lines
				do
				{
					word = strsep(&buffer, " ");

					if ( (strlen(phrase) + strlen(word) <= MAX_LINE_CHARS) && (word == NULL) )
					{
						//Last line. Do nothing
					}
					else if ( (strlen(phrase) + strlen(word) <= MAX_LINE_CHARS) && (word != NULL) )
					{
						strcat(phrase, word);
						strcat(phrase, " ");
					}
					else 
					{
						memset(phrase, 0, 64);
						strcpy(phrase, word);
						strcat(phrase, " ");
						total_lines++;
					}
				}
				while ( word != NULL );

				buffer = safeBuffer;
				word = safeWord;
				memset(phrase, 0, 64);

				strcpy_P(buffer, msg);

				//Second pass to print
				do
				{
					word = strsep(&buffer, " ");

					if ( (strlen(phrase) + strlen(word) <= MAX_LINE_CHARS) && (word == NULL) )
					{
						phrase[strlen(phrase) - 1] = '\0';
						switch (total_lines)
						{
							case 1:
								if(align_top)
								{
									text(phrase, 0, 3);
								}
								else
								{
									text(phrase, 0, 16);
								}
								break;
							case 2:
								if(n_lines == 1)
								{
									if(align_top)
									{
										text(phrase, 0, 3);
									}
									else
									{
										text(phrase, 0, 10);
									}
								}
								else
								{
									text(phrase, 0, 0);
								}
								break;
							case 3:
								if(n_lines == 1)
								{
									if(align_top)
									{
										text(phrase, 0, 3);
									}
									else
									{
										text(phrase, 0, 6);
									}
								}
								else
								{
									text(phrase, 0, 0);
								}
								break;
							case 4:
								text(phrase, 0, 0);
								break;
							default:
								break;
						}
					}
					else if ( (strlen(phrase) + strlen(word) <= MAX_LINE_CHARS) && (word != NULL) )
					{
						strcat(phrase, word);
						strcat(phrase, " ");
					}
					else
					{
						phrase[strlen(phrase) - 1] = '\0';
						switch (total_lines)
						{
							case 1:
								if(align_top)
								{
									text(phrase, 0, 3);
								}
								else
								{
									text(phrase, 0, 16);
								}
								break;
							case 2:
								if(n_lines == 1)
								{
									if(align_top)
									{
										text(phrase, 0, 3);
									}
									else
									{
										text(phrase, 0, 10);
									}
								}
								else
								{
									text(phrase, 0, 0);
								}
								break;
							case 3:
								if(n_lines == 1)
								{
									if(align_top)
									{
										text(phrase, 0, 3);
									}
									else
									{
										text(phrase, 0, 6);
									}
								}
								else
								{
									text(phrase, 0, 0);
								}
								break;
							case 4:
								text(phrase, 0, 0);
								break;
							default:
								break;
						}
						memset(phrase, 0, 64);
						strcpy(phrase, word);
						strcat(phrase, " ");
						n_lines++;
					}
				}
				while ( word != NULL );
			}
			free(safeWord);
			free(safeBuffer);
		}
	}

	void GuiPainter::setFont(const u8g_fntpgm_uint8_t* font)
	{
		m_impl.setFont(font);
	}

	void GuiPainter::setColorIndex(uint8_t color)
	{
		m_impl.setColorIndex(color);
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
