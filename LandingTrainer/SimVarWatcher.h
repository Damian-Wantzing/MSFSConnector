#pragma once

#include <unordered_map>

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
	std::unordered_map<std::string, SimVar> simVars;
	size_t watcherID = 0;
};

