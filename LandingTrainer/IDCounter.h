#pragma once

#include <Windows.h>

class IDCounter
{
public:
	static DWORD getID();
private:
	static DWORD IDCount;
};

