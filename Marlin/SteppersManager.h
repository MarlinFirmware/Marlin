#ifndef STEPPERS_MANAGER_H
#define STEPPERS_MANAGER_H

#include "Subject.h"

class SteppersManager : public Subject<bool>
{
	public:
      static SteppersManager & getInstance();
      static void setState();

		void state(bool state);
		bool state();

	protected:
		SteppersManager();
		~SteppersManager();

	private:
		void notify();
      SteppersManager(SteppersManager const & orig);
      SteppersManager & operator=(SteppersManager & orig);

	private:
		bool m_state;
};

#endif //STEPPERS_MANAGER_H
