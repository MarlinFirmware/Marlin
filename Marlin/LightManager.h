#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Subject.h"

class LightManager : public Subject<bool>
{
	public:
		static LightManager & getInstance();
		static void setState();

		void state(bool state);
		bool state();

	protected:
		LightManager();
		~LightManager();

	private:
		void notify();
		LightManager(LightManager const & orig);
		LightManager & operator=(LightManager & orig);

	private:
		bool m_state;
};

#endif //LIGHT_MANAGER_H
