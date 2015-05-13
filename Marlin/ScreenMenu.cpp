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
			SERIAL_ECHO((int)x_init);
			SERIAL_ECHO(" ");
			SERIAL_ECHO((int)x_end);
			SERIAL_ECHO(" ");
			SERIAL_ECHO((int)y_init);
			SERIAL_ECHO(" ");
			SERIAL_ECHOLN((int)y_end);
			for (unsigned int i = 0;i <= m_num_items -1; ++i)
			{
				int col = i % 5;
				int row = i / 5;
				int row_t = (m_num_items-1) / 5;

				SERIAL_ECHO(" ");
				SERIAL_ECHO((int)((x_end + x_init)/2 - (m_num_items*26/(1+row_t)-2)/2 +col*26));
				SERIAL_ECHO(" ");
				SERIAL_ECHOLN((int)((y_end+y_init)/(2*(1+row_t)) - (24/2) + (24*row)));

				if (i == m_index)
				{
					(m_items[i]->icon(Icon::SELECTED)).draw((x_end + x_init)/2 - (m_num_items*26/(1+row_t)-2)/2 +col*26,(y_end+y_init)/(2*(1+row_t)) - (24/2) + (24*row));
					//(m_items[i]->icon(Icon::SELECTED)).draw((pix_av_x/2 - pix_x/2)+col*26, 2*(pix_av_y/2 - pix_y/2)+row*(24+1));
				}
				else
				{
					(m_items[i]->icon(Icon::NORMAL)).draw((x_end + x_init)/2 - (m_num_items*26/(1+row_t)-2)/2 +col*26,(y_end+y_init)/(2*(1+row_t)) - (24/2) + (24*row));
					//(m_items[i]->icon(Icon::NORMAL)).draw((pix_av_x/2 - pix_x/2)+col*26, 2*(pix_av_y/2 - pix_y/2)+row*(24+1));
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
