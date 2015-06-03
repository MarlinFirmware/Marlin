#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#include "Subject.h"

typedef enum
{
	STOPPED = 0,
	PRINTING,
	PAUSED,
	NUM_PRINTER_STATES
} PrinterState_t;

class PrintManager : public Subject<PrinterState_t>
{
	public:
		static PrintManager & getInstance();

		static void startPrint();
		static void stopPrint();
		static void pausePrint();
		static void resumePrint();

		void state(PrinterState_t state);
		PrinterState_t state();

	protected:
		PrintManager();
		~PrintManager();

	private:
		PrintManager(PrintManager const & orig);
		PrintManager & operator=(PrintManager & orig);

		void notify();

	private:
		PrinterState_t m_state;
};

#endif //PRINT_MANAGER_H