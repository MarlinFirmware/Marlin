#include "ScreenMenu.h"
#include "dogm_font_data_marlin.h"

namespace screen
{
	ScreenMenu::ScreenMenu(const char * title)
		: Screen(title, MENU)
		, m_icon_line(0)
		, m_index(0)
		, m_num_items(0)
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
		/*SERIAL_ECHO(m_title);
		SERIAL_ECHO(">>>");
		SERIAL_ECHO(" [");
		SERIAL_ECHO(m_items[m_index]->title());
		SERIAL_ECHOLN("] "); 	*/
		painter.firstPage();
		do 
		{
			drawTitle();
			painter.box(m_items[m_index]->title());
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_end = painter.coordinateYEnd();
			for (unsigned int i = 0;i <= m_num_items -1; ++i)
			{
				int col = i % 5;
				int row = i / 5;
				int row_t = (m_num_items-1) / 5;

				int x = (x_end + x_init)/2 - (m_num_items*(icon_width+2)/(1+row_t)-2)/2 +col*(icon_width+2);
				int y = (y_end+y_init)/(2*(1+row_t)) + row_t - (icon_height/2) + ((icon_height+3)*row);

				if (i == m_index)
				{
					(m_items[i]->icon(Icon::SELECTED)).draw(x,y);
				}
				else
				{
					(m_items[i]->icon(Icon::NORMAL)).draw(x,y);
				}
			}		
		} while( painter.nextPage() ); 
	}

	Screen & ScreenMenu::press(Screen * parent_view)
	{
		if (m_items[m_index]->type() == Screen::SIMPLE)
		{
			return * parent_view;
		}
		return * m_items[m_index];
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
