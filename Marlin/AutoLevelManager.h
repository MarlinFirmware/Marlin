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
		void notify();

		static void setState();


	private:
		bool m_state;
};

#endif //AUTOLEVEL_MANAGER_H
