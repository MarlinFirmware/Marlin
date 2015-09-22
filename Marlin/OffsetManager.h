#ifndef OFFSET_MANAGER_H
#define OFFSET_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class OffsetManager : public Subject<float>
{
	public:
		typedef Singleton<OffsetManager> single;

	public:
		OffsetManager();

		void offset(float value);
		float offset();
		void saveOffset();

	private:
		void notify();

	private:
		float m_offset;
};

#endif //OFFSET_MANAGER_H
