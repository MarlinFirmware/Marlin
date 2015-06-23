#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

typedef enum
{
	SD_IS_NOT_INSERTED,
	SD_IS_INSERTED,
	NUM_SD_STATES
} SDState_t;

class SDManager : public Subject<SDState_t>
{
	public:
		typedef Singleton<SDManager> single;

	public:
		SDManager();

		static void updateSDStatus();

		void notify();

	private:
		void state(SDState_t state);
		SDState_t state();

	private:
		SDState_t m_state;
};

#endif //SD_MANAGER_H