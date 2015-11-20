///////////////////////////////////////////////////////////////////////////////
/// \file TemperatureControl.h
///
/// \author Victor Andueza
///
/// \brief Temperature controller class.
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

#ifndef TEMPERATURE_CONTROL_H
#define TEMPERATURE_CONTROL_H

#include <stdint.h>

namespace temp
{
	class TemperatureControl 
	{
		public:
			TemperatureControl();
			~TemperatureControl();

			void setCurrentTemperature(uint16_t temp);
			void setTargetControl(uint16_t const & target);
			const uint16_t & getTargetControl() const;
			void manageControl();

		public:
			float m_control_output;

		private:
			float m_current_temperature;
			uint16_t m_target_temperature;	
			float m_kp;
			float m_ki;
			float m_kd;
			float m_kb;
			float m_bW;
			float m_cW;
	};
}


#endif //TEMPERATURE_CONTROL_H
