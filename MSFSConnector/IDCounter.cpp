#include "IDCounter.h"

namespace MSFSConnector
{
	DWORD IDCounter::IDCount = 0;

	DWORD IDCounter::getID()
	{
		return IDCount++;
	}
}