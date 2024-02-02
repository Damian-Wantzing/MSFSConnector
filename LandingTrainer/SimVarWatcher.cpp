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
	// does the simVar already exist?
	for (std::list<SimVar>::iterator it = simVars.begin(); it != simVars.end(); ++it) if (it->name == simVar.name) return;

	// TODO: add rwMutex since this can be accessed in multiple threads
	this->simVars.push_back(simVar);

	SimConnect_RequestDataOnSimObject(sim, simConnectWatcherID, simConnectWatcherID, objectID, SIMCONNECT_PERIOD_NEVER);
	SimConnect_AddToDataDefinition(sim, simConnectWatcherID, simVar.name.c_str(), simVar.unitType.c_str());
	SimConnect_RequestDataOnSimObject(sim, simConnectWatcherID, simConnectWatcherID, objectID, interval);
}

void SimVarWatcher::removeSimVar(std::string name)
{
	bool exists = false;

	// to remove a simvar we have to complete clear the definition and send a new request with a new ID
	// TODO: add rwMutex since this can be accessed in multiple threads
	for (std::list<SimVar>::iterator it = simVars.begin(); it != simVars.end(); ++it)
	{
		if (it->name != name) continue;
		simVars.erase(it);
		exists = true;
		break;
	}

	// if the simvar does not exist we can return
	if (!exists) return;

	// remove our entire current data definition, since we will be making a new one
	SimConnect_ClearDataDefinition(sim, simConnectWatcherID);

	// remove the key from the simVarResults
	// TODO: add rwMutex since this can be accessed in multiple threads
	simVarResults.erase(name);

	// get a new watcher ID for the new request
	simConnectWatcherID = DefinitionCounter::getDefinitionID();

	// if there are no more simvars, we return
	// if we do not return here we will attempt to request a non existing data definition and msfs crashes
	if (simVars.empty()) return;

	// TODO: add rwMutex since this can be accessed in multiple threads
	for (std::list<SimVar>::iterator it = simVars.begin(); it != simVars.end(); ++it)
	{
		SimConnect_AddToDataDefinition(sim, simConnectWatcherID, it->name.c_str(), it->unitType.c_str());
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

	char* dataArray = reinterpret_cast<char*>(&objectData->dwData);

	// TODO: add rwMutex since this can be accessed in multiple threads
	for (std::list<SimVar>::iterator it = simVars.begin(); it != simVars.end(); ++it)
	{
		switch (it->dataType)
		{
		case SIMCONNECT_DATATYPE_FLOAT32:
		{
			float value = *reinterpret_cast<float*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(float);
			break;
		}
		case SIMCONNECT_DATATYPE_FLOAT64:
		{
			double value = *reinterpret_cast<double*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(double);
			break;
		}
		case SIMCONNECT_DATATYPE_INT32:
		{
			int32_t value = *reinterpret_cast<int32_t*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(int32_t);
			break;
		}
		case SIMCONNECT_DATATYPE_INT64:
		{
			int64_t value = *reinterpret_cast<int64_t*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(int64_t);
			break;
		}
		case SIMCONNECT_DATATYPE_STRING8:
		{
			std::string value(dataArray, 8);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_STRING32:
		{
			std::string value(dataArray, 32);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_STRING64:
		{
			std::string value(dataArray, 64);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_STRING128:
		{
			std::string value(dataArray, 128);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_STRING256:
		{
			std::string value(dataArray, 256);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_STRING260:
		{
			std::string value(dataArray, 260);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_STRINGV:
		{
			// TODO: figure out if these are c-style strings. If so we can continue until we find the null terminated character and determine the string length bases on that
			throw std::runtime_error("Variable length strings are not supported yet");
			break;
		}
		case SIMCONNECT_DATATYPE_INITPOSITION:
		{
			SIMCONNECT_DATA_INITPOSITION value = *reinterpret_cast<SIMCONNECT_DATA_INITPOSITION*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_MARKERSTATE:
		{
			SIMCONNECT_DATA_MARKERSTATE value = *reinterpret_cast<SIMCONNECT_DATA_MARKERSTATE*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_WAYPOINT:
		{
			SIMCONNECT_DATA_WAYPOINT value = *reinterpret_cast<SIMCONNECT_DATA_WAYPOINT*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_LATLONALT:
		{
			SIMCONNECT_DATA_LATLONALT value = *reinterpret_cast<SIMCONNECT_DATA_LATLONALT*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		case SIMCONNECT_DATATYPE_XYZ:
		{
			SIMCONNECT_DATA_XYZ value = *reinterpret_cast<SIMCONNECT_DATA_XYZ*>(dataArray);
			// TODO: add rwMutex since this can be accessed in multiple threads
			simVarResults.insert(std::pair<std::string, std::any>(it->name, value));
			dataArray += sizeof(value);
			break;
		}
		default:
			break;
		}
	}
}