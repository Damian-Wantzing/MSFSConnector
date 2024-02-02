#include "DefinitionCounter.h"

DWORD DefinitionCounter::definitionCount = 0;

DWORD DefinitionCounter::getDefinitionID()
{
	return definitionCount++;
}