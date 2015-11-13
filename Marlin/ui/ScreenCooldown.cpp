///////////////////////////////////////////////////////////////////////////////
/// \file ScreenCooldown.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of cooldown screen.
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

#include "ScreenCooldown.h"

#include "Language.h"
#include "GuiBitmaps_witbox_2.h"

namespace ui
{
	ScreenCooldown::ScreenCooldown(const char * title, const char * text, uint16_t target, Subject<float> * model)
		: ScreenAnimation(title, text, LESS_OR_EQUAL, target, model)
	{
		this->connect();
		lcd_enable_button();
	}

	ScreenCooldown::~ScreenCooldown()
	{ }

	void ScreenCooldown::draw()
	{
		char temp[21] = { 0 };
		char c_current[4] = { 0 };
		dtostrf(m_observed, 3, 0, c_current);
		strcat(temp, c_current);
		strcat(temp, "\xb0");
		strcat(temp, " / ");

		if(temp::TemperatureManager::single::instance().getTargetTemperature() >= temp::min_temp_cooling)
		{
			char c_target[4] = { 0 };
			snprintf(c_target, 4, "%d", m_target);
			strcat(temp, c_target);
			strcat(temp, "\xb0");
		}
		else
		{
			strcat_P(temp, MSG_TEMP_OFF());
		}

		//Check time for next progress bar frame
		m_current_time = millis();

		if(m_current_time >= m_previous_time + 100)
		{
			m_current_time = millis();
			m_previous_time = m_current_time;
			m_frame = (m_frame + 1) % 8;

			painter.firstPage();
			do
			{
				painter.title(m_title);
				painter.box(m_text);

				//Draw temperature
				uint8_t y_init = painter.coordinateYInit();
				uint8_t y_end = painter.coordinateYEnd();

				painter.setColorIndex(1);
				painter.setPrintPos(64 - strlen(temp)*6/2, (y_end + y_init)/2 - 9/2 - 3);
				painter.print(temp);

				//Draw progress bar
				painter.setColorIndex(1);
				painter.drawBox(8,38,112,6);
				painter.setColorIndex(0);
				painter.drawBox(9,39,110,4);
				painter.setColorIndex(1);

				switch(m_frame)
				{
					case 0:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_1);
						break;
					case 1:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_2);
						break;
					case 2:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_3);
						break;
					case 3:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_4);
						break;
					case 4:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_5);
						break;
					case 5:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_6);
						break;
					case 6:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_7);
						break;
					case 7:
						painter.drawBitmap(10, 40, progress_width, progress_height, bits_progress_8);
						break;
				}

			} while ( painter.nextPage() );
		}

		if ( isConditionMet() )
		{
			lcd_enable_button();
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	bool ScreenCooldown::isConditionMet()
	{
		switch(m_condition)
		{
			case LESS_OR_EQUAL:
			{
				if(temp::TemperatureManager::single::instance().getTargetTemperature() >= temp::min_temp_cooling)
				{
					return (round(m_observed <= m_target));
				}
				else
				{
					return (round(m_observed <= temp::min_temp_cooling));
				}
				break;
			}
			case EQUAL:
			{
				return (round(m_observed == m_target));
				break;
			}
			case GREATER_OR_EQUAL:
			{
				return (round(m_observed >= m_target));
				break;
			}
			default:
			{
				return false;
				break;
			}
		}
	}
}