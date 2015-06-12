#ifndef STEPPERS_MANAGER_H
#define STEPPERS_MANAGER_H

#include "Singleton.h"
#include "Subject.h"
#include "Stepper.h"

class SteppersManager : public Subject<bool>
{
	public:
		typedef enum
		{
			AXIS_X = 0,
			AXIS_Y,
			AXIS_Z,
			EXTRUDER,
			NUM_STEPPERS
		} Stepper_t;

		typedef Singleton<SteppersManager> single;

	public:
		SteppersManager();

		static void disableAllSteppers();

		void enableStepper(Stepper_t stepper);
		void disableStepper(Stepper_t stepper);
		void notify();

	private:
		bool m_steppers_disabled;

		Stepper m_steppers[NUM_STEPPERS];
};

#endif //STEPPERS_MANAGER_H
