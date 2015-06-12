#ifndef OBSERVER_H
#define OBSERVER_H

template <typename T> class Subject;

template <typename T>
	class Observer
{
	public:
		Observer(Subject<T> * model);
		virtual void update(T value) = 0;
		void connect();

	public:
		Subject<T> * m_model;
};

#include "Subject.h"

template <typename T>
Observer<T>::Observer(Subject<T> * model)
	: m_model(model)
{ 
	if (m_model != 0)
	{
		m_model->attach(this);
	}
}

template <typename T>
void Observer<T>::connect()
{
	if (m_model != 0)
	{
		m_model->notify();
	}
}
#endif //OBSERVER_H
