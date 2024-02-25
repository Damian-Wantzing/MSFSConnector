#include "InputEventSender.h"

#include "SimConnectExceptions.h"

namespace MSFSConnector
{
	InputEventSender::InputEventSender(HANDLE sim)
	{
		this->sim = sim;
		requestID = IDCounter::getID();
		dispatcher = Dispatcher::getInstance(sim);
		dispatcher->registerCallback([this](SIMCONNECT_RECV* data) { this->callbackHandler(data); });

		HRESULT hr;
		
		hr = SimConnect_EnumerateInputEvents(sim, requestID);
		if (hr == E_FAIL) throw SimConnectFailureException("there was an error enumerating input events");
	}

	InputEventSender::~InputEventSender()
	{
		dispatcher->deregisterCallback(callbackID);
	}

	void InputEventSender::callbackHandler(SIMCONNECT_RECV* data)
	{
		if (data->dwID != SIMCONNECT_RECV_ID_ENUMERATE_INPUT_EVENTS) return;

		SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS* evt = (SIMCONNECT_RECV_ENUMERATE_INPUT_EVENTS*)data;
		SIMCONNECT_INPUT_EVENT_DESCRIPTOR* eventDescriptor = evt->rgData;

		for (DWORD i = 0; i < evt->dwArraySize; i++)
		{
			eventHashes.set(std::pair<std::string, UINT64>(eventDescriptor[i].Name, eventDescriptor[i].Hash));
		}

		promise.set_value();
	}

	void InputEventSender::sendEvent(std::string name, std::any value, DWORD valueSize)
	{
		HRESULT hr;

		// does the event exist
		if (!hasEvent(name))
		{

			// event does not exist yet, re-request the inputevents to see if it does exist
			hr = SimConnect_EnumerateInputEvents(sim, requestID);
			if (hr == E_FAIL) throw SimConnectFailureException("there was an error enumerating input events");

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
		hr = SimConnect_SetInputEvent(sim, eventHashes.get(name), valueSize, &value);
		if (hr == E_FAIL) throw SimConnectFailureException("there was an error setting an input event");

	}

	bool InputEventSender::hasEvent(std::string name)
	{
		return eventHashes.count(name) > 0;
	}
}