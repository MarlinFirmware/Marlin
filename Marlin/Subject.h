#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

template <typename T>
	class Subject
{
	public:
      void attach (Observer<T> * observer)
		{
  			m_observer = observer;
		};

   protected:
      Observer<T> * m_observer;
};
#endif //SUBJECT_H
