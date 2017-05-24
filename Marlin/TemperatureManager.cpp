///////////////////////////////////////////////////////////////////////////////
/// \file TemperatureManager.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the temperature subsystem.
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

#include <math.h>

#include "TemperatureManager.h"

#include "Configuration.h"
#include "temperature.h"
#include "Marlin.h"
#include "cardreader.h"
#include "StorageManager.h"
#include "boards.h"
#include "PrintManager.h"

#ifdef DOGLCD
	#include "GuiManager.h"
	#include "TemperatureControl.h"
	#include "ViewManager.h"
	#include "FanManager.h"
	#include "HeatedbedManager.h"
#endif

namespace temp
{
	TemperatureManager::TemperatureManager()
		: Subject<float>()
		, m_target_temperature(0)
		, m_current_temperature(0)
		, m_current_temperature_raw(0)
#ifdef DOGLCD
		, m_bed_target_temperature(0)
		, m_bed_current_temperature(0)
		, m_bed_current_temperature_raw(0)
		, m_control()
#endif // DOGLCD
		, m_blower_control(true)
	{
		setTargetTemperature(0);
		SET_OUTPUT(HEATER_0_PIN);
#ifdef FAN_BLOCK_PIN
		pinMode(FAN_BLOCK_PIN, OUTPUT);
		digitalWrite(FAN_BLOCK_PIN, LOW);
#endif //FAN_BLOCK_PIN

#ifdef DOGLCD
		m_control = new TemperatureControl();
#endif // DOGLCD

#ifdef DOGLCD
	#if (HEATER_BED_PIN > -1)
		if(HeatedbedManager::single::instance().detected())
		{
			SET_OUTPUT(HEATER_BED_PIN);

			// we start with the bed OFF!
			digitalWrite(HEATER_BED_PIN, HIGH);
		}
	#endif
#endif // DOGLCD
	}

	TemperatureManager::~TemperatureManager()
	{
#ifdef DOGLCD
		delete m_control;
#endif // DOGLCD
	}

	void TemperatureManager::init()
	{
		// Init ADC
		ADCSRA = 0x87;
		ADCSRB = 0x08;
		ADMUX = 0x45;

		ADCSRA |= 0x40;
		ADCSRA |= 0x10;
		delay(1);

		while ((ADCSRA & 0x10) == 0) {}
		
		uint16_t initial_raw = 0;
		initial_raw |= ADC;
		initial_raw *= OVERSAMPLENR;

		uint8_t i;
		short (*tt)[][2] = (short (*)[][2]) temptable_99;
		for (i = 1; i < 61; i++)
		{
			if ((short)pgm_read_word(&(*tt)[i][0]) > initial_raw)
			{
				break;
			}
		}

		float initial_temperature = (short)pgm_read_word(&(*tt)[i-1][1]) +
			(initial_raw - (short)pgm_read_word(&(*tt)[i-1][0])) *
			(float)((short)pgm_read_word(&(*tt)[i][1]) - (short)pgm_read_word(&(*tt)[i-1][1])) /
			(float)((short)pgm_read_word(&(*tt)[i][0]) - (short)pgm_read_word(&(*tt)[i-1][0]));

		updateCurrentTemperature(initial_temperature);
		updateCurrentTemperatureRaw(initial_raw);
		updateLUTCache();
		
#ifdef DOGLCD
		if(HeatedbedManager::single::instance().detected())
		{
			ADMUX = 0x47;
			
			ADCSRA |= 0x40;
			ADCSRA |= 0x10;
			delay(1);
			
			uint16_t initial_bed_raw = 0;
			initial_bed_raw |= ADC;
			initial_bed_raw *= OVERSAMPLENR;
			
			for (i = 1; i < 61; i++)
			{
				if ((short)pgm_read_word(&(*tt)[i][0]) > initial_bed_raw)
				{
					break;
				}
			}
			
			float initial_bed_temperature = (short)pgm_read_word(&(*tt)[i-1][1]) +
				(initial_bed_raw - (short)pgm_read_word(&(*tt)[i-1][0])) *
				(float)((short)pgm_read_word(&(*tt)[i][1]) - (short)pgm_read_word(&(*tt)[i-1][1])) /
				(float)((short)pgm_read_word(&(*tt)[i][0]) - (short)pgm_read_word(&(*tt)[i-1][0]));
			
			updateBedCurrentTemperature(initial_bed_temperature);
			updateBedCurrentTemperatureRaw(initial_bed_raw);
			updateBedLUTCache();
			
			ADMUX = 0x45;
		}
#endif // DOGLCD
		configureSetup();
	}
	
