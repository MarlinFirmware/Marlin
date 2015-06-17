#include "ScreenMenu.h"

namespace screen
{
	ScreenMenu::ScreenMenu(const char * title, const char * text)
		: Screen(title, MENU)
		, m_index(0)
		, m_num_items(0)
		, m_text(text)
	{ }

	ScreenMenu::~ScreenMenu()
	{ }

	void ScreenMenu::left()
	{
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
		if ( m_index == (m_num_items -1) )
		{
			m_index = m_num_items -1;
		}
		else
		{
			++m_index;
		}
	}

	void ScreenMenu::draw()
	{
		uint8_t num_rows = m_num_items / 5;

		uint8_t items_per_row_0;
		uint8_t items_per_row_1;

		if (num_rows > 0)
		{
			items_per_row_0 = m_num_items / 2 + m_num_items % 2;
			items_per_row_1 = m_num_items - items_per_row_0;
		}
		else
		{
			items_per_row_0 = m_num_items;
			items_per_row_1 = 0;
		}

		//Start painting sequence
		painter.firstPage();
		do 
		{
			//Paint title on top of screen
			painter.title(m_title);
			//Paint text on the screen
			painter.text_P(m_text, 0, 5);
			//Paint selection box on bottom of screen
			painter.box((m_items[m_index]->icon()).text());
			//Icon grid
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit() + 5;
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_end = painter.coordinateYEnd();

			for (unsigned int i = 0;i <= m_num_items -1; ++i)
			{
				uint8_t row = i / items_per_row_0;
				uint8_t col = i % items_per_row_0;

				uint8_t x = 0;
				uint8_t y = 0;

				if (row == 0)
				{
					x = (x_end + x_init) / 2 - (items_per_row_0 * (icon_width + 2) / 2) + col * (icon_width + 2) + items_per_row_0 % 2;
					y = (y_end + y_init) / 2 - (num_rows * (icon_height + 5) + icon_height) / 2;
				}
				else
				{
					x = (x_end + x_init) / 2 - (items_per_row_1 * (icon_width + 2) / 2) + col * (icon_width + 2) + items_per_row_1 % 2;
					y = (y_end + y_init) / 2 - (num_rows * (icon_height + 5) + icon_height) / 2 + (icon_height + 5);
				}


				if (i == m_index)
				{
					m_items[i]->icon().draw(x,y, true);
				}
				else
				{
					m_items[i]->icon().draw(x,y);
				}
			}	

		} while( painter.nextPage() ); 	
	}

	void ScreenMenu::press()
	{
		ViewManager::getInstance().activeView(m_items[m_index]);
	}

	void ScreenMenu::add(Screen & component)
	{
		if (m_num_items < max_items)
		{
			m_items[m_num_items] = &component;
			++m_num_items;
		}
	}
}
