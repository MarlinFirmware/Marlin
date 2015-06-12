#ifndef AUTOLEVEL_MANAGER_H
#define AUTOLEVEL_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

class AutoLevelManager : public Subject<bool>
{
	public:
		typedef Singleton<AutoLevelManager> single;

	public:
		AutoLevelManager();

		void state(bool state);
		bool state();

		static void setState();

	private:
		void notify();

	private:
		bool m_state;
};

#endif //AUTOLEVEL_MANAGER_H