	void TemperatureManager::configureSetup()
	{
#ifdef DOGLCD
		if(HeatedbedManager::single::instance().detected())
		{
			TCCR2B = 0x0F;
			OCR2A = 0x7F;
		}
		else
#endif // DOGLCD
		{
			TCCR2B = 0x07;
		}

		TCCR2A = 0x03;
		ADCSRA |= 0x08;

		TIMSK2 = 0x01;
	}

	void TemperatureManager::updateLUTCache()
	{
		uint8_t i;
		short (*tt)[][2] = (short (*)[][2]) temptable_99;

		if ( (m_current_temperature_raw < (34 * OVERSAMPLENR)) ||
		     (m_current_temperature_raw > (1009 * OVERSAMPLENR)) )
		{
			setTargetTemperature(0);
#ifdef DOGLCD
			lcd_disable_button();
			ui::ViewManager::getInstance().activeView(ui::screen_error_temperature);
#endif
			return;
		}

		for (i = 1; i < 61; i++)
		{
			if ((short)pgm_read_word(&(*tt)[i][0]) >= m_current_temperature_raw)
			{
				i -= 2;
				break;
			}
		}

		for (uint8_t j = 0; j < 4; i++, j++)
		{
			m_cache[j].raw = pgm_read_word(&(*tt)[i][0]);
			m_cache[j].temperature = pgm_read_word(&(*tt)[i][1]);
		}
	}

	short TemperatureManager::getRawLUTCache(uint8_t index)
	{
		return m_cache[index].raw;
	}

	short TemperatureManager::getTemperatureLUTCache(uint8_t index)
	{
		return m_cache[index].temperature;
	}

	void TemperatureManager::updateCurrentTemperature(float temp)
	{
#ifdef DOGLCD
		m_control->setCurrentTemperature(temp);
#endif // DOGLCD
		
		if (m_current_temperature != temp)
		{
			m_current_temperature = temp;
			notify();
		}
		fanControl();
	}

	void TemperatureManager::updateCurrentTemperatureRaw(uint16_t temp)
	{
		m_current_temperature_raw = temp;
	}

	uint16_t const & TemperatureManager::getCurrentTemperature()
	{
		m_round_temperature = round(m_current_temperature);
		return m_round_temperature;
	}

	void TemperatureManager::setTargetTemperature(uint16_t target)
	{
		m_target_temperature = target;
	#ifdef DOGLCD
		if(target > HEATER_0_MINTEMP)
		{
			TCCR2A |= 0x20;
		}
		else
		{
			TCCR2A &= 0xDF;
		}
		
		m_control->setTargetControl(target);
	#else
		target_temperature[0] = target;
	#endif // DOGLCD
	}

	uint16_t const & TemperatureManager::getTargetTemperature() const
	{
	#ifdef DOGLCD
		return m_control->getTargetControl();
	#else
		return target_temperature[0];
	#endif // DOGLCD
	}
	
#ifdef DOGLCD
	void TemperatureManager::updateBedLUTCache()
	{
		uint8_t i;
		short (*tt)[][2] = (short (*)[][2]) temptable_99;

		if ( (m_bed_current_temperature_raw < (34 * OVERSAMPLENR)) ||
		     (m_bed_current_temperature_raw > (1009 * OVERSAMPLENR)) )
		{
			setBedTargetTemperature(0);
			m_bed_current_temperature = 0;
			HeatedbedManager::single::instance().disableHeatedbed();
			return;
		}

		for (i = 1; i < 61; i++)
		{
			if ((short)pgm_read_word(&(*tt)[i][0]) >= m_bed_current_temperature_raw)
			{
				i -= 2;
				break;
			}
		}

		for (uint8_t j = 0; j < 4; i++, j++)
  		{
   	 		m_bed_cache[j].raw = pgm_read_word(&(*tt)[i][0]);
  	 		m_bed_cache[j].temperature = pgm_read_word(&(*tt)[i][1]);
		}
	}
	
