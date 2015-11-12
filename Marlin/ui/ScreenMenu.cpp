///////////////////////////////////////////////////////////////////////////////
/// \file ScreenMenu.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of menu-type screens.
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

#include "ScreenMenu.h"

namespace ui
{
	ScreenMenu::ScreenMenu(const char * title, const char * text)
		: Screen(title, MENU)
		, m_index(0)
		, m_num_items(0)
		, m_num_icons(0)
		, m_text(text)
	{ }

	ScreenMenu::~ScreenMenu()
	{ 
		for (uint8_t i = 0;i < m_num_icons; ++i)
		{
			delete m_icons[i];
		}
	}

	void ScreenMenu::init(uint16_t index)
	{
		for (uint8_t i = 0;i < m_num_icons; ++i)
		{
			m_icons[i]->show();
		}
		m_index = index;
	}

	void ScreenMenu::left()
	{
		m_needs_drawing = true;
		if (m_index == 0)
		{
			m_index = 0;
		}
		else
		{
			--m_index;
		}
	}

	void ScreenMenu::right()
	{
		m_needs_drawing = true;
		if ( m_index == (m_num_items -1) )
		{
			m_index = m_num_items -1;
		}
		else
		{
			++m_index;
		}
	}

	void ScreenMenu::press()
	{
		ViewManager::getInstance().setLastFocus(m_index);
		ViewManager::getInstance().activeView(m_items[m_index]);
	}

	void ScreenMenu::draw()
	{
		if ( !m_text || m_needs_drawing)
		{
			m_needs_drawing = false;
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

			Area text_area(0, 18, 127, 30);
			Area icons_area;
			if (m_text == 0)
			{
				icons_area = Area(0, 4, 127, 52);
			}
			else
			{
				icons_area = Area(0, 31, 127, 52);
			}

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
				painter.text_P(m_text);

				//Icon grid
				painter.setWorkingArea(icons_area);

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

	void ScreenMenu::add(ScreenIndex_t const & component)
	{
		if (m_num_items < max_items)
		{
			m_items[m_num_items] = component;
			++m_num_items;
		}
	}

	void ScreenMenu::icon(Icon * component)
	{
		if (m_num_icons < max_items)
		{
			m_icons[m_num_icons] = component;
			++m_num_icons;
		}
	}
}
