#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;

class Observer
{
	public:
		Observer(Subject * model);
		virtual void update(bool state) = 0;

	public:
		Subject * m_model;
};

#endif //OBSERVER_H
