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

#include "AtomicList.h"

struct Runway
{
	double altitude;
	float length;
	float heading;
	int32_t primary;
	int32_t primaryDesignator;
	int32_t secondary;
	int32_t secondaryDesignator;
};

struct Airport
{
	char name[32];
	char icao[8];
	char region[8];
};

void CALLBACK myCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	if (pData->dwID != SIMCONNECT_RECV_ID_FACILITY_DATA) return;

	SIMCONNECT_RECV_FACILITY_DATA* data = (SIMCONNECT_RECV_FACILITY_DATA*)pData;
	switch (data->Type)
	{
	case SIMCONNECT_FACILITY_DATA_AIRPORT:
	{
		Airport* airport = (Airport*)&data->Data;
		printf("airport: %s icao: %s region: %s\n", airport->name, airport->icao, airport->region);
		break;
	}
	case SIMCONNECT_FACILITY_DATA_RUNWAY:
	{
		Runway* runway = (Runway*)&data->Data;
		std::string primaryDesig{};
		std::string secondaryDesig{};

		switch (runway->primaryDesignator)
		{
		case 0:
			primaryDesig = "";
			break;
		case 1:
			primaryDesig = "L";
			break;
		case 2:
			primaryDesig = "R";
			break;
		case 3:
			primaryDesig = "C";
			break;
		case 4:
			primaryDesig = "Water";
			break;
		case 5:
			primaryDesig = "A";
			break;
		case 6:
			primaryDesig = "B";
			break;
		case 7:
			primaryDesig = "Last";
			break;
		}

		switch (runway->secondaryDesignator)
		{
		case 0:
			secondaryDesig = "";
			break;
		case 1:
			secondaryDesig = "L";
			break;
		case 2:
			secondaryDesig = "R";
			break;
		case 3:
			secondaryDesig = "C";
			break;
		case 4:
			secondaryDesig = "Water";
			break;
		case 5:
			secondaryDesig = "A";
			break;
		case 6:
			secondaryDesig = "B";
			break;
		case 7:
			secondaryDesig = "Last";
			break;
		}

		printf("Runway altitude: %f length: %f heading: %f primary number: %d%s secondary number %d%s\n", runway->altitude, runway->length, runway->heading, runway->primary, primaryDesig.c_str(), runway->secondary, secondaryDesig.c_str());
		break;
	}
	default:
		break;
	}
}

int main()
{
	HANDLE sim;

	if (SUCCEEDED(SimConnect_Open(&sim, "Request Data", NULL, 0, 0, 0)))
	{
		printf("Connected to sim\n");

		SimConnect_AddToFacilityDefinition(sim, 0, "OPEN AIRPORT");
		SimConnect_AddToFacilityDefinition(sim, 0, "NAME");
		SimConnect_AddToFacilityDefinition(sim, 0, "ICAO");
		SimConnect_AddToFacilityDefinition(sim, 0, "REGION");

		SimConnect_AddToFacilityDefinition(sim, 0, "OPEN RUNWAY");
		SimConnect_AddToFacilityDefinition(sim, 0, "ALTITUDE");
		SimConnect_AddToFacilityDefinition(sim, 0, "LENGTH");
		SimConnect_AddToFacilityDefinition(sim, 0, "HEADING");
		SimConnect_AddToFacilityDefinition(sim, 0, "PRIMARY_NUMBER");
		SimConnect_AddToFacilityDefinition(sim, 0, "PRIMARY_DESIGNATOR");
		SimConnect_AddToFacilityDefinition(sim, 0, "SECONDARY_NUMBER");
		SimConnect_AddToFacilityDefinition(sim, 0, "SECONDARY_DESIGNATOR");

		SimConnect_AddToFacilityDefinition(sim, 0, "CLOSE RUNWAY");

		SimConnect_AddToFacilityDefinition(sim, 0, "CLOSE AIRPORT");

		SimConnect_RequestFacilityData(sim, 0, 0, "EHAM");

		while (true)
		{
			SimConnect_CallDispatch(sim, myCallback, NULL);
		}

		Sleep(10000000);

		SimConnect_Close(sim);
	}
}