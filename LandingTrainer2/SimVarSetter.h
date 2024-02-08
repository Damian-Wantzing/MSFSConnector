#pragma once

#include <any>
#include <string>

#include <Windows.h>
#include "SimConnect.h"
#include "IDCounter.h"

namespace MSFSConnector
{
	class SimVarSetter
	{
	public:
		static void setSimVar(HANDLE sim, std::string name, std::string unitType, SIMCONNECT_DATATYPE dataType, std::any data, DWORD dataSize, SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER)
		{
			DWORD requestID = IDCounter::getID();
			SimConnect_AddToDataDefinition(sim, requestID, name.c_str(), unitType.c_str(), dataType);
			SimConnect_SetDataOnSimObject(sim, requestID, objectID, 0, 0, dataSize, &data);
		}
	};
}