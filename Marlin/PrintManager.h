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
	NUM_PRINTER_STATES
} PrinterState_t;

class PrintManager : public Subject<PrinterState_t>
{
	public:
		typedef Singleton<PrintManager> single;

	public:
		PrintManager();

		PrinterState_t state();

		static Time_t printingTime();

		static void startPrint();
		static void stopPrint();
		static void pausePrint();
		static void resumePrint();
		static void endPrint();

		static void togglePause();

		static void startTime();
		static void updateTime();

		void notify();


		void state(PrinterState_t state);

	private:
		PrinterState_t m_state;

		Time_t m_printing_time;
		uint32_t m_printing_time_raw;
};

#endif //PRINT_MANAGER_H
