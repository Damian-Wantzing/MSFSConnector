#pragma once

#include <any>
#include <string>

#include <Windows.h>
#include "SimConnect.h"
#include "IDCounter.h"
#include <stdexcept>

#include "SimConnectExceptions.h"

namespace MSFSConnector
{
	class SimVarSetter
	{
	public:
		// Set a sim var in MSFS.
		static void setSimVar(HANDLE sim, std::string name, std::string unitType, SIMCONNECT_DATATYPE dataType, std::any data, DWORD dataSize, SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER)
		{
			HRESULT hr;

			DWORD requestID = IDCounter::getID();
			try { hr = SimConnect_AddToDataDefinition(sim, requestID, name.c_str(), unitType.c_str(), dataType); }
			catch (std::exception&) { throw SimConnectUnresponsiveException("There was an error connecting to the sim"); }
			if (FAILED(hr)) throw SimConnectFailureException("there was an error adding to the SimVar data defintion");

			try { hr = SimConnect_SetDataOnSimObject(sim, requestID, objectID, 0, 0, dataSize, &data); }
			catch (std::exception&) { throw SimConnectUnresponsiveException("There was an error connecting to the sim"); }
			if (FAILED(hr)) throw SimConnectFailureException("there was an error setting a simvar on a sim object");
		}
	};
}