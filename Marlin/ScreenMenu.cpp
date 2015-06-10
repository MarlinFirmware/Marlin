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
		//Start painting sequence
		painter.firstPage();
		do 
		{
			//Paint title on top of screen
			painter.title(m_title);
			//Paint text on the screen
			painter.text_P(m_text, 5);
			//Paint selection box on bottom of screen
			painter.box((m_items[m_index]->icon()).text());
			//Icon grid
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit() + 5;
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_end = painter.coordinateYEnd();
			for (unsigned int i = 0;i <= m_num_items -1; ++i)
			{
				int col = i % 5;
				int row = i / 5;
				int row_t = (m_num_items-1) / 5;

				int x = (x_end + x_init)/2 - (m_num_items*(icon_width+2)/(1+row_t)-2)/2 +col*(icon_width+2);
				int y = (y_end + y_init)/(2*(1+row_t)) + row_t - (icon_height/2) + ((icon_height+5)*row);

				if (i == m_index)
				{
					(m_items[i]->icon()).draw(x,y, true);
				}
				else
				{
					(m_items[i]->icon()).draw(x,y);
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
