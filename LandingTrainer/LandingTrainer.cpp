// LandingTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "SimConnect.h"
#include "SimVarWatcher.h"
#include "SimVarSetter.h"

#include "AtomicList.h"

void callback(SIMCONNECT_RECV* data)
{
	printf("This is a callback being called\n");
}

static enum EVENT_ID {
	EVENT_MY_EVENT,
	EVENT_DME
};

HANDLE sim;

void CALLBACK MyInputEventCallback(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
	if (pData->dwID == SIMCONNECT_RECV_ID_ENUMERATE_INPUT_EVENTS) {
		SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS* evt = (SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS*)pData;
		SIMCONNECT_INPUT_EVENT_DESCRIPTOR* eventDescriptor = evt->rgData;

		for (int i = 0; i < evt->dwArraySize; i++)
		{
			printf("name: %s hash: %u\n", eventDescriptor[i].Name, eventDescriptor[i].Hash);
			continue;

			/*if (strcmp(eventDescriptor[i].Name, "FNX320_Input_Knob_PushPull_E_FCU_ALTITUDE_Knob") == 0)
			{
				printf("found %s\n", eventDescriptor[i].Name);
				double value = 1;
				SimConnect_SetInputEvent(sim, (UINT64)eventDescriptor[i].Hash, sizeof(double), &value);
			}*/
		}
	}
}

int main()
{
	if (SUCCEEDED(SimConnect_Open(&sim, "Request Data", NULL, 0, 0, 0)))
	{
		printf("\nConnected to sim\n");

		//SimVarSetter::setSimVar(sim, "PLANE ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64, 1000.0, sizeof(double));
		//SimVarSetter::setSimVar(sim, "PLANE ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64, 10000.0, sizeof(double));

		/*SimConnect_EnumerateInputEvents(sim, 0);

		while (true)
		{
			SimConnect_CallDispatch(sim, MyInputEventCallback, NULL);
			Sleep(1);
		}*/

		

		Sleep(100000);

		SimConnect_Close(sim);
	}
}