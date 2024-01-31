// LandingTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>
#include <thread>

#include "SimConnect.h"
#include "Dispatcher.h"

void callback(SIMCONNECT_RECV* data)
{
	printf("This is a callback being called\n");
}

int main()
{
	HANDLE sim;

	if (SUCCEEDED(SimConnect_Open(&sim, "Request Data", NULL, 0, 0, 0)))
	{
		printf("\nConnected to sim\n");

		SimConnect_AddToDataDefinition(sim, 0, "Plane Altitude", "feet");
		SimConnect_AddToDataDefinition(sim, 0, "Plane Latitude", "degrees");
		SimConnect_AddToDataDefinition(sim, 0, "Plane Longitude", "degrees");

		SimConnect_RequestDataOnSimObject(sim, 0, 0, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);

		Dispatcher dispatcher(sim);
		dispatcher.subscribe(callback);

		Sleep(5000);

		SimConnect_Close(sim);
	}
}