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


int main()
{
	HANDLE sim;

	if (SUCCEEDED(SimConnect_Open(&sim, "Request Data", NULL, 0, 0, 0)))
	{
		printf("Connected to sim\n");

		ClientEventSender::sendEvent(sim, "FLAPS_DOWN");

		Sleep(10000000);

		SimConnect_Close(sim);
	}
}