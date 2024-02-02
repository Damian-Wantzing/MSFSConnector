#pragma once

#include <map>
#include <any>

#include <windows.h>
#include "SimConnect.h"

#include "Dispatcher.h"
#include "SimVar.h"
#include "DefinitionCounter.h"

class SimVarWatcher
{
public:
	SimVarWatcher(HANDLE sim, SIMCONNECT_PERIOD interval, SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER);
	void addSimVar(SimVar simVar);
	void removeSimVar(std::string name);
	void callbackHandler(SIMCONNECT_RECV* data);

private:
	HANDLE sim;
	SIMCONNECT_PERIOD interval;
	SIMCONNECT_OBJECT_ID objectID;
	std::map<std::string, SimVar> simVars;
	DWORD simConnectWatcherID = 0; // The watcherID is only used for the request and should not be used to tell apart two different watchers, since this variable is prone to changing when adding or removing SimVars
};

