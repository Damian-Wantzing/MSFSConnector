#include "DefinitionCounter.h"

size_t DefinitionCounter::definitionCount = 0;

size_t DefinitionCounter::getDefinitionID()
{
	return definitionCount++;
}