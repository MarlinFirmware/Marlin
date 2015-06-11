#ifndef AUTOLEVEL_MANAGER_H
#define AUTOLEVEL_MANAGER_H

#include "Subject.h"

class AutoLevelManager : public Subject<bool>
{
	public:
		static AutoLevelManager & getInstance();
		static void setState();

		void state(bool state);
		bool state();

	protected:
		AutoLevelManager();
		~AutoLevelManager();

	private:
		void notify();
		AutoLevelManager(AutoLevelManager const & orig);
		AutoLevelManager & operator=(AutoLevelManager & orig);

	private:
		bool m_state;
};

#endif //AUTOLEVEL_MANAGER_H