	short TemperatureManager::getBedRawLUTCache(uint8_t index)
	{
		return m_bed_cache[index].raw;
	}
	
	short TemperatureManager::getBedTemperatureLUTCache(uint8_t index)
	{
		return m_bed_cache[index].temperature;
	}

	void TemperatureManager::updateBedCurrentTemperature(float temp)
	{
		if (m_bed_current_temperature != temp)
		{
			m_bed_current_temperature = temp;
		}
	}

	void TemperatureManager::updateBedCurrentTemperatureRaw(uint16_t temp)
	{
		m_bed_current_temperature_raw = temp;
	}

	uint16_t const & TemperatureManager::getBedCurrentTemperature()
	{
		m_bed_round_temperature = round(m_bed_current_temperature);
		return m_bed_round_temperature;
	}

	void TemperatureManager::setBedTargetTemperature(uint16_t target)
	{
	#if MB(BQ_ZUM_MEGA_3D)
		if(HeatedbedManager::single::instance().detected())
		{
			m_bed_target_temperature = target;
			m_bed_target_temperature = constrain (m_bed_target_temperature, 0, BED_MAXTEMP);
		}
	#endif // MB(BQ_ZUM_MEGA_3D)
	}

	uint16_t const & TemperatureManager::getBedTargetTemperature() const
	{
		return m_bed_target_temperature;
	}
#endif // DOGLCD
	
	void TemperatureManager::notify()
	{
		if (this->m_observer != 0)
		{
			this->m_observer->update(m_current_temperature);
		}
	}
	
	void TemperatureManager::setBlowerControlState(bool state)
	{
		m_blower_control = state;
	}

	void TemperatureManager::fanControl()
	{
		if (m_current_temperature > min_temp_cooling)
		{
		#ifdef FAN_BLOCK_PIN
			digitalWrite(FAN_BLOCK_PIN, HIGH);
		#endif //FAN_BLOCK_PIN
			if (m_target_temperature < min_temp_cooling)
			{
				if (m_blower_control == true)
				{
					fanSpeed = 255;
				}
			}
			else
			{
				if (m_blower_control == true)
				{
					fanSpeed = 0;
				}
			}
		}
		else
		{
		#ifdef FAN_BLOCK_PIN
			digitalWrite(FAN_BLOCK_PIN, LOW);
		#endif //FAN_BLOCK_PIN
			if (m_blower_control == true)
			{
				fanSpeed = 0;
			}
		}
#ifdef FAN_BLOWER_PIN
		analogWrite(FAN_BLOWER_PIN,fanSpeed);
#endif

#ifdef FAN_BOX_PIN
	    if(FanManager::single::instance().state() == true)
	    {
			if(!card.isFileOpen() && PrintManager::single::instance().state() != SERIAL_CONTROL)
			{
				if(m_current_temperature > min_temp_cooling)
				{
				digitalWrite(FAN_BOX_PIN, HIGH);
				}
				else
				{
				digitalWrite(FAN_BOX_PIN, LOW);
				}
			}
	    }
#endif //FAN_BOX_PIN
	}

	void TemperatureManager::manageTemperatureControl()
	{
	#ifdef DOGLCD
		updateLUTCache();
		#if MB(BQ_ZUM_MEGA_3D)
			if(HeatedbedManager::single::instance().detected())
			{
				updateBedLUTCache();
			}
		#endif // MB(BQ_ZUM_MEGA_3D)
	#else
		manage_heater();
	#endif
	}
	
	void TemperatureManager::heatBed()
	{
#ifdef DOGLCD
#if HEATER_BED_PIN > -1
		if(m_bed_current_temperature < m_bed_target_temperature)
		{
			digitalWrite(HEATER_BED_PIN, LOW);
		}
		else
		{
			digitalWrite(HEATER_BED_PIN, HIGH);
		}
#endif // HEATER_BED_PIN > -1
#endif // DOGLCD
	}
}

static bool control_flag = false;

#ifdef DOGLCD

