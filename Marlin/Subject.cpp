#include "Subject.h"
#include "Observer.h"

void Subject::attach (Observer * observer)
{
   m_observer = observer;
}
