///////////////////////////////////////////////////////////////////////////////
/// \file ScreenPrint.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of printing screen.
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

#include "ScreenPrint.h"

#include "cardreader.h"
#include "TemperatureManager.h"
#include "ViewManager.h"
#include "Language.h"

namespace screen
{
	ScreenPrint::ScreenPrint(const char * title, Subject<float> * model)
		: ScreenMenu(title)
		, Observer<float>(model)
		, m_observed(0)
		, m_printed_time( { 0, 0, 0, 0 })
		, m_percent_done(0)
		, m_printing_status(PRINTING)
		, m_target_temperature(0)
	{ }

	ScreenPrint::~ScreenPrint()
	{
		this->m_model->dettach();
	}

	void ScreenPrint::init(uint16_t index)
	{
		this->connect();

		for (unsigned int i = 0;i <= m_num_icons -1; ++i)
		{
			m_icons[i]->show();
		}
	}

	void ScreenPrint::draw()
	{
		PrintManager::updateTime();
		if ( (m_printed_time.hours !=  PrintManager::printingTime().hours) ||
			(m_printed_time.minutes != PrintManager::printingTime().minutes) )
		{
			m_printed_time = PrintManager::printingTime();
			m_needs_drawing = true;
		}

		uint8_t percent_done = card.percentDone();
		if (m_percent_done != percent_done)
		{
			m_percent_done = percent_done;
			m_needs_drawing = true;
		}

		if(PrintManager::single::instance().state() != m_printing_status)
		{
			m_printing_status = PrintManager::single::instance().state();
			switch(m_printing_status)
			{
				case PRINTING:
					m_title = MSG_SCREEN_PRINT_PRINTING();
					break;
				case PAUSED:
					m_title = MSG_SCREEN_PRINT_PAUSED();
					break;
				case PAUSING:
					m_title = MSG_SCREEN_PRINT_PAUSING();
					break;
				default:
					break;
			}
			m_needs_drawing = true;
		}

		char t_target[4] = { 0 };
		if(TemperatureManager::single::instance().getTargetTemperature() != m_target_temperature)
		{
			m_target_temperature = TemperatureManager::single::instance().getTargetTemperature();
			m_needs_drawing = true;
		}
		dtostrf(m_target_temperature, 3, 0, t_target);
		int size_target = strlen(t_target);

		if (m_needs_drawing)
		{
			m_needs_drawing = false;
			//Start painting sequence
			painter.firstPage();
			do
			{
				//Paint title on top of screen
				painter.title(m_title);

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
				painter.printingStatus(m_percent_done, m_printed_time.hours, m_printed_time.minutes);
				//Paint selection box on bottom of screen
				painter.arrowBox((m_icons[m_index])->text());
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

		if (PrintManager::single::instance().state() == STOPPED)
		{
			ViewManager::getInstance().activeView(screen_print_complete);
		}
	}

	void ScreenPrint::update(float value)
	{
		if ((int) value != (int) m_observed)
		{
			m_observed = value;
			m_needs_drawing = true;
		}
	}
}
