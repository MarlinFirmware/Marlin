///////////////////////////////////////////////////////////////////////////////
/// \file TemperatureControl.cpp
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

#include "TemperatureControl.h"
#include "Configuration.h"


namespace temp
{
	TemperatureControl::TemperatureControl()
		: m_current_temperature(0)
		, m_control_output(0)
		, m_target_temperature(0)
	{
		m_kp = DEFAULT_Kp; 
		m_ki = DEFAULT_Ki * PID_dT;
		m_kd = DEFAULT_Kd / PID_dT;
		m_kb = DEFAULT_Kb * PID_dT;
		m_bW = DEFAULT_bW;
		m_cW = DEFAULT_cW;
	}

	TemperatureControl::~TemperatureControl()
	{ }

	void TemperatureControl::setCurrentTemperature(uint16_t temp)
	{
		m_current_temperature = temp;
	}

	void TemperatureControl::setTargetControl(uint16_t const & target)
	{
		m_target_temperature = target;
	}

	const uint16_t & TemperatureControl::getTargetControl() const
	{
		return m_target_temperature;
	}

	void TemperatureControl::manageControl()
	{
		float pTerm = 0;
		static float iTerm = 0;
		float dTerm = 0;
		static float bTerm = 0;
		float errorP = 0;
		float errorI = 0;
		float errorD = 0;
		static float lastErrorD = 0;
		uint16_t control_input = 0;
		float control_output = 0;

		static uint32_t prev_millis = millis();
		static uint16_t max_time = 0;

		control_input = constrain (m_target_temperature, 0, HEATER_0_MAXTEMP);

		if (control_input <= HEATER_0_MINTEMP)
		{
			m_control_output = 0;
			iTerm = 0;
			lastErrorD = 0;
		} 
		else
		{
			errorP = control_input * m_bW - m_current_temperature;
			errorI = control_input - m_current_temperature;
			errorD = control_input * m_cW - m_current_temperature;
			pTerm = m_kp * errorP;
			iTerm += errorI * m_ki + bTerm;
			dTerm = m_kd *(errorD - lastErrorD);
			control_output = pTerm + iTerm + dTerm;
			
			if ( control_output >= PID_MAX )
			{
				m_control_output = PID_MAX;		
			}
			else if ( control_output <= 0 )
			{
				m_control_output = 0;
			}
			else
			{
				m_control_output = control_output;
			}

			bTerm = m_kb * ( m_control_output - control_output );
			lastErrorD = errorD;
		}
		OCR2B = (uint8_t) m_control_output;
	}
}
