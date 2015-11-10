///////////////////////////////////////////////////////////////////////////////
/// \file ScreenSerial.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of serial screen.
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

#ifndef SCREEN_SERIAL_H
#define SCREEN_SERIAL_H

#include <stdint.h>

#include "Screen.h"

namespace ui
{
	class ScreenSerial : public Screen
	{
		public:
			ScreenSerial(const char * title = 0, const char * text = 0);
			virtual ~ScreenSerial();

			void init(uint16_t index = 0);
			void left();
			void right();
			void press();
			virtual void draw();
			void text(const char * text);

		protected:
			uint16_t m_index;
			uint16_t m_num_items;
			char m_text[64];
	};
}
#endif //SCREEN_SERIAL_H
