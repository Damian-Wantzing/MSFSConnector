#include "SimVarWatcher.h"
#include <iostream>

SimVarWatcher::SimVarWatcher(HANDLE sim, SIMCONNECT_PERIOD interval, SIMCONNECT_OBJECT_ID objectID)
{
	this->sim = sim;
	this->interval = interval;
	this->objectID = objectID;

	simConnectWatcherID = DefinitionCounter::getDefinitionID();
	Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) {this->callbackHandler(data); });
}

void SimVarWatcher::addSimVar(SimVar simVar)
{
	this->simVars.insert(std::pair<std::string, SimVar>(simVar.name, simVar));

	SimConnect_RequestDataOnSimObject(sim, simConnectWatcherID, simConnectWatcherID, objectID, SIMCONNECT_PERIOD_NEVER);
	SimConnect_AddToDataDefinition(sim, simConnectWatcherID, simVar.name.c_str(), simVar.unitType.c_str());
	SimConnect_RequestDataOnSimObject(sim, simConnectWatcherID, simConnectWatcherID, objectID, interval);
}

void SimVarWatcher::removeSimVar(std::string name)
{
	// TODO: to remove a simvar we have to complete clear the definition and send a new request with a new ID
	simVars.erase(name);
	
	// remove our entire current data definition, since we will be making a new one
	SimConnect_ClearDataDefinition(sim, simConnectWatcherID);

	// get a new watcher ID for the new request
	simConnectWatcherID = DefinitionCounter::getDefinitionID();

	// if there are no more simvars, we return
	// if we do not return here we will attempt to request a non existing data definition and msfs crashes
	if (simVars.empty()) return;

	for (std::map<std::string, SimVar>::iterator it = simVars.begin(); it != simVars.end(); ++it)
	{
		SimConnect_AddToDataDefinition(sim, simConnectWatcherID, it->second.name.c_str(), it->second.unitType.c_str());
	}

	SimConnect_RequestDataOnSimObject(sim, simConnectWatcherID, simConnectWatcherID, objectID, interval);
}

void SimVarWatcher::callbackHandler(SIMCONNECT_RECV* data)
{
	// if this is not simobject data, we are not interested
	if (data->dwID != SIMCONNECT_RECV_ID_SIMOBJECT_DATA) return;

	SIMCONNECT_RECV_SIMOBJECT_DATA* objectData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)data;

	// if it is not the correct define id, we are not interested
	if (objectData->dwRequestID != simConnectWatcherID) return;

	double* dataArray = reinterpret_cast<double*>(&objectData->dwData);
	for (DWORD i = 0; i < objectData->dwDefineCount; i++) {
		printf("dwData[%d]: %f\n", i, dataArray[i]);
	}
}