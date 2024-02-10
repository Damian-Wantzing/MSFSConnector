#pragma once

#include <atomic>

#include <Windows.h>

namespace MSFSConnector
{
	class IDCounter
	{
	public:
		static DWORD getID();
	private:
		static std::atomic<DWORD> IDCount;
	};
}