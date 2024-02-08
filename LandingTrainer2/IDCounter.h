#pragma once

#include <Windows.h>

namespace MSFSConnector
{
	class IDCounter
	{
	public:
		static DWORD getID();
	private:
		static DWORD IDCount;
	};
}