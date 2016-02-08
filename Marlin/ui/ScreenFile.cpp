///////////////////////////////////////////////////////////////////////////////
/// \file ScreenFile.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of file confirmation screen.
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

#include "ScreenFile.h"

#include "cardreader.h"
#include "GuiImpl_witbox_2.h"

namespace ui
{
	ScreenFile::ScreenFile(const char * title , Subject<bool> * model)
		: ScreenMenu(title)
		, Observer<bool>(model)
	{ }

	ScreenFile::~ScreenFile()
	{ }

	void ScreenFile::draw()
	{
		int total_width_0 = 0;
		int total_width_1 = 0;
		int items_per_row_0 = 0;
		int items_per_row_1 = 0;
		int num_rows = 2;
		int max_width = 128;

		//Get total width of icons
		for(uint8_t i = 0; i < m_num_icons; i++)
		{
			total_width_1 += m_icons[i]->width() + 2;
		}

		total_width_0 = 0;
		total_width_1 = total_width_1 - 2;

		int aux_width_0 = total_width_0;
		int aux_width_1 = total_width_1;

		if(total_width_1 <= max_width)
		{
			num_rows = 1;
			items_per_row_0 = m_num_icons;
			items_per_row_1 = 0;
			aux_width_0 = total_width_1;
			aux_width_1 = 0;
		}
		else
		{
			//Split the number of icons between rows uniformly assuring line 0 width > line 1 width
			int i = 0;
			while(aux_width_0 < aux_width_1)
			{
				aux_width_0 += m_icons[i]->width() + 2;
				aux_width_1 -= m_icons[i]->width() + 2;
				items_per_row_0++;
				i++;
			}
			items_per_row_1 = m_num_icons - items_per_row_0;
			aux_width_0 -= 2;
		}
		
		char text[18];
		if(strlen(card.longFilename) == 0)
		{
			strncpy(text,card.filename,18);
		}
		else
		{
			strncpy(text,card.longFilename,18);
		}
		text[17] = '\0';

		Area text_area(0, 18, 127, 30);
		Area icons_area(0, 31, 127, 52);

		if(m_needs_drawing)
		{
			m_needs_drawing = false;

			//Start painting sequence
			painter.firstPage();
			do
			{
				total_width_0 = aux_width_0;
				total_width_1 = aux_width_1;
				int aux2_width_0 = 0;
				int aux2_width_1 = 0;

				//Paint title on top of screen
				painter.title(m_title);
				//Paint selection box on bottom of screen
				if ( m_index != (m_num_items -1) && m_index != 0)
				{
					painter.box((m_icons[m_index])->text(), BOTH);
				}
				else if(m_index == (m_num_items -1))
				{
					painter.box((m_icons[m_index])->text(), LEFT);
				}
				else if (m_index == 0)
				{
					painter.box((m_icons[m_index])->text(), RIGHT);
				}

				//Paint text on the screen
				painter.setWorkingArea(text_area);
				painter.text(text);

				//Icon grid
				uint8_t x_init = icons_area.x_init;
				uint8_t y_init = icons_area.y_init;
				uint8_t x_end = icons_area.x_end;
				uint8_t y_end = icons_area.y_end;

				for (uint8_t i = 0; i < m_num_icons; i++)
				{
					uint8_t row = i / items_per_row_0;
					uint8_t x = 0;
					uint8_t y = 0;

					if (i < items_per_row_0)
					{
						x = x_init + (icons_area.width() / 2) - (total_width_0 / 2) + aux2_width_0;
						y = y_init + (icons_area.height() / 2) - ((num_rows - 1) * (m_icons[i]->height() + 5) + m_icons[i]->height()) / 2;
						aux2_width_0 += m_icons[i]->width() + 2;
					}
					else
					{
						x = x_init + (icons_area.width() / 2) - (total_width_1 / 2) + aux2_width_1;
						y = y_init + (icons_area.height() / 2) - ((num_rows - 1) * (m_icons[i]->height() + 5) + m_icons[i]->height()) / 2 + (m_icons[i]->height() + 5);
						aux2_width_1 += m_icons[i]->width() + 2;
					}

					//Choosing bitmap state
					if (i == m_index)
					{
						m_icons[i]->draw(x,y, true);
					}
					else
					{
						m_icons[i]->draw(x,y);
					}
				}
			} while( painter.nextPage() );
		}
	}

	void ScreenFile::update(bool is_inserted)
	{
		if(is_inserted == false)
		{
			card.release();
			ViewManager::getInstance().activeView(screen_main);
		}
	}
}
