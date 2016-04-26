///////////////////////////////////////////////////////////////////////////////
/// \file StatsManager.h
///
/// \author Koldo Imanol de Miguel
///
/// \brief Management class for management of printing statistics
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

#ifndef STATS_MANAGER_H
#define STATS_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "PrintManager.h"

class StatsManager
{
	public:
		typedef Singleton<StatsManager> single;

	public:
		StatsManager();
		
		void loadStats();
		
		void updateTotalTime(Time_t printTime);
		void increaseTotalPrints();
		void increaseSuccededPrints();
		
		inline uint16_t getHours() { return m_hours; };
		inline uint8_t getMinutes() { return m_minutes; };
		inline uint16_t getFailedPrints() { return (m_total_prints - m_succeded); };
		inline uint16_t getSuccededPrints() { return m_succeded; };
		inline uint16_t getTotalPrints() { return m_total_prints; };
		
	private:
		void resetStats();
	
		uint16_t m_hours;
		uint8_t m_minutes;
		uint16_t m_total_prints;
		uint16_t m_succeded;
		bool m_print_started;
};

#endif //STATS_MANAGER_H
