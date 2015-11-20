///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSetting.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of Settings screen class.
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

#include "ScreenSetting.h"
#include "Language.h"

namespace ui
{

	ScreenSetting::ScreenSetting(const char * title)
		: Screen(title, LIST)
		, m_index(0)
		, m_num_items(0)
	{ }

	ScreenSetting::~ScreenSetting()
	{
		for (uint8_t i = 0;i < m_num_items; ++i)
		{
			delete m_item[i];
		}
	}

	void ScreenSetting::init(uint16_t index)
	{ }

	void ScreenSetting::left()
	{
		if (m_index > 0)
		{
			m_needs_drawing = true;
			--m_index;
		}
	}

	void ScreenSetting::right()
	{
		if ( m_index < m_num_items - 1 )
		{
			m_needs_drawing = true;
			++m_index;
		}
	}

	void ScreenSetting::draw()
	{
		if(m_needs_drawing)
		{
			m_needs_drawing = false;

			painter.firstPage();
			do
			{
				painter.setColorIndex(1);
				painter.setFont(FontType_t::BODY_FONT);

				painter.title(m_title);

				m_scroll_size = (float) 47 / m_num_items;

				// Draw list
				uint8_t window_size = 50 / (max_font_height + 1);
				uint8_t window_selector = window_size / 2;
				if (window_size % 2 == 0)
				{
					window_selector--;
				}

				Area cell_area(0, 14, 122, 14 + max_font_height + 1);

				for (uint8_t i = 0; i < window_size; i++)
				{
					if (i == window_selector)
					{
						painter.setColorIndex(1);
						painter.drawBox(painter.coordinateXInit(), 14 + i * (max_font_height + 1), 128, max_font_height);
						painter.setColorIndex(0);
					}
					else
					{
						painter.setColorIndex(1);
					}

					if ((int)(m_index + i - window_selector) < 0 || (m_index + i - window_selector) >= m_num_items)
					{
						cell_area.y_init += max_font_height + 1;
						cell_area.y_end += max_font_height + 1;
						continue;
					}

					painter.setWorkingArea(cell_area);

					m_item[m_index + i - window_selector]->draw();

					cell_area.y_init += max_font_height + 1;
					cell_area.y_end += max_font_height + 1;
				}
				
				// Draw scroll bar
				painter.setColorIndex(1);
				painter.drawBox(122, 13, 6, 51);
				painter.setColorIndex(0);
				painter.drawBox(123, 14, 4, 49);

				int8_t scroll_bottom_bar = (m_index + 1) * m_scroll_size;
				if (scroll_bottom_bar < 1)
				{
					scroll_bottom_bar = 1;
				}
				painter.setColorIndex(1);
				painter.drawBox(124, 15, 2, scroll_bottom_bar);

				int8_t scroll_upper_bar = scroll_bottom_bar - m_scroll_size;
				if (scroll_upper_bar > 0)
				{
					painter.setColorIndex(0);
					painter.drawBox(124, 15, 2, scroll_upper_bar);
				}
			} while( painter.nextPage() );
		}
	}

	void ScreenSetting::press()
	{
		m_item[m_index]->press();
	}

	void ScreenSetting::add(Option * view)
	{
		if(m_num_items < m_max_items)
		{
			m_item[m_num_items] = view;
			++m_num_items;
		}
	}
}
