#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

template <typename T>
	class Subject
{
	public:
		void attach (Observer<T> * observer);
		void dettach();
		virtual void notify() = 0;

   protected:
      Observer<T> * m_observer;
};

template <typename T>
void Subject<T>::attach (Observer<T> * observer)
{
	m_observer = observer;
};

template <typename T>
void Subject<T>::dettach()
{
	m_observer = 0;
};
#endif //SUBJECT_H
