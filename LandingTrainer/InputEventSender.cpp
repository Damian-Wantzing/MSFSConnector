#include "InputEventSender.h"

InputEventSender::InputEventSender(HANDLE sim)
{
	this->sim = sim;
	Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });
	SimConnect_EnumerateInputEvents(sim, IDCounter::getID());
}

void InputEventSender::callbackHandler(SIMCONNECT_RECV* data)
{
	if (data->dwID != SIMCONNECT_RECV_ID_ENUMERATE_INPUT_EVENTS) return;

	SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS* evt = (SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS*)data;
	SIMCONNECT_INPUT_EVENT_DESCRIPTOR* eventDescriptor = evt->rgData;

	for (int i = 0; i < evt->dwArraySize; i++)
	{
		eventHashes.insert(std::pair<std::string, UINT64>(eventDescriptor[i].Name, eventDescriptor[i].Hash));
	}
}

void InputEventSender::sendEvent(std::string name, std::any value, DWORD valueSize)
{
	if (eventHashes.count(name) == 0)
	{
		throw std::runtime_error("event name not found: " + name);
		return;
	}
	SimConnect_SetInputEvent(sim, eventHashes.get(name), valueSize, &value);
}