///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSerial.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of serial screen.
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

#include "ScreenSerial.h"

#include "Language.h"

#include "GuiManager.h"

namespace ui
{
	ScreenSerial::ScreenSerial(const char * title, const char * text)
		: Screen(title, MENU)
	{
		memset(m_text, 0, sizeof(m_text));

		if( (text != NULL) && (strlen_P(text) > 0) )
		{
			strncpy(m_text, text, sizeof(m_text));
		}
	}

	ScreenSerial::~ScreenSerial()
	{ }

	void ScreenSerial::init(uint16_t index)
	{
		lcd_disable_encoder();
	}

	void ScreenSerial::left()
	{ }

	void ScreenSerial::right()
	{ }

	void ScreenSerial::press()
	{ }

	void ScreenSerial::draw()
	{
		if ( !m_text || m_needs_drawing)
		{
			m_needs_drawing = false;
			
			//Start painting sequence
			painter.firstPage();
			do
			{
				painter.setColorIndex(1);
				//Paint title on top of screen
				painter.title(MSG_SCREEN_SERIAL_TITLE());

				//Paint bitmap on the left
				uint8_t x_init = painter.coordinateXInit();
				uint8_t y_init = painter.coordinateYInit();
				uint8_t x_end = painter.coordinateXEnd();
				uint8_t y_end = painter.coordinateYEnd();
				uint8_t x_offset = 6;
				uint8_t y_offset = 4;
				painter.setColorIndex(1);
				painter.drawBitmap(x_init + x_offset, y_init + y_offset, serial_width, serial_height, bits_serial);

				//Print state
				Area text_area(x_init + x_offset + serial_width, y_init, 127, 63);
				painter.setWorkingArea(text_area);
				SERIAL_ECHOLN(m_text);
				painter.multiText(m_text);

			} while( painter.nextPage() );
		}
	}

	void ScreenSerial::text(const char * text)
	{
		if( (text != NULL) && (strlen(text) > 0) )
		{
			strncpy(m_text, text, sizeof(m_text));
		}

		m_needs_drawing = true;
	}
}
