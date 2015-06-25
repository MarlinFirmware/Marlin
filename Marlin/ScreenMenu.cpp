#include "ScreenMenu.h"

namespace screen
{
	ScreenMenu::ScreenMenu(const char * title, const char * text)
		: Screen(title, MENU)
		, m_index(0)
		, m_num_items(0)
		, m_num_icons(0)
		, m_text(text)
	{ }

	ScreenMenu::~ScreenMenu()
	{ }

	void ScreenMenu::init()
	{
		for (unsigned int i = 0;i <= m_num_icons -1; ++i)
		{
			m_icons[i]->show();
		}
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
		ViewManager::getInstance().activeView(m_items[m_index]);
	}

	void ScreenMenu::draw()
	{
		if ( !m_text || m_needs_drawing ) 
		{
			m_needs_drawing = false;
			int total_width_0 = 0;
			int total_width_1 = 0;
			int items_per_row_0 = 0;
			int items_per_row_1 = 0;
			int num_rows = 2;
			int max_width = 128;

			//Get total width of icons
			for(unsigned int i = 0; i < m_num_icons; i++)
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
				//Paint text on the screen
				painter.text_P(m_text, 0, 5);
				//Paint selection box on bottom of screen
				painter.box((m_icons[m_index])->text());
				//Icon grid
				uint8_t x_init = painter.coordinateXInit();
				uint8_t y_init = painter.coordinateYInit() + 5;
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_end = painter.coordinateYEnd();

				for (unsigned int i = 0; i < m_num_icons; i++)
				{
					uint8_t row = i / items_per_row_0;
					uint8_t x = 0;
					uint8_t y = 0;

					if (i < items_per_row_0)
					{
						x = (x_end + x_init) / 2 - (total_width_0 / 2) + aux2_width_0;
						y = (y_end + y_init) / 2 - ((num_rows - 1) * (m_icons[i]->height() + 5) + m_icons[i]->height()) / 2;
						aux2_width_0 += m_icons[i]->width() + 2;
					}
					else
					{
						x = (x_end + x_init) / 2 - (total_width_1 / 2) + aux2_width_1;
						y = (y_end + y_init) / 2 - ((num_rows - 1) * (m_icons[i]->height() + 5) + m_icons[i]->height()) / 2 + (m_icons[i]->height() + 5);
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

   void ScreenMenu::icon(Icon & component)
	{
		if (m_num_icons < max_items)
		{
			m_icons[m_num_icons] = &component;
			++m_num_icons;
		}
	}
}
