#include "ScreenPrint.h"

#include "cardreader.h"

namespace screen
{
	ScreenPrint::ScreenPrint(const char * title)
		: ScreenMenu(title)
	{ }

	ScreenPrint::~ScreenPrint()
	{ }

	void ScreenPrint::draw()
	{
      //Start painting sequence
		painter.firstPage();
		do 
		{
         //Paint title on top of screen
			painter.title(m_title);
			//Status widget
			uint16_t actual_time = 0;
			if (starttime != 0)
			{
				actual_time = millis()/60000 - starttime/60000;
			}
			painter.printing_status(card.percentDone(), actual_time);
			//Paint selection box on bottom of screen
			painter.box((m_icons[m_index])->text());
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
				int y = (y_end+y_init)/(2*(1+row_t)) + row_t - (icon_height/2) + ((icon_height+3)*row);

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
