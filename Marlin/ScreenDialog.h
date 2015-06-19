#ifndef SCREEN_DIALOG_H
#define SCREEN_DIALOG_H

#include "Screen.h"
#include "Functor.h"

namespace screen
{
	template <typename R, typename... Args>
		class ScreenDialog : public Screen , public Functor<R, Args ...>
	{
		public:
			ScreenDialog(const char * title, const char * message, const char * box, typename Functor<R, Args...>::FuncPtr fptr = do_nothing);
			virtual ~ScreenDialog();

			void press();
			void draw();

		protected:
			const char * m_message;
			const char * m_box;
	};

	template <typename R, typename... Args>
	ScreenDialog<R, Args...>::ScreenDialog(const char * title, const char * message, const char * box, typename Functor<R, Args...>::FuncPtr fptr)
		: Screen(title, DIALOG)
		, Functor<R, Args...>(fptr)
		, m_message(message)
		, m_box(box)
	{ }

	template <typename R, typename... Args>
	ScreenDialog<R, Args...>::~ScreenDialog()
	{ }

	template <typename R, typename... Args>
	void ScreenDialog<R, Args...>::press()
	{
		this->action();
		ViewManager::getInstance().activeView(m_next_screen);
	}

	template <typename R, typename... Args>
	void ScreenDialog<R, Args...>::draw()
	{
		painter.firstPage();
		do
		{
			painter.title(m_title);
			painter.multiText(m_message);
			painter.box(m_box);

		} while ( painter.nextPage() );
	}
}

#endif //SCREEN_DIALOG_H
