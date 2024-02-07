#pragma once

#include <String>

#include <Windows.h>
#include "SimConnect.h"

#include "IDCounter.h"

namespace MSFSConnector
{
	class ClientEventSender
	{
	public:
		static void sendEvent(HANDLE sim, std::string name, DWORD data = 0, SIMCONNECT_OBJECT_ID objectID = SIMCONNECT_OBJECT_ID_USER)
		{
			DWORD eventID = IDCounter::getID();
			SimConnect_MapClientEventToSimEvent(sim, eventID, name.c_str());
			SimConnect_TransmitClientEvent(sim, 0, eventID, data, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
		}
	};
}