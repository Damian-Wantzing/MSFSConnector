#include "InputEventSender.h"

InputEventSender::InputEventSender(HANDLE sim)
{
	this->sim = sim;
	requestID = IDCounter::getID();
	callbackID = Dispatcher::getInstance(sim).registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });
	SimConnect_EnumerateInputEvents(sim, requestID);
}

InputEventSender::~InputEventSender()
{
	Dispatcher::getInstance(sim).deregisterCallback(callbackID);
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

	promise.set_value();
}

void InputEventSender::sendEvent(std::string name, std::any value, DWORD valueSize)
{
	// does the event exist
	if (!hasEvent(name))
	{
		// event does not exist yet, re-request the inputevents to see if it does exist
		SimConnect_EnumerateInputEvents(sim, requestID);

		// wait for the callbackHandler to finish
		future.get();

		// reset the future/promise
		promise = std::promise<void>();
		future = promise.get_future();

		// does the event still not exist?
		if (!hasEvent(name))
		{
			throw std::runtime_error("event name not found: " + name);
			return;
		}
	}
	SimConnect_SetInputEvent(sim, eventHashes.get(name), valueSize, &value);
}

bool InputEventSender::hasEvent(std::string name)
{
	return eventHashes.count(name) > 0;
}