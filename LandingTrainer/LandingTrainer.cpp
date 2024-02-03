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

		watcher.addSimVar(SimVar{ "TITLE", "", SIMCONNECT_DATATYPE_STRINGV });
		watcher.addSimVar(SimVar{ "Plane Latitude", "degrees"});

		try
		{
			Sleep(2000);
			std::string title = watcher.get<std::string>("TITLE");
			double latitude = watcher.get<double>("Plane Latitude");
			printf("%s: %f", title.c_str(), latitude);
		}
		catch (const std::runtime_error& error)
		{
			printf("%s", error.what());
		}

		Sleep(10000000);

		SimConnect_Close(sim);
	}
}