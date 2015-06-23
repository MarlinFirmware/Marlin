#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

typedef enum
{
	PRINTING,
	PAUSED,
	STOPPED,
	NUM_PRINTER_STATES
} PrinterState_t;

class PrintManager : public Subject<PrinterState_t>
{
	public:
		typedef Singleton<PrintManager> single;

	public:
		PrintManager();

		PrinterState_t state();

		static void startPrint();
		static void stopPrint();
		static void pausePrint();
		static void resumePrint();
		static void togglePause();

		void notify();

	private:
		void state(PrinterState_t state);

	private:
		PrinterState_t m_state;
};

#endif //PRINT_MANAGER_H
