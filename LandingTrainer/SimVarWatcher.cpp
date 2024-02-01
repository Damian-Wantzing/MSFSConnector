#include "SimVarWatcher.h"

SimVarWatcher::SimVarWatcher(HANDLE sim, SIMCONNECT_PERIOD interval, SIMCONNECT_OBJECT_ID objectID)
{
	this->sim = sim;
	defineID = DefinitionCounter::getDefinitionID();

	SimConnect_RequestDataOnSimObject(sim, 0, defineID, objectID, interval);

	Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) {this->callbackHandler(data); });
}

void SimVarWatcher::addSimVar(SimVar simVar)
{
	HRESULT hr = SimConnect_AddToDataDefinition(sim, defineID, simVar.name.c_str(), simVar.unitType.c_str());
	if (FAILED(hr)) throw std::runtime_error("error adding simvar to data definition");
}

void SimVarWatcher::removeSimVar(std::string name)
{

}

void SimVarWatcher::callbackHandler(SIMCONNECT_RECV* data)
{
	printf("Handling callback");
}
