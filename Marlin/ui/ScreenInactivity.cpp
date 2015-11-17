///////////////////////////////////////////////////////////////////////////////
/// \file ScreenInactivity.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of inactivity screen.
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

#include "ScreenInactivity.h"

#include "Language.h"
#include "GuiBitmaps_witbox_2.h"
#include "PrintManager.h"

namespace ui
{
	ScreenInactivity::ScreenInactivity(const char * title, const char * text, uint16_t target, Subject<float> * model)
		: ScreenAnimation(title, text, LESS_OR_EQUAL, target, model)
	{
		this->connect();
		lcd_enable_button();
	}

	ScreenInactivity::~ScreenInactivity()
	{ }

	void ScreenInactivity::init(uint16_t index)
	{
		this->connect();
	}

	void ScreenInactivity::draw()
	{
		char c_target[4] = { 0 };
		snprintf(c_target, 4, "%d", m_target);

		char c_current[4] = { 0 };
		dtostrf(m_observed, 3, 0, c_current);

		//Check time for next progress bar frame
		m_current_time = millis();

		if(m_current_time >= m_previous_time)
		{
			m_current_time = millis();
			m_previous_time = m_current_time;
			m_frame = (m_frame + 1) % 8;

			painter.firstPage();
			do
			{
				painter.setColorIndex(1);
				painter.drawBox(0,0,128,64);

				painter.setColorIndex(0);
				painter.box(m_text);

				//Paint bitmap logo on the left
				uint8_t x_init = painter.coordinateXInit();
				uint8_t y_init = painter.coordinateYInit();
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_end = painter.coordinateYEnd();
				uint8_t x_offset = 9;
				uint8_t y_offset = 9;
				painter.drawBitmap(x_init + x_offset, y_init + y_offset, logo_width, logo_height, bits_logo_about);

				//Print text
				x_offset = 8;
				y_offset = 7;
				x_init = 128 - strlen_P(MSG_MODE())*6 - x_offset;
				painter.setPrintPos(x_init, y_offset);
				painter.print_P(MSG_MODE());
				x_init = 128 - strlen_P(MSG_INACTIVE())*6 - x_offset;
				y_init = y_offset + max_font_height + 1;
				painter.setPrintPos(x_init, y_init + 1);
				painter.print_P(MSG_INACTIVE());

				//Paint bitmap inactivity on the right
				x_init = 128 - inactivity_width - x_offset;
				y_init += 3;
				painter.drawBitmap(x_init, y_init + y_offset, inactivity_width, inactivity_height, icon_inactivity);

				//Print temp
				char temp[21] = "";
				strcat(temp, c_current);
				strcat(temp, "\xb0");

				painter.setPrintPos(x_init - strlen(temp)*6 - 4, y_init + (max_font_height + 1)*2 - 5);
				painter.print(temp);

				//Draw progress bar
				x_init = 8;
				y_init = 47;
				painter.setColorIndex(1);
				painter.drawBox(x_init, y_init, 112, 6);
				painter.setColorIndex(0);
				painter.drawBox(x_init + 1, y_init + 1, 110, 4);
				painter.setColorIndex(1);

				switch(m_frame)
				{
					case 0:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_1);
						break;
					case 1:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_2);
						break;
					case 2:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_3);
						break;
					case 3:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_4);
						break;
					case 4:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_5);
						break;
					case 5:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_6);
						break;
					case 6:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_7);
						break;
					case 7:
						painter.drawBitmap(x_init + 2, y_init + 2, progress_width, progress_height, bits_progress_8);
						break;
				}

			} while ( painter.nextPage() );
		}

		if(!PrintManager::single::instance().getInactivityFlag())
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	void ScreenInactivity::press()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}

	void ScreenInactivity::left()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}

	void ScreenInactivity::right()
	{
		ViewManager::getInstance().activeView(m_next_screen);
	}

	void ScreenInactivity::add(ScreenIndex_t const & component)
	{
		m_next_screen = component;
	}

	void ScreenInactivity::update(float value)
	{
		m_observed = value;
	}

	bool ScreenInactivity::isConditionMet()
	{
		switch(m_condition)
		{
			case LESS_OR_EQUAL:
				return ((uint16_t) m_observed <= m_target);
			case EQUAL:
				return ((uint16_t) m_observed == m_target);
			case GREATER_OR_EQUAL:
				return ((uint16_t) m_observed >= m_target);
			default:
				return false;
		}
	}
}
