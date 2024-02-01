// LandingTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>
#include <thread>

#include "SimConnect.h"
#include "SimVarWatcher.h"

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

		SimVarWatcher watcher(sim, SIMCONNECT_PERIOD_SECOND);
		watcher.addSimVar(
			SimVar{
				"Plane Latitude",
				"degrees"
			}
		);

		SimVarWatcher watcher2(sim, SIMCONNECT_PERIOD_SECOND);
		watcher2.addSimVar(
			SimVar{
				"Plane Latitude",
				"degrees"
			}
		);

		Sleep(5000);
		
		watcher.removeSimVar("Plane Latitude");

		Sleep(10000000);

		SimConnect_Close(sim);
	}
}