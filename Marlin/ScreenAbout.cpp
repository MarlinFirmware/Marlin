///////////////////////////////////////////////////////////////////////////////
/// \file ScreenAbout.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of About Screen.
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

#include "ScreenAbout.h"
#include "GuiBitmaps_witbox_2.h"
#include "language.h"

namespace screen
{
	ScreenAbout::ScreenAbout(const char * title, const char * message, const char * box, const unsigned char * bitmap)
		: ScreenDialog<void>(title, message, box)
		, m_bitmap(bitmap)
	{ }

	ScreenAbout::~ScreenAbout()
	{ }

	void ScreenAbout::draw()
	{
		//Start painting sequence
		painter.firstPage();
		do
		{
			painter.setColorIndex(1);
			//Paint title on top of screen
			painter.title(m_title);
			//Paint selection box on bottom of screen
			painter.box(m_box);
			//Paint bitmap on the left
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t x_end = painter.coordinateXEnd();
			uint8_t y_end = painter.coordinateYEnd();
			uint8_t x_offset = 6;
			uint8_t y_offset = 4;
			painter.setColorIndex(1);
			painter.drawBitmap(x_init + x_offset, y_init + y_offset, logo_width, logo_height, bits_logo_about);
			//Paint text on the screen
			painter.setPrintPos(x_offset + logo_width + 8, y_init + 8);
			painter.print(MACHINE_NAME);
			painter.setPrintPos(x_offset + logo_width + 8, y_init + 24);
			painter.print(BUILD_VER);
		} while( painter.nextPage() );
	}
}
