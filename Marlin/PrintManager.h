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

	private:
		void inactivityTriggered();

	private:
		PrinterState_t m_state;

		Time_t m_printing_time;
		uint32_t m_printing_time_raw;

		uint32_t m_inactivity_time;
		bool m_inactivity_flag;

		bool m_known_position;
};

#endif //PRINT_MANAGER_H
