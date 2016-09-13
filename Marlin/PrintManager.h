///////////////////////////////////////////////////////////////////////////////
/// \file PrintManager.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the state of the printer.
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

#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#include <stdint.h>

#include "Singleton.h"
#include "Subject.h"

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
} Time_t;

typedef enum
{
	PRINTING,
	PAUSED,
	PAUSING,
	STOPPED,
	HOMING,
	LEVELING,
	READY,
	COMPLETE,
	SERIAL_CONTROL,
	INITIALIZING,
	NUM_PRINTER_STATES
} PrinterState_t;

class PrintManager : public Subject<PrinterState_t>
{
	public:
		typedef Singleton<PrintManager> single;

	public:
		PrintManager();

		PrinterState_t state();
		void state(PrinterState_t state);

		uint32_t getInactivityTime();
		void resetInactivityTime();
		bool getInactivityFlag();
		void setInactivityFlag(bool state);

		bool getKnownPosition();
		void setKnownPosition(bool state);

		void notify();

		static void startPrint();
		static void stopPrint();
		static void pausePrint();
		static void resumePrint();
		static void togglePause();
		static void endPrint();
		static Time_t completePrint();

		static void startTime();
		static void updateTime();
		static Time_t printingTime();

		static void resetInactivity();
		static void updateInactivity();

		static bool knownPosition();
		static void knownPosition(bool state);

		bool getBedMissingFlag();
		void setBedMissingFlag(bool flag);
		
		static bool checkPTFEMaintenance();

	private:
		void inactivityTriggered();

	private:
		PrinterState_t m_state;

		Time_t m_printing_time;
		uint32_t m_printing_time_raw;

		uint32_t m_inactivity_time;
		bool m_inactivity_flag;

		bool m_known_position;
		
		bool m_bed_missing_flag;
};

#endif //PRINT_MANAGER_H
