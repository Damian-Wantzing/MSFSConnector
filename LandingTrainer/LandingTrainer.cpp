// LandingTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>

#include "SimConnect.h"

static int DATA_DEFINE_ID = 0;
static int DATA_REQUEST_ID = 0;

struct AirplaneData
{
	double latitude;
	double longitude;
	double beacon;
};

int main()
{
	HANDLE sim;

	if (SUCCEEDED(SimConnect_Open(&sim, "Request Data", NULL, 0, 0, 0)))
	{
		printf("\n Connected to sim");

		SimConnect_AddToDataDefinition(sim, DATA_DEFINE_ID, "Plane Latitude", "degrees");
		SimConnect_AddToDataDefinition(sim, DATA_DEFINE_ID, "Plane Longitude", "degrees");
		SimConnect_AddToDataDefinition(sim, DATA_DEFINE_ID, "L:S_OH_EXT_LT_BEACON", "Boolean");

		SimConnect_RequestDataOnSimObject(sim, DATA_REQUEST_ID, DATA_DEFINE_ID, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME);

		while (true)
		{
			SIMCONNECT_RECV* pData;
			DWORD cbData;

			if (FAILED(SimConnect_GetNextDispatch(sim, &pData, &cbData))) continue;
			
			switch (pData->dwID)
			{
			case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
				SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

				DWORD objectID = pObjData->dwObjectID;
				AirplaneData* airplaneData = (AirplaneData*)&pObjData->dwData;
				printf("\33[2K\r");
				printf("Object id: %d Latitude: %f\ Longitude: %f Beacon: %f\r", objectID, airplaneData->latitude, airplaneData->longitude, airplaneData->beacon);
			}

			Sleep(1);
		}

		SimConnect_Close(sim);
	}
}