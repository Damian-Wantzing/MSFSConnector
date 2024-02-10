#include "IDCounter.h"

namespace MSFSConnector
{
	std::atomic<DWORD> IDCounter::IDCount = 1;

	DWORD IDCounter::getID()
	{
		DWORD idToReturn = IDCount.fetch_add(1);
		return idToReturn;
	}
}