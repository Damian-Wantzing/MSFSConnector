#pragma once

#include <Windows.h>

class DefinitionCounter
{
public:
	static DWORD getDefinitionID();
private:
	static DWORD definitionCount;
};

