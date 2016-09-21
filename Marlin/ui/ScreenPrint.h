///////////////////////////////////////////////////////////////////////////////
/// \file ScreenPrint.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Definition of printing screen.
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

#ifndef SCREEN_PRINT_H
#define SCREEN_PRINT_H

#include <stdint.h>

#include "ScreenMenu.h"
#include "PrintManager.h"

namespace ui
{
	class ScreenPrint : public ScreenMenu , public Observer<float>
	{
		public:
			ScreenPrint(const char * title = 0, Subject<float> * model = 0);
			virtual ~ScreenPrint();

			void init(uint16_t index = 0);

			virtual void draw();

			void update(float value);

		private:
			float m_observed;
			Time_t m_printed_time;
			uint8_t m_previous_minutes;
			uint32_t m_previous_millis;
			uint8_t m_percent_done;
			uint16_t m_target_temperature;
			PrinterState_t m_printing_status;

			char m_temperature_info[9];
			char m_time_info[8];
			char m_pause_info[9];
			
		#if MB(BQ_ZUM_MEGA_3D)
			uint8_t m_bed_observed;
			char m_bed_info[9];	
			bool m_bed_heating;
			bool m_hotend_heating;
		#endif
	};
}
#endif //SCREEN_PRINT_H
