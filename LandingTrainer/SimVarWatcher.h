#pragma once

#include <map>

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
	std::map<std::string, SimVar> simVars;
	size_t defineID = 0;
};

