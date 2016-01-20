///////////////////////////////////////////////////////////////////////////////
/// \file ScreenStats.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///			Koldo Imanol de Miguel
///
/// \brief Implementation of Print Complete screen.
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

#include "ScreenStats.h"

#include "StatsManager.h"
#include "Language.h"

namespace ui
{
	ScreenStats::ScreenStats(const char * title,  const char * box)
		: Screen(title, DIALOG),
		m_box(box)
	{ 
		m_hours = StatsManager::single::instance().getHours();
		m_failed_prints = StatsManager::single::instance().getFailedPrints();
		m_succeded_prints = StatsManager::single::instance().getSuccededPrints();
	}

	ScreenStats::~ScreenStats()
	{ }

	void ScreenStats::draw()
	{
		char temp_str[24];
		uint8_t temp_lenght;
		uint8_t str_lenght;
		uint8_t num_spaces;
		
		// First line composition
		char total_time[24];
		strcpy_P(total_time, MSG_SCREEN_VIEW_STATS_TEXT1());
		str_lenght = strnlen(total_time,24);
		
		snprintf(temp_str, 24, "%01uh",m_hours);
		temp_lenght = strnlen(temp_str,24);
				
		num_spaces = 21-(str_lenght+temp_lenght);
		memset(&(total_time[str_lenght]), ' ', num_spaces);
		total_time[str_lenght + num_spaces] = '\0';
		strcat(total_time, temp_str);
		
		// Second line composition	
		char successful_prints[24];
		strcpy_P(successful_prints, MSG_SCREEN_VIEW_STATS_TEXT2());	
		str_lenght = strnlen(successful_prints,24);
		
		snprintf(temp_str, 24, "%01u", m_succeded_prints);
		temp_lenght = strnlen(temp_str,24);
		
		num_spaces = 21-(str_lenght+temp_lenght);
		memset(&(successful_prints[str_lenght]), ' ', num_spaces);
		successful_prints[str_lenght + num_spaces] = '\0';
		strcat(successful_prints, temp_str);
		
		// Third line composition
		char failed_prints[24];
		strcpy_P(failed_prints, MSG_SCREEN_VIEW_STATS_TEXT3());	
		str_lenght = strnlen(failed_prints,24);
		
		snprintf(temp_str, 24, "%01u", m_failed_prints);
		temp_lenght = strnlen(temp_str,24);
		
		num_spaces = 21-(str_lenght+temp_lenght);
		memset(&(failed_prints[str_lenght]), ' ', num_spaces);
		failed_prints[str_lenght + num_spaces] = '\0';
		strcat(failed_prints, temp_str);
		
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.box(m_box);

			painter.setColorIndex(1);
			
			//First line, printing time
			Area text_area(0, 17, 127, 29);
			painter.setWorkingArea(text_area);
			painter.text(total_time);
			
			//Second line, successful prints
			Area success_area(0, 30, 127, 42);
			painter.setWorkingArea(success_area);
			painter.text(successful_prints);
			
			//Third line, failed prints
			Area fail_area(0, 43, 127, 55);
			painter.setWorkingArea(fail_area);
			painter.text(failed_prints);

		} while( painter.nextPage() );
	}

	void ScreenStats::press()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}
	
	void ScreenStats::init(uint16_t index)
	{
		lcd_disable_button();
		draw();
		lcd_enable_button();
	}
	
}
