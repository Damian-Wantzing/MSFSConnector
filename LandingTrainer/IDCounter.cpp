#include "IDCounter.h"

DWORD IDCounter::IDCount = 0;

DWORD IDCounter::getID()
{
	return IDCount++;
}