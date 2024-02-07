// LandingTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "SimConnect.h"
#include "SimVarWatcher.h"
#include "SimVarSetter.h"
#include "InputEventSender.h"
#include "ClientEventSender.h"
#include "Facilities.h"
#include "Airport.h"
#include "RunwayDesignator.h"

#include "AtomicList.h"

void CALLBACK myCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	if (pData->dwID != SIMCONNECT_RECV_ID_FACILITY_DATA) return;

	SIMCONNECT_RECV_FACILITY_DATA* data = (SIMCONNECT_RECV_FACILITY_DATA*)pData;

	switch (data->Type)
	{
	case SIMCONNECT_FACILITY_DATA_AIRPORT:
		printf("airport\n");
		break;
	case SIMCONNECT_FACILITY_DATA_ARRIVAL:
		char* name = reinterpret_cast<char*>(&data->Data);
		printf("arrival name: %s\n", name);
		break;
	}
}

int main()
{
	HANDLE sim;

	if (SUCCEEDED(SimConnect_Open(&sim, "Request Data", NULL, 0, 0, 0)))
	{
		printf("Connected to sim\n");

		/*SimConnect_AddToFacilityDefinition(sim, 0, "OPEN AIRPORT");
		SimConnect_AddToFacilityDefinition(sim, 0, "OPEN ARRIVAL");
		SimConnect_AddToFacilityDefinition(sim, 0, "NAME");
		SimConnect_AddToFacilityDefinition(sim, 0, "CLOSE ARRIVAL");
		SimConnect_AddToFacilityDefinition(sim, 0, "CLOSE AIRPORT");*/

		//SimConnect_RequestFacilityData(sim, 0, 0, "EHAM");

		/*while (true)
		{
			SimConnect_CallDispatch(sim, myCallback, NULL);
		}*/

		std::vector<Approach> approaches = Facilities::getApproaches(sim, "EHAM");

		SimConnect_Close(sim);
	}
}