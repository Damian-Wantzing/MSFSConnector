#pragma once

#include <any>
#include <Windows.h>

#include "AtomicMap.h"
#include "IDCounter.h"
#include "Dispatcher.h"

class InputEventSender
{
public:
	InputEventSender(HANDLE sim);
	void callbackHandler(SIMCONNECT_RECV* data);
	void sendEvent(std::string name, std::any value, DWORD valueSize);

private:
	HANDLE sim;
	AtomicMap<std::string, UINT64> eventHashes;
};