ISR(TIMER2_OVF_vect)
{
	static uint8_t temp_counter = 0;
	static uint16_t bed_control_counter = 0;
	
	if (control_flag == true)
	{
		temp::TemperatureManager::single::instance().m_control->manageControl();
		control_flag = false;
	}

	temp_counter++;
	if (temp_counter == 1)
	{
		ADCSRA |= 0x40;
		temp_counter = 0;
	}
#if MB(BQ_ZUM_MEGA_3D)
	if(HeatedbedManager::single::instance().getMode() != eeprom::HEATEDBED_OFF)
	{
		bed_control_counter++;
		if(bed_control_counter == BED_UPDATE_FREQUENCY)
		{
			temp::TemperatureManager::single::instance().heatBed();
			bed_control_counter = 0;			
		}
	}
#endif // MB(BQ_ZUM_MEGA_3D)
}

ISR(ADC_vect)
{
	static uint8_t	target_sensor = 0;

	static uint8_t sample_number = 0;
	static uint16_t accumulate = 0;
#if MB(BQ_ZUM_MEGA_3D)
	static uint8_t bed_sample_number = 0;
	static uint16_t bed_accumulate = 0;
#endif // MB(BQ_ZUM_MEGA_3D)
	uint16_t current_sample = ADC;

	switch(target_sensor)
	{
		// //Hotend
		case 0:	
		accumulate += current_sample;
				sample_number++;

				if (sample_number == OVERSAMPLENR)
				{
					temp::TemperatureManager::single::instance().updateCurrentTemperatureRaw(accumulate);

					for (uint8_t i = 0; i < 4; i++)
					{
						if ( accumulate < temp::TemperatureManager::single::instance().getRawLUTCache(i) )
						{
							float temperature = temp::TemperatureManager::single::instance().getTemperatureLUTCache(i-1) +
								(accumulate - temp::TemperatureManager::single::instance().getRawLUTCache(i-1)) *
								( (float) (temp::TemperatureManager::single::instance().getTemperatureLUTCache(i) - temp::TemperatureManager::single::instance().getTemperatureLUTCache(i-1)) ) /
								( (float) (temp::TemperatureManager::single::instance().getRawLUTCache(i) - temp::TemperatureManager::single::instance().getRawLUTCache(i-1)) );

							temp::TemperatureManager::single::instance().updateCurrentTemperature(temperature);
							break;
						}
					}

					control_flag = true;
					sample_number = 0;
					accumulate = 0;
				}
					
			#if MB(BQ_ZUM_MEGA_3D)
				if(HeatedbedManager::single::instance().detected())
				{
					ADMUX = 0x47;
					target_sensor = 1;
				}
			#endif MB(BQ_ZUM_MEGA_3D)
				break;
				
	#if MB(BQ_ZUM_MEGA_3D)
		case 1: 
				bed_accumulate += current_sample;
				++bed_sample_number;
				if (bed_sample_number == OVERSAMPLENR)
				{
					temp::TemperatureManager::single::instance().updateBedCurrentTemperatureRaw(bed_accumulate);

					for (uint8_t i = 0; i < 4; i++)
					{
						if ( bed_accumulate < temp::TemperatureManager::single::instance().getBedRawLUTCache(i) )
						{
							float temperature = temp::TemperatureManager::single::instance().getBedTemperatureLUTCache(i-1) +
								(bed_accumulate - temp::TemperatureManager::single::instance().getBedRawLUTCache(i-1)) *
								( (float) (temp::TemperatureManager::single::instance().getBedTemperatureLUTCache(i) - temp::TemperatureManager::single::instance().getBedTemperatureLUTCache(i-1)) ) /
								( (float) (temp::TemperatureManager::single::instance().getBedRawLUTCache(i) - temp::TemperatureManager::single::instance().getBedRawLUTCache(i-1)) );
							
							temp::TemperatureManager::single::instance().updateBedCurrentTemperature(temperature);
							break;
						}
					}
					bed_sample_number = 0;
					bed_accumulate = 0;
				}
				
				ADMUX = 0x45; //hotend sensor
				target_sensor = 0;
				break;
		#endif // MB(BQ_ZUM_MEGA_3D)
	}	
}

#endif // DOGLCD
