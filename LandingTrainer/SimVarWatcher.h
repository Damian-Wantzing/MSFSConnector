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

	template <typename T>
	T get(std::string name) // TODO: get the simvar value for a simvar name. This function will check if the simvar exists, if the provided type is correct for the simvar and then return the result.
	{
		if (simVarResults.count(name) == 0 || simVarResults[name].type() != typeid(T)) throw std::runtime_error("Provided type does not match the SimVar type");
		return std::any_cast<T>(simVarResults[name]);
	}
private:
	HANDLE sim;
	SIMCONNECT_PERIOD interval;
	SIMCONNECT_OBJECT_ID objectID;
	std::map<std::string, SimVar> simVars;
	std::map<std::string, std::any> simVarResults;
	DWORD simConnectWatcherID = 0; // The watcherID is only used for the request and should not be used to tell apart two different watchers, since this variable is prone to changing when adding or removing SimVars
};

