///////////////////////////////////////////////////////////////////////////////
/// \file GuiPainter.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Painting interface class to U8glib
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or 
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#include "GuiPainter.h"
#include "GuiBitmaps_witbox_2.h"

#include <avr/pgmspace.h>

#include "PrintManager.h"
#include "Language.h"

namespace ui
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
		clearWorkingArea();
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

	Area GuiPainter::getWorkingArea()
	{
		return m_working_area;
	}


	void GuiPainter::title(const char * title)
	{
		if ( (title != NULL) && (strlen_P(title) > 0) )
		{
			uint8_t save_color_index = m_impl.getColorIndex();

			setColorIndex(1);
			setFont(FontType_t::BODY_FONT);
			setPrintPos(0, 3);
			print_P(title);

			drawLine(0, title_height - 1, title_width - 1, title_height - 1);

			coordinateYInit(14);
			setColorIndex(save_color_index);
		}
	}

	void GuiPainter::box(const char* text, ArrowType_t arrow)
	{
		uint8_t save_color_index = m_impl.getColorIndex();

		//Print box
		setColorIndex(1);
		m_impl.drawBox(0, ((screen_height - 1) - (box_height - 1)), box_width, box_height);

		//Set font and color
		setFont(FontType_t::BODY_FONT);
		setColorIndex(0);

		//Print arrows
		switch (arrow)
		{
			case LEFT:
				setPrintPos(2, ((screen_height - 1) - (max_font_height - 1)));
				print("<");
				break;

			case RIGHT:
				setPrintPos(((screen_width - 1) - 2 - (max_font_width - 1)), ((screen_height - 1) - (max_font_height - 1)));
				print(">");
				break;

			case BOTH:
				setPrintPos(2, ((screen_height - 1) - (max_font_height - 1)));
				print("<");
				setPrintPos(((screen_width - 1) - 2 - (max_font_width - 1)), ((screen_height - 1) - (max_font_height - 1)));
				print(">");
				break;

			case NONE:
				break;
		}

		//Print text label
		if ( (text != NULL) && (strlen_P(text) > 0) )
		{
			setPrintPos( (screen_width - strlen_P(text) * max_font_width) / 2, ((screen_height - 1) - (max_font_height - 1)));
			print_P(text);
		}

		coordinateYEnd(51);
		setColorIndex(save_color_index);
	}

	void GuiPainter::printingStatus(const uint8_t percentage, bool progress_bar)
	{
		Area save_working_area = m_working_area;
		uint8_t save_color_index = m_impl.getColorIndex();

		Area status_area(0, 19, 95, 25);
		setWorkingArea(status_area);
		setColorIndex(1);
		setFont(FontType_t::BODY_FONT);

		// Draw SD small icon
		drawBitmap(m_working_area.x_init, m_working_area.y_init, little_icon_width, little_icon_height, bits_sd_small);

		// Draw the percentage done
		m_working_area.x_init += little_icon_width + 4;
		char s_percentage[4] = { 0 };
		strcpy(s_percentage, itostr3left(percentage));
		setPrintPos(m_working_area.x_init, m_working_area.y_init);
		print(s_percentage);
		print("%");

		if(progress_bar == true)
		{

			// Draw the progress bar
			m_working_area.x_init += (strlen(s_percentage) + 1) * max_font_width + 1;
			m_working_area.y_init += 1;
			m_impl.drawBox(m_working_area.x_init, m_working_area.y_init, m_working_area.width(), m_working_area.height());

			setColorIndex(0);
			m_working_area.x_init += 1;
			m_working_area.x_end -= 1;
			m_working_area.y_init += 1;
			m_working_area.y_end -= 1;
			m_impl.drawBox(m_working_area.x_init, m_working_area.y_init, m_working_area.width(), m_working_area.height());

			setColorIndex(1);
			m_working_area.x_init += 1;
			m_working_area.x_end -= 1;
			m_working_area.y_init += 1;
			m_working_area.y_end -= 1;
			m_impl.drawBox(m_working_area.x_init, m_working_area.y_init, m_working_area.width() * percentage / 100, m_working_area.height());
		}

		setWorkingArea(save_working_area);
		setColorIndex(save_color_index);
	}

	void GuiPainter::text(const char * msg)
	{
		if ( (msg != NULL) && (strlen(msg) > 0) )
		{
			setColorIndex(1);
			setFont(FontType_t::BODY_FONT);
			setPrintPos(m_working_area.x_init + (m_working_area.width() / 2) - (strlen(msg) * max_font_width / 2), m_working_area.y_init);
			print(msg);
		}
	}

	void GuiPainter::text_P(const char * msg)
	{
		if ( (msg != NULL) && (strlen_P(msg) > 0) )
		{
			setColorIndex(1);
			setFont(FontType_t::BODY_FONT);
			setPrintPos(m_working_area.x_init + (m_working_area.width() - strlen_P(msg) * max_font_width) / 2, m_working_area.y_init);
			print_P(msg);
		}
	}

	void GuiPainter::multiText(const char * msg)
	{
		if ( (msg != NULL) && (strlen(msg) > 0) )
		{
			Area save_working_area = m_working_area;

			uint8_t max_chars_per_line = m_working_area.width() / max_font_width;
			uint8_t max_num_lines = m_working_area.height() / max_font_height;

			uint8_t chars_per_line[max_num_lines];

			uint8_t num_line = 0;
			uint8_t total_num_lines = 1;

			uint8_t msg_length = strlen(msg) + 1;

			char * text_safe_buffer = (char *) malloc (msg_length);
			char * text_buffer = text_safe_buffer;

			char * word_safe_buffer = (char *) malloc (msg_length);
			char * word_buffer = word_safe_buffer;

			char * line_safe_buffer = (char *) malloc (msg_length);
			char * line_buffer = line_safe_buffer;

			if ( (text_buffer != NULL) && (word_buffer != NULL) && (line_buffer != NULL) )
			{
				memset(text_buffer, 0, msg_length);
				memset(word_buffer, 0, msg_length);
				memset(line_buffer, 0, msg_length);

				strcpy(text_buffer, msg);

				//First pass to get number of lines
				word_buffer = strsep(&text_buffer, " ");

				while ( (word_buffer != NULL) && (total_num_lines <= max_num_lines) )
				{
					// The current line is not full yet.
					if (strlen(line_buffer) + strlen(word_buffer) <= max_chars_per_line )
					{
						strcat(line_buffer, word_buffer);
						strcat(line_buffer, " ");
					}
					// The current line is full.
					else
					{

						line_buffer[strlen(line_buffer) - 1] = '\0';
						chars_per_line[num_line] = strlen(line_buffer);

						num_line++;
						if(total_num_lines < 4)
						{
							total_num_lines++;
						}
						memset(line_buffer, 0, msg_length);

						strcat(line_buffer, word_buffer);
						strcat(line_buffer, " ");
					}

					word_buffer = strsep(&text_buffer, " ");
				}

				line_buffer[strlen(line_buffer) - 1] = '\0';
				chars_per_line[num_line] = strlen(line_buffer);

				// Calculate the working area initial position
				uint8_t initial_position = m_working_area.y_init + (m_working_area.height() / 2) - (total_num_lines * max_font_height / 2);

				text_buffer = text_safe_buffer;
				strcpy(text_buffer, msg);

				Area line_area = getWorkingArea();
				line_area.y_init = initial_position;
				line_area.y_end = initial_position + max_font_height;
				setWorkingArea(line_area);

				for (uint8_t i = 0; i < total_num_lines; i++)
				{
					char line[max_chars_per_line + 1];
					memset(line, 0, max_chars_per_line + 1);
					if(chars_per_line[i] < (max_chars_per_line + 1))
					{
						strncpy(line, text_buffer, chars_per_line[i]);
					}
					else
					{
						strncpy(line, text_buffer, max_chars_per_line);
					}
					line[max_chars_per_line] = '\0';
					text_buffer += chars_per_line[i] + 1;

					text(line);

					line_area.y_init += max_font_height;
					line_area.y_end += max_font_height;
					setWorkingArea(line_area);
				}
			}

			free(word_safe_buffer);
			free(text_safe_buffer);
			free(line_safe_buffer);

			setWorkingArea(save_working_area);
		}
	}

	void GuiPainter::multiText_P(const char * msg)
	{
		if ( (msg != NULL) && (strlen_P(msg) > 0) )
		{
			Area save_working_area = m_working_area;

			uint8_t max_chars_per_line = m_working_area.width() / max_font_width;
			uint8_t max_num_lines = m_working_area.height() / max_font_height;

			uint8_t chars_per_line[max_num_lines];

			uint8_t num_line = 0;
			uint8_t total_num_lines = 1;

			uint8_t msg_length = strlen_P(msg) + 1;

			char * text_safe_buffer = (char *) malloc (msg_length);
			char * text_buffer = text_safe_buffer;

			char * word_safe_buffer = (char *) malloc (msg_length);
			char * word_buffer = word_safe_buffer;

			char * line_safe_buffer = (char *) malloc (msg_length);
			char * line_buffer = line_safe_buffer;

			if ( (text_buffer != NULL) && (word_buffer != NULL) && (line_buffer != NULL) )
			{
				memset(text_buffer, 0, msg_length);
				memset(word_buffer, 0, msg_length);
				memset(line_buffer, 0, msg_length);

				strcpy_P(text_buffer, msg);

				//First pass to get number of lines
				word_buffer = strsep(&text_buffer, " ");

				while ( (word_buffer != NULL) && (total_num_lines <= max_num_lines) )
				{
					// The current line is not full yet.
					if (strlen(line_buffer) + strlen(word_buffer) <= max_chars_per_line)
					{
						strcat(line_buffer, word_buffer);
						strcat(line_buffer, " ");
					}
					// The current line is full.
					else
					{
						line_buffer[strlen(line_buffer) - 1] = '\0';
						chars_per_line[num_line] = strlen(line_buffer);

						num_line++;
						if(total_num_lines < 4)
						{
							total_num_lines++;
						}
						memset(line_buffer, 0, msg_length);

						strcat(line_buffer, word_buffer);
						strcat(line_buffer, " ");
					}

					word_buffer = strsep(&text_buffer, " ");
				}

				line_buffer[strlen(line_buffer) - 1] = '\0';
				chars_per_line[num_line] = strlen(line_buffer);

				// Calculate the working area initial position
				uint8_t initial_position = m_working_area.y_init + (m_working_area.height() / 2) - (total_num_lines * max_font_height / 2);

				text_buffer = text_safe_buffer;
				strcpy_P(text_buffer, msg);

				Area line_area = getWorkingArea();
				line_area.y_init = initial_position;
				line_area.y_end = initial_position + max_font_height;
				setWorkingArea(line_area);

				for (uint8_t i = 0; i < total_num_lines; i++)
				{
					char line[max_chars_per_line + 1];
					memset(line, 0, max_chars_per_line + 1);
					if(chars_per_line[i] < (max_chars_per_line + 1))
					{
						strncpy(line, text_buffer, chars_per_line[i]);
					}
					else
					{
						strncpy(line, text_buffer, max_chars_per_line);
					}
					line[max_chars_per_line] = '\0';
					text_buffer += chars_per_line[i] + 1;

					text(line);

					line_area.y_init += max_font_height;
					line_area.y_end += max_font_height;
					setWorkingArea(line_area);
				}
			}

			free(word_safe_buffer);
			free(text_safe_buffer);
			free(line_safe_buffer);

			setWorkingArea(save_working_area);
		}
	}

	void GuiPainter::setFont(FontType_t font)
	{
		switch(font)
		{

			case FontType_t::BODY_FONT:
				if(LANG == Language::RU)
				{
					m_impl.setFont(u8g_font_6x9_rus);
				}
				else //Latin font
				{	
					m_impl.setFont(u8g_font_6x9);
				}
				break;

		}
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
		if ( (text == NULL) && !(strlen(text) > 0) )
		{
			return;
		}

		if (strlen(text) > 21)
		{
			char tmp_text[22];
			strncpy(tmp_text, text, 21);
			tmp_text[21] = '\0';

			m_impl.print(tmp_text);
		}
		else
		{
			m_impl.print(text);
		}
	}

	void GuiPainter::print_P(const char * text)
	{
		if ( (text != NULL) && (strlen_P(text) > 0) )
		{
			char c;
			while ((c = pgm_read_byte(text)) != '\0')
			{
				m_impl.print(c);
				text++;
			}
		}
	}

	void GuiPainter::animate(const char * text, uint8_t window, uint32_t delay_ms)
	{

		if ( window < strlen(text) )
		{
			if (m_animation_loop)
			{
				uint8_t diff = strlen(text) - window;

				if (m_animation_index < diff)
				{
					m_current_update_time = millis();
					if( m_current_update_time - m_previous_update_time > delay_ms)
					{
						const char * cadena = text + m_animation_index;
						strncpy(m_animation_text, cadena, window * sizeof(char));
						m_animation_text[window] = '\0';
						print(m_animation_text);

						m_animation_index++;
						m_previous_update_time = m_current_update_time;
					}
					else
					{
						const char * cadena = text + m_animation_index;
						strncpy(m_animation_text, cadena, window * sizeof(char));
						m_animation_text[window] = '\0';
						print(m_animation_text);
					}
				}
				else
				{
					const char * cadena = text + m_animation_index;
					strncpy(m_animation_text, cadena, window * sizeof(char));
					m_animation_text[window] = '\0';
					print(m_animation_text);
				}

				m_animation_loop = !m_animation_loop;
			}
			else
			{
				const char * cadena = text + m_animation_index;
				strncpy(m_animation_text, cadena, window * sizeof(char));
				m_animation_text[window] = '\0';
				print(m_animation_text);
				m_animation_loop = !m_animation_loop;
			}
		}
		else
		{
			print(text);
			m_animation_index = 0;
		}

	}

	void GuiPainter::animationReset(uint32_t timeout)
	{
		m_previous_update_time = 0;
		m_animation_index = 0;
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
		if (xx >= 100) 
		{
			conv[0]=(xx/100)%10+'0';
			conv[1]=(xx/10)%10+'0';
			conv[2]=(xx)%10+'0';
			conv[3]=0;
		}
		else if (xx >= 10)
		{
			conv[0]=(xx/10)%10+'0';
			conv[1]=(xx)%10+'0';
			conv[2]=0;
		}
		else
		{
			conv[0]=(xx)%10+'0';
			conv[1]=0;
		}
		return conv;
	}
}
