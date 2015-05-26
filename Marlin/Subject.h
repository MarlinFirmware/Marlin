#ifndef SUBJECT_H
#define SUBJECT_H

class Observer;

class Subject
{
	public:
      void attach (Observer * observer);

   protected:
      Observer * m_observer;
};
#endif //SUBJECT_H
