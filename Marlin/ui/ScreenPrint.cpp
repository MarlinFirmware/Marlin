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
#include "Language.h"
#include "Marlin.h"
#include "planner.h"

namespace ui
{
	ScreenPrint::ScreenPrint(const char * title, Subject<float> * model)
		: ScreenMenu(title)
		, Observer<float>(model)
		, m_observed(0)
		, m_printed_time( { 0, 0, 0, 0 })
		, m_previous_minutes(0)
		, m_previous_millis(0)
		, m_percent_done(0)
		, m_printing_status(PRINTING)
		, m_target_temperature(0)
#if MB(BQ_ZUM_MEGA_3D)
		, m_bed_observed(0)
		, m_bed_heating(false)
		, m_hotend_heating(true)
#endif
	{
		m_target_temperature = temp::TemperatureManager::single::instance().getTargetTemperature();
		char t_target[4] = { 0 };
		dtostrf(m_target_temperature, 3, 0, t_target);
		strcpy(m_temperature_info, "0/");
		strcat(m_temperature_info, t_target);
		strcat(m_temperature_info, "\xb0");
		strcpy(m_time_info, "00:00");
		strcpy(m_pause_info, "Z:0.0");
	}

	ScreenPrint::~ScreenPrint()
	{ }

	void ScreenPrint::init(uint16_t index)
	{
		this->connect();

		for (uint8_t i = 0;i <= m_num_icons -1; ++i)
		{
			m_icons[i]->show();
		}

		m_needs_drawing = true;
	}

