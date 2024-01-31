#pragma once

#include <vector>
#include <Windows.h>

#include "SimConnect.h"

class Dispatcher
{
public:
	Dispatcher(HANDLE sim);
	bool subscribe(void (*callback)(SIMCONNECT_RECV* data));
	bool unsubscribe(void (*callback)(SIMCONNECT_RECV* data));
	void run();

private:
	static void handleStatic(SIMCONNECT_RECV* pData, DWORD cbData, void* context);
	void CALLBACK handle(SIMCONNECT_RECV* pData, DWORD cbData);
	
	std::vector<void(*)(SIMCONNECT_RECV* data)> callbacks;
	HANDLE sim;
};