///////////////////////////////////////////////////////////////////////////////
/// \file TemperatureManager.h
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

#ifndef TEMPERATURE_MANAGER_H
#define TEMPERATURE_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "Subject.h"

namespace temp
{
	class TemperatureControl;


	// Temperature parameters
	const static uint8_t default_temp_change_filament = 200;
	const static uint8_t min_temp_cooling = 50;
	const static uint8_t min_temp_operation = 170;
	const static uint8_t max_temp_operation = 250;

	class TemperatureManager : public Subject<float>
	{
		public:
			typedef Singleton<TemperatureManager> single;

		struct LookUpTableEntry
		{
			LookUpTableEntry()
			: raw(0)
			, temperature(0)
			{ };

			short raw;
			short temperature;
		};

		public:
			TemperatureManager();
			~TemperatureManager();

			void init();
			void configureSetup();

			void updateLUTCache();
			short getRawLUTCache(uint8_t index);
			short getTemperatureLUTCache(uint8_t index);
		
			void updateCurrentTemperature(float temp);
			void updateCurrentTemperatureRaw(uint16_t temp);

			uint16_t const & getCurrentTemperature();
			void setTargetTemperature(uint16_t target);
			uint16_t const & getTargetTemperature() const;
			
			void updateBedLUTCache();
			short getBedRawLUTCache(uint8_t index);
			short getBedTemperatureLUTCache(uint8_t index);
					
			void updateBedCurrentTemperature(float temp);
			void updateBedCurrentTemperatureRaw(uint16_t temp);
			
			uint16_t const & getBedCurrentTemperature();
			void setBedTargetTemperature(uint16_t target);
			uint16_t const & getBedTargetTemperature() const;		
			
			void notify();
			void setBlowerControlState(bool state);
			void fanControl();

			void manageTemperatureControl();
			
			//test functions
			void heatBed();
			
		private:

		public:

			TemperatureControl * m_control;	

		private:
			float m_target_temperature;
			float m_current_temperature;
			uint16_t m_current_temperature_raw;

			uint16_t m_bed_target_temperature;
			float m_bed_current_temperature;
			uint16_t m_bed_current_temperature_raw;			

			bool m_blower_control;

			LookUpTableEntry m_cache[4];
			uint16_t m_round_temperature;
			
			LookUpTableEntry m_bed_cache[4];
			uint16_t m_bed_round_temperature;
	};
}

#endif //TEMPERATURE_MANAGER_H
