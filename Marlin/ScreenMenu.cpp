#include "ScreenMenu.h"
#include "dogm_font_data_marlin.h"

namespace screen
{
	ScreenMenu::ScreenMenu(const char * title)
		: Screen(MENU, title)
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
		drawTitle();

		SERIAL_ECHO(m_title);
		SERIAL_ECHO(">>>");
		SERIAL_ECHO(" [");
		SERIAL_ECHO(m_items[m_index]->title());
		SERIAL_ECHOLN("] ");
		if (m_num_items > 5)
		{
			m_icon_line = 1;
		}
		else
		{
			m_icon_line = 0;
		}
		uint16_t pix_x = (26*m_num_items-2)/(m_icon_line+1); //Number of pixels needed
		uint16_t pix_av_x = 128; //Number of available pixels
		uint16_t pix_y = 24*(m_icon_line+1);
		uint16_t pix_av_y = 64-13;    	
		painter.firstPage();
		do 
		{
			painter.setColorIndex(1);	// black on white
			for (unsigned int i = 0;i <= m_num_items -1; ++i)
			{
				int col = i % 5;
				int row = i / 5;
				if (i == m_index)
				{
					(m_items[i]->icon(Icon::SELECTED)).draw((pix_av_x/2 - pix_x/2)+col*26, 2*(pix_av_y/2 - pix_y/2)+row*(24+1));
				}
				else
				{
					(m_items[i]->icon(Icon::NORMAL)).draw((pix_av_x/2 - pix_x/2)+col*26, 2*(pix_av_y/2 - pix_y/2)+row*(24+1));
				}
			}		
			painter.drawBox(0, pix_av_y, 128, 64);
			painter.setColorIndex(0);
			painter.setFont(u8g_font_6x9);
			painter.setPrintPos(2,60);
			painter.print("<");
			painter.setPrintPos(128-7,60);
			painter.print(">");
			painter.setPrintPos((128-strlen(m_items[m_index]->title())*6)/2,60);
			painter.print(m_items[m_index]->title());
		} while( painter.nextPage() ); 
	}

	Screen & ScreenMenu::press()
	{
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
