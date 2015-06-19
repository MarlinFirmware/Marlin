#include "ScreenPrint.h"

#include "cardreader.h"
#include "TemperatureManager.h"

namespace screen
{
	ScreenPrint::ScreenPrint(const char * title, Subject<float> * model)
		: ScreenMenu(title)
		, Observer<float>(model)
		, m_observed(0)
	{ 
		this->connect();
	}

	ScreenPrint::~ScreenPrint()
	{ }

	void ScreenPrint::init()
	{
		this->m_model->attach(this);
		this->connect();

		for (unsigned int i = 0;i <= m_num_icons -1; ++i)
		{
			m_icons[i]->show();
		}
	}

	void ScreenPrint::draw()
	{
		char t_target[4] = { 0 };
		dtostrf(TemperatureManager::single::instance().getTargetTemperature(), 3, 0, t_target);
		int size_target = strlen(t_target);

		//Start painting sequence
		painter.firstPage();
		do 
		{
			//Paint title on top of screen
			painter.title(m_title);

			//Print widget text
			painter.setColorIndex(1);
			char t_current[4] = { 0 };
			dtostrf(m_observed, 3, 0, t_current);
			int size_current = strlen(t_current);
			
			painter.setPrintPos(128 - (size_target*6) - (size_current*6) - 11, 3);
			painter.print(t_current);
			painter.print("/");
			painter.print(t_target);
			painter.print("\xb0");

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

	void ScreenPrint::update(float value)
	{
		m_observed = value;
	}
}
