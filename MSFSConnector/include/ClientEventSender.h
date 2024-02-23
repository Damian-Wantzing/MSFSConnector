#pragma once

#include <String>

#include <Windows.h>
#include "SimConnect.h"

#include "IDCounter.h"
#include <stdexcept>
#include "SimConnectExceptions.h"

namespace MSFSConnector
{
	class ClientEventSender
	{
	public:
		/* Send a client event to the sim. These events are the standard events in MSFS.
		Examples are: "BRAKES", "FLAPS_DOWN", "FLAPS_UP". */
		inline static void sendEvent(HANDLE sim, std::string name, DWORD data = 0, SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER)
		{
			DWORD eventID = IDCounter::getID();
			
			HRESULT hr;

			try { hr = SimConnect_MapClientEventToSimEvent(sim, eventID, name.c_str()); }
			catch (const std::exception& e) { throw SimConnectUnresponsiveException("There was an error connecting to the sim"); }

			if (FAILED(hr)) throw SimConnectFailureException("there was an error mapping a client event to a sim event: " + name);

			try { hr = SimConnect_TransmitClientEvent(sim, 0, eventID, data, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY); }
			catch (const std::exception& e) { throw SimConnectUnresponsiveException("There was an error connecting to the sim"); }

			if (FAILED(hr)) throw SimConnectFailureException("there was an error transmitting a client event: " + name);
		}
	};
}