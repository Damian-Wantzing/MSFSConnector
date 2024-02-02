// LandingTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>

#include "SimConnect.h"
#include "SimVarWatcher.h"

#include "AtomicList.h"

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

		SimVarWatcher watcher(sim, SIMCONNECT_PERIOD_ONCE);

		watcher.addSimVar(SimVar{ "Plane Latitude","degrees" });
		watcher.addSimVar(SimVar{ "Plane Longitude","degrees" });
		watcher.addSimVar(SimVar{ "Plane Altitude","feet" });

		try
		{
			Sleep(2000);
			double altitude = watcher.get<double>("Plane Altitude");
			double longitude = watcher.get<double>("Plane Longitude");
			double latitude = watcher.get<double>("Plane Latitude");
			printf("altitude: %f\nlatitude: %f\nlongitude: %f\n", altitude, latitude, longitude);
		}
		catch (const std::runtime_error& error)
		{
			printf("%s", error.what());
		}

		Sleep(10000000);

		SimConnect_Close(sim);
	}
}