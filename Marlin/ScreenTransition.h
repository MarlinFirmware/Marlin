#ifndef SCREEN_TRANSITION_H
#define SCREEN_TRANSITION_H

#include "Screen.h"
#include "Functor.h"
#include "PrintManager.h"


namespace screen
{
	class ScreenTransition : public Screen , public Functor<void>, public Observer<PrinterState_t>
	{
		public:
			ScreenTransition(const char * title, const char * message, const char * box, typename Functor<void>::FuncPtr fptr = do_nothing,  Subject<PrinterState_t> * model = 0);
			virtual ~ScreenTransition();

			void init();
			void draw();
			void update(PrinterState_t value);

		private:
			const char * m_message;
			const char * m_box;
			PrinterState_t m_printing_status;
	};
}

#endif //SCREEN_TRANSITION_H