	void ScreenPrint::draw()
	{
		uint32_t m_current_millis = millis();
		
		bool buffer_priority = false;
		if(planner_priority && !long_command)
		{
			buffer_priority = true;
		}
		
	#if MB(BQ_ZUM_MEGA_3D)
		if ( (m_current_millis - m_previous_millis >= 1000) && !buffer_priority )
		{
			m_bed_heating = false;
			uint8_t bed_temp = temp::TemperatureManager::single::instance().getBedCurrentTemperature();
			uint8_t bed_target = temp::TemperatureManager::single::instance().getBedTargetTemperature();
			if(bed_temp <= bed_target-2)
			{
				m_bed_heating = true;
			}
			
			if((abs(m_bed_observed - bed_temp) >= 1) && m_bed_heating)
			{
				char s_target[4] = { 0 };
				char s_current[4] = { 0 };
				dtostrf(bed_target, 3, 0, s_target);
				dtostrf(bed_temp, 3, 0, s_current);
				strcpy(m_bed_info, s_current);
				strcat(m_bed_info, "/");
				strcat(m_bed_info, s_target);
				strcat(m_bed_info, "B");
				
				m_bed_heating = true;
				m_needs_drawing = true;
			}
			
			if(abs(m_bed_observed - bed_temp) > 1)
			{
				m_bed_observed = bed_temp;
			}
		}
	#endif // MB(BQ_ZUM_MEGA_3D)
		
		if ( (m_current_millis - m_previous_millis >= 1500) && !buffer_priority )
		{
			m_previous_millis = m_current_millis;
			m_printed_time = PrintManager::printingTime();

			if (m_printed_time.minutes != m_previous_minutes)
			{
				m_previous_minutes = m_printed_time.minutes;
				strcpy(m_time_info, painter.itostr2(m_printed_time.hours));
				strcat(m_time_info, ":");
				strcat(m_time_info, painter.itostr2(m_printed_time.minutes));

				m_needs_drawing = true;
			}

			uint8_t percent_done = card.percentDone();
			if (m_percent_done < percent_done)
			{
				m_percent_done = percent_done;

				m_needs_drawing = true;
			}
		}

		if(PrintManager::single::instance().state() != m_printing_status)
		{
			char z_value[6] = { 0 };
			m_printing_status = PrintManager::single::instance().state();
			switch(m_printing_status)
			{
				case PRINTING:
					m_title = MSG_SCREEN_PRINT_PRINTING();
					break;
				case PAUSED:
					m_title = MSG_SCREEN_PRINT_PAUSED();
					dtostrf(z_height, 5, 2, z_value);
					strcpy(m_pause_info, "Z:");
					strcat(m_pause_info, z_value);
					break;
				case PAUSING:
					m_title = MSG_SCREEN_PRINT_PAUSING();
					break;
				default:
					break;
			}

			m_needs_drawing = true;
		}
		
		if (temp::TemperatureManager::single::instance().getTargetTemperature() != m_target_temperature)
		{
			m_target_temperature = temp::TemperatureManager::single::instance().getTargetTemperature();
			char t_target[4] = { 0 };
			char t_current[4] = { 0 };
			dtostrf(m_target_temperature, 3, 0, t_target);
			dtostrf(m_observed, 3, 0, t_current);
			strcpy(m_temperature_info, t_current);
			strcat(m_temperature_info, "/");
			strcat(m_temperature_info, t_target);
			strcat(m_temperature_info, "\xb0");
			
		#if MB(BQ_ZUM_MEGA_3D)
			m_hotend_heating = true;
		#endif // MB(BQ_ZUM_MEGA_3D)
			m_needs_drawing = true;
		}

		if (m_needs_drawing == true)
		{
			m_needs_drawing = false;

			//Start painting sequence
			painter.firstPage();
			do
			{
				// Paint title on top of screen
				painter.title(m_title);
				painter.setColorIndex(1);
				
			#if MB(BQ_ZUM_MEGA_3D)
				if(m_hotend_heating || !m_bed_heating)
				{
			#endif // MB(BQ_ZUM_MEGA_3D)
					painter.setPrintPos(127 - (strlen(m_temperature_info) * 6) + 1, 3);
					painter.print(m_temperature_info);
			#if MB(BQ_ZUM_MEGA_3D)
				}
				else
			
				{
					painter.setPrintPos(127 - (strlen(m_bed_info) * 6) + 1, 3);
					painter.print(m_bed_info);
				}
			#endif // MB(BQ_ZUM_MEGA_3D)

				if(m_printing_status == PRINTING || m_printing_status == READY)
				{
					// Draw status progress bar
					painter.printingStatus(m_percent_done, true);
				}
				else
				{
					//Draw status and Z height
					painter.printingStatus(m_percent_done, false);
					painter.setPrintPos( (127 - strlen(m_pause_info) * 6) / 2, 19);
					painter.print(m_pause_info);
				}

				painter.setPrintPos(127 - (strlen(m_time_info) * 6) + 1, 19);
				painter.print(m_time_info);

				if ( m_index != (m_num_items -1) && m_index != 0)
				{
					painter.box((m_icons[m_index])->text(), BOTH);
				}
				else if(m_index == (m_num_items -1))
				{
					painter.box((m_icons[m_index])->text(), LEFT);
				}
				else if (m_index == 0)
				{
					painter.box((m_icons[m_index])->text(), RIGHT);
				}

				// Draw icon grid
				Area icons_area(0, 31, 127, 54);
				painter.setWorkingArea(icons_area);

				uint8_t x_init = icons_area.x_init;
				uint8_t y_init = icons_area.y_init;
				uint8_t x_end = icons_area.x_end;
				uint8_t y_end = icons_area.y_end;

				for (uint8_t i = 0; i < m_num_items; i++)
				{
					int x = x_init + (icons_area.width() / 2) - ((m_num_items * (icon_width + 2) - 2) / 2) + (i * (icon_width + 2));
					int y = y_init;

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

	void ScreenPrint::update(float value)
	{
		if (abs(m_observed - value) >= 1)
		{
			m_observed = round(value);

			char t_target[4] = { 0 };
			char t_current[4] = { 0 };
			dtostrf(m_target_temperature, 3, 0, t_target);
			dtostrf(m_observed, 3, 0, t_current);
			strcpy(m_temperature_info, t_current);
			strcat(m_temperature_info, "/");
			strcat(m_temperature_info, t_target);
			strcat(m_temperature_info, "\xb0");
			
			m_needs_drawing = true;
		}
	
	#if MB(BQ_ZUM_MEGA_3D)
		if(m_observed <= (m_target_temperature - 5))
		{
			m_hotend_heating = true;
		}
		else
		{
			m_hotend_heating = false;
		}
	#endif // MB(BQ_ZUM_MEGA_3D)
	}
}
