#include "SimVarWatcher.h"

SimVarWatcher::SimVarWatcher(HANDLE sim, SIMCONNECT_PERIOD interval, SIMCONNECT_OBJECT_ID objectID)
{
	this->sim = sim;
	this->interval = interval;
	this->objectID = objectID;

	watcherID = DefinitionCounter::getDefinitionID();
	Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) {this->callbackHandler(data); });
}

void SimVarWatcher::addSimVar(SimVar simVar)
{
	HRESULT hr = SimConnect_AddToDataDefinition(sim, watcherID, simVar.name.c_str(), simVar.unitType.c_str());
	if (FAILED(hr)) throw std::runtime_error("error adding simvar to data definition");
	simVars.insert(std::pair<std::string, SimVar>(simVar.name, simVar));
	SimConnect_RequestDataOnSimObject(sim, watcherID, watcherID, objectID, interval);
}

void SimVarWatcher::removeSimVar(std::string name)
{
	SimConnect_ClearDataDefinition(sim, watcherID);
	simVars.erase(name);

	// if there are no more simvars, we return
	// if we do not return here we will attempt to request a non existing data definition and msfs crashes
	if (simVars.empty()) return;

	for (std::unordered_map<std::string, SimVar>::iterator it = simVars.begin(); it != simVars.end(); ++it)
	{
		SimConnect_AddToDataDefinition(sim, watcherID, it->second.name.c_str(), it->second.unitType.c_str());
	}

	SimConnect_RequestDataOnSimObject(sim, watcherID, watcherID, objectID, interval);
}

void SimVarWatcher::callbackHandler(SIMCONNECT_RECV* data)
{
	// if this is not simobject data, we are not interested
	if (data->dwID != SIMCONNECT_RECV_ID_SIMOBJECT_DATA) return;

	SIMCONNECT_RECV_SIMOBJECT_DATA* objectData = (SIMCONNECT_RECV_SIMOBJECT_DATA*) data;

	// if it is not the correct define id, we are not interested
	if (objectData->dwDefineID != watcherID) return;

	printf("class defineID: %d; data requestID: %d; data defineID: %d\n", (int)this->watcherID, (int)objectData->dwRequestID, (int)objectData->dwDefineID);
}
