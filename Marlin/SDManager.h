#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

class SDManager : public Subject<bool>
{
	public:
		typedef Singleton<SDManager> single;

	public:
		SDManager();

		static void updateSDStatus();

		bool const & isInserted() const;
		void setInserted(bool state);

		void notify();

	private:
		bool m_is_inserted;
};

#endif //SD_MANAGER_H
