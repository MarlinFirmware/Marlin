#ifndef LIGHT_H
#define LIGHT_H

#include "Subject.h"

template <typename T>
	class Light : public Subject<T>
{
	public:
		Light();

		void setState(bool state);

		bool getState();

	private:
		void notify();

	private:
		bool m_state;
};

template <typename T>
Light<T>::Light()
	: Subject<T>()
  , m_state(false)
{ }

template <typename T>
void Light<T>::setState(bool state)
{
	m_state = state;
	notify();
}

template <typename T>
bool Light<T>::getState()
{
	return m_state;
}

template <typename T>
void Light<T>::notify()
{
	if (this->m_observer != 0)
	{
		this->m_observer->update(m_state);
	}
}



#endif //LIGHT_H
